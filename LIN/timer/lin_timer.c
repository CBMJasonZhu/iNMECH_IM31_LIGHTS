/*********************************************************************
 *
 * Project:      LIN 2.1 driver suite
 *
 * File name:    lin_timer.c
 *
 * Copyright:    (c) STMicroelectronics
 *
 * Author:       Giuseppe Stefano Fazio
 *
 * Description:  Implementation of the lin timer
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

/** \file lin_timer.c
 */

#define LIN_TIMER_C

#define LIN_TIMER_C_MAJOR_VERSION  1
#define LIN_TIMER_C_MINOR_VERSION  31
#define LIN_TIMER_C_PATCH_VERSION  2

#define LIN_TIMER_C_STM

/******************************************************************************/
/*                                Includes                                    */
/******************************************************************************/
#include "lin_def.h"
#include "lin_types.h"
#include "lin_def_gen.h"
#include "lin_def_arch_include.h"
#include "lin_general.h"
#include "lin.h"
#include "lin_timer.h"
/* Begin BUGZILLA_540 */
#if (defined(LIN_21) && defined(LIN_INCLUDE_COOKED_TP))
  #include "lin_diag.h"
#endif /* LIN_21 && LIN_INCLUDE_COOKED_TP */
/* End BUGZILLA_540 */
#ifdef LIN_USE_HARDWARE_TIMER
  #include "lin_arch_include.h"
#endif

#include "lin_version_control.h"


/******************************************************************************/
/*                             Global variables                               */
/******************************************************************************/

TIMER Timer ;           /* Timer structure, will be initialized with 0 by "C" */
/* Begin BUGZILLA_540 */
#if (defined(LIN_21) && defined(LIN_INCLUDE_COOKED_TP))
  TIMER l_N_As_Timer ;  /* LIN 2.1 N_As Timer used for Transport Protocol     */
  TIMER l_N_Cr_Timer ;  /* LIN 2.1 N_Cr Timer used for Transport Protocol     */
#endif /* LIN_21 && LIN_INCLUDE_COOKED_TP */
/* End BUGZILLA_540 */


/******************************************************************************/
/*                                Prototypes                                  */
/******************************************************************************/

#ifdef LIN_USE_HARDWARE_TIMER
  STACK static void l_timer_tick (void) REENTRANT ;
/* Begin BUGZILLA_540 */
  #if (defined(LIN_21) && defined(LIN_INCLUDE_COOKED_TP))
    STACK static void l_timer_tick_N_As(void) REENTRANT ;
    STACK static void l_timer_tick_N_Cr(void) REENTRANT ;
  #endif /* LIN_21 && LIN_INCLUDE_COOKED_TP */
/* End BUGZILLA_540 */
#endif


/******************************************************************************/
/*****************************                    *****************************/
/*****                           Implementation                           *****/
/*****************************                    *****************************/
/******************************************************************************/



/*****************************************************************************
 * l_TimerInit()
 */
/**
 * \brief this function initializes the LIN timer
 *
 * Initialize the driver, really needed only when hardware timer is used
 *
 * - internal function
 * - master and slave
 *
 * \param  none
 *
 * \retval none
 */
STACK void l_TimerInit (void)
{
  #ifdef LIN_USE_HARDWARE_TIMER

    (void) l_sys_init_arch () ; /* Init hardware timer */

    /* Start first timer cycle */
    LIN_START_TIMER_ARCH(LIN_TIME_BASE_IN_MS) ;

  #endif
}  /* end l_TimerInit */

/*********************************************************************
 *
 * l_TimerStart()
 */
/**
 * \brief function to start a LIN timer
 *
 * This function starts the timer for one timer channel
 * If the timer channel is already running, it will be
 * restarted with the new timeout value
 *
 * - internal function
 * - master and slave
 *
 * \param  timer          timer channel to be started
 * \param  SubChannel     Subchannel to be started
 * \param  TimeOut        Timeout in milliseconds
 *
 * \retval none
 *
 */
STACK void l_TimerStart (__OPTIONAL_TIMER_DEFINITION_COMMA__ l_u8 SubChannel, l_u16 Timeout)
{
  l_irqmask irqMask;       /* Save the interrupt status */


  #ifdef LIN_DEVELOPMENT
    if (SubChannel == (l_u8) LIN_SUBCHANNEL_NOT_USED)          /* This should not happen */
    {
      return ;
    }
    #ifdef __MORE_IFCS__
      if (timer >= LIN_TIMER_NR_MAX)
      {
        return ;
      }
    #endif
  #endif

  irqMask = l_sys_irq_disable();

  /* Adjust to real time base which is a multiple of one ms */
  /* Add one if possible to compensate for glitches         */
  Timeout = ((Timeout + LIN_TIME_BASE_IN_MS) - 1) / LIN_TIME_BASE_IN_MS ;
  if (Timeout < (l_u16) 0xFFFFU)
  {
    Timeout++ ;
  }

  #ifdef __MORE_IFCS__
    Timer.SubChannel[timer] = SubChannel ;      /* Set new subchannel ...    */
    Timer.Ticks[timer]      = Timeout ;         /* ... and new timeout value */
  #else
    Timer.SubChannel        = SubChannel ;      /* Set new subchannel ...    */
    Timer.Ticks             = Timeout ;         /* ... and new timeout value */
  #endif

  #ifdef LIN_USE_HARDWARE_TIMER
    LIN_CHECK_HARDWARE_TIMER() ;
  #endif

  l_sys_irq_restore(irqMask);

} /* end TimerStart */

/*********************************************************************
 *
 * l_TimerStop()
 */
/**
 * \brief function to stop timer channel
 *
 * This function stops the timer for one timer channel.
 * \attention The timer channel is invalided and the ticks are set to 0 
 * - internal function
 * - master and slave
 *
 * \param timer          timer channel to be stopped
 *
 * \retval none
 *
 */
STACK void l_TimerStop (__OPTIONAL_TIMER_DEFINITION__)
{
  l_irqmask irqMask;       /* Save the interrupt status */


  #if (defined LIN_DEVELOPMENT) && (defined __MORE_IFCS__)
    if (timer >= LIN_TIMER_NR_MAX)
    {
      return ;
    }
  #endif

  irqMask = l_sys_irq_disable();

  /* Deactivate timer channel */
  #ifdef __MORE_IFCS__
    Timer.SubChannel[timer] = (l_u8)  LIN_SUBCHANNEL_NOT_USED ;
    Timer.Ticks[timer]      = (l_u16) 0 ;
  #else
    Timer.SubChannel        = (l_u8)  LIN_SUBCHANNEL_NOT_USED ;
    Timer.Ticks             = (l_u16) 0 ;
  #endif

  l_sys_irq_restore(irqMask);
}  /* end l_TimerStop */

/*********************************************************************
 *
 * l_timer_tick()
 */
/**
 * \brief function to indicate a timer tick
 *
 * This function should be called from the application every
 * LIN_TIME_BASE_IN_MS millisecond (see lin_def.h).
 * This function will count down the LIN timers and will
 * evaluate the expired timer channels.
 *
 * - STMicroelectronics specific API (or intern for HW timer)
 * - master and slave
 *
 * \param  none
 *
 * \retval none
 *
 */
 
#ifdef LIN_USE_HARDWARE_TIMER
  STACK static void l_timer_tick (void) REENTRANT
#else
  STACK void l_timer_tick (void) REENTRANT
#endif

{
  #ifndef LIN_USE_HARDWARE_TIMER
    l_irqmask irqMask;       /* Save the interrupt status */
  #endif
  #ifdef __MORE_IFCS__
    l_u8      i ;            /* Counter */
  #endif


  #ifndef LIN_USE_HARDWARE_TIMER
    irqMask = l_sys_irq_disable();
  #endif

  #ifdef __MORE_IFCS__
    for (i = 0; i < LIN_TIMER_NR_MAX; ++i)
    {
      if (Timer.Ticks[i] != 0)
      {
        Timer.Ticks[i]-- ;
        if (!Timer.Ticks[i])                                    /* Timer is expired */
        {
          l_evaluateTimeout (i) ;
        }
      }
    }
  #else
    if (Timer.Ticks != 0)
    {
      Timer.Ticks-- ;
      if (!Timer.Ticks)                                         /* Timer is expired */
      {
        l_evaluateTimeout () ;
      }
    }
  #endif

  #ifndef LIN_USE_HARDWARE_TIMER
    l_sys_irq_restore(irqMask);
  #endif
}  /* end l_timer_tick */

/* Begin BUGZILLA_540 */
#if (defined(LIN_21) && defined(LIN_INCLUDE_COOKED_TP))
/*********************************************************************
 *
 * l_TimerStart_N_As()
 */
/**
 * \brief function to start a LIN 2.1 N_As Transport Protocol timer
 *
 * This function starts the timer for one timer channel
 * If the timer channel is already running, it will be
 * restarted with the new timeout value
 *
 * - internal function
 * - master and slave
 *
 * \param  timer          timer channel to be started
 * \param  TimeOut        Timeout in milliseconds
 *
 * \retval none
 *
 */
STACK void l_TimerStart_N_As (__OPTIONAL_TIMER_DEFINITION_COMMA__ l_u16 Timeout)
{
  l_irqmask irqMask;       /* Save the interrupt status */

  irqMask = l_sys_irq_disable();

  /* Adjust to real time base which is a multiple of one ms */
  /* Add one if possible to compensate for glitches         */
  Timeout = ((Timeout + LIN_TIME_BASE_IN_MS) - 1) / LIN_TIME_BASE_IN_MS ;
  if (Timeout < (l_u16) 0xFFFFU)
  {
    Timeout++ ;
  }

  #ifdef __MORE_IFCS__
    l_N_As_Timer.SubChannel[timer] = LIN_N_AS_SUBCHANNEL ;      /* Set new subchannel ...    */
    l_N_As_Timer.Ticks[timer]      = Timeout ;                  /* ... and new timeout value */
  #else
    l_N_As_Timer.SubChannel        = LIN_N_AS_SUBCHANNEL ;      /* Set new subchannel ...    */
    l_N_As_Timer.Ticks             = Timeout ;                  /* ... and new timeout value */
  #endif

  #ifdef LIN_USE_HARDWARE_TIMER
    LIN_CHECK_HARDWARE_TIMER() ;
  #endif

  l_sys_irq_restore(irqMask);

} /* end l_TimerStart_N_As */

/*********************************************************************
 *
 * l_TimerStart_N_Cr()
 */
/**
 * \brief function to start a LIN 2.1 N_Cr Transport Protocol timer
 *
 * This function starts the timer for one timer channel
 * If the timer channel is already running, it will be
 * restarted with the new timeout value
 *
 * - internal function
 * - master and slave
 *
 * \param  timer          timer channel to be started
 * \param  TimeOut        Timeout in milliseconds
 *
 * \retval none
 *
 */
STACK void l_TimerStart_N_Cr (__OPTIONAL_TIMER_DEFINITION_COMMA__ l_u16 Timeout)
{
  l_irqmask irqMask;       /* Save the interrupt status */

  irqMask = l_sys_irq_disable();

  /* Adjust to real time base which is a multiple of one ms */
  /* Add one if possible to compensate for glitches         */
  Timeout = ((Timeout + LIN_TIME_BASE_IN_MS) - 1) / LIN_TIME_BASE_IN_MS ;
  if (Timeout < (l_u16) 0xFFFFU)
  {
    Timeout++ ;
  }

  #ifdef __MORE_IFCS__
    l_N_Cr_Timer.SubChannel[timer] = LIN_N_CR_SUBCHANNEL ;      /* Set new subchannel ...    */
    l_N_Cr_Timer.Ticks[timer]      = Timeout ;                  /* ... and new timeout value */
  #else
    l_N_Cr_Timer.SubChannel        = LIN_N_CR_SUBCHANNEL ;      /* Set new subchannel ...    */
    l_N_Cr_Timer.Ticks             = Timeout ;                  /* ... and new timeout value */
  #endif

  #ifdef LIN_USE_HARDWARE_TIMER
    LIN_CHECK_HARDWARE_TIMER() ;
  #endif

  l_sys_irq_restore(irqMask);

} /* end l_TimerStart_N_Cr */

/*********************************************************************
 *
 * l_TimerStop_N_As()
 */
/**
 * \brief function to stop LIN 2.1 N_As Transport Protocol timer channel
 *
 * This function stops the timer for one timer channel.
 * \attention The timer channel is invalided and the ticks are set to 0 
 * - internal function
 * - master and slave
 *
 * \param timer          timer channel to be stopped
 *
 * \retval none
 *
 */
STACK void l_TimerStop_N_As (__OPTIONAL_TIMER_DEFINITION__)
{
  l_irqmask irqMask;       /* Save the interrupt status */

  irqMask = l_sys_irq_disable();

  /* Deactivate timer channel */
  #ifdef __MORE_IFCS__
    l_N_As_Timer.SubChannel[timer] = (l_u8)  LIN_SUBCHANNEL_NOT_USED ;
    l_N_As_Timer.Ticks[timer]      = (l_u16) 0 ;
  #else
    l_N_As_Timer.SubChannel        = (l_u8)  LIN_SUBCHANNEL_NOT_USED ;
    l_N_As_Timer.Ticks             = (l_u16) 0 ;
  #endif

  l_sys_irq_restore(irqMask);
}  /* end l_TimerStop_N_As */

/*********************************************************************
 *
 * l_TimerStop_N_Cr()
 */
/**
 * \brief function to stop LIN 2.1 N_Cr Transport Protocol timer channel
 *
 * This function stops the timer for one timer channel.
 * \attention The timer channel is invalided and the ticks are set to 0 
 * - internal function
 * - master and slave
 *
 * \param timer          timer channel to be stopped
 *
 * \retval none
 *
 */
STACK void l_TimerStop_N_Cr (__OPTIONAL_TIMER_DEFINITION__)
{
  l_irqmask irqMask;       /* Save the interrupt status */

  irqMask = l_sys_irq_disable();

  /* Deactivate timer channel */
  #ifdef __MORE_IFCS__
    l_N_Cr_Timer.SubChannel[timer] = (l_u8)  LIN_SUBCHANNEL_NOT_USED ;
    l_N_Cr_Timer.Ticks[timer]      = (l_u16) 0 ;
  #else
    l_N_Cr_Timer.SubChannel        = (l_u8)  LIN_SUBCHANNEL_NOT_USED ;
    l_N_Cr_Timer.Ticks             = (l_u16) 0 ;
  #endif

  l_sys_irq_restore(irqMask);
}  /* end l_TimerStop_N_Cr */

/*********************************************************************
 *
 * l_timer_tick_N_As()
 */
/**
 * \brief function to indicate a timer tick
 *
 * This function should be called from the application every
 * LIN_TIME_BASE_IN_MS millisecond (see lin_def.h).
 * This function will count down the LIN LIN 2.1 N_As Transport 
 * Protocol timer and will evaluate the expired timer channels.
 *
 * - STMicroelectronics specific API (or intern for HW timer)
 * - master and slave
 *
 * \param  none
 *
 * \retval none
 *
 */
 
#ifdef LIN_USE_HARDWARE_TIMER
  STACK static void l_timer_tick_N_As (void) REENTRANT
#else
  STACK void l_timer_tick_N_As (void) REENTRANT
#endif

{
  #ifndef LIN_USE_HARDWARE_TIMER
    l_irqmask irqMask;       /* Save the interrupt status */
  #endif
  #ifdef __MORE_IFCS__
    l_u8      i ;            /* Counter */
  #endif


  #ifndef LIN_USE_HARDWARE_TIMER
    irqMask = l_sys_irq_disable();
  #endif

  #ifdef __MORE_IFCS__
    for (i = 0; i < LIN_TIMER_NR_MAX; ++i)
    {
      if (l_N_As_Timer.Ticks[i] != 0)
      {
        l_N_As_Timer.Ticks[i]-- ;
        if (!l_N_As_Timer.Ticks[i])                                    /* Timer is expired */
        {
          LIN_COOKED_TP_TIMEOUT_FLAGS(i) |= LIN_COOKED_N_AS_TIMEOUT;
        }
      }
    }
  #else
    if (l_N_As_Timer.Ticks != 0)
    {
      l_N_As_Timer.Ticks-- ;
      if (!l_N_As_Timer.Ticks)                                         /* Timer is expired */
      {
        LIN_COOKED_TP_TIMEOUT_FLAGS(0) |= LIN_COOKED_N_AS_TIMEOUT;
      }
    }
  #endif

  #ifndef LIN_USE_HARDWARE_TIMER
    l_sys_irq_restore(irqMask);
  #endif
}  /* end l_timer_tick_N_As */

/*********************************************************************
 *
 * l_timer_tick_N_Cr()
 */
/**
 * \brief function to indicate a timer tick
 *
 * This function should be called from the application every
 * LIN_TIME_BASE_IN_MS millisecond (see lin_def.h).
 * This function will count down the LIN LIN 2.1 N_Cr Transport 
 * Protocol timer and will evaluate the expired timer channels.
 *
 * - STMicroelectronics specific API (or intern for HW timer)
 * - master and slave
 *
 * \param  none
 *
 * \retval none
 *
 */
 
#ifdef LIN_USE_HARDWARE_TIMER
  STACK static void l_timer_tick_N_Cr (void) REENTRANT
#else
  STACK void l_timer_tick_N_Cr (void) REENTRANT
#endif

{
  #ifndef LIN_USE_HARDWARE_TIMER
    l_irqmask irqMask;       /* Save the interrupt status */
  #endif
  #ifdef __MORE_IFCS__
    l_u8      i ;            /* Counter */
  #endif


  #ifndef LIN_USE_HARDWARE_TIMER
    irqMask = l_sys_irq_disable();
  #endif

  #ifdef __MORE_IFCS__
    for (i = 0; i < LIN_TIMER_NR_MAX; ++i)
    {
      if (l_N_Cr_Timer.Ticks[i] != 0)
      {
        l_N_Cr_Timer.Ticks[i]-- ;
        if (!l_N_Cr_Timer.Ticks[i])                                    /* Timer is expired */
        {
          LIN_COOKED_TP_TIMEOUT_FLAGS(i) |= LIN_COOKED_N_CR_TIMEOUT;
        }
      }
    }
  #else
    if (l_N_Cr_Timer.Ticks != 0)
    {
      l_N_Cr_Timer.Ticks-- ;
      if (!l_N_Cr_Timer.Ticks)                                         /* Timer is expired */
      {
        LIN_COOKED_TP_TIMEOUT_FLAGS(0) |= LIN_COOKED_N_CR_TIMEOUT;
      }
    }
  #endif

  #ifndef LIN_USE_HARDWARE_TIMER
    l_sys_irq_restore(irqMask);
  #endif
}  /* end l_timer_tick_N_Cr */
#endif /* LIN_21 && LIN_INCLUDE_COOKED_TP */
/* End BUGZILLA_540 */

#ifdef LIN_USE_HARDWARE_TIMER
  /*********************************************************************
   *
   * l_timerISR()
   */
  /**
   * \brief timer interrupt service routine
   *
   * This ISR will be activated every LIN_TIME_BASE_IN_MS millisecond
   * from the timer hardware.
   * First the timer will be restarted for another LIN_TIME_BASE_IN_MS
   * millisecond.
   * Then l_timer_tick will be called to handle the timing.
   * Moreover, for LIN 2.1, also the l_timer_tick_N_As() and the
   * l_timer_tick_N_Cr functions will be called to manage the LIN 2.1
   * Trasport Protocol timeouts.
   *
   * \param  none
   *
   * \retval none
   *
   */
  STACK void l_timerISR(void) REENTRANT
  {
    l_irqmask        irqMask;       /* Save the interrupt status */


    irqMask = l_sys_irq_disable();

    LIN_TIMER_CLEAR_IRQ_FLAG();

    LIN_RESTART_TIMER_ARCH(LIN_TIME_BASE_IN_MS) ;

    l_timer_tick () ;

/* Begin BUGZILLA_540 */
#if (defined(LIN_21) && defined(LIN_INCLUDE_COOKED_TP))
    l_timer_tick_N_As() ;
    l_timer_tick_N_Cr() ;
#endif /* LIN_21 */
/* End BUGZILLA_540 */

    l_sys_irq_restore(irqMask);
  } /* end l_timerISR() */
#endif