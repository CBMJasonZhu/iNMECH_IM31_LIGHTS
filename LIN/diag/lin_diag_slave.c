/*********************************************************************
 *
 * Project:      LIN 2.1 driver suite
 *
 * File name:    lin_diag_slave.c
 *
 * Copyright:    (c) STMicroelectronics
 *
 * Author:       Giuseppe Stefano Fazio
 *
 * Description:  Diagnostic functionality specific to the slave node
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
 * 20100430  v1.2   JK  - BUGZILLA_686: LIN 2.1 / ld_set_configuration
 * 20100430  v1.2   JK  - BUGZILLA_687: LIN 2.1 / ld_read_configuration
 * 20100430  v1.2   JK  - BUGZILLA_1006: LIN 2.1/LIN_COOKED_TP Issue
 * 20100430  v1.2   JK  - BUGZILLA_1006: Updated also for free usage NAD's. 
                          'ld_signalRxFrameError' is called when not in IDLE state.
 * 20110530  v1.3   JK  - BUGZILLA_1142: Read By Id response wrong 
 * 20110530  v1.3   JK  - BUGZILLA_1143: The slave node is not able
 *                           to ignore any interleaved functional addressed
 *                           transmission from the master node when a
 *                           Physical RX is on going.                        
 *
 *********************************************************************/

/** \file lin_diag_slave.c
 */

#define LIN_DIAG_SLAVE_C

#define LIN_DIAG_SLAVE_C_MAJOR_VERSION  1
#define LIN_DIAG_SLAVE_C_MINOR_VERSION  33
#define LIN_DIAG_SLAVE_C_PATCH_VERSION  1

#define LIN_DIAG_SLAVE_C_STM

/*********************************************************************
 * includes
 *********************************************************************/

#include "lin_diag_slave.h"
#include "lin_def.h"
#include "lin_def_gen.h"
#include "lin_def_arch_include.h"
#include "lin_cfg_types.h"
#include "lin_types.h"
#include "lin_general.h"
#include "lin_diag.h"
#include "lin_slave.h"

#include "lin_version_control.h"

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
 * global variables
 *********************************************************************/

#if ((defined (__ARCH_STM8__) || defined (__ARCH_STM8L__)) && defined (LIN_INCLUDE_COND_CHANGE_NAD))
  /* variables for cond change nad command,
   * STM8 needs static arrays instead of automatic ones
   */
  /*
   * MISRA deviation, rule 8.7
   * Reason: COSMIC compiler cannot always handle arrays as
   *         automatic variables
   */

  #define LIN_DIAG_SLAVE_VARIABLE_DEFINED
  static l_u8 condChangeNadBuf[LIN_DIAG_FRAME_LEN_MAX];
  static l_u8 condChangeNadByte;

#endif

#ifdef LIN_INCLUDE_DIAGNOSTICS
  #ifndef  LIN_DIAG_SLAVE_VARIABLE_DEFINED
    #define LIN_DIAG_SLAVE_VARIABLE_DEFINED
  #endif

  /** overall diagnostic flags */
#ifdef __COSMIC__
@tiny l_u8                 l_diagFlags;

#elif __RAISONANCE__
page0 l_u8                 l_diagFlags;

#elif __IAR__
__tiny l_u8                 l_diagFlags;
#endif

#endif

#ifdef LIN_INCLUDE_DIAG2
  #ifndef  LIN_DIAG_SLAVE_VARIABLE_DEFINED
    #define LIN_DIAG_SLAVE_VARIABLE_DEFINED
  #endif


#ifdef __COSMIC__
  /** diagnostic node address */
   @tiny l_u8               l_diagNad;
  /** configuration flads */
   @tiny l_u8               l_diag2ConfigFlags;
#elif __RAISONANCE__
  /** diagnostic node address */
   page0 l_u8               l_diagNad;
  /** configuration flads */
   page0 l_u8               l_diag2ConfigFlags;
#elif __IAR__
  /** diagnostic node address */
  __tiny l_u8               l_diagNad;
  /** configuration flads */
  __tiny l_u8               l_diag2ConfigFlags;
#endif


  /** buffer to compose configuration response to */
  static l_u8        l_diag2Buffer[LIN_DIAG_FRAME_LEN_MAX];
#endif

#if (defined LIN_NEED_LIB_PLACEBO) && (!defined(LIN_DIAG_SLAVE_VARIABLE_DEFINED))
  /** dummy variable to satisfy STM8 libmaker tool */
  l_u8 libPlaceboDiag2;

#endif 

/*********************************************************************
 * local functions
 *********************************************************************/

STACK static l_bool ld_diag2RxHandler(const l_u8 *pBuffer);

STACK static l_bool ld_readById(l_u8 id, l_u8 *pBuffer);

#if (defined(LIN_21) && defined(LIN_INCLUDE_SAVE_CONFIGURATION))
  STACK void ld_saveConfig (__OPTIONAL_IFC_DEFINITION__);
#endif /* defined(LIN_21) && defined(LIN_INCLUDE_SAVE_CONFIGURATION */
/*********************************************************************
 *
 * class 2 diagnostic module
 *
 *********************************************************************/
#ifdef LIN_INCLUDE_DIAG2
  /*********************************************************************
   *
   * ld_diag2RxHandler()
   *
   */
  /**
   * \brief function to handle incoming configuration requests
   *
   * This function evaluates an incoming class 2 master request frame
   *
   * - internal function
   * - slave only
   *
   * \param  ifc            handle of the interface
   * \param  pBuffer        buffer holding the received message
   *
   * \retval 1   SID was     a diag 2 request
   * \retval 0   SID was not a diag 2 request
   *
   */
  STACK static l_bool ld_diag2RxHandler(const l_u8 *      pBuffer)
  {
    l_u8 i=5;
    l_bool                    retval = 1;
    l_u16                     supplierId;
    l_u16                     functionId;
    l_u16                     messageId;
#if (defined(LIN_INCLUDE_ASSIGN_FRAME_ID_RANGE) && defined(LIN_21))
    l_u8                      start_index;
    l_u8                      l_pidBuffer[4];
#endif /* defined(LIN_INCLUDE_ASSIGN_FRAME_ID_RANGE) && defined(LIN_21) */

    /*
     * set diag 2 buffer to 0xff
     */
    LIN_DIAG2_BUFFER(ifc)[LIN_DIAG_FRAME_NAD_POS]         = LIN_DIAG_NAD(ifc);
    LIN_DIAG2_BUFFER(ifc)[LIN_DIAG_FRAME_PCI_POS]         = LIN_DIAG2_PCI_ACK;
   
     do{
         LIN_DIAG2_BUFFER(ifc)[LIN_DIAG_SF_FRAME_DATA_POS + --i] = 0xFF;
       }while(i);
	 
    switch (pBuffer[LIN_DIAG_SF_FRAME_SID_POS])
    {
      #ifdef LIN_INCLUDE_ASSIGN_FRAME_ID
        case LIN_DIAG_SID_ASSIGN_FRAMEID:
          if (pBuffer[LIN_DIAG_FRAME_PCI_POS]
               ==
               LIN_DIAG_PCI_ASSIGN_FRAMEID)
          {
            /*
             * extract data
             */
            supplierId = (l_u16) pBuffer[3]
                         + ((l_u16) pBuffer[4] << CHAR_BIT);


            if ((supplierId == (l_u16) LD_ANY_SUPPLIER)
                ||
                (supplierId == l_productId.supplierId))
            {
              /*
               * supplier id is valid
               */
              messageId =
                 (l_u16) pBuffer[LIN_DIAG_SF_FRAME_DATA_POS + 2]
                 + ((l_u16) pBuffer[LIN_DIAG_SF_FRAME_DATA_POS + 3]
                    << CHAR_BIT);

            
              if (l_changeFrameId(messageId,
                                   pBuffer[LIN_DIAG_SF_FRAME_DATA_POS + 4])
                  != 0)
              {
                /*
                 * success, so prepare positive response
                 */
                LIN_DIAG2_BUFFER(ifc)[LIN_DIAG_SF_FRAME_RSID_POS] = 0xf1;
                
                LIN_DIAG_FLAGS(ifc) |= (l_u8) LIN_DIAG2_TX_READY;

                LIN_DIAG2_CONFIG_FLAGS(ifc)
                   |= LIN_DIAG2_FLAGS_ASSIGN_FRAME_ID;
              } /* no else, no neg response defined
               */
            
            }
          } /* if (valid pci) */
          
          break;
      #endif /* LIN_INCLUDE_ASSIGN_FRAME_ID */

      #if (defined(LIN_INCLUDE_READ_BY_ID) || defined(LIN_21))
      /* Service Read By Identifier is mandatory in LIN 2.1 */
        case LIN_DIAG_SID_READ_BY_ID:

          if (pBuffer[LIN_DIAG_FRAME_PCI_POS]
              ==
              LIN_DIAG_PCI_READ_BY_ID)
          {

            supplierId = (l_u16) pBuffer[4]
                         + ((l_u16) pBuffer[5] << CHAR_BIT);
            functionId = (l_u16) pBuffer[6]
                         + ((l_u16) pBuffer[7] << CHAR_BIT);

            /*
             * check product id
             */
            if (((supplierId == (l_u16)LD_ANY_SUPPLIER)
                 ||
                 (supplierId == l_productId.supplierId))
                &&
                ((functionId == (l_u16)LD_ANY_FUNCTION)
                 ||
                 (functionId == l_productId.functionId)))
            {
              /*
               * ok, correct product id
               */
              if (ld_readById(pBuffer[3],
                              LIN_DIAG2_BUFFER(ifc))
                  != 0)
              {
                LIN_DIAG2_CONFIG_FLAGS(ifc)
                   |= LIN_DIAG2_FLAGS_READ_BY_ID;
              }
              else
              {
                /*
                 * command failed, compose neg response
                 */
                LIN_DIAG2_BUFFER(ifc)[LIN_DIAG_FRAME_PCI_POS]         =
                   LIN_DIAG2_PCI_NAK;

                LIN_DIAG2_BUFFER(ifc)[LIN_DIAG_SF_FRAME_RSID_POS]     =
                   LIN_DIAG_FRAME_RSID_NAK;

                LIN_DIAG2_BUFFER(ifc)[LIN_DIAG_SF_FRAME_DATA_POS]     =
                   LIN_DIAG_SID_READ_BY_ID;

                LIN_DIAG2_BUFFER(ifc)[LIN_DIAG_SF_FRAME_DATA_POS + 1] =
                   LIN_DIAG_NAK_ERROR_CODE;
               
              } /* if (command failed) */

              /*
               * there is no need to compose a positive response message
               * here because ld_readById() will have filled the buffer
               * with the correct message!
               */
              LIN_DIAG_FLAGS(ifc) |= (l_u8) LIN_DIAG2_TX_READY;
              
            } /* if (product id ok) */
          } /* if (valid pci) */
          
          break;
      #endif /* defined(LIN_INCLUDE_READ_BY_ID) || defined(LIN_21) */

      #ifdef LIN_INCLUDE_ASSIGN_NAD
        case LIN_DIAG_SID_ASSIGN_NAD:
           /*
            * the service ASSIGN_NAD must not accept the current nad,
            * only the intial NAD may be accepted!
            * Notice that current NAD may be equal to initial NAD,
            * so it must not be checked for inequality against current NAD!
            */
          if (pBuffer[LIN_DIAG_FRAME_PCI_POS]
              ==
              LIN_DIAG_PCI_ASSIGN_NAD)
          {
            if (((pBuffer[LIN_DIAG_FRAME_NAD_POS] == l_initialNad)
                  ||
                  (pBuffer[LIN_DIAG_FRAME_NAD_POS] == LD_BROADCAST)))
            {
              supplierId = (l_u16) pBuffer[3]
                                   + ((l_u16) pBuffer[4] << CHAR_BIT);
              functionId = (l_u16) pBuffer[5]
                                   + ((l_u16) pBuffer[6] << CHAR_BIT);
              /*
               * check product id
               */
              if (((supplierId == (l_u16)LD_ANY_SUPPLIER)
                   ||
                   (supplierId == l_productId.supplierId))
                  &&
                  ((functionId == (l_u16)LD_ANY_FUNCTION)
                   ||
                   (functionId == l_productId.functionId)))
              {
                /*
                 * ok, correct product id
                 */
                LIN_DIAG_NAD(ifc) = pBuffer[7];
                LIN_DIAG2_CONFIG_FLAGS(ifc) |= LIN_DIAG2_FLAGS_ASSIGN_NAD;
                
                /*
                 * the service ASSIGN_NAD also uses the initial NAD in
                 * the response message
                 */
                LIN_DIAG2_BUFFER(ifc)[LIN_DIAG_FRAME_NAD_POS]        =
                   l_initialNad;

                LIN_DIAG2_BUFFER(ifc)[LIN_DIAG_SF_FRAME_RSID_POS]    =
                   LIN_DIAG_SID_ASSIGN_NAD | LIN_DIAG_FRAME_RSID_ID;
              
                LIN_DIAG_FLAGS(ifc) |= (l_u8) LIN_DIAG2_TX_READY;
              
              } /* if (product id ok) */
            
            } /* if (initial NAD) */

          } /* if (valid PCI) */
        break;
      #endif /* LIN_INCLUDE_ASSIGN_NAD */

      #ifdef LIN_INCLUDE_COND_CHANGE_NAD
        case LIN_DIAG_SID_COND_CHANGE_NAD:
          if (pBuffer[LIN_DIAG_FRAME_PCI_POS]
              ==
              LIN_DIAG_PCI_COND_CHANGE_NAD)
          {

             #if ( !defined (__ARCH_STM8__) || !defined (__ARCH_STM8L__))
               /*
                * STM8 needs static arrays
                */
               l_u8 condChangeNadBuf[LIN_DIAG_FRAME_LEN_MAX];
               l_u8 condChangeNadByte;
             #endif /* __ARCH_STM8__ */
            /*
             * collect data
             */
            if (ld_readById(pBuffer[3], condChangeNadBuf) != 0)
            {
              condChangeNadByte
                 = condChangeNadBuf[(LIN_DIAG_SF_FRAME_DATA_POS +
                                    pBuffer[4]) - 1];

              condChangeNadByte ^= pBuffer[6];
              condChangeNadByte &= pBuffer[5];

              if (condChangeNadByte == 0)
              {
                /*
                 * change the NAD of the node
                 */
                LIN_DIAG_NAD(ifc) = pBuffer[7];
                LIN_DIAG2_CONFIG_FLAGS(ifc)
                   |= LIN_DIAG2_FLAGS_COND_CHANGE_NAD;
                
                LIN_DIAG2_BUFFER(ifc)[LIN_DIAG_SF_FRAME_RSID_POS]  =
                   LIN_DIAG_SID_COND_CHANGE_NAD | LIN_DIAG_FRAME_RSID_ID;
                
                /*
                 * slave node shall reply with old nad, so new nad
                 * is not written to response buffer here
                 */
                
                LIN_DIAG_FLAGS(ifc) |= LIN_DIAG2_TX_READY;
                
              }
            } /* no else, nothing to be done here.
               * if the conditions are not fullfilled we do not know whether
               * this message is really intended for that node, hence we
               * keep silent
               */
          } /* if (valid NAD) */
          
        break;
      #endif /* LIN_INCLUDE_COND_CHANGE_NAD */

      #ifdef LIN_INCLUDE_DATA_DUMP
        case LIN_DIAG_SID_DATA_DUMP:
          if (pBuffer[LIN_DIAG_FRAME_PCI_POS]
              ==
              LIN_DIAG_PCI_DATA_DUMP)
          {
            l_bool result;
            /*
             * data dump requested, this is up to the application
             */				 
            LIN_DIAG2_CONFIG_FLAGS(ifc) |= LIN_DIAG2_FLAGS_DATA_DUMP;
          }
        break;
      #endif /* LIN_INCLUDE_DATA_DUMP */

#ifdef LIN_21
      #ifdef LIN_INCLUDE_SAVE_CONFIGURATION
        case LIN_DIAG_SID_SAVE_CONFIGURATION:
          if (pBuffer[LIN_DIAG_FRAME_PCI_POS]
              ==
              LIN_DIAG_PCI_SAVE_CONFIGURATION)
          {
            /*
             * We must set a flag in the status register
             */

            ld_saveConfig();

            /*
             * getting here means that the application has provided
             * a response to the data dump request...
             */
            LIN_DIAG2_BUFFER(ifc)[LIN_DIAG_FRAME_PCI_POS]      =
                  LIN_DIAG_PCI_SAVE_CONFIGURATION;
                  
            LIN_DIAG2_BUFFER(ifc)[LIN_DIAG_SF_FRAME_RSID_POS]  =
               LIN_DIAG_SID_SAVE_CONFIGURATION | LIN_DIAG_FRAME_RSID_ID;
 
            LIN_DIAG_FLAGS(ifc) |= LIN_DIAG2_TX_READY;

            LIN_DIAG2_CONFIG_FLAGS(ifc) 
               |= LIN_DIAG2_FLAGS_SAVE_CONFIGURATION;
            }
        break;
      #endif /* LIN_INCLUDE_SAVE_CONFIGURATION */

      #ifdef LIN_INCLUDE_ASSIGN_FRAME_ID_RANGE
        case LIN_DIAG_SID_ASSIGN_FRAMEID_RANGE:
          if (pBuffer[LIN_DIAG_FRAME_PCI_POS]
               ==
               LIN_DIAG_PCI_ASSIGN_FRAMEID_RANGE)
          {
            /*
             * extract data
             */
            start_index    = pBuffer[3];
            l_pidBuffer[0] = pBuffer[4];
            l_pidBuffer[1] = pBuffer[5];
            l_pidBuffer[2] = pBuffer[6];
            l_pidBuffer[3] = pBuffer[7];


						 
            if (l_changePIDs(start_index, &(l_pidBuffer[0])) != 0)
              {
                /*
                 * success, so prepare positive response
                 */
                LIN_DIAG2_BUFFER(ifc)[LIN_DIAG_SF_FRAME_RSID_POS] = 0xf7;
                
                LIN_DIAG_FLAGS(ifc) |= (l_u8) LIN_DIAG2_TX_READY;

                LIN_DIAG2_CONFIG_FLAGS(ifc)
                   |= LIN_DIAG2_FLAGS_ASSIGN_FRAME_ID_RANGE;
              } /* no else, no neg response defined */
          } /* if (valid pci) */
          
          break;
      #endif /* LIN_INCLUDE_ASSIGN_FRAME_ID_RANGE */
#endif /* LIN_21 */
      
      default:
        /* No valid/suppported diag 2 SID */
        retval = 0;

    } /* switch (SID) */

    return retval; /* Valid diag 2 SID was processed */

  } /* end ld_diag2RxHandler() */

#endif /* LIN_INCLUDE_DIAG2 */

/*********************************************************************
 *
 * the diagnostic frame manager
 *
 *********************************************************************/

#ifdef LIN_INCLUDE_DIAGNOSTICS
  /*********************************************************************
   *
   * ld_requestReceived()
   *
   */
  /**
   * \brief fuunction to handle Master Request frames
   *
   * This function handles incoming master response frames
   *
   * - internal function
   * - slave only
   *
   * \param  ifc            the handle of the interface
   * \param  pBuffer        buffer holding the received message
   *
   * \retval none
   *
   */
  STACK void ld_requestReceived (__OPTIONAL_IFC_DEFINITION_COMMA__
                                 const l_u8 *   pBuffer)
  {
#ifdef LIN_21
    l_u16 length = 0;
    l_u8  frametype = LIN_DIAG_PCI_SF;
#endif /* LIN_21 */
    
    /*
     * first, we check the state of the interface
     */
     if ((LIN_DIAG_FLAGS(ifc) & (l_u8) LIN_DIAG_ORIGINATOR_MASK) != 0)
     {
       /*
        * this means that a response message has been given
        * to the driver but its transmission has not been confirmed
        * we assume this is an transmission error
        */
       ld_signalTxFrameError(__OPTIONAL_IFC_PARAMETER__);
     }

     /* anyway, clear this now */
     LIN_DIAG_FLAGS(ifc) &= (l_u8) ~LIN_DIAG_ORIGINATOR_MASK;

    /* Begin BUGZILLA_1143 */
#ifdef LIN_21
      if (pBuffer[LIN_DIAG_FRAME_NAD_POS] != (l_u8)LD_FUNCTIONAL_NAD)
      {
        /*
         * a new master request means that a stored class2 response
         * is no longer valid
         */
        LIN_DIAG_FLAGS(ifc) &= (l_u8) ~LIN_DIAG2_TX_READY;
      }
      else
      {
        NoOperation();
      }

      /* If a Functional request was received and there is a pending response, return */
      /* since the Functional request must be skipped and then there is no need to analyze it */
      if ((pBuffer[LIN_DIAG_FRAME_NAD_POS] == (l_u8)LD_FUNCTIONAL_NAD) 
      	  && 
      	  ((LIN_DIAG_FLAGS(ifc) & ((l_u8) LIN_DIAG2_TX_READY)) != 0))
      {
        return ;
      }
      else
      {
        NoOperation();
      }			
#else
     /*
      * a new master request means that a stored class2 response
      * is no longer valid
      */
     LIN_DIAG_FLAGS(ifc) &= (l_u8) ~LIN_DIAG2_TX_READY; 
#endif
    /* End   BUGZILLA_1143 */
     
    /*
     * check whether the message is destined for the slave node
     * (this depends mainly on the NAD)
     */
    if (pBuffer[LIN_DIAG_FRAME_NAD_POS] == LIN_DIAG_NAD_GOTO_SLEEP)
    {
      /*
       * entering sleep mode requested
       */ 
      l_gotoSleep(__OPTIONAL_IFC_PARAMETER__);

      return ;
    }
    #ifdef LIN_INCLUDE_DIAGNOSTICS
    
    #ifdef LIN_21
      /* Begin BUGZILLA_472 */
      frametype = ((l_u8) (pBuffer[LIN_DIAG_FRAME_PCI_POS] & LIN_DIAG_PCI_FRAMETYPE_MASK));
      /* End BUGZILLA_472 */
       switch (frametype)
        {
          case LIN_DIAG_PCI_SF: /* Single Frame PDU (SF) */
            /* length includes SID */
            length = ((l_u16) pBuffer[LIN_DIAG_FRAME_PCI_POS] & LIN_DIAG_PCI_LENGTH_MASK);
            if (length > 6)
            {
              /*
               * LIN 2.1 - 3.2.3: A single frame PDU (SF) with a length value greater than 
               * six (6) bytes shall be ignored by the receiver
               */
              return ;
            }
            break;

          case LIN_DIAG_PCI_FF: /* First Frame PDU (FF) */
            /* length includes SID */
            length = (l_u16) ((((l_u16) (pBuffer[LIN_DIAG_FRAME_PCI_POS] & LIN_DIAG_PCI_LENGTH_MASK)) 
                      << CHAR_BIT) + pBuffer[LIN_DIAG_FF_FRAME_LEN_POS]);
            if ((length < 7) || (length > LIN_MAX_RX_BUFFER_SIZE))
            {
              /*
               * LIN 2.1 - 3.2.3: A First Frame PDU (FF) with a length value greater than
               * the maximum available receive buffer size of the slave node shall be
               * ignored by the receiver and the receiver shall not start the reception
               * of a segmented message.
               * LIN 2.1 - 3.2.3: First Frame PDU (FF) with a length value less than 
               * seven (7) bytes shall be ignored by the receiver
               */
              return ;
            }
            /* Begin BUGZILLA_540 */
            #if (defined(LIN_21) && defined(LIN_INCLUDE_COOKED_TP))
              l_TimerStart_N_Cr(__OPTIONAL_TIMER_IFC_PARAMETER_COMMA__
                   l_N_Cr_timeout );
            #endif /* LIN_21 && LIN_INCLUDE_COOKED_TP */
            /* End BUGZILLA_540 */
            break;

          case LIN_DIAG_PCI_CF: /* Consecutive Frame PDU (CF) */
            /* Begin BUGZILLA_540 */
            #if (defined(LIN_21) && defined(LIN_INCLUDE_COOKED_TP))
              /* Reload timer N_Cr */
              l_TimerStart_N_Cr(__OPTIONAL_TIMER_IFC_PARAMETER_COMMA__
                   l_N_Cr_timeout );
            #endif /* LIN_21 && LIN_INCLUDE_COOKED_TP */
            /* End BUGZILLA_540 */
            break;

          default:
            /* should never happen */
            return ;
        } /* end switch (frametype) */
#endif /* LIN_21 */

      if (pBuffer[LIN_DIAG_FRAME_NAD_POS] > LD_BROADCAST)
      {
        /*
         * this is not a diagnostic NAD - free usage NADs - available for user defined diagnostic
         */
         /* Begin BUGZILLA_1006 */
             /* Pass to Idle state and reset TP State machine */
             #ifdef LIN_INCLUDE_COOKED_TP
               #ifdef LIN_21
                 #ifdef LIN_INCLUDE_DIAGNOSTICS
								 if(IFCCTRL_STATUSTPWORD(ifc) != LIN_IFC_STATUS_IDLE)
								 {
                   ld_signalRxFrameError(__OPTIONAL_IFC_PARAMETER__);
								 }
                 #endif /* LIN_INCLUDE_DIAGNOSTICS */
               #endif
             #endif
          /* End BUGZILLA_1006 */
        return ;
      }
      
#ifdef LIN_21
      if (pBuffer[LIN_DIAG_FRAME_NAD_POS] < LD_FUNCTIONAL_NAD)
      {
        /*
         * not a braodcast message nor a functional NAD
         */
#else
      if (pBuffer[LIN_DIAG_FRAME_NAD_POS] != LD_BROADCAST)
      {
        /*
         * not a braodcast message
         */
#endif /* LIN_21 */

        if (pBuffer[LIN_DIAG_FRAME_NAD_POS] != LIN_DIAG_NAD(ifc))
        {
          /*
           * actual nad does not match, check initial nad next...
           */
          if ((pBuffer[LIN_DIAG_FRAME_PCI_POS] & LIN_DIAG_PCI_FRAMETYPE_MASK)
              == LIN_DIAG_PCI_SF)
          {
            if ((pBuffer[LIN_DIAG_SF_FRAME_SID_POS]
                 != (l_u8)LIN_DIAG_SID_ASSIGN_NAD)
                ||
                (pBuffer[LIN_DIAG_FRAME_NAD_POS] != l_initialNad))
            {
              /*
               * message not for this slave node
               */
               
              /* Begin BUGZILLA_1006 */
             /* Pass to Idle state and reset TP State machine */
             #ifdef LIN_INCLUDE_COOKED_TP
               #ifdef LIN_21
                 #ifdef LIN_INCLUDE_DIAGNOSTICS
                 if(IFCCTRL_STATUSTPWORD(ifc) != LIN_IFC_STATUS_IDLE)
								 {
                   ld_signalRxFrameError(__OPTIONAL_IFC_PARAMETER__);
								 }
                 #endif /* LIN_INCLUDE_DIAGNOSTICS */
               #endif
             #endif
             /* End BUGZILLA_1006 */
             
              return ;
            
            } /* if (SID & initial NAD) */

          } /* if (SF) */
          else
          {
            /*
             * not a single frame message. this message is not for this
             * slave node
             */
             /* Begin BUGZILLA_1006 */
             /* Pass to Idle state and reset TP State machine */
             #ifdef LIN_INCLUDE_COOKED_TP
               #ifdef LIN_21
                 #ifdef LIN_INCLUDE_DIAGNOSTICS
                 if(IFCCTRL_STATUSTPWORD(ifc) != LIN_IFC_STATUS_IDLE)
								 {
                   ld_signalRxFrameError(__OPTIONAL_IFC_PARAMETER__);
								 }
                 #endif /* LIN_INCLUDE_DIAGNOSTICS */
               #endif
             #endif
             /* End BUGZILLA_1006 */
            return ;
          } /* if (SF) */

        } /* if (current NAD) */

      } /* if (no broadcast) */

      #ifdef LIN_INCLUDE_DIAG2
        /*
         * now we know that the slave node should be consuming this message
         */
        if ((pBuffer[LIN_DIAG_FRAME_PCI_POS] & LIN_DIAG_PCI_FRAMETYPE_MASK)
            == LIN_DIAG_PCI_SF)
        {
          if (ld_diag2RxHandler(__OPTIONAL_IFC_PARAMETER_COMMA__ pBuffer)
              != 0)
          {
            return ; /* Valid Diag 2 request was recognized */
          }
        } /* if (SF) */
      #endif /* LIN_INCLUDE_DIAG2 */

      /*
       * ok, should be class 3, try cooked module first
       */
      #ifdef LIN_INCLUDE_COOKED_TP
        if (ld_diag3CookedRxHandler(__OPTIONAL_IFC_PARAMETER_COMMA__ pBuffer)
            != 0)
        {
          /*
           * the cooked module did consume the frame
           */
          return ;
        }
      #endif /* LIN_INCLUDE_COOKED_TP */

      #ifdef LIN_INCLUDE_RAW_TP
        /*
         * perhaps raw tp will handle the frame
         */
        (void)ld_diag3RawRxHandler(pBuffer);
      
      #endif /* LIN_INCLUDE_RAW_TP */

    #endif /* LIN_INCLUDE_DIAGNOSTICS */

    return ;
  
  } /* end ld_requestReceived() */

  /*********************************************************************
   *
   * ld_composeSlaveResponse()
   *
   */
  /**
   * \brief function to compose a slave response message
   *
   * This function fills the contents of a slave response frame
   * to be sent into the buffer provided.
   *
   * - internal function
   * - slave only
   *
   * \param  ifc            handle of the interface
   * \param  pBuffer        buffer to compose message into
   *
   * \retval 1 if a response message must be sent
   * \retval 0 if no message is due
   *
   */
  STACK l_bool ld_composeSlaveResponse(__OPTIONAL_IFC_DEFINITION_COMMA__
                                       l_u8 *    pBuffer)
  {
	l_u8 i;
    l_bool                    composed = 0; 
    #ifdef LIN_INCLUDE_DIAG2
      if ((LIN_DIAG_FLAGS(ifc) & (l_u8) LIN_DIAG2_TX_READY) != 0)
      {
        /*
         * a class 2 diagnostic response is ready, copy it to signal
         * buffer
         */
				 
				for(i=0;i<8;i++)                                  
				  pBuffer[i] = LIN_DIAG2_BUFFER(ifc)[i];
				
        LIN_DIAG_FLAGS(ifc) |= (l_u8 )LIN_ORIGINATOR_DIAG2_TX;
        LIN_DIAG_FLAGS(ifc) &= (l_u8) ~LIN_DIAG2_TX_READY;
      
        composed = 1;
      }
    #endif /* LIN_INCLUDE_DIAG2 */

    #ifdef LIN_INCLUDE_COOKED_TP
      if (!composed)
      {
        if (ld_diag3CookedTxHandler(__OPTIONAL_IFC_PARAMETER_COMMA__ pBuffer)
            != 0)
        {
           LIN_DIAG_FLAGS(ifc) |= (l_u8) LIN_ORIGINATOR_COOKED_TX;
           composed = 1;
        }
      }
    #endif /* LIN_INCLUDE_COOKED_TP */

    #ifdef LIN_INCLUDE_RAW_TP
      if (!composed)
      {
        if (ld_diag3RawTxHandler(__OPTIONAL_IFC_PARAMETER_COMMA__ pBuffer)
            != 0)
        {
          LIN_DIAG_FLAGS(ifc) |= (l_u8) LIN_ORIGINATOR_RAW_TX;
          composed = 1;
        }
      }
    #endif /* LIN_INCLUDE_RAW_TP */

    return composed;

  } /* end ld_composeSlaveResponse() */

  /*********************************************************************
   *
   * ld_transmittedSlaveResponse()
   *
   */
  /**
   * \brief function to indicate successful transmission of slave
   *        response
   *
   * This function is called whenever a slave response frame has been
   * successfully sent over the bus.
   *
   * - internal function
   * - slave only
   *
   * \param  ifc            handle of the interface
   *
   * \retval none
   *
   */
  STACK void ld_transmittedSlaveResponse(__OPTIONAL_IFC_DEFINITION__)
  {
    #ifdef LIN_INCLUDE_COOKED_TP
      if ((LIN_DIAG_FLAGS(ifc) & LIN_ORIGINATOR_COOKED_TX) != 0)
      {
        LIN_COOKED_TP_FLAGS(ifc) &= (l_u8) ~LIN_COOKED_TX_FLAG_MASK;
        LIN_COOKED_TP_FLAGS(ifc) |= LIN_COOKED_TX_SUCCESS;
        #ifdef LIN_21
          /* Reset timeout flag */
          LIN_COOKED_TP_TIMEOUT_FLAGS(ifc)  = 0;
          /* Set status of the Slave Node Transmission Handler to Idle */
          l_ifc_set_TP_status(LIN_STATUS_IDLE);
        #endif /* LIN_21 */
      }
      /* Begin BUGZILLA_540 */
      #ifdef LIN_21
        if ((LIN_DIAG_FLAGS(ifc) & (l_u8) LIN_COOKED_TX_READY) != 0)
        {
          /* There is yet something to send: reload timer N_As */
          l_TimerStart_N_As(__OPTIONAL_TIMER_IFC_PARAMETER_COMMA__ l_N_As_timeout );
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

  } /* end ld_transmittedSlaveResponse() */

  /*********************************************************************
   *
   * ld_signalRxFrameError()
   *
   */
  /**
   * \brief function to indicate diagnostic Rx error
   *
   * This function will be called by the driver if a error occurs
   * *while receiving a diagnostic frame* \n
   * this is relevant especially for ongoing cooked Rx
   *
   * - internal function
   * - slave only
   *
   * \param  ifc            handle of the interface
   *
   * \retval none
   *
   */
  STACK void ld_signalRxFrameError (__OPTIONAL_IFC_DEFINITION__)
  {
    /*
     * if case of rx error we have to inform both tp variants
     */
    #ifdef LIN_INCLUDE_COOKED_TP

      if ((LIN_COOKED_TP_FLAGS(ifc) &
          (LIN_COOKED_RX_LISTENING | LIN_COOKED_RX_ACTIVE))
          != 0)
      {
        LIN_COOKED_TP_FLAGS(ifc) &= (l_u8) ~LIN_COOKED_RX_FLAG_MASK;
        LIN_COOKED_TP_FLAGS(ifc) |= LIN_COOKED_RX_ERROR;
        #ifdef LIN_21
          /* Reset timeout flag */
          LIN_COOKED_TP_TIMEOUT_FLAGS(ifc)  = 0;
          /* Set status of the Slave Node Transmission Handler to Idle */
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

    /* If there is an error on the previous diagnostic frame received */
    /* no response should be sent by the slave node, so stop any  */
    /* pending communication. */
    LIN_DIAG_FLAGS(ifc) &= (l_u8) ~LIN_DIAG_TX_READY_MASK;

    return ;

  } /* end ld_signalRxFrameError() */

  /*********************************************************************
   *
   * ld_signalTxFrameError()
   *
   */
  /**
   * \brief function to indicate diagnostic Tx error
   *
   * This function is called whenever sending of a slave response frame
   * issued by this node fails
   *
   * - internal function
   * - slave only
   *
   * \param  ifc            handle of the interface
   *
   * \retval none
   *
   */
  STACK void ld_signalTxFrameError (__OPTIONAL_IFC_DEFINITION__)
  {
    /*
     * let the transmitting module handle the error
     */
    #ifdef LIN_INCLUDE_COOKED_TP
      if ((LIN_DIAG_FLAGS(ifc) & LIN_ORIGINATOR_COOKED_TX) != 0)
      {
        LIN_DIAG_FLAGS(ifc)       &= (l_u8) ~LIN_COOKED_TX_READY;
        LIN_COOKED_TP_FLAGS(ifc)  &= (l_u8) ~LIN_COOKED_TX_FLAG_MASK;
        LIN_COOKED_TP_FLAGS(ifc)  |= LIN_COOKED_TX_ERROR;
        #ifdef LIN_21
          /* Reset timeout flag */
          LIN_COOKED_TP_TIMEOUT_FLAGS(ifc)  = 0;
          /* Set status of the Slave Node Transmission Handler to Idle */
          l_ifc_set_TP_status(LIN_STATUS_IDLE);
        #endif /* LIN_21 */
      }
    #endif /* LIN_INCLUDE_COOKED_TP */

    #ifdef LIN_INCLUDE_RAW_TP
      if ((LIN_DIAG_FLAGS(ifc) & LIN_ORIGINATOR_RAW_TX) != 0)
      {
        LIN_RAW_TP_FLAGS(ifc)     &= (l_u8) LIN_RAW_TX_FLAG_MASK;
        LIN_RAW_TP_FLAGS(ifc)     |= LIN_RAW_TX_ERROR;
      }
    #endif /* LIN_INCLUDE_RAW_TP */

    LIN_DIAG_FLAGS(ifc) &= (l_u8) ~LIN_DIAG_ORIGINATOR_MASK;

    return ;

  } /* end ld_signalTxFrameError() */

#endif /* LIN_INCLUDE_DIAGNOSTICS */

/*********************************************************************
 *
 * helper functions
 *
 *********************************************************************/

#if (defined(LIN_INCLUDE_READ_BY_ID) || defined(LIN_INCLUDE_COND_CHANGE_NAD) || defined(LIN_21))
  /*********************************************************************
   *
   * ld_readById()
   *
   */
  /**
   * \brief function to read internal diagnostic data
   *
   * This function does the data gathering that is caused by a
   * read by id request from the master node
   * It will compose the response message into the buffer
   * provided (the buffer must be at least 8 bytes in size)
   *
   * - internal function
   * - slave only
   *
   * \param  nad            the address of the responding note
   * \param  id             id of the requested item
   * \param  pBuffer        buffer to hold the response message
   *
   * \retval 1 if a message has been successfully coded into the response
   *         buffer
   * \retval 0 otherwise.
   *
   * \attention The buffer will be altered in any case!
   *
   */
  STACK static l_bool ld_readById(l_u8 id, l_u8 *pBuffer)
  {
    /*
     * first write the constant items into the buffer
     */
    pBuffer[LIN_DIAG_SF_FRAME_RSID_POS] = LIN_DIAG_RSID_READ_BY_ID;

    /*
     * the handling does essentially depend on the id of the
     * requested item
     */
    if (id == LIN_DIAG2_READ_PROD_ID)
    {
      /*
       * product id requested
       */
      pBuffer[LIN_DIAG_FRAME_PCI_POS] =
         LIN_DIAG_PCI_READ_BY_ID_SUPPLIER;

      pBuffer[LIN_DIAG_SF_FRAME_DATA_POS + 0]
         = (l_u8) (l_productId.supplierId & LIN_BYTE_MASK);

      pBuffer[LIN_DIAG_SF_FRAME_DATA_POS + 1]
         = (l_u8) (l_productId.supplierId >> CHAR_BIT);

      pBuffer[LIN_DIAG_SF_FRAME_DATA_POS + 2]
         = (l_u8) (l_productId.functionId & LIN_BYTE_MASK);

      pBuffer[LIN_DIAG_SF_FRAME_DATA_POS + 3]
         = (l_u8) (l_productId.functionId >> CHAR_BIT);

      pBuffer[LIN_DIAG_SF_FRAME_DATA_POS + 4]
         = (l_u8) (l_productId.variant);
    }
    else if (id == LIN_DIAG2_READ_SERIAL_NR)
    {
      /*
       * serial number requested: it's 4 bytes long
       */
        pBuffer[LIN_DIAG_FRAME_PCI_POS] =
           LIN_DIAG_PCI_READ_BY_ID_SERIAL;

        pBuffer[LIN_DIAG_SF_FRAME_DATA_POS + 0]
           = BYTE_0(SERIAL_NUMBER);

        pBuffer[LIN_DIAG_SF_FRAME_DATA_POS + 1]
           = BYTE_1(SERIAL_NUMBER);

        pBuffer[LIN_DIAG_SF_FRAME_DATA_POS + 2]
           = BYTE_2(SERIAL_NUMBER);

        pBuffer[LIN_DIAG_SF_FRAME_DATA_POS + 3]
           = BYTE_3(SERIAL_NUMBER);
    }
   #ifndef LIN_21
    else if ((id >= LIN_DIAG2_READ_MSG_ID_MIN)
              &&
             (id <= LIN_DIAG2_READ_MSG_ID_MAX))
    {
      /*
       * a message id has been requested
       */
      if ((id - LIN_DIAG2_READ_MSG_ID_MIN) < LIN_FRAME_NR_MAX)
      {
        l_frame_handle frameId;

        /*
         * this is a valid frame id
         */
        frameId = (l_frame_handle) (id - LIN_DIAG2_READ_MSG_ID_MIN);

        if ((l_pFrameId[frameId] == LIN_MASTER_REQ_FRAME_ID)
            ||
            (l_pFrameId[frameId] == LIN_SLAVE_RESP_FRAME_ID))
        {
           /*
            * do not provide the message ids for diagnostic frames
            */
           return 0;
        }
         
        pBuffer[LIN_DIAG_FRAME_PCI_POS] = LIN_DIAG_PCI_READ_BY_ID_FRAME_ID;

        /*
         * perhaps it would be more elegant to call a special driver function
         * here that delivered the message id and frame identifier
         * (these are driver data) but for efficiency this solution is better
         */
        pBuffer[LIN_DIAG_SF_FRAME_DATA_POS + 0]
           = (l_u8) (l_pMessageId[frameId] & LIN_BYTE_MASK);

        pBuffer[LIN_DIAG_SF_FRAME_DATA_POS + 1]
          = (l_u8) (l_pMessageId[frameId] >> CHAR_BIT);

        pBuffer[LIN_DIAG_SF_FRAME_DATA_POS + 2] = (l_pFrameId[frameId]);
      }
      else
      {
         /*
          * invalid frame index, provide negative response
          */
         return 0;
      }

    }
   #endif /* !LIN_21*/
    else if ((id >= LIN_DIAG2_READ_USR_DEF_MIN)
             &&
             (id <= LIN_DIAG2_READ_USR_DEF_MAX))
    {
      /*
       * a user defined request, this has not been defined now
       */

   #ifdef LIN_21
   		l_u8 retval = 0;
   		
   		/* This is an example of response; real implementation is application-dependent */
   		/* You can use one of the following define to set PCI of response frame for     */
   		/* this service to the correct value that must be in the range [0x02..0x06]: */
   		/*                 */
   		/* LIN_DIAG_PCI_READ_BY_ID_USER_DEF_2 == 0x02 */
   		/* LIN_DIAG_PCI_READ_BY_ID_USER_DEF_3 == 0x03 */
   		/* LIN_DIAG_PCI_READ_BY_ID_USER_DEF_4 == 0x04 */
   		/* LIN_DIAG_PCI_READ_BY_ID_USER_DEF_5 == 0x05 */
   	  /* LIN_DIAG_PCI_READ_BY_ID_USER_DEF_6 == 0x06 */
   	  /* Begin BUGZILLA_1142 */
   		/* pBuffer[LIN_DIAG_FRAME_PCI_POS] = LIN_DIAG_PCI_READ_BY_ID_USER_DEF_4;*/
      /* End   BUGZILLA_1142 */
      retval = ld_read_by_id_callout(id, pBuffer);
      
      if (retval == LD_NO_RESPONSE)
      {
        /*
         * No response to the user defined request
         */
         return 0;
      }
      else if (retval == LD_NEGATIVE_RESPONSE)
      {
        /*
         * A negative response is ready to be sent to the user defined request
         */
        pBuffer[LIN_DIAG_SF_FRAME_RSID_POS] = LIN_DIAG_FRAME_RSID_NAK;

        pBuffer[LIN_DIAG_FRAME_PCI_POS]     = LIN_DIAG2_PCI_NAK;
      }
      else
      {
        /*
         * A positive response is ready to be sent to the user defined request
         */
        /* Begin BUGZILLA_1142 */
        pBuffer[LIN_DIAG_FRAME_PCI_POS]     = LIN_DIAG_PCI_READ_BY_ID_USER_DEF_6;
        /* End   BUGZILLA_1142 */
      }
   #else
      return ld_readByIdCallback(id, pBuffer);
   #endif /* LIN_21 */
    }
    else
    {
      /*
       * no valid id requested
       */
      return 0;
    }

    return 1;

  } /* end ld_readById() */

#endif /* defined(LIN_INCLUDE_READ_BY_ID) || defined(LIN_INCLUDE_COND_CHANGE_NAD) || defined(LIN_21) */

#if (defined(LIN_21) && defined(LIN_INCLUDE_SAVE_CONFIGURATION))
  /*********************************************************************
   *
   * ld_saveConfig()
   *
   */
  /**
   * \brief function to save slave node configuration data
   *
   * This function must
   * 1) set a flag in the status register;
   * 2) read-out the actual configuration;
   * 3) save actual configuration in a non-volatile space.
   *
   * Points 2) and 3) should be managed out of this function,
   * possibly in the application, checking the returned value
   * of the API l_ifc_read_status(), for example like the following:
   *
   * if (l_ifc_read_status_SCIx() & LIN_IFC_STATUS_SAVE_CONFIGURATION)
   * {
   *   read-out the actual configuration;
   *   save actual configuration in a non-volatile space
   * }
   *
   * - internal function
   * - slave only
   *
   * \param  ifc            handle of the interface
   *
   * \retval none
   *
   */
  STACK void ld_saveConfig (__OPTIONAL_IFC_DEFINITION__)
  {
    l_u8 i;
    
    /*
     * set the SAVE CONFIGURATION status in the status word (16 bit value)
     */
    l_ifcSetStatus(__OPTIONAL_IFC_PARAMETER_COMMA__ LIN_STATUS_SAVE_CONFIGURATION, 60);

    return ;

  } /* end ld_saveConfig() */

#endif /* defined(LIN_21) && defined(LIN_INCLUDE_SAVE_CONFIGURATION) */

#ifdef LIN_21
  /*********************************************************************
   *
   * ld_set_configuration()
   *
   */
  /**
   * \brief function to configure slave node according to data
   *
   * This function will configure the NAD and the PIDs according to the
   * configuration given by data. The intended usage is to restore a saved
   * configuration or set an initial configuration (e.g. coded by I/O pins).
   * The function shall be called after calling l_ifc_init().
   * The caller shall set the size of the data area before calling the function.
   * The data contains the NAD and the PIDs and occupies one byte each.
   * The structure of the data is: NAD and then all PIDs for the frames.
   * The order of the PIDs are the same as the frame list in the LDF.
   * This function will not transport anything on the bus.
   *
   * - diagnostic API
   * - slave only
   *
   * \param  ifc                   handle of the interface
   * \param  data                  structure containing the NAD and all the
   *                               n PIDs for the frames of the specified NAD
   * \param  length                length of data (1+n, NAD+PIDs)
   *
   * \retval LD_SET_OK             if the service was successful
   * \retval LD_LENGTH_NOT_CORRECT if the required size of the configuration is not equal to the given length
   * \retval LD_DATA_ERROR         The set of configuration could not be made
   *
   */
  STACK l_u8 ld_set_configuration(l_ifc_handle ifc, const l_u8* const mydata, l_u16 length)
  {
    l_u8 i;
    
    /* Set the default returned value to LD_DATA_ERROR */
    l_u8 retval = LD_DATA_ERROR;
    
    /** Set the expected length value to 
     * 
     * EXP = NF + NN, where :
     *
     * NF = the number of configurable frames;
     * NN = the number of NAD.
     *
     * Moreover:
     *
     * NF = dimension of l_pMessageId[LIN_FRAME_COUNT]
     */
    l_u16 expected_length = (l_u16) (LIN_FRAME_COUNT + 1);
    
    if (length < expected_length)
    {
      /* The 'data' size is not enough to contain NAD+PIDs */
      retval = LD_LENGTH_NOT_CORRECT;
    }
    else 
    {
      /* The 'data' size is enough to contain NAD+PIDs, so proceed to read from 'data' */
      
      /* Read actual NAD from 'data' */
      LIN_DIAG_NAD(ifc) = mydata[0];

      /* Copy protected IDs in 'data' */
      for (i = 1; i < expected_length; ++i)
      {
         /* Begin BUGZILLA_686 */
         l_pFrameId[i - 1] = mydata[i];
         /* End BUGZILLA_686 */
      }
      
      /* No error, return OK */
      retval = LD_SET_OK;
    }

  	return retval;

  } /* end ld_set_configuration() */

  /*********************************************************************
   *
   * ld_read_configuration()
   *
   */
  /**
   * \brief function to copy current configuration in a reserved area
   *
   * This call will serialize the current configuration and copy it to 
   * the area (data pointer) provided by the application. The intention
   * is to call this function when the save configuration request flag 
   * is set in the status register.
   * After the call is finished the application is responsible to store
   * the data in appropriate memory.
   * The caller shall reserve bytes in the data area equal to length,
   * before calling this function.
   * The function will set the length parameter to the actual size of 
   * the configuration. In case the data area is too short the function 
   * will return with no action.
   * In case the NAD has not been set by a previous call to 
   * ld_set_configuration or the master node has used the configuration 
   * services, the returned NAD will be the initial NAD.
   * The data contains the NAD and the PIDs and occupies one byte each. 
   * The structure of the data is: NAD and then all PIDs for the frames. 
   * The order of the PIDs are the same as the frame list in the LDF.
   * This function will not transport anything on the bus.
   *
   * - diagnostic API
   * - slave only
   *
   * \param  ifc                    handle of the interface
   * \param  data                   structure that will contain the NAD and 
   *                                all the n PIDs for the frames of the 
   *                                specified NAD
   * \param  length                 length of data (1+n, NAD+PIDs)
   *
   * \retval LD_READ_OK             if the service was successful
   * \retval LD_LENGTH_TOO_SHORT    if the configuration size is greater than
   *                                the length. It means that the data area
   *                                does not contain a valid configuration.
   *
   */
  STACK l_u8 ld_read_configuration(l_ifc_handle ifc, l_u8* const mydata, l_u8* const length)
  {
    l_u8 i, temp;
    
    /* Set the default returned value to LD_READ_OK */
    l_u8 retval = LD_READ_OK;
    
    /** Set the expected length value to 
     * 
     * EXP = NF + NN, where :
     *
     * NF = the number of configurable frames;
     * NN = the number of NAD.
     *
     * Moreover:
     *
     * NF = dimension of l_pMessageId[LIN_FRAME_COUNT]
     */
    l_u8 expected_length = (LIN_FRAME_COUNT + 1);
    
    temp = *length;
    if (temp < expected_length)
    {
      /* The 'data' size is not enough to store NAD+PIDs */
      retval = LD_LENGTH_TOO_SHORT;
    }
    else 
    {
      /* The 'data' size is enough to store NAD+PIDs, so proceed to store them */
      
      /* Copy actual NAD in 'data' */
      mydata[0] = LIN_DIAG_NAD(ifc);

      /* Copy protected IDs in 'data' */
      for (i = 1; i < expected_length; ++i)
      {
         /* Begin BUGZILLA_687 */
         mydata[i] = l_pFrameId[i - 1];
         /* End BUGZILLA_687 */
      }
      
      /* Set the length parameter to the actual size of the configuration */
      *length = expected_length;
    }

  	return retval;

  } /* end ld_read_configuration() */
#endif /* LIN_21 */

/*
 * eof lin_slave_diag.c
 */
