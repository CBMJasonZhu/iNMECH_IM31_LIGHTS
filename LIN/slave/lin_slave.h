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
 *
 *********************************************************************/

/** \file lin_slave.h
 */

#ifndef LIN_SLAVE_H
  #define LIN_SLAVE_H

#define LIN_SLAVE_H_MAJOR_VERSION  1
#define LIN_SLAVE_H_MINOR_VERSION  31
#define LIN_SLAVE_H_PATCH_VERSION  1

#define LIN_SLAVE_H_STM


/*********************************************************************
 * includes
 *********************************************************************/

#include "lin_def.h"
#include "lin_def_gen.h"
#include "lin_def_arch_include.h"
#include "lin_general.h"

#ifdef LIN_VERSION_CONTROL_H
  #error "lin_version_control.h shall not be included in h-files and shall be the last file to be included in a c-file!"
#endif


/*********************************************************************
 * global macros
 *********************************************************************/
#ifdef LIN_13
  #define LIN_WAKEUP_TIMEOUT_VAL_SHORT(IFC) (l_u16)   (128u / (l_u16) ((l_u16) IFCCTRL_NORMALBAUDRATE(ifc) / (l_u16) 1000u)) + (l_u16) 1  /* in ms */
  #define LIN_WAKEUP_TIMEOUT_VAL_LONG(IFC)  (l_u16) (15000u / (l_u16) ((l_u16) IFCCTRL_NORMALBAUDRATE(ifc) / (l_u16) 1000u)) + (l_u16) 1  /* in ms */
  #define LIN_BUSSLEEP_TIMEOUT_VAL(IFC)     (l_u16) (25000u / (l_u16) ((l_u16) IFCCTRL_NORMALBAUDRATE(ifc) / (l_u16) 1000u)) + (l_u16) 1  /* in ms */
#endif

/*********************************************************************
 * global typedefs
 *********************************************************************/

/*********************************************************************
 * global constants
 *********************************************************************/

/*********************************************************************
 * global variables
 *********************************************************************/

/*********************************************************************
 * global functions
 *********************************************************************/

STACK extern void   l_evaluateData(__OPTIONAL_IFC_DEFINITION_COMMA__ l_u8 rxChar);

#ifndef LIN_13
  STACK extern l_bool l_changeFrameId(l_u16 messageId, l_u8 frameId);
#endif

#ifdef LIN_21
  STACK extern l_bool l_changePIDs(l_u8 startId, const l_u8 *pBuffer);
#endif /* LIN_21 */
STACK extern void   l_gotoSleep(__OPTIONAL_IFC_DEFINITION__);
STACK extern void   l_terminateTransfer(__OPTIONAL_IFC_DEFINITION__);
STACK extern void   l_stateIdle(__OPTIONAL_IFC_DEFINITION_COMMA__ l_u8 rxChar);
STACK extern void   l_stateReceiveSync(__OPTIONAL_IFC_DEFINITION_COMMA__ l_u8 rxChar);
STACK extern void   l_serialError(__OPTIONAL_IFC_DEFINITION__);


#endif /* LIN_SLAVE_H */

/*
 * eof lin_slave.h
 */
