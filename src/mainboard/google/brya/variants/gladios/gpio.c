/* SPDX-License-Identifier: GPL-2.0-or-later */

#include <baseboard/gpio.h>
#include <baseboard/variants.h>
#include <commonlib/helpers.h>
#include <soc/gpio.h>

/* Pad configuration in ramstage */
static const struct pad_config override_gpio_table[] = {
	/* A14 : USB_OC1# ==> NC */
	PAD_NC_LOCK(GPP_A14, NONE, LOCK_CONFIG),
	/* A15 : USB_OC2# ==> NC */
	PAD_NC_LOCK(GPP_A15, NONE, LOCK_CONFIG),
	/* A18 : DDSP_HPDB ==> HDMIB_HPD */
	PAD_CFG_NF(GPP_A18, NONE, DEEP, NF1),
	/* A19 : DDSP_HPD1 ==> NC */
	PAD_NC_LOCK(GPP_A19, NONE, LOCK_CONFIG),
	/* A20 : DDSP_HPD2 ==> NC */
	PAD_NC_LOCK(GPP_A20, NONE, LOCK_CONFIG),
	/* A21 : DDPC_CTRCLK ==> EN_PP3300_EMMC */
	PAD_CFG_GPO(GPP_A21, 1, DEEP),
	/* A22 : DDPC_CTRLDATA ==> NC */
	PAD_NC_LOCK(GPP_A22, NONE, LOCK_CONFIG),

	/* B2  : VRALERT# ==> M2_SSD_PLA_L */
	PAD_NC(GPP_B2, NONE),
	/* B3  : PROC_GP2 ==> EMMC_PERST_L */
	PAD_CFG_GPO_LOCK(GPP_B3, 1, LOCK_CONFIG),
	/* B7  : ISH_12C1_SDA ==> PCH_I2C_MISCB_SDA */
	PAD_CFG_NF_LOCK(GPP_B7, NONE, NF2, LOCK_CONFIG),
	/* B8  : ISH_I2C1_SCL ==> PCH_I2C_MISCB_SCL */
	PAD_CFG_NF_LOCK(GPP_B8, NONE, NF2, LOCK_CONFIG),

	/* D0  : ISH_GP0 ==> NC */
	PAD_NC_LOCK(GPP_D0, NONE, LOCK_CONFIG),
	/* D1  : ISH_GP1 ==> NC */
	PAD_NC_LOCK(GPP_D1, NONE, LOCK_CONFIG),
	/* D2  : ISH_GP2 ==> NC */
	PAD_NC_LOCK(GPP_D2, NONE, LOCK_CONFIG),
	/* D3  : ISH_GP3 ==> NC */
	PAD_NC_LOCK(GPP_D3, NONE, LOCK_CONFIG),
	/* D9  : ISH_SPI_CS# ==> NC */
	PAD_NC_LOCK(GPP_D9, NONE, LOCK_CONFIG),
	/* D10 : ISH_SPI_CLK ==> GPI */
	PAD_CFG_GPI_LOCK(GPP_D10, NONE, LOCK_CONFIG),
	/* D17 : UART1_RXD ==> NC */
	PAD_NC_LOCK(GPP_D17, NONE, LOCK_CONFIG),

	/* E4  : SATA_DEVSLP0 ==> NC */
	PAD_NC(GPP_E4, NONE),
	/* E5  : SATA_DEVSLP1 ==> USB_A0_RT_RST_ODL */
	PAD_CFG_GPO(GPP_E5, 1, DEEP),
	/* E14 : DDSP_HPDA ==> HDMIA_HPD */
	PAD_CFG_NF(GPP_E14, NONE, DEEP, NF1),
	/* E18 : DDP1_CTRLCLK ==> NC */
	PAD_NC(GPP_E18, NONE),
	/* E20 : DDP2_CTRLCLK ==> DDIA_HDMI_CTRLCLK */
	PAD_CFG_NF(GPP_E20, NONE, DEEP, NF1),
	/* E21 : DDP2_CTRLDATA ==> DDIA_HDMI_CTRLDATA */
	PAD_CFG_NF(GPP_E21, NONE, DEEP, NF1),

	/* F11 : THC1_SPI2_CLK ==> NC */
	PAD_NC_LOCK(GPP_F11, NONE, LOCK_CONFIG),
	/* F12 : GSXDOUT ==> NC */
	PAD_NC_LOCK(GPP_F12, NONE, LOCK_CONFIG),
	/* F13 : GSXDOUT ==> NC */
	PAD_NC_LOCK(GPP_F13, NONE, LOCK_CONFIG),
	/* F15 : GSXSRESET# ==> NC */
	PAD_NC_LOCK(GPP_F15, NONE, LOCK_CONFIG),
	/* F16 : GSXCLK ==> NC */
	PAD_NC_LOCK(GPP_F16, NONE, LOCK_CONFIG),

	/* H19 : SRCCLKREQ4# ==> CLKREQ_4 */
	PAD_CFG_NF(GPP_H19, NONE, DEEP, NF1),
	/* H21 : IMGCLKOUT2 ==>  EMMC_PE_WAKE_ODL */
	PAD_CFG_GPO(GPP_H21, 1, DEEP),

	/* R4 : HDA_RST# ==> NC */
	PAD_NC(GPP_R4, NONE),
	/* R5 : HDA_SDI1 ==> NC */
	PAD_NC(GPP_R5, NONE),
	/* R6 : I2S2_TXD ==> NC */
	PAD_NC(GPP_R6, NONE),
	/* R7 : I2S2_RXD ==> NC */
	PAD_NC(GPP_R7, NONE),
};

/* Early pad configuration in bootblock */
static const struct pad_config early_gpio_table[] = {
	/* A13 : PMC_I2C_SCL ==> GSC_PCH_INT_ODL */
	PAD_CFG_GPI_APIC(GPP_A13, NONE, PLTRST, LEVEL, INVERT),
	/* A21 : DDPC_CTRCLK ==> EN_PP3300_EMMC */
	PAD_CFG_GPO(GPP_A21, 1, DEEP),
	/* B4  : PROC_GP3 ==> SSD_PERST_L */
	PAD_CFG_GPO(GPP_B4, 0, DEEP),
	/* E15 : RSVD_TP ==> PCH_WP_OD */
	PAD_CFG_GPI_GPIO_DRIVER(GPP_E15, NONE, DEEP),
	/* F14 : GSXDIN ==> EN_PP3300_SSD */
	PAD_CFG_GPO(GPP_F14, 1, DEEP),
	/* F18 : THC1_SPI2_INT# ==> EC_IN_RW_OD */
	PAD_CFG_GPI(GPP_F18, NONE, DEEP),
	/* H6  : I2C1_SDA ==> PCH_I2C_TPM_SDA */
	PAD_CFG_NF(GPP_H6, NONE, DEEP, NF1),
	/* H7  : I2C1_SCL ==> PCH_I2C_TPM_SCL */
	PAD_CFG_NF(GPP_H7, NONE, DEEP, NF1),
	/* H10 : UART0_RXD ==> UART_PCH_RX_DBG_TX */
	PAD_CFG_NF(GPP_H10, NONE, DEEP, NF2),
	/* H11 : UART0_TXD ==> UART_PCH_TX_DBG_RX */
	PAD_CFG_NF(GPP_H11, NONE, DEEP, NF2),
	/* H13 : I2C7_SCL ==> EN_PP3300_SD */
	PAD_CFG_GPO(GPP_H13, 1, DEEP),

	/* CPU PCIe VGPIO for PEG60 */
	PAD_CFG_NF_VWEN(GPP_vGPIO_PCIE_48, NONE, PLTRST, NF1),
	PAD_CFG_NF_VWEN(GPP_vGPIO_PCIE_49, NONE, PLTRST, NF1),
	PAD_CFG_NF_VWEN(GPP_vGPIO_PCIE_50, NONE, PLTRST, NF1),
	PAD_CFG_NF_VWEN(GPP_vGPIO_PCIE_51, NONE, PLTRST, NF1),
	PAD_CFG_NF_VWEN(GPP_vGPIO_PCIE_52, NONE, PLTRST, NF1),
	PAD_CFG_NF_VWEN(GPP_vGPIO_PCIE_53, NONE, PLTRST, NF1),
	PAD_CFG_NF_VWEN(GPP_vGPIO_PCIE_54, NONE, PLTRST, NF1),
	PAD_CFG_NF_VWEN(GPP_vGPIO_PCIE_55, NONE, PLTRST, NF1),
	PAD_CFG_NF_VWEN(GPP_vGPIO_PCIE_56, NONE, PLTRST, NF1),
	PAD_CFG_NF_VWEN(GPP_vGPIO_PCIE_57, NONE, PLTRST, NF1),
	PAD_CFG_NF_VWEN(GPP_vGPIO_PCIE_58, NONE, PLTRST, NF1),
	PAD_CFG_NF_VWEN(GPP_vGPIO_PCIE_59, NONE, PLTRST, NF1),
	PAD_CFG_NF_VWEN(GPP_vGPIO_PCIE_60, NONE, PLTRST, NF1),
	PAD_CFG_NF_VWEN(GPP_vGPIO_PCIE_61, NONE, PLTRST, NF1),
	PAD_CFG_NF_VWEN(GPP_vGPIO_PCIE_62, NONE, PLTRST, NF1),
	PAD_CFG_NF_VWEN(GPP_vGPIO_PCIE_63, NONE, PLTRST, NF1),
	PAD_CFG_NF_VWEN(GPP_vGPIO_PCIE_76, NONE, PLTRST, NF1),
	PAD_CFG_NF_VWEN(GPP_vGPIO_PCIE_77, NONE, PLTRST, NF1),
	PAD_CFG_NF_VWEN(GPP_vGPIO_PCIE_78, NONE, PLTRST, NF1),
	PAD_CFG_NF_VWEN(GPP_vGPIO_PCIE_79, NONE, PLTRST, NF1),
};

static const struct pad_config romstage_gpio_table[] = {
	/* B4  : PROC_GP3 ==> SSD_PERST_L */
	PAD_CFG_GPO(GPP_B4, 1, DEEP),
};

const struct pad_config *variant_gpio_override_table(size_t *num)
{
	*num = ARRAY_SIZE(override_gpio_table);
	return override_gpio_table;
}

const struct pad_config *variant_early_gpio_table(size_t *num)
{
	*num = ARRAY_SIZE(early_gpio_table);
	return early_gpio_table;
}

const struct pad_config *variant_romstage_gpio_table(size_t *num)
{
	*num = ARRAY_SIZE(romstage_gpio_table);
	return romstage_gpio_table;
}
