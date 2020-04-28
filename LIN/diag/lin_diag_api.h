/*********************************************************************
 *
 * Project:      LIN 2.1 driver suite
 *
 * File name:    lin_diag_api.h
 *
 * Copyright:    (c) STMicroelectronics
 *
 * Author:       Giuseppe Stefano Fazio
 *
 * Description:  Declaration of the diagnostic API
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

/** \file lin_diag_api.h
 */

#ifndef LIN_DIAG_API_H
  #define LIN_DIAG_API_H

#define LIN_DIAG_API_H_MAJOR_VERSION  1
#define LIN_DIAG_API_H_MINOR_VERSION  31
#define LIN_DIAG_API_H_PATCH_VERSION  1

#define LIN_DIAG_API_H_STM

/*********************************************************************
 * includes
 *********************************************************************/

#include "lin_types.h"
#include "lin_cfg_types.h"
#include "lin_def.h"
#include "lin_def_gen.h"
#include "lin_def_arch_include.h"


#ifdef LIN_VERSION_CONTROL_H
  #error "lin_version_control.h shall not be included in h-files and shall be the last file to be included in a c-file!"
#endif


/*********************************************************************
 * global macros
 *********************************************************************/

/* error codes */
#define LD_NO_ERROR                                         0    /* Do not change this, '0' is used driver code */
#define LD_ERROR_GENERAL_REJECT                          0x10
#define LD_ERROR_SERVICE_NOT_SUPPORTED                   0x11
#define LD_ERROR_SUBFUNCTION_NOT_SUPPORTED               0x12

/* response codes for class 2 */
#ifdef LIN_21
  #define LD_RSID_DEFAULT                                   0
  #define LD_ERROR_CODE_DEFAULT                             0
  #define LD_SERVICE_BUSY                                   0
  #define LD_REQUEST_FINISHED                               1
  #define LD_SERVICE_IDLE                                   2
  #define LD_SERVICE_ERROR                                  3
  #define LD_READ_OK                                        4
  #define LD_LENGTH_TOO_SHORT                               5
  #define LD_SET_OK                                         6
  #define LD_LENGTH_NOT_CORRECT                             7
  #define LD_DATA_ERROR                                     8
  #define LD_NEGATIVE_RESPONSE                              9
  #define LD_POSITIVE_RESPONSE                             10
#endif /* LIN_21 */

#define LD_SUCCESS                                          0    /* Do not change this, '0' is used in init */
#define LD_NEGATIVE                                         1
#define LD_NO_RESPONSE                                      2
#define LD_OVERWRITTEN                                      3

/* response codes for cooked class 3 */
#define LD_COMPLETED                                        0    /* Do not change this, '0' is used in init */
#define LD_IN_PROGRESS                                      1
#define LD_FAILED                                           2
#ifdef LIN_21
  #define LD_N_AS_TIMEOUT                                   3
  #define LD_N_CR_TIMEOUT                                   4
  #define LD_WRONG_SN                                       5
#endif /* LIN_21 */

/* response codes for raw class 3 */
#define LD_QUEUE_EMPTY                                      0
#define LD_QUEUE_FULL                                       1
#ifdef LIN_21
  #define LD_QUEUE_AVAILABLE                                2
  #define LD_TRANSMIT_ERROR                                 3
  #define LD_RECEIVE_ERROR                                  5
  #define LD_NO_DATA                                        6
#else
  #define LD_QUEUE_READY                                    2
  #define LD_TRANSFER_ERROR                                 3
#endif /* LIN_21 */
#define LD_DATA_AVAILABLE                                   4

#ifdef LIN_SLAVE_NODE
  #define LIN_DIAG2_FLAGS_ASSIGN_FRAME_ID                0x01
  #define LIN_DIAG2_FLAGS_ASSIGN_NAD                     0x02
  #define LIN_DIAG2_FLAGS_COND_CHANGE_NAD                0x04
  #define LIN_DIAG2_FLAGS_READ_BY_ID                     0x08
  #define LIN_DIAG2_FLAGS_DATA_DUMP                      0x10
  #ifdef LIN_21
    #define LIN_DIAG2_FLAGS_ASSIGN_FRAME_ID_RANGE        0x20
    #define LIN_DIAG2_FLAGS_SAVE_CONFIGURATION           0x40
  #endif /* LIN_21 */
#endif

/*
 * Wildcards for product id, message id and NAD
 */
#ifdef LIN_21
  #define LD_FUNCTIONAL_NAD                               126
#endif /* LIN_21 */
#define LD_BROADCAST                                      127
#define LD_ANY_SUPPLIER                               0x7FFFu
#define LD_ANY_FUNCTION                               0xFFFFu
#define LD_ANY_MESSAGE                                0xFFFFu

/*********************************************************************
 * global typedefs
 *********************************************************************/

/*********************************************************************
 * global constants
 *********************************************************************/

/*********************************************************************
 * global variables
 *********************************************************************/

/*********************************************************************
 * global functions
 *********************************************************************/

#if (defined(LIN_INCLUDE_TP) && defined(LIN_21))
   STACK extern void ld_init (l_ifc_handle ifc) REENTRANT;
#endif /* defined(LIN_INCLUDE_TP) && defined(LIN_21) */

#ifdef LIN_MASTER_NODE

  #ifdef LIN_INCLUDE_DIAG2
    #ifdef LIN_21
      STACK extern l_u8 ld_is_ready(l_ifc_handle ifc) REENTRANT;
      STACK extern void ld_check_response(l_ifc_handle ifc, l_u8 *const pRsid, l_u8 *const pErrorCode) REENTRANT;
    #else
      STACK extern l_bool ld_is_ready(l_ifc_handle ifc) REENTRANT;
      STACK extern l_u8 ld_check_response(l_ifc_handle ifc, l_u8 *pRsid, l_u8 *pErrorCode) REENTRANT;
    #endif /* LIN_21 */

  #endif /* LIN_INCLUDE_DIAG2 */

  #ifdef LIN_INCLUDE_ASSIGN_NAD
    #ifdef LIN_21
      STACK extern void ld_assign_NAD(l_ifc_handle ifc, l_u8 initial_nad, l_u16 supplier_id, l_u16 function_id, l_u8 new_nad) REENTRANT;
    #else
      STACK extern void ld_assign_NAD(l_ifc_handle ifc, l_u8 nad, l_u16 supplier_id, l_u16 function_id, l_u8 new_nad) REENTRANT;
    #endif /* LIN_21 */
  #endif /* LIN_INCLUDE_ASSIGN_NAD */

  #ifdef LIN_INCLUDE_ASSIGN_FRAME_ID
    STACK extern void ld_assign_frame_id(l_ifc_handle ifc, l_u8 nad, l_u16 supplier_id, l_u16 message_id, l_u8 pid) REENTRANT;
  #endif /* LIN_INCLUDE_ASSIGN_FRAME_ID */

  #ifdef LIN_21
    #ifdef LIN_INCLUDE_ASSIGN_FRAME_ID_RANGE
      STACK void ld_assign_frame_id_range(l_ifc_handle ifc, l_u8 nad, l_u8 start_index, const l_u8* const PIDs) REENTRANT;
    #endif /* LIN_INCLUDE_ASSIGN_FRAME_ID_RANGE */

    #ifdef LIN_INCLUDE_SAVE_CONFIGURATION
      STACK void ld_save_configuration(l_ifc_handle ifc, l_u8 nad) REENTRANT;
    #endif /* LIN_INCLUDE_SAVE_CONFIGURATION */
  #endif /* LIN_21 */
  
  #ifdef LIN_INCLUDE_READ_BY_ID
    STACK extern void ld_read_by_id(l_ifc_handle ifc, l_u8 nad, l_u16 supplier_id, l_u16 function_id, l_u8 id, l_u8 *const data) REENTRANT;
  #endif /* LIN_INCLUDE_READ_BY_ID */

  #ifdef LIN_INCLUDE_COND_CHANGE_NAD
    STACK extern void ld_conditional_change_NAD(l_ifc_handle ifc, l_u8 nad, l_u8 id, l_u8 byte, l_u8 mask, l_u8 invert, l_u8 new_nad) REENTRANT;
  #endif /* LIN_INCLUDE_COND_CHANGE_NAD */

  #ifdef LIN_INCLUDE_DATA_DUMP
    STACK extern void ld_data_dump(l_ifc_handle ifc, l_u8 nad, const l_u8 *const sendBuf, l_u8 *const receiveBuf) REENTRANT;
  #endif /* LIN_INCLUDE_DATA_DUMP */

#endif /* LIN_MASTER_NODE */


#ifdef LIN_INCLUDE_RAW_TP

  STACK extern void ld_put_raw       (l_ifc_handle ifc, const l_u8 *const pData) REENTRANT;
  STACK extern void ld_get_raw       (l_ifc_handle ifc, l_u8 *const pData) REENTRANT;
  STACK extern l_u8 ld_raw_tx_status (l_ifc_handle ifc) REENTRANT;
  STACK extern l_u8 ld_raw_rx_status (l_ifc_handle ifc) REENTRANT;
  STACK extern l_bool ld_raw_tx_delete(l_ifc_handle ifc) REENTRANT;

#endif /* LIN_INCLUDE_RAW_TP */


#ifdef LIN_INCLUDE_COOKED_TP

  STACK extern void ld_send_message    (l_ifc_handle ifc, l_u16 length, l_u8 nad, const l_u8 *const pData) REENTRANT;
#ifdef LIN_21
  STACK extern void ld_receive_message (l_ifc_handle ifc, l_u16 *const length, l_u8 *const nad, l_u8 *const pData) REENTRANT;
#else
  STACK extern void ld_receive_message (l_ifc_handle ifc, l_u16 *length, l_u8 *nad, l_u8 *const pData) REENTRANT;
#endif /* LIN_21 */
  STACK extern l_u8 ld_tx_status       (l_ifc_handle ifc) REENTRANT;
  STACK extern l_u8 ld_rx_status       (l_ifc_handle ifc) REENTRANT;

#endif /* LIN_INCLUDE_COOKED_TP */


#endif /* LIN_DIAG_API_H */

/*
 * eof lin_diag_api.h
 */
