/* SPDX-License-Identifier: GPL-2.0-only */

#include <acpi/acpigen.h>
#include <console/console.h>
#include <device/device.h>
#include <device/pci_rom.h>
#include <option.h>
#include <soc/ramstage.h>
#include <static.h>
#include "gpio.h"

#define GPIO_GPU_RST		GPP_E22 // active low
#define GPIO_1R8VIDEO_AON_ON	GPP_E23

#define GPIO_DGFX_PWRGD		GPP_F3

#define GPIO_DISCRETE_PRESENCE	GPP_D9	// active low
#define GPIO_DGFX_VRAM_ID0	GPP_D11
#define GPIO_DGFX_VRAM_ID1	GPP_D12

void mainboard_silicon_init_params(FSP_SIL_UPD *params)
{
	// Setup GPIOs
	variant_config_gpios();
}

static void mainboard_fill_ssdt(const struct device *dev)
{
	struct rom_header *rom;
	struct device *dgpu = DEV_PTR(dgpu);

	/* Add entry for dGPU if present/enabled */
	if (!dgpu || !dgpu->enabled)
		return;

	/* ROM should be already loaded? */
	rom = dgpu->pci_vga_option_rom;
	if (!rom) {
		rom = pci_rom_probe(dgpu);
		if (!pci_rom_load(dgpu, rom))
			return;
	}

	acpigen_write_scope("\\_SB.PCI0.RP01.PEGP");
	acpigen_write_rom((void *)rom, rom->size * 512);
	acpigen_pop_len();
}

static void dgpu_detect(void)
{
	static const char * const dgfx_vram_id_str[] = { "1GB", "2GB", "4GB", "N/A" };

	int dgfx_vram_id;

	// Detect and enable dGPU
	if (gpio_get(GPIO_DISCRETE_PRESENCE) == 0) { // active low
		dgfx_vram_id = gpio_get(GPIO_DGFX_VRAM_ID0) | gpio_get(GPIO_DGFX_VRAM_ID1) << 1;
		printk(BIOS_DEBUG, "Discrete GPU present with %s VRAM\n", dgfx_vram_id_str[dgfx_vram_id]);

		// NOTE: i pulled this GPU enable sequence from thin air
		// it sometimes works but is buggy and the GPU disappears in some cases so disabling it by default.
		// also unrelated to this enable sequence the nouveau driver only works on 6.8-6.9 kernels
		if (get_uint_option("dgpu_enable", 0)) {
			printk(BIOS_DEBUG, "Enabling discrete GPU\n");
			gpio_set(GPIO_1R8VIDEO_AON_ON, 1);	// Enable GPU power rail
			while (!gpio_get(GPIO_DGFX_PWRGD))	// Wait for power good signal from GPU
				;
			gpio_set(GPIO_GPU_RST, 1);		// Release GPU from reset
		} else {
			printk(BIOS_DEBUG, "Discrete GPU will remain disabled\n");
		}

	} else {
		printk(BIOS_DEBUG, "Discrete GPU not present\n");
	}
}

static void mainboard_enable(struct device *dev)
{
	dgpu_detect();

	dev->ops->acpi_fill_ssdt = mainboard_fill_ssdt;
}

struct chip_operations mainboard_ops = {
	.enable_dev = mainboard_enable,
};
