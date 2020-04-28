/*********************************************************************
 *
 * Project:      LIN 2.1 driver suite
 *
 * File name:    lin_types.h
 *
 * Copyright:    (c) STMicroelectronics
 *
 * Author:       Giuseppe Stefano Fazio
 *
 * Description:  General type definitions of the driver
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
 * 20090420  v1.1   gsf  - BUGZILLA_540: LIN 2.1 / N_Cr_LIN timeout
 *                         and N_As_LIN timeout management missing.
 *
 *********************************************************************/

#ifndef LIN_TYPES_H
  #define LIN_TYPES_H

#define LIN_TYPES_H_MAJOR_VERSION  1
#define LIN_TYPES_H_MINOR_VERSION  31
#define LIN_TYPES_H_PATCH_VERSION  1

#define LIN_TYPES_H_STM

/*********************************************************************
 * includes
 *********************************************************************/

#include <limits.h>

#include "lin_def.h"
#include "lin_def_gen.h"
#include "lin_def_arch_include.h"
#include "lin_cfg_types.h"


#ifdef LIN_VERSION_CONTROL_H
  #error "lin_version_control.h shall not be included in h-files and shall be the last file to be included in a c-file!"
#endif


/*********************************************************************
 * global macros
 *********************************************************************/
#define LIN_TEST_BIT(A,B)  ((l_bool)((((A) & (1U << (B))) != 0) ? 1 : 0))
//#define LIN_TEST_BIT(A,B)  (!((A) & (1U << (B))))
//#define LIN_TEST_BIT(A,B)  (((A) & (1U << (B))))

#define LIN_SET_BIT(A,B)                      ((A) |= (l_u8) (1U << (B)))
#define LIN_CLEAR_BIT(A,B)               ((A) &= ((l_u8) (~(1U << (B)))))
#define LIN_BYTE_MASK                              ((1U << CHAR_BIT) - 1)
#define LIN_FRAME_LEN_MAX                                              10
/* Returns the low byte of the 32-bit value    */
#define BYTE_0(n)                              ((l_u8)((n) & (l_u8)0xFF))
/* Returns the second byte of the 32-bit value */
#define BYTE_1(n)                        ((l_u8)(BYTE_0((n) >> (l_u8)8)))
/* Returns the third byte of the 32-bit value  */
#define BYTE_2(n)                       ((l_u8)(BYTE_0((n) >> (l_u8)16)))
/* Returns high byte of the 32-bit value       */
#define BYTE_3(n)                       ((l_u8)(BYTE_0((n) >> (l_u8)24)))

/*********************************************************************
 * global typedefs
 *********************************************************************/
#ifdef LIN_MASTER_NODE
  /** \brief enumeration of the states of the state machine of the driver */
  typedef enum
  {
    LIN_STATE_IDLE,              /* Do not change this, '0' is used in init */
    LIN_STATE_SEND_SYNC,
    LIN_STATE_SEND_IDENTIFIER,
    LIN_STATE_RECEIVE_IDENTIFIER,
    LIN_STATE_SEND_DATA,
    LIN_STATE_RECEIVE_DATA,
    LIN_STATE_BUSSLEEP
  } l_ifcState_t;
#else
  /** \brief enumeration of the states of the state machine of the driver */
  typedef enum
  {
    #ifdef LIN_BAUDRATE_DETECT
      LIN_STATE_PRE_IDLE,
      LIN_STATE_PRE_SYNC,
    #endif
    LIN_STATE_IDLE,
    LIN_STATE_RECEIVE_SYNC,
    LIN_STATE_RECEIVE_IDENTIFIER,
    LIN_STATE_SEND_DATA,
    LIN_STATE_RECEIVE_DATA,
    LIN_STATE_BUSSLEEP
  } l_ifcState_t;

  /** \brief type to hold product id of slave node
   * as required by the standard
   */
  typedef struct
  {
    l_u16                    supplierId;
    l_u16                    functionId;
    l_u8                     variant;
  } l_productId_t;

#endif

/* structure to hold the ifc control function pointers */
typedef struct ifc_ctrl_hooks
{
  STACK l_bool               (*initHook) (void);
  STACK l_bool               (*connectHook) (void);
  STACK l_bool               (*disconnectHook) (void);
  STACK void                 (*txCharHook) (l_u8);
  STACK void                 (*rxFunc) (void);
  STACK void                 (*txFunc) (void);
  STACK void                 (*baudrateHook) (l_u16);
  #if (defined(LIN_SLAVE_NODE) && defined(LIN_BAUDRATE_DETECT))
    STACK void               (*baudrateCallback) (__OPTIONAL_IFC_DEFINITION_COMMA__ l_u16);
  #endif
  #ifdef LIN_PROTOCOL_SWITCH
    STACK void               (*protocolCallback) (void);
  #endif /* LIN_PROTOCOL_SWITCH */
} IFC_CTRL_HOOKS_TYPE;


/*********************************************************************
 * typedefs  for timer module
 **********************************************************************/
/**
 * \brief enumeration of all used timer channels, one channel per interface
 */
typedef enum
{
  #ifdef SCI0_USED
    LIN_TIMER_SCI0,
  #endif

  #ifdef SCI1_USED
    LIN_TIMER_SCI1,
  #endif

  #ifdef SCI2_USED
    LIN_TIMER_SCI2,
  #endif

  #ifdef SCI3_USED
    LIN_TIMER_SCI3,
  #endif

  #ifdef SCI4_USED
    LIN_TIMER_SCI4,
  #endif

  #ifdef SCI5_USED
    LIN_TIMER_SCI5,
  #endif

  #ifdef SCI6_USED
    LIN_TIMER_SCI6,
  #endif

  #ifdef SCI7_USED
    LIN_TIMER_SCI7,
  #endif

  #ifdef SCI8_USED
    LIN_TIMER_SCI8,
  #endif

  #ifdef SCI9_USED
    LIN_TIMER_SCI9,
  #endif

  LIN_TIMER_NR_MAX
} l_timer_handle;

/**
 * \brief enumeration of all used timer subchannels
 * only max one subchannel per interface is active at a time
 */
typedef enum
{
  LIN_SUBCHANNEL_NOT_USED,  /* This is 0 and also the init value after startup           */
  LIN_WAKEUP_SUBCHANNEL,
  LIN_FRAME_SUBCHANNEL,
  #ifdef LIN_SLAVE_NODE
    LIN_BUSSLEEP_SUBCHANNEL,
    #ifdef LIN_BAUDRATE_DETECT
      LIN_BAUDRATE_SUBCHANNEL,
    #endif
  #endif
  /* Begin BUGZILLA_540 */
  #if (defined(LIN_21) && defined(LIN_INCLUDE_COOKED_TP))
    LIN_N_AS_SUBCHANNEL,
    LIN_N_CR_SUBCHANNEL,
  #endif /* LIN_21 && LIN_INCLUDE_COOKED_TP */
  /* End BUGZILLA_540 */
  LIN_INVALID_SUBCHANNEL
} l_subchannel_handle;


/**
 * \brief timer control data type
 */
typedef struct timer
{
  #ifdef __MORE_IFCS__
    l_u16 Ticks[LIN_TIMER_NR_MAX] ;
    l_u8  SubChannel[LIN_TIMER_NR_MAX] ;
  #else
    l_u16 Ticks ;
    l_u8  SubChannel ;
  #endif
} TIMER ;

/*********************************************************************
 * global constants
 *********************************************************************/

#endif /* LIN_TYPES_H */

/*
 * eof lin_types.h
 */
