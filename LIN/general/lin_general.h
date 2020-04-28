/*********************************************************************
 *
 * Project:      LIN 2.1 driver suite
 *
 * File name:    lin_general.h
 *
 * Copyright:    (c) STMicroelectronics
 *
 * Author:       Giuseppe Stefano Fazio
 *
 * Description:  General definitions, constants and macros
 *               of the driver
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

/** \file lin_general.h
 */

#ifndef LIN_GENERAL_H
  #define LIN_GENERAL_H

#define LIN_GENERAL_H_MAJOR_VERSION  1
#define LIN_GENERAL_H_MINOR_VERSION 32
#define LIN_GENERAL_H_PATCH_VERSION  1

#define LIN_GENERAL_H_STM

/*********************************************************************
 * includes
 *********************************************************************/

#include "lin_cfg_types.h"
#include "lin_types.h"
#include "lin_timer.h"
#include "lin_def.h"
#include "lin_def_gen.h"
#include "lin_def_arch_include.h"

#include "lin_arch_include.h"


#ifdef LIN_VERSION_CONTROL_H
  #error "lin_version_control.h shall not be included in h-files and shall be the last file to be included in a c-file!"
#endif


/*********************************************************************
 * global macros
 *********************************************************************/


/*********************************************************************
 * General defines
 *********************************************************************/

#define __CAT__(x,y)  x##y
#define __CAT(x,y)    __CAT__(x,y)

#define ___CAT___(x,y)  x##y
#define ___CAT(x,y)    ___CAT___(x,y)

/*
 * ifc handling
 */
#define LIN_IFC_INIT_FLAG                                0x01u
#define LIN_IFC_CONNECT_FLAG                             0x02u
#define LIN_IFC_EVENT_TRIGGERED_FLAG                     0x08u
#define LIN_IFC_COLLISION_FLAG                           0x10u
#define LIN_PROTOCOL_OTHER                               0x80u

#ifdef LIN_13
  #define LIN_WAKEUP_SIG                                 0x80
  #define LIN_RECEIVE_WAKEUP_SIG                         0x02
#else
  #define LIN_WAKEUP_SIG                                 0xE0
  #define LIN_RECEIVE_WAKEUP_SIG                         0x03
#endif

#define LIN_BREAK_PATTERN                                   0
#define LIN_SYNC_PATTERN                                 0x55
#define LIN_INVALID_PATTERN                              0x66

#define LIN_STATUS_ERROR                                    1
#define LIN_STATUS_SUCCESS                                  2
#define LIN_STATUS_BUSSLEEP                                 4
#ifdef LIN_21
  #define LIN_STATUS_EVENT_TRIGGERED_FRAME_COLLISION_SOLVED 8
  #define LIN_STATUS_EVENT_TRIGGERED_FRAME_COLLISION       16
  #define LIN_STATUS_SAVE_CONFIGURATION                    32
#endif /* LIN_21 */

#define LIN_IFC_STATUS_ERROR                                1
#define LIN_IFC_STATUS_SUCCESSFUL                           2
#define LIN_IFC_STATUS_OVERRUN                              4
#define LIN_IFC_STATUS_GOTO_SLEEP                           8
#ifdef LIN_21
  #define LIN_IFC_STATUS_BUS_ACTIVITY                      16
  #define LIN_IFC_STATUS_EVENT_TRIGGERED_FRAME_COLLISION   32
  #define LIN_IFC_STATUS_SAVE_CONFIGURATION                64
#endif /* LIN_21 */
#define LIN_IFC_STATUS_FRAME_ID_POS                         8

#define LIN_DIAG_TX_FLAG                                    1u
#define LIN_DIAG_RX_FLAG                                    2

/*
 * signal handling
 */
#define LIN_BYTE_ARRAY_FLAG_POS                  (CHAR_BIT-1)
#define LIN_SIGNAL_SIZE_MASK ((1u << LIN_BYTE_ARRAY_FLAG_POS) - 1u)

#define LIN_SCALAR_SIGNAL_SIZE_MAX                         16

#define LIN_BOOL_SIGNAL_SIZE                                1
#define LIN_BYTE_SIGNAL_SIZE                         CHAR_BIT

#define LIN_TRANSFER_SEND                                   1
#define LIN_TRANSFER_RECEIVE                                2

/*
 * frame handling
 */
#define LIN_INVALID_PROT_ID                              0xFFu
#define LIN_INVALID_ID_API                               0x40
#define LIN_FRAME_UNUSED_BYTE                            0xFF
#define LIN_FRAME_TYPE_MASK                                 3
#define LIN_UNCON_FRAME                                     1
#define LIN_EVT_TRIG_FRAME                                  2
#define LIN_SPORADIC_FRAME                                  3
#define LIN_PREDEF_DIAG_FRAME                               4
#define LIN_INVALID_MAPPED_FRAME                         0xffu
#define LIN_PAR_BIT1_POS                                    6
#define LIN_PAR_BIT2_POS                                    7

#define LIN_FRAME_ID_POS                                    0
#define LIN_FRAME_DATA_POS                                  1

#define LIN_FRAME_ID_MASK                                0x3F
#define LIN_CHECKSUM_CLASSIC                                0
#define LIN_CHECKSUM_ENHANCED                               1
#define LIN_CHECKSUM_BOTH                                   2

#define LIN_FRAME_PUBLISH                                0x80
#define LIN_FRAME_RECEIVE                                0x40

#define LIN_MASTER_REQ_FRAME_ID                            60
#define LIN_SLAVE_RESP_FRAME_ID                           125
#define LIN_USER_DEFINED_FRAME_ID                         254
#define LIN_RESERVED_FRAME_ID                             191

#define LIN_IDENTIFIER_LEN                                  1
#define LIN_CHECKSUM_LEN                                    1

/*
 * flag handling
 */
#define LIN_FLAGS_PER_SIGNAL                                2
#define LIN_FLAGS_PER_FRAME                                 2
#define LIN_SIGNAL_RX_FLAG                                  0
#define LIN_SIGNAL_TX_FLAG                                  1
#define LIN_FRAME_RX_FLAG                                   0
#define LIN_FRAME_TX_FLAG                                   1


/*
 * baudrate detection
 */
#define LIN_BAUDRATE_DETERMINED                          0x01
#define LIN_BAUDRATE_KEEP_SILENT                 ((l_u8) 0x02)
#define LIN_BAUDRATE_WAKEUP                              0x04

#ifdef LIN_DEBUG
  #define LIN_ERROR_SUCCESS                              0x0001
  #define LIN_ERROR_RESPONSE_ERROR                       0x0002
  #define LIN_ERROR_READBACK_ERROR                       0x0004
  #define LIN_ERROR_FRAME_TIMER                          0x0010
  #define LIN_ERROR_BUSSLEEP                             0x0020
  #define LIN_ERROR_SERIAL_ERROR                         0x0040
  #define LIN_ERROR_BREAK_ERROR                          0x0080
  #define LIN_ERROR_CHECKSUM_ERROR                       0x0100
#endif /* LIN_DEBUG */


/* Access to the interface data */
#ifdef __MORE_IFCS__

  #define INIT_ARCH(IFC)              (ARRAY_IFCCtrlHooks[IFC].initHook)()
  #define CONNECT_ARCH(IFC)           (ARRAY_IFCCtrlHooks[IFC].connectHook)()
  #define DISCONNECT_ARCH(IFC)        (ARRAY_IFCCtrlHooks[IFC].disconnectHook)()
  #define TXCHAR_ARCH(IFC,A)          (ARRAY_IFCCtrlHooks[IFC].txCharHook)(A)
  #define RX_ARCH(IFC)                (ARRAY_IFCCtrlHooks[IFC].rxFunc)()
  #define TX_ARCH(IFC)                (ARRAY_IFCCtrlHooks[IFC].txFunc)()
  #define CHANGEBAUDRATE_ARCH(IFC,A)  (ARRAY_IFCCtrlHooks[IFC].baudrateHook)(A)
  #define BAUDRATECALLBACK(IFC,A)     (ARRAY_IFCCtrlHooks[IFC].baudrateCallback)(IFC,A)
  #define PROTOCOLCALLBACK(IFC)       (ARRAY_IFCCtrlHooks[IFC].protocolCallback)()

  #define IFCCTRL_NORMALBAUDRATE(ifc)      l_ifcNormalBaudrate[(ifc)]
  #define IFCCTRL_FRAMETIMERVALUE(ifc)     l_ifcFrameTimerValue[(ifc)]
  #define IFCCTRL_STATUSWORD(ifc)          l_ifcStatusWord[(ifc)]
  #if (defined(LIN_INCLUDE_COOKED_TP) && defined(LIN_21))
    #define IFCCTRL_STATUSTPWORD(ifc)      l_ifcStatusTPWord[(ifc)]
  #endif /* defined(LIN_INCLUDE_COOKED_TP) && defined(LIN_21) */

  #define IFCCTRL_STATE(ifc)               l_ifcState[(ifc)]
  #define IFCCTRL_FLAGS(ifc)               l_ifcFlags[(ifc)]
  #define IFCCTRL_SERIALERROR(ifc)         l_ifcSerialError[(ifc)]
  #define IFCCTRL_DIAGFLAGS(ifc)           l_ifcDiagFlags[(ifc)]

  #define TRANSFERCTRL_BYTESSENT(ifc)      l_transferBytesSent[(ifc)]
  #define TRANSFERCTRL_BYTESTOSEND(ifc)    l_transferBytesToSend[(ifc)]
  #define TRANSFERCTRL_CHARSENT(ifc)       l_transferCharSent[(ifc)]
  #define TRANSFERCTRL_STATE(ifc)          l_transferState[(ifc)]
  #define TRANSFERCTRL_RETRIES(ifc)        l_transferRetries[(ifc)]
  #define TRANSFERCTRL_FRAME(ifc)          l_transferFrame[(ifc)]
  #define TRANSFERCTRL_BUFFER(ifc)         l_transferBuffer[(ifc)]
  #define TRANSFERCTRL_CHECKSUM(ifc)       l_checksum[(ifc)]


  #ifdef LIN_MASTER_NODE
    #define IFCCTRL_SCHEDULE(ifc)           l_ifcSchedule[(ifc)]
    #define IFCCTRL_ENTRY(ifc)              l_ifcEntry[(ifc)]
    #ifdef LIN_21
      #define IFCCTRL_SCHEDULE_AFTER_COLLISION(ifc)           l_ifcScheduleAfterCollision[(ifc)]
      #define IFCCTRL_ENTRY_AFTER_COLLISION(ifc)              l_ifcEntryAfterCollision[(ifc)]
    #endif /* LIN_21 */
    #define IFCCTRL_DELAY(ifc)              l_ifcDelay[(ifc)]
    #define IFCCTRL_GOTOSLEEPREQ(ifc)       l_ifcGotoSleepReq[(ifc)]
    #define IFCCTRL_GOTOSLEEPSCHEDULE(ifc)  l_ifcGotoSleepSchedule[(ifc)]
    #define IFCCTRL_GOTOSLEEPENTRY(ifc)     l_ifcGotoSleepEntry[(ifc)]
  #else
    #ifdef LIN_BAUDRATE_DETECT
      #define IFCCTRL_BAUDRATEFLAGS(ifc)      l_ifcBaudrateFlags[(ifc)]
    #endif /* LIN_BAUDRATE_DETECT */
  #endif /* LIN_MASTER_NODE */

#else /* Only single IFC */

  #define INIT_ARCH(IFC)              __CAT(l_ifc_initArch_,__SINGLE_IFC__)()
  #define CONNECT_ARCH(IFC)           __CAT(l_ifc_connectArch_,__SINGLE_IFC__)()
  #define DISCONNECT_ARCH(IFC)        __CAT(l_ifc_disconnectArch_,__SINGLE_IFC__)()
  #define TXCHAR_ARCH(IFC,A)          __CAT(l_txCharArch_,__SINGLE_IFC__)(A)
  #define RX_ARCH(IFC)                __CAT(l_ifc_rx_,__SINGLE_IFC__)()
  #define TX_ARCH(IFC)                __CAT(l_ifc_tx_,__SINGLE_IFC__)()
  #define CHANGEBAUDRATE_ARCH(IFC,A)  __CAT(l_changeBaudrateArch_,__SINGLE_IFC__)(A)
  #define BAUDRATECALLBACK(IFC,A)     __CAT(l_baudrateCallback_,__SINGLE_IFC__)(A)
  #define PROTOCOLCALLBACK(IFC)       __CAT(l_protocolCallback_,__SINGLE_IFC__)()

  #ifdef LIN_USE_HEADERDETECTION
    #define SET_HEADERDETECTION_ARCH(IFC,A) __CAT(l_ifc_set_HeaderdetectionArch_,__SINGLE_IFC__)(A)
  #else
    #define SET_HEADERDETECTION_ARCH(IFC,A)
  #endif

  #define IFCCTRL_NORMALBAUDRATE(ifc)      l_ifcNormalBaudrate
  #define IFCCTRL_FRAMETIMERVALUE(ifc)     l_ifcFrameTimerValue
  #define IFCCTRL_STATUSWORD(ifc)          l_ifcStatusWord
  #if (defined(LIN_INCLUDE_COOKED_TP) && defined(LIN_21))
    #define IFCCTRL_STATUSTPWORD(ifc)      l_ifcStatusTPWord
  #endif /* defined(LIN_INCLUDE_COOKED_TP) && defined(LIN_21) */

  #define IFCCTRL_STATE(ifc)               l_ifcState
  #define IFCCTRL_FLAGS(ifc)               l_ifcFlags
  #define IFCCTRL_SERIALERROR(ifc)         l_ifcSerialError
  #define IFCCTRL_DIAGFLAGS(ifc)           l_ifcDiagFlags

  #define TRANSFERCTRL_BYTESSENT(ifc)      l_transferBytesSent
  #define TRANSFERCTRL_BYTESTOSEND(ifc)    l_transferBytesToSend
  #define TRANSFERCTRL_CHARSENT(ifc)       l_transferCharSent
  #define TRANSFERCTRL_STATE(ifc)          l_transferState
  #define TRANSFERCTRL_RETRIES(ifc)        l_transferRetries
  #define TRANSFERCTRL_FRAME(ifc)          l_transferFrame
  #define TRANSFERCTRL_BUFFER(ifc)         l_transferBuffer
  #define TRANSFERCTRL_CHECKSUM(ifc)       l_checksum

  #ifdef LIN_MASTER_NODE
    #define IFCCTRL_SCHEDULE(ifc)           l_ifcSchedule
    #define IFCCTRL_ENTRY(ifc)              l_ifcEntry
    #ifdef LIN_21
      #define IFCCTRL_SCHEDULE_AFTER_COLLISION(ifc)           l_ifcScheduleAfterCollision
      #define IFCCTRL_ENTRY_AFTER_COLLISION(ifc)              l_ifcEntryAfterCollision
    #endif /* LIN_21 */
    #define IFCCTRL_DELAY(ifc)              l_ifcDelay
    #define IFCCTRL_GOTOSLEEPREQ(ifc)       l_ifcGotoSleepReq
    #define IFCCTRL_GOTOSLEEPSCHEDULE(ifc)  l_ifcGotoSleepSchedule
    #define IFCCTRL_GOTOSLEEPENTRY(ifc)     l_ifcGotoSleepEntry
  #else
    #ifdef LIN_BAUDRATE_DETECT
      #define IFCCTRL_BAUDRATEFLAGS(ifc)      l_ifcBaudrateFlags
    #endif /* LIN_BAUDRATE_DETECT */
  #endif /* LIN_MASTER_NODE */

#endif

/*** Macros for parameter checking, can be disabled for production ***/

/* Check of the interface number */
#if (defined LIN_DEVELOPMENT) && (defined __MORE_IFCS__)
  #define __CHECK_INTERFACE_VOID__(ifc)      \
          if (ifc >= LIN_IFC_NR_MAX)         \
          {                                  \
            return ;                         \
          }
  #define __CHECK_INTERFACE_VALUE__(ifc,x)   \
          if (ifc >= LIN_IFC_NR_MAX)         \
          {                                  \
            return x ;                       \
          }
#else
  #define __CHECK_INTERFACE_VOID__(ifc)
  #define __CHECK_INTERFACE_VALUE__(ifc,x)
#endif


/*********************************************************************
 * global typedefs
 *********************************************************************/

/*********************************************************************
 * global constants
 *********************************************************************/

/*********************************************************************
 * global variables
 *********************************************************************/

extern const l_u16            l_speed[LIN_IFC_NR_MAX+1];

#ifdef __MORE_IFCS__
  /** \brief function pointers to interface control functions */ 
  extern const IFC_CTRL_HOOKS_TYPE ARRAY_IFCCtrlHooks[LIN_IFC_NR_MAX];

  /** \brief normal baudrate of the interface */
  LIN_ZERO_PAGE7 extern l_u16      l_ifcNormalBaudrate[LIN_IFC_NR_MAX];

  /** \brief maximum duration of frame (for supervision of timeslot) */
  extern l_u8                      l_ifcFrameTimerValue[LIN_IFC_NR_MAX];

  /** \brief the interface status word (to be read by application) */
  LIN_ZERO_PAGE3 extern l_u16      l_ifcStatusWord[LIN_IFC_NR_MAX];

  #if (defined(LIN_INCLUDE_COOKED_TP) && defined(LIN_21))
    /** \brief the interface status word (to be read by application) */
    LIN_ZERO_PAGE3 extern l_u16      l_ifcStatusTPWord[LIN_IFC_NR_MAX];
  #endif /* defined(LIN_INCLUDE_COOKED_TP) && defined(LIN_21) */

  /** internal state of the driver, controls the behaviour of
   * the state machine
   */
  LIN_ZERO_PAGE1 extern l_u8       l_ifcState[LIN_IFC_NR_MAX];

  /** general interface control flags */
  LIN_ZERO_PAGE4 extern l_u8       l_ifcFlags[LIN_IFC_NR_MAX];

  /** flag indicating serial error */
  LIN_ZERO_PAGE9 extern l_u8       l_ifcSerialError[LIN_IFC_NR_MAX];

  /** diagnostic flags (read by the application) */
  extern l_u8                      l_ifcDiagFlags[LIN_IFC_NR_MAX];

  /** number of bytes sent/received in current transfer */
  LIN_ZERO_PAGE2 extern l_u8       l_transferBytesSent[LIN_IFC_NR_MAX];

  /** number of bytes to send/receive in current transfer */
  LIN_ZERO_PAGE5 extern l_u8       l_transferBytesToSend[LIN_IFC_NR_MAX];

  /** last character sent (for readback check) */
  extern l_u8                      l_transferCharSent[LIN_IFC_NR_MAX];

  /** state of the current transfer (different from overall state) */
  extern l_u8                      l_transferState[LIN_IFC_NR_MAX];

  /** number of retries sending the wakeup signal*/
  LIN_ZERO_PAGE8 extern l_u8       l_transferRetries[LIN_IFC_NR_MAX];

  /** frame index of frame currently transferred */
  LIN_ZERO_PAGE6 extern l_u16      l_transferFrame[LIN_IFC_NR_MAX];

  /** buffer holding the frame currently transmitted/received */
  LIN_ZERO_PAGE10 extern l_u8      l_transferBuffer[LIN_IFC_NR_MAX][10];

  /** checksum value for received frames */
  extern l_u8                      l_checksum[LIN_IFC_NR_MAX];

  #ifdef LIN_MASTER_NODE
    /** \brief baudrate used for break generation */
    extern l_u16                     l_ifcBreakBaudrate[LIN_IFC_NR_MAX];

    /** index of active schedule table */
    extern l_u16                   l_ifcSchedule[LIN_IFC_NR_MAX];

    /** index of current entry in schedule table */
    extern l_u8                    l_ifcEntry[LIN_IFC_NR_MAX];

    #ifdef LIN_21
	   /** index of collision resolving schedule table */
       extern l_u16                   l_ifcScheduleAfterCollision[LIN_IFC_NR_MAX];

       /** index of entry in schedule table after collision */
       extern l_u8                    l_ifcEntryAfterCollision[LIN_IFC_NR_MAX];
    #endif /* LIN_21 */

    /** delay value for current frame slot */
    extern l_u8                    l_ifcDelay[LIN_IFC_NR_MAX];

    /** flag to indicate goto sleep request */
    extern l_u8                    l_ifcGotoSleepReq[LIN_IFC_NR_MAX];

    /** index of schedule table interrupted by goto sleep request */
    extern l_u16                   l_ifcGotoSleepSchedule[LIN_IFC_NR_MAX];

    /** current entry of interupted schedule table */
    extern l_u8                    l_ifcGotoSleepEntry[LIN_IFC_NR_MAX];

  #else
    #ifdef LIN_BAUDRATE_DETECT
       /** flags to control the baudrate detection feature */
       extern l_u8                 l_ifcBaudrateFlags[LIN_IFC_NR_MAX];
    #endif /* LIN_BAUDRATE_DETECT */
  #endif

#else
  /** \brief normal baudrate of the interface */
  LIN_ZERO_PAGE7 extern l_u16      l_ifcNormalBaudrate;

  /** \brief maximum duration of frame (for supervision of timeslot) */
  extern l_u8                      l_ifcFrameTimerValue;

  /** \brief the interface status word (to be read by application) */
  LIN_ZERO_PAGE3 extern l_u16      l_ifcStatusWord;

  #if (defined(LIN_INCLUDE_COOKED_TP) && defined(LIN_21))
    /** \brief the interface status word (to be read by application) */
    LIN_ZERO_PAGE3 extern l_u16      l_ifcStatusTPWord;
  #endif /* defined(LIN_INCLUDE_COOKED_TP) && defined(LIN_21) */

  /** \brief internal state of the driver
   * controls the behaviour of
   * the state machine
   */
  LIN_ZERO_PAGE1 extern l_u8       l_ifcState;

  /** \brief general interface control flags */
  LIN_ZERO_PAGE4 extern l_u8       l_ifcFlags;

  /** \brief flag indicating serial error */
  LIN_ZERO_PAGE9 extern l_u8       l_ifcSerialError;


  /** \brief number of bytes sent/received in current transfer */
  LIN_ZERO_PAGE2 extern l_u8       l_transferBytesSent;

  /** \brief number of bytes to send/receive in current transfer */
  LIN_ZERO_PAGE5 extern l_u8       l_transferBytesToSend;

  /** \brief number of retries sending the wakeup signal*/
  LIN_ZERO_PAGE8 extern l_u8       l_transferRetries;


  /** \brief buffer holding the frame currently transmitted/received */
  LIN_ZERO_PAGE10 extern l_u8      l_transferBuffer[10];


 #ifdef __COSMIC__       
  /** \brief diagnostic flags (read by the application) */                                                         
@tiny extern l_u8                      l_ifcDiagFlags;

  /** \brief checksum value for received frames */
@tiny extern l_u8                      l_checksum;

  /** \brief frame index of frame currently transferred */
@tiny  extern l_u16                    l_transferFrame;

 /** \brief state of the current transfer (different from overall state) */
@tiny extern l_u8                      l_transferState;

/** \brief last character sent (for readback check) */
@tiny extern l_u8                      l_transferCharSent;

#elif __RAISONANCE__
  /** \brief diagnostic flags (read by the application) */ 
 extern l_u8                      l_ifcDiagFlags;

/** \brief checksum value for received frames */
 extern l_u8                      l_checksum;

/** \brief frame index of frame currently transferred */
  extern l_u16                    l_transferFrame;

/** \brief state of the current transfer (different from overall state) */
 extern l_u8                      l_transferState;

/** \brief last character sent (for readback check) */
 extern l_u8                      l_transferCharSent;

#elif __IAR__
/** \brief diagnostic flags (read by the application) */ 
 extern l_u8                      l_ifcDiagFlags;

/** \brief checksum value for received frames */
 extern l_u8                      l_checksum;

/** \brief frame index of frame currently transferred */
  extern l_u16                    l_transferFrame;

/** \brief state of the current transfer (different from overall state) */
 extern l_u8                      l_transferState;

/** \brief last character sent (for readback check) */
 extern l_u8                      l_transferCharSent;
#endif


  #ifdef LIN_MASTER_NODE
    /** \brief baudrate used for break generation */
    extern l_u16                     l_ifcBreakBaudrate;

    /** \brief index of active schedule table */
    extern l_u16                   l_ifcSchedule;

    /** \brief index of current entry in schedule table */
    extern l_u8                    l_ifcEntry;

    #ifdef LIN_21
       /** \brief index of collision resolving schedule table */
       extern l_u16                   l_ifcScheduleAfterCollision;

       /** \brief index of entry in schedule table after collision */
       extern l_u8                    l_ifcEntryAfterCollision;
    #endif /* LIN_21 */

    /** \brief delay value for current frame slot */
    extern l_u8                    l_ifcDelay;

    /** \brief flag to indicate goto sleep request */
    extern l_u8                    l_ifcGotoSleepReq;

    /** \brief index of schedule table interrupted by goto sleep request */
    extern l_u16                   l_ifcGotoSleepSchedule;

    /** \brief current entry of interupted schedule table */
    extern l_u8                    l_ifcGotoSleepEntry;
  #else
    #ifdef LIN_BAUDRATE_DETECT
       /** \brief flags to control the baudrate detection feature */
       extern l_u8                 l_ifcBaudrateFlags;
    #endif /* LIN_BAUDRATE_DETECT */
  #endif

#endif

#ifdef LIN_DEBUG
  /** \brief debug variable */
  extern l_u16 l_errorState[];
#endif /* LIN_DEBUG */

/*********************************************************************
 * global functions
 *********************************************************************/
STACK extern l_irqmask l_sys_irq_disable (void) REENTRANT;
STACK extern void      l_sys_irq_restore (l_irqmask irqMask) REENTRANT;

STACK extern void l_initTransferCtrl (__OPTIONAL_IFC_DEFINITION__);

#ifndef LIN_13
  STACK extern void l_ifcSetStatus (__OPTIONAL_IFC_DEFINITION_COMMA__ l_u8 status, l_u8 frame) ;
#else
  #ifdef __MORE_IFCS__
    #define l_ifcSetStatus(ifc,A,B)
  #else
    #define l_ifcSetStatus(A,B)
  #endif
#endif

STACK extern void l_sendByte         (__OPTIONAL_IFC_DEFINITION_COMMA__ l_u8 sendByte);

STACK extern l_bool l_sys_init_arch  (void);

STACK extern void   l_evaluateTimeout(__OPTIONAL_TIMER_DEFINITION__);
STACK extern void   l_setBaudrate    (__OPTIONAL_IFC_DEFINITION_COMMA__ l_u16 baudrate);



#ifdef LIN_DEBUG

  STACK extern void l_dbgMasterSendBuffer   (l_u8 *pBuffer, l_u8 size);

  STACK extern void l_dbgMasterReceiveBuffer(l_u8 *pBuffer, l_u8 size);
  STACK extern void l_dbgSlaveSendBuffer    (l_u8 *pBuffer, l_u8 size);
  STACK extern void l_dbgSlaveReceiveBuffer (l_u8 *pBuffer, l_u8 size);

#endif

#ifndef LIN_MASTER_NODE
  #ifdef LIN_INCLUDE_PID_PARITY_CHECK
    STACK extern l_u8 l_makeProtId(l_u8 identifier);
  #endif
  STACK extern l_frame_handle l_getFrameIndex (__OPTIONAL_IFC_DEFINITION_COMMA__ l_u8 id);
#endif
  
#endif /* LIN_GENERAL_H */
/*
 * eof lin_general.h
 */
