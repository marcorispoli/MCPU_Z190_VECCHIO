/*HEADER**********************************************************************
*
* Copyright 2012 Freescale Semiconductor, Inc.
*
* This software is owned or controlled by Freescale Semiconductor.
* Use of this software is governed by the Freescale MQX RTOS License
* distributed with this Material.
* See the MQX_RTOS_LICENSE file distributed for more details.
*
* Brief License Summary:
* This software is provided in source form for you to use free of charge,
* but it is not open source software. You are allowed to use this software
* but you cannot redistribute it or derivative works of it in source form.
* The software may be used only in connection with a product containing
* a Freescale microprocessor, microcontroller, or digital signal processor.
* See license agreement file for full license terms including other
* restrictions.
*****************************************************************************
*
* Comments:
* ARM Generic Interrupt Controller version 2.0
*
*
*END************************************************************************/

#ifndef __gic_h__
#define __gic_h__

#include <mqx.h>

/* Initialization of interrupt vector in GIC (vector, priority, enable/disable) */
_mqx_uint _gic_int_init(_mqx_uint irq, _mqx_uint prior, bool enable);

/* Enable interrupt for given vector */
_mqx_uint _gic_int_enable(_mqx_uint irq);

/* Disable interrupt for given vector */
_mqx_uint _gic_int_disable(_mqx_uint irq);

#endif // __gic_h__
