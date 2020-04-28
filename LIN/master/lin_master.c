/*********************************************************************
 *
 * Project:      LIN 2.1 driver suite
 *
 * File name:    lin_master.c
 *
 * Copyright:    (c) STMicroelectronics
 *
 * Author:       Giuseppe Stefano Fazio
 *
 * Description:  Master node specific functionality
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
 * 20071217  v1.0  gsf  - BUGZILLA_378: Wrong driver implementation
 *                                      for Break and Del readback
 *                 gsf  - BUGZILLA_341: Expiring frame timer shall be
 *                                      interpreted as collision in event
 *                                      triggered frame
 *
 *********************************************************************/

/** \file lin_master.c
 */

#define LIN_MASTER_C

#define LIN_MASTER_C_MAJOR_VERSION  1
#define LIN_MASTER_C_MINOR_VERSION  31
#define LIN_MASTER_C_PATCH_VERSION  1

#define LIN_MASTER_C_STM

/*********************************************************************
 * includes
 *********************************************************************/

#include "lin_cfg_types.h"      /* Defines specific to the used configuration, derived from the .ldf and the .lgn files */

#include "lin_types.h"
#include "lin_timer.h"

#include "lin_master.h"
#include "lin_def.h"
#include "lin_def_gen.h"
#include "lin_def_arch_include.h"
#include "lin_general.h"
#include "lin_arch_include.h"
#include "lin_diag.h"
#include "lin_diag_master.h"
#include "lin_cfg.h"

#include "lin.h"


#include "lin_version_control.h"


/*********************************************************************
 * global constants
 *********************************************************************/

/*********************************************************************
 * global variables
 *********************************************************************/

/*********************************************************************
 * local constants
 *********************************************************************/

/*********************************************************************
 * local variables
 *********************************************************************/

/*********************************************************************
 * local functions
 *********************************************************************/

/*********************************************************************
 *
 * implementation
 *
 *********************************************************************/

/*********************************************************************
 *
 * API functions
 *
 *********************************************************************/

/*********************************************************************
 *
 * l_sch_set()
 */
/**
 * \brief schedule control function
 *
 * Sets the entry specified to be next scheduled frame
 *
 * - standard API
 * - master only
 *
 * \param  ifc            handle of the interface
 * \param  schedule       handle of the schedule table to select
 * \param  entry          entry of the table to select
 *
 * \retval none
 *
 */
STACK void l_sch_set (l_ifc_handle ifc, l_schedule_handle schedule, l_u8 entry) REENTRANT
{
  l_irqmask irqMask;


  __CHECK_INTERFACE_VOID__(ifc);

  #ifdef LIN_DEVELOPMENT
    if (schedule > (LIN_SCHED_TAB_NR_MAX - 1) )
    {
      /*
       * parameters invalid, return
       */
      return ;
    }
  #endif

  /*
   * parameters are ok (entry cannot be checked), so copy the values
   */
  irqMask = l_sys_irq_disable();

  #ifdef __MORE_IFCS__
    if ((l_pSchTabIfc[schedule] == LIN_INVALID_IFC_INDEX)
        ||
        (l_pSchTabIfc[schedule] == ifc))
       /*
        * check that the schedule is intended for the requested interface
        */
  #endif
  {
    if (entry != 0)
    {
      entry -= 1;
    }
    IFCCTRL_SCHEDULE(ifc) = schedule;
    IFCCTRL_ENTRY(ifc)    = entry;

    /* Note: the delay value will not be altered here, since it is
     *       determined by the last frame transmitted, not the next
     *       frame due
     */
  }
  l_sys_irq_restore(irqMask);

  return ;

} /* end l_sch_set() */

#if (!defined(LIN_13) || defined(LIN_INCLUDE_2x_SLEEP_MODE_API))

  /*********************************************************************
   *
   * l_ifc_goto_sleep()
   */
  /**
   * \brief goto sleep request function
   *
   * This function sets the interface to sleep mode
   *
   * - standard API
   * - master only
   *
   * \param ifc            the handle of the interface to enter sleep mode
   *
   * \retval none
   *
   */
  STACK void l_ifc_goto_sleep (l_ifc_handle ifc) REENTRANT
  {
    l_irqmask irqMask;


    __CHECK_INTERFACE_VOID__(ifc);

    irqMask = l_sys_irq_disable();

    /*
     * store the current schedule table
     */
    if (IFCCTRL_GOTOSLEEPREQ(ifc) == 0)
    {
      IFCCTRL_GOTOSLEEPSCHEDULE(ifc) = IFCCTRL_SCHEDULE(ifc);
      IFCCTRL_GOTOSLEEPENTRY(ifc)    = IFCCTRL_ENTRY(ifc);

      IFCCTRL_SCHEDULE(ifc)          = LIN_TAB_GOTO_SLEEP;
      IFCCTRL_ENTRY(ifc)             = 0;
      IFCCTRL_DELAY(ifc)             = 1;
      IFCCTRL_GOTOSLEEPREQ(ifc)      = 1;

      /*
       * the command will be sent on the next call to l_sch_tick()
       * Note: It is possible that the user will not call
       *       l_sch_tick() if the module is in bussleep
       */
    }

    l_sys_irq_restore(irqMask);

    return ;

  } /* end l_ifc_goto_sleep() */

  /*********************************************************************
   *
   * l_ifc_wake_up()
   */
  /**
   * \brief wake up request function
   *
   * This function awakes the selected interface from sleep mode
   *
   * - standard API
   * - master and slave
   *
   * \param  ifc            the interface handle
   *
   * \retval - none
   *
   */
  STACK void l_ifc_wake_up (l_ifc_handle ifc) REENTRANT
  {
    l_irqmask irqMask;


    __CHECK_INTERFACE_VOID__(ifc);

    irqMask = l_sys_irq_disable();

    /*
     * check ifc state
     */
    if (IFCCTRL_STATE(ifc) == LIN_STATE_BUSSLEEP)
    {
      /* We must send wakeup signal */
      CHANGEBAUDRATE_ARCH(__MANDATORY_IFC_PARAMETER__, LIN_WAKEUP_BAUDRATE);
      l_sendByte(__OPTIONAL_IFC_PARAMETER_COMMA__ LIN_WAKEUP_SIG);

      /*
       * no need to change state here since
       * we will also receive the wakeup signal and
       * can act upon the signal there
       */
    } /* if (state = BUSSLEEP)
     * no else, we will issue that byte only in sleep mode
     */

    l_sys_irq_restore(irqMask);

    return ;

  } /* end l_ifc_wake_up() */

#endif /* !LIN_13 or LIN_INCLUDE_2x_SLEEP_MODE_API */

/*********************************************************************
 *
 * API extensions
 *
 *********************************************************************/

#ifdef LIN_USE_COOLING

  /*********************************************************************
   *
   * l_ifc_set_cooling_break ()
   */
  /**
   * \brief cooling control function
   *
   * This function sets the break length to the cooling value or to the
   * standard value
   *
   * - STMicroelectronics extension
   * - master only
   *
   * \param  ifc            the interface handle
   * \param  on             TRUE: set cooling break length, otherwise
   *                        set standard break length
   *
   * \retval none
   *
   */
  STACK void l_ifc_set_cooling_break (l_ifc_handle ifc, l_bool on) REENTRANT
  {
    l_irqmask irqMask ;    /* Save old interrupt status */
    l_u16      Duration ;   /* Length of Synch Break     */


    __CHECK_INTERFACE_VOID__(ifc);

    #ifdef LIN_DEVELOPMENT
      if (ifc >= LIN_IFC_NR_MAX)
      {
        return;
      }
    #endif

    irqMask = l_sys_irq_disable();

    if (on != 0)
    {
      Duration = (l_u16) LIN_COOLING_BREAK_DURATION_BITS ;
    }
    else
    {
      Duration  = (l_u16) LIN_BREAK_DURATION_BITS ;
    }

    l_sys_irq_restore(irqMask);

    return ;

  } /* end l_ifc_set_cooling_break */

#endif


/*********************************************************************
 *
 * internal control functions
 *
 *********************************************************************/

/*********************************************************************
 *
 * l_evaluateData()
 */
/**
 * \brief main entry point from ISR
 *
 * This is the main function to control transmissions. It will
 * be called whenever a character is received in the LIN bus (i.e.
 * from the serial ISR).
 * As the handling of the several states will become complex,
 * separate functions will be called for every substate
 * 
 * - internal function
 * - master only
 *
 * \param  ifc            the handle of the interface
 * \param  rxChar         the character just received
 *
 * \retval none
 *
 */
STACK void l_evaluateData (__OPTIONAL_IFC_DEFINITION_COMMA__ const l_u8 rxChar)
{
  __CHECK_INTERFACE_VOID__(ifc);

  /*
   * serial error to be handled separately
   */
  if (IFCCTRL_SERIALERROR(ifc) != 0)
  {

     l_serialError(__OPTIONAL_IFC_PARAMETER__);

    /* reset error flag */
    IFCCTRL_SERIALERROR(ifc) = 0;

  }
  else
  {
    /*
     * handling depends on the current state of the interface
     */
    switch ((l_ifcState_t) IFCCTRL_STATE(ifc))
    {
      case LIN_STATE_SEND_SYNC:
          l_sendByte(__OPTIONAL_IFC_PARAMETER_COMMA__ LIN_SYNC_PATTERN);

          IFCCTRL_STATE(ifc) = LIN_STATE_SEND_IDENTIFIER;

        break;
        
      case LIN_STATE_RECEIVE_DATA:
        
        l_stateReceiveData(__OPTIONAL_IFC_PARAMETER_COMMA__
                           rxChar);
        
        break;
        
      case LIN_STATE_BUSSLEEP:
        
        /*
         * check whether this is a valid signal
         */
        if ((rxChar & LIN_RECEIVE_WAKEUP_SIG) == 0)
        {
          /* The signal is valid */ 
          l_setBaudrate(__OPTIONAL_IFC_PARAMETER_COMMA__
                        IFCCTRL_NORMALBAUDRATE(ifc));
          
          #ifdef __MORE_IFCS__
            if (Timer.SubChannel[ifc] != (l_u8) LIN_WAKEUP_SUBCHANNEL)
          #else
            if (Timer.SubChannel != (l_u8) LIN_WAKEUP_SUBCHANNEL)
          #endif
          {
            /* The timer is not yet running, start it */
            l_TimerStart(__OPTIONAL_TIMER_IFC_PARAMETER_COMMA__
                         (l_u8) LIN_WAKEUP_SUBCHANNEL,
                         LIN_WAKEUP_TIMER_VALUE(ifc));
          } /* no else,
             * timer is already running
             */
            
        } /* if (valid signal) */
        
        /*
         * further handling is done when timer expires
         */
        break;
        
      default:
        /* This should only be the state LIN_STATE_IDLE */
        break;
        
    } /* switch (state) */
  }
  
  return ;

} /* end l_evaluateData() */


/*********************************************************************
 *
 * l_evaluateTimeout()
 */
/**
 * \brief entry point to handle expired timers
 *
 * This function handles all expiring timers for the master node
 *
 * - internal function
 * - master only
 *
 * \param  timer          the id of the timer
 *
 * \retval none
 *
 */
STACK void l_evaluateTimeout (__OPTIONAL_TIMER_DEFINITION__)
{
  l_u8                        subChannel;
  #ifdef __MORE_IFCS__
    l_ifc_handle              ifc = (l_ifc_handle)timer;
  #endif


  /*
   * timer handling mainly depends on the ifc state
   */

  #ifdef __MORE_IFCS__
    subChannel = Timer.SubChannel[timer];
  #else
    subChannel = Timer.SubChannel;
  #endif

  switch ((l_ifcState_t) IFCCTRL_STATE(ifc))
  {

    case LIN_STATE_SEND_SYNC:
    case LIN_STATE_SEND_IDENTIFIER:
    case LIN_STATE_RECEIVE_IDENTIFIER:
      if (subChannel == (l_u8) LIN_FRAME_SUBCHANNEL)
      {
        /*
         * abort transfer
         */
        l_terminateTransfer(__OPTIONAL_IFC_PARAMETER__);

      } /* no else,
         * ignore wakeup timer
         */

      break;

    case LIN_STATE_SEND_DATA:
      if (subChannel == (l_u8) LIN_FRAME_SUBCHANNEL)
      {
        /*
         * abort transfer
         */
        l_terminateTransfer(__OPTIONAL_IFC_PARAMETER__);

      }
       /* no else,
        * ignore wakeup timer
        */
      break;
  /* Begin BUGZILLA_341 */
    case LIN_STATE_RECEIVE_DATA:
      if (subChannel == (l_u8) LIN_FRAME_SUBCHANNEL)
      {
        /*
         * abort transfer
         */
        if ((IFCCTRL_FLAGS(ifc) & LIN_IFC_EVENT_TRIGGERED_FLAG)
            &&
            (TRANSFERCTRL_BYTESSENT(ifc) > 1))
        {
          /*
           * an expiring frame timer could mean a collision
           * in an event triggered frame
           */
#ifdef LIN_21
          IFCCTRL_FLAGS(ifc) &= (l_u8) (~LIN_IFC_EVENT_TRIGGERED_FLAG);
          IFCCTRL_FLAGS(ifc) |= LIN_IFC_COLLISION_FLAG;
#else
          l_serialError(__OPTIONAL_IFC_PARAMETER__);
#endif /* LIN_21 */
        }
        else
        {
        l_terminateTransfer(__OPTIONAL_IFC_PARAMETER__);
        }
  /* End BUGZILLA_341 */
      }
       /* no else,
        * ignore wakeup timer
        */

      break;

    case LIN_STATE_BUSSLEEP:
      /*
       * only wakeup timer relevant here
       */
      if (subChannel == (l_u8) LIN_WAKEUP_SUBCHANNEL)
      {
        IFCCTRL_STATE(ifc) = LIN_STATE_IDLE;

      }
       /* no else, only handle wakeup timer */
      break;

    case LIN_STATE_IDLE:
    default:
      /*
       * ignore the timer
       */
      break;

  } /* switch (ifcState) */

  return ;

} /* end l_evaluateTimeout() */


/*********************************************************************
 *
 * helper functions
 *
 *********************************************************************/

/*********************************************************************
 *
 * l_terminateTransfer()
 */
/**
 * \brief function aborting a transfer
 *
 * This function terminates an ongoing transfer on the bus.
 * The control structure will be reset (hence all data to be
 * transferred will be deleted), the baudrate of the interface will
 * be set to the normal value for that interface
 *
 * - internal function
 * - master only
 *
 * \param  ifc            the handle of the interface
 *
 * \retval none
 *
 */
STACK void l_terminateTransfer(__OPTIONAL_IFC_DEFINITION__)
{
  l_u8  l_Buffer[10], i;

  for (i = 0; i < 10; ++i)
  {
    l_Buffer[i] = TRANSFERCTRL_BUFFER(ifc)[i];
  }

  /* Take care of any running timers */
  l_TimerStop (__OPTIONAL_TIMER_IFC_PARAMETER__);

  /*
   * special handling for slave response frame
   * no need to accept class2 responses from now on
   */
  #ifdef LIN_INCLUDE_DIAG2
    if (TRANSFERCTRL_BUFFER(ifc)[0] == LIN_SLAVE_RESP_FRAME_ID)
    {
       LIN_DIAG2_FLAGS(ifc) &= (l_u8) ~LIN_DIAG2_EXPECT_RESPONSE;
    }
  #endif

  /*
   * set all variables to default values
   */

  l_initTransferCtrl(__OPTIONAL_IFC_PARAMETER__);

  IFCCTRL_DIAGFLAGS(ifc) = 0;
  
  /*
   * handle bussleep request
   * (if an error occured during transmission of the goto sleep
   *  command, the frame is not resent, but the bus will remain
   *  silent, hence a slave should in any case enter sleep mode
   *  after 4 sec)
   */

if ((IFCCTRL_GOTOSLEEPREQ(ifc) != 0)  
 && (l_Buffer[0] == LIN_MASTER_REQ_FRAME_ID)
 && (l_Buffer[1] == 0x00)
 && (l_Buffer[2] == 0xFF)
 && (l_Buffer[3] == 0xFF)
 && (l_Buffer[4] == 0xFF)
 && (l_Buffer[5] == 0xFF)
 && (l_Buffer[6] == 0xFF)
 && (l_Buffer[7] == 0xFF)
 && (l_Buffer[8] == 0xFF)
)
  {
    LIN_RESET_LBDL_BIT();
    IFCCTRL_STATE(ifc)        = LIN_STATE_BUSSLEEP;
    /* Begin BUGZILLA_378 */
    LIN_RIE();
    /* End BUGZILLA_378 */
    IFCCTRL_GOTOSLEEPREQ(ifc) = 0;
    IFCCTRL_DELAY(ifc) = 0x01;

    /*
     * and change baudrate here, in order to guarantee
     * correct detection of slave node wakeup signals
     */
    CHANGEBAUDRATE_ARCH(ifc, LIN_WAKEUP_BAUDRATE);

    l_ifcSetStatus(__OPTIONAL_IFC_PARAMETER_COMMA__ LIN_STATUS_BUSSLEEP, 0);

  }

  /*
   * set interface state to IDLE
   */
  if (IFCCTRL_STATE(ifc) != LIN_STATE_BUSSLEEP)
  {
    IFCCTRL_STATE(ifc) = LIN_STATE_IDLE;
  }
  
  return ;

} /* end l_terminateTransfer() */

/*********************************************************************
 *
 * l_sendBreak()
 */
/**
 * \brief function sending break characters
 *
 * This function sends a break signal on the specified interface
 * The break pattern does not fit into the 8N1 pattern, hence
 * we send a 0 with a lower baudrate
 *
 * - internal function
 * - master only
 *
 *
 * \param  ifc            handle of the interface
 *
 * \retval none
 *
 */
STACK void l_sendBreak(__OPTIONAL_IFC_DEFINITION__)
{
  __CHECK_INTERFACE_VOID__(ifc);

  LIN_BDIE();
  LIN_SEND_BREAK();
  /* Begin BUGZILLA_378 */
  /* End BUGZILLA_378 */
  TRANSFERCTRL_CHARSENT(ifc) = 0;
  return ;

} /* end l_sendBreak() */

/*********************************************************************
 *
 * documentation of variables and functions defined in code
 * generated by lingen
 *
 *********************************************************************/

/**
 * \var l_speed
 *
 * \brief Arrray holding the bus speed for every interface
 */

/**
 * \var l_pFrameBuf
 *
 * \brief byte array holding the current signal values
 */

/**
 * \var l_lastMappedFrame
 *
 * \brief array to control behaviour of event triggered frames
 */


/*
 * eof lin_master.c
 */

