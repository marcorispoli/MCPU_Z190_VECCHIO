#----------------------------------------------------------
# setup required variables 
#----------------------------------------------------------


PROJ_DIR	    = $(abspath /mnt/hgfs/DBT/M4)
MQX_ROOTDIR         = $(PROJ_DIR)/MQX
SHARED_DIR	    = $(PROJ_DIR)/../shared_a5_m4
COM_DIR	    	    = $(PROJ_DIR)/COMMON
SRC_DIR	    	    = $(PROJ_DIR)/M4_MASTER/source
SRC_BYM	    	    = $(PROJ_DIR)/M4_MASTER/source/BIOPSY


#LIBRARY_ROOTDIR     = $(MQX_ROOTDIR)/lib/$(BOARD).$(TOOL)
LIBRARY_ROOTDIR     = $(MQX_ROOTDIR)/lib/$(BOARD).$(TOOL)
APPLICATION_DIR     = ./$(TOOL)/$(LOADNAME)_$(CONFIG)
APPLICATION_FILE    = $(APPLICATION_DIR)/$(NAME).$(APPLICATION_EXT)
LINKER_FILE         = $(call GET_BSP_LINKER_FILE)

TYPE                = application
NAME                = m4_master
BOARD               = twrvf65gs10_m4


#-----------------------------------------------------------
# load tool settings
#-----------------------------------------------------------
-include tools/$(TOOL).mak


#-----------------------------------------------------------
# search paths
#-----------------------------------------------------------
INCLUDE += $(SRC_DIR)
INCLUDE += $(COM_DIR)
INCLUDE += $(SHARED_DIR)
INCLUDE += $(LIBRARY_ROOTDIR)/$(CONFIG)/mcc


SOURCES += $(SRC_BYM)/BIOPSY_EXTENDED/biopsyExtended.c
SOURCES += $(SRC_BYM)/BIOPSY_EXTENDED/biopsyExtended_driver.c
SOURCES += $(SRC_BYM)/BIOPSY_EXTENDED/biopsy_simulator.c
SOURCES += $(SRC_BYM)/BIOPSY_STANDARD/biopsyStandard.c
SOURCES += $(SRC_BYM)/biopsy_startup.c
SOURCES += $(SRC_DIR)/fault.c
SOURCES += $(SRC_DIR)/loader.c
SOURCES += $(SRC_DIR)/master_can.c 
SOURCES += $(SRC_DIR)/actuators.c
SOURCES += $(SRC_DIR)/pcb215.c 
SOURCES += $(SRC_DIR)/pcb249U1.c
SOURCES += $(SRC_DIR)/seqrx_std.c 
SOURCES += $(SRC_DIR)/seqrx_tomo.c
SOURCES += $(SRC_DIR)/gui.c 
SOURCES += $(SRC_DIR)/main.c
SOURCES += $(SRC_DIR)/pcb190.c 
SOURCES += $(SRC_DIR)/pcb244.c
SOURCES += $(SRC_DIR)/pcb249U2.c
SOURCES += $(SRC_DIR)/seqrx_std_aec.c
SOURCES += $(SRC_DIR)/seqrx_ae.c
SOURCES += $(SRC_DIR)/seqrx_tomo_aec.c
SOURCES += $(SRC_DIR)/sequences.c
SOURCES += $(SRC_DIR)/ser422.c
SOURCES += $(COM_DIR)/common.c
SOURCES += $(COM_DIR)/canopen.c
SOURCES += $(COM_DIR)/mccClass.c
#----------------------------------------------------------
# include partial makefiles
#----------------------------------------------------------
include $(MQX_ROOTDIR)/build/common/make/global.mak
-include $(MQX_ROOTDIR)/build/$(BOARD)/make/tools/$(TOOL).mak
include $(MQX_ROOTDIR)/build/common/make/verify.mak
include $(MQX_ROOTDIR)/build/common/make/app-process.mak
#----------------------------------------------------------


elf:
	/opt/arm-2017/bin/arm-none-eabi-objcopy -O binary $(APPLICATION_FILE) $(PROJ_DIR)/BIN/$(NAME).bin
	cp $(PROJ_DIR)/BIN/$(NAME).bin /data/CHIMERA/Workspace/DBT/M4_MASTER
	/opt/arm-2012/bin/arm-none-eabi-readelf $(APPLICATION_FILE) -h | grep "Entry point"

