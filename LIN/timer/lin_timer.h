/*********************************************************************
 *
 * Project:      LIN 2.1 driver suite
 *
 * File name:    lin_timer.h
 *
 * Copyright:    (c) STMicroelectronics
 *
 * Author:       Giuseppe Stefano Fazio
 *
 * Description:  Definitions and interface of the timer
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
 * 20071217  v1.0   gsf  - Initial version for STM8
 * 20090420  v1.1   gsf  - BUGZILLA_540: LIN 2.1 / N_Cr_LIN timeout
 *                         and N_As_LIN timeout management missing.
 *
 *********************************************************************/

/** \file lin_timer.h
 */

#ifndef LIN_TIMER_H
  #define LIN_TIMER_H

#define LIN_TIMER_H_MAJOR_VERSION  1
#define LIN_TIMER_H_MINOR_VERSION  31
#define LIN_TIMER_H_PATCH_VERSION  2

#define LIN_TIMER_H_STM

/******************************************************************************/
/*                                Includes                                    */
/******************************************************************************/

#include "lin_cfg_types.h"
#include "lin_types.h"
#include "lin_def.h"
#include "lin_def_gen.h"
#include "lin_def_arch_include.h"
#include "lin_general.h"


#ifdef LIN_VERSION_CONTROL_H
  #error "lin_version_control.h shall not be included in h-files and shall be the last file to be included in a c-file!"
#endif


/*****************************************************************************/
/*                                 Defines                                   */
/*****************************************************************************/

#define LIN_TIMER_NOT_USED          ((l_u8) LIN_TIMER_NR_MAX)

#define LIN_BITS_PER_FRAME          (110 + LIN_BREAK_DURATION_BITS + ((LIN_BREAK_DURATION_BITS + 8) / 9))

#define LIN_FRAME_TIMER_VALUE(ifc) IFCCTRL_FRAMETIMERVALUE(ifc)


#ifdef LIN_MASTER_NODE
  #ifndef LIN_13
    #define LIN_WAKEUP_TIMER_VALUE(IFC) ((l_u16) (LIN_MASTER_WAKEUP_TIMER_VALUE))
  #else
    /* Wait at least 4 Bits */
    #define LIN_WAKEUP_TIMER_VALUE(IFC) (((l_u16) (4u / ((l_u16) (IFCCTRL_NORMALBAUDRATE(ifc) / 1000))) + (l_u16) 1))
  #endif
#endif






/*****************************************************************************/
/*                              External variables                           */
/*****************************************************************************/

extern TIMER Timer ;                        /* Timer structure */
/* Begin BUGZILLA_540 */
#if (defined(LIN_21) && defined(LIN_INCLUDE_COOKED_TP))
  extern TIMER l_N_As_Timer ;  /* LIN 2.1 N_As Timer structure used for TP */
  extern TIMER l_N_Cr_Timer ;  /* LIN 2.1 N_Cr Timer structure used for TP */
#endif /* LIN_21 && LIN_INCLUDE_COOKED_TP */
/* End BUGZILLA_540 */


/*****************************************************************************/
/*****                           Prototypes                              *****/
/*****************************************************************************/
STACK extern void l_TimerInit (void) ;

STACK extern void l_TimerStart (__OPTIONAL_TIMER_DEFINITION_COMMA__ l_u8 SubChannel, l_u16 Timeout);

STACK extern void l_TimerStop (__OPTIONAL_TIMER_DEFINITION__) ;

/* Begin BUGZILLA_540 */
#if (defined(LIN_21) && defined(LIN_INCLUDE_COOKED_TP))
  STACK extern void l_TimerStart_N_As (__OPTIONAL_TIMER_DEFINITION_COMMA__ l_u16 Timeout);
  STACK extern void l_TimerStart_N_Cr (__OPTIONAL_TIMER_DEFINITION_COMMA__ l_u16 Timeout);
  STACK extern void l_TimerStop_N_As (__OPTIONAL_TIMER_DEFINITION__);
  STACK extern void l_TimerStop_N_Cr (__OPTIONAL_TIMER_DEFINITION__);
#endif /* LIN_21 && LIN_INCLUDE_COOKED_TP */
/* End BUGZILLA_540 */

#endif  /* LIN_TIMER_H */
