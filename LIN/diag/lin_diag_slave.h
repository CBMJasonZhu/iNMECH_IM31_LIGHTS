/*********************************************************************
 *
 * Project:      LIN 2.1 driver suite
 *
 * File name:    lin_diag_slave.h
 *
 * Copyright:    (c) STMicroelectronics
 *
 * Author:       Giuseppe Stefano Fazio
 *
 * Description:  Diagnostic functionality specific to the slave node
 *
 * Target:       all
 *
 * Names:        gsf  Giuseppe Stefano Fazio  STMicroelectronics
 *
 * NOT TO BE CHANGED BY THE USER
 *
 *********************************************************************/

/*********************************************************************
 *
 * History:
 *
 * 20071213  v1.0   gsf  - Initial version for STM8
 *
 *********************************************************************/

/** \file lin_diag_slave.h
 */

#ifndef LIN_DIAG_SLAVE_H
  #define LIN_DIAG_SLAVE_H

#define LIN_DIAG_SLAVE_H_MAJOR_VERSION  1
#define LIN_DIAG_SLAVE_H_MINOR_VERSION  32
#define LIN_DIAG_SLAVE_H_PATCH_VERSION  1

#define LIN_DIAG_SLAVE_H_STM

/*********************************************************************
 * includes
 *********************************************************************/

#include "lin_def.h"
#include "lin_def_gen.h"
#include "lin_def_arch_include.h"
#include "lin_cfg_types.h"
#include "lin_types.h"


#ifdef LIN_VERSION_CONTROL_H
  #error "lin_version_control.h shall not be included in h-files and shall be the last file to be included in a c-file!"
#endif


/*********************************************************************
 * global macros
 *********************************************************************/
/*
 * bits for common diagnostic flags
 */
#define LIN_USER_DIAG_TX_READY                 0x01
#define LIN_ORIGINATOR_USER_DIAG_TX            0x10

/*
 * bits for common diagnostic flags
 */
#define LIN_DIAG2_TX_READY                     0x02u
#define LIN_ORIGINATOR_DIAG2_TX                0x20

#define LIN_DIAG_TX_READY_MASK                 0x0f
#define LIN_DIAG_ORIGINATOR_MASK               0xf0u



#define LIN_DIAG_FLAGS(ifc)          l_diagFlags

#define LIN_DIAG2_CONFIG_FLAGS(ifc)  l_diag2ConfigFlags
#define LIN_DIAG_NAD(ifc)            l_diagNad
#define LIN_DIAG2_BUFFER(ifc)        l_diag2Buffer

/*********************************************************************
 * global typedefs
 *********************************************************************/


/*********************************************************************
 * global constants
 *********************************************************************/

#ifdef LIN_INCLUDE_DIAGNOSTICS

#ifdef __COSMIC__                                           
 @tiny extern l_u8            l_diagFlags;

#elif __RAISONANCE__
 page0 extern l_u8            l_diagFlags;

#elif __IAR__
 __tiny extern l_u8            l_diagFlags;

#endif
	
#endif

#ifdef LIN_INCLUDE_DIAG2

#ifdef __COSMIC__                                           
 @tiny extern l_u8            l_diag2ConfigFlags;
 @tiny extern l_u8            l_diagNad;

#elif __RAISONANCE__
 page0 extern l_u8            l_diag2ConfigFlags;
 page0 extern l_u8            l_diagNad;
#elif __IAR__
 __tiny extern l_u8            l_diag2ConfigFlags;
 __tiny extern l_u8            l_diagNad;
#endif

#endif /* LIN_INCLUDE_DIAG2 */

/*********************************************************************
 * global variables
 *********************************************************************/

/*********************************************************************
 * global functions
 *********************************************************************/


#ifdef LIN_INCLUDE_READ_BY_ID
  STACK l_bool ld_readByIdCallback
  (
    l_u8                       id,
    l_u8 *                     pBuffer
  );
#endif

#if (defined(LIN_21) && (defined(LIN_INCLUDE_READ_BY_ID) || defined(LIN_INCLUDE_COND_CHANGE_NAD)))
  STACK l_u8 ld_read_by_id_callout
  (
    l_u8                       id,
    l_u8 *                     data
  );
#endif

#ifdef LIN_INCLUDE_DATA_DUMP
  STACK l_bool ld_dataDumpCallback
  (
    const l_u8 *               sendBuf,
    l_u8 *                     recBuf
  );
#endif

STACK extern void   ld_requestReceived (__OPTIONAL_IFC_DEFINITION_COMMA__
                                        const l_u8 *pBuffer);

STACK extern l_bool ld_composeSlaveResponse(__OPTIONAL_IFC_DEFINITION_COMMA__ l_u8 *pBuffer);
STACK extern void ld_transmittedSlaveResponse(__OPTIONAL_IFC_DEFINITION__);
STACK extern void ld_signalRxFrameError(__OPTIONAL_IFC_DEFINITION__);
STACK extern void ld_signalTxFrameError(__OPTIONAL_IFC_DEFINITION__);

#ifdef LIN_21
  STACK extern l_u8 ld_set_configuration(l_ifc_handle ifc, const l_u8* const mydata, l_u16 length);
  STACK extern l_u8 ld_read_configuration(l_ifc_handle ifc, l_u8* const data, l_u8* const length);
#endif /* LIN_21 */

#endif /* LIN_DIAG_SLAVE_H */

/*
 * eof lin_diag_slave.h
 */

