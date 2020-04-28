/*********************************************************************
 *
 * Project:      LIN 2.1 driver suite
 *
 * File name:    lin_def_stm8.h
 *
 * Copyright:    (c) STMicroelectronics
 *
 * Author:       Giuseppe Stefano Fazio, STMicroelectronics
 *
 * Description:  Hardware specific configuration and settings for
 *               STM8
 *
 * Target:       STM8
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
 * 20090420  v1.1   gsf  - BUGZILLA_479: LIN_SLAVE_LINSCI_FULL and 
 *                         LIN_SLAVE_LINSCI_MUTE_MODE constants must 
 *                         never be disabled by user.
 *
 *********************************************************************/

/** \file lin_def_stm8.h
 */

#ifndef LIN_DEF_STM8_H
  #define LIN_DEF_STM8_H

#define LIN_DEF_STM8_H_MAJOR_VERSION  1
#define LIN_DEF_STM8_H_MINOR_VERSION  32
#define LIN_DEF_STM8_H_PATCH_VERSION  1

#define LIN_DEF_STM8_H_STM

/*********************************************************************
 * includes
 *********************************************************************/

#ifdef LIN_VERSION_CONTROL_H
  #error "lin_version_control.h shall not be included in h-files and shall be the last file to be included in a c-file!"
#endif

/***Uncomment the statement if External Crystal is used as clock source***/
#undef EXT_CLK 


/*********************************************************************
 * Select the mcu   STM8AF622x(8K DIE) or others.
 * Undef(#undef STM8AF622x) the statement below for 
 *                   MCU other than STM8AF622x(8K DIE) 
 *********************************************************************/
#define STM8AF622x


/*********************************************************************
 * define frequency of the cpu
 *********************************************************************/
#define LIN_BOARD_CPU_FREQ_HZ   16000000   /** It can be 8000000 or 16000000 **/


/*********************************************************************
 *
 * Activate the usage of a hardware timer for LIN via
 * "#define LIN_USE_HARDWARE_TIMER"
 * Deactivate the usage of a hardware timer for LIN via
 * "#undef  LIN_USE_HARDWARE_TIMER"
 *
 *********************************************************************/
#define LIN_USE_HARDWARE_TIMER


#ifdef LIN_USE_HARDWARE_TIMER

 /********************************************************************
  *
  * choose the timer used by the LIN driver
  * (valid values depend on the architecture used)
  *
  ********************************************************************/
  #define LIN_TIMER                    1

#endif

/* Begin BUGZILLA_479 */
/******************************************************************
 * User must define this constant to allow the slave node driver
 * to resynchronize automatically on the Synch Field, when using
 * a non precise clock source.
 ******************************************************************/
#undef LIN_SLAVE_LINSCI_AUTOSYNC

/******************************************************************
 * !!! Caution: Following constants must never be changed by user!
 ******************************************************************/
#undef LIN_SLAVE_LINSCI_BREAK_DETECTION
#define LIN_SLAVE_LINSCI_FULL
#define LIN_SLAVE_LINSCI_MUTE_MODE
/* End BUGZILLA_479 */

#ifdef  LIN_SLAVE_LINSCI_FULL
  #undef LIN_SLAVE_LINSCI_BREAK_DETECTION
#endif

/*******************************************************************
 * Optimisation for STM8
 * Give size available in fastest/most efficient memory area
 * you can spare for the LIN driver in bytes
 *******************************************************************/
#define LIN_ZERO_PAGE_SIZE                              5

/*******************************************************************
 * More stuff to be put into the zero page
 * Consult STM8 architecture notes for details
 *******************************************************************/
#undef LIN_TX_FLAGS_IN_ZERO_PAGE
#undef LIN_RX_FLAGS_IN_ZERO_PAGE
#undef LIN_CHANGED_FLAGS_IN_ZERO_PAGE
#undef LIN_FRAME_BUFFER_IN_ZERO_PAGE
#undef LIN_FRAME_IDS_IN_ZERO_PAGE

#endif /* LIN_DEF_STM8_H */

/*
 * eof lin_def_stm8.h
 */
