#!/usr/bin/env bash

# expect forward slash paths
ROOTDIR="${1}"
OUTPUTDIR="${2}"
TOOL="${3}"


# copy common files
mkdir -p "${OUTPUTDIR}"
mkdir -p "${OUTPUTDIR}/"
mkdir -p "${OUTPUTDIR}/.."
cp -f "${ROOTDIR}/mqx/source/io/lwgpio/lwgpio.h" "${OUTPUTDIR}/lwgpio.h"
cp -f "${ROOTDIR}/mqx/source/io/hwtimer/hwtimer_pit.h" "${OUTPUTDIR}/hwtimer_pit.h"
cp -f "${ROOTDIR}/mqx/source/io/hwtimer/hwtimer_gtim.h" "${OUTPUTDIR}/hwtimer_gtim.h"
cp -f "${ROOTDIR}/mqx/source/io/spi/spi_dspi_common.h" "${OUTPUTDIR}/spi_dspi_common.h"
cp -f "${ROOTDIR}/mqx/source/io/enet/enet_rev.h" "${OUTPUTDIR}/enet_rev.h"
cp -f "${ROOTDIR}/mqx/source/io/sai/int/sai_int_prv.h" "${OUTPUTDIR}/sai_int_prv.h"
cp -f "${ROOTDIR}/mqx/source/io/enet/phy/phy_dp83xxx.h" "${OUTPUTDIR}/phy_dp83xxx.h"
cp -f "${ROOTDIR}/mqx/source/io/flashx/freescale/flash_vybrid.h" "${OUTPUTDIR}/flash_vybrid.h"
cp -f "${ROOTDIR}/mqx/source/io/enet/macnet/macnet_1588.h" "${OUTPUTDIR}/macnet_1588.h"
cp -f "${ROOTDIR}/mqx/source/include/mqx.h" "${OUTPUTDIR}/mqx.h"
cp -f "${ROOTDIR}/mqx/source/io/cm/vybrid/bsp_clk_nodes.h" "${OUTPUTDIR}/bsp_clk_nodes.h"
cp -f "${ROOTDIR}/mqx/source/io/enet/macnet/macnet_rev.h" "${OUTPUTDIR}/macnet_rev.h"
cp -f "${ROOTDIR}/mqx/source/io/lpm/vybrid/lpm_vybrid.h" "${OUTPUTDIR}/lpm_vybrid.h"
cp -f "${ROOTDIR}/mqx/source/io/spi/spi_dspi_dma.h" "${OUTPUTDIR}/spi_dspi_dma.h"
cp -f "${ROOTDIR}/mqx/source/io/enet/enet.h" "${OUTPUTDIR}/enet.h"
cp -f "${ROOTDIR}/mqx/source/io/cm/vybrid/bsp_clk_name.h" "${OUTPUTDIR}/bsp_clk_name.h"
cp -f "${ROOTDIR}/mqx/source/io/flashx/flashx.h" "${OUTPUTDIR}/flashx.h"
cp -f "${ROOTDIR}/mqx/source/io/cm/cm.h" "${OUTPUTDIR}/cm.h"
cp -f "${ROOTDIR}/mqx/source/io/usb/if_dev_khci.h" "${OUTPUTDIR}/if_dev_khci.h"
cp -f "${ROOTDIR}/mqx/source/io/tfs/tfs.h" "${OUTPUTDIR}/tfs.h"
cp -f "${ROOTDIR}/mqx/source/io/serial/serl_kuart.h" "${OUTPUTDIR}/serl_kuart.h"
cp -f "${ROOTDIR}/mqx/source/io/nandflash/nandflash.h" "${OUTPUTDIR}/nandflash.h"
cp -f "${ROOTDIR}/config/common/small_ram_config.h" "${OUTPUTDIR}/../small_ram_config.h"
cp -f "${ROOTDIR}/mqx/source/io/pcb/io_pcb.h" "${OUTPUTDIR}/io_pcb.h"
cp -f "${ROOTDIR}/config/common/maximum_config.h" "${OUTPUTDIR}/../maximum_config.h"
cp -f "${ROOTDIR}/mqx/source/io/usb/usb_bsp.h" "${OUTPUTDIR}/usb_bsp.h"
cp -f "${ROOTDIR}/mqx/source/io/qspi/qspi.h" "${OUTPUTDIR}/qspi.h"
cp -f "${ROOTDIR}/mqx/source/io/qspi/qspi_quadspi.h" "${OUTPUTDIR}/qspi_quadspi.h"
cp -f "${ROOTDIR}/mqx/source/io/enet/ethernet.h" "${OUTPUTDIR}/ethernet.h"
cp -f "${ROOTDIR}/mqx/source/io/pcb/shmem/pcb_shm.h" "${OUTPUTDIR}/pcb_shm.h"
cp -f "${ROOTDIR}/mqx/source/io/fbdev/fbdev.h" "${OUTPUTDIR}/fbdev.h"
cp -f "${ROOTDIR}/mqx/source/io/enet/macnet/macnet_vybrid.h" "${OUTPUTDIR}/macnet_vybrid.h"
cp -f "${ROOTDIR}/mqx/source/bsp/twrvf65gs10_a5/bsp.h" "${OUTPUTDIR}/bsp.h"
cp -f "${ROOTDIR}/mqx/source/io/lpm/lpm.h" "${OUTPUTDIR}/lpm.h"
cp -f "${ROOTDIR}/mqx/source/io/sdcard/sdcard_esdhc/sdcard_esdhc.h" "${OUTPUTDIR}/sdcard_esdhc.h"
cp -f "${ROOTDIR}/mqx/source/io/core_mutex/core_mutex_sema4.h" "${OUTPUTDIR}/core_mutex_sema4.h"
cp -f "${ROOTDIR}/mqx/source/io/i2c/i2c_qi2c.h" "${OUTPUTDIR}/i2c_qi2c.h"
cp -f "${ROOTDIR}/config/common/verif_enabled_config.h" "${OUTPUTDIR}/../verif_enabled_config.h"
cp -f "${ROOTDIR}/mqx/source/io/dma/dma.h" "${OUTPUTDIR}/dma.h"
cp -f "${ROOTDIR}/mqx/source/bsp/twrvf65gs10_a5/bsp_rev.h" "${OUTPUTDIR}/bsp_rev.h"
cp -f "${ROOTDIR}/mqx/source/io/cm/vybrid/bsp_clk_api.h" "${OUTPUTDIR}/bsp_clk_api.h"
cp -f "${ROOTDIR}/mqx/source/io/sai/sai_ksai.h" "${OUTPUTDIR}/sai_ksai.h"
cp -f "${ROOTDIR}/mqx/source/io/dma/edma.h" "${OUTPUTDIR}/edma.h"
cp -f "${ROOTDIR}/mqx/source/io/hwtimer/hwtimer.h" "${OUTPUTDIR}/hwtimer.h"
cp -f "${ROOTDIR}/mqx/source/io/esdhc/esdhc.h" "${OUTPUTDIR}/esdhc.h"
cp -f "${ROOTDIR}/mqx/source/io/flashx/freescale/flash_quadspi.h" "${OUTPUTDIR}/flash_quadspi.h"
cp -f "${ROOTDIR}/mqx/source/io/can/flexcan/fsl_flexcan_hal.h" "${OUTPUTDIR}/fsl_flexcan_hal.h"
cp -f "${ROOTDIR}/mqx/source/io/io_mem/io_mem.h" "${OUTPUTDIR}/io_mem.h"
cp -f "${ROOTDIR}/mqx/source/io/spi/spi_dspi.h" "${OUTPUTDIR}/spi_dspi.h"
cp -f "${ROOTDIR}/mqx/source/io/cm/vybrid/bsp_clk_nodes_impl.h" "${OUTPUTDIR}/bsp_clk_nodes_impl.h"
cp -f "${ROOTDIR}/mqx/source/io/lpm/vybrid/vybrid_wkpu.h" "${OUTPUTDIR}/vybrid_wkpu.h"
cp -f "${ROOTDIR}/mqx/source/io/lwadc/lwadc.h" "${OUTPUTDIR}/lwadc.h"
cp -f "${ROOTDIR}/mqx/source/io/usb/if_host_khci.h" "${OUTPUTDIR}/if_host_khci.h"
cp -f "${ROOTDIR}/mqx/source/io/dma/edma_prv.h" "${OUTPUTDIR}/edma_prv.h"
cp -f "${ROOTDIR}/mqx/source/io/core_mutex/core_mutex.h" "${OUTPUTDIR}/core_mutex.h"
cp -f "${ROOTDIR}/mqx/source/bsp/twrvf65gs10_a5/twrvf65gs10_a5.h" "${OUTPUTDIR}/twrvf65gs10_a5.h"
cp -f "${ROOTDIR}/mqx/source/io/lwadc/lwadc_vadc.h" "${OUTPUTDIR}/lwadc_vadc.h"
cp -f "${ROOTDIR}/mqx/source/io/spi/spi.h" "${OUTPUTDIR}/spi.h"
cp -f "${ROOTDIR}/mqx/source/io/serial/serial.h" "${OUTPUTDIR}/serial.h"
cp -f "${ROOTDIR}/mqx/source/io/core_mutex/sema4.h" "${OUTPUTDIR}/sema4.h"
cp -f "${ROOTDIR}/mqx/source/io/pcb/mqxa/pcbmqxav.h" "${OUTPUTDIR}/pcbmqxav.h"
cp -f "${ROOTDIR}/mqx/source/io/usb/if_host_ehci.h" "${OUTPUTDIR}/if_host_ehci.h"
cp -f "${ROOTDIR}/mqx/source/io/pcb/mqxa/pcb_mqxa.h" "${OUTPUTDIR}/pcb_mqxa.h"
cp -f "${ROOTDIR}/mqx/source/io/can/flexcan/fsl_flexcan_driver.h" "${OUTPUTDIR}/fsl_flexcan_driver.h"
cp -f "${ROOTDIR}/mqx/source/io/sai/sai.h" "${OUTPUTDIR}/sai.h"
cp -f "${ROOTDIR}/mqx/source/io/i2c/qi2c.h" "${OUTPUTDIR}/qi2c.h"
cp -f "${ROOTDIR}/mqx/source/io/usb/if_dev_ehci.h" "${OUTPUTDIR}/if_dev_ehci.h"
cp -f "${ROOTDIR}/mqx/source/io/cm/cm_vybrid.h" "${OUTPUTDIR}/cm_vybrid.h"
cp -f "${ROOTDIR}/mqx/source/io/pcb/shmem/pcbshmv.h" "${OUTPUTDIR}/pcbshmv.h"
cp -f "${ROOTDIR}/mqx/source/io/rtc/rtc.h" "${OUTPUTDIR}/rtc.h"
cp -f "${ROOTDIR}/mqx/source/io/pipe/io_pipe.h" "${OUTPUTDIR}/io_pipe.h"
cp -f "${ROOTDIR}/mqx/source/bsp/twrvf65gs10_a5/bsp_cm.h" "${OUTPUTDIR}/bsp_cm.h"
cp -f "${ROOTDIR}/mqx/source/io/nandflash/nfc/nfc.h" "${OUTPUTDIR}/nfc.h"
cp -f "${ROOTDIR}/mqx/source/io/lwgpio/lwgpio_vgpio.h" "${OUTPUTDIR}/lwgpio_vgpio.h"
cp -f "${ROOTDIR}/config/twrvf65gs10_a5/user_config.h" "${OUTPUTDIR}/../user_config.h"
cp -f "${ROOTDIR}/mqx/source/io/enet/enet_wifi.h" "${OUTPUTDIR}/enet_wifi.h"
cp -f "${ROOTDIR}/mqx/source/io/io_null/io_null.h" "${OUTPUTDIR}/io_null.h"
cp -f "${ROOTDIR}/mqx/source/io/dcu4/dcu4.h" "${OUTPUTDIR}/dcu4.h"
cp -f "${ROOTDIR}/mqx/source/io/i2c/i2c.h" "${OUTPUTDIR}/i2c.h"
cp -f "${ROOTDIR}/mqx/source/io/sai/sai_audio.h" "${OUTPUTDIR}/sai_audio.h"
cp -f "${ROOTDIR}/mqx/source/io/sdcard/sdcard.h" "${OUTPUTDIR}/sdcard.h"
cp -f "${ROOTDIR}/mqx/source/io/rtc/srtc.h" "${OUTPUTDIR}/srtc.h"
cp -f "${ROOTDIR}/mqx/source/io/can/flexcan/fsl_flexcan_int.h" "${OUTPUTDIR}/fsl_flexcan_int.h"
cp -f "${ROOTDIR}/mqx/source/bsp/twrvf65gs10_a5/init_lpm.h" "${OUTPUTDIR}/init_lpm.h"
cp -f "${ROOTDIR}/config/common/smallest_config.h" "${OUTPUTDIR}/../smallest_config.h"


# ds5 files
if [ "${TOOL}" = "ds5" ]; then
mkdir -p "${OUTPUTDIR}/"
cp -f "${ROOTDIR}/mqx/source/bsp/twrvf65gs10_a5/ds5/intram.scf" "${OUTPUTDIR}/intram.scf"
cp -f "${ROOTDIR}/mqx/source/bsp/twrvf65gs10_a5/ds5/ddr.scf" "${OUTPUTDIR}/ddr.scf"
mkdir -p "${OUTPUTDIR}/Generated_Code"
mkdir -p "${OUTPUTDIR}/Sources"
:
fi

# gcc_arm files
if [ "${TOOL}" = "gcc_arm" ]; then
mkdir -p "${OUTPUTDIR}/"
cp -f "${ROOTDIR}/mqx/source/bsp/twrvf65gs10_a5/gcc_arm/intram.ld" "${OUTPUTDIR}/intram.ld"
cp -f "${ROOTDIR}/mqx/source/bsp/twrvf65gs10_a5/gcc_arm/ddr.ld" "${OUTPUTDIR}/ddr.ld"
mkdir -p "${OUTPUTDIR}/Generated_Code"
mkdir -p "${OUTPUTDIR}/Sources"
:
fi


