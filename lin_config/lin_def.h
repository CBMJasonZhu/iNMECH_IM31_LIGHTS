/*********************************************************************
 *
 * Project:      LIN 2.1 driver suite
 *
 * File name:    lin_def.h
 *
 * Copyright:    (c) STMicroelectronics
 *
 * Author:       Giuseppe Stefano Fazio, STMicroelectronics
 *
 * Description:  User definitions regarding the driver hardware configuration
 *
 * Target:       all
 *
 * Names:        gsf  Giuseppe Stefano Fazio  STMicroelectronics
 *
 * TO BE CHANGED BY THE USER
 *
 *********************************************************************/

/*********************************************************************
 *
 * History:
 *
 * 20071218  v1.0   gsf  - Initial version for STM8
 * 20090420  v1.1   gsf  - BUGZILLA_540: LIN 2.1 / N_Cr_LIN timeout
 *                         and N_As_LIN timeout management missing. 
 *                         BUGZILLA_541: Response_error bit management
 *                         in case of no response or incomplete response.
 * 20100430  v1.2   JK  -  BUGZILLA_1104: LIN_ABORT_TP_PREVIOUS_SESSION.
 *
 *********************************************************************/

/** \file lin_def.h
 */

#ifndef LIN_DEF_H
  #define LIN_DEF_H

#define LIN_DEF_H_MAJOR_VERSION  1
#define LIN_DEF_H_MINOR_VERSION  31
#define LIN_DEF_H_PATCH_VERSION  1

#define LIN_DEF_H_STM

/*********************************************************************
 * includes
 *********************************************************************/

#include "lin_cfg_types.h"


#ifdef LIN_VERSION_CONTROL_H
  #error "lin_version_control.h shall not be included in h-files and shall be the last file to be included in a c-file!"
#endif

/*********************************************************************
 *
 * driver configuration section
 *
 *********************************************************************/


/*********************************************************************
 * Set the driver for development or production:
 *
 *
 * For development:
 * #define LIN_DEVELOPMENT, several checks of input parmeters are 
 * performed.
 * This will be quite useful for debugging during the development
 * phase.
 *
 * For production:
 * #undef LIN_DEVELOPMENT, only a few checks on the input parameters 
 * of the functions are performed. Activate this for smaller and faster
 *  code for the production phase after development.
 *********************************************************************/

#define LIN_DEVELOPMENT


/*********************************************************************
 *
 * Set the time base of the lin timer in ms.
 * This is the time base of the timer ticks of the application driven 
 * timer or the time base of the hardware timer
 *
 *********************************************************************/
#define LIN_TIME_BASE_IN_MS           2


/*********************************************************************
 *
 * select the maximum frame transfer time in multiples of the nominal
 * time (*100)
 *
 *********************************************************************/
#define LIN_FRAME_TIME_MULTIPLIER    140

/*********************************************************************
 *
 * length of the normal break signal in bit times (nr of bits)
 * recommended is 13
 * Please adjust LIN_FRAME_TIME_MULTIPLIER if necessary
 *
 *********************************************************************/
#define LIN_BREAK_DURATION_BITS       13

/*********************************************************************
 *
 * Activate the Cooling option with  "#define LIN_USE_COOLING"
 * Deactivate it with                "#undef  LIN_USE_COOLING"
 *
 *********************************************************************/
#undef LIN_USE_COOLING


#ifdef LIN_USE_COOLING

/*********************************************************************
 *
 * length of the break signal in bit times (nr of bits)
 * Please adjust LIN_FRAME_TIME_MULTIPLIER if necessary
 *
 *********************************************************************/
  #define LIN_COOLING_BREAK_DURATION_BITS       36

#endif


/*********************************************************************
 *
 * select whether the slave node will start the bussleep timer on
 * connect
 * Note: The slave may loose the first frame if the master node
 *       does not start with a Wakeup signal followed by 100ms silence
 *
 *********************************************************************/
#undef LIN_START_BUSSLEEP_TIMER_ON_CONNECT

/*********************************************************************
 *
 * select whether the master node should start a wakeup sequence
 * on connect
 *
 *********************************************************************/
#undef LIN_SEND_WAKEUP_SIG_ON_CONNECT

/*********************************************************************
 *
 * Configuration features. Select by define'ing.
 * Default values match the mandatory services defined by the
 * standard
 *
 *********************************************************************/

/*********************************************************************
 * parity check (optional/slave node only)
 * if set, this define will enable parity checking on protected
 * identifiers. Note that the identifiers will only check when set
 * either from the master node via configuration requests or by the
 * slave node application, not during frame header reception
 *********************************************************************/
#undef LIN_INCLUDE_PID_PARITY_CHECK

/*********************************************************************
 * service Assign Frame Id (mandatory for LIN 2.0, obsolete for LIN 2.1)
 *********************************************************************/
#undef LIN_INCLUDE_ASSIGN_FRAME_ID

/*********************************************************************
 * service Assign NAD (optional for LIN 2.x)
 *********************************************************************/
#define LIN_INCLUDE_ASSIGN_NAD

/*********************************************************************
 * service Read By Id (mandatory for LIN 2.x)
 *********************************************************************/
#define LIN_INCLUDE_READ_BY_ID

/*********************************************************************
 *service Conditional Change NAD (optional for LIN 2.x)
 *********************************************************************/
#define LIN_INCLUDE_COND_CHANGE_NAD

/*********************************************************************
 * service Data Dump (optional for LIN 2.x)
 * Note: The standard strongly discourages use of this service in
 *       operational LIN clusters
 *********************************************************************/
#define LIN_INCLUDE_DATA_DUMP

/********************************************************************
 *
 * choose Serial Number (optional for Slave node)
 * Slave node may have a serial number to identify a specific instance
 * of a slave node product. The serial number is 4 bytes long.
 *
 ********************************************************************/
#define SERIAL_NUMBER            (l_u32) 0xFFFF

#ifdef LIN_21
/*********************************************************************
 * service Save Configuration (optional for LIN 2.1)
 *********************************************************************/
#define LIN_INCLUDE_SAVE_CONFIGURATION

/*********************************************************************
 * service Assign Frame Id Range (mandatory for LIN 2.1)
 *********************************************************************/
#define LIN_INCLUDE_ASSIGN_FRAME_ID_RANGE

/*********************************************************************
 * choose Diagnostic Class (mandatory for LIN 2.1 slave node)
 * LIN 2.1 slave nodes must have a Diagnostic Class value defined.
 * This value can be: 1,2 or 3 (other values will involve in an error).
 *********************************************************************/
/*
 * DIAGNOSTIC_CLASS 1: Only the node configuration services are supported. 
 * The slave does not support any other diagnostic services.
 * Single frames (SF) transport protocol support is sufficient.
 * Node Identification is limited to the mandatory read by identifier 
 * service.
 *
 * DIAGNOSTIC_CLASS 2: Node configuration and identification services 
 * are supported. 
 * Full transport layer implementation is required to support 
 * multi-frame transmissions.
 * Node Identification is extended to all the Read By Id services 
 * Slave-nodes will support a set of ISO 14229-1 diagnostic services
 * like Node identification (SID 0x22), Reading data parameter (SID 0x22)
 * if applicable, Writing parameters (SID 0x2E) if applicable.
 *
 * DIAGNOSTIC_CLASS 3: Node configuration and identification services 
 * are supported. 
 * Full transport layer implementation is required to support 
 * multi-frame transmissions.
 * Node Identification is extended to all the Read By Id services 
 * Slave-nodes shall support all services as of Class II.
 * Additionally, other services may be supported depending on the 
 * features which are implemented by the slave node: for example
 * Session control (SID 0x10), I/O control by identifier (0x2F),
 * Read and clear DTC (SID 0x19, 0x14).
 * Only class 3 slave nodes can reprogram the application via the LIN bus.
 *********************************************************************/ 
 #define LIN_DIAGNOSTIC_CLASS 1

 #ifdef LIN_SLAVE_NODE
   #ifndef LIN_DIAGNOSTIC_CLASS
     #error "For a LIN 2.1 slave node, LIN Diagnostic Class is mandatory and must be defined!"
   #endif
   
   #if ((LIN_DIAGNOSTIC_CLASS < 1) || (LIN_DIAGNOSTIC_CLASS > 3))
     #error "LIN 2.1 Diagnostic Class value can be 1,2 or 3!"
   #endif
 #else
   #if ((!defined(LIN_MASTER_ONLY)) || (!defined(LIN_SLAVE_ONLY)))
     #error "A LIN 2.1 master node must support the Interleaved Diagnostics schedule Mode (mandatory)!"
   #endif
 #endif

#endif /* LIN_21 */

/*********************************************************************
 *
 * LIN TP features. Select by define'ing.
 * TP is disabled by default
 *
 *********************************************************************/

/*********************************************************************
 * the cooked diagnostic TP
 *********************************************************************/
#undef LIN_INCLUDE_COOKED_TP

/*********************************************************************
 * the raw diagnostic TP
 *********************************************************************/
#undef LIN_INCLUDE_RAW_TP

/*********************************************************************
 * define the stack size of the raw tp fifo stacks
 * (in numbers of frames) 
 *********************************************************************/
#define LIN_DIAG3_FIFO_SIZE_MAX                             1

/*********************************************************************
 * slave node only
 * must be defined if the semi-automatic baudrate detection is to be
 * used in this node
 *
 *********************************************************************/
#undef LIN_BAUDRATE_DETECT


/*********************************************************************
 * slave or master node
 * for some applications (or testing) it may be desirable to use an
 * alternative protocol over the interface used by LIN. If
 * this item is defined, it is possible to switch between LIN and
 * another protocol.
 *********************************************************************/
#undef LIN_PROTOCOL_SWITCH


/*********************************************************************
 * When switching from L_NULL_SCHEDULE to a valid schedule table,
 * the initial delay is undefined. Use this #define to set the initial
 * value of the delay in timebase ticks
 *********************************************************************/
#define LIN_DELAY_INIT 2

/*********************************************************************
 * LIN 1.2/1.3 specific setting
 * #define this if you want to use LIN 2.x goto sleep/wakeup API
 * for LIN 1.2/1.3
 *********************************************************************/
#define LIN_INCLUDE_2x_SLEEP_MODE_API

/*********************************************************************
 * LIN 2.x specific setting
 * The value for the bussleep timeout is configurable here (in
 * milliseconds). The recommended default value given in the
 * standard is 4secs.
 * The other two defines give the period between the signals
 * in milliseconds, the standard demands 150 and 1500 ms
 *********************************************************************/
#ifndef LIN_13
  #define LIN_BUSSLEEP_TIMEOUT_VAL(IFC)     (l_u16) 4000
  #define LIN_WAKEUP_TIMEOUT_VAL_SHORT(IFC) (l_u16)  150
  #define LIN_WAKEUP_TIMEOUT_VAL_LONG(IFC)  (l_u16) 1500
#endif

/*********************************************************************
 * Master specific setting
 * Here the delay after wakeup before sending frames is specified
 * for LIN 2.x.
 * The value is configurable here (in milliseconds). The default value
 * is 100 milli secs.
 * Here you can specify other values, for example shorter times if
 * you know that your slaves are responding faster.
 * In particular the CANOe.lin conformance test needs a shorter time,
 * recommended in this case is 75.
 *********************************************************************/
#define LIN_MASTER_WAKEUP_TIMER_VALUE                 100

/*********************************************************************
 * The number of wakeup retries to send.
 * If after a wakeup signal from the slave the master does not
 * start to send frame headers, the slave may retry to send the
 * wakeup signal. The define gives the maximum number of retries
 *********************************************************************/
#define LIN_WAKEUP_RETRIES_MAX                          3

/* Begin BUGZILLA_541 */
#ifdef LIN_20
/*********************************************************************
 * Response error set on incomplete received data (optional for 
 * LIN 2.0 slave node).
 * By defining this macro, LIN 2.0 slave nodes can choose to set the 
 * Response_error bit when the slave receives some data bytes, but the
 * response is not complete (data bytes missing or checksum is missing
 * in response), and only if the slave is interested in the frame.
 *********************************************************************/ 
 #define LIN_RESPONSE_ERROR_WHEN_INCOMPLETE_RX

#endif /* LIN_20 */
/* End BUGZILLA_541 */

/* Begin BUGZILLA_540 */
#if (defined(LIN_21) && defined(LIN_INCLUDE_COOKED_TP))
/*********************************************************************
 * LIN 2.1 N_As timeout value (mandatory for LIN 2.1 master nodes only).
 *
 * LIN 2.1 N_As_timeout is automatically generated by Lingen only for 
 * LIN 2.1 slave nodes taking the information directly from the LDF 
 * file (Node_attributes section). For LIN 2.1 Master node, the driver
 * will use the following define.
 *
 *********************************************************************/
    #undef N_AS_TIMEOUT        /* 1000 msec is the default value */

/*********************************************************************
 * LIN 2.1 N_Cr timeout value (mandatory for LIN 2.1 master nodes only).
 *
 * LIN 2.1 N_Cr_timeout is automatically generated by Lingen only for 
 * LIN 2.1 slave nodes taking the information directly from the LDF 
 * file (Node_attributes section). For LIN 2.1 Master node, the driver
 * will use the following define.
 *
 *********************************************************************/
  #undef N_CR_TIMEOUT          /* 1000 msec is the default value */
  
/* Begin BUGZILLA_1104 */   
/*********************************************************************
 * LIN 2.1 Abort segmented TP session (optional for LIN 2.1 slave nodes only).
 *
 * If a slave node receives a diagnostic request with the NAD equal to the
 * address of the node or equal to the broadcast address, the new request
 * must be interpreted and carried out even if the ECU was in session of
 * segmentation (in this case the session in progress must be aborted).
 *
 *********************************************************************/
  #define LIN_ABORT_TP_PREVIOUS_SESSION
  
/* End BUGZILLA_1104 */
#endif /* LIN_21 && LIN_INCLUDE_COOKED_TP */
/* End BUGZILLA_540 */

#endif /* LIN_DEF_H */

/*
 * eof lin_def.h
 */
