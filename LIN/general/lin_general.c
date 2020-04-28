/*********************************************************************
 *
 * Project:      LIN 2.1 driver suite
 *
 * File name:    lin_general.c
 *
 * Copyright:    (c) STMicroelectronics
 *
 * Author:       Giuseppe Stefano Fazio
 *
 * Description:  General functionality of the driver
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
 * 20071218  v1.1   gsf  - BUGZILLA_378: Wrong driver implementation
 *                                       for Break and Del readback
 * 20090420  v1.2   gsf  - BUGZILLA_472: LIN driver does not compile if
 *                         Cosmic "strict" option is enabled.
 *                         BUGZILLA_478: “Mute Mode” must be disabled 
 *                         for a LINSCI Slave node, when this is in 
 *                         Sleep State mode.
 *                         BUGZILLA_540: LIN 2.1 / N_Cr_LIN timeout
 *                         and N_As_LIN timeout management missing.
 *
 *********************************************************************/

/** \file lin_general.c
 */

#define LIN_GENERAL_C

#define LIN_GENERAL_C_MAJOR_VERSION  1
#define LIN_GENERAL_C_MINOR_VERSION  32
#define LIN_GENERAL_C_PATCH_VERSION  2

#define LIN_GENERAL_C_STM

/*********************************************************************
 *
 * Note: The LIN standard defines a set of API functions as well as
 *       some types to be provided. The layout of the names is
 *       l_xxx_yyy
 *       For all functions and types to be used by the application
 *       software the code follows this specification, whereas
 *       internal types and interfaces use
 *       l_xxxYyy
 *       This has been done to clearly distinguish between internal
 *       and external interfaces
 *
 *********************************************************************/

/*********************************************************************
 * includes
 *********************************************************************/

#include <limits.h>

#include "lin_types.h"
#include "lin_def.h"
#include "lin_def_gen.h"
#include "lin_def_arch_include.h"
#include "lin_arch_include.h"
#include "lin_timer.h"
#include "lin_general.h"
#include "lin_cfg_types.h"
#include "lin_cfg.h"
#include "lin_diag.h"

#include "lin.h"

#if defined(LIN_MASTER_NODE)
  #include "lin_master.h"
  #include "lin_diag_master.h"
#elif defined(LIN_SLAVE_NODE)
  #include "lin_slave.h"
  #include "lin_diag_slave.h"
#else
  #error "No valid node defined!"
#endif

#include "lin_version_control.h"
#include "lin_test_hardware.h" 


#if (CHAR_BIT != 8)
  #error "driver designed for machines with 8 bit chars"
#endif

#if ( (1ul * LIN_BITS_PER_FRAME * LIN_FRAME_TIME_MULTIPLIER) > 0xFFFFul )
  #error "Overflow - choose smaller values for LIN_BREAK_DURATION_BITS or LIN_FRAME_TIME_MULTIPLIER."
#endif

/*********************************************************************
 * local macros
 *********************************************************************/

#if (defined(LIN_BAUDRATE_DETECT) && defined(LIN_PROTOCOL_SWITCH))

  #define CREATE_LIN_COM_HOOKS_STRUCT(IFC) \
       __CAT(l_ifc_initArch_,IFC),         \
       __CAT(l_ifc_connectArch_,IFC),      \
       __CAT(l_ifc_disconnectArch_,IFC),   \
       __CAT(l_txCharArch_,IFC),           \
       __CAT(l_ifc_rx_,IFC),               \
       __CAT(l_ifc_tx_,IFC),               \
       __CAT(l_changeBaudrateArch_,IFC),   \
       __CAT(l_baudrateCallback_,IFC),     \
       __CAT(l_protocolCallback_,IFC)

#elif defined(LIN_BAUDRATE_DETECT)

  #define CREATE_LIN_COM_HOOKS_STRUCT(IFC) \
       __CAT(l_ifc_initArch_,IFC),         \
       __CAT(l_ifc_connectArch_,IFC),      \
       __CAT(l_ifc_disconnectArch_,IFC),   \
       __CAT(l_txCharArch_,IFC),           \
       __CAT(l_ifc_rx_,IFC),               \
       __CAT(l_ifc_tx_,IFC),               \
       __CAT(l_changeBaudrateArch_,IFC),   \
       __CAT(l_baudrateCallback_,IFC)

#elif defined(LIN_PROTOCOL_SWITCH)

  #define CREATE_LIN_COM_HOOKS_STRUCT(IFC) \
       __CAT(l_ifc_initArch_,IFC),         \
       __CAT(l_ifc_connectArch_,IFC),      \
       __CAT(l_ifc_disconnectArch_,IFC),   \
       __CAT(l_txCharArch_,IFC),           \
       __CAT(l_ifc_rx_,IFC),               \
       __CAT(l_ifc_tx_,IFC),               \
       __CAT(l_changeBaudrateArch_,IFC),   \
       __CAT(l_protocolCallback_,IFC)

#else

  #define CREATE_LIN_COM_HOOKS_STRUCT(IFC) \
       __CAT(l_ifc_initArch_,IFC),         \
       __CAT(l_ifc_connectArch_,IFC),      \
       __CAT(l_ifc_disconnectArch_,IFC),   \
       __CAT(l_txCharArch_,IFC),           \
       __CAT(l_ifc_rx_,IFC),               \
       __CAT(l_ifc_tx_,IFC),               \
       __CAT(l_changeBaudrateArch_,IFC)

#endif


/*********************************************************************
 * global consts and variables
 *********************************************************************/

  LIN_ZERO_PAGE11 l_u8    l_pTxFlagData[LIN_TRANSMIT_FLAG_DATA_SIZE];
  LIN_ZERO_PAGE12 l_u8    l_pRxFlagData[LIN_TRANSMIT_FLAG_DATA_SIZE];

  LIN_ZERO_PAGE13 l_u8    l_pChangedFlagData[LIN_CHANGED_FLAG_DATA_SIZE];

#ifdef __MORE_IFCS__
   LIN_ZERO_PAGE7 l_u16       l_ifcNormalBaudrate[LIN_IFC_NR_MAX];
   l_u8                       l_ifcFrameTimerValue[LIN_IFC_NR_MAX];
   LIN_ZERO_PAGE3 l_u16       l_ifcStatusWord[LIN_IFC_NR_MAX];
	
	#if (defined(LIN_INCLUDE_COOKED_TP) && defined(LIN_21))
		LIN_ZERO_PAGE3 l_u16      l_ifcStatusTPWord[LIN_IFC_NR_MAX];
	#endif /* defined(LIN_INCLUDE_COOKED_TP) && defined(LIN_21) */
	
	LIN_ZERO_PAGE1 l_u8        l_ifcState[LIN_IFC_NR_MAX];
	LIN_ZERO_PAGE4 l_u8        l_ifcFlags[LIN_IFC_NR_MAX];
	l_u8                       l_ifcDiagFlags[LIN_IFC_NR_MAX];
	LIN_ZERO_PAGE2 l_u8        l_transferBytesSent[LIN_IFC_NR_MAX];
	LIN_ZERO_PAGE5 l_u8        l_transferBytesToSend[LIN_IFC_NR_MAX];
	l_u8                       l_transferCharSent[LIN_IFC_NR_MAX];
	LIN_ZERO_PAGE10 l_u8       l_transferBuffer[LIN_IFC_NR_MAX][10];
	l_u8                       l_checksum[LIN_IFC_NR_MAX];
	
	#ifdef LIN_MASTER_NODE
		l_u16                    l_ifcBreakBaudrate[LIN_IFC_NR_MAX];
		LIN_ZERO_PAGE8 l_u8      l_transferState[LIN_IFC_NR_MAX];
		l_u8                     l_transferRetries[LIN_IFC_NR_MAX];
		l_u8                     l_ifcSerialError[LIN_IFC_NR_MAX];
		l_u16                    l_transferFrame[LIN_IFC_NR_MAX];
		LIN_ZERO_PAGE9 l_u16     l_ifcSchedule[LIN_IFC_NR_MAX];
		LIN_ZERO_PAGE6 l_u8      l_ifcEntry[LIN_IFC_NR_MAX];
		
		#ifdef LIN_21
			LIN_ZERO_PAGE10 l_u16    l_ifcScheduleAfterCollision[LIN_IFC_NR_MAX];
			LIN_ZERO_PAGE11 l_u8     l_ifcEntryAfterCollision[LIN_IFC_NR_MAX];
		#endif /* LIN_21 */
		
		l_u8                     l_ifcDelay[LIN_IFC_NR_MAX];
		l_u8                     l_ifcGotoSleepReq[LIN_IFC_NR_MAX];
		l_u16                    l_ifcGotoSleepSchedule[LIN_IFC_NR_MAX];
		l_u8                     l_ifcGotoSleepEntry[LIN_IFC_NR_MAX];

	#else

		l_u8                     l_transferState[LIN_IFC_NR_MAX];
		LIN_ZERO_PAGE8 l_u8      l_transferRetries[LIN_IFC_NR_MAX];
		LIN_ZERO_PAGE9 l_u8      l_ifcSerialError[LIN_IFC_NR_MAX];
		LIN_ZERO_PAGE6 l_u16     l_transferFrame[LIN_IFC_NR_MAX];
		
		#ifdef LIN_BAUDRATE_DETECT
			l_u8                   l_ifcBaudrateFlags[LIN_IFC_NR_MAX];
		#endif /* LIN_BAUDRATE_DETECT */
	#endif

#else
	LIN_ZERO_PAGE7 l_u16       l_ifcNormalBaudrate;
	l_u8                       l_ifcFrameTimerValue;
	LIN_ZERO_PAGE3 l_u16       l_ifcStatusWord;
	
	#if (defined(LIN_INCLUDE_COOKED_TP) && defined(LIN_21))
		LIN_ZERO_PAGE3 l_u16      l_ifcStatusTPWord;
	#endif /* defined(LIN_INCLUDE_COOKED_TP) && defined(LIN_21) */
	
	LIN_ZERO_PAGE1 l_u8        l_ifcState;
	LIN_ZERO_PAGE4 l_u8        l_ifcFlags;
	LIN_ZERO_PAGE2 l_u8        l_transferBytesSent;
	LIN_ZERO_PAGE5 l_u8        l_transferBytesToSend;
	LIN_ZERO_PAGE10 l_u8       l_transferBuffer[10];

#ifdef __COSMIC__
@tiny l_u8                       l_ifcDiagFlags;
@tiny l_u8                       l_transferCharSent;
@tiny l_u8                       l_checksum;

#elif __RAISONANCE__
 l_u8                       l_ifcDiagFlags;
 l_u8                       l_transferCharSent;
 l_u8                       l_checksum;

#elif __IAR__
  l_u8                       l_ifcDiagFlags;
  l_u8                       l_transferCharSent;
  l_u8                       l_checksum;
#endif
	
	#ifdef LIN_MASTER_NODE
		LIN_ZERO_PAGE8 l_u8      l_transferState;
		LIN_ZERO_PAGE9 l_u16     l_ifcSchedule;
		
#ifdef __COSMIC__
@tiny l_u16                        l_ifcBreakBaudrate;
@tiny l_u8                         l_transferRetries;
@tiny l_u8                         l_ifcSerialError;
@tiny l_u8                        l_ifcEntry;
@tiny l_u16                       l_transferFrame;

#elif __RAISONANCE__
 l_u16                        l_ifcBreakBaudrate;
 l_u8                         l_transferRetries;
 l_u8                         l_ifcSerialError;
 l_u8                        l_ifcEntry;
 l_u16                       l_transferFrame;

#elif __IAR__
 l_u16                        l_ifcBreakBaudrate;
 l_u8                         l_transferRetries;
 l_u8                         l_ifcSerialError;
 l_u8                        l_ifcEntry;
 l_u16                       l_transferFrame;

#endif
		#ifdef LIN_21
			l_u16    l_ifcScheduleAfterCollision;
			LIN_ZERO_PAGE11 l_u8     l_ifcEntryAfterCollision;
		#endif /* LIN_21 */
		

#ifdef __COSMIC__
@tiny l_u16                        l_ifcGotoSleepSchedule;
@tiny l_u8                         l_ifcDelay;
@tiny l_u8                         l_ifcGotoSleepReq;
@tiny l_u8                        l_ifcGotoSleepEntry;


#elif __RAISONANCE__
  l_u16                         l_ifcGotoSleepSchedule;
  l_u8                         l_ifcDelay;
  l_u8                         l_ifcGotoSleepReq;
  l_u8                        l_ifcGotoSleepEntry;


#elif __IAR__
  l_u16                        l_ifcGotoSleepSchedule;
  l_u8                         l_ifcDelay;
  l_u8                         l_ifcGotoSleepReq;
  l_u8                        l_ifcGotoSleepEntry;


#endif
	#else
		LIN_ZERO_PAGE8 l_u8      l_transferRetries;
		LIN_ZERO_PAGE9 l_u8      l_ifcSerialError;
		
#ifdef __COSMIC__
@tiny l_u16                        l_transferFrame;		
@tiny l_u8                         l_transferState;



#elif __RAISONANCE__
  l_u16                         l_transferFrame;		
  l_u8                        l_transferState;

#elif __IAR__
  l_u16                        l_transferFrame;
  l_u8                         l_transferState;

#endif

		#ifdef LIN_BAUDRATE_DETECT

#ifdef __COSMIC__	
@tiny l_u8                           l_ifcBaudrateFlags;



#elif __RAISONANCE__		
 l_u8                        l_ifcBaudrateFlags;

#elif __IAR__
 l_u8                           l_ifcBaudrateFlags;

#endif
		#endif /* LIN_BAUDRATE_DETECT */
	#endif
#endif

/* Initialization of the interface control structures */

#ifdef __MORE_IFCS__

  const IFC_CTRL_HOOKS_TYPE ARRAY_IFCCtrlHooks[] =
  {
    #ifdef SCI0_USED
      {
        CREATE_LIN_COM_HOOKS_STRUCT(SCI0)
      } ,
    #endif

    #ifdef SCI1_USED
      {
        CREATE_LIN_COM_HOOKS_STRUCT(SCI1)
      } ,
    #endif

    #ifdef SCI2_USED
      {
        CREATE_LIN_COM_HOOKS_STRUCT(SCI2)
      } ,
    #endif

    #ifdef SCI3_USED
      {
        CREATE_LIN_COM_HOOKS_STRUCT(SCI3)
      } ,
    #endif

    #ifdef SCI4_USED
      {
        CREATE_LIN_COM_HOOKS_STRUCT(SCI4)
      } ,
    #endif

    #ifdef SCI5_USED
      {
        CREATE_LIN_COM_HOOKS_STRUCT(SCI5)
      } ,
    #endif

    #ifdef SCI6_USED
      {
        CREATE_LIN_COM_HOOKS_STRUCT(SCI6)
      } ,
    #endif

    #ifdef SCI7_USED
      {
        CREATE_LIN_COM_HOOKS_STRUCT(SCI7)
      } ,
    #endif

    #ifdef SCI8_USED
      {
        CREATE_LIN_COM_HOOKS_STRUCT(SCI8)
      } ,
    #endif

    #ifdef SCI9_USED
      {
        CREATE_LIN_COM_HOOKS_STRUCT(SCI9)
      } ,
    #endif

  };

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

/*********************************************************************
 *
 * implementation
 *
 *********************************************************************/

/*********************************************************************
 *
 * system handling
 *
 *********************************************************************/

/*********************************************************************
 */
/** l_sys_init()
 *
 * \brief initialization function
 *
 * This function initializes the LIN core. It must be called by the
 * application prior to any other LIN API call.
 *
 * - standard API
 * - master and slave
 *
 * \param   none
 *
 * \retval  0 if the initialization succeeded (currently always)
 *
 */
STACK l_bool l_sys_init (void) REENTRANT
{
  l_u8    ifc;
  l_irqmask irqMask;

  irqMask = l_sys_irq_disable();
  /*
   * we will initialize the baudrate for all
   * interfaces here, since it is not quite clear whether an
   * interface may be operated at a baudrate different from the one
   * defined in the LDF
   */
  for (ifc = 0; ifc < LIN_IFC_NR_MAX; ++ifc)
  {
    IFCCTRL_NORMALBAUDRATE(ifc) = l_speed[ifc];
  } /* for (all interfaces) */

  l_TimerInit(); /* Initialize timer */

  l_sys_irq_restore(irqMask);

  return (l_bool) 0;   /* Always success */

} /* end l_sys_init() */

/*********************************************************************
 *
 * frame handling
 *
 *********************************************************************/
#ifndef LIN_MASTER_NODE
  #ifdef LIN_INCLUDE_PID_PARITY_CHECK

    /*********************************************************************
     */
    /** l_makeProtId()
     *
     * \brief internal helper function
     *
     * For a given identifier, this function returns the protected
     * identifier, generating two parity bits
     *
     * - internal function
     * - slave node only
     * 
     * \param identfifier    the identifier to generate the protected id for
     *
     * \return the protected identifier
     *
     */
    STACK l_u8 l_makeProtId(l_u8 identifier)
    {
      l_u8 parBit1;
      l_u8 parBit2;
      l_u8 IdentifierBits;
      
      IdentifierBits   = identifier;
      parBit1          = IdentifierBits; /* Bit 0 */
      IdentifierBits >>= 1;
      parBit1         ^= IdentifierBits; /* Bit 1 */
      parBit2          = IdentifierBits;
      IdentifierBits >>= 1;
      parBit1         ^= IdentifierBits; /* Bit 2 */
      IdentifierBits >>= 1;
      parBit2         ^= IdentifierBits; /* Bit 3 */
      IdentifierBits >>= 1;
      parBit1         ^= IdentifierBits; /* Bit 4 */
      parBit2         ^= IdentifierBits;
      IdentifierBits >>= 1;
      parBit2         ^= IdentifierBits; /* Bit 5 */
      
      parBit1 &= (l_u8) 1;
      /* Begin BUGZILLA_472 */
      parBit2  = ((l_u8) ((l_u8) ((l_u8) ( ~ parBit2)) & (l_u8) 1));
      
      identifier |=
         ((l_u8) ((l_u8) ((l_u8)(parBit1 << LIN_PAR_BIT1_POS)) | ((l_u8)(parBit2 << LIN_PAR_BIT2_POS))));
      /* End BUGZILLA_472 */      
      return identifier;

    } /* end l_MakeProtId() */
  #endif /* LIN_INCLUDE_PID_PARITY_CHECK */

  /********************************************************************
   */
  /** l_getFrameIndex()
   *
   * \brief internal helper function (slave node only)
   *
   * This function returns the frame index for a given frame identifier
   *
   * - internal function
   * - slave node only
   *
   * \param id             identifier of the frame (protected or not)
   *
   * \return the index of the frame
   *
   */
  STACK l_frame_handle l_getFrameIndex(__OPTIONAL_IFC_DEFINITION_COMMA__ l_u8 id)
  {
    l_u8 i;
    l_frame_handle retval = LIN_INVALID_FRAME_INDEX;

    for (i = 0; i < LIN_FRAME_NR_MAX; ++i)
    {
      #ifdef __MORE_IFCS__
        if ((l_pFrameId[i] == id)
            &&
            (l_pIfcIndex == ifc))
        {
           retval = (l_frame_handle) i;
        }
      #else
        if (l_pFrameId[i] == id)
        {
           retval = (l_frame_handle) i;
        }
      #endif

    } /* for (all frames) */

    return retval;

  } /* l_getFrameIndex() */
#endif

/*********************************************************************
 *
 * interface related stuff
 *
 *********************************************************************/

/*********************************************************************
 */
/** l_ifc_init()
 *
 * \brief initialization function
 *
 * This function initializes the specified interface
 *
 * - standard API
 * - master and slave
 *
 * \param  ifc            interface handle
 *
 * \retval none (in LIN 2.0 or LIN 1.3)
 * \retval  0 if the initialization succeeded (currently always) (bool in LIN 2.1)
 *
 */
#ifdef LIN_21
  STACK l_bool l_ifc_init (l_ifc_handle ifc) REENTRANT
#else
  STACK void l_ifc_init (l_ifc_handle ifc) REENTRANT
#endif /* LIN_21 */
{
  l_irqmask irqMask;


  __CHECK_INTERFACE_VOID__(ifc);

  irqMask = l_sys_irq_disable();


  IFCCTRL_STATUSWORD(ifc)    = 0;
  IFCCTRL_STATE(ifc)         = 0;
  IFCCTRL_FLAGS(ifc)         = 0;
  IFCCTRL_SERIALERROR(ifc)   = 0;
  IFCCTRL_DIAGFLAGS(ifc)     = 0;

  l_initTransferCtrl(__OPTIONAL_IFC_PARAMETER__);

  #ifdef LIN_MASTER_NODE

    IFCCTRL_ENTRY(ifc)            = 0;
    IFCCTRL_GOTOSLEEPREQ(ifc)     = 0;
    IFCCTRL_GOTOSLEEPENTRY(ifc)   = 0;

  #else
    #ifdef LIN_BAUDRATE_DETECT

      IFCCTRL_BAUDRATEFLAGS(ifc)  = 0;

    #endif /* LIN_BAUDRATE_DETECT */
  #endif /* LIN_MASTER_NODE */

  IFCCTRL_FRAMETIMERVALUE(ifc)
     = (l_u8) (((l_u16) ((1ul * LIN_BITS_PER_FRAME * LIN_FRAME_TIME_MULTIPLIER)
                         / (IFCCTRL_NORMALBAUDRATE(ifc) / 10)) + (l_u16) 1));

  #ifdef LIN_MASTER_NODE
    IFCCTRL_SCHEDULE(ifc)          = L_NULL_SCHEDULE;
    IFCCTRL_DELAY(ifc)             = LIN_DELAY_INIT;
    IFCCTRL_GOTOSLEEPSCHEDULE(ifc) = LIN_SCHED_TAB_NR_MAX;
    IFCCTRL_FLAGS(ifc)             = LIN_IFC_INIT_FLAG;
  #endif

  (void)INIT_ARCH(ifc);

  #ifdef LIN_INCLUDE_DIAGNOSTICS
    #ifdef LIN_INCLUDE_RAW_TP
      LIN_RAW_TP_FLAGS(ifc) |=
         (l_u8) (LIN_RAW_TX_QUEUE_EMPTY | LIN_RAW_RX_QUEUE_EMPTY);
    #endif
  
    #ifdef LIN_SLAVE_NODE   
      LIN_DIAG_NAD(ifc) = l_initialNad;
    #endif
  #endif /* LIN_INCLUDE_DIAGNOSTICS */

  #ifdef LIN_21
    /* What was done in LIN 2.0 calling the l_ifc_connect() API, must now be done here */
    /* since l_ifc_connect() and l_ifc_disconnect() API are no more present in LIN 2.1 */

    /*
     * call provided hook and check return value
     */
    if (!(CONNECT_ARCH(ifc) ) )
    {
      IFCCTRL_FLAGS(ifc) |= (l_u8) LIN_IFC_CONNECT_FLAG;
      CHANGEBAUDRATE_ARCH(ifc, IFCCTRL_NORMALBAUDRATE(ifc));
    } /* if (successful connect) */
    #if (defined(LIN_SLAVE_NODE) && defined(LIN_START_BUSSLEEP_TIMER_ON_CONNECT))
      l_TimerStart(__OPTIONAL_TIMER_IFC_PARAMETER_COMMA__
                   LIN_BUSSLEEP_SUBCHANNEL,
                   LIN_BUSSLEEP_TIMEOUT_VAL(__MANDATORY_IFC_PARAMETER__) );
    #endif

    #if (defined(LIN_MASTER_NODE) && defined(LIN_SEND_WAKEUP_SIG_ON_CONNECT))
      LIN_RESET_LBDL_BIT();
      IFCCTRL_STATE(ifc) = LIN_STATE_BUSSLEEP;
     /* Begin BUGZILLA_378 */
      LIN_RIE();
     /* End BUGZILLA_378 */
      l_ifc_wake_up(__MANDATORY_IFC_PARAMETER__);
    #endif

    l_sys_irq_restore(irqMask);

    return (l_bool) 0;   /* Always success */
  #else

    l_sys_irq_restore(irqMask);

    return ;
  #endif /* LIN_21 */

} /* end l_ifc_init() */

/*********************************************************************
 */
/** l_initTransferCtrl()
 *
 * \brief initialization function
 *
 * This function initializes the transfer control structure of
 * the give interface. Should be called before initiating a new
 * transfer over the interface
 *
 * - internal function
 * - master and slave
 *
 * \param  ifc            handle of the interface
 *
 * \retval none
 *
 */
STACK void l_initTransferCtrl (__OPTIONAL_IFC_DEFINITION__)
{
	l_u8 i = 10;
  TRANSFERCTRL_BYTESSENT(ifc)    = 0;
  TRANSFERCTRL_BYTESTOSEND(ifc)  = 0;
  TRANSFERCTRL_CHARSENT(ifc)     = 0;
  TRANSFERCTRL_STATE(ifc)        = 0;
  TRANSFERCTRL_RETRIES(ifc)      = 0;
  TRANSFERCTRL_FRAME(ifc)        = 0;
  TRANSFERCTRL_CHECKSUM(ifc)     = 0;

	do{                                      
	TRANSFERCTRL_BUFFER(ifc)[--i] = 0xFF;
  }while(i);
	
  TRANSFERCTRL_FRAME(ifc) = LIN_FRAME_NR_MAX;

  return ;

} /* end l_initTransferCtrl() */

#ifndef LIN_21
/*********************************************************************
 */
/** l_ifc_connect()
 *
 * \brief interface connection function
 *
 * This function connects the interface to the cluster and initiates
 * the transmission on the interface
 *
 * - standard API
 * - master and slave
 *
 * \param  ifc            interface handle
 *
 * \retval 0 if operation was successful
 * \retval 1 in error case
 *
 */
STACK l_bool l_ifc_connect(l_ifc_handle ifc) REENTRANT
{
  l_irqmask irqMask;


  __CHECK_INTERFACE_VALUE__(ifc, (l_bool)1);

  irqMask = l_sys_irq_disable();

  /*
   * call provided hook and check return value
   */
  if (!(CONNECT_ARCH(ifc) ) )
  {
    IFCCTRL_FLAGS(ifc) |= (l_u8) LIN_IFC_CONNECT_FLAG;
    CHANGEBAUDRATE_ARCH(ifc, IFCCTRL_NORMALBAUDRATE(ifc));
  } /* if (successful connect) */
  #if (defined(LIN_SLAVE_NODE) && defined(LIN_START_BUSSLEEP_TIMER_ON_CONNECT))
    l_TimerStart(__OPTIONAL_TIMER_IFC_PARAMETER_COMMA__
                 LIN_BUSSLEEP_SUBCHANNEL,
                 LIN_BUSSLEEP_TIMEOUT_VAL(__MANDATORY_IFC_PARAMETER__) );
  #endif

  #if (defined(LIN_MASTER_NODE) && defined(LIN_SEND_WAKEUP_SIG_ON_CONNECT))
    LIN_RESET_LBDL_BIT();
    IFCCTRL_STATE(ifc) = LIN_STATE_BUSSLEEP;
   /* Begin BUGZILLA_378 */
    LIN_RIE();
   /* End BUGZILLA_378 */
    l_ifc_wake_up(__MANDATORY_IFC_PARAMETER__);
  #endif

  l_sys_irq_restore(irqMask);

  return (l_bool)0;

} /* end l_ifc_connect() */

/*********************************************************************
 */
/** l_ifc_disconnect()
 *
 * \brief interface disconnect function
 *
 * This function disconnects the interface from the cluster and stops
 * transmission on the interface
 *
 * - standard API
 * - master and slave
 *
 * \param  ifc            interface handle
 *
 * \retval 0 if operation was successful
 * \retval 1 in error case
 *
 */
STACK l_bool l_ifc_disconnect (l_ifc_handle ifc) REENTRANT
{
  l_irqmask irqMask;


  __CHECK_INTERFACE_VALUE__(ifc, (l_bool)1);

  irqMask = l_sys_irq_disable();

  /*
   * call provided hook and check return value
   */
  if (!(DISCONNECT_ARCH(ifc)))
  {
    IFCCTRL_FLAGS(ifc) &= (l_u8) ~LIN_IFC_CONNECT_FLAG;
  } /* if (successful connect) */

  l_terminateTransfer(__OPTIONAL_IFC_PARAMETER__);

  l_TimerStop (__OPTIONAL_TIMER_IFC_PARAMETER__);

  l_sys_irq_restore(irqMask);

  return (l_bool)0;

} /* end l_ifc_disconnect() */
#endif /* !LIN_21 */

/*********************************************************************
 */
/** l_ifc_ioctl()
 *
 * \brief interface control function
 *
 * ioctl functionality for the interface
 * For supported functionality see the user's guide
 *
 * - standard API
 * - master and slave
 *
 * \param ifc            the interface handle
 * \param operation      the operation requested
 * \param pParams        pointer to the parameter block
 *
 * \retval l_u16, interpretation depends on the operation requested
 *
 */
STACK l_u16 l_ifc_ioctl (l_ifc_handle ifc, l_ioctl_op operation, void *pParams) REENTRANT
{
  #if (defined(LIN_SLAVE_NODE) && !defined(LIN_13))
    l_frameMessageId_t *        pData;
    l_u16                       i;
  #endif

  l_u16     retval = 0;
  l_irqmask irqMask;


  __CHECK_INTERFACE_VALUE__(ifc, (l_u16) 0);

  irqMask = l_sys_irq_disable();

  switch (operation)
  {
    case LIN_IOCTL_DRIVER_STATE:
      retval = (l_u16) (IFCCTRL_STATE(ifc) & 0xFF);
      #ifdef LIN_MASTER_NODE
        if ((retval != LIN_STATE_IDLE)
            &&
            (retval != LIN_STATE_BUSSLEEP))
        {
          retval +=
             ((l_u16) TRANSFERCTRL_BUFFER(ifc)[0]) << CHAR_BIT;
        }
        else
        {
          retval |= 0xFF00U;
        }
      #else
        if ((retval == LIN_STATE_SEND_DATA)
            ||
            (retval == LIN_STATE_RECEIVE_DATA))
        {
          retval = (l_u16) (l_pFrameId[TRANSFERCTRL_FRAME(ifc)]) << CHAR_BIT;
        }
        else
        {
          retval |= 0xFF00U;
        }

      #endif
      break;

      #if (defined(LIN_SLAVE_NODE) && !defined(LIN_13))
        case LIN_IOCTL_READ_FRAME_ID:
          pData = (l_frameMessageId_t *) pParams;
          retval = 1;

          for (i = 0; i < LIN_FRAME_NR_MAX; ++i)
          {
            if (l_pMessageId[i]
                ==
                pData->messageId)
            {
              pData->frameId     = l_pFrameId[i];
              pData->frameIndex  = (l_u8) i;
              retval = 0;
              break;
            }
          }

          break;

        case LIN_IOCTL_READ_MESSAGE_ID:
          pData = (l_frameMessageId_t *) pParams;
          retval = 1;

          for (i = 0; i < LIN_FRAME_NR_MAX; ++i)
          {
            if (l_pFrameId[i] ==  pData->frameId)
            {
              pData->messageId   = l_pMessageId[i];
              pData->frameIndex  = (l_u8) i;
              retval = 0;
              break;
            }
          }
          break;

        case LIN_IOCTL_READ_FRAME_ID_BY_INDEX:
          pData = (l_frameMessageId_t *) pParams;

          if (pData->frameIndex < LIN_FRAME_NR_MAX)
          {
            pData->messageId  = l_pMessageId[pData->frameIndex];
            pData->frameId    = l_pFrameId[pData->frameIndex];
          }
          else
          {
            retval = 1;
          }
          break;

        case LIN_IOCTL_SET_FRAME_ID:
          pData = (l_frameMessageId_t *) pParams;

          if (!l_changeFrameId(pData->messageId, pData->frameId))
          {
            retval = 1;
          }
          break;

        case LIN_IOCTL_FORCE_BUSSLEEP:
          l_terminateTransfer(__OPTIONAL_IFC_PARAMETER__);
          l_TimerStop(__OPTIONAL_IFC_PARAMETER__);
          /* Begin BUGZILLA_540 */
          #if (defined(LIN_21) && defined(LIN_INCLUDE_COOKED_TP))
            l_TimerStop_N_As(__OPTIONAL_TIMER_IFC_PARAMETER__);
            l_TimerStop_N_Cr(__OPTIONAL_TIMER_IFC_PARAMETER__);
          #endif /* LIN_21 && LIN_INCLUDE_COOKED_TP */
          /* End BUGZILLA_540 */
          /* Begin BUGZILLA_478 */
          #if (defined(LIN_SLAVE_LINSCI) && defined(LIN_SLAVE_LINSCI_MUTE_MODE))
          {
            /*
             * disable mute mode
             */
            LIN_MUTE_MODE_DISABLE();
          }
          #endif
		   #if (defined(LIN_SLAVE_LINSCI)) 
          /* End BUGZILLA_478 */
          LIN_RESET_LBDL_BIT();
		  #endif
          IFCCTRL_STATE(ifc) = LIN_STATE_BUSSLEEP;
          /* Begin BUGZILLA_378 */
          LIN_RIE();
          /* End BUGZILLA_378 */
          break;

        case LIN_IOCTL_READ_CONFIG_FLAGS:
          #ifdef LIN_INCLUDE_DIAG2
            retval = LIN_DIAG2_CONFIG_FLAGS(ifc);
            LIN_DIAG2_CONFIG_FLAGS(ifc) = 0;
          #else
            retval = 0;
          #endif
          break;

        case LIN_IOCTL_SET_VARIANT_ID:
          l_productId.variant = *((l_u8 *) pParams);

          break;

        case LIN_IOCTL_READ_VARIANT_ID:
          retval = l_productId.variant;

          break;

      #endif /* LIN_SLAVE_NODE && !LIN_13 */

      #ifdef LIN_SLAVE_NODE
        #ifdef LIN_INCLUDE_DIAGNOSTICS
          case LIN_IOCTL_READ_NAD:

             retval = LIN_DIAG_NAD(ifc);
             break;

          case LIN_IOCTL_WRITE_NAD:

             LIN_DIAG_NAD(ifc) = *((l_u8 *) pParams);
             retval = 0;
             break;

         case LIN_IOCTL_WRITE_INITIAL_NAD:

            l_initialNad = *((l_u8 *) pParams);
            retval = 0;
            break;

        #endif /* LIN_INCLUDE_DIAGNOSTICS */
      #endif /* LIN_SLAVE_NODE */

                
      default:
        retval = 1;
        break;
  }

  l_sys_irq_restore(irqMask);

  return retval;

} /* end l_ifc_ioctl() */

/*********************************************************************
 */
/** l_ifc_rx()
 *
 * \brief transmission control function
 *
 * Dynamic variant of the rx notification function.
 * Must be called by user whenever a character has been received
 * (usually in interrupt context)
 *
 * - standard API
 * - master and slave
 *
 * \param  ifc            handle of the interface
 *
 * \retval none
 *
 */
STACK void l_ifc_rx (l_ifc_handle ifc)
{
  __CHECK_INTERFACE_VOID__(ifc);
  RX_ARCH(ifc);
} /* end l_ifc_rx() */

/*********************************************************************
 */
/** l_ifc_tx()
 *
 * \brief transmission control function
 *
 * Dynamic variant of the tx notification function.
 * Must be called by user whenever a character has been sent
 * (usually in interrupt context)
 *
 * - standard API
 * - master and slave
 *
 * \param  ifc            handle of the interface
 *
 * \retval none
 *
 */
void l_ifc_tx (l_ifc_handle ifc)
{
  __CHECK_INTERFACE_VOID__(ifc);
  TX_ARCH(ifc);
} /* end l_ifc_tx() */


#ifndef LIN_13
  /*********************************************************************
   */
  /** l_ifcSetStatus()
   *
   * \brief internal helper function
   *
   * This function sets the ifc status word entries according to the
   * provided parameters
   *
   * - internal function
   * - master and slave
   *
   * \param  ifc            the handle of the interface
   * \param  status         the status to set
   * \param  frame          protected id of the last frame transmitted
   *
   * \retval none
   *
   */
  STACK void l_ifcSetStatus (__OPTIONAL_IFC_DEFINITION_COMMA__ l_u8 status, l_u8 frame)
  {

     if ((status & (LIN_STATUS_ERROR | LIN_STATUS_SUCCESS)) != 0 )
     {
        /*
         * set overrun bit first
         */
        if ((IFCCTRL_STATUSWORD(ifc)
             &
             (LIN_IFC_STATUS_ERROR | LIN_IFC_STATUS_SUCCESSFUL)) != 0)
        {
           /*
            * according to the API implementation hints, the overrun
            * flag must be set in this case, because we have a previous
            * transfer
            */
           IFCCTRL_STATUSWORD(ifc) |= LIN_IFC_STATUS_OVERRUN;
        }
        
        if ((status & LIN_STATUS_ERROR) != 0)
        {
           IFCCTRL_STATUSWORD(ifc) |= LIN_IFC_STATUS_ERROR;
           
           #ifdef LIN_SLAVE_NODE
           
             /*
              * for a slave node, we must set the response error signal
              * as well
              */
             l_setErrorSignal();
           #endif
           
        }
        else
        {
           /*
            * success
            */
           IFCCTRL_STATUSWORD(ifc) |= LIN_IFC_STATUS_SUCCESSFUL;
        }
        /*
         * set last frame transmitted to 0
         */
        IFCCTRL_STATUSWORD(ifc) &= ( (1 << LIN_IFC_STATUS_FRAME_ID_POS) - 1);
        
        /*
         * code new frame id
         */
        IFCCTRL_STATUSWORD(ifc) |= (l_u16) frame << LIN_IFC_STATUS_FRAME_ID_POS;

     }
#ifdef LIN_21
     /*
      * in all the previous cases there is bus activity
      */
     IFCCTRL_STATUSWORD(ifc) |= LIN_IFC_STATUS_BUS_ACTIVITY;

     /* In the slave, Event triggered frame collision bit will always be zero. */
     #ifdef LIN_MASTER_NODE
       if ((status & LIN_STATUS_EVENT_TRIGGERED_FRAME_COLLISION) != 0)
       {
          IFCCTRL_STATUSWORD(ifc) |= LIN_IFC_STATUS_EVENT_TRIGGERED_FRAME_COLLISION;
       }
       if ((status & LIN_STATUS_EVENT_TRIGGERED_FRAME_COLLISION_SOLVED) != 0)
       {
          IFCCTRL_STATUSWORD(ifc) &= (l_u16) ~LIN_IFC_STATUS_EVENT_TRIGGERED_FRAME_COLLISION;
       }
     #else
     /*
      * Set 'save configuration' bit, if the corresponding request was received.
      * It is set only in the slave node, in the master node is always zero.
      */
       if ((status & LIN_STATUS_SAVE_CONFIGURATION) != 0)
       {
          IFCCTRL_STATUSWORD(ifc) |= LIN_IFC_STATUS_SAVE_CONFIGURATION;
       }
     #endif /* LIN_MASTER_NODE */
#endif /* LIN_21 */
     if ((status & LIN_STATUS_BUSSLEEP) != 0)
     {
        IFCCTRL_STATUSWORD(ifc) |= LIN_IFC_STATUS_GOTO_SLEEP;
#ifdef LIN_21
     /*
      * only in this case there is no bus activity
      */
        IFCCTRL_STATUSWORD(ifc) &= (l_u16) ~LIN_IFC_STATUS_BUS_ACTIVITY;
#endif /* LIN_21 */
     }
     
     return ;
     
  } /* end l_ifcSetStatus() */

  /*********************************************************************
   */
  /** l_ifc_read_status()
   *
   * \brief interface control function
   *
   * This function returns the interface status word
   *
   * - standard API
   * - master and slave
   *
   * \param  ifc            interface handle
   *
   * \return the interface status word
   *
   */
  STACK l_u16 l_ifc_read_status (l_ifc_handle ifc) REENTRANT
  {
    l_u16     statusWord;
    l_irqmask irqMask;


    __CHECK_INTERFACE_VALUE__(ifc, (l_u16)0);

    irqMask    = l_sys_irq_disable();

    statusWord = IFCCTRL_STATUSWORD(ifc);

    /*
     * clear status word after reading
     */
    IFCCTRL_STATUSWORD(ifc) = 0;

    l_sys_irq_restore(irqMask);

    return statusWord;

  } /* end l_ifc_read_status() */

#endif /* !LIN_13 */

#ifdef LIN_PROTOCOL_SWITCH
  /*********************************************************************
   */
  /** l_protocol_switch()
   *
   * \brief interface control function (STMicroelectronics extension)
   *
   * This function can be used to switch an interface to a different
   * protocol (or back to LIN)
   *
   * - STMicroelectronics specific API
   * - master and slave
   *
   * \param  ifc            the handle of the interface
   * \param  linEnable      switch LIN protocol on (1) or off (0)
   *
   * \retval none
   *
   */
  STACK void l_protocol_switch (l_ifc_handle ifc, l_bool linEnable) REENTRANT
  {
    l_irqmask irqMask;

    irqMask = l_sys_irq_disable();

    if (linEnable)
    {
      /*
       * turn LIN on
       */
      if (IFCCTRL_FLAGS(ifc) & LIN_PROTOCOL_OTHER)
      {
        /*
         * LIN was disabled, hence we must reconnect LIN
         */
        INIT_ARCH(ifc);

				#ifndef LIN_21
          l_ifc_connect(__MANDATORY_IFC_PARAMETER__);
        #else
          /* What was done in LIN 2.0 calling the l_ifc_connect() API, must now be done here */
          /* since l_ifc_connect() and l_ifc_disconnect() API are no more present in LIN 2.1 */

          /*
           * call provided hook and check return value
           */
          if (!(CONNECT_ARCH(ifc) ) )
          {
            IFCCTRL_FLAGS(ifc) |= (l_u8) LIN_IFC_CONNECT_FLAG;
            CHANGEBAUDRATE_ARCH(ifc, IFCCTRL_NORMALBAUDRATE(ifc));
          } /* if (successful connect) */
          #if (defined(LIN_SLAVE_NODE) && defined(LIN_START_BUSSLEEP_TIMER_ON_CONNECT))
            l_TimerStart(__OPTIONAL_TIMER_IFC_PARAMETER_COMMA__
                         LIN_BUSSLEEP_SUBCHANNEL,
                         LIN_BUSSLEEP_TIMEOUT_VAL(__MANDATORY_IFC_PARAMETER__) );
          #endif

          #if (defined(LIN_MASTER_NODE) && defined(LIN_SEND_WAKEUP_SIG_ON_CONNECT))
            LIN_RESET_LBDL_BIT();
            IFCCTRL_STATE(ifc) = LIN_STATE_BUSSLEEP;
           /* Begin BUGZILLA_378 */
            LIN_RIE();
           /* End BUGZILLA_378 */
            l_ifc_wake_up(__MANDATORY_IFC_PARAMETER__);
          #endif
        #endif /* !LIN_21 */

        IFCCTRL_FLAGS(ifc) &= (l_u8) ~ LIN_PROTOCOL_OTHER;

      } /* no else, LIN already active
         */

    }
    else
    {
      /*
       * turn LIN off
       */
      if (!(IFCCTRL_FLAGS(ifc) & LIN_PROTOCOL_OTHER) )
      {
        /*
         * LIN is active, disable
         */
        l_terminateTransfer(__OPTIONAL_IFC_PARAMETER__);

				#ifndef LIN_21
          l_ifc_disconnect(__MANDATORY_IFC_PARAMETER__);
        #else
          /* What was done in LIN 2.0 calling the l_ifc_disconnect() API, must now be done here */
          /* since l_ifc_connect() and l_ifc_disconnect() API are no more present in LIN 2.1 */

          /*
           * call provided hook and check return value
           */
          if (!(DISCONNECT_ARCH(ifc)))
          {
            IFCCTRL_FLAGS(ifc) &= (l_u8) ~LIN_IFC_CONNECT_FLAG;
          } /* if (successful connect) */

          l_terminateTransfer(__OPTIONAL_IFC_PARAMETER__);

          l_TimerStop (__OPTIONAL_TIMER_IFC_PARAMETER__);

          /* Begin BUGZILLA_540 */
          #ifdef LIN_INCLUDE_COOKED_TP
          l_TimerStop_N_As(__OPTIONAL_TIMER_IFC_PARAMETER__);
          l_TimerStop_N_Cr(__OPTIONAL_TIMER_IFC_PARAMETER__);
          #endif /* LIN_INCLUDE_COOKED_TP */
          /* End BUGZILLA_540 */
        #endif /* !LIN_21 */

        IFCCTRL_FLAGS(ifc) |= LIN_PROTOCOL_OTHER;
      } /* no else, LIN already disabled
         */

    }

    l_sys_irq_restore(irqMask);
  } /* end l_protocol_switch() */

#endif


/*********************************************************************
 */
/** l_setBaudrate()
 *
 * \brief interface control function (internal)
 *
 * This function sets the baudrate for the specified interface
 * to the requested value
 *
 * - internal function
 * - master and slave
 *
 * \param  ifc            handle of the interface to set baudrate for
 * \param  baudrate       baudrate to set
 *
 * \retval none
 *
 */
STACK void l_setBaudrate (__OPTIONAL_IFC_DEFINITION_COMMA__ l_u16 baudrate)
{
  __CHECK_INTERFACE_VOID__(ifc);

  CHANGEBAUDRATE_ARCH(ifc, baudrate);
  
} /* end l_setBaudrate() */

/*********************************************************************
 *
 * generic Rx/Tx functions
 *
 *********************************************************************/

/*********************************************************************
 */
/** l_sendByte()
 *
 * \brief interface control function (internal)
 *
 * This function sends a single byte over the specified interface
 *
 * - internal function
 * - master and slave
 *
 * \param  ifc            the interface to send on
 * \param  sendByte       the byte to send
 *
 * \retval none
 *
 */
STACK void l_sendByte (__OPTIONAL_IFC_DEFINITION_COMMA__ l_u8 sendByte)
{
  __CHECK_INTERFACE_VOID__(ifc);
	
  TRANSFERCTRL_CHARSENT(ifc) = sendByte;
  TXCHAR_ARCH(ifc, sendByte);
	
} /* end l_sendByte() */

/*
 * eof lin_general.c
 */
