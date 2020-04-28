/*********************************************************************
 *
 * Project:      LIN 2.1 driver suite
 *
 * File name:    lin_diag_master.h
 *
 * Copyright:    (c) STMicroelectronics
 *
 * Author:       Giuseppe Stefano Fazio
 *
 * Description:  Diagnostic functionality specific to the master node
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

/** \file lin_diag_master.h
 */

#ifndef LIN_DIAG_MASTER_H
  #define LIN_DIAG_MASTER_H

#define LIN_DIAG_MASTER_H_MAJOR_VERSION  1
#define LIN_DIAG_MASTER_H_MINOR_VERSION  31
#define LIN_DIAG_MASTER_H_PATCH_VERSION  1

#define LIN_DIAG_MASTER_H_STM

/*********************************************************************
 * includes
 *********************************************************************/

#include "lin_cfg_types.h"
#include "lin_types.h"
#include "lin_def_gen.h"
#include "lin_def_arch_include.h"

#ifdef LIN_VERSION_CONTROL_H
  #error "lin_version_control.h shall not be included in h-files and shall be the last file to be included in a c-file!"
#endif


/*********************************************************************
 * global macros
 *********************************************************************/

/*********************************************************************
 * global typedefs
 *********************************************************************/

/*********************************************************************
 * global constants
 *********************************************************************/

/*********************************************************************
 * global variables
 *********************************************************************/

#ifdef LIN_INCLUDE_DIAGNOSTICS
  /*
   * bits for common diagnostic flags
   */
  #define LIN_USER_DIAG_TX_READY                 0x01u
  #define LIN_ORIGINATOR_USER_DIAG_TX            0x10

  /*
   * bits for common diagnostic flags
   */
  #define LIN_DIAG2_TX_READY                     0x02u
  #define LIN_ORIGINATOR_DIAG2_TX                0x20

  #define LIN_DIAG_TX_READY_MASK                 0x0f
  #define LIN_DIAG_ORIGINATOR_MASK               0xf0u

  /*
   * bits for dedicated diagnostic class2 flags
   */

  #define LIN_DIAG2_EXPECT_RESPONSE            0x01u
  #define LIN_DIAG2_ERROR                      0x02u
  #define LIN_DIAG2_RESPONSE_RECEIVED          0x04u
  #define LIN_DIAG2_DATA_VALID                 0x08u

  #ifdef __MORE_IFCS__
    #define LIN_DIAG_FLAGS(ifc)            l_diagFlags[ifc]

    #define LIN_DIAG2_SIZE(ifc)            l_diag2Size[ifc]
    #define LIN_DIAG2_RSID(ifc)            l_diag2Rsid[ifc]
    #define LIN_DIAG2_ERROR_CODE(ifc)      l_diag2ErrorCode[ifc]
    #define LIN_DIAG2_READ_BUFFER(ifc)     l_diag2ReadBuffer[ifc]
    #define LIN_DIAG2_FLAGS(ifc)           l_diag2Flags[ifc]
    #define LIN_DIAG2_BUFFER(ifc)          l_diag2Buffer[ifc]

  #else
    #define LIN_DIAG_FLAGS(ifc)            l_diagFlags

    #define LIN_DIAG2_SIZE(ifc)            l_diag2Size
    #define LIN_DIAG2_RSID(ifc)            l_diag2Rsid
    #define LIN_DIAG2_ERROR_CODE(ifc)      l_diag2ErrorCode
    #define LIN_DIAG2_READ_BUFFER(ifc)     l_diag2ReadBuffer
    #define LIN_DIAG2_FLAGS(ifc)           l_diag2Flags
    #define LIN_DIAG2_BUFFER(ifc)          l_diag2Buffer
  #endif

  #ifdef __MORE_IFCS__
    extern l_u8                 l_diagFlags[LIN_IFC_NR_MAX];
  #else
    extern l_u8                 l_diagFlags;
  #endif


#endif /* LIN_INCLUDE_DIAGNOSTICS */

#ifdef LIN_INCLUDE_DIAG2
  #ifdef __MORE_IFCS__
    extern l_u8                 l_diag2Flags[LIN_IFC_NR_MAX];
  #else
    extern l_u8                 l_diag2Flags;
  #endif
#endif /* LIN_INCLUDE_DIAG2*/


/*********************************************************************
 * global functions
 *********************************************************************/

STACK extern l_bool ld_signalRequestDue     (__OPTIONAL_IFC_DEFINITION_COMMA__ l_u8 *pBuffer);
STACK extern void ld_signalRequestReceived  (const l_ifc_handle ifc);
STACK extern void ld_signalResponseReceived (const l_ifc_handle ifc, const l_u8 *pBuffer);
STACK extern void ld_signalTxFrameError     (__OPTIONAL_IFC_DEFINITION__);
STACK extern void ld_signalRxFrameError     (__OPTIONAL_IFC_DEFINITION__);
STACK extern void ld_signalNonDiagFrame     (__OPTIONAL_IFC_DEFINITION__);
STACK extern void ld_signalPreDefFrame      (__OPTIONAL_IFC_DEFINITION__);
STACK extern void ld_responseDistributor(__OPTIONAL_IFC_DEFINITION_COMMA__
                                          const l_u8 *pBuffer);
STACK extern void ld_composeMasterRequest(__OPTIONAL_IFC_DEFINITION_COMMA__
                                          l_u8 *  pBuffer);
STACK extern void ld_transmittedMasterRequest(__OPTIONAL_IFC_DEFINITION__);


#endif /* LIN_DIAG_MASTER_H */

/*
 * eof lin_diag_master.h
 */
