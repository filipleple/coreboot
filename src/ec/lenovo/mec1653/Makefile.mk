## SPDX-License-Identifier: GPL-2.0-only

ifeq ($(CONFIG_EC_LENOVO_MEC1653),y)

bootblock-y += mec1653.c
ramstage-y += mec1653.c

endif
