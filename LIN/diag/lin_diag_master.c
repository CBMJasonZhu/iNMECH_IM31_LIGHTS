/*********************************************************************
 *
 * Project:      LIN 2.1 driver suite
 *
 * File name:    lin_diag_master.c
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
 * 20090420  v1.1   gsf  - BUGZILLA_472: LIN driver does not compile if 
 *                         Cosmic "strict" option is enabled.
 *                         BUGZILLA_540: LIN 2.1 / N_Cr_LIN timeout
 *                         and N_As_LIN timeout management missing.
 *
 *********************************************************************/

#define LIN_DIAG_MASTER_C

#define LIN_DIAG_MASTER_C_MAJOR_VERSION  1
#define LIN_DIAG_MASTER_C_MINOR_VERSION  31
#define LIN_DIAG_MASTER_C_PATCH_VERSION  1

#define LIN_DIAG_MASTER_C_STM

/*********************************************************************
 * includes
 *********************************************************************/

#include "lin_diag_master.h"
#include "lin_cfg_types.h"
#include "lin_types.h"
#include "lin_general.h"
#include "lin_diag.h"
#include "lin_master.h"
#include "lin_def.h"
#include "lin_def_gen.h"
#include "lin_def_arch_include.h"

#include "lin_version_control.h"


/*********************************************************************
 * local macros
 *********************************************************************/

/*********************************************************************
 * local typedefs
 *********************************************************************/

/*********************************************************************
 * global constants
 *********************************************************************/
#ifdef __MORE_IFCS__
  /** size of class 2 request message */
  static l_u8        l_diag2Size[LIN_IFC_NR_MAX];

  /** RSID of class 2 response */
  static l_u8        l_diag2Rsid[LIN_IFC_NR_MAX];

  /** error code of configuration request */
  static l_u8        l_diag2ErrorCode[LIN_IFC_NR_MAX];

  /** buffer for Read By Id requests */
  static l_u8 *      l_diag2ReadBuffer[LIN_IFC_NR_MAX];

  /** control flags of configuration module */
  l_u8               l_diag2Flags[LIN_IFC_NR_MAX];

  /** frame buffer for configuration requests */
  static l_u8        l_diag2Buffer[LIN_IFC_NR_MAX][LIN_DIAG_FRAME_LEN_MAX];

  /** diagnostic flags */
  l_u8               l_diagFlags[LIN_IFC_NR_MAX];

#else

  /** size of class 2 request message */
  static l_u8        l_diag2Size;

  /** RSID of class 2 response */
  static l_u8        l_diag2Rsid;

  /** error code of configuration request */
  static l_u8        l_diag2ErrorCode;

  /** buffer for Read By Id requests */
  static l_u8 *      l_diag2ReadBuffer;

  /** control flags of configuration module */
  l_u8               l_diag2Flags;

  /** frame buffer for configuration requests */
  static l_u8        l_diag2Buffer[LIN_DIAG_FRAME_LEN_MAX];

  /** diagnostic flags */
  l_u8               l_diagFlags;
#endif

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

STACK static l_bool ld_diag2RxHandler(__OPTIONAL_IFC_DEFINITION_COMMA__
                                       const l_u8 * pBuffer);

#ifdef LIN_INCLUDE_DIAGNOSTICS
  /*********************************************************************
   *
   * ld_composeMasterRequest()
   */
  /**
   * \brief function to compose a master request frame
   *
   * This diagnostic function composes a master request frame. When
   * the function is called it has already been checked that there is
   * something to send
   *
   * - internal function
   * - master only
   *
   * \param  ifc            the handle of the interface
   * \param  pBuffer        frame buffer section to code signals to
   *                        if needed
   * \retval  none
   *
   */
  STACK void ld_composeMasterRequest(__OPTIONAL_IFC_DEFINITION_COMMA__
                                     l_u8 *  pBuffer)
  {
     l_u8                     consumed = 0;
     
     if ((LIN_DIAG_FLAGS(ifc) & ((l_u8) LIN_USER_DIAG_TX_READY)) == 0)
     {
        #ifdef LIN_INCLUDE_DIAG2
          if ((LIN_DIAG_FLAGS(ifc) & ((l_u8) LIN_DIAG2_TX_READY)) != 0)
          {
            /*
             * diag 2 ready to send, simply copy the buffer
             */
            pBuffer[0] = LIN_DIAG2_BUFFER(ifc)[0];
            pBuffer[1] = LIN_DIAG2_BUFFER(ifc)[1];
            pBuffer[2] = LIN_DIAG2_BUFFER(ifc)[2];
            pBuffer[3] = LIN_DIAG2_BUFFER(ifc)[3];
            pBuffer[4] = LIN_DIAG2_BUFFER(ifc)[4];
            pBuffer[5] = LIN_DIAG2_BUFFER(ifc)[5];
            pBuffer[6] = LIN_DIAG2_BUFFER(ifc)[6];
            pBuffer[7] = LIN_DIAG2_BUFFER(ifc)[7];

            LIN_DIAG2_FLAGS(ifc) |= (l_u8) LIN_DIAG2_EXPECT_RESPONSE;
            LIN_DIAG_FLAGS(ifc) &= (l_u8) ~LIN_DIAG2_TX_READY;

            consumed = 1;
          }
        #endif

        #ifdef LIN_INCLUDE_COOKED_TP
          if (!consumed)
          {
            if ((LIN_DIAG_FLAGS(ifc) & ((l_u8) LIN_COOKED_TX_READY)) != 0)
            {
              consumed =
                 ld_diag3CookedTxHandler(__OPTIONAL_IFC_PARAMETER_COMMA__
                                         pBuffer);
            }
          }
        #endif

        #ifdef LIN_INCLUDE_RAW_TP
          if (!consumed)
          {
             if ((LIN_DIAG_FLAGS(ifc) & (l_u8) LIN_RAW_TX_READY) != 0)
             {
                consumed =
                   ld_diag3RawTxFifoGet(__OPTIONAL_IFC_PARAMETER_COMMA__
                                        pBuffer);
             }
          }
        #endif
     }
     else
     {
        LIN_DIAG_FLAGS(ifc) &= (l_u8) ~LIN_USER_DIAG_TX_READY;
     }
     return ;

  } /* end ld_composeMasterRequest() */

  /*********************************************************************
   *
   * ld_transmittedMasterRequest()
   */
  /**
   * \brief function indicating succesful transmission of master request
   *
   * This function is called whenever a master request frame has been
   * succesfully transmitted.
   * It mainly sets the flags right.
   *
   * - internal function
   * - master only
   *
   * \param  ifc            interface to act upon
   *
   * \retval none
   *
   */
  STACK void ld_transmittedMasterRequest(__OPTIONAL_IFC_DEFINITION__)
  {

     #ifdef LIN_INCLUDE_COOKED_TP
       if ((LIN_DIAG_FLAGS(ifc) & LIN_ORIGINATOR_COOKED_TX) != 0)
       {
          LIN_COOKED_TP_FLAGS(ifc) &= (l_u8) ~LIN_COOKED_TX_FLAG_MASK;
          LIN_COOKED_TP_FLAGS(ifc) |= LIN_COOKED_TX_SUCCESS;
          #ifdef LIN_21
            /* Reset timeout flag */
            LIN_COOKED_TP_TIMEOUT_FLAGS(ifc)  = 0;
            /* Set status of the Master or Slave Node Transmission Handler to Idle */
            l_ifc_set_TP_status(LIN_STATUS_IDLE);
          #endif /* LIN_21 */
       }
       /* Begin BUGZILLA_540 */
       #ifdef LIN_21
         #ifndef N_AS_TIMEOUT           /* If not defined in lin_def.h, we define the default value */
           #define N_AS_TIMEOUT 1000    /* 1000 msec is the default value */
         #endif /* N_AS_TIMEOUT */
         if ((LIN_DIAG_FLAGS(ifc) & (l_u8) LIN_COOKED_TX_READY) != 0)
         {
           /* There is yet something to send: reload timer N_As */
           l_TimerStart_N_As(__OPTIONAL_TIMER_IFC_PARAMETER_COMMA__ N_AS_TIMEOUT );
         }
         else
	       {
	         /* This is the last CF sent correctly, so here we stop the LIN 2.1 N_As timer */
           l_TimerStop_N_As(__OPTIONAL_TIMER_IFC_PARAMETER__);
         }
       #endif /* LIN_21 */
       /* End BUGZILLA_540 */
     #endif

     /*
      * clear originator
      */
     LIN_DIAG_FLAGS(ifc) &= (l_u8) ~LIN_DIAG_ORIGINATOR_MASK;

     return ;

  } /* end ld_transmittedMasterRequest() */
#endif /* LIN_INCLUDE_DIAGNOSTICS */

/*********************************************************************
 *********************************************************************
 *********************************************************************
 **
 ** the diagnostic class 2 module
 **
 *********************************************************************
 *********************************************************************
 *********************************************************************/

/*********************************************************************
 *
 * LIN diag level 2 API
 *
 *********************************************************************/

#ifdef LIN_INCLUDE_DIAG2

  /*********************************************************************
   *
   * ld_is_ready()
   */
  /**
   * \brief function indicating a busy diagnostic module
   *
   * This function indicates whether the diagnostic level 2 module is
   * ready for a new request. The application must not issue a new
   * level 2 diagnostic request until this function has returned TRUE
   *
   * - diagnostic API
   * - master only
   *
   * \param  ifc            handle of the interface to check
   *
   * \LIN 2.0
   * \retval  1 if the diagnostic module is ready for a new request
   * \retval  0 otherwise
   *
   * \LIN 2.1
   * \retval LD_SERVICE_BUSY     service is on going
   * \retval LD_REQUEST_FINISHED the configuration request has been completed
   * \retval LD_SERVICE_IDLE     the configuration req./resp. combination has been completed
   * \retval LD_SERVICE_ERROR    the configuration request or response experienced a bus error
   *
   */
#ifdef LIN_21
  STACK l_u8 ld_is_ready(l_ifc_handle ifc) REENTRANT
  {
    l_irqmask                 irqMask;
    l_u8                      retval = LD_SERVICE_IDLE;
#else
  STACK l_bool ld_is_ready(l_ifc_handle ifc) REENTRANT
  {
    l_irqmask                 irqMask;
    l_bool                    retval = 1;
#endif /* LIN_21 */
    
    __CHECK_INTERFACE_VALUE__(ifc, (l_bool) 0);

    irqMask = l_sys_irq_disable();

#ifdef LIN_21
    if ((LIN_DIAG_FLAGS(ifc) & ((l_u8) LIN_DIAG2_ERROR)) != 0)
    {
      /*
       * bus error
       */       
      retval = LD_SERVICE_ERROR;
    }
    else if ((LIN_DIAG_FLAGS(ifc) & ((l_u8) LIN_DIAG2_TX_READY)) == 0)
    {
      /*
       * service is on going
       */       
      retval = LD_SERVICE_BUSY;
    }
    else if (((LIN_DIAG_FLAGS(ifc) & ((l_u8) LIN_DIAG2_TX_READY)) != 0)
        ||
        ((LIN_DIAG2_FLAGS(ifc) & ((l_u8) LIN_DIAG2_EXPECT_RESPONSE)) != 0))
    {
      /*
       * the configuration request has been completed
       */       
      retval = LD_REQUEST_FINISHED;
    }
    else
    {
      /*
       * in all other case, no request has been called or req/resp has finished
       */
      retval = LD_SERVICE_IDLE;;
    }
#else
    if (((LIN_DIAG_FLAGS(ifc) & ((l_u8) LIN_DIAG2_TX_READY)) != 0)
        ||
        ((LIN_DIAG2_FLAGS(ifc) & ((l_u8) LIN_DIAG2_EXPECT_RESPONSE)) != 0))
    {
      /*
       * the interface is not ready
       */       
      retval = 0;
    }
#endif /* LIN_21 */

    l_sys_irq_restore(irqMask);

    return retval;

  } /* end ld_is_ready() */

  /*********************************************************************
   *
   * ld_check_response()
   */
  /**
   * \brief function to query status of configuration request
   *
   * This function can be called to obtain the response status of the
   * last diagnostic request.
   * \attention Usually it does not make sense to call this function before
   *            ld_is_ready() returned TRUE
   *
   * - diagnostic API
   * - master only
   *
   * \param  ifc            handle of the interface
   * \param  pRsid          pointer to storage for RSID of last response
   * \param  pErrorCode     pointer to error code of last response
   *
   * \LIN 2.0
   * \retval LD_SUCCESS     positive ACK in last response
   * \retval LD_NEGATIVE    call failed
   * \retval LD_NO_RESPONSE slave did not answer
   * \retval LD_OVERWRITTEN items have been overwritten by TP request
   *
   * \LIN 2.1
   * \retval none
   *
   */
#ifdef LIN_21
  STACK void ld_check_response(l_ifc_handle ifc,
                               l_u8 * const pRsid,
                               l_u8 * const pErrorCode) REENTRANT
#else
  STACK l_u8 ld_check_response(l_ifc_handle ifc,
                               l_u8 *pRsid,
                               l_u8 *pErrorCode) REENTRANT
#endif /* LIN_21 */
  {
    l_irqmask                 irqMask;
    l_u8                      retval = LD_NO_RESPONSE;


    __CHECK_INTERFACE_VALUE__(ifc, retval);

    irqMask = l_sys_irq_disable();

#ifdef LIN_21
    /*
     * the behavior is API specific: for the moment in the parameters,
     * we always return the RSID and only when an error occurs also the error code;
     * who will call this function must pay attention to pass parameters initialized
     * to the default values LD_RSID_DEFAULT and LD_ERROR_CODE_DEFAULT.
     */
    
    /* return always the RSID  */
    *pRsid = LIN_DIAG2_RSID(ifc);
    
    if ((LIN_DIAG2_FLAGS(ifc) & ((l_u8) LIN_DIAG2_ERROR)) != 0)
    {
       /* if an error occurs return also the error code  */
       *pErrorCode = LIN_DIAG2_ERROR_CODE(ifc);
    }
#else
    /*
     * extract additional information from diagnostic
     * signals
     */
    *pRsid = LIN_DIAG2_RSID(ifc);
    *pErrorCode = LIN_DIAG2_ERROR_CODE(ifc);

    /*
     * now set return value
     */

    if ((LIN_DIAG2_FLAGS(ifc) & ((l_u8) LIN_DIAG2_RESPONSE_RECEIVED)) != 0)
    {
       retval =  LD_SUCCESS;
    }
    if ((LIN_DIAG2_FLAGS(ifc) & ((l_u8) LIN_DIAG2_ERROR)) != 0)
    {
       retval =  LD_NEGATIVE;
    }
#endif /* LIN_21 */

    l_sys_irq_restore(irqMask);

#ifdef LIN_21
    return ;
#else
    return retval;
#endif /* LIN_21 */

  } /* end l_check_response() */

#endif /* LIN_INCLUDE_DIAG2 */

#if (defined(LIN_INCLUDE_ASSIGN_FRAME_ID_RANGE) && defined(LIN_21))
  /*********************************************************************
   *
   * ld_assign_frame_id_range()
   */
  /**
   * \brief function to issue a PIDs configuration request to a slave node
   *
   * This function is called to assign the PIDs of up to four frames
   * in the slave node with the addressed NAD. The PIDs parameter 
   * shall be four bytes long, each byte shall contain a PID, 
   * do not care or unassign value.
   *
   * PID = Protected IDentifier
   * 0x00 = the 'unassign value' PID;
   * 0xFF = the 'do not care' PID;
   * valid PID range is only that is described on:
   *      - par. 2.8.2, table 2.4 of LIN 2.1 specifications, or
   *      - par. 7.2              of LIN 2.0 specifications
   *
   * - diagnostic API
   * - master only
   *
   * \param  ifc            the interface handle
   * \param  nad            the diagnostic address of the publishing node
   * \param  start_index    frame index from which to start to assign PIDs
   * \param  PIDs           Up to 4 bytes containing the new PIDs
   *
   * \retval none
   *
   */
  STACK void ld_assign_frame_id_range(l_ifc_handle ifc,
                                      l_u8 nad,
                                      l_u8 start_index,
                                      const l_u8* const PIDs) REENTRANT
  {
    l_irqmask  irqMask;


    __CHECK_INTERFACE_VOID__(ifc);

    irqMask = l_sys_irq_disable();

    LIN_DIAG2_BUFFER(ifc)[LIN_DIAG_FRAME_NAD_POS]          = nad;

    LIN_DIAG2_BUFFER(ifc)[LIN_DIAG_FRAME_PCI_POS]          =
       LIN_DIAG_PCI_ASSIGN_FRAMEID_RANGE;

    LIN_DIAG2_BUFFER(ifc)[LIN_DIAG_SF_FRAME_SID_POS]       =
       LIN_DIAG_SID_ASSIGN_FRAMEID_RANGE;

    LIN_DIAG2_BUFFER(ifc)[LIN_DIAG_SF_FRAME_DATA_POS + 0]  =
       (l_u8) start_index;

    LIN_DIAG2_BUFFER(ifc)[LIN_DIAG_SF_FRAME_DATA_POS + 1]  = PIDs[0];

    LIN_DIAG2_BUFFER(ifc)[LIN_DIAG_SF_FRAME_DATA_POS + 2]  = PIDs[1];

    LIN_DIAG2_BUFFER(ifc)[LIN_DIAG_SF_FRAME_DATA_POS + 3]  = PIDs[2];

    LIN_DIAG2_BUFFER(ifc)[LIN_DIAG_SF_FRAME_DATA_POS + 4]  = PIDs[3];

    LIN_DIAG2_FLAGS(ifc) = 0;

    LIN_DIAG_FLAGS(ifc) |= (l_u8) LIN_DIAG2_TX_READY;

    l_sys_irq_restore(irqMask);

    return ;

  } /* end ld_assign_frame_id_range() */

#endif /* defined(LIN_INCLUDE_ASSIGN_FRAME_ID_RANGE) && defined(LIN_21) */

#if (defined(LIN_INCLUDE_SAVE_CONFIGURATION) && defined(LIN_21))
  /*********************************************************************
   *
   * ld_save_configuration()
   *
   */
  /**
   * \brief function to issue a save configuration request to a slave node
   *
   * This function is called to send a save configuration request to a specific
   * slave node with the given NAD, or to all slave nodes if NAD is set to broadcast
   *
   *
   * - diagnostic API
   * - master only
   *
   * \param  ifc            the handle of the interface
   * \param  nad            diagnostic address of the node
   *
   * \retval none
   *
   */
  STACK void ld_save_configuration(l_ifc_handle ifc,
                                   l_u8 nad) REENTRANT
  {
    l_irqmask  irqMask;


    __CHECK_INTERFACE_VOID__(ifc);

    irqMask = l_sys_irq_disable();

    LIN_DIAG2_BUFFER(ifc)[LIN_DIAG_FRAME_NAD_POS]          = nad;

    LIN_DIAG2_BUFFER(ifc)[LIN_DIAG_FRAME_PCI_POS]          =
       LIN_DIAG_PCI_SAVE_CONFIGURATION;

    LIN_DIAG2_BUFFER(ifc)[LIN_DIAG_SF_FRAME_SID_POS]       =
       LIN_DIAG_SID_SAVE_CONFIGURATION;

    LIN_DIAG2_BUFFER(ifc)[LIN_DIAG_SF_FRAME_DATA_POS + 0]  = 0xFF;

    LIN_DIAG2_BUFFER(ifc)[LIN_DIAG_SF_FRAME_DATA_POS + 1]  = 0xFF;

    LIN_DIAG2_BUFFER(ifc)[LIN_DIAG_SF_FRAME_DATA_POS + 2]  = 0xFF;

    LIN_DIAG2_BUFFER(ifc)[LIN_DIAG_SF_FRAME_DATA_POS + 3]  = 0xFF;

    LIN_DIAG2_BUFFER(ifc)[LIN_DIAG_SF_FRAME_DATA_POS + 4]  = 0xFF;

    LIN_DIAG2_FLAGS(ifc) = 0;

    LIN_DIAG_FLAGS(ifc) |= (l_u8) LIN_DIAG2_TX_READY;

    l_sys_irq_restore(irqMask);

    return ;

  } /* end ld_save_configuration() */

#endif /* defined(LIN_INCLUDE_SAVE_CONFIGURATION) && defined(LIN_21) */

#ifdef LIN_INCLUDE_ASSIGN_NAD
  /*********************************************************************
   *
   * ld_assign_NAD()
   */
  /**
   * \brief function to issue configuration request to a slave node
   *
   * This function is called to change the diagnostic address of a node
   *
   * - diagnostic API
   * - master only
   *
   * \param  ifc             the interface to address
   * \param  nad/initial_nad the nad of the node
   * \param  supplier_id     the supplier id of the node
   * \param  function_id     the function id of the node
   * \param  new_nad         (yet?)  the nad to change the node to
   *
   * \retval none
   *
   */
#ifdef LIN_21
  STACK void ld_assign_NAD(l_ifc_handle ifc,
                           l_u8 initial_nad,
                           l_u16 supplier_id,
                           l_u16 function_id,
                           l_u8 new_nad) REENTRANT
#else
  STACK void ld_assign_NAD(l_ifc_handle ifc,
                           l_u8 nad,
                           l_u16 supplier_id,
                           l_u16 function_id,
                           l_u8 new_nad) REENTRANT
#endif /* LIN_21 */
  {

    l_irqmask                 irqMask;


    __CHECK_INTERFACE_VOID__(ifc);

    irqMask = l_sys_irq_disable();

#ifdef LIN_21
    LIN_DIAG2_BUFFER(ifc)[LIN_DIAG_FRAME_NAD_POS]          = initial_nad;
#else
    LIN_DIAG2_BUFFER(ifc)[LIN_DIAG_FRAME_NAD_POS]          = nad;
#endif /* LIN_21 */

    LIN_DIAG2_BUFFER(ifc)[LIN_DIAG_FRAME_PCI_POS]          =
       LIN_DIAG_PCI_ASSIGN_NAD;

    LIN_DIAG2_BUFFER(ifc)[LIN_DIAG_SF_FRAME_SID_POS]       =
       LIN_DIAG_SID_ASSIGN_NAD;

    LIN_DIAG2_BUFFER(ifc)[LIN_DIAG_SF_FRAME_DATA_POS + 0]  =
       (l_u8) supplier_id;

    LIN_DIAG2_BUFFER(ifc)[LIN_DIAG_SF_FRAME_DATA_POS + 1]  =
       (l_u8) (supplier_id >> CHAR_BIT);

    LIN_DIAG2_BUFFER(ifc)[LIN_DIAG_SF_FRAME_DATA_POS + 2]  =
       (l_u8) function_id;

    LIN_DIAG2_BUFFER(ifc)[LIN_DIAG_SF_FRAME_DATA_POS + 3]  =
       (l_u8) (function_id >> CHAR_BIT);

    LIN_DIAG2_BUFFER(ifc)[LIN_DIAG_SF_FRAME_DATA_POS + 4]  = new_nad;

    LIN_DIAG2_FLAGS(ifc) = 0;
    LIN_DIAG_FLAGS(ifc) |= (l_u8) LIN_DIAG2_TX_READY;

    l_sys_irq_restore(irqMask);

    return ;

  } /* end ld_assign_NAD() */
#endif /* LIN_INCLUDE_ASSIGN_NAD */

#ifdef LIN_INCLUDE_ASSIGN_FRAME_ID

  /*********************************************************************
   *
   * ld_assign_frame_id()
   */
  /**
   * \brief function to issue configuration request to a slave node
   *
   * This function is called to assign a new frame id to a message
   * published by a slave node
   *
   * - diagnostic API
   * - master only
   *
   * \param  ifc            the interface handle
   * \param  nad            the diagnostic address of the publishing node
   * \param  supplier_id    supplier id of the node
   * \param  message_id     id of the message to assign frame id to
   * \param  pid            protected message identifier
   *
   * \retval none
   *
   */
  STACK void ld_assign_frame_id(l_ifc_handle ifc,
                                l_u8 nad,
                                l_u16 supplier_id,
                                l_u16 message_id,
                                l_u8 pid) REENTRANT
  {
    l_irqmask  irqMask;


    __CHECK_INTERFACE_VOID__(ifc);

    irqMask = l_sys_irq_disable();

    LIN_DIAG2_BUFFER(ifc)[LIN_DIAG_FRAME_NAD_POS]          = nad;

    LIN_DIAG2_BUFFER(ifc)[LIN_DIAG_FRAME_PCI_POS]          =
       LIN_DIAG_PCI_ASSIGN_FRAMEID;

    LIN_DIAG2_BUFFER(ifc)[LIN_DIAG_SF_FRAME_SID_POS]       =
       LIN_DIAG_SID_ASSIGN_FRAMEID;

    LIN_DIAG2_BUFFER(ifc)[LIN_DIAG_SF_FRAME_DATA_POS + 0]  =
       (l_u8) supplier_id;

    LIN_DIAG2_BUFFER(ifc)[LIN_DIAG_SF_FRAME_DATA_POS + 1]  =
       (l_u8) (supplier_id >> CHAR_BIT);

    LIN_DIAG2_BUFFER(ifc)[LIN_DIAG_SF_FRAME_DATA_POS + 2]  =
       (l_u8) message_id;

    LIN_DIAG2_BUFFER(ifc)[LIN_DIAG_SF_FRAME_DATA_POS + 3]  =
       (l_u8) (message_id >> CHAR_BIT);

    LIN_DIAG2_BUFFER(ifc)[LIN_DIAG_SF_FRAME_DATA_POS + 4]  = pid;

    LIN_DIAG2_FLAGS(ifc) = 0;

    LIN_DIAG_FLAGS(ifc) |= (l_u8) LIN_DIAG2_TX_READY;

    l_sys_irq_restore(irqMask);

    return ;

  } /* end ld_assign_frame_id() */

#endif /* LIN_INCLUDE_ASSIGN_FRAME_ID */

#ifdef LIN_INCLUDE_READ_BY_ID

   /*********************************************************************
   *
   * ld_read_by_id()
   */
  /**
   * \brief function to issue configuration request to a slave node
   *
   * This function is used to request data from a slave node
   *
   * - diagnostic API
   * - master only
   *
   * \param  ifc            the handle of the interface
   * \param  nad            diagnostic address of the node
   * \param  supplier_id    the supplier id of the slave node
   * \param  function_id    the function id of the slave node
   * \param  id             id of the data item to read
   * \param  data           pointer to buffer to receive the desired
   *                  information
   *
   * \retval none
   *
   */
  STACK void ld_read_by_id(l_ifc_handle ifc,
                           l_u8 nad,
                           l_u16 supplier_id,
                           l_u16 function_id,
                           l_u8 id,
                           l_u8 *const mydata) REENTRANT
  {
    l_irqmask  irqMask;

    __CHECK_INTERFACE_VOID__(ifc);

    irqMask = l_sys_irq_disable();

    LIN_DIAG2_BUFFER(ifc)[LIN_DIAG_FRAME_NAD_POS]     = nad;

    LIN_DIAG2_BUFFER(ifc)[LIN_DIAG_FRAME_PCI_POS]     = LIN_DIAG_PCI_READ_BY_ID;

    LIN_DIAG2_BUFFER(ifc)[LIN_DIAG_SF_FRAME_SID_POS]  = LIN_DIAG_SID_READ_BY_ID;

    if (id == LIN_DIAG2_READ_PROD_ID)
    {
       /*
        * request to read product id of a node
        */
       LIN_DIAG2_SIZE(ifc) = 5;
    }
    else if (id == LIN_DIAG2_READ_SERIAL_NR)
    {
       /*
        * request to read serial id of a node
        */
       LIN_DIAG2_SIZE(ifc) = 4;
    }
    else if ((id >= LIN_DIAG2_READ_MSG_ID_MIN)
             &&
             (id <= LIN_DIAG2_READ_MSG_ID_MAX))
    {
       /*
        * request to read the id of a message
        */
       LIN_DIAG2_SIZE(ifc) = 3;

    }
    else if ((id >= LIN_DIAG2_READ_USR_DEF_MIN)
             &&
             (id <= LIN_DIAG2_READ_USR_DEF_MAX))
    {
       /*
        * user defined request, don't know how long they will
        * be, this could become dangerous, as we will always be
        * copying 5 bytes!!!
        * So the user must always provide a buffer that is at least
        * 5 bytes long!
        */
       LIN_DIAG2_SIZE(ifc) = 5;

    }
    else
    {
       /*
        * this must be an id from the reserved id block
        */
       LIN_DIAG2_SIZE(ifc) = 0;

    }

    LIN_DIAG2_READ_BUFFER(ifc)                             = mydata;

    LIN_DIAG2_BUFFER(ifc)[LIN_DIAG_SF_FRAME_DATA_POS + 0]  = id;

    LIN_DIAG2_BUFFER(ifc)[LIN_DIAG_SF_FRAME_DATA_POS + 1]  =
       (l_u8) supplier_id;

    LIN_DIAG2_BUFFER(ifc)[LIN_DIAG_SF_FRAME_DATA_POS + 2]  =
       (l_u8) (supplier_id >> CHAR_BIT);

    LIN_DIAG2_BUFFER(ifc)[LIN_DIAG_SF_FRAME_DATA_POS + 3]  =
       (l_u8) function_id;

    LIN_DIAG2_BUFFER(ifc)[LIN_DIAG_SF_FRAME_DATA_POS + 4]  =
       (l_u8) (function_id >> CHAR_BIT);

    LIN_DIAG2_FLAGS(ifc) = 0;

    LIN_DIAG_FLAGS(ifc) |= (l_u8) LIN_DIAG2_TX_READY;

    l_sys_irq_restore(irqMask);

    return ;

  } /* end ld_read_by_id() */

#endif /* LIN_INCLUDE_READ_BY_ID */

#ifdef LIN_INCLUDE_DATA_DUMP

  /*********************************************************************
   *
   * ld_data_dump()
   */
  /**
   * \brief function to issue configuration request to a slave node
   *
   * This function issues a data dump request towards the slave nodes
   *
   * - diagnostic API
   * - master only
   *
   * \param  ifc            handle of the interface
   * \param  nad            the nad of the slave node addressed
   * \param  sendBuf        buffer to send towards the slave
   * \param  receiveBuf     buffer to receive the slaves response
   *
   * \retval none
   *
   */
  STACK void ld_data_dump(l_ifc_handle         ifc,
                          l_u8                 nad,
                          const l_u8 * const   sendBuf,
                          l_u8 * const         receiveBuf) REENTRANT
  {
    l_irqmask  irqMask;


    __CHECK_INTERFACE_VOID__(ifc);

    irqMask = l_sys_irq_disable();

    LIN_DIAG2_BUFFER(ifc)[LIN_DIAG_FRAME_NAD_POS]          = nad;

    LIN_DIAG2_BUFFER(ifc)[LIN_DIAG_FRAME_PCI_POS]          =
       LIN_DIAG_PCI_DATA_DUMP;

    LIN_DIAG2_BUFFER(ifc)[LIN_DIAG_SF_FRAME_SID_POS]       =
       LIN_DIAG_SID_DATA_DUMP;

    LIN_DIAG2_BUFFER(ifc)[LIN_DIAG_SF_FRAME_DATA_POS + 0]  = sendBuf[0];

    LIN_DIAG2_BUFFER(ifc)[LIN_DIAG_SF_FRAME_DATA_POS + 1]  = sendBuf[1];

    LIN_DIAG2_BUFFER(ifc)[LIN_DIAG_SF_FRAME_DATA_POS + 2]  =  sendBuf[2];

    LIN_DIAG2_BUFFER(ifc)[LIN_DIAG_SF_FRAME_DATA_POS + 3]  = sendBuf[3];

    LIN_DIAG2_BUFFER(ifc)[LIN_DIAG_SF_FRAME_DATA_POS + 4]  = sendBuf[4];

    LIN_DIAG2_FLAGS(ifc) = 0;

    LIN_DIAG_FLAGS(ifc) |= LIN_DIAG2_TX_READY;

    /*
     * store pointer to receive buffer
     */
    LIN_DIAG2_READ_BUFFER(ifc) = receiveBuf;

    LIN_DIAG2_SIZE(ifc)        = 5;

    l_sys_irq_restore(irqMask);

    return ;

  } /* end ld_data_dump() */

#endif /* LIN_INCLUDE_DATA_DUMP */

#ifdef LIN_INCLUDE_COND_CHANGE_NAD

  /*********************************************************************
   *
   * ld_conditional_change_NAD()
   */
  /**
   * \brief function to issue configuration request to a slave node
   *
   * This function composes a diagnostic message to perform a conditional
   * NAD change
   * Note: The handling of this message is rather strange (within the
   *       slave node), for a complete description see the LIN standard
   *
   * - diagnostic API
   * - master only
   *
   * \param  ifc            interface to sent request over
   * \param  nad            node address of the desired node
   * \param  id             as required by the standard
   * \param  byte           as required by the standard
   * \param  mask           as required by the standard
   * \param  invert         as required by the standard
   * \param  new_nad        new address of the node
   *
   * \retval none
   *
   */
  STACK void ld_conditional_change_NAD(l_ifc_handle ifc,
                                       l_u8 nad,
                                       l_u8 id,
                                       l_u8 byte,
                                       l_u8 mask,
                                       l_u8 invert,
                                       l_u8 new_nad) REENTRANT
  {
    l_irqmask  irqMask;

    __CHECK_INTERFACE_VOID__(ifc);

    irqMask = l_sys_irq_disable();

    LIN_DIAG2_BUFFER(ifc)[LIN_DIAG_FRAME_NAD_POS]          = nad;

    LIN_DIAG2_BUFFER(ifc)[LIN_DIAG_FRAME_PCI_POS]          =
       LIN_DIAG_PCI_COND_CHANGE_NAD;

    LIN_DIAG2_BUFFER(ifc)[LIN_DIAG_SF_FRAME_SID_POS]       =
       LIN_DIAG_SID_COND_CHANGE_NAD;

    LIN_DIAG2_BUFFER(ifc)[LIN_DIAG_SF_FRAME_DATA_POS + 0]  = id;

    LIN_DIAG2_BUFFER(ifc)[LIN_DIAG_SF_FRAME_DATA_POS + 1]  = byte;

    LIN_DIAG2_BUFFER(ifc)[LIN_DIAG_SF_FRAME_DATA_POS + 2]  = mask;

    LIN_DIAG2_BUFFER(ifc)[LIN_DIAG_SF_FRAME_DATA_POS + 3]  = invert;

    LIN_DIAG2_BUFFER(ifc)[LIN_DIAG_SF_FRAME_DATA_POS + 4]  = new_nad;

    LIN_DIAG2_FLAGS(ifc) = 0;

    LIN_DIAG_FLAGS(ifc) |= LIN_DIAG2_TX_READY;

    l_sys_irq_restore(irqMask);

    return ;

  } /* end ld_conditional_change_NAD() */

#endif /* LIN_INCLUDE_COND_CHANGE_NAD */

#ifdef LIN_INCLUDE_DIAG2

  /*********************************************************************
   *
   * LIN diag level 2 Rx handler
   *
   *********************************************************************/

  /*********************************************************************
   *
   * ld_diag2RxHandler()
   */
  /**
   * \brief This is the Rx handler for the class 2 diagnostic module
   *
   * This function handles all incoming responses to configuration
   * requests
   *
   * - internal function
   * master only
   *
   * \param  ifc            handle of the interface
   * \param  pBuffer        buffer holding the received message
   *
   * \retval 1 if the message has been consumed
   * \retval 0 otherwise
   *
   */
  STACK static l_bool ld_diag2RxHandler(__OPTIONAL_IFC_DEFINITION_COMMA__
                                        const l_u8 *    pBuffer)
  {
    l_bool                    retval = 0;
    l_u8                      i;

    if ((LIN_DIAG2_FLAGS(ifc) & ((l_u8) LIN_DIAG2_EXPECT_RESPONSE)) != 0)
    {
      /*
       * now check whether this has been a positive response
       */
      if (pBuffer[LIN_DIAG_SF_FRAME_RSID_POS]
          != LIN_DIAG_FRAME_RSID_NAK)
      {
        /*
         * this is a positive response
         */
         LIN_DIAG2_FLAGS(ifc) = LIN_DIAG2_RESPONSE_RECEIVED |
                                LIN_DIAG2_DATA_VALID;



        /* If there is some data, copy it */
        for (i = 0; i < LIN_DIAG2_SIZE(ifc); ++i)
        {
          LIN_DIAG2_READ_BUFFER(ifc)[i]
             = pBuffer[LIN_DIAG_SF_FRAME_DATA_POS + i];
        }

      } /* if (positive response) */
      else
      {
        /*
         * a negative response
         */

         LIN_DIAG2_ERROR_CODE(ifc)
            = pBuffer[LIN_DIAG_FRAME_SF_ERROR_CODE_POS];
         LIN_DIAG2_RSID(ifc) = pBuffer[LIN_DIAG_FRAME_SF_REQ_SID_POS];
         LIN_DIAG2_FLAGS(ifc) = LIN_DIAG2_ERROR |
                                LIN_DIAG2_DATA_VALID;

      } /* if (positive response) */

      retval = 1;

    } /* if (response expected) */

    return retval;

  } /* end ld_diag2RxHandler() */

#endif /* LIN_INCLUDE_DIAG2 */

#ifdef LIN_INCLUDE_DIAGNOSTICS
  /*********************************************************************
   *
   * LIN diagnostic statemachine
   *
   *********************************************************************/

  /*********************************************************************
   *
   * ld_signalTxFrameError()
   */
  /**
   * \brief function to indicate transmission error
   * 
   * This function will be called whenever a transmit error is detected
   * during the transfer of a diagnostic (Tx) frame
   *
   * - internal function
   * - master only
   *
   * \param  ifc            handle of the interface
   *
   * \retval none
   *
   */
  STACK void ld_signalTxFrameError(__OPTIONAL_IFC_DEFINITION__)
  {
    /*
     * let the transmitting module handle the error
     */
    #ifdef LIN_INCLUDE_DIAG2
      if ((LIN_DIAG_FLAGS(ifc) & LIN_ORIGINATOR_DIAG2_TX) != 0)
      {
         LIN_DIAG2_FLAGS(ifc) = (LIN_DIAG2_ERROR | LIN_DIAG2_DATA_VALID);
      }
    #endif /* LIN_INCLUDE_DIAG2 */

    #ifdef LIN_INCLUDE_COOKED_TP
      if ((LIN_DIAG_FLAGS(ifc) & LIN_ORIGINATOR_COOKED_TX) != 0)
      {
         LIN_DIAG_FLAGS(ifc)       &= (l_u8) ~LIN_COOKED_TX_READY;
         LIN_COOKED_TP_FLAGS(ifc)  &= (l_u8) ~LIN_COOKED_TX_FLAG_MASK;
         LIN_COOKED_TP_FLAGS(ifc)  |= LIN_COOKED_TX_ERROR;
         #ifdef LIN_21
           /* Reset timeout flag */
           LIN_COOKED_TP_TIMEOUT_FLAGS(ifc)  = 0;
           /* Set status of the Master Node Transmission Handler to Idle */
           l_ifc_set_TP_status(LIN_STATUS_IDLE);
         #endif /* LIN_21 */
      }
    #endif /* LIN_INCLUDE_COOKED_TP */

    #ifdef LIN_INCLUDE_RAW_TP
      if ((LIN_DIAG_FLAGS(ifc) & LIN_ORIGINATOR_RAW_TX) != 0)
      {
         LIN_RAW_TP_FLAGS(ifc)     &= (l_u8) ~LIN_RAW_TX_FLAG_MASK;
         LIN_RAW_TP_FLAGS(ifc)     |=  LIN_RAW_TX_ERROR;
      }
    #endif /* LIN_INCLUDE_RAW_TP */

    LIN_DIAG_FLAGS(ifc) &= (l_u8) ~LIN_DIAG_ORIGINATOR_MASK;

    return ;

  } /* end ld_signalTxFrameError() */

  /*********************************************************************
   *
   * ld_signalRxFrameError()
   */
  /**
   * \brief function to indicate reception error in Slave Response
   *
   * This function will be called whenever a transmit error is detected
   * during the transfer of a diagnostic (Rx) frame
   *
   * - internal function
   * - master only
   *
   * \param  ifc            handle of the interface
   *
   * \retval none
   *
   */
  STACK void ld_signalRxFrameError(__OPTIONAL_IFC_DEFINITION__)
  {

    /*
     * for Rx, we cannot determine which module is waiting for a
     * response, or - if multiple modules accept a response - which
     * module was to receive the response. Hence we will abort all
     * rx operations
     */

    #ifdef LIN_INCLUDE_DIAG2
     /*
      * error is relevant only if module is receiving
      */
      if ((LIN_DIAG2_FLAGS(ifc) & ((l_u8) LIN_DIAG2_EXPECT_RESPONSE)) != 0)
      {
        /*
         * module awaits response, abort rx transfer
         */
         LIN_DIAG2_FLAGS(ifc) = (LIN_DIAG2_ERROR | LIN_DIAG2_DATA_VALID);

      } /* if (response expected) */

    #endif /* LIN_INCLUDE_DIAG2 */

    #ifdef LIN_INCLUDE_COOKED_TP

      if ((LIN_COOKED_TP_FLAGS(ifc) &
          (LIN_COOKED_RX_LISTENING | LIN_COOKED_RX_ACTIVE)) != 0)
      {
         LIN_COOKED_TP_FLAGS(ifc) &= (l_u8) ~LIN_COOKED_RX_FLAG_MASK;
         LIN_COOKED_TP_FLAGS(ifc) |=  LIN_COOKED_RX_ERROR;
         #ifdef LIN_21
           /* Reset timeout flag */
           LIN_COOKED_TP_TIMEOUT_FLAGS(ifc)  = 0;
           /* Set status of the Master Node Transmission Handler to Idle */
           l_ifc_set_TP_status(LIN_STATUS_IDLE);
         #endif /* LIN_21 */
      }

    #endif /* LIN_INCLUDE_COOKED_TP */

    #ifdef LIN_INCLUDE_RAW_TP
      /*
       * we cannot know whether raw tp was really interested in the
       * frame, still we simply set the flag
       */
      LIN_RAW_TP_FLAGS(ifc) |= (l_u8) LIN_RAW_RX_ERROR;

    #endif /* LIN_INCLUDE_RAW_TP */

    return ;

  } /* end ld_signalRxFrameError() */

#endif /* LIN_INCLUDE_DIAGNOSTICS */

#ifdef LIN_INCLUDE_DIAGNOSTICS

  /*********************************************************************
   *********************************************************************
   *********************************************************************
   **
   ** the diagnostic frame manager
   **
   *********************************************************************
   *********************************************************************
   *********************************************************************/
  /*********************************************************************
   *
   * LIN diag response distributor
   *
   *********************************************************************/

  /*********************************************************************
   *
   * ld_responseDistributor()
   */
  /**
   * \brief function distributing incoming Slave Response Frames
   *
   * This function distributes an incoming slave response frame
   * to the correct diagnostic module
   *
   * - internal function
   * - master only
   *
   * \param  ifc            the handle of the interface
   * \param  pBuffer        pointer to the buffer holding the received message
   *
   * \retval none
   *
   */
  STACK void ld_responseDistributor(__OPTIONAL_IFC_DEFINITION_COMMA__
                                           const l_u8 *pBuffer)
  {
    l_u8 consumed = 0;

    #ifdef LIN_INCLUDE_DIAG2
      l_u8 frameType = 0;
      l_u8 service;


      /*
       * check frame type first
       */
      /* Begin BUGZILLA_472 */
      frameType = ((l_u8) (pBuffer[LIN_DIAG_FRAME_PCI_POS] & LIN_DIAG_PCI_FRAMETYPE_MASK));
      /* End BUGZILLA_472 */
      if (frameType == LIN_DIAG_PCI_SF)
      {
        /*
         * This is a single frame message,
         * this could be either level 2 or level 3, must look
         * at RSID byte
         */
        service = pBuffer[LIN_DIAG_SF_FRAME_RSID_POS];

        /*
         * check whether this is a negative response
         */
        if (service == LIN_DIAG_FRAME_RSID_NAK)
        {
          service = pBuffer[LIN_DIAG_SF_FRAME_DATA_POS];
        }
        else
        {
          /*
           * a positive response, must extract original sid
           */
          service &= ((l_u8) ~LIN_DIAG_FRAME_RSID_ID);
        }

        /*
         * now we can decide whether this is a response to
         * a level 2 request
         */
        if ((service >= LIN_DIAG_CONFIG_SID_MIN)
            &&
            (service <= LIN_DIAG_CONFIG_SID_MAX))
        {
           /* a valid level 2 frame */
           if ((LIN_DIAG2_FLAGS(ifc)
                & ((l_u8) LIN_DIAG2_EXPECT_RESPONSE)) != 0)
           {
              consumed = ld_diag2RxHandler(__OPTIONAL_IFC_PARAMETER_COMMA__
                                           pBuffer);
           }
        }

      } /* if (single frame) */
    #endif /* LIN_INCLUDE_DIAG2 */

    /*
     * multi-frame messages are always level 3, and
     * so are all yet unhandled SF messages
     */
    #ifdef LIN_INCLUDE_COOKED_TP
      if (!consumed)
      {
        consumed = ld_diag3CookedRxHandler(__OPTIONAL_IFC_PARAMETER_COMMA__
                                           pBuffer);
      }
    #endif

    #ifdef LIN_INCLUDE_RAW_TP
      if (!consumed)
      {
        ld_diag3RawRxHandler(__OPTIONAL_IFC_PARAMETER_COMMA__
                             pBuffer);
      }
    #endif

    return ;

  } /* end ld_responseDistributor() */

#endif /* LIN_INCLUDE_DIAGNOSTICS */

/*
 * eof lin_diag_master.c
 */
