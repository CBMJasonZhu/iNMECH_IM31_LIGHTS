/*********************************************************************
 *
 * Project:      LIN 2.1 driver suite
 *
 * File name:    lin_arch_include.h
 *
 * Copyright:    (c) STMicroelectronics
 *
 * Author:       Giuseppe Stefano Fazio
 *
 * Description:  Include the hardware dependent header file
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
 * 20120522  V1.1   jkh  - modified for STM8L
 *
 *********************************************************************/

/** \file lin_arch_include.h
 */

#ifndef LIN_ARCH_INCLUDE_H
  #define LIN_ARCH_INCLUDE_H

#define LIN_ARCH_INCLUDE_H_MAJOR_VERSION  1
#define LIN_ARCH_INCLUDE_H_MINOR_VERSION  32
#define LIN_ARCH_INCLUDE_H_PATCH_VERSION  1

#define LIN_ARCH_INCLUDE_H_STM

/*********************************************************************
 * includes
 *********************************************************************/

#ifdef __ARCH_STM8__
  #include "lin_stm8.h"
#elif defined(__ARCH_STM8L__)
	#include "lin_stm8L.h"
#else
  #error "This architecture is not supported by this LIN driver"
#endif


#ifdef LIN_VERSION_CONTROL_H
  #error "lin_version_control.h shall not be included in h-files and shall be the last file to be included in a c-file!"
#endif


#endif /* LIN_ARCH_INCLUDE_H */
