/*********************************************************************
 *
 * Project:      LIN 2.1 driver suite
 *
 * File name:    lin_diag.c
 *
 * Copyright:    (c) STMicroelectronics
 *
 * Author:       Giuseppe Stefano Fazio
 *
 * Description:  General diagnostic functionality
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
 *                         BUGZILLA_472: LIN driver does not compile if 
 *                         Cosmic "strict" option is enabled.
 *                         BUGZILLA_540: LIN 2.1 / N_Cr_LIN timeout
 *                         and N_As_LIN timeout management missing.
 * 20100430  v1.2   JK  - BUGZILLA_788: Modified ld_init() for the LIN_COOKED_N_CR_TIMEOUT
 *                          LIN_COOKED_N_AS_TIMEOUT, LIN_COOKED_RX_WRONG_SN, LIN_COOKED_RX_ERROR, 
 *                          LIN_COOKED_TX_ERROR
 * 20100430  v1.2   JK  - BUGZILLA_1006: Transport Protocol issue
 * 20100430  v1.2   JK  - BUGZILLA_1104: LIN_ABORT_TP_PREVIOUS_SESSION
 * 20110624  v1.3   JK  - BUGZILLA_1164: Test Case 13.4.2: Receiving segmented request with functional request between request parts
 * 20110624  v1.3   JK  - BUGZILLA_1165: Test Case 13.8.2: Sending segmented response with functional request between response parts
 * 20110728  v1.3   JK  - BUGZILLA_1167: Test Case 13.5.2: N_As timer to be started also for Single frames
 * 20120822  v1.4   JK  - Modified the handling of second FF received after the first FF.  
 *********************************************************************/

/** \file lin_diag.c
 */

#define LIN_DIAG_C

#define LIN_DIAG_C_MAJOR_VERSION  1
#define LIN_DIAG_C_MINOR_VERSION  32
#define LIN_DIAG_C_PATCH_VERSION  2

#define LIN_DIAG_C_STM

/*********************************************************************
 * includes
 *********************************************************************/
#include "lin_types.h"
#include "lin_def_arch_include.h"
#include "lin_def.h"
#include "lin_def_gen.h"
#include "lin_cfg_types.h"

#include "lin_diag.h"
#include "lin_diag_api.h"

#ifdef LIN_MASTER_NODE
  #include "lin_diag_master.h"
#else
  #include "lin_diag_slave.h"
#endif

#include "lin_general.h"

#include "lin_version_control.h"


/*********************************************************************
 * local macros
 *********************************************************************/
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
 * local typedefs
 *********************************************************************/

/*********************************************************************
 * global constants
 *********************************************************************/

/*********************************************************************
 * global variables
 *********************************************************************/

#ifdef LIN_INCLUDE_COOKED_TP
  #define LIN_DIAG_VARIABLE_DEFINED
  #ifdef __MORE_IFCS__
    /** total length of cooked message */
    static l_u16         l_cookedTxLength[LIN_IFC_NR_MAX];

    /** bytes of cooked message already transferred */
    static l_u16         l_cookedTxBytesSent[LIN_IFC_NR_MAX];

    /** frame counter for cooked transfer  */
    static l_u8          l_cookedTxFrameCounter[LIN_IFC_NR_MAX];

    /** NAD for cooked transfer */
    static l_u8          l_cookedTxNad[LIN_IFC_NR_MAX];

    /** pointer to buffer holding complete cooked message */
    static const l_u8 *  l_cookedTxBuffer[LIN_IFC_NR_MAX];


    /** maximum number of bytes, also length of buffer */
    static l_u16         l_cookedRxBytesExpected[LIN_IFC_NR_MAX];

    /** frame counter of cooked rx transfer */
    static l_u16         l_cookedRxFrameCounter[LIN_IFC_NR_MAX];

    /** pointer to buffer into which cooked message will be written */
    static l_u8 *        l_cookedRxBuffer[LIN_IFC_NR_MAX];

    /** pointer to receive NAD for cooked TP, is returned to the
     * application
     */
    static l_u8 *        l_cookedRxNad[LIN_IFC_NR_MAX];

    /** pointer to location to store received length, will also be
     * returned to the application
     */
    static l_u16 *       l_cookedRxLength[LIN_IFC_NR_MAX];

    /** flags to control the cooked module */
    l_u8                 l_cookedTpFlags[LIN_IFC_NR_MAX];
    
    #ifdef LIN_21
      /** flags to control the diagnostic timeouts on cooked module */
      l_u8                 l_cookedTpTimeoutFlags[LIN_IFC_NR_MAX];
    #endif /* LIN_21 */
  #else
    /** total length of cooked message */
    static l_u16         l_cookedTxLength;

    /** bytes of cooked message already transferred */
    static l_u16         l_cookedTxBytesSent;

    /** frame counter for cooked transfer  */
    static l_u8          l_cookedTxFrameCounter;

    /** NAD for cooked transfer */
    static l_u8          l_cookedTxNad;

    /** pointer to buffer holding complete cooked message */
    static const l_u8 *  l_cookedTxBuffer;

    /** maximum number of bytes, also length of buffer */
    static l_u16         l_cookedRxBytesExpected;

    /** frame counter of cooked rx transfer */
    static l_u16         l_cookedRxFrameCounter;

    /** pointer to buffer into which cooked message will be written */
    static l_u8 *        l_cookedRxBuffer;

    /** pointer to receive NAD for cooked TP, is returned to the
     * application
     */
    static l_u8 *        l_cookedRxNad;

    /** pointer to location to store received length, will also be
     * returned to the application
     */
    static l_u16 *       l_cookedRxLength;

    /** flags to control the cooked module */
    l_u8                 l_cookedTpFlags;

    #ifdef LIN_21
      /** flags to control the diagnostic timeouts on cooked module */
      l_u8                 l_cookedTpTimeoutFlags;
    #endif /* LIN_21 */

  #endif
  
    #ifdef LIN_ABORT_TP_PREVIOUS_SESSION  
      static l_u8             previous_frameType;
    #endif
#endif/* LIN_INCLUDE_COOKED_TP */


#ifdef LIN_INCLUDE_RAW_TP
  #ifndef LIN_DIAG_VARIABLE_DEFINED
    #define LIN_DIAG_VARIABLE_DEFINED
  #endif

  #ifdef __MORE_IFCS__
    /** raw Tx fifo stack */
    static l_u8          l_rawTxStack[LIN_IFC_NR_MAX]
                            [LIN_DIAG_FRAME_LEN_MAX * LIN_DIAG3_FIFO_SIZE_MAX];

    /** first valid element on raw Tx stack */
    static l_u8          l_rawTxStackStart[LIN_IFC_NR_MAX];

    /** last valid element on raw Tx stack */
    static l_u8          l_rawTxStackEnd[LIN_IFC_NR_MAX];

    /** raw Rx fifo stack */
    static l_u8          l_rawRxStack[LIN_IFC_NR_MAX]
                           [LIN_DIAG_FRAME_LEN_MAX * LIN_DIAG3_FIFO_SIZE_MAX];

    /** first valid element on raw Rx stack */
    static l_u8          l_rawRxStackStart[LIN_IFC_NR_MAX];

    /** last valid element on raw Rx stack */
    static l_u8          l_rawRxStackEnd[LIN_IFC_NR_MAX];

    /** raw TP control flags */
    l_u8                 l_rawTpFlags[LIN_IFC_NR_MAX];
  #else
    /** raw Tx fifo stack */
    static l_u8          l_rawTxStack[LIN_DIAG_FRAME_LEN_MAX
                                      * LIN_DIAG3_FIFO_SIZE_MAX];

    /** first valid element on raw Tx stack */
    static l_u8          l_rawTxStackStart;

    /** last valid element on raw Tx stack */
    static l_u8          l_rawTxStackEnd;

    /** raw Rx fifo stack */
    static l_u8          l_rawRxStack[LIN_DIAG_FRAME_LEN_MAX
                                      * LIN_DIAG3_FIFO_SIZE_MAX];

    /** first valid element on raw Rx stack */
    static l_u8          l_rawRxStackStart;

    /** last valid element on raw Rx stack */
    static l_u8          l_rawRxStackEnd;

    /** raw TP control flags */
    l_u8                 l_rawTpFlags;
  #endif

#endif /* LIN_INCLUDE_RAW_TP */

/*
 * STM8 needs at least one symbol in source file in order to build
 * library
 */
#if (defined LIN_NEED_LIB_PLACEBO) && (!defined LIN_DIAG_VARIABLE_DEFINED)
  l_u8 libPlacebo;
#endif

/*********************************************************************
 * local constants
 *********************************************************************/

/*********************************************************************
 * local variables
 *********************************************************************/

/*********************************************************************
 * local functions
 *********************************************************************/

#ifdef LIN_INCLUDE_RAW_TP

  STACK static l_bool ld_diag3RawTxFifoStore (__OPTIONAL_IFC_DEFINITION_COMMA__ const l_u8 *pBuffer);
  STACK static l_bool ld_diag3RawRxFifoGet   (__OPTIONAL_IFC_DEFINITION_COMMA__ l_u8 *pBuffer);
  STACK static void ld_diag3RawRxFifoStore (__OPTIONAL_IFC_DEFINITION_COMMA__ const l_u8 *pBuffer);

#endif

/*********************************************************************
 *********************************************************************
 *********************************************************************
 **
 ** LIN 2.0 class 3 diagnostic module
 **
 *********************************************************************
 *********************************************************************
 *********************************************************************/


/*********************************************************************
 *
 * LIN diag level 3 API
 *
 *********************************************************************/

#if (defined(LIN_INCLUDE_TP) && defined(LIN_21))

  /*********************************************************************
   *
   * ld_init()
   */
  /**
   * \brief initialization of raw or cooked TP
   *
   * This function will (re)initialize the raw and the cooked layers
   * on the interface ifc.
   * All transport layer buffers will be initialized.
   * If there is an ongoing diagnostic frame transporting a cooked
   * or raw message on the bus, it will not be aborted.
   *
   * - diagnostic API
   * - master and slave
   *
   * \param  ifc            handle of the interface
   *
   * \retval none
   *
   */
  STACK void ld_init (l_ifc_handle ifc) REENTRANT
  {
    l_irqmask irqMask;
    l_u8      i;


    __CHECK_INTERFACE_VOID__(ifc);


    irqMask = l_sys_irq_disable();

    #ifdef LIN_INCLUDE_RAW_TP
      if (((LIN_RAW_TP_FLAGS(ifc) & (l_u8) LIN_RAW_TX_QUEUE_EMPTY) != 0) &&
      	  ((LIN_RAW_TP_FLAGS(ifc) & (l_u8) LIN_RAW_RX_QUEUE_EMPTY) != 0))
      {
      /*
       * the RX and TX stack are completely empty: only in this case
       * we can (re)initialize the raw layer.
       */
        LIN_RAW_TP_FLAGS(ifc)             = 0;  
        for (i = 0; i < (LIN_DIAG_FRAME_LEN_MAX*LIN_DIAG3_FIFO_SIZE_MAX); ++i)
        {
          LIN_RAW_TX_STACK(ifc)[i] = 0;
        } /* for all the stack */
        LIN_RAW_TX_STACK_START(ifc)       = 0;
        LIN_RAW_TX_STACK_END(ifc)         = 0;
        for (i = 0; i < (LIN_DIAG_FRAME_LEN_MAX*LIN_DIAG3_FIFO_SIZE_MAX); ++i)
        {
          LIN_RAW_RX_STACK(ifc)[i] = 0;
        } /* for all the stack */
        LIN_RAW_RX_STACK_START(ifc)       = 0;
        LIN_RAW_RX_STACK_END(ifc)         = 0;
        LIN_DIAG_FLAGS(ifc)              &= (l_u8) ~LIN_RAW_TX_READY;
      }
    #endif /* LIN_INCLUDE_RAW_TP */
    /* Begin BUGZILLA_788 */
    #ifdef LIN_INCLUDE_COOKED_TP
      if ((((LIN_COOKED_TP_FLAGS(ifc) & (LIN_COOKED_RX_LISTENING | LIN_COOKED_RX_ACTIVE)) == 0) &&
      	  ((LIN_DIAG_FLAGS(ifc)              & (l_u8) LIN_COOKED_TX_READY)      == 0)) ||
      	  ((LIN_COOKED_TP_TIMEOUT_FLAGS(ifc) & (l_u8) LIN_COOKED_N_CR_TIMEOUT)  != 0)  ||
          ((LIN_COOKED_TP_TIMEOUT_FLAGS(ifc) & (l_u8) LIN_COOKED_N_AS_TIMEOUT)  != 0)  ||
          ((LIN_COOKED_TP_FLAGS(ifc)         & (l_u8) LIN_COOKED_RX_WRONG_SN)   != 0)  ||
          ((LIN_COOKED_TP_FLAGS(ifc)         & (l_u8) LIN_COOKED_RX_ERROR)      != 0)  ||
          ((LIN_COOKED_TP_FLAGS(ifc)         & (l_u8) LIN_COOKED_TX_ERROR)      != 0))
      {
     /* End BUGZILLA_788 */
      /*
       * neither RX in progress nor TX in progress: only in this case
       * we can (re)initialize the cooked layer.
       */
        LIN_COOKED_TP_FLAGS(ifc)          = 0;
        LIN_COOKED_TX_LENGTH(ifc)         = 0;
        LIN_COOKED_TX_NAD(ifc)            = 0;
        LIN_COOKED_TX_BUFFER(ifc)         = 0;
        LIN_COOKED_TX_BYTES_SENT(ifc)     = 0;
        LIN_COOKED_TX_FRAME_COUNTER(ifc)  = 0;
        LIN_COOKED_RX_BYTES_EXPECTED(ifc) = 0;
        LIN_COOKED_RX_NAD(ifc)            = 0;
        LIN_COOKED_RX_LENGTH(ifc)         = 0;
        LIN_COOKED_RX_BUFFER(ifc)         = 0;
        LIN_COOKED_RX_FRAME_COUNTER(ifc)  = 0;
        LIN_DIAG_FLAGS(ifc)              &= (l_u8) ~LIN_COOKED_TX_READY;
        LIN_COOKED_TP_TIMEOUT_FLAGS(ifc)  = 0;
        /* Set status of the Master or Slave Node Transmission Handler to Idle */
        l_ifc_set_TP_status(LIN_STATUS_IDLE);
      }
    #endif /* LIN_INCLUDE_COOKED_TP */
    
    /* anyway, clear this now */
    LIN_DIAG_FLAGS(ifc) &= (l_u8) ~LIN_DIAG_ORIGINATOR_MASK;

    l_sys_irq_restore(irqMask);

  } /* end ld_init() */

#endif /* defined(LIN_INCLUDE_TP) && defined(LIN_21) */

#ifdef LIN_INCLUDE_RAW_TP

  /*********************************************************************
   *
   * ld_put_raw()
   */
  /**
   * \brief starts raw TP send
   *
   * This function queues a raw diagnostic frame for Tx
   * The application should check ld_raw_tx_status() before a frame
   * is passed to the function - if no space is available on the stack,
   * the data is discarded without further notification
   *
   * - diagnostic API
   * - master and slave
   *
   * \param  ifc            handle of the interface
   * \param  pData          pointer to the frame data
   *
   * \retval none
   *
   */
  STACK void ld_put_raw (l_ifc_handle ifc, const l_u8 *const pData) REENTRANT
  {
    l_irqmask irqMask;


    __CHECK_INTERFACE_VOID__(ifc);

    irqMask = l_sys_irq_disable();

    /*
     * we only have to put the frame on the stack
     */
    (void) ld_diag3RawTxFifoStore(__OPTIONAL_IFC_PARAMETER_COMMA__ pData);

    l_sys_irq_restore(irqMask);

  } /* end ld_put_raw() */

  /*********************************************************************
   *
   * ld_get_raw()
   */
  /**
   * \brief gets a raw TP frame from receive buffer
   *
   * This function copies the oldest frame from the Rx Fifo stack into
   * the buffer provided. ld_raw_rx_status() should be checked before,
   * this function will not tell whether we have copied a frame to the
   * buffer
   *
   * - diagnostic API
   * - master and frame
   *
   * \param  ifc            handle of the interface
   * \param  pData          pointer to the buffer
   *
   * \retval none
   *
   */
  STACK void ld_get_raw (l_ifc_handle ifc, l_u8 *const pData) REENTRANT
  {
    l_irqmask irqMask;


    __CHECK_INTERFACE_VOID__(ifc);

    irqMask = l_sys_irq_disable();

    (void) ld_diag3RawRxFifoGet(__OPTIONAL_IFC_PARAMETER_COMMA__ pData);

    l_sys_irq_restore(irqMask);

  } /* end ld_get_raw() */

  /*********************************************************************
   *
   * ld_raw_tx_status()
   */
  /**
   * \brief queries status of raw TP Tx module
   *
   * This function reports the current state of the raw tx module
   * to the application.
   *
   * - diagnostic API
   * - master and slave
   *
   * \param  ifc            handle of the interface
   *
   * \return status of the module, as defined in the LIN standard
   *
   */
  STACK l_u8 ld_raw_tx_status (l_ifc_handle ifc) REENTRANT
  {
    l_u8                      retval; 
#ifdef LIN_21
    __CHECK_INTERFACE_VALUE__(ifc, LD_TRANSMIT_ERROR);
#else
    __CHECK_INTERFACE_VALUE__(ifc, LD_TRANSFER_ERROR);
#endif /* LIN_21 */

    /*
     * first check whether we have an error here
     */
    if ((LIN_RAW_TP_FLAGS(ifc) & LIN_RAW_TX_ERROR) != 0)

    {
      /*
       * an error occurred during the last transfer.
       * This return value does not really fit in here, but it is
       * required by the standard
       */
#ifdef LIN_21
      retval = LD_TRANSMIT_ERROR;
#else
      retval = LD_TRANSFER_ERROR;
#endif /* LIN_21 */
    }
    else if ((LIN_RAW_TP_FLAGS(ifc) & (l_u8) LIN_RAW_TX_QUEUE_FULL) != 0)
    {
      /*
       * the stack is full and cannot accept new frames
       */
      retval = LD_QUEUE_FULL;
    }
    else if ((LIN_RAW_TP_FLAGS(ifc) & (l_u8) LIN_RAW_TX_QUEUE_EMPTY) != 0)
    {
      /*
       * the stack is completely empty, we will check below whether
       * all frames have yet been transferred
       */
      retval = LD_QUEUE_EMPTY;
    }
    else
    {
      /*
       * the queue is not empty, but has storage for more frames
       */
#ifdef LIN_21
      retval = LD_QUEUE_AVAILABLE;
#else
      retval = LD_QUEUE_READY;
#endif /* LIN_21 */
    }


    return retval;

  } /* end ld_raw_tx_status() */

  /*********************************************************************
   *
   * ld_raw_rx_status()
   */
  /**
   * \brief  queries status of raw TP Rx module
   *
   * This function returns the current status of the raw Rx Fifo module
   *
   * - diagnostic API
   * - master and slave
   *
   * \param  ifc            the handle of the interface
   *
   * \return state of the interface, as required by the standard
   *
   */
  STACK l_u8 ld_raw_rx_status (l_ifc_handle ifc) REENTRANT
  {
     l_u8                     retval;
#ifdef LIN_21
    __CHECK_INTERFACE_VALUE__(ifc, LD_RECEIVE_ERROR);
#else
    __CHECK_INTERFACE_VALUE__(ifc, LD_TRANSFER_ERROR);
#endif /* LIN_21 */

    /*
     * first check for error
     */
    if ((LIN_RAW_TP_FLAGS(ifc) & (l_u8) LIN_RAW_RX_ERROR) != 0)
    {
      /*
       * the last transfer has been corrupted
       */
#ifdef LIN_21
      retval = LD_RECEIVE_ERROR;
#else
      retval = LD_TRANSFER_ERROR;
#endif /* LIN_21 */
    }
    else if ((LIN_RAW_TP_FLAGS(ifc) & (l_u8) LIN_RAW_RX_QUEUE_EMPTY) != 0)
    {
      /*
       * the stack is empty
       */
#ifdef LIN_21
      retval = LD_NO_DATA;
#else
      retval = LD_QUEUE_EMPTY;
#endif /* LIN_21 */
    }
    else
    {
       retval = LD_DATA_AVAILABLE;
    }
    
    /*
     * if the stack is not empty, we have some data available
     */
    return retval;

  } /* end ld_raw_rx_status() */

  /*********************************************************************
   *
   * ld_raw_tx_delete()
   */
  /**
   * \brief removes a frame from Tx stack
   *
   * Deletes the oldest frame from the raw tx stack
   *
   * - STMicroelectronics extension
   * - master and slave
   *
   * \param  ifc            handle of the interface
   *
   * \retval 1 if a frame has been deleted
   * \retval 0 otherwise
   */
  STACK l_bool ld_raw_tx_delete(l_ifc_handle ifc) REENTRANT
  {
    l_u8                       pBuffer[LIN_DIAG_FRAME_LEN_MAX];
    l_irqmask                  irqMask;
    l_bool                     retval = 0;


    __CHECK_INTERFACE_VOID__(ifc);

    irqMask = l_sys_irq_disable();

    /*
     * no need to check the state, for this is done in
     * ld_diag3RawTxFifoGet()
     */
    retval = ld_diag3RawTxFifoGet(__OPTIONAL_IFC_PARAMETER_COMMA__
                                  pBuffer);

    l_sys_irq_restore(irqMask);

    return  retval;

  }/* end ld_raw_tx_delete() */

#endif /* LIN_INCLUDE_RAW_TP */

#ifdef LIN_INCLUDE_COOKED_TP

  /*********************************************************************
   *
   * ld_send_message()
   */
  /**
   * \brief initiates a cooked message send
   *
   * Send one cooked message over the diagnostic interface
   *
   * - diagnostic API
   * - master and slave
   *
   * \param  ifc            handle of the interface
   * \param  length         length of the message
   * \param  nad            address of the receiving node
   * \param  pData          buffer holding the requested data
   *
   * \retval none
   */
  STACK void ld_send_message(l_ifc_handle ifc,
                             l_u16 length,
                             l_u8 nad,
                             const l_u8 * const pData) REENTRANT
  {
    l_irqmask               irqMask;

    __CHECK_INTERFACE_VOID__(ifc);

    irqMask = l_sys_irq_disable();

    /* Begin BUGZILLA_540 */
    /* Only for FF, not for SF, we must start the N_As timer */
    /* Begin BUGZILLA_1167 */
    //if (length > (LIN_DIAG_SF_FRAME_DATA_LEN_MAX + 1))
    /* End BUGZILLA_1167 */
    {
      #ifdef LIN_21
        #ifdef LIN_SLAVE_NODE
          l_TimerStart_N_As(__OPTIONAL_TIMER_IFC_PARAMETER_COMMA__ l_N_As_timeout );
        #else
          #ifndef N_AS_TIMEOUT           /* If not defined in lin_def.h, we define the default value */
            #define N_AS_TIMEOUT 1000    /* 1000 msec is the default value */
           #endif /* N_AS_TIMEOUT */
           l_TimerStart_N_As(__OPTIONAL_TIMER_IFC_PARAMETER_COMMA__ N_AS_TIMEOUT );
        #endif /* LIN_SLAVE_NODE */
      #endif /* LIN_21 */
    }
    /* End BUGZILLA_540 */

    /*
     * check state here
     */
    if ((LIN_DIAG_FLAGS(ifc) & (l_u8) LIN_COOKED_TX_READY) == 0)
    {
      /*
       * the module is ready to accept a message now,
       * so fill in the values
       */
      LIN_DIAG_FLAGS(ifc)              |= (l_u8) LIN_COOKED_TX_READY;

      LIN_COOKED_TX_LENGTH(ifc)         = length;
      #ifdef LIN_MASTER_NODE
        LIN_COOKED_TX_NAD(ifc)          = nad;
      #else
        LIN_COOKED_TX_NAD(ifc)          = LIN_DIAG_NAD(ifc);
      #endif

      LIN_COOKED_TX_BUFFER(ifc)         = pData;

      LIN_COOKED_TX_BYTES_SENT(ifc)     = 0;
      LIN_COOKED_TX_FRAME_COUNTER(ifc)  = 0;
    }

    /*
     * message will be transferred when the next diagnostic frame is due
     */
    l_sys_irq_restore(irqMask);

    return ;

  } /* end ld_send_message() */

  /*********************************************************************
   *
   * ld_receive_message()
   */
  /**
   * \brief starts receiving a cooked TP message
   *
   * This function instructs the cooked diagnostics module to accept
   * one multiframe diagnostic message
   *
   * - diagnostic API
   * - master and slave
   *
   * \param  ifc            handle of the interface
   * \param  length         length of provided buffer, will hold number of
   *                        received bytes after reception
   * \param  nad            address of the sending node
   * \param  pData          pointer to destination buffer
   *
   * \retval none
   *
   */
#ifdef LIN_21
  STACK void ld_receive_message(l_ifc_handle ifc,
                                l_u16 * const length,
                                l_u8 * const nad,
                                l_u8 * const pData) REENTRANT
#else
  STACK void ld_receive_message(l_ifc_handle ifc,
                                l_u16 *      length,
                                l_u8 *       nad,
                                l_u8 * const pData) REENTRANT
#endif /* LIN_21 */
  {
    l_irqmask               irqMask;

    __CHECK_INTERFACE_VOID__(ifc);

    irqMask = l_sys_irq_disable();

    if ((LIN_COOKED_TP_FLAGS(ifc)
         & (LIN_COOKED_RX_LISTENING | LIN_COOKED_RX_ACTIVE))
        == 0)
    {
      /*
       * the module is not ready to accept a new request
       */

      LIN_COOKED_TP_FLAGS(ifc)          |= LIN_COOKED_RX_LISTENING;

      LIN_COOKED_RX_BYTES_EXPECTED(ifc)  = *length;
      LIN_COOKED_RX_NAD(ifc)             = nad;
      LIN_COOKED_RX_LENGTH(ifc)          = length;
      LIN_COOKED_RX_BUFFER(ifc)          = pData;
      LIN_COOKED_RX_FRAME_COUNTER(ifc)   = 0;
      /*
       * we set length to 0 here because we will use it to count the
       * bytes actually received
       */
      *length = 0;

    }

    l_sys_irq_restore(irqMask);

    return ;

  } /* end ld_receive_message() */

  /*********************************************************************
   *
   * ld_tx_status()
   */
  /**
   * \brief function to read status of cooked Tx module
   *
   * The function will return the status of the last cooked Tx request
   *
   * - diagnostic API
   * - master and slave
   *
   * \param  ifc            handle of the interface
   *
   * \return state of the interface, as defined in the LIN standard
   *
   */
  STACK l_u8 ld_tx_status(l_ifc_handle ifc) REENTRANT
  {
    l_u8                      retval;
     
    __CHECK_INTERFACE_VALUE__(ifc, LD_FAILED);

#ifdef LIN_21
    /* Begin BUGZILLA_540 */
    if ((LIN_COOKED_TP_TIMEOUT_FLAGS(ifc) & LIN_COOKED_N_AS_TIMEOUT) != 0)
    /* End BUGZILLA_540 */
    {
       retval = LD_N_AS_TIMEOUT;
    }
    else
#endif /* LIN_21 */
    if ((LIN_COOKED_TP_FLAGS(ifc) & LIN_COOKED_TX_ERROR) != 0)
    {
       retval = LD_FAILED;
    }
    else if ((LIN_DIAG_FLAGS(ifc) & (l_u8) LIN_COOKED_TX_READY) != 0)
    {
       retval = LD_IN_PROGRESS;
    }
    else
    {
       retval = LD_COMPLETED;
    }
    
    return retval;

  } /* end ld_tx_status() */

  /*********************************************************************
   *
   * ld_rx_status()
   */
  /**
   * \brief function to read status of cooked Rx module
   *
   * The function will return the status of the last cooked Rx request
   *
   * - diagnostic API
   * - master and slave
   *
   * \param  ifc            handle of the interface
   *
   * \retval state of the interface, as defined in the LIN standard
   *
   */
  STACK l_u8 ld_rx_status (l_ifc_handle ifc) REENTRANT
  {
    l_u8                      retval;
    __CHECK_INTERFACE_VALUE__(ifc, LD_FAILED);


#ifdef LIN_21
    /* Begin BUGZILLA_540 */
    if ((LIN_COOKED_TP_TIMEOUT_FLAGS(ifc) & LIN_COOKED_N_CR_TIMEOUT) != 0)
    /* End BUGZILLA_540 */
    {
       retval = LD_N_CR_TIMEOUT;
    }
    else if ((LIN_COOKED_TP_FLAGS(ifc) & LIN_COOKED_RX_WRONG_SN) != 0)
    {
       retval = LD_WRONG_SN;
    }
    else
#endif /* LIN_21 */
    if ((LIN_COOKED_TP_FLAGS(ifc) & LIN_COOKED_RX_ERROR) != 0)
    {
       retval = LD_FAILED;
    }
    else if ((LIN_COOKED_TP_FLAGS(ifc)
         & (LIN_COOKED_RX_LISTENING | LIN_COOKED_RX_ACTIVE)) != 0)
    {
       retval = LD_IN_PROGRESS;
    }
    else
    {
       retval = LD_COMPLETED;
    }

    return retval;

  } /* end ld_rx_status() */

#endif /* LIN_INCLUDE_COOKED_TP */

/*********************************************************************
 *
 * LIN 2.0 class 3 cooked diagnostic module
 *
 *********************************************************************/

#ifdef LIN_INCLUDE_COOKED_TP

  /*********************************************************************
   *
   * ld_diag3CookedTxHandler()
   */
  /**
   * \brief function to handle cooked message send
   *
   * This function will be called to determine whether a class 3 cooked
   * Tx transfer is due
   * If a frame is due, the function will compose the frame into pBuffer
   *
   * - internal function
   * - master and slave
   *
   * Parameters:
   * \param  ifc            handle of the interface
   * \param  pBuffer        buffer to compose diagnostic frame in
   *
   * \retval 1 if a diagnostic frame has been copied into buffer
   * \retval 0 otherwise
   *
   *
   * \attention Usually a diagnostic frame is identified by a SID as well.
   *            When doing class 3 cooked diagnostics, the SID is part
   *            of the message, and is contained in the data area.
   *            Therefore, the length of the message is one more than the
   *            actual data length
   *
   */
  STACK l_bool ld_diag3CookedTxHandler(__OPTIONAL_IFC_DEFINITION_COMMA__
                                       l_u8 * pBuffer)
  {
    l_u8                      i;
    l_u8                      offset;
    l_u8                      size;
    l_bool                    retval = 0;

    if ((LIN_DIAG_FLAGS(ifc) & (l_u8) LIN_COOKED_TX_READY) != 0)
    {
      /*
       * there is something to send
       */
      /*
       * we just fill the data buffer with 0xff, this is more efficient
       * than figuring out whether this is really needed
       */
      pBuffer[LIN_DIAG_CF_FRAME_DATA_POS + 1] = 0xff;
      pBuffer[LIN_DIAG_CF_FRAME_DATA_POS + 2] = 0xff;
      pBuffer[LIN_DIAG_CF_FRAME_DATA_POS + 3] = 0xff;
      pBuffer[LIN_DIAG_CF_FRAME_DATA_POS + 4] = 0xff;
      pBuffer[LIN_DIAG_CF_FRAME_DATA_POS + 5] = 0xff;

      pBuffer[LIN_DIAG_FRAME_NAD_POS]     = LIN_COOKED_TX_NAD(ifc);

#if (defined(LIN_21) && defined(LIN_SLAVE_NODE))
      /* Set status of LIN 2.1 Slave Node Transmission Handler - NAD is own NAD */
      l_ifc_set_TP_status(LIN_STATUS_TX_PHYSICAL_RESPONSE);
#endif /* defined(LIN_21) && defined(LIN_SLAVE_NODE) */

      if (LIN_COOKED_TX_BYTES_SENT(ifc) == 0)
      {
        /*
         * this is the first frame
         */
        if (LIN_COOKED_TX_LENGTH(ifc)
            <= (LIN_DIAG_SF_FRAME_DATA_LEN_MAX + 1) /* SID*/)
        {
           #if (defined(LIN_21) && defined(LIN_MASTER_NODE))
             /* Set status of LIN 2.1 Master Node Transmission Handler */
             /* If NAD = 0x7E (Functional request can be only a Single Frame) */
             if (pBuffer[LIN_DIAG_FRAME_NAD_POS] == LD_FUNCTIONAL_NAD)
             	{
                l_ifc_set_TP_status(LIN_STATUS_TX_FUNCTIONAL_ACTIVE);
              }
           #endif /* defined(LIN_21) && defined(LIN_MASTER_NODE) */
           
           /*
            * data will fit into one frame, so compose
            * SF message
            */
           pBuffer[LIN_DIAG_FRAME_PCI_POS]    = (l_u8) (LIN_COOKED_TX_LENGTH(ifc) +
                                                LIN_DIAG_PCI_SF);

           pBuffer[LIN_DIAG_SF_FRAME_SID_POS] = LIN_COOKED_TX_BUFFER(ifc)[0];

           /* SID is already coded */
           LIN_COOKED_TX_BYTES_SENT(ifc)      = 1;
           /* Begin BUGZILLA_472 */
           size    = (l_u8) (LIN_COOKED_TX_LENGTH(ifc) - 1); /* SID */
           /* End BUGZILLA_472 */
           offset  = LIN_DIAG_SF_FRAME_DATA_POS;

        }
        else
        {
           #if (defined(LIN_21) && defined(LIN_MASTER_NODE))
             /* Set status of LIN 2.1 Master Node Transmission Handler */
             /* A First Frame can only be a Physical request */
             l_ifc_set_TP_status(LIN_STATUS_TX_PHYSICAL_ACTIVE);
           #endif /* defined(LIN_21) && defined(LIN_MASTER_NODE) */
           
           /*
            * several frames needed, compose FF
            */
           /* Begin BUGZILLA_472 */
           pBuffer[LIN_DIAG_FRAME_PCI_POS]
              = ((l_u8) (((l_u8)(((l_u8)(LIN_COOKED_TX_LENGTH(ifc) / 256)) & LIN_DIAG_PCI_LENGTH_MASK)) + LIN_DIAG_PCI_FF));
           /* End BUGZILLA_472 */

           pBuffer[LIN_DIAG_FF_FRAME_LEN_POS]
              = (l_u8) LIN_COOKED_TX_LENGTH(ifc);

           pBuffer[LIN_DIAG_FF_FRAME_SID_POS] = LIN_COOKED_TX_BUFFER(ifc)[0];

           /* SID is already coded */
           LIN_COOKED_TX_BYTES_SENT(ifc)      = 1;

           size    = 4;
           offset  = LIN_DIAG_FF_FRAME_DATA_POS;
        }
      } /* if (first frame) */
      else
      {
        #if (defined(LIN_21) && defined(LIN_MASTER_NODE))
          /* Set status of LIN 2.1 Master Node Transmission Handler */
          /* A First Frame can only be a Physical request */
           l_ifc_set_TP_status(LIN_STATUS_TX_PHYSICAL_ACTIVE);
        #endif /* defined(LIN_21) && defined(LIN_MASTER_NODE) */

        /*
         * not the first frame, will always be a CF
         */
        /* Begin BUGZILLA_472 */
        pBuffer[LIN_DIAG_FRAME_PCI_POS] =
           ((l_u8) (((l_u8) (LIN_COOKED_TX_FRAME_COUNTER(ifc) & LIN_DIAG_PCI_FRAMECOUNTER_MASK)) + LIN_DIAG_PCI_CF));
        /* End BUGZILLA_472 */
        if (LIN_COOKED_TX_LENGTH(ifc) >= (LIN_COOKED_TX_BYTES_SENT(ifc) + 6))
        {
           size = 6;
        }
        else
        {
           size = (l_u8) (LIN_COOKED_TX_LENGTH(ifc) - LIN_COOKED_TX_BYTES_SENT(ifc));
        }

        offset = LIN_DIAG_CF_FRAME_DATA_POS;
      } /* if (first frame) */

      /*
       * now we fill the data buffer of the message
       */
      for (i = 0; i < size; ++i)
      {
         pBuffer[offset + i]
            = LIN_COOKED_TX_BUFFER(ifc)[LIN_COOKED_TX_BYTES_SENT(ifc) + (l_u16) i];
      }

      LIN_COOKED_TX_BYTES_SENT(ifc) += (l_u16) i;


      if (LIN_COOKED_TX_BYTES_SENT(ifc) >= LIN_COOKED_TX_LENGTH(ifc))
      {
         /*
          * the message has been completely sent,
          * clean up now
          */
         LIN_DIAG_FLAGS(ifc) &= (l_u8) ~LIN_COOKED_TX_READY;

      }
      else
      {
         /*
          * not yet finsished, store status
          */
         LIN_COOKED_TX_FRAME_COUNTER(ifc)  =
            (l_u8) ((LIN_COOKED_TX_FRAME_COUNTER(ifc) + (l_u8) 1)
                     % (l_u8) LIN_DIAG_FRAME_COUNTER_MAX);
      }

      retval = 1;

    } /* if (something to send) */

    return retval;

  } /* end ld_diag3CookedTxHandler() */

  /*********************************************************************
   *
   * ld_diag3CookedRxHandler()
   */
  /**
   * \brief function controlling cooked message reception
   *
   * This function is called with a diagnostic frame received.
   * It will figure out whether this message is currently expected.
   *
   * - internal function
   * - master and slave
   *   
   * \param  ifc            handle of the interface
   * \param  pBuffer        buffer holding the message received
   *
   * \retval 1 if the message has been consumed
   * \retval 0 otherwise
   *
   */
  STACK l_bool ld_diag3CookedRxHandler(__OPTIONAL_IFC_DEFINITION_COMMA__
                                       const l_u8 *    pBuffer)
  {
    l_bool                  retval = 0;
    l_u8                    frameType;
    l_u8                    offset = 0;
    l_u8                    size = 0;
    l_u16                   length;
    #ifdef LIN_ABORT_TP_PREVIOUS_SESSION
    l_u16 									temp = 0;
    #endif
    l_u8                    i;
    l_u8                    receiveError = 0;
#ifdef LIN_21
    l_bool                  receiveErrorSerialNumber = 0;
    l_bool                  receiveFunctAsFF = 0;
    l_bool                  receiveFunctAsCF = 0;
#endif /* LIN_21 */

#if (defined(LIN_21) && defined(LIN_MASTER_NODE))
    /* Set status of LIN 2.1 Master Node Transmission Handler */
    /* Regardless of frame type, it can only be a Physical request */
    l_ifc_set_TP_status(LIN_STATUS_RX_PHYSICAL_ACTIVE);
#endif /* defined(LIN_21) && defined(LIN_MASTER_NODE) */
        
    frameType = (l_u8) (pBuffer[LIN_DIAG_FRAME_PCI_POS]
                        & LIN_DIAG_PCI_FRAMETYPE_MASK);

    if ((LIN_COOKED_TP_FLAGS(ifc) & LIN_COOKED_RX_LISTENING) != 0)
    {
      /*
       * receive has been requested, but did not start yet
       */
			/* Begin BUGZILLA_1165 */
      #if (defined(LIN_21) && defined(LIN_SLAVE_NODE)) 
      #ifdef LIN_ABORT_TP_PREVIOUS_SESSION
			if((LIN_DIAG_FLAGS(ifc) &  LIN_COOKED_TX_READY) != 0)
			{
			  if (pBuffer[LIN_DIAG_FRAME_NAD_POS] == LD_FUNCTIONAL_NAD)
				{
					return retval;
				}
			  else 
				{
					LIN_DIAG_FLAGS(ifc)              &= (l_u8) ~LIN_COOKED_TX_READY;
					LIN_COOKED_TX_LENGTH(ifc)         = 0;
					LIN_COOKED_TX_NAD(ifc)            = 0;
					LIN_COOKED_TX_BUFFER(ifc)         = 0;
					LIN_COOKED_TX_BYTES_SENT(ifc)     = 0;
					LIN_COOKED_TX_FRAME_COUNTER(ifc)  = 0;
				}				
      }			
      #endif	 
			#endif
			/* End BUGZILLA_1165 */
      /*
       * handling depends on frame type
       */
      if (frameType == LIN_DIAG_PCI_FF)
      {
         /* Begin BUGZILLA_540 */
         #ifdef LIN_21
           #ifdef LIN_MASTER_NODE
             #ifndef N_CR_TIMEOUT           /* If not defined in lin_def.h, we define the default value */
               #define N_CR_TIMEOUT 1000    /* 1000 msec is the default value */
             #endif /* N_CR_TIMEOUT */
             l_TimerStart_N_Cr(__OPTIONAL_TIMER_IFC_PARAMETER_COMMA__ N_CR_TIMEOUT );
           #endif /* LIN_MASTER_NODE */
         #endif /* LIN_21 */
         /* End BUGZILLA_540 */
         LIN_COOKED_TP_FLAGS(ifc) &= (l_u8) ~LIN_COOKED_RX_FLAG_MASK;
         LIN_COOKED_TP_FLAGS(ifc) |=  LIN_COOKED_RX_ACTIVE;

         /*
          * this frame will make the node start receiving the frame
          */
         *(LIN_COOKED_RX_NAD(ifc)) = pBuffer[LIN_DIAG_FRAME_NAD_POS];

         /*
          * get length of message
          */
         length
            = (l_u16) ((((l_u16) (pBuffer[LIN_DIAG_FRAME_PCI_POS]
                                  & LIN_DIAG_PCI_LENGTH_MASK)) << CHAR_BIT) +
                           pBuffer[LIN_DIAG_FF_FRAME_LEN_POS]);

         size = 4 + 1 /* SID */;
         offset = LIN_DIAG_FF_FRAME_SID_POS;

        /*
         * compare expected length against the possible maximum length
         */
        if (length < LIN_COOKED_RX_BYTES_EXPECTED(ifc))
        {
          /*
           * the message will be shorter than the buffer provided
           * so adjust the value
           */
           LIN_COOKED_RX_BYTES_EXPECTED(ifc) = length;

        } /* if (length check) */

#if (defined(LIN_21) && defined(LIN_SLAVE_NODE))
        if (pBuffer[LIN_DIAG_FRAME_NAD_POS] == LD_FUNCTIONAL_NAD)
        {
        	/* It is not permitted to send a functional request as First Frame */
        	/* then set an error */
          receiveError = 1;
          receiveFunctAsFF = 1;
        } /* if NAD = 0x7E (functional) */
        else
        {
        	/* Set status of Slave Node Transmission Handler */
        	l_ifc_set_TP_status(LIN_STATUS_RX_PHYSICAL_REQUEST);
        } /* NAD is own NAD or 0x7F (broadcast) */	
#endif /* defined(LIN_21) && defined(LIN_SLAVE_NODE) */

        retval = 1;
      } /* if (frame type) */
      else if (frameType == LIN_DIAG_PCI_SF)
      {
        /*
         * we are about to receive a single frame message
         */
         *(LIN_COOKED_RX_NAD(ifc)) = pBuffer[LIN_DIAG_FRAME_NAD_POS];

         /* length includes SID */
         length = ((l_u16) pBuffer[LIN_DIAG_FRAME_PCI_POS] & LIN_DIAG_PCI_LENGTH_MASK);

         size = (l_u8) length;
         offset = LIN_DIAG_SF_FRAME_SID_POS;

         /*
          * compare expected length against the possible maximum length
          */
         if (length < LIN_COOKED_RX_BYTES_EXPECTED(ifc))
         {
           /*
            * the message will be shorter than the buffer provided
            * so adjust the value
            */
           LIN_COOKED_RX_BYTES_EXPECTED(ifc) = length;
           
         } /* if (length check) */

#if (defined(LIN_21) && defined(LIN_SLAVE_NODE))
        if (pBuffer[LIN_DIAG_FRAME_NAD_POS] == LD_FUNCTIONAL_NAD)
        {
        	/* Set status of Slave Node Transmission Handler */
          l_ifc_set_TP_status(LIN_STATUS_RX_FUNCTIONAL_REQUEST);
        } /* if NAD = 0x7E (functional) */
        else
        {
        	/* Set status of Slave Node Transmission Handler */
        	l_ifc_set_TP_status(LIN_STATUS_RX_PHYSICAL_REQUEST);
        } /* NAD is own NAD or 0x7F (broadcast) */	
#endif /* defined(LIN_21) && defined(LIN_SLAVE_NODE) */

         retval = 1;

      } /* if (frame type) */
      else
      {
        /*
         * a CF frame means that there is already a transmission
         * ongoing. We will ignore this transfer and wait for the
         * next complete transmission. Hence, there is nothing to do
         * now
         */
        ;

      } /* if (frameType) */

    } /* if (state) */
    else if ((LIN_COOKED_TP_FLAGS(ifc) & LIN_COOKED_RX_ACTIVE) != 0)
    {
      /*
       * a receive operation has already started
       * only CF now valid
       */

      if (frameType == LIN_DIAG_PCI_CF)
      {
        /*
         * check whether the frame is ok
         */

#if (defined(LIN_21) && defined(LIN_SLAVE_NODE))
        if (pBuffer[LIN_DIAG_FRAME_NAD_POS] != (*(LIN_COOKED_RX_NAD(ifc))))
        {
          /*
           * the nad does not match
           */
          receiveError = 1;
        } /* if (NAD ok) */
        else if (pBuffer[LIN_DIAG_FRAME_NAD_POS] == LD_FUNCTIONAL_NAD)
        {
        	/* It is not permitted to send a functional request as First Frame */
        	/* then set an error */
          receiveError = 1;
          receiveFunctAsCF = 1;
        } /* if NAD = 0x7E (functional) */
        else if ((((l_u16) pBuffer[LIN_DIAG_FRAME_PCI_POS] & (l_u16) LIN_DIAG_PCI_FRAMECOUNTER_MASK))
              != ((LIN_COOKED_RX_FRAME_COUNTER(ifc)) & (l_u16)LIN_DIAG_PCI_FRAMECOUNTER_MASK))
        {
          /*
           * the frame counter does not match
           */
          receiveError = 1;
          receiveErrorSerialNumber = 1;
        } /* if (frame counter ok) */
#else
         if ((pBuffer[LIN_DIAG_FRAME_NAD_POS] != (*(LIN_COOKED_RX_NAD(ifc))))
             ||
             ((((l_u16) pBuffer[LIN_DIAG_FRAME_PCI_POS] & (l_u16) LIN_DIAG_PCI_FRAMECOUNTER_MASK))
              != ((LIN_COOKED_RX_FRAME_COUNTER(ifc)) & (l_u16)LIN_DIAG_PCI_FRAMECOUNTER_MASK)))
        {
          /*
           * either the nad or the frame counter does not
           * match
           */
          receiveError = 1;
        } /* if (frame ok) */
#endif /* defined(LIN_21) && defined(LIN_SLAVE_NODE) */
        else
        {
          /*
           * the frame seems to be ok, copy contents
           */
           if ((*(LIN_COOKED_RX_LENGTH(ifc)) + 6) > LIN_COOKED_RX_BYTES_EXPECTED(ifc))
           {
              /* This is the last CF received correctly, so here we stop the LIN 2.1 N_Cr timer */
              /* Begin BUGZILLA_540 */
              #ifdef LIN_21
                l_TimerStop_N_Cr(__OPTIONAL_TIMER_IFC_PARAMETER__);
              #endif /* LIN_21 */
              /* End BUGZILLA_540 */
              size = (l_u8) (LIN_COOKED_RX_BYTES_EXPECTED(ifc) - *(LIN_COOKED_RX_LENGTH(ifc)));
           }
           else
           {
              size = 6;
              /* Begin BUGZILLA_540 */
              #ifdef LIN_21
                #ifdef LIN_MASTER_NODE
                  #ifndef N_CR_TIMEOUT           /* If not defined in lin_def.h, we define the default value */
                    #define N_CR_TIMEOUT 1000    /* 1000 msec is the default value */
                  #endif /* N_CR_TIMEOUT */
                  /* Reload timer N_Cr */
                  l_TimerStart_N_Cr(__OPTIONAL_TIMER_IFC_PARAMETER_COMMA__ N_CR_TIMEOUT );
                #endif /* LIN_MASTER_NODE */
              #endif /* LIN_21 */
              /* End BUGZILLA_540 */
           }
           offset = LIN_DIAG_CF_FRAME_DATA_POS;

#if (defined(LIN_21) && defined(LIN_SLAVE_NODE))
        	 /* Set status of Slave Node Transmission Handler - NAD is own NAD or 0x7F (broadcast) */
        	 l_ifc_set_TP_status(LIN_STATUS_RX_PHYSICAL_REQUEST);
#endif /* defined(LIN_21) && defined(LIN_SLAVE_NODE) */

           retval = 1;

        } /* CF data valid
           */
      } /* if (CF) */
      else
      {
          /* Begin BUGZILLA_1164 */
					#if (defined(LIN_21) && defined(LIN_SLAVE_NODE))
          if (pBuffer[LIN_DIAG_FRAME_NAD_POS] == LD_FUNCTIONAL_NAD)
          {
						return retval;  // no action if functional NAD comes
					}					
          #endif /* defined(LIN_21) && defined(LIN_SLAVE_NODE) */
					/* End BUGZILLA_1164 */
					
					#ifdef LIN_ABORT_TP_PREVIOUS_SESSION
          if (frameType == LIN_DIAG_PCI_SF)
          {
            switch (previous_frameType)
            {
              case LIN_DIAG_PCI_CF:
              case LIN_DIAG_PCI_FF:

              	/* Clear the buffer */
              	temp = ((l_u16) *(LIN_COOKED_RX_LENGTH(ifc)));

              	for (i = 0; i < temp; ++i)
                {
                  LIN_COOKED_RX_BUFFER(ifc)[i] = 0;
                }                

                /* Reply the same management we did when SF was received as first message */

                /*
                 * we are about to receive a single frame message
                 */
                *(LIN_COOKED_RX_NAD(ifc)) = pBuffer[LIN_DIAG_FRAME_NAD_POS];

                /* length includes SID */
                length = ((l_u16) pBuffer[LIN_DIAG_FRAME_PCI_POS] & LIN_DIAG_PCI_LENGTH_MASK);

                size = (l_u8) length;
                offset = LIN_DIAG_SF_FRAME_SID_POS;

                /* the length is known and is = size */
                LIN_COOKED_RX_BYTES_EXPECTED(ifc) = length;

                /* Clear the length buffer */
                *(LIN_COOKED_RX_LENGTH(ifc)) = (l_u16) 0;
								
                /* Begin BUGZILLA_1164 */
								#if (defined(LIN_21) && defined(LIN_SLAVE_NODE))
								l_ifc_set_TP_status(LIN_STATUS_RX_PHYSICAL_REQUEST); 
                #endif /* defined(LIN_21) && defined(LIN_SLAVE_NODE)
                /* End BUGZILLA_1164 */
                								
                retval = 1;

              break;

              case LIN_DIAG_PCI_SF:
                NoOperation();  /* should never happen: no action */
              break;

              default:
                NoOperation();  /* should never happen: no action */
              break;
            }
          }
          else if (frameType == LIN_DIAG_PCI_FF) /* FF received */
          {
            switch (previous_frameType)
            {
              case LIN_DIAG_PCI_CF:
              case LIN_DIAG_PCI_SF:
              case LIN_DIAG_PCI_FF:
							
							  /* Begin BUGZILLA_540 */
               #ifdef LIN_21
                 #ifdef LIN_MASTER_NODE
                   #ifndef N_CR_TIMEOUT           /* If not defined in lin_def.h, we define the default value */
                     #define N_CR_TIMEOUT 1000    /* 1000 msec is the default value */
                   #endif /* N_CR_TIMEOUT */
                   l_TimerStart_N_Cr(__OPTIONAL_TIMER_IFC_PARAMETER_COMMA__ N_CR_TIMEOUT );
                 #endif /* LIN_MASTER_NODE */
               #endif /* LIN_21 */
               /* End BUGZILLA_540 */
               LIN_COOKED_TP_FLAGS(ifc) &= (l_u8) ~LIN_COOKED_RX_FLAG_MASK;
               LIN_COOKED_TP_FLAGS(ifc) |=  LIN_COOKED_RX_ACTIVE;


              	/* Clear the buffer */
              	temp = ((l_u16) *(LIN_COOKED_RX_LENGTH(ifc)));

              	for (i = 0; i < temp; ++i)
                {
                  LIN_COOKED_RX_BUFFER(ifc)[i] = 0;
                }


                /* Clear frame counter */
                LIN_COOKED_RX_FRAME_COUNTER(ifc)  = 0;

                /* Reply the same management we did when FF was received as first message */

                /*
                 * we are about to receive a first frame message
                 */
                *(LIN_COOKED_RX_NAD(ifc)) = pBuffer[LIN_DIAG_FRAME_NAD_POS];

                /* length includes SID */ 
                length
                        = (l_u16) ((((l_u16) (pBuffer[LIN_DIAG_FRAME_PCI_POS]
                                  & LIN_DIAG_PCI_LENGTH_MASK)) << CHAR_BIT) +
                           pBuffer[LIN_DIAG_FF_FRAME_LEN_POS]);

                size = 4 + 1 /* SID */;
                offset = LIN_DIAG_FF_FRAME_SID_POS;

								/*
								* compare expected length against the possible maximum length
                */
                if (length < LIN_COOKED_RX_BYTES_EXPECTED(ifc))
								{
									/*
                   * the message will be shorter than the buffer provided
                   * so adjust the value
                  */                
                    LIN_COOKED_RX_BYTES_EXPECTED(ifc) = length;
 
                } /* if (length check) */


                /* Clear the length buffer */
                *(LIN_COOKED_RX_LENGTH(ifc)) = (l_u16) 0;

								
								#if (defined(LIN_21) && defined(LIN_SLAVE_NODE))
								l_ifc_set_TP_status(LIN_STATUS_RX_PHYSICAL_REQUEST); 
                #endif                          

                retval = 1;

              break;

              default:
                NoOperation();  /* should never happen: no action */
              break;
            }
          }
					else
					{
						/* no action */
					}
        #else
        /*
          * only CFs are currently expected, so this is an error
          */
        receiveError = 1;
        #endif
      }

    } /* if (listening/active) */
    else
    {
       /*
        * do nothing if module is neither active nor listening
        */
       ;
    }
    /*
     * now we copy the contents of the frame
     */
    if (retval != 0)
    {
       for (i = 0; i < size; ++i)
       {
          LIN_COOKED_RX_BUFFER(ifc)[*(LIN_COOKED_RX_LENGTH(ifc)) + (l_u16) i]
             = pBuffer[offset + i];
       }

       *(LIN_COOKED_RX_LENGTH(ifc)) += (l_u16) size;

       /*
        * now check whether the message is complete
        */
       if (*LIN_COOKED_RX_LENGTH(ifc) >= LIN_COOKED_RX_BYTES_EXPECTED(ifc))
       {
          /*
           * the frame is complete, clear cooked Rx flags
           */
          LIN_COOKED_TP_FLAGS(ifc) &= (l_u8) ~LIN_COOKED_RX_FLAG_MASK;
          
                    
          #if (defined(LIN_21) && defined(LIN_SLAVE_NODE))
            /* Set status of LIN 2.1 Slave Node Transmission Handler - NAD is own NAD */
            l_ifc_set_TP_status(LIN_STATUS_TX_PHYSICAL_RESPONSE);
          #endif /* defined(LIN_21) && defined(LIN_SLAVE_NODE) */
       }
       else
       {
          /*
           * not the last frame, only advance the frame counter
           */
          LIN_COOKED_RX_FRAME_COUNTER(ifc)++;
       }

    } /* if (frame consumed) */

    if (receiveError != 0)
    {
       /*
        * something went wrong, transfer of the message has been corrupted!
        */
       LIN_COOKED_TP_FLAGS(ifc) &= (l_u8) ~LIN_COOKED_RX_FLAG_MASK;
#if (defined(LIN_21) && defined(LIN_SLAVE_NODE))
       /* Set status of Slave Node Transmission Handler to Idle*/
       l_ifc_set_TP_status(LIN_STATUS_IDLE);
       if (receiveErrorSerialNumber != 0)
       {
         /*
          * received serial number is wrong, transfer of the message has been corrupted!
          */
       	 LIN_COOKED_TP_FLAGS(ifc) |= LIN_COOKED_RX_WRONG_SN;
       	 receiveErrorSerialNumber = 0;
       } /* if (receiveErrorSerialNumber) */
       else if (receiveFunctAsFF != 0)
       {
         /*
          * received a functional request as First Frame is wrong, transfer of the message has been corrupted!
          */
       	 LIN_COOKED_TP_FLAGS(ifc) |= LIN_COOKED_RX_ERROR;
       	 receiveFunctAsFF = 0;
       } /* if (receiveFunctAsFF) */
       else if (receiveFunctAsCF != 0)
       {
         /*
          * received a functional request as Consecutive Frame is wrong, transfer of the message has been corrupted!
          */
       	 LIN_COOKED_TP_FLAGS(ifc) |= LIN_COOKED_RX_ERROR;
       	 receiveFunctAsCF = 0;
       } /* if (receiveFunctAsCF) */
       /* Begin BUGZILLA_1006 */
       else
       {
         /* 
          * received a SF or FF instead of a Consecutive Frame; that is wrong and the transfer of the message has been corrupted!
          */
       	 LIN_COOKED_TP_FLAGS(ifc) |= LIN_COOKED_RX_ERROR;
       }
       /* End BUGZILLA_1006 */
#else
       LIN_COOKED_TP_FLAGS(ifc) |= LIN_COOKED_RX_ERROR;
#endif /* defined(LIN_21) && defined(LIN_SLAVE_NODE) */
    } /* if (receiveError) */

    #ifdef LIN_ABORT_TP_PREVIOUS_SESSION  
    previous_frameType = frameType;
    #endif
    return retval;

  } /* end ld_diag3CookedRxHandler() */

#endif /* LIN_INCLUDE_COOKED_TP */

/*********************************************************************
 *
 * LIN 2.0 class 3 raw diagnostic module
 *
 *********************************************************************/

#ifdef LIN_INCLUDE_RAW_TP

  /*********************************************************************
   *
   * ld_diag3RawTxHandler()
   */
  /**
   * \brief function handling raw frame send
   *
   * This function is called to determine whether a level 3 raw tx
   * operation has been requested
   *
   * - internal function
   * - master and slave
   *
   * \param  ifc            handle of the interface to send request on
   * \param  pBuffer        buffer to compose message into
   *
   * \retval 1 if a frame is to be sent
   * \retval 0 otherwise
   *
   */
  STACK l_bool ld_diag3RawTxHandler(__OPTIONAL_IFC_DEFINITION_COMMA__
                                    l_u8 *     pBuffer)
  {
    return ld_diag3RawTxFifoGet(__OPTIONAL_IFC_PARAMETER_COMMA__
                                pBuffer);

  } /* end ld_diag3RawTxHandler() */

  /*********************************************************************
   *
   * ld_diag3RawRxHandler()
   */
  /**
   * \brief function handling raw frame receive
   *
   * This function is called whenever an incoming diagnostic request
   * has not been previously consumed
   *
   * - internal function
   * - master and slave
   *
   * \param  ifc            handle of the interface
   * \param  pBuffer        buffer holding the message
   *
   * \retval 1 always (for a uniform interface)
   *
   */
  STACK void ld_diag3RawRxHandler (__OPTIONAL_IFC_DEFINITION_COMMA__
                                     const l_u8 *pBuffer)
  {

    ld_diag3RawRxFifoStore(__OPTIONAL_IFC_PARAMETER_COMMA__ pBuffer);

    return ;

  } /* end ld_diag3RawRxHandler() */

  /*********************************************************************
   *
   * ld_diag3RawTxFifoGet()
   */
  /**
   * \brief management of raw Tx stack
   *
   * This function extracts one frame from the Tx Fifo stack
   *
   * - internal function
   * - master and slave
   *
   * \param  ifc            handle of the interface
   * \param  pBuffer        buffer to copy message to
   *
   * \retval 1 if a message has been filled into pBuffer
   * \retval 0 otherwise
   *
   */
  STACK  l_bool ld_diag3RawTxFifoGet (__OPTIONAL_IFC_DEFINITION_COMMA__
                                      l_u8 *pBuffer)
  {
    l_bool               retval = 0;
    l_u8                 i;
    l_u8                 j;
    /* Begin BUGZILLA_472 */
    l_u8              temp = (l_u8) (LIN_RAW_TX_STACK_START(ifc) * LIN_DIAG_FRAME_LEN_MAX);
    /* End BUGZILLA_472 */

    /*
     * check whether we have a frame in stock
     */
    if ((LIN_RAW_TP_FLAGS(ifc) & (l_u8) LIN_RAW_TX_QUEUE_EMPTY) == 0)
    {
      /*
       * we do have a frame, copy it into buffer
       */
       /*
        * MISRA deviation, rule 12.10
        * Reason: Two variables control behaviour of for loop
        *         hence comma operator is required
        */
      /* Begin BUGZILLA_472 */
      for (i = 0, j = temp; i < LIN_DIAG_FRAME_LEN_MAX; ++i, ++j)
      /* End BUGZILLA_472 */
      {
        pBuffer[i] = LIN_RAW_TX_STACK(ifc)[j];
      }

      /*
       * advance stack start counter
       */
      LIN_RAW_TX_STACK_START(ifc) =
         (LIN_RAW_TX_STACK_START(ifc) + 1) % LIN_DIAG3_FIFO_SIZE_MAX;

      /*
       * stack cannot be full anymore
       */
      LIN_RAW_TP_FLAGS(ifc) &= (l_u8) ~LIN_RAW_TX_QUEUE_FULL;

      if (LIN_RAW_TX_STACK_START(ifc) == LIN_RAW_TX_STACK_END(ifc))
      {
        /*
         * we have removed the last frame from the stack
         */
         LIN_DIAG_FLAGS(ifc)   &= (l_u8) ~LIN_RAW_TX_READY;
         LIN_RAW_TP_FLAGS(ifc) |= (l_u8) LIN_RAW_TX_QUEUE_EMPTY;
      }
      else
      {
         /*
          * if there is data left, the module is still able to send
          */
         LIN_DIAG_FLAGS(ifc) |= (l_u8) LIN_RAW_TX_READY;
      }
      retval = 1;

    } /* if (!stackEmpty) */

    return retval;

  } /* end ld_diag3RawTxFifoGet() */

  /*********************************************************************
   *
   * ld_diag3RawTxFifoStore()
   */
  /**
   * \brief management of raw Tx stack
   *
   * This function stores one diag frame on the tx stack
   *
   * - internal function
   * - master and slave
   *
   * \param  ifc            handle of the interface
   * \param  pBuffer        buffer holding the frame
   *
   * \retval 1 if the frame has been accepted
   * \retval 0 otherwise
   *
   */
  STACK static l_bool ld_diag3RawTxFifoStore(__OPTIONAL_IFC_DEFINITION_COMMA__
                                             const l_u8 *   pBuffer)
  {
    l_bool               retval = 0;
    l_u8                 i;
    l_u8                 j;
    /* Begin BUGZILLA_472 */
    l_u8              temp = (l_u8) (LIN_RAW_TX_STACK_END(ifc) * LIN_DIAG_FRAME_LEN_MAX);
    /* End BUGZILLA_472 */

    if ((LIN_RAW_TP_FLAGS(ifc) & (l_u8) LIN_RAW_TX_QUEUE_FULL) == 0)
    {
      /*
       * there is storage left on stack, store frame
       */

      /*
       * copy frame on stack
       */
      /*
       * MISRA deviation, rule 12.10
       * Reason: Two variables control behaviour of for loop
       *         hence comma operator is required
       */
      /* Begin BUGZILLA_472 */
      for (i = 0, j = temp; i < LIN_DIAG_FRAME_LEN_MAX; ++i, ++j)
      /* End BUGZILLA_472 */
      {
        LIN_RAW_TX_STACK(ifc)[j] = pBuffer[i];
      } /* for (complete frame) */

      /*
       * advance stack end counter
       */
      LIN_RAW_TX_STACK_END(ifc) =
         (LIN_RAW_TX_STACK_END(ifc) + 1) % LIN_DIAG3_FIFO_SIZE_MAX;

      /*
       * adding one element means the stack is not empty
       */
      LIN_RAW_TP_FLAGS(ifc) &= (l_u8) ~LIN_RAW_TX_QUEUE_EMPTY;

      /*
       * check whether stack is full now
       */
      if (LIN_RAW_TX_STACK_START(ifc) == LIN_RAW_TX_STACK_END(ifc))
      {
        /*
         * we have added the last element, the stack is now full
         */
         LIN_RAW_TP_FLAGS(ifc) |= (l_u8) LIN_RAW_TX_QUEUE_FULL;
      }

      /*
       * the module is ready to send
       */
      LIN_DIAG_FLAGS(ifc) |= (l_u8) LIN_RAW_TX_READY;

      retval = 1;

    } /* if (stack full) */

    return retval;

  } /* end ld_diag3RawTxFifoStore() */

  /*********************************************************************
   *
   * ld_diag3RawRxFifoGet()
   */
  /**
   * \brief management of raw Rx stack
   *
   * This function extracts one frame from the raw Rx Fifo stack
   *
   * - internal function
   * - master and slave
   *
   * \param  ifc            handle of the interface
   * \param  pBuffer        buffer to copy message to
   *
   * \retval 1 if frame has been returned
   * \retval 0 otherwise
   *
   */
  STACK static l_bool ld_diag3RawRxFifoGet(__OPTIONAL_IFC_DEFINITION_COMMA__
                                           l_u8 *   pBuffer)
  {
    l_bool               retval = 0;
    l_u8                 i;
    l_u8                 j;
    /* Begin BUGZILLA_472 */
    l_u8              temp = (l_u8) (LIN_RAW_RX_STACK_START(ifc) * LIN_DIAG_FRAME_LEN_MAX);
    /* End BUGZILLA_472 */

    if ((LIN_RAW_TP_FLAGS(ifc) &(l_u8)  LIN_RAW_RX_QUEUE_EMPTY) == 0)
    {
      /*
       * there is data on the stack
       */
      /*
       * MISRA deviation, rule 12.10
       * Reason: Two variables control behaviour of for loop
       *         hence comma operator is required
       */
      /* Begin BUGZILLA_472 */
      for (i = 0, j = temp; i < LIN_DIAG_FRAME_LEN_MAX; ++i, ++j)
      /* End BUGZILLA_472 */
      {
        pBuffer[i] = LIN_RAW_RX_STACK(ifc)[j];
      } /* for (complete frame) */

      /*
       * advance stack start counter
       */
      LIN_RAW_RX_STACK_START(ifc) =
         (LIN_RAW_RX_STACK_START(ifc) + 1) % LIN_DIAG3_FIFO_SIZE_MAX;

      /*
       * adjust stack state
       */
      /*
       * one frame extracted, stack cannot be full
       */
      LIN_RAW_TP_FLAGS(ifc) &= (l_u8) ~LIN_RAW_RX_QUEUE_FULL;

      if (LIN_RAW_RX_STACK_START(ifc) == LIN_RAW_RX_STACK_END(ifc))
      {
         LIN_RAW_TP_FLAGS(ifc) |= (l_u8) LIN_RAW_RX_QUEUE_EMPTY;
      }

      retval = 1;

    } /* if (!stack empty) */

    return retval;

  } /* end ld_diag3RawRxFifoGet() */

  /*********************************************************************
   *
   * ld_diag3RawRxFifoStore()
   */
  /**
   * \brief management of raw Rx stack
   *
   * This function copies a frame to the Rx Fifo stack. The frame is
   * always accepted; if the stack was already full the oldest frame
   * is overwritten
   *
   * - internal function
   * - master and slave
   *
   * \param  ifc            handle of the interface
   * \param  pBuffer        buffer holding the frame to be copied
   *
   * \retval 1 always  (for uniform interface)
   *
   */
  STACK static void ld_diag3RawRxFifoStore(__OPTIONAL_IFC_DEFINITION_COMMA__
                                             const l_u8 *    pBuffer)
  {
    l_u8                 i;
    l_u8                 j;
    /* Begin BUGZILLA_472 */
    l_u8              temp = (l_u8) (LIN_RAW_RX_STACK_END(ifc) * LIN_DIAG_FRAME_LEN_MAX);
    /* End BUGZILLA_472 */

    /*
     * always copy frame to stack
     */
    /*
     * MISRA deviation, rule 12.10
     * Reason: Two variables control behaviour of for loop
     *         hence comma operator is required
     */
    /* Begin BUGZILLA_472 */
    for (i = 0, j = temp; i < LIN_DIAG_FRAME_LEN_MAX; ++i, ++j)
    /* End BUGZILLA_472 */
    {
      LIN_RAW_RX_STACK(ifc)[j] = pBuffer[i];
    } /* for (complete frame) */

    /*
     * advance stack end counter
     */
    LIN_RAW_RX_STACK_END(ifc) =
       (LIN_RAW_RX_STACK_END(ifc) + 1) % LIN_DIAG3_FIFO_SIZE_MAX;

    /*
     * the queue cannot be empty, and this was not an error
     */
    LIN_RAW_TP_FLAGS(ifc) &= (l_u8) ~(LIN_RAW_RX_QUEUE_EMPTY | LIN_RAW_RX_ERROR);

    if ((LIN_RAW_TP_FLAGS(ifc) & (l_u8) LIN_RAW_RX_QUEUE_FULL) != 0)
    {
      /*
       * the stack was already full, so we have overwritten
       * the oldest frame. We must adjust the start of the stack
       */
       LIN_RAW_RX_STACK_START(ifc) = LIN_RAW_RX_STACK_END(ifc);

      /*
       * stack remains full, so don't change state here
       */
    }
    else if (LIN_RAW_RX_STACK_START(ifc) == LIN_RAW_RX_STACK_END(ifc))
    {
      /*
       * now the stack is full
       */
      LIN_RAW_TP_FLAGS(ifc) |=  (l_u8) LIN_RAW_RX_QUEUE_FULL;

    }
    else
    {
       /*
        * nothing to do here
        */
       ;
    }
    return ;

  } /* end ld_diag3RawRxFifoStore() */

#endif /* LIN_INCLUDE_RAW_TP */

#if (defined(LIN_21) && defined(LIN_INCLUDE_COOKED_TP))
  /*********************************************************************
   */
  /** l_ifc_set_TP_status()
   *
   * \Master or Slave node handler
   *
   * This function will allow for diagnostic communication without frame
   * collisions on the cluster.
   * During diagnostic the broadcast NAD is normally not used.
   * If this will happen the slave node will process requests with broadcast
   * NAD (0x7F) in the same way as if it is the slave node's own NAD.
   * Note the difference between the broadcast NAD (0x7F) and functional NAD (0x7E).
   *
   * - internal function
   * - master or slave
   *
   * \param  ifc            the handle of the interface
   * \param  status         the status to set
   *                        this parameter can be one of the following values:
   *
   *********************************SLAVE STATUS************************************************
   *
   *  - LIN_STATUS_IDLE                   In this state the slave node is neither receiving 
   *                                      nor transmitting any messages in the cluster. 
   *                                      It is consequently available for any incoming request 
   *                                      from the master node. It shall not respond to slave 
   *                                      response frames.
   *
   *  - LIN_STATUS_RX_FUNCTIONAL_REQUEST  In this state a slave node is receiving a functional 
   *                                      transmission from the master node. The slave node shall 
   *                                      not respond to any slave response frames.
   *
   *  - LIN_STATUS_RX_PHYSICAL_REQUEST    In this state the slave node is receiving a transmission 
   *                                      from the master node. It is receiving and processing the 
   *                                      transport layer frames as received from the master node. 
   *                                      The slave node shall ignore any interleaved functional 
   *                                      addressed transmission from the master node.
   *
   *  - LIN_STATUS_TX_PHYSICAL_RESPONSE   In this state a slave node is currently still processing 
   *                                      the previously received request, is ready to transmit a 
   *                                      physical response or is actually transmitting the response 
   *                                      to the previously received request. A slave node shall not 
   *                                      receive nor process interleaved functional addressed 
   *                                      (NAD 0x7E) transmissions from the master node. Physical 
   *                                      transmissions shall be received and will make the slave
   *                                      node discard the current request data or response data. 
   *                                      If the request is addressed to the slave node the request 
   *                                      shall be received and processed.
   *
   *********************************MASTER STATUS***********************************************
   *
   *  - LIN_STATUS_IDLE                  In this state the master node is neither receiving nor 
   *                                     transmitting any transmission on the LIN cluster. It is 
   *                                     consequently available for any incoming transmission from 
   *                                     the back-bone bus (e.g. CAN).
   *
   *  - LIN_STATUS_TX_FUNCTIONAL_ACTIVE  In this state the master node is routing a functional 
   *                                     addressed request from the back-bone bus to the LIN cluster.
   *                                     This can only be a single frame (SF) according to restrictions 
   *                                     for the transport protocol on LIN (see Transport Layer Specification).
   *
   *  - LIN_STATUS_TX_PHYSICAL_ACTIVE    In this state the master node is currently routing data from 
   *                                     the back-bone bus to one slave node in the LIN cluster. The 
   *                                     master node is consequently occupied and cannot route any other 
   *                                     physical transmission from the back-bone bus to the LIN cluster. 
   *                                     Also no response from a slave node can be routed to the back-bone bus.
   *
   *  - LIN_STATUS_RX_PHYSICAL_ACTIVE    In this state the master node is routing a transmission from a 
   *                                     slave node to the back-bone bus. It is possible to transmit functional 
   *                                     addressed requests to the LIN cluster but cannot handle further 
   *                                     physical transmissions to a slave node.
   *
   *  - LIN_STATUS_INTERLEAVED_TX_FUNC_DURING_TX_PHYS    This is the state in which the master node routes a 
   *                                     functional addressed request from the back-bone bus to the LIN cluster 
   *                                     while a transmission to a slave node is currently active. Functional 
   *                                     addressed Single Frames (SF) can be transmitted, but shall be ignored 
   *                                     by the active slave node while receiving a physically addressed transmission.
   *
   *  - LIN_STATUS_INTERLEAVED_RX_FUNC_DURING_RX_PHYS   In this state a slave node is currently still processing 
   *                                     the previously received request, is ready to transmit a 
   *                                     physical response or is actually transmitting the response 
   *                                     to the previously received request. A slave node shall not 
   *                                     receive nor process interleaved functional addressed 
   *                                     (NAD 0x7E) transmissions from the master node. Physical 
   *                                     transmissions shall be received and will make the slave
   *                                     node discard the current request data or response data. 
   *                                     If the request is addressed to the slave node the request 
   *                                     shall be received and processed.
   *
   *************************************************************************************************
   * \retval none
   *
   */
  STACK void l_ifc_set_TP_status (__OPTIONAL_IFC_DEFINITION_COMMA__ l_ifcTPState_t status)
  {
    switch (status)
    {
      #ifdef LIN_MASTER_NODE
        case LIN_STATUS_IDLE:
      	  if ((IFCCTRL_STATUSTPWORD(ifc) != LIN_IFC_STATUS_INTERLEAVED_TX_FUNC_DURING_TX_PHYS) &&
      		    (IFCCTRL_STATUSTPWORD(ifc) != LIN_IFC_STATUS_INTERLEAVED_RX_FUNC_DURING_RX_PHYS))
          {
      	    IFCCTRL_STATUSTPWORD(ifc) = LIN_IFC_STATUS_IDLE;
      	  }
      	  break;
        case LIN_STATUS_TX_FUNCTIONAL_ACTIVE:
      	  if (IFCCTRL_STATUSTPWORD(ifc) == LIN_IFC_STATUS_IDLE)
          {
      	    IFCCTRL_STATUSTPWORD(ifc) = LIN_IFC_STATUS_TX_FUNCTIONAL_ACTIVE;
      	  }
          break;
        case LIN_STATUS_TX_PHYSICAL_ACTIVE:
      	  if ((IFCCTRL_STATUSTPWORD(ifc) == LIN_IFC_STATUS_IDLE) ||
      		  (IFCCTRL_STATUSTPWORD(ifc) == LIN_IFC_STATUS_INTERLEAVED_TX_FUNC_DURING_TX_PHYS))
          {
      	    IFCCTRL_STATUSTPWORD(ifc) = LIN_IFC_STATUS_TX_PHYSICAL_ACTIVE;
      	  }
      	  break;
        case LIN_STATUS_RX_PHYSICAL_ACTIVE:
      	  if ((IFCCTRL_STATUSTPWORD(ifc) == LIN_IFC_STATUS_IDLE) ||
      		    (IFCCTRL_STATUSTPWORD(ifc) == LIN_IFC_STATUS_INTERLEAVED_RX_FUNC_DURING_RX_PHYS) ||
      		    (IFCCTRL_STATUSTPWORD(ifc) == LIN_IFC_STATUS_TX_PHYSICAL_ACTIVE))
          {
      	    IFCCTRL_STATUSTPWORD(ifc) = LIN_IFC_STATUS_RX_PHYSICAL_ACTIVE;
      	  }
      	  break;
        case LIN_STATUS_INTERLEAVED_TX_FUNC_DURING_TX_PHYS:
      	  if (IFCCTRL_STATUSTPWORD(ifc) == LIN_IFC_STATUS_TX_PHYSICAL_ACTIVE)
          {
      	    IFCCTRL_STATUSTPWORD(ifc) = LIN_IFC_STATUS_INTERLEAVED_TX_FUNC_DURING_TX_PHYS;
      	  }
      	  break;
        case LIN_STATUS_INTERLEAVED_RX_FUNC_DURING_RX_PHYS:
      	  if (IFCCTRL_STATUSTPWORD(ifc) == LIN_IFC_STATUS_RX_PHYSICAL_ACTIVE)
          {
      	    IFCCTRL_STATUSTPWORD(ifc) = LIN_IFC_STATUS_INTERLEAVED_RX_FUNC_DURING_RX_PHYS;
      	  }
      	  break;
      #else
        case LIN_STATUS_IDLE:
      	  IFCCTRL_STATUSTPWORD(ifc) = LIN_IFC_STATUS_IDLE;
      	  break;
        case LIN_STATUS_RX_FUNCTIONAL_REQUEST:
      	  if (IFCCTRL_STATUSTPWORD(ifc) == LIN_IFC_STATUS_IDLE)
          {
      	    IFCCTRL_STATUSTPWORD(ifc) = LIN_IFC_STATUS_RX_FUNCTIONAL_REQUEST;
      	  }
          break;
        case LIN_STATUS_RX_PHYSICAL_REQUEST:
      	  if (IFCCTRL_STATUSTPWORD(ifc) != LIN_IFC_STATUS_RX_FUNCTIONAL_REQUEST)
          {
      	    IFCCTRL_STATUSTPWORD(ifc) = LIN_IFC_STATUS_RX_PHYSICAL_REQUEST;
      	  }
      	  break;
        case LIN_STATUS_TX_PHYSICAL_RESPONSE:
      	  if (IFCCTRL_STATUSTPWORD(ifc) == LIN_IFC_STATUS_RX_PHYSICAL_REQUEST)
          {
      	    IFCCTRL_STATUSTPWORD(ifc) = LIN_IFC_STATUS_TX_PHYSICAL_RESPONSE;
      	  }
      	  break;
      #endif
    }

    return ;
  } /* end l_ifc_set_TP_status() */
#endif /* defined(LIN_21) && defined(LIN_INCLUDE_COOKED_TP) */
/*
 * eof lin_diag.c
 */
