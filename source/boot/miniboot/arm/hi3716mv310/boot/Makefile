################################################################################
#     COPYRIGHT (C) 2012 Czyong. Hisilicon
#     All rights reserved.
# 
#     Create by Czyong 2012-12-20
# 
################################################################################

CFLAGS += -Wall -Wstrict-prototypes -fno-stack-protector

SRCS-y := start.S ddrphy_training_func.S ddrphy_training_print.S

SRCS-y += ddrphy_training.c lowlevel_init.c \
	ddr_dataeye_training.c irq.c hi3716mv310_ddr_training.c

SRCS-y += early_uart.S chip.c cpu.c

TEMPS   += $(TOPDIR)/.init.reg

$(SRCDIR)start.o: $(TOPDIR)/.init.reg

$(TOPDIR)/.init.reg: $(INIT_REG)
	@cp -f $< $@

