/* SPDX-License-Identifier: GPL-2.0-only */

#include <acpi/acpigen.h>
#include <arch/smp/mpspec.h>
#include <arch/vga.h>
#include <assert.h>
#include <cbmem.h>
#include <cpu/intel/turbo.h>
#include <device/mmio.h>
#include <device/pci.h>
#include <intelblocks/acpi.h>
#include <intelblocks/cpulib.h>
#include <intelblocks/pmclib.h>
#include <soc/acpi.h>
#include <soc/iomap.h>
#include <soc/msr.h>
#include <soc/pci_devs.h>
#include <soc/pm.h>
#include <soc/soc_util.h>
#include <soc/util.h>
#include <hob_iiouds.h>

int soc_madt_sci_irq_polarity(int sci)
{
	if (sci >= 20)
		return MP_IRQ_POLARITY_LOW;
	else
		return MP_IRQ_POLARITY_HIGH;
}

uint32_t soc_read_sci_irq_select(void)
{
	/* PMC controller is hidden - hence PWRMBASE can't be accessbile using PCI cfg space */
	uintptr_t pmc_bar = PCH_PWRM_BASE_ADDRESS;
	return read32((void *)pmc_bar + PMC_ACPI_CNT);
}

void soc_fill_fadt(acpi_fadt_t *fadt)
{
	const uint16_t pmbase = ACPI_BASE_ADDRESS;

	fadt->FADT_MinorVersion = 1;
	fadt->pm_tmr_blk = pmbase + PM1_TMR;
	fadt->pm_tmr_len = 4;
	/* Clear flags set by common/block/acpi/acpi.c acpi_fill_fadt() */
	fadt->flags &= ~ACPI_FADT_SEALED_CASE;

	fadt->preferred_pm_profile = PM_ENTERPRISE_SERVER;
	fadt->pm2_cnt_blk = pmbase + PM2_CNT;
	fadt->pm2_cnt_len = 1;

	/* PM Extended Registers */
	fill_fadt_extended_pm_io(fadt);
}

/*
 * Add a DSDT ACPI Name field for STACK enable setting.
 *  This is retrieved by the device _STA defined in iiostack.asl
 */
static void create_dsdt_stack_sta(uint8_t socket, uint8_t stack, bool stack_enabled)
{
	char stack_sta[16];
	snprintf(stack_sta, sizeof(stack_sta), "ST%d%X", socket, stack);

	if (!stack_enabled)
		acpigen_write_name_integer(stack_sta, ACPI_STATUS_DEVICE_ALL_OFF);
	else
		acpigen_write_name_integer(stack_sta, ACPI_STATUS_DEVICE_ALL_ON);
}

void uncore_fill_ssdt(const struct device *device)
{
	bool stack_enabled;

	/* Only add RTxx entries once. */
	if (device->upstream->secondary != 0)
		return;

	/*
	   Write stack scope - this needs to match RP ACPI scopes.
	   Stacks 0 (TYPE_UBOX_IIO)
		Scope: PC<socket><stack>, ResourceTemplate: P0RS
	   Stacks 1 .. 5 (TYPE_UBOX_IIO)
		Scope: PC<socket><stack> & CX<socket><stack>, ResourceTemplate: RBRS
	   Stacks 8 .. B (TYPE_DINO)
		Scope: DI<socket><stack> for DINO, ResourceTemplate: RBRS
		Scope: CP<socket><stack> for CPM (i.e., QAT), ResourceTemplate: RBRS
		Scope: HQ<socket><stack> for HQM (i.e., DLB), ResourceTemplate: RBRS
	   Stacks D .. E (TYPE_UBOX)
		Scope: UC<socket><0..1> for UBOX[1-2], ResourceTemplate: UNRS
	*/

	printk(BIOS_DEBUG, "%s device: %s\n", __func__, dev_path(device));

	/* The _CSR generation must match SPR iiostack.asl. */
	const IIO_UDS *hob = get_iio_uds();
	/* Iterate over CONFIG_MAX_SOCKET to keep ASL templates and DSDT injection in sync */
	for (uint8_t socket = 0; socket < CONFIG_MAX_SOCKET; ++socket) {
		for (int stack = 0; stack < MAX_LOGIC_IIO_STACK; ++stack) {
			const STACK_RES *ri =
				&hob->PlatformData.IIO_resource[socket].StackRes[stack];

			stack_enabled = soc_cpu_is_enabled(socket) &&
					ri->Personality < TYPE_RESERVED;

			printk(BIOS_DEBUG, "%s processing socket: %d, stack: %d, type: %d\n",
			       __func__, socket, stack, ri->Personality);

			if (stack <= IioStack5) { // TYPE_UBOX_IIO
				create_dsdt_stack_sta(socket, stack, stack_enabled);
			} else if (stack >= IioStack8 && stack <= IioStack11) { // TYPE_DINO
				create_dsdt_stack_sta(socket, stack, stack_enabled);
			} else if (stack == IioStack13) { // TYPE_UBOX
				create_dsdt_stack_sta(socket, stack, stack_enabled);
			}
		}
	}
}

/* TODO: See if we can use the common generate_p_state_entries */
void soc_power_states_generation(int core, int cores_per_package)
{
	int ratio_min, ratio_max, ratio_turbo, ratio_step;
	int coord_type, power_max, power_unit, num_entries;
	int ratio, power, clock, clock_max;
	msr_t msr;

	/* Determine P-state coordination type from MISC_PWR_MGMT[0] */
	msr = rdmsr(MSR_MISC_PWR_MGMT);
	if (msr.lo & MISC_PWR_MGMT_EIST_HW_DIS)
		coord_type = SW_ANY;
	else
		coord_type = HW_ALL;

	/* Get bus ratio limits and calculate clock speeds */
	msr = rdmsr(MSR_PLATFORM_INFO);
	ratio_min = (msr.hi >> (40 - 32)) & 0xff; /* Max Efficiency Ratio */

	/* Determine if this CPU has configurable TDP */
	if (cpu_config_tdp_levels()) {
		/* Set max ratio to nominal TDP ratio */
		msr = rdmsr(MSR_CONFIG_TDP_NOMINAL);
		ratio_max = msr.lo & 0xff;
	} else {
		/* Max Non-Turbo Ratio */
		ratio_max = (msr.lo >> 8) & 0xff;
	}
	clock_max = ratio_max * CONFIG_CPU_BCLK_MHZ;

	/* Calculate CPU TDP in mW */
	msr = rdmsr(MSR_PKG_POWER_SKU_UNIT);
	power_unit = 2 << ((msr.lo & 0xf) - 1);
	msr = rdmsr(MSR_PKG_POWER_SKU);
	power_max = ((msr.lo & 0x7fff) / power_unit) * 1000;

	/* Write _PCT indicating use of FFixedHW */
	acpigen_write_empty_PCT();

	/* Write _PPC with no limit on supported P-state */
	acpigen_write_PPC_NVS();

	/* Write PSD indicating configured coordination type */
	acpigen_write_PSD_package(core, 1, coord_type);

	/* Add P-state entries in _PSS table */
	acpigen_write_name("_PSS");

	/* Determine ratio points */
	ratio_step = PSS_RATIO_STEP;
	num_entries = ((ratio_max - ratio_min) / ratio_step) + 1;
	if (num_entries > PSS_MAX_ENTRIES) {
		ratio_step += 1;
		num_entries = ((ratio_max - ratio_min) / ratio_step) + 1;
	}

	/* P[T] is Turbo state if enabled */
	if (get_turbo_state() == TURBO_ENABLED) {
		/* _PSS package count including Turbo */
		acpigen_write_package(num_entries + 2);

		msr = rdmsr(MSR_TURBO_RATIO_LIMIT);
		ratio_turbo = msr.lo & 0xff;

		/* Add entry for Turbo ratio */
		acpigen_write_PSS_package(clock_max + 1,	  /* MHz */
					  power_max,		  /* mW */
					  PSS_LATENCY_TRANSITION, /* lat1 */
					  PSS_LATENCY_BUSMASTER,  /* lat2 */
					  ratio_turbo << 8,	  /* control */
					  ratio_turbo << 8);	  /* status */
	} else {
		/* _PSS package count without Turbo */
		acpigen_write_package(num_entries + 1);
	}

	/* First regular entry is max non-turbo ratio */
	acpigen_write_PSS_package(clock_max,		  /* MHz */
				  power_max,		  /* mW */
				  PSS_LATENCY_TRANSITION, /* lat1 */
				  PSS_LATENCY_BUSMASTER,  /* lat2 */
				  ratio_max << 8,	  /* control */
				  ratio_max << 8);	  /* status */

	/* Generate the remaining entries */
	for (ratio = ratio_min + ((num_entries - 1) * ratio_step); ratio >= ratio_min;
	     ratio -= ratio_step) {

		/* Calculate power at this ratio */
		power = common_calculate_power_ratio(power_max, ratio_max, ratio);
		clock = ratio * CONFIG_CPU_BCLK_MHZ;
		// clock = 1;
		acpigen_write_PSS_package(clock,		  /* MHz */
					  power,		  /* mW */
					  PSS_LATENCY_TRANSITION, /* lat1 */
					  PSS_LATENCY_BUSMASTER,  /* lat2 */
					  ratio << 8,		  /* control */
					  ratio << 8);		  /* status */
	}

	/* Fix package length */
	acpigen_pop_len();
}

unsigned long xeonsp_acpi_create_madt_lapics(unsigned long current)
{
	struct device *cpu;
	uint8_t num_cpus = 0;

	for (cpu = all_devices; cpu; cpu = cpu->next) {
		if ((cpu->path.type != DEVICE_PATH_APIC)
		    || (cpu->upstream->dev->path.type != DEVICE_PATH_CPU_CLUSTER)) {
			continue;
		}
		if (!cpu->enabled)
			continue;
		current = acpi_create_madt_one_lapic(current, num_cpus, cpu->path.apic.apic_id);
		num_cpus++;
	}

	return current;
}

unsigned long acpi_fill_cedt(unsigned long current)
{
	const IIO_UDS *hob = get_iio_uds();
	union uid {
		uint32_t data;
		struct {
			uint8_t byte0;
			uint8_t byte1;
			uint8_t byte2;
			uint8_t byte3;
		};
	} cxl_uid;
	u32 cxl_ver;
	u64 base;

	cxl_uid.byte0 = 'C';
	cxl_uid.byte1 = 'X';
	/* Loop through all sockets and stacks, add CHBS for each CXL IIO stack */
	for (uint8_t socket = 0, iio = 0; iio < hob->PlatformData.numofIIO; ++socket) {
		if (!soc_cpu_is_enabled(socket))
			continue;
		iio++;
		for (int x = 0; x < MAX_LOGIC_IIO_STACK; ++x) {
			const STACK_RES *ri;
			ri = &hob->PlatformData.IIO_resource[socket].StackRes[x];
			if (!is_iio_cxl_stack_res(ri))
				continue;
			/* uid needs to match with ACPI CXL device ID, eg. acpi/iiostack.asl */
			cxl_uid.byte2 = socket + '0';
			cxl_uid.byte3 = x + '0';
			cxl_ver = ACPI_CEDT_CHBS_CXL_VER_1_1;
			base = ri->Mmio32Base; /* DP RCRB base */
			current += acpi_create_cedt_chbs((acpi_cedt_chbs_t *)current,
							 cxl_uid.data, cxl_ver, base);
		}
	}

	return current;
}
