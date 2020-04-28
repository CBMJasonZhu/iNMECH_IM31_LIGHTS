/*********************************************************************
 *
 * Project:      LIN 2.1 driver suite
 *
 * File name:    lin_master.h
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
 * 20071213  v1.0   gsf  - Initial version for STM8
 *
 *********************************************************************/

/** \file lin_master.h
 */
#ifndef LIN_MASTER_H
  #define LIN_MASTER_H

#define LIN_MASTER_H_MAJOR_VERSION  1
#define LIN_MASTER_H_MINOR_VERSION  31
#define LIN_MASTER_H_PATCH_VERSION  1

#define LIN_MASTER_H_STM

/*********************************************************************
 * includes
 *********************************************************************/

#include "lin_def_gen.h"
#include "lin_def_arch_include.h"


#ifdef LIN_VERSION_CONTROL_H
  #error "lin_version_control.h shall not be included in h-files and shall be the last file to be included in a c-file!"
#endif


/*********************************************************************
 * global macros
 *********************************************************************/

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

STACK extern void l_evaluateData(__OPTIONAL_IFC_DEFINITION_COMMA__ const l_u8 rxChar);
STACK extern void l_terminateTransfer(__OPTIONAL_IFC_DEFINITION__);
STACK extern void l_sendBreak(__OPTIONAL_IFC_DEFINITION__);

#endif /* LIN_MASTER_H */

