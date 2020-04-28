/*********************************************************************
 *
 * Project:      LIN 2.1 driver suite
 *
 * File name:    lin_diag.h
 *
 * Copyright:    (c) STMicroelectronics
 *
 * Author:       Giuseppe Stefano Fazio
 *
 * Description:  Common diagnostic functionality
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
 * 20090420  v1.1   gsf  - Modified LIN_COOKED_RX_N_CR_TIMEOUT in
 *                         LIN_COOKED_N_CR_TIMEOUT and 
 *                         LIN_COOKED_TX_N_AS_TIMEOUT in 
 *                         LIN_COOKED_N_AS_TIMEOUT
 * 20120822  v1.2   jk  -  LIN_DIAG_FRAME_COUNTER_MAX value changed from '15' to '16'
 *
 *********************************************************************/

/** file \lin_diag.h
 */

#ifndef LIN_DIAG_H
  #define LIN_DIAG_H

#define LIN_DIAG_H_MAJOR_VERSION  1
#define LIN_DIAG_H_MINOR_VERSION  31
#define LIN_DIAG_H_PATCH_VERSION  2

#define LIN_DIAG_H_STM

/*********************************************************************
 * includes
 *********************************************************************/

#include "lin_types.h"
#include "lin_def.h"
#include "lin_def_gen.h"
#include "lin_def_arch_include.h"
#include "lin_cfg_types.h"
#include "lin_diag_api.h"
#include "lin_cfg.h"

#ifdef LIN_MASTER_NODE
  #include "lin_diag_master.h"
#else
  #include "lin_diag_slave.h"
#endif


#ifdef LIN_VERSION_CONTROL_H
  #error "lin_version_control.h shall not be included in h-files and shall be the last file to be included in a c-file!"
#endif


/*********************************************************************
 * global macros
 *********************************************************************/

#ifdef LIN_INCLUDE_RAW_TP
  #if (LIN_DIAG3_FIFO_SIZE_MAX > 32)
    #error "Raw Fifo stack cannot hold more than 32 frames"
  #endif
#endif


/*
 * diagnostic frames
 */
#define LIN_DIAG_FRAME_LEN_MAX                              8         /* Do not change this, this is used in the driver code */

#define LIN_DIAG_FRAME_RSID_ID                           0x40u
#define LIN_DIAG_FRAME_RSID_NAK                          0x7F

#define LIN_DIAG_FRAME_NAD_NAK                           0xFF

/* PCI information */
#define LIN_DIAG_PCI_FRAMETYPE_MASK                      0xF0
#define LIN_DIAG_PCI_FRAMECOUNTER_MASK                   0x0F
#define LIN_DIAG_PCI_LENGTH_MASK                         0x0F

#define LIN_DIAG_PCI_SF                                  0x00
#define LIN_DIAG_PCI_FF                                  0x10
#define LIN_DIAG_PCI_CF                                  0x20


/* common PDU structure */
#define LIN_DIAG_FRAME_NAD_POS                              0
#define LIN_DIAG_FRAME_PCI_POS                              1

/* PDU structure for SF frames */
#define LIN_DIAG_SF_FRAME_SID_POS                           2
#define LIN_DIAG_SF_FRAME_RSID_POS                          2
#define LIN_DIAG_SF_FRAME_DATA_POS                          3
#define LIN_DIAG_SF_FRAME_DATA_LEN_MAX                      5

/* PDU structure for FF frames */
#define LIN_DIAG_FF_FRAME_LEN_POS                           2
#define LIN_DIAG_FF_FRAME_SID_POS                           3
#define LIN_DIAG_FF_FRAME_RSID_POS                          3
#define LIN_DIAG_FF_FRAME_DATA_POS                          4
#define LIN_DIAG_FF_FRAME_DATA_LEN_MAX                      4

/* PDU structure for CF frames */
#define LIN_DIAG_CF_FRAME_DATA_POS                          2
#define LIN_DIAG_CF_FRAME_DATA_LEN_MAX                      6


/* diagnostic messages */
#define LIN_DIAG_MESSAGE_SID_POS                            0

#define LIN_DIAG_FRAME_COUNTER_MAX                         16
#define LIN_DIAG_FRAME_UNUSED_BYTE                       0xFF


#define LIN_DIAG_FRAME_CHECKSUM_POS                         8

#define LIN_DIAG_NAD_GOTO_SLEEP                             0

#ifdef LIN_21
  #define LIN_MAX_RX_BUFFER_SIZE                          256
#endif /* LIN_21 */

/*
 * configuration commands
 */
#ifdef LIN_21
  #define LIN_DIAG_PCI_SAVE_CONFIGURATION                0x01
  #define LIN_DIAG_SID_SAVE_CONFIGURATION                0xB6u
  #define LIN_DIAG_SAVE_CONFIGURATION_DATA_LEN           0x00

  #define LIN_DIAG_PCI_ASSIGN_FRAMEID_RANGE              0x06
  #define LIN_DIAG_SID_ASSIGN_FRAMEID_RANGE              0xB7u
  #define LIN_DIAG_ASSIGN_FRAMEID_DATA_LEN_RANGE         0x05
#endif /* LIN_21 */

#define LIN_DIAG_PCI_ASSIGN_NAD                          0x06
#define LIN_DIAG_SID_ASSIGN_NAD                          0xB0u
#define LIN_DIAG_ASSIGN_NAD_DATA_LEN                     0x05

#define LIN_DIAG_PCI_ASSIGN_FRAMEID                      0x06
#define LIN_DIAG_SID_ASSIGN_FRAMEID                      0xB1u
#define LIN_DIAG_ASSIGN_FRAMEID_DATA_LEN                 0x05

#define LIN_DIAG_PCI_READ_BY_ID                          0x06
#define LIN_DIAG_SID_READ_BY_ID                          0xB2u
#define LIN_DIAG_READ_BY_ID_DATA_LEN                     0x05
#define LIN_DIAG_PCI_READ_BY_ID_SUPPLIER                 0x06
#define LIN_DIAG_PCI_READ_BY_ID_SERIAL                   0x05
#define LIN_DIAG_PCI_READ_BY_ID_FRAME_ID                 0x04
#define LIN_DIAG_PCI_READ_BY_ID_USER_DEF_2               0x02
#define LIN_DIAG_PCI_READ_BY_ID_USER_DEF_3               0x03
#define LIN_DIAG_PCI_READ_BY_ID_USER_DEF_4               0x04
#define LIN_DIAG_PCI_READ_BY_ID_USER_DEF_5               0x05
#define LIN_DIAG_PCI_READ_BY_ID_USER_DEF_6               0x06

#define LIN_DIAG_PCI_DATA_DUMP                           0x06
#define LIN_DIAG_SID_DATA_DUMP                           0xB4u
#define LIN_DIAG_DATA_DUMP_DATA_LEN                      0x05

#define LIN_DIAG_PCI_COND_CHANGE_NAD                     0x06
#define LIN_DIAG_SID_COND_CHANGE_NAD                     0xB3u
#define LIN_DIAG_COND_CHANGE_NAD_DATA_LEN                0x05

#define LIN_DIAG2_READ_PROD_ID                              0
#define LIN_DIAG2_READ_SERIAL_NR                            1
#define LIN_DIAG2_READ_MSG_ID_MIN                          16
#define LIN_DIAG2_READ_MSG_ID_MAX                          31
#define LIN_DIAG2_READ_USR_DEF_MIN                         32
#define LIN_DIAG2_READ_USR_DEF_MAX                         63

#define LIN_DIAG2_PCI_ACK                                0x01
#define LIN_DIAG2_PCI_NAK                                0x03

/*
 * class 2 defines
 */

/* position of error code */
#define LIN_DIAG_FRAME_SF_REQ_SID_POS                       3
#define LIN_DIAG_FRAME_SF_ERROR_CODE_POS                    4

/* special class 2 nads */
#define LIN_DIAG_NAD_MIN                                    1
#ifdef LIN_21
  #define LIN_DIAG_NAD_MAX                                125
#else
  #define LIN_DIAG_NAD_MAX                                126
#endif /* LIN_21 */

#define LIN_DIAG_RSID_READ_BY_ID                         0xF2
#define LIN_DIAG_CONFIG_SID_MIN                          0xB0
#ifdef LIN_21
  #define LIN_DIAG_CONFIG_SID_MAX                          0xB7
#else
  #define LIN_DIAG_CONFIG_SID_MAX                          0xB4
#endif /* LIN_21 */
#define LIN_DIAG_NAK_ERROR_CODE                          0x12



/*********************************************************************
 * diagnostic control variables
 *********************************************************************/

#ifdef LIN_INCLUDE_COOKED_TP

#ifdef LIN_21
  #ifdef LIN_MASTER_NODE
    /** \brief enumeration of the states of the Master node transmission handler */
    typedef enum
    {
      LIN_STATUS_IDLE,
      LIN_STATUS_TX_FUNCTIONAL_ACTIVE,
      LIN_STATUS_TX_PHYSICAL_ACTIVE,
      LIN_STATUS_RX_PHYSICAL_ACTIVE,
      LIN_STATUS_INTERLEAVED_TX_FUNC_DURING_TX_PHYS,
      LIN_STATUS_INTERLEAVED_RX_FUNC_DURING_RX_PHYS
    } l_TPState_t;
  
    typedef enum
    {
      LIN_IFC_STATUS_IDLE,
      LIN_IFC_STATUS_TX_FUNCTIONAL_ACTIVE,
      LIN_IFC_STATUS_TX_PHYSICAL_ACTIVE,
      LIN_IFC_STATUS_RX_PHYSICAL_ACTIVE,
      LIN_IFC_STATUS_INTERLEAVED_TX_FUNC_DURING_TX_PHYS,
      LIN_IFC_STATUS_INTERLEAVED_RX_FUNC_DURING_RX_PHYS
    } l_ifcTPState_t;
  #else
    /** \brief enumeration of the states of the Slave node transmission handler */
    typedef enum
    {
      LIN_STATUS_IDLE,
      LIN_STATUS_RX_FUNCTIONAL_REQUEST,
      LIN_STATUS_RX_PHYSICAL_REQUEST,
      LIN_STATUS_TX_PHYSICAL_RESPONSE
    } l_TPState_t;
  
    typedef enum
    {
      LIN_IFC_STATUS_IDLE,
      LIN_IFC_STATUS_RX_FUNCTIONAL_REQUEST,
      LIN_IFC_STATUS_RX_PHYSICAL_REQUEST,
      LIN_IFC_STATUS_TX_PHYSICAL_RESPONSE
    } l_ifcTPState_t;
  #endif /* LIN_MASTER_NODE */

   /*
    * bits for TP diagnostic timeout flags
    */
   #define LIN_COOKED_N_AS_TIMEOUT             0x01
   #define LIN_COOKED_N_CR_TIMEOUT             0x02
#endif /* LIN_21 */

   /*
    * bits for common diagnostic flags
    */
   #define LIN_COOKED_TX_READY                    0x04u
#ifdef LIN_21
   #define LIN_ORIGINATOR_COOKED_TX               0x80
#else
   #define LIN_ORIGINATOR_COOKED_TX               0x40
#endif /* LIN_21 */

   /*
    * bits for dedicated cooked tp diagnostic
    * flags
    */
   #define LIN_COOKED_TX_SUCCESS                  0x01
   #define LIN_COOKED_TX_ERROR                    0x02
   #define LIN_COOKED_RX_LISTENING                0x04
   #define LIN_COOKED_RX_ACTIVE                   0x08
   #define LIN_COOKED_RX_SUCCESS                  0x10
   #define LIN_COOKED_RX_ERROR                    0x20
#ifdef LIN_21
   #define LIN_COOKED_RX_WRONG_SN                 0x40
#endif /* LIN_21 */


   #define LIN_COOKED_TX_FLAG_MASK                0x03u
   #define LIN_COOKED_RX_FLAG_MASK                0xfcu

   /*
    * mapping of variable access, depending on number
    * of interfaces
    */
   #ifdef __MORE_IFCS__
     #define LIN_COOKED_TX_LENGTH(ifc)          l_cookedTxLength[ifc]
     #define LIN_COOKED_TX_BYTES_SENT(ifc)      l_cookedTxBytesSent[ifc]
     #define LIN_COOKED_TX_FRAME_COUNTER(ifc)   l_cookedTxFrameCounter[ifc]
     #define LIN_COOKED_TX_NAD(ifc)             l_cookedTxNad[ifc]
     #define LIN_COOKED_TX_BUFFER(ifc)          l_cookedTxBuffer[ifc]

     #define LIN_COOKED_RX_BYTES_EXPECTED(ifc)  l_cookedRxBytesExpected[ifc]
     #define LIN_COOKED_RX_FRAME_COUNTER(ifc)   l_cookedRxFrameCounter[ifc]
     #define LIN_COOKED_RX_BUFFER(ifc)          l_cookedRxBuffer[ifc]
     #define LIN_COOKED_RX_NAD(ifc)             l_cookedRxNad[ifc]
     #define LIN_COOKED_RX_LENGTH(ifc)          l_cookedRxLength[ifc]

     #define LIN_COOKED_TP_FLAGS(ifc)           l_cookedTpFlags[ifc]
     #ifdef LIN_21
       #define LIN_COOKED_TP_TIMEOUT_FLAGS(ifc)   l_cookedTpTimeoutFlags[ifc]
     #endif /* LIN_21 */

   #else
     #define LIN_COOKED_TX_LENGTH(ifc)          l_cookedTxLength
     #define LIN_COOKED_TX_BYTES_SENT(ifc)      l_cookedTxBytesSent
     #define LIN_COOKED_TX_FRAME_COUNTER(ifc)   l_cookedTxFrameCounter
     #define LIN_COOKED_TX_NAD(ifc)             l_cookedTxNad
     #define LIN_COOKED_TX_BUFFER(ifc)          l_cookedTxBuffer

     #define LIN_COOKED_RX_BYTES_EXPECTED(ifc)  l_cookedRxBytesExpected
     #define LIN_COOKED_RX_FRAME_COUNTER(ifc)   l_cookedRxFrameCounter
     #define LIN_COOKED_RX_BUFFER(ifc)          l_cookedRxBuffer
     #define LIN_COOKED_RX_NAD(ifc)             l_cookedRxNad
     #define LIN_COOKED_RX_LENGTH(ifc)          l_cookedRxLength

     #define LIN_COOKED_TP_FLAGS(ifc)           l_cookedTpFlags
     #ifdef LIN_21
       #define LIN_COOKED_TP_TIMEOUT_FLAGS(ifc)   l_cookedTpTimeoutFlags
     #endif /* LIN_21 */
   #endif

   #ifdef __MORE_IFCS__
     extern l_u8                l_cookedTpFlags[LIN_IFC_NR_MAX];
     #ifdef LIN_21
       extern l_u8                l_cookedTpTimeoutFlags[LIN_IFC_NR_MAX];
     #endif /* LIN_21 */
   #else
     extern l_u8                l_cookedTpFlags;
     #ifdef LIN_21
       extern l_u8                l_cookedTpTimeoutFlags;
     #endif /* LIN_21 */
   #endif

#endif /* LIN_INCLUDE_COOKED_TP */


#ifdef LIN_INCLUDE_RAW_TP

  /*
   * bits for common diagnostic flags
   */
  #define LIN_RAW_TX_READY                       0x08u
  #define LIN_ORIGINATOR_RAW_TX                  0x80

  /*
   * bits for dedicated raw tp diagnostic flags
   */
  #define LIN_RAW_TX_QUEUE_FULL                  0x01u
  #define LIN_RAW_TX_QUEUE_EMPTY                 0x02u
  #define LIN_RAW_TX_ERROR                       0x04
  #define LIN_RAW_RX_QUEUE_FULL                  0x08u
  #define LIN_RAW_RX_QUEUE_EMPTY                 0x10u
  #define LIN_RAW_RX_ERROR                       0x20u

  #define LIN_RAW_TX_FLAG_MASK                   0x07u
  #define LIN_RAW_RX_FLAG_MASK                   0xf8

  #ifdef __MORE_IFCS__
    #define LIN_RAW_TX_STACK(ifc)        l_rawTxStack[ifc]
    #define LIN_RAW_TX_STACK_START(ifc)  l_rawTxStackStart[ifc]
    #define LIN_RAW_TX_STACK_END(ifc)    l_rawTxStackEnd[ifc]

    #define LIN_RAW_RX_STACK(ifc)        l_rawRxStack[ifc]
    #define LIN_RAW_RX_STACK_START(ifc)  l_rawRxStackStart[ifc]
    #define LIN_RAW_RX_STACK_END(ifc)    l_rawRxStackEnd[ifc]

    #define LIN_RAW_TP_FLAGS(ifc)        l_rawTpFlags[ifc]

  #else
    #define LIN_RAW_TX_STACK(ifc)        l_rawTxStack
    #define LIN_RAW_TX_STACK_START(ifc)  l_rawTxStackStart
    #define LIN_RAW_TX_STACK_END(ifc)    l_rawTxStackEnd

    #define LIN_RAW_RX_STACK(ifc)        l_rawRxStack
    #define LIN_RAW_RX_STACK_START(ifc)  l_rawRxStackStart
    #define LIN_RAW_RX_STACK_END(ifc)    l_rawRxStackEnd

    #define LIN_RAW_TP_FLAGS(ifc)        l_rawTpFlags
  #endif

  #ifdef __MORE_IFCS__
    extern l_u8                 l_rawTpFlags[LIN_IFC_NR_MAX];
  #else
    extern l_u8                 l_rawTpFlags;
  #endif

#endif

/*********************************************************************
 * global functions
 *********************************************************************/
#ifdef LIN_INCLUDE_COOKED_TP

  STACK extern l_bool ld_diag3CookedTxHandler (__OPTIONAL_IFC_DEFINITION_COMMA__ l_u8 *pBuffer);
  STACK extern l_bool ld_diag3CookedRxHandler (__OPTIONAL_IFC_DEFINITION_COMMA__ const l_u8 *pBuffer);
  #ifdef LIN_21
    STACK extern void l_ifc_set_TP_status(__OPTIONAL_IFC_DEFINITION_COMMA__ l_ifcTPState_t status);
  #endif
#endif

#ifdef LIN_INCLUDE_RAW_TP

  STACK extern l_bool ld_diag3RawTxHandler (__OPTIONAL_IFC_DEFINITION_COMMA__ l_u8 *pBuffer);
  STACK extern void ld_diag3RawRxHandler (__OPTIONAL_IFC_DEFINITION_COMMA__ const l_u8 *pBuffer);
  STACK extern l_bool ld_diag3RawTxFifoGet   (__OPTIONAL_IFC_DEFINITION_COMMA__ l_u8 *pBuffer);
#endif /* LIN_INCLUDE_RAW_TP */


#endif /* LIN_DIAG_H */

/*
 * eof lin_diag.h
 */


