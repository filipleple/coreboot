## SPDX-License-Identifier: GPL-2.0-only

bootblock-y += bootblock.c

romstage-y += variants/$(VARIANT_DIR)/memory_init_params.c

ramstage-$(CONFIG_DRIVERS_OPTION_CFR) += cfr.c
ramstage-y += ramstage.c
ramstage-y += variants/$(VARIANT_DIR)/gpio.c variants/$(VARIANT_DIR)/hda_verb.c
ramstage-$(CONFIG_MAINBOARD_USE_LIBGFXINIT) += variants/$(VARIANT_DIR)/gma-mainboard.ads

# Add files spd_0.bin to spd_20.bin to the cbfs image
ifeq ($(CONFIG_BOARD_LENOVO_T480S),y)
SPD_BINS := $(shell seq 0 20)
define SPD_template
cbfs-files-y += spd_$(1).bin
spd_$(1).bin-file := variants/$(VARIANT_DIR)/spd/spd_$(1).bin
spd_$(1).bin-type := raw
endef
$(foreach n,$(SPD_BINS),$(eval $(call SPD_template,$(n))))
endif

# Add files spd_0.bin to spd_9.bin to the cbfs image
ifeq ($(CONFIG_BOARD_LENOVO_T460S),y)
SPD_BINS := $(shell seq 0 9)
define SPD_template
cbfs-files-y += spd_$(1).bin
spd_$(1).bin-file := variants/t480s/spd/spd_$(1).bin
spd_$(1).bin-type := raw
endef
$(foreach n,$(SPD_BINS),$(eval $(call SPD_template,$(n))))
endif
