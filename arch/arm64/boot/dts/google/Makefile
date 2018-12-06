ifeq ($(CONFIG_BOARD_B1C1),y)
ifeq ($(CONFIG_BUILD_ARM64_DT_OVERLAY),y)

# V2.1 SDM845
dtbo-$(CONFIG_BOARD_B1C1) += \
	sdm845-b1-dvt1.1.dtbo \
	sdm845-c1-dvt1.1.dtbo

sdm845-b1-dvt1.1.dtbo-base		:= ../qcom/sdm845-v2.1.dtb
sdm845-c1-dvt1.1.dtbo-base		:= ../qcom/sdm845-v2.1.dtb

endif
endif #CONFIG_BOARD_B1C1

ifeq ($(CONFIG_BOARD_BONITO),y)
ifeq ($(CONFIG_BUILD_ARM64_DT_OVERLAY),y)

dtbo-$(CONFIG_BOARD_BONITO) += \
	sdm670-b4-dev.dtbo \
	sdm670-s4-dev.dtbo \
	sdm670-b4-proto.dtbo \
	sdm670-s4-proto.dtbo \
	sdm670-b4-evt.dtbo \
	sdm670-s4-evt.dtbo \
	sdm670-b4-dvt.dtbo \
	sdm670-s4-dvt.dtbo \
	sdm670-b4-pvt.dtbo \
	sdm670-s4-pvt.dtbo

sdm670-b4-dev.dtbo-base		:= ../qcom/sdm670.dtb
sdm670-s4-dev.dtbo-base		:= ../qcom/sdm670.dtb
sdm670-b4-proto.dtbo-base		:= ../qcom/sdm670.dtb
sdm670-s4-proto.dtbo-base		:= ../qcom/sdm670.dtb
sdm670-b4-evt.dtbo-base		:= ../qcom/sdm670.dtb
sdm670-s4-evt.dtbo-base		:= ../qcom/sdm670.dtb
sdm670-b4-dvt.dtbo-base		:= ../qcom/sdm670.dtb
sdm670-s4-dvt.dtbo-base		:= ../qcom/sdm670.dtb
sdm670-b4-pvt.dtbo-base		:= ../qcom/sdm670.dtb
sdm670-s4-pvt.dtbo-base		:= ../qcom/sdm670.dtb

endif
endif #CONFIG_BOARD_BONITO

always		:= $(dtb-y)
subdir-y	:= $(dts-dirs)
clean-files	:= *.dtb
