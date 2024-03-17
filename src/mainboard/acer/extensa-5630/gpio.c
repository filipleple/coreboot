/* SPDX-License-Identifier: GPL-2.0-only */

#include <southbridge/intel/common/gpio.h>

static const struct pch_gpio_set1 pch_gpio_set1_mode = {
	.gpio0  = GPIO_MODE_NATIVE,
	.gpio1  = GPIO_MODE_GPIO,
	.gpio2  = GPIO_MODE_NATIVE,
	.gpio3  = GPIO_MODE_NATIVE,
	.gpio4  = GPIO_MODE_NATIVE,
	.gpio5  = GPIO_MODE_NATIVE,
	.gpio6  = GPIO_MODE_GPIO,
	.gpio7  = GPIO_MODE_GPIO,
	.gpio8  = GPIO_MODE_GPIO,
	.gpio9  = GPIO_MODE_GPIO,
	.gpio10 = GPIO_MODE_GPIO,
	.gpio11 = GPIO_MODE_NATIVE,
	.gpio12 = GPIO_MODE_GPIO,
	.gpio13 = GPIO_MODE_GPIO,
	.gpio14 = GPIO_MODE_GPIO,
	.gpio15 = GPIO_MODE_NATIVE,
	.gpio16 = GPIO_MODE_NATIVE,
	.gpio17 = GPIO_MODE_GPIO,
	.gpio18 = GPIO_MODE_GPIO,
	.gpio19 = GPIO_MODE_GPIO,
	.gpio20 = GPIO_MODE_GPIO,
	.gpio21 = GPIO_MODE_GPIO,
	.gpio22 = GPIO_MODE_GPIO,
	.gpio23 = GPIO_MODE_GPIO,
	.gpio24 = GPIO_MODE_GPIO,
	.gpio25 = GPIO_MODE_NATIVE,
	.gpio26 = GPIO_MODE_NATIVE,
	.gpio27 = GPIO_MODE_GPIO,
	.gpio28 = GPIO_MODE_GPIO,
	.gpio29 = GPIO_MODE_NATIVE,
	.gpio30 = GPIO_MODE_NATIVE,
	.gpio31 = GPIO_MODE_NATIVE,
};

static const struct pch_gpio_set1 pch_gpio_set1_direction = {
	.gpio1  = GPIO_DIR_OUTPUT,
	.gpio6  = GPIO_DIR_INPUT,
	.gpio7  = GPIO_DIR_INPUT,
	.gpio8  = GPIO_DIR_INPUT,
	.gpio9  = GPIO_DIR_OUTPUT,
	.gpio10 = GPIO_DIR_OUTPUT,
	.gpio12 = GPIO_DIR_OUTPUT,
	.gpio13 = GPIO_DIR_OUTPUT,
	.gpio14 = GPIO_DIR_OUTPUT,
	.gpio17 = GPIO_DIR_INPUT,
	.gpio18 = GPIO_DIR_OUTPUT,
	.gpio19 = GPIO_DIR_OUTPUT,
	.gpio20 = GPIO_DIR_OUTPUT,
	.gpio21 = GPIO_DIR_OUTPUT,
	.gpio22 = GPIO_DIR_INPUT,
	.gpio23 = GPIO_DIR_OUTPUT,
	.gpio24 = GPIO_DIR_OUTPUT,
	.gpio27 = GPIO_DIR_OUTPUT,
	.gpio28 = GPIO_DIR_OUTPUT,
};

static const struct pch_gpio_set1 pch_gpio_set1_level = {
	.gpio1  = GPIO_LEVEL_LOW,
	.gpio9  = GPIO_LEVEL_LOW,
	.gpio10 = GPIO_LEVEL_LOW,
	.gpio12 = GPIO_LEVEL_LOW,
	.gpio13 = GPIO_LEVEL_LOW,
	.gpio14 = GPIO_LEVEL_LOW,
	.gpio18 = GPIO_LEVEL_LOW,
	.gpio19 = GPIO_LEVEL_LOW,
	.gpio20 = GPIO_LEVEL_HIGH,
	.gpio21 = GPIO_LEVEL_LOW,
	.gpio23 = GPIO_LEVEL_LOW,
	.gpio24 = GPIO_LEVEL_HIGH,
	.gpio27 = GPIO_LEVEL_HIGH,
	.gpio28 = GPIO_LEVEL_HIGH,
};

static const struct pch_gpio_set1 pch_gpio_set1_blink = {
};

static const struct pch_gpio_set1 pch_gpio_set1_invert = {
	.gpio7  = GPIO_INVERT,
	.gpio8  = GPIO_INVERT,
};

static const struct pch_gpio_set2 pch_gpio_set2_mode = {
	.gpio32 = GPIO_MODE_NATIVE,
	.gpio33 = GPIO_MODE_GPIO,
	.gpio34 = GPIO_MODE_GPIO,
	.gpio35 = GPIO_MODE_NATIVE,
	.gpio36 = GPIO_MODE_GPIO,
	.gpio37 = GPIO_MODE_GPIO,
	.gpio38 = GPIO_MODE_GPIO,
	.gpio39 = GPIO_MODE_GPIO,
	.gpio40 = GPIO_MODE_NATIVE,
	.gpio41 = GPIO_MODE_NATIVE,
	.gpio42 = GPIO_MODE_NATIVE,
	.gpio43 = GPIO_MODE_NATIVE,
	.gpio44 = GPIO_MODE_NATIVE,
	.gpio45 = GPIO_MODE_NATIVE,
	.gpio46 = GPIO_MODE_NATIVE,
	.gpio47 = GPIO_MODE_NATIVE,
	.gpio48 = GPIO_MODE_GPIO,
	.gpio49 = GPIO_MODE_GPIO,
	.gpio50 = GPIO_MODE_NATIVE,
	.gpio51 = GPIO_MODE_NATIVE,
	.gpio52 = GPIO_MODE_NATIVE,
	.gpio53 = GPIO_MODE_NATIVE,
	.gpio54 = GPIO_MODE_NATIVE,
	.gpio55 = GPIO_MODE_NATIVE,
	.gpio56 = GPIO_MODE_GPIO,
	.gpio57 = GPIO_MODE_GPIO,
	.gpio58 = GPIO_MODE_NATIVE,
	.gpio59 = GPIO_MODE_NATIVE,
	.gpio60 = GPIO_MODE_NATIVE,
	.gpio61 = GPIO_MODE_NATIVE,
	.gpio62 = GPIO_MODE_NATIVE,
	.gpio63 = GPIO_MODE_NATIVE,
};

static const struct pch_gpio_set2 pch_gpio_set2_direction = {
	.gpio33 = GPIO_DIR_OUTPUT,
	.gpio34 = GPIO_DIR_OUTPUT,
	.gpio36 = GPIO_DIR_OUTPUT,
	.gpio37 = GPIO_DIR_OUTPUT,
	.gpio38 = GPIO_DIR_OUTPUT,
	.gpio39 = GPIO_DIR_OUTPUT,
	.gpio48 = GPIO_DIR_INPUT,
	.gpio49 = GPIO_DIR_OUTPUT,
	.gpio56 = GPIO_DIR_INPUT,
	.gpio57 = GPIO_DIR_OUTPUT,
};

static const struct pch_gpio_set2 pch_gpio_set2_level = {
	.gpio33 = GPIO_LEVEL_LOW,
	.gpio34 = GPIO_LEVEL_LOW,
	.gpio36 = GPIO_LEVEL_LOW,
	.gpio37 = GPIO_LEVEL_LOW,
	.gpio38 = GPIO_LEVEL_LOW,
	.gpio39 = GPIO_LEVEL_LOW,
	.gpio49 = GPIO_LEVEL_LOW,
	.gpio57 = GPIO_LEVEL_HIGH,
};

/* ICH9 does not have set 3
 
static const struct pch_gpio_set3 pch_gpio_set3_mode = {
	.gpio64 = GPIO_MODE_NATIVE,
	.gpio65 = GPIO_MODE_NATIVE,
	.gpio66 = GPIO_MODE_NATIVE,
	.gpio67 = GPIO_MODE_NATIVE,
	.gpio68 = GPIO_MODE_NATIVE,
	.gpio69 = GPIO_MODE_NATIVE,
	.gpio70 = GPIO_MODE_NATIVE,
	.gpio71 = GPIO_MODE_NATIVE,
	.gpio72 = GPIO_MODE_NATIVE,
	.gpio73 = GPIO_MODE_NATIVE,
	.gpio74 = GPIO_MODE_NATIVE,
	.gpio75 = GPIO_MODE_NATIVE,
	.gpio76 = GPIO_MODE_NATIVE,
	.gpio77 = GPIO_MODE_NATIVE,
	.gpio78 = GPIO_MODE_NATIVE,
	.gpio79 = GPIO_MODE_NATIVE,
	.gpio80 = GPIO_MODE_NATIVE,
	.gpio81 = GPIO_MODE_NATIVE,
	.gpio82 = GPIO_MODE_NATIVE,
	.gpio83 = GPIO_MODE_NATIVE,
	.gpio84 = GPIO_MODE_NATIVE,
	.gpio85 = GPIO_MODE_NATIVE,
	.gpio86 = GPIO_MODE_NATIVE,
	.gpio87 = GPIO_MODE_NATIVE,
	.gpio88 = GPIO_MODE_NATIVE,
	.gpio89 = GPIO_MODE_NATIVE,
	.gpio90 = GPIO_MODE_NATIVE,
	.gpio91 = GPIO_MODE_NATIVE,
	.gpio92 = GPIO_MODE_NATIVE,
	.gpio93 = GPIO_MODE_NATIVE,
	.gpio94 = GPIO_MODE_NATIVE,
	.gpio95 = GPIO_MODE_NATIVE,
};

static const struct pch_gpio_set3 pch_gpio_set3_direction = {
};

static const struct pch_gpio_set3 pch_gpio_set3_level = {
};

*/

const struct pch_gpio_map mainboard_gpio_map = {
	.set1 = {
		.mode      = &pch_gpio_set1_mode,
		.direction = &pch_gpio_set1_direction,
		.level     = &pch_gpio_set1_level,
		.blink     = &pch_gpio_set1_blink,
		.invert    = &pch_gpio_set1_invert,
	},
	.set2 = {
		.mode      = &pch_gpio_set2_mode,
		.direction = &pch_gpio_set2_direction,
		.level     = &pch_gpio_set2_level,
	}
/*
	.set3 = {
		.mode      = &pch_gpio_set3_mode,
		.direction = &pch_gpio_set3_direction,
		.level     = &pch_gpio_set3_level,
	},
*/

};
