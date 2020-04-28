/*********************************************************************
 *
 * Project:      LIN 2.1 driver suite
 *
 * File name:    lin_slave.h
 *
 * Copyright:    (c) STMicroelectronics
 *
 * Author:       Giuseppe Stefano Fazio
 *
 * Description:  Slave node specific functionality
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
 * 20090420  v1.1   gsf  - BUGZILLA_472: LIN driver does not compile if 
 *                         Cosmic "strict" option is enabled.
 *                         BUGZILLA_473: Bus activity bit returned by 
 *                         l_ifc_read_status not set on wakeup pulse 
 *                         reception.
 *                         BUGZILLA_478: “Mute Mode” must be disabled 
 *                         for a LINSCI Slave node, when this is in 
 *                         Sleep State mode.
 *                         BUGZILLA_537: LIN 2.1 / Node configuration.
 *                         BUGZILLA_538: LIN 2.1 / AssignFrameIdRange Service
 *                         BUGZILLA_539: Syntax error in code if 
 *                         LIN_INCLUDE_PID_PARITY_CHECK is enabled.
 *                         BUGZILLA_540: LIN 2.1 / N_Cr_LIN timeout
 *                         and N_As_LIN timeout management missing.
 *                         BUGZILLA_541: Response_error bit management
 *                         in case of no response or incomplete response.
 * 20100430  v1.2   JKh  - BUGZILLA_1022: Fix the bug in l_changePIDs function
 * 20110705  v1.3   JKh  - BUGZILLA_1144: The IUT does answer to all
 *                           the Assign Frame ID Range requests with Positive Response.
 * 20120522  v1.4   JKh  - Modification done to support STM8AL
 *
 *********************************************************************/

/** \file lin_slave.c
 */

#define LIN_SLAVE_C

#define LIN_SLAVE_C_MAJOR_VERSION  1
#define LIN_SLAVE_C_MINOR_VERSION  33
#define LIN_SLAVE_C_PATCH_VERSION  1

#define LIN_SLAVE_C_STM


/*********************************************************************
 * includes
 *********************************************************************/

#include "lin_def.h"
#include "lin_def_gen.h"
#include "lin_def_arch_include.h"
#include "lin_cfg_types.h"
#include "lin_types.h"
#include "lin_general.h"
#include "lin_slave.h"
#include "lin_cfg.h"

#include "lin.h"

#include "lin_arch_include.h"

#include "lin_diag.h"
#include "lin_diag_slave.h"

#include "lin_version_control.h"

#include "lin_test_hardware.h" 


#ifdef __MORE_IFCS__
  #error "The LIN slave driver will support only one IFC"
#endif

/*********************************************************************
 * global variables
 *********************************************************************/

/*********************************************************************
 * local macros
 *********************************************************************/
#define LIN_WAKEUP_RETRIES                                       3
#ifdef __COSMIC__
#define enableInterrupts() {_asm("rim\n");} /* enable interrupts */
#define disableInterrupts() {_asm("sim\n");} /* disable interrupts */
#define NoOperation() {_asm("nop\n");} /* no operation */
#elif __RAISONANCE__
#define enableInterrupts()  _rim_()  /* enable interrupts */
#define disableInterrupts() _sim_()  /* disable interrupts */
#define NoOperation() _nop_()  /* no operation */
#elif __IAR__
#define enableInterrupts() {asm("rim\n");} /* enable interrupts */
#define disableInterrupts() {asm("sim\n");} /* disable interrupts */
#define NoOperation() {asm("nop\n");} /* no operation */

#else
	#error "No compiler defined"
#endif


/*********************************************************************
 * local variables
 *********************************************************************/

/*********************************************************************
 * local functions
 *********************************************************************/

STACK static void l_stateBussleep(__OPTIONAL_IFC_DEFINITION_COMMA__ l_u8 rxChar);
#if (defined LIN_BAUDRATE_DETECT) && (!defined LIN_SLAVE_LIN_UART_HEADER)
  STACK static void l_statePreSync(__OPTIONAL_IFC_DEFINITION_COMMA__ l_u8 rxChar);
  STACK static void l_statePreIdle(__OPTIONAL_IFC_DEFINITION_COMMA__ l_u8 rxChar);
#endif


/*********************************************************************
 *
 * implementation
 *
 *********************************************************************/

/*********************************************************************
 *
 * l_evaluateData()
 *
 */
/**
 * \brief main entry point for character reception
 *
 * This function evaluates a received character
 *
 * - internal function
 * - slave only
 * 
 * \param  ifc            the handle of the interface
 * \param  rxChar         the character just received
 *
 * \retval none
 *
 */
STACK void l_evaluateData(__OPTIONAL_IFC_DEFINITION_COMMA__ l_u8 rxChar)
{
  /*
   * handle serial error
   */
  if (IFCCTRL_SERIALERROR(ifc) != 0)
  {

    l_serialError(__OPTIONAL_IFC_PARAMETER__);

  }
  else
  {
    /*
     * further handling depends on the current state of the interface
     */
    switch ((l_ifcState_t) IFCCTRL_STATE(ifc))
    {
      #if (defined LIN_BAUDRATE_DETECT) && (!defined LIN_SLAVE_LIN_UART_HEADER)
        case LIN_STATE_PRE_IDLE:
          l_statePreIdle(__OPTIONAL_IFC_PARAMETER_COMMA__ rxChar);
          break;

        case LIN_STATE_PRE_SYNC:
          l_statePreSync(__OPTIONAL_IFC_PARAMETER_COMMA__ rxChar);
          break;
      #endif  /* LIN_BAUDRATE_DETECT && !LIN_SLAVE_LIN_UART_HEADER */

      case LIN_STATE_IDLE:
         #ifndef LIN_SLAVE_LINSCI_FULL
           l_stateIdle(__OPTIONAL_IFC_PARAMETER_COMMA__ rxChar);
         #endif
         break;

      case LIN_STATE_RECEIVE_SYNC:
        #ifndef LIN_SLAVE_LINSCI_FULL
          l_stateReceiveSync(__OPTIONAL_IFC_PARAMETER_COMMA__ rxChar);
        #endif
        break;

      case LIN_STATE_RECEIVE_IDENTIFIER:
        l_stateReceiveIdentifier(__OPTIONAL_IFC_PARAMETER_COMMA__ rxChar);
        break;

      case LIN_STATE_SEND_DATA:
        l_stateSendData(__OPTIONAL_IFC_PARAMETER_COMMA__ rxChar);
        break;

      case LIN_STATE_RECEIVE_DATA:
        l_stateReceiveData(__OPTIONAL_IFC_PARAMETER_COMMA__ rxChar);
        break;

      case LIN_STATE_BUSSLEEP:
        l_stateBussleep(__OPTIONAL_IFC_PARAMETER_COMMA__ rxChar);
        break;

      default:
        /*
         * this should never happen
         */

        break;

    } /* switch (state) */

  } /* if (serial error) */
  
  return ;

} /* end l_evaluateData() */


/*********************************************************************
 *
 * l_evaluateTimeout()
 *
 */
/**
 * \brief function to handle an expired timer
 *
 * This function evaluates a timeout \n
 * timer handling mainly depends on the ifc state
 *
 * - internal function
 * - slave only
 * 
 * \param  timer          the handle of the timeout
 *
 * \retval none
 *
 */
STACK void l_evaluateTimeout(__OPTIONAL_TIMER_DEFINITION__)
{
  l_u16               timeout;


  /* the baudrate timer requires special handling */
  #if (defined LIN_BAUDRATE_DETECT) && (!defined LIN_SLAVE_LIN_UART_HEADER)
    if (Timer.SubChannel == LIN_BAUDRATE_SUBCHANNEL)
    {
      /* Begin BUGZILLA_442 */
      IFCCTRL_BAUDRATEFLAGS(ifc) &= ((l_u8) ~LIN_BAUDRATE_KEEP_SILENT);
      /* End BUGZILLA_442 */
      return ;
    }
  #endif  /* LIN_BAUDRATE_DETECT && !LIN_SLAVE_LIN_UART_HEADER */

  /*
   * timer handling mainly depends on the ifc state
   */

  switch ((l_ifcState_t) IFCCTRL_STATE(ifc))
  {
    #if (defined LIN_BAUDRATE_DETECT) && (!defined LIN_SLAVE_LIN_UART_HEADER)
      case LIN_STATE_PRE_IDLE:
        /*
         * only bussleep timer relevant here
         */

        if (Timer.SubChannel == LIN_BUSSLEEP_SUBCHANNEL)
        {
          /*
           * any ongoing transfer will be interrupted
           */
          l_terminateTransfer(__OPTIONAL_IFC_PARAMETER__);

          #if defined(LIN_SLAVE_LINSCI)
		  LIN_CLEAR_LDUM();
		  #endif

          CHANGEBAUDRATE_ARCH(__MANDATORY_IFC_PARAMETER__, LIN_WAKEUP_BAUDRATE);

          /* Begin BUGZILLA_478 */
          #if (defined(LIN_SLAVE_LINSCI) && defined(LIN_SLAVE_LINSCI_MUTE_MODE))
          {
            /*
             * disable mute mode
             */
            LIN_MUTE_MODE_DISABLE();
          }
          #endif
		  #if defined(LIN_SLAVE_LINSCI)
          /* End BUGZILLA_478 */
          LIN_RESET_LBDL_BIT();
		  #endif
          IFCCTRL_STATE(ifc) = LIN_STATE_BUSSLEEP;
        }
         /* if (bussleep timer) */

        break;

    #endif /* LIN_BAUDRATE_DETECT && !LIN_SLAVE_LIN_UART_HEADER */

    case LIN_STATE_IDLE:
    case LIN_STATE_BUSSLEEP:
      if (Timer.SubChannel == LIN_BUSSLEEP_SUBCHANNEL)
      {
        /*
         * any ongoing transfer will be interrupted
         */

        l_terminateTransfer(__OPTIONAL_IFC_PARAMETER__);
		
        #if defined(LIN_SLAVE_LINSCI)
        LIN_CLEAR_LDUM();
		#endif

        CHANGEBAUDRATE_ARCH(__MANDATORY_IFC_PARAMETER__, LIN_WAKEUP_BAUDRATE);

        /* Begin BUGZILLA_478 */
        #if (defined(LIN_SLAVE_LINSCI) && defined(LIN_SLAVE_LINSCI_MUTE_MODE))
        {
          /*
           * disable mute mode
           */
          LIN_MUTE_MODE_DISABLE();
        }
        #endif
		#if defined(LIN_SLAVE_LINSCI)
        /* End BUGZILLA_478 */
        LIN_RESET_LBDL_BIT();
		#endif
        IFCCTRL_STATE(ifc) = LIN_STATE_BUSSLEEP;
        SET_HEADERDETECTION_ARCH(__MANDATORY_IFC_PARAMETER__, 0) ;

      } /* if bussleep timer) */
      else if (Timer.SubChannel == LIN_WAKEUP_SUBCHANNEL)
      {
         #if (LIN_WAKEUP_RETRIES_MAX > 0)
           if (TRANSFERCTRL_RETRIES(ifc) < LIN_WAKEUP_RETRIES_MAX)
         #endif
         {
          /*
           * this probably means that the master failed to respond
           * to a wakeup request
           */
          l_sendByte(__OPTIONAL_IFC_PARAMETER_COMMA__ LIN_WAKEUP_SIG);
          ++(TRANSFERCTRL_RETRIES(ifc));

          /* Begin BUGZILLA_478 */
          #if (defined(LIN_SLAVE_LINSCI) && defined(LIN_SLAVE_LINSCI_MUTE_MODE))
          {
            /*
             * disable mute mode
             */
            LIN_MUTE_MODE_DISABLE();
          }
          #endif
		  #if defined(LIN_SLAVE_LINSCI)
          /* End BUGZILLA_478 */
          LIN_RESET_LBDL_BIT();
		  #endif
          IFCCTRL_STATE(ifc) = LIN_STATE_BUSSLEEP;

          /*
           * start the wakeup timer with the correct timer value
           */
          if ((TRANSFERCTRL_RETRIES(ifc) % LIN_WAKEUP_RETRIES) != 0)
          {
             timeout = LIN_WAKEUP_TIMEOUT_VAL_SHORT
                (__MANDATORY_IFC_PARAMETER__);
             
          }
          else
          {
             timeout
                = LIN_WAKEUP_TIMEOUT_VAL_LONG(__MANDATORY_IFC_PARAMETER__);
          }

          l_TimerStart (__OPTIONAL_TIMER_PARAMETER_COMMA__
                        LIN_WAKEUP_SUBCHANNEL, timeout);
        }
        #if (LIN_WAKEUP_RETRIES_MAX > 0)
          else
          {
             #if defined(LIN_SLAVE_LINSCI)
			 LIN_CLEAR_LDUM();
			 #endif

             CHANGEBAUDRATE_ARCH(__MANDATORY_IFC_PARAMETER__, LIN_WAKEUP_BAUDRATE);

             /* Begin BUGZILLA_478 */
             #if (defined(LIN_SLAVE_LINSCI) && defined(LIN_SLAVE_LINSCI_MUTE_MODE))
             {
               /*
                * disable mute mode
                */
               LIN_MUTE_MODE_DISABLE();
             }
             #endif
			 #if defined(LIN_SLAVE_LINSCI)
             /* End BUGZILLA_478 */
             LIN_RESET_LBDL_BIT();
			 #endif
             IFCCTRL_STATE(ifc) = LIN_STATE_BUSSLEEP;
          }
        #endif

      } /* if (wakeup timer) */
      else
      {
         /*
          * these timers do not affect the current state
          */
         ;
      }
      break;
    #if (defined LIN_BAUDRATE_DETECT) && (!defined LIN_SLAVE_LIN_UART_HEADER)
      case LIN_STATE_PRE_SYNC:
    #endif /* LIN_BAUDRATE_DETECT && !LIN_SLAVE_LIN_UART_HEADER */
    case LIN_STATE_RECEIVE_SYNC:
    case LIN_STATE_RECEIVE_IDENTIFIER:
       if ((Timer.SubChannel == LIN_FRAME_SUBCHANNEL)
           ||
           (Timer.SubChannel == LIN_BUSSLEEP_SUBCHANNEL))
      {
        /*
         * Note: we cannot inform diagnostics here, since
         *       we don't have a frame identifier right now
         */
         
         l_terminateTransfer(__OPTIONAL_IFC_PARAMETER__);

      }
      break;

    case LIN_STATE_SEND_DATA:
    case LIN_STATE_RECEIVE_DATA:
      if (Timer.SubChannel == LIN_FRAME_SUBCHANNEL)
      {
          /* Begin BUGZILLA_541 */
          /* According to LIN 2.1, response_error bit must be set in case of no response or
           * incomplete response.
           * 1) error_in_response (§2.7.4) and Response_error (§2.5.2.2) flags are not set
           * if the slave does not receive any data byte (header received but no response).
           *
           * 2) If the slave receives some data bytes, but the response is not complete
           * (data bytes missing or checksum missing in response), there are 2 cases:
           * 2a) if the slave is interested in the frame, both the Error_in_response and
           * the Response_error flags shall be set.
           * 2b) if the slave is not interested both flags shall NOT be set.
           *
           * Regarding LIN 2.0 slave nodes, add a compilation option to either set
             Response_error bit (according to the conditions defined for LIN 2.1) or not. */
          #if ((defined LIN_21) || ((defined LIN_20) && (defined LIN_RESPONSE_ERROR_WHEN_INCOMPLETE_RX)))
            if ((TRANSFERCTRL_BYTESSENT(ifc) < TRANSFERCTRL_BYTESTOSEND(ifc)) && 
            	  (TRANSFERCTRL_BYTESSENT(ifc) > 1))
            {
              l_serialErrorFrameBody(__OPTIONAL_IFC_PARAMETER_COMMA__ 1);
            }
            else
          #endif
          /* End BUGZILLA_541 */
          {
            /*
             * do not set serial error flag because this is *not*
             * a hardware error!
             */
            l_serialErrorFrameBody(__OPTIONAL_IFC_PARAMETER_COMMA__ 0);
          }
      }
      else
      {
        if (Timer.SubChannel == LIN_BUSSLEEP_SUBCHANNEL)
        {
           /*
            * this should never happen, still we terminate
            * ongoing transfer (to put the driver to a defined
            * state) and restart the bussleep timer
            * (in l_terminateTransfer)
            */
           l_terminateTransfer(__OPTIONAL_IFC_PARAMETER__);
        }

      } /* if (timerId) */

      break;

    default:
      /*
       * this should never happen.
       */
      break;

  } /* switch (ifcState) */

  return ;

} /* end l_evaluateTimeout() */

#ifndef LIN_SLAVE_LINSCI_FULL
/*********************************************************************
 *
 * l_stateIdle()
 *
 */
/**
 * \brief state machine function
 *
 * This function handles incoming characters in state IDLE
 *
 * - internal function
 * - slave only
 * 
 * \param  ifc            handle of the interface
 * \param  rxChar         the character just received
 *
 * \retval none
 *
 */
STACK void l_stateIdle(__OPTIONAL_IFC_DEFINITION_COMMA__ l_u8 rxChar)
{
  /*
   * check whether we have received a BREAK signal
   */

  if (rxChar == LIN_BREAK_PATTERN)
  {
    /*
     * ok, this has been a BREAK signal
     */

    /*
     * start frame timer and change state of ifc
     */
    l_TimerStart (__OPTIONAL_TIMER_IFC_PARAMETER_COMMA__ LIN_FRAME_SUBCHANNEL, LIN_FRAME_TIMER_VALUE(__MANDATORY_IFC_PARAMETER__) );

    #ifdef LIN_SLAVE_LINSCI_AUTOSYNC
      /*
       * when in AUTOSYNC mode with BREAK detection the
       * SYNC byte will not generate an interrupt
       * except when defective: a serial error is generated in that
       * case
       */
      IFCCTRL_STATE(ifc) = LIN_STATE_RECEIVE_IDENTIFIER;
    #else
      IFCCTRL_STATE(ifc) = LIN_STATE_RECEIVE_SYNC;
    #endif /* LIN_SLAVE_LINSCI_AUTOSYNC */
  }
  #ifdef LIN_SLAVE_LIN_UART_HEADER
    else
    {
      /*
       * no valid break character, restart bussleep timer
       */
      #ifdef __MORE_IFCS__
        if (Timer.SubChannel[(l_timer_handle) ifc] != LIN_WAKEUP_SUBCHANNEL)
      #else
        if (Timer.SubChannel != LIN_WAKEUP_SUBCHANNEL)
      #endif
      {
        l_TimerStart(__OPTIONAL_TIMER_PARAMETER_COMMA__
                     LIN_BUSSLEEP_SUBCHANNEL,
                     LIN_BUSSLEEP_TIMEOUT_VAL
                     (__MANDATORY_IFC_PARAMETER__));
        #ifdef LIN_SLAVE_LIN_UART_HEADER
          SET_HEADERDETECTION_ARCH(__MANDATORY_IFC_PARAMETER__, 1);
        #endif
    }
  #endif

  return ;

} /* end l_stateIdle() */
#endif /* LIN_SLAVE_LINSCI_FULL */

#if (defined LIN_BAUDRATE_DETECT) && (!defined LIN_SLAVE_LIN_UART_HEADER)
  /*********************************************************************
   *
   * l_statePreIdle()
   *
   */
  /**
   * \brief state machine function
   *
   * This function handles incoming characters in state PRE_IDLE
   * \attention this function is used only when baudrate detection
   *            is active
   *
   * - internal function
   * - slave only
   * 
   * \param  ifc            handle of the interface
   * \param  rxChar         the character just received
   *
   * \retval none
   *
   */
  STACK static void l_statePreIdle(__OPTIONAL_IFC_DEFINITION_COMMA__ l_u8 rxChar)
  {
    #ifndef LIN_BAUDRATE_MODE_SLOPPY
      if (rxChar == LIN_BREAK_PATTERN)
    #else
      /*
       * Dummy operation to prevent compiler warnings
       */
      {
        rxChar = rxChar;
      }
    #endif
    {
      IFCCTRL_STATE(ifc) = LIN_STATE_PRE_SYNC;
    }

   return ;
  } /* end l_statePreIdle() */

  /*********************************************************************
   *
   * l_statePreSync()
   *
   */
  /**
   * \brief state machine function
   *
   * This function handles incoming characters in state PRE_SYNC
   *
   * \attention this function is used only when baudrate detection
   *            is active
   *
   * - internal function
   * - slave only
   * 
   * \param  ifc            handle of the interface
   * \param  rxChar         the character just received
   *
   * \retval none
   *
   */
  STACK static void l_statePreSync(__OPTIONAL_IFC_DEFINITION_COMMA__ l_u8 rxChar)
  {
    if (rxChar == LIN_SYNC_PATTERN)
    {
      /*
       * very well, we do have a valid baudrate
       */
      l_TimerStop (__OPTIONAL_TIMER_IFC_PARAMETER__);

      /* Begin BUGZILLA_540 */
      #if (defined(LIN_21) && defined(LIN_INCLUDE_COOKED_TP))
        l_TimerStop_N_As(__OPTIONAL_TIMER_IFC_PARAMETER__);
        l_TimerStop_N_Cr(__OPTIONAL_TIMER_IFC_PARAMETER__);
      #endif /* LIN_21 && LIN_INCLUDE_COOKED_TP */
      /* End BUGZILLA_540 */

      IFCCTRL_BAUDRATEFLAGS(ifc) = LIN_BAUDRATE_DETERMINED;

      IFCCTRL_STATE(ifc) = LIN_STATE_RECEIVE_IDENTIFIER;
    }
    else
    {
      /*
       * this does not seem to be the correct baudrate
       */
      IFCCTRL_STATE(ifc) = LIN_STATE_PRE_IDLE;

      /*
       * inform the application (if the time is right)
       */
      if (!(IFCCTRL_BAUDRATEFLAGS(ifc) & LIN_BAUDRATE_KEEP_SILENT) )
      {
        BAUDRATECALLBACK(__MANDATORY_IFC_PARAMETER__, IFCCTRL_NORMALBAUDRATE(ifc));
      }
    }

    return ;

  } /* end l_statePreSync() */

  /*********************************************************************
   *
   * l_change_baudrate()
   *
   */
  /**
   * \brief function to change baudrate
   *
   * This is an STMicroelectronics specific API function that allows the user to
   * change the baudrate of the serial interface
   *
   * \attention this function is used only when baudrate detection
   *            is active
   *
   * - STMicroelectronics specific API
   * - slave only
   * 
   * \param  ifc          the interface to change the baudrate for
   * \param  baudrate     value of the baudrate to set on the interface
   *
   * \retval none
   *
   */
  STACK void l_change_baudrate(l_ifc_handle ifc, l_u16 baudrate) REENTRANT
  {
    /*
     * change baudrate here, but we must ensure that there is no
     * interrupt pending before we change the baudrate...
     */

    IFCCTRL_NORMALBAUDRATE(ifc) = baudrate;

    /* Begin BUGZILLA_472 */
    IFCCTRL_FRAMETIMERVALUE(ifc)
       = ((l_u8) ((l_u16)((l_u16) ((l_u16) ((l_u16) (LIN_BITS_PER_FRAME * LIN_FRAME_TIME_MULTIPLIER))
                   / ((l_u16) (IFCCTRL_NORMALBAUDRATE(ifc) / (l_u16) 10)))) + (l_u16) 1));
    /* End BUGZILLA_472 */

    IFCCTRL_BAUDRATEFLAGS(ifc)  = LIN_BAUDRATE_KEEP_SILENT;

    l_terminateTransfer(__OPTIONAL_IFC_PARAMETER__);

    l_TimerStart (__OPTIONAL_TIMER_IFC_PARAMETER_COMMA__ LIN_BAUDRATE_SUBCHANNEL, LIN_FRAME_TIMER_VALUE(__MANDATORY_IFC_PARAMETER__) );
    INIT_ARCH(ifc);

    return ;

  } /* end l_change_baudrate() */

#endif /* LIN_BAUDRATE_DETECT && !LIN_SLAVE_LIN_UART_HEADER */

#ifndef LIN_SLAVE_LINSCI_FULL
/*********************************************************************
 *
 * l_stateReceiveSync()
 *
 */
/**
 * \brief state machine function
 *
 * This function handles incoming characters in state RECEIVE_SYNC
 *
 * - internal function
 * - slave only
 *
 * \param  ifc            handle of the interface
 * \param  rxChar         the character received
 *
 * \retval none
 *
 */
STACK void l_stateReceiveSync (__OPTIONAL_IFC_DEFINITION_COMMA__ l_u8 rxChar)
{
  if (rxChar == LIN_SYNC_PATTERN)
  {
    /*
     * a valid sync pattern, switch to state receive identifier
     */
    IFCCTRL_STATE(ifc) = LIN_STATE_RECEIVE_IDENTIFIER;

  }
  else
  {
    /*
     * this is not a valid sync pattern
     */
      l_terminateTransfer(__OPTIONAL_IFC_PARAMETER__);

  }

  return ;

} /* end l_stateReceiveSync() */
#endif /* LIN_SLAVE_LINSCI_FULL */

/*********************************************************************
 *
 * l_stateBussleep()
 *
 */
/**
 * \brief state machine function
 *
 * Handles an incoming character in state BUSSLEEP
 *
 * - internal function
 * - slave only
 *
 * \param  ifc            handle of the interface
 * \param  rxChar         the character received
 *
 * \retval none
 *
 */
STACK static void l_stateBussleep (__OPTIONAL_IFC_DEFINITION_COMMA__ l_u8 rxChar)
{
  /*
   * only valid wakeup patterns are accepted here
   */
  if ((rxChar & LIN_RECEIVE_WAKEUP_SIG) == 0)
  {
    CHANGEBAUDRATE_ARCH(__MANDATORY_IFC_PARAMETER__,
                        IFCCTRL_NORMALBAUDRATE(ifc));
  	#if defined(LIN_SLAVE_LINSCI)
    LIN_SET_LBDL_BIT();
	#endif
    #if (defined LIN_BAUDRATE_DETECT) && (!defined LIN_SLAVE_LIN_UART_HEADER)
      if (!(IFCCTRL_BAUDRATEFLAGS(ifc) & LIN_BAUDRATE_DETERMINED))
      {
        IFCCTRL_STATE(ifc) = LIN_STATE_PRE_IDLE;
      }
      else
    #endif /* LIN_BAUDRATE_DETECT && !LIN_SLAVE_LIN_UART_HEADER */
    {
      IFCCTRL_STATE(ifc) = LIN_STATE_IDLE;
      SET_HEADERDETECTION_ARCH(__MANDATORY_IFC_PARAMETER__, 1) ;
    }

    /*
     * restart bussleep timer
     */
    #ifdef __MORE_IFCS__
       if (Timer.SubChannel[(l_timer_handle) ifc] != LIN_WAKEUP_SUBCHANNEL)
    #else
       if (Timer.SubChannel != LIN_WAKEUP_SUBCHANNEL)
    #endif
    {
      l_TimerStart(__OPTIONAL_TIMER_PARAMETER_COMMA__
                   LIN_BUSSLEEP_SUBCHANNEL,
                   LIN_BUSSLEEP_TIMEOUT_VAL
                   (__MANDATORY_IFC_PARAMETER__));
    }
    /* Begin BUGZILLA_473 */
    #ifdef LIN_21
       IFCCTRL_STATUSWORD(ifc) |= LIN_IFC_STATUS_BUS_ACTIVITY;
    #endif /* LIN_21 */
    /* End BUGZILLA_473 */
  } /* if (wakeup pattern) */

  return ;

} /* end l_stateBussleep() */

/*********************************************************************
 *
 * l_serialError()
 *
 */
/**
 * \brief (serial) error handling function
 *
 * This function will be called if a serial error has been detected
 *
 * - internal function
 * - slave only
 *
 * \param  ifc            handle of the interface
 *
 * \retval none
 *
 */
STACK void l_serialError (__OPTIONAL_IFC_DEFINITION__)
{
  /*
   * error flag must not been cleared here since it indicates
   * a hardware error, and this information is used when
   * aborting a transfer
   */
  
  /*
   * the handling of the error depends on the state of the
   * interface
   */
  switch ((l_ifcState_t) IFCCTRL_STATE(ifc))
  {
    #if (defined LIN_BAUDRATE_DETECT) && (!defined LIN_SLAVE_LIN_UART_HEADER)
      case LIN_STATE_PRE_IDLE:
        /*
         * a BREAK signal may be too long to be detected correctly,
         * hence we pretend to have received a valid BREAK
         */
        l_statePreIdle(__OPTIONAL_IFC_PARAMETER_COMMA__ LIN_BREAK_PATTERN);

        break;

      case LIN_STATE_PRE_SYNC:
        /*
         * this could possibly be another BREAK due to a previous
         * WAKEUP request... we shall ignore it *once*
         */
        if (!(IFCCTRL_BAUDRATEFLAGS(ifc) & LIN_BAUDRATE_WAKEUP))
        {
          IFCCTRL_BAUDRATEFLAGS(ifc) |= LIN_BAUDRATE_WAKEUP;
        }
        else
        {
          l_statePreSync(__OPTIONAL_IFC_PARAMETER_COMMA__ LIN_SYNC_PATTERN - 1);
        }
        break;
    #endif  /* LIN_BAUDRATE_DETECT && !LIN_SLAVE_LIN_UART_HEADER */

    case LIN_STATE_IDLE:
      /*
       * a BREAK signal may be too long to be detected correctly,
       * hence we pretend to have received a valid BREAK
       */
      #ifdef LIN_SLAVE_STANDARD_UART
          l_stateIdle(__OPTIONAL_IFC_PARAMETER_COMMA__ LIN_BREAK_PATTERN);
      #elif defined(LIN_SLAVE_LINSCI_BREAK_DETECTION)
          /*
           * this can happen if a wakeup signal was too long
           */
          l_stateIdle(__OPTIONAL_IFC_PARAMETER_COMMA__ LIN_BREAK_PATTERN);
      #elif defined(LIN_SLAVE_LINSCI_FULL)
          /*
           * do nothing here
           */
          
      #else
          l_stateIdle(__OPTIONAL_IFC_PARAMETER_COMMA__ LIN_INVALID_PATTERN);
      #endif  /* LIN_SLAVE_STANDARD_UART */
      break;

    case LIN_STATE_SEND_DATA:
    case LIN_STATE_RECEIVE_DATA:

       l_serialErrorFrameBody(__OPTIONAL_IFC_PARAMETER_COMMA__ 1);

      break;

    case LIN_STATE_RECEIVE_SYNC:
    case LIN_STATE_RECEIVE_IDENTIFIER:

      /*
       * the transfer is corrupted and should be ignored
       */
      l_terminateTransfer(__OPTIONAL_IFC_PARAMETER__);

      break;

    case LIN_STATE_BUSSLEEP:
      /*
       * the wakeup signal has no defined maximum length, so it
       * is possible that a valid wakeup signal causes a serial error
       * We accept this and try to wake up
       */
      l_stateBussleep(__OPTIONAL_IFC_PARAMETER_COMMA__ LIN_WAKEUP_SIG);
      break;

    default:
      /*
       * this should never happen, so do nothing here
       */
      break;

  } /* switch (state) */

  /*
   * clear the serial error flag, we have handled it
   */
  IFCCTRL_SERIALERROR(ifc) = 0;

  return ;

} /* end l_serialError() */


#if (!defined(LIN_13) || defined(LIN_INCLUDE_2x_SLEEP_MODE_API))
  /*********************************************************************
   *
   * l_ifc_wake_up()
   *
   */
  /**
   * \brief function to request wake up
   *
   * This function is called by the application to wake up the bus
   *
   * - standard API
   * - slave only
   *
   * \param  ifc            handle of the interface
   *
   * \retval none
   *
   */
  STACK void l_ifc_wake_up (l_ifc_handle ifc) REENTRANT
  {

    l_irqmask irqMask;

    __CHECK_INTERFACE_VOID__(ifc);
    /*
     * accept command only in state bussleep
     */
    irqMask = l_sys_irq_disable();

    if (IFCCTRL_STATE(ifc) == LIN_STATE_BUSSLEEP)
    {
      CHANGEBAUDRATE_ARCH(__MANDATORY_IFC_PARAMETER__, IFCCTRL_NORMALBAUDRATE(ifc));
      l_sendByte(__OPTIONAL_IFC_PARAMETER_COMMA__ LIN_WAKEUP_SIG);
      TRANSFERCTRL_RETRIES(ifc) = 1;
      l_TimerStart  (__OPTIONAL_TIMER_IFC_PARAMETER_COMMA__ LIN_WAKEUP_SUBCHANNEL, LIN_WAKEUP_TIMEOUT_VAL_SHORT(__MANDATORY_IFC_PARAMETER__) );

    } /* if (bussleep) */

    l_sys_irq_restore(irqMask);
  } /* end l_ifc_wake_up() */
#endif /* !LIN_13 && LIN_INCLUDE_2x_SLEEP_MODE_API */

#ifndef LIN_13
  /*********************************************************************
   *
   * l_changeFrameId()
   *
   */
  /**
   * \brief function to change a frame identifier
   *
   * For the slave node this function changes a frame identifier of
   * a frame with a certain message id
   * \attention the driver will perform some checks so that not
   *            all frames identifiers can be set
   * \attention this function shall not be used by the application to
   *            configure the driver. Use l_ifc_ioctl() instead!
   *
   * - internal function
   * - slave only
   *
   * \param  messageId      the id of the message to assign frame id to
   * \param  frameId        identifier of the message (protected or not)
   *
   * \retval 1 if the message has been found and the identifier has been
   *     changed
   * \retval 0 if the frame identifier has not been set
   *
   */
  STACK l_bool l_changeFrameId (l_u16 messageId, l_u8 frameId)
  {
    l_u8           retval = 0;
    l_u8 i;
    /* Begin BUGZILLA_472 */
    l_u8           temp = ((l_u8) (frameId & LIN_FRAME_ID_MASK));
    /* End BUGZILLA_472 */

    #ifdef LIN_INCLUDE_PID_PARITY_CHECK
			/* Begin PR_8 */    
			#ifdef LIN_SLAVE_LINSCI /* slave LINSCI */
			    /* Begin BUGZILLA_539 */
					if (LIN_PARITY_EN()) /* Parity control is enabled */
      		{
              if (LIN_PARITY_CHECK()) /* Parity error */
			    /* End BUGZILLA_539 */
      			  {
        		  		return retval;
      			  }
      		}
			    else /* Parity control is disabled */
      		{
              /* Begin BUGZILLA_472 */
              if ((frameId != l_makeProtId(temp))
              /* End BUGZILLA_472 */
		        	      &&
        			    (frameId != LIN_INVALID_ID_API))
      				{
		        			return retval;
      				}
      		}
			#else		/* slave SCI */
          /* Begin BUGZILLA_472 */
					if ((frameId != l_makeProtId(temp))
          /* End BUGZILLA_472 */
        		    &&
        		  (frameId != LIN_INVALID_ID_API))
      		{
        		 return retval;
      		}
			#endif /* LIN_SLAVE_LINSCI */
			/* End PR_8 */     
    #endif
    for (i = 0; i < LIN_FRAME_NR_MAX; ++i)
    {
      if ((l_pMessageId[i] == messageId)
          ||
          (messageId == LD_ANY_MESSAGE))
      {
         if ((l_pFrameId[i] != LIN_MASTER_REQ_FRAME_ID)
             &&
             (l_pFrameId[i] != LIN_SLAVE_RESP_FRAME_ID)
             &&
             (l_pFrameId[i] != LIN_USER_DEFINED_FRAME_ID)
             &&
             (l_pFrameId[i] != LIN_RESERVED_FRAME_ID))
         {
            l_pFrameId[i] = (l_u8) frameId;

            retval = 1;
         }
      }
    } /* for (all frames) */

    return retval;

  } /* end l_changeFrameId() */
#endif


#ifdef LIN_21
  /*********************************************************************
   *
   * l_changePIDs()
   *
   */
  /**
   * \brief function to change a frame identifier
   *
   * For the slave node this function changes a frame identifier of
   * a frame with a certain message id
   * \attention the driver will perform some checks so that not
   *            all frames identifiers can be set
   * \attention this function shall not be used by the application to
   *            configure the driver. Use l_ifc_ioctl() instead!
   *
   * - internal function
   * - slave only
   *
   * \param  messageId      the id of the message to assign frame id to
   * \param  frameId        identifier of the message (protected or not)
   *
   * \retval 1 if the message has been found and the identifier has been
   *     changed
   * \retval 0 if the frame identifier has not been set
   *
   */
  STACK l_bool l_changePIDs(l_u8 startId, const l_u8 *pidBuffer)
  {
    #define MAX_PID_NUMBER_CHANGEABLE      4
    #define PID_DO_NOT_CARE             0xFF
    l_bool retval = 0;
    l_u8 i;
    l_u8 modified = 0;
    /* Begin BUGZILLA_1144 */
    l_bool         pid_ok       = 1; /* 1 --> OK, 0 --> error */
    /* End   BUGZILLA_1144 */
    l_bool error = 0;
    /* Begin BUGZILLA_538 */
    l_u8 real_startId = 0;
    /* End BUGZILLA_538 */
    
    /* Begin BUGZILLA_1144 */
    for (i = 0; i < MAX_PID_NUMBER_CHANGEABLE; ++i)
    {
      if ((pidBuffer[i] == LIN_MASTER_REQ_FRAME_ID)
          ||
          (pidBuffer[i] == LIN_SLAVE_RESP_FRAME_ID)
          ||
          (pidBuffer[i] == LIN_USER_DEFINED_FRAME_ID)
          ||
          (pidBuffer[i] == LIN_RESERVED_FRAME_ID))
      {
        pid_ok = 0; /* One of the input PIDs has a reserved value, then exit */
      }
      else
      {
        /* 'else' block inserted for the MISRA rules */
        NoOperation();
      }
    }

    if (pid_ok)
    {
    /* End   BUGZILLA_1144 */
    
	    /* Begin BUGZILLA_472 */
	    if (startId < (LIN_FRAME_COUNT - 2))
	    /* End BUGZILLA_472 */
	    {
	    	/* Begin BUGZILLA_538 */
	    	/* This instruction expects a certain order of the configurable LIN Frames. The
	       * order has to be according to the configurable frames in the LDF file. The
	       * Lingen tools sorts the frames according to the initial ID => this leads to
	       * problems in the conformance test. Then, Lingen generates a new array respecting
	       * the exact order of "configurable_frames" section in the LDF file. Using this
	       * array we can modify exactly what requested by the user.
	       * Frame order of l_pFrameId[] array generated by Lingen cannot be modified to
	       * respect the exact order of "configurable_frames" section in the LDF file, since
	         other functions needing this order will not work. */
	      real_startId = ((l_u8)(l_pConfigFrameOrder[startId] - ((l_u8) 1)));
	      for (i = 0; i < MAX_PID_NUMBER_CHANGEABLE; ++i)
	      {
	         if ((l_pFrameId[real_startId] != LIN_MASTER_REQ_FRAME_ID)
	             &&
	             (l_pFrameId[real_startId] != LIN_SLAVE_RESP_FRAME_ID)
	             &&
	             (l_pFrameId[real_startId] != LIN_USER_DEFINED_FRAME_ID)
	             &&
	             (l_pFrameId[real_startId] != LIN_RESERVED_FRAME_ID)
	             &&
	             /* Begin BUGZILLA_537 */
	             /* If only one frame should be reconfigured, this works only with index 0.
	              * If two frames should be reconfigured, this works only with  index 1
	                and so on... */
	             (pidBuffer[i] != PID_DO_NOT_CARE))
	             /* End BUGZILLA_537 */
	         {
	            if (modified <= (LIN_FRAME_COUNT - 2))
	            {
	              l_pFrameId[real_startId] = (l_u8) pidBuffer[i];
	              startId++;
	              real_startId = ((l_u8)(l_pConfigFrameOrder[startId] - ((l_u8) 1)));
	              /* End BUGZILLA_538 */
	              modified++;
	            }
	            else
	            {
	            	/* Begin BUGZILLA_1144 */
              	pid_ok = 0;
                /* End   BUGZILLA_1144 */
	            }
	         }
	         /* Begin BUGZILLA_1022 */
					 else
					 {
						 startId++;
						 real_startId = ((l_u8)(l_pConfigFrameOrder[startId] - ((l_u8) 1)));					 
					 }
					 /* End BUGZILLA_1022  */
	      }
	    /* Begin BUGZILLA_1144 */
	    } /* for (all frames) */
	    else
	    {
	      pid_ok = 0;
	    }
  	}
  	else
    {
      /* 'else' block inserted for the MISRA rules */
      NoOperation();
    }    
    retval = pid_ok;
    /* End   BUGZILLA_1144 */    
    return retval;

  } /* end l_changePIDs() */
#endif /* LIN_21 */


/*********************************************************************
 *
 * l_gotoSleep()
 *
 */
/**
 * \brief internal sleep mode function
 *
 * This function puts the slave node driver into sleep mode
 * \attention This function shall not be used to put the driver into
 *            sleep mode. If an application really needs to do so, it
 *            shall use l_ifc_ioctl()!
 * 
 * - internal function
 * - slave only
 *
 * \param  ifc            handle of the interface
 *
 * \retval none
 *
 */
STACK void l_gotoSleep (__OPTIONAL_IFC_DEFINITION__)
{
  l_TimerStop   (__OPTIONAL_TIMER_IFC_PARAMETER__);

  /* Begin BUGZILLA_540 */
  #if (defined(LIN_21) && defined(LIN_INCLUDE_COOKED_TP))
    l_TimerStop_N_As(__OPTIONAL_TIMER_IFC_PARAMETER__);
    l_TimerStop_N_Cr(__OPTIONAL_TIMER_IFC_PARAMETER__);
  #endif /* LIN_21 && LIN_INCLUDE_COOKED_TP */
  /* End BUGZILLA_540 */

  l_ifcSetStatus(__OPTIONAL_IFC_PARAMETER_COMMA__ LIN_STATUS_BUSSLEEP, 0);

  #if defined(LIN_SLAVE_LINSCI)
  LIN_CLEAR_LDUM();
  #endif
  CHANGEBAUDRATE_ARCH(__MANDATORY_IFC_PARAMETER__, LIN_WAKEUP_BAUDRATE);

  /* Begin BUGZILLA_478 */
  #if (defined(LIN_SLAVE_LINSCI) && defined(LIN_SLAVE_LINSCI_MUTE_MODE))
  {
    /*
     * disable mute mode
     */
     LIN_MUTE_MODE_DISABLE();
  }
  #endif
  #if defined(LIN_SLAVE_LINSCI)
  /* End BUGZILLA_478 */
  LIN_RESET_LBDL_BIT();
  #endif
  IFCCTRL_STATE(ifc) = LIN_STATE_BUSSLEEP;

  SET_HEADERDETECTION_ARCH(__MANDATORY_IFC_PARAMETER__, 0) ;

  return ;
  
} /* end l_gotoSleep() */

/*********************************************************************
 *
 * l_terminateTransfer()
 *
 */
/**
 * \brief function to stop an ongoing transfer
 *
 * This function terminates an ongoing transfer on the bus.
 * The control structure will be reset (hence all data to be
 * transferred will be deleted), the baudrate of the interface will
 * be set to the BREAK value for that interface
 *
 * - internal function
 * - slave only
 *
 * \param  ifc            the handle of the interface
 *
 * \retval none
 *
 */
STACK void l_terminateTransfer(__OPTIONAL_IFC_DEFINITION__)
{

   /* Start bussleep timer */
  l_TimerStart(__OPTIONAL_TIMER_IFC_PARAMETER_COMMA__ LIN_BUSSLEEP_SUBCHANNEL, LIN_BUSSLEEP_TIMEOUT_VAL(__MANDATORY_IFC_PARAMETER__));

  /*
   * set all variables to default values
   */

  l_initTransferCtrl(__OPTIONAL_IFC_PARAMETER__);
  #ifdef LIN_INCLUDE_DIAGNOSTICS
    IFCCTRL_DIAGFLAGS(ifc) = 0;
  #endif

  if (IFCCTRL_STATE(ifc) != LIN_STATE_BUSSLEEP)
  {
     LIN_SLAVE_RESET_NOMINAL_BAUDRATE();
     #ifdef LIN_SLAVE_STANDARD_UART
       if (IFCCTRL_SERIALERROR(ifc) != 0)
       {
         #if (defined LIN_BAUDRATE_DETECT) && (!defined LIN_SLAVE_LIN_UART_HEADER)
           if (!(IFCCTRL_BAUDRATEFLAGS(ifc) & LIN_BAUDRATE_DETERMINED))
           {
             IFCCTRL_STATE(ifc) = LIN_STATE_PRE_IDLE;
           }
           else
         #endif  /* LIN_BAUDRATE_DETECT && !LIN_SLAVE_LIN_UART_HEADER */
         {
            /*
             * this could be a break character
             */
            l_stateIdle(LIN_BREAK_PATTERN);
            return ;
         }
       } /* if (serial error) */
       else   
    #endif /* LIN_SLAVE_STANDARD_UART */
    {
      #if (defined LIN_BAUDRATE_DETECT) && (!defined LIN_SLAVE_LIN_UART_HEADER)
        if (!(IFCCTRL_BAUDRATEFLAGS(ifc) & LIN_BAUDRATE_DETERMINED) )
        {
          IFCCTRL_STATE(ifc) = LIN_STATE_PRE_IDLE;
        } 
        else
      #endif  /* LIN_BAUDRATE_DETECT && !LIN_SLAVE_LIN_UART_HEADER */
      {
        IFCCTRL_STATE(ifc) = LIN_STATE_IDLE;
        SET_HEADERDETECTION_ARCH(__MANDATORY_IFC_PARAMETER__, 1) ;
      }
    } 
  } /* if (!bussleep) */
  
  return ;

} /* end l_terminateTransfer() */


/*********************************************************************
 *
 * documentation of variables and functions defined in code
 * generated by lingen
 *
 *********************************************************************/

/**
 * \var l_speed
 *
 * \brief Array holding the bus speed for every interface
 */

/**
 * \var l_initialNad
 *
 * \brief the NAD of the slave node configured in the LDF. \n
 * it is used for Assign_NAD request
 */

/**
 * \var l_productId
 *
 * \brief the product id of the slave node, as given in the LDF. \n
 * the variant id may be changed by the application
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

/**
 * \var l_pMessageId
 *
 * \brief the message ids of all frames known to the slaves
 */

/**
 * \var l_pFrameId
 *
 * \brief the protected identifers of all frames known to the slave \n
 * at startup, these identifiers are usually invalid!
 */


/*
 * eof lin_slave.c
 */


