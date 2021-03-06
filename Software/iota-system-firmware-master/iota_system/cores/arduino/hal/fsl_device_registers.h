/*
** ###################################################################
**     Version:             rev. 1.0, 2014-05-15
**     Build:               b141209
**
**     Abstract:
**         Common include file for CMSIS register access layer headers.
**
**     Copyright (c) 2014 Freescale Semiconductor, Inc.
**     All rights reserved.
**
**     Redistribution and use in source and binary forms, with or without modification,
**     are permitted provided that the following conditions are met:
**
**     o Redistributions of source code must retain the above copyright notice, this list
**       of conditions and the following disclaimer.
**
**     o Redistributions in binary form must reproduce the above copyright notice, this
**       list of conditions and the following disclaimer in the documentation and/or
**       other materials provided with the distribution.
**
**     o Neither the name of Freescale Semiconductor, Inc. nor the names of its
**       contributors may be used to endorse or promote products derived from this
**       software without specific prior written permission.
**
**     THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
**     ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
**     WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
**     DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
**     ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
**     (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
**     LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
**     ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
**     (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
**     SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
**
**     http:                 www.freescale.com
**     mail:                 support@freescale.com
**
**     Revisions:
**     - rev. 1.0 (2014-05-15)
**         Customer release.
**
** ###################################################################
*/

#ifndef __FSL_DEVICE_REGISTERS_H__
#define __FSL_DEVICE_REGISTERS_H__

# define assert(__e) ((void)0)
#include <stdint.h>
#include <stdbool.h>
/*
 * Include the cpu specific register header files.
 *
 * The CPU macro should be declared in the project or makefile.
 */
#if (defined(CPU_MK22FN512CAP12) || defined(CPU_MK22FN512VDC12) || defined(CPU_MK22FN512VLH12) || \
    defined(CPU_MK22FN512VLL12) || defined(CPU_MK22FN512VMP12))

    #define K22F51212_SERIES

    /* CMSIS-style register definitions */
    #include "MK22F51212/include/MK22F51212.h"
    /* Extension register definitions */
    #include "MK22F51212/include/MK22F51212_extension.h"
    /* CPU specific feature definitions */
    #include "MK22F51212/include/MK22F51212_features.h"
    
    #include "system_MK22F51212.h"
    
    #include "fsl_sim_hal_MK22F51212.h"

#elif (defined(CPU_MKL17Z128VFM4) || defined(CPU_MKL17Z256VFM4) || defined(CPU_MKL17Z128VFT4) || \
    defined(CPU_MKL17Z256VFT4) || defined(CPU_MKL17Z128VLH4) || defined(CPU_MKL17Z256VLH4) || \
    defined(CPU_MKL17Z128VMP4) || defined(CPU_MKL17Z256VMP4))

    #define KL17Z4_SERIES

    /* CMSIS-style register definitions */
    #include "MKL17Z4/include/MKL17Z4.h"
    /* Extension register definitions */
    #include "MKL17Z4/include/MKL17Z4_extension.h"
    /* CPU specific feature definitions */
    #include "MKL17Z4/include/MKL17Z4_features.h"

    #include "system_MKL17Z4.h"
    
    #include "fsl_sim_hal_MKL17Z4.h"

#else
    #error "No valid CPU defined!"
#endif

#endif /* __FSL_DEVICE_REGISTERS_H__ */

/*******************************************************************************
 * EOF
 ******************************************************************************/
