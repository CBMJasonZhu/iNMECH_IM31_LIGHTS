/*********************************************************************
 *
 * Project:      LIN 2.1 driver suite
 *
 * File name:    lin_def_arch_include.h
 *
 * Copyright:    (c) STMicroelectronics
 *
 * Author:       Giuseppe Stefano Fazio
 *
 * Description:  Header file to include correct hw specific configuration
 *               file
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
 * 20120522	 v1.1   jkh  - modified for STM8L
 *
 *********************************************************************/

/** \file lin_def_arch_include.h
 */

#ifndef LIN_DEF_ARCH_INCLUDE_H
  #define LIN_DEF_ARCH_INCLUDE_H

#define LIN_DEF_ARCH_INCLUDE_H_MAJOR_VERSION  1
#define LIN_DEF_ARCH_INCLUDE_H_MINOR_VERSION  32
#define LIN_DEF_ARCH_INCLUDE_H_PATCH_VERSION  1

#define LIN_DEF_ARCH_INCLUDE_H_STM


/*********************************************************************
 * includes
 *********************************************************************/

#if defined(__ARCH_STM8__)
  #include "lin_def_stm8.h"
  #include "lin_def_stm8_gen.h"
#elif defined(__ARCH_STM8L__)
  #include "lin_def_stm8L.h"
  #include "lin_def_stm8L_gen.h"
#else
  #error "This architecture is not supported by this LIN driver"
#endif

#ifdef LIN_VERSION_CONTROL_H
  #error "lin_version_control.h shall not be included in h-files and shall be the last file to be included in a c-file!"
#endif


#endif /* LIN_DEF_ARCH_INCLUDE_H */
/*
 * eof lin_def_arch_include.h
 */
