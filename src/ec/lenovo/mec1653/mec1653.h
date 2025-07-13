/* SPDX-License-Identifier: GPL-2.0-only */

#ifndef EC_LENOVO_MEC1653_H
#define EC_LENOVO_MEC1653_H

// EC configuration base address
#define EC_CFG_PORT		0x4e

// Chip global registers
#define PNP_LDN_SELECT		0x07
# define LDN_UART		0x07
# define LDN_LPCIF		0x0c
#define EC_DEVICE_ID		0x20
#define EC_DEVICE_REV		0x21

// LPC I/F registers
#define LPCIF_SIRQ(i)		(0x40 + (i))

#define LPCIF_BAR_CFG		0x60
#define LPCIF_BAR_MAILBOX	0x64
#define LPCIF_BAR_8042		0x68
#define LPCIF_BAR_ACPI_EC0	0x6c
#define LPCIF_BAR_ACPI_EC1	0x70
#define LPCIF_BAR_ACPI_EC2	0x74
#define LPCIF_BAR_ACPI_EC3	0x78
#define LPCIF_BAR_ACPI_PM0	0x7c
#define LPCIF_BAR_UART		0x80
#define LPCIF_BAR_FAST_KYBD	0x84
#define LPCIF_BAR_EMBED_FLASH	0x88
#define LPCIF_BAR_GP_SPI	0x8c
#define LPCIF_BAR_EMI		0x90
#define LPCIF_BAR_PMH7		0x94
#define LPCIF_BAR_PORT80_DBG0	0x98
#define LPCIF_BAR_PORT80_DBG1	0x9c
#define LPCIF_BAR_RTC		0xa0

// UART registers
#define UART_ACTIVATE		0x30
#define UART_CONFIG_SELECT	0xf0

void microchip_pnp_enter_conf_state(uint16_t port);
void microchip_pnp_exit_conf_state(uint16_t port);
uint8_t pnp_read(uint16_t port, uint8_t index);
uint32_t pnp_read_le32(uint16_t port, uint8_t index);
void pnp_write(uint16_t port, uint8_t index, uint8_t value);
void pnp_write_le32(uint16_t port, uint8_t index, uint32_t value);

#define EC0_CMD		0x0066
#define EC0_DATA	0x0062
#define EC1_CMD		0x1604
#define EC1_DATA	0x1600
#define EC2_CMD		0x1634
#define EC2_DATA	0x1630
#define EC3_CMD		0x161c
#define EC3_DATA	0x1618

#define EC_OBF		(1 << 0)
#define EC_IBF		(1 << 1)

#define EC_READ		0x80
#define EC_WRITE	0x81

uint8_t ecN_read(uint16_t cmd_port, uint16_t data_port, uint8_t addr);

void ecN_write(uint16_t cmd_port, uint16_t data_port, uint8_t addr, uint8_t val);

// EC0 and EC1 mostly are useful with the READ/WRITE commands
#define ec0_read(addr) ecN_read(EC0_CMD, EC0_DATA, addr)
#define ec0_write(addr, val) ecN_write(EC0_CMD, EC0_DATA, addr, val)
#define ec1_read(addr) ecN_read(EC1_CMD, EC1_DATA, addr)
#define ec1_write(addr, val) ecN_write(EC1_CMD, EC1_DATA, addr, val)

// Read from the emulated EEPROM
uint8_t eeprom_read(uint16_t addr);

// Write to the emulated EEPROM
void eeprom_write(uint16_t addr, uint8_t val);

// Read loaded debug key mask
uint16_t debug_loaded_keys(void);

// The following location (via either EC0 or EC1) can be used to interact with the debug interface
#define EC_DEBUG_CMD 0x3d

void debug_read_key(uint8_t i, uint8_t *key);

void debug_write_key(uint8_t i, const uint8_t *key);

uint32_t debug_read_dword(uint32_t addr);

void debug_write_dword(uint32_t addr, uint32_t val);

// RW unlock key index
#define DEBUG_RW_KEY_IDX 1

void bootblock_ec_init(void);

#endif /* EC_LENOVO_MEC1653_H */
