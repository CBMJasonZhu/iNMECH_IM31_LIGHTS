/*********************************************************************
 *
 * Project:      LIN 2.1 driver suite
 *
 * File name:    lin.h
 *
 * Copyright:    (c) STMicroelectronics
 *
 * Author:       Giuseppe Stefano Fazio
 *
 * Description:  LIN API include file for applications
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
 * 20090420  v1.1   gsf  - BUGZILLA_469: Missing "#ifndef LIN_13" in 
 *                         the lin.h file.
 * 20100430  v1.2   JKh  - BUGZILLA_1097: Software Timer: Missing defines for l_timer_tick_N_As() 
 *                         and l_timer_tick_N_Cr()  
 *
 *********************************************************************/

/** \file lin.h
 */

#ifndef LIN_H
  #define LIN_H

#define LIN_H_MAJOR_VERSION  1
#define LIN_H_MINOR_VERSION  31
#define LIN_H_PATCH_VERSION  1

#define LIN_H_STM

/*********************************************************************
 * includes
 *********************************************************************/

#include "lin_cfg_types.h"
#include "lin_cfg.h"

#include "lin_def.h"
#include "lin_def_arch_include.h"

#include "lin_diag_api.h"


#ifdef LIN_VERSION_CONTROL_H
  #error "lin_version_control.h shall not be included in h-files and shall be the last file to be included in a c-file!"
#endif


/*********************************************************************
 *
 * LIN API (non diagnostic)
 *
 *********************************************************************/

/*********************************************************************
 * initialization
 *********************************************************************/

STACK extern l_bool l_sys_init       (void) REENTRANT;

#ifdef LIN_21
  STACK extern l_bool l_ifc_init         (l_ifc_handle ifc) REENTRANT;
#else
  STACK extern void l_ifc_init         (l_ifc_handle ifc) REENTRANT;
#endif /* LIN_21 */

#ifndef LIN_21
  STACK extern l_bool l_ifc_connect    (l_ifc_handle ifc) REENTRANT;
  STACK extern l_bool l_ifc_disconnect (l_ifc_handle ifc) REENTRANT;
#endif /* !LIN_21 */

/*********************************************************************
 * interface related API functions
 *********************************************************************/

STACK extern l_u16 l_ifc_ioctl       (l_ifc_handle ifc, l_ioctl_op operation, void *pParams) REENTRANT;


#if (!defined(LIN_13) || defined(LIN_INCLUDE_2x_SLEEP_MODE_API))
  STACK extern void l_ifc_wake_up    (l_ifc_handle ifc) REENTRANT;
#endif

/* Begin BUGZILLA_469 */
#ifndef LIN_13
  STACK extern l_u16 l_ifc_read_status (l_ifc_handle ifc) REENTRANT;
#endif
/* End BUGZILLA_469 */
STACK extern void l_ifc_rx           (l_ifc_handle ifc);

STACK extern void l_ifc_tx           (l_ifc_handle ifc);


/*********************************************************************
 * timer handling, implemented in lin_timer.c
 *********************************************************************/

#ifdef LIN_USE_HARDWARE_TIMER
  STACK extern void l_timerISR   (void) REENTRANT;
#else  /* Software timer */
  STACK extern void l_timer_tick (void) REENTRANT;
  /* Begin BUGZILLA_1097 */
  #if (defined(LIN_21) && defined(LIN_INCLUDE_COOKED_TP))
   STACK extern void l_timer_tick_N_As (void) REENTRANT;
	 STACK extern void l_timer_tick_N_Cr (void) REENTRANT;
  /* End BUGZILLA_1097 */
	#endif	
#endif


/*********************************************************************
 * master node specific API
 *********************************************************************/

#ifdef LIN_MASTER_NODE

  #define LIN_IOCTL_DRIVER_STATE           0u

  STACK extern l_u8 l_sch_tick       (l_ifc_handle ifc) REENTRANT;

  STACK extern void l_sch_set        (l_ifc_handle ifc, l_schedule_handle schedule, l_u8 entry) REENTRANT;

  #if (!defined(LIN_13) || defined(LIN_INCLUDE_2x_SLEEP_MODE_API))
    STACK extern void l_ifc_goto_sleep (l_ifc_handle ifc) REENTRANT;
  #endif

  /*********************************************************************
   * STMicroelectronics extensions for master node
   *********************************************************************/

  #ifdef LIN_USE_COOLING

    STACK extern void l_ifc_set_cooling_break (l_ifc_handle ifc, l_bool on) REENTRANT;

  #endif

#endif /* LIN_MASTER_NODE */


/*********************************************************************
 * slave node specific API
 *********************************************************************/

#ifdef LIN_SLAVE_NODE

  /* l_ifc is currently unused */
  #define l_ifc_aux(ifc)

  /*********************************************************************
   * STMicroelectronics extensions for slave node
   *********************************************************************/

  #ifdef LIN_BAUDRATE_DETECT

    STACK extern void l_change_baudrate (l_ifc_handle ifc, l_u16 baudrate) REENTRANT;

  #endif

  #ifndef LIN_13
    /**
     * type used to present frame and message id to the application
     */
    typedef struct
    {
      l_u16 messageId;
      l_u8  frameId;
      l_u8  frameIndex;
    } l_frameMessageId_t;
  #endif

  #define LIN_IOCTL_DRIVER_STATE           0u
  #define LIN_IOCTL_READ_FRAME_ID          1u
  #define LIN_IOCTL_READ_MESSAGE_ID        2u
  #define LIN_IOCTL_READ_FRAME_ID_BY_INDEX 3u
  #define LIN_IOCTL_SET_FRAME_ID           4u
  #define LIN_IOCTL_FORCE_BUSSLEEP         5u
  #define LIN_IOCTL_SET_VARIANT_ID         6u
  #define LIN_IOCTL_READ_VARIANT_ID        7u
  #define LIN_IOCTL_READ_CONFIG_FLAGS      8u
  #ifdef LIN_INCLUDE_DIAGNOSTICS
     #define LIN_IOCTL_READ_NAD            9u
     #define LIN_IOCTL_WRITE_NAD          10u
     #define LIN_IOCTL_WRITE_INITIAL_NAD  11u
  #endif

  #ifdef LIN_PROTOCOL_SWITCH
    STACK extern void l_protocol_switch (l_ifc_handle ifc, l_bool linEnable) REENTRANT;
  #endif

#endif



#endif /*  __LIN_H__ */
