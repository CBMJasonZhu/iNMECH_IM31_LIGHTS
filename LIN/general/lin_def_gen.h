/*********************************************************************
 *
 * Project:      LIN 2.1 driver suite
 *
 * File name:    lin_def_gen.h
 *
 * Copyright:    (c) STMicroelectronics
 *
 * Author:       Giuseppe Stefano Fazio
 *
 * Description:  Generic configuration file
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

/** \file lin_def_gen.h
 */

#ifndef LIN_DEF_GEN_H
  #define LIN_DEF_GEN_H

#define LIN_DEF_GEN_H_MAJOR_VERSION  1
#define LIN_DEF_GEN_H_MINOR_VERSION  31
#define LIN_DEF_GEN_H_PATCH_VERSION  1

#define LIN_DEF_GEN_H_STM

/*********************************************************************
 * includes
 *********************************************************************/
#include "lin_cfg_types.h"

#ifdef LIN_VERSION_CONTROL_H
  #error "lin_version_control.h shall not be included in h-files and shall be the last file to be included in a c-file!"
#endif

/*********************************************************************
 *
 * define interface(s) used
 * (not all interfaces may be supported by all architectures)
 *
 *********************************************************************/

/*
 * Calculate if one or more interfaces are used and check if at least one
 * interface is used
 */
#ifdef SCI0_USED
  #define __SINGLE_IFC__ SCI0
#endif
#ifdef SCI1_USED
  #ifdef __SINGLE_IFC__
    #ifndef __MORE_IFCS__
      #define __MORE_IFCS__
    #endif
  #else
    #define __SINGLE_IFC__ SCI1
  #endif
#endif
#ifdef SCI2_USED
  #ifdef __SINGLE_IFC__
    #ifndef __MORE_IFCS__
      #define __MORE_IFCS__
    #endif
  #else
    #define __SINGLE_IFC__ SCI2
  #endif
#endif
#ifdef SCI3_USED
  #ifdef __SINGLE_IFC__
    #ifndef __MORE_IFCS__
      #define __MORE_IFCS__
    #endif
  #else
    #define __SINGLE_IFC__ SCI3
  #endif
#endif
#ifdef SCI4_USED
  #ifdef __SINGLE_IFC__
    #ifndef __MORE_IFCS__
      #define __MORE_IFCS__
    #endif
  #else
    #define __SINGLE_IFC__ SCI4
  #endif
#endif
#ifdef SCI5_USED
  #ifdef __SINGLE_IFC__
    #ifndef __MORE_IFCS__
      #define __MORE_IFCS__
    #endif
  #else
    #define __SINGLE_IFC__ SCI5
  #endif
#endif
#ifdef SCI6_USED
  #ifdef __SINGLE_IFC__
    #ifndef __MORE_IFCS__
      #define __MORE_IFCS__
    #endif
  #else
    #define __SINGLE_IFC__ SCI6
  #endif
#endif
#ifdef SCI7_USED
  #ifdef __SINGLE_IFC__
    #ifndef __MORE_IFCS__
      #define __MORE_IFCS__
    #endif
  #else
    #define __SINGLE_IFC__ SCI7
  #endif
#endif
#ifdef SCI8_USED
  #ifdef __SINGLE_IFC__
    #ifndef __MORE_IFCS__
      #define __MORE_IFCS__
    #endif
  #else
    #define __SINGLE_IFC__ SCI8
  #endif
#endif
#ifdef SCI9_USED
  #ifdef __SINGLE_IFC__
    #ifndef __MORE_IFCS__
      #define __MORE_IFCS__
    #endif
  #else
    #define __SINGLE_IFC__ SCI9
  #endif
#endif

#ifndef __SINGLE_IFC__
  #error "There is no interface defined!"
#endif

/*****************************************************************************
 *
 * Optional function parameters for interface (ifc) and timer (timer).
 *
 * These parameters are only used if more than one interface (and therefore
 * timer channel) is used.
 *
 * These macros shall replace the corresponding '#ifdef's in the common
 * source files.
 *
 *****************************************************************************/

#ifdef __MORE_IFCS__        /* The parameters for ifc and timer are in use */

  /* Parameters for the function calls */

  #define __OPTIONAL_IFC_PARAMETER__              ifc
  #define __OPTIONAL_IFC_PARAMETER_COMMA__        ifc,
  #define __MANDATORY_IFC_PARAMETER__             ifc

  #define __OPTIONAL_TIMER_PARAMETER__            timer
  #define __OPTIONAL_TIMER_PARAMETER_COMMA__      timer,

  #define __OPTIONAL_TIMER_IFC_PARAMETER__        (l_timer_handle) ifc
  #define __OPTIONAL_TIMER_IFC_PARAMETER_COMMA__  (l_timer_handle) ifc,

  /* Parameters for the function definitions */

  #define __OPTIONAL_IFC_DEFINITION__             l_ifc_handle ifc
  #define __OPTIONAL_IFC_DEFINITION_COMMA__       l_ifc_handle ifc,

  #define __OPTIONAL_TIMER_DEFINITION__           l_timer_handle timer
  #define __OPTIONAL_TIMER_DEFINITION_COMMA__     l_timer_handle timer,

#else /* No parameters for ifc and timer are in use */

  /* Parameters for the function calls empty or 0 */

  #define __OPTIONAL_IFC_PARAMETER__
  #define __OPTIONAL_IFC_PARAMETER_COMMA__
  #define __MANDATORY_IFC_PARAMETER__             (l_ifc_handle) 0

  #define __OPTIONAL_TIMER_PARAMETER__
  #define __OPTIONAL_TIMER_PARAMETER_COMMA__

  #define __OPTIONAL_TIMER_IFC_PARAMETER__
  #define __OPTIONAL_TIMER_IFC_PARAMETER_COMMA__


  /* Parameters for the function definitions */

  /*
   * MISRA deviation, rule 19.4
   * Reason: void is not renamed but __OPTIONAL_IFC_DEFINITION__ must be
   *         mapped to void in case of single interface
   */
  #define __OPTIONAL_IFC_DEFINITION__         void
  #define __OPTIONAL_IFC_DEFINITION_COMMA__

  /*
   * MISRA deviation, rule 19.4
   * Reason: void is not renamed but __OPTIONAL_TIMER_DEFINITION__ must be
   *         mapped to void in case of single interface
   */
  #define __OPTIONAL_TIMER_DEFINITION__       void
  #define __OPTIONAL_TIMER_DEFINITION_COMMA__
#endif


/*********************************************************************
 *
 * macros to control compilation of diagnostic module
 *
 *********************************************************************/
#ifdef LIN_13
  #undef LIN_INCLUDE_ASSIGN_FRAME_ID
  #undef LIN_INCLUDE_ASSIGN_NAD
  #undef LIN_INCLUDE_READ_BY_ID
  #undef LIN_INCLUDE_COND_CHANGE_NAD
  #undef LIN_INCLUDE_DATA_DUMP
  #undef LIN_INCLUDE_COOKED_TP
  #undef LIN_INCLUDE_RAW_TP
  #undef LIN_PROTOCOL_SWITCH
#endif

#if (defined(LIN_INCLUDE_ASSIGN_FRAME_ID) || defined(LIN_INCLUDE_READ_BY_ID) || defined(LIN_INCLUDE_ASSIGN_NAD) || defined (LIN_INCLUDE_COND_CHANGE_NAD) || defined(LIN_INCLUDE_DATA_DUMP) || defined (LIN_INCLUDE_ASSIGN_FRAME_ID_RANGE) || defined(LIN_INCLUDE_SAVE_CONFIGURATION) )
  #define LIN_INCLUDE_DIAG2
#else
  #undef LIN_INCLUDE_DIAG2
#endif


#if (defined(LIN_INCLUDE_COOKED_TP) || defined(LIN_INCLUDE_RAW_TP))
  #define LIN_INCLUDE_TP
#else
  #undef LIN_INCLUDE_TP
#endif

#if (defined(LIN_INCLUDE_TP) || defined(LIN_INCLUDE_DIAG2))
  #define LIN_INCLUDE_DIAGNOSTICS
#else
  #undef LIN_INCLUDE_DIAGNOSTICS
#endif

/*********************************************************************
 * interface handle type
 *********************************************************************/
/**
 * enum for interface handle
 */
/*
 * MISRA deviation, rule 9.3
 * Reason: It is not known in advance how many interfaces are used,
 *         initialization must be left to the compiler
 */
typedef enum
{
  #ifdef SCI0_USED
    LIN_IFC_SCI0,
  #endif

  #ifdef SCI1_USED
    LIN_IFC_SCI1,
  #endif

  #ifdef SCI2_USED
    LIN_IFC_SCI2,
  #endif

  #ifdef SCI3_USED
    LIN_IFC_SCI3,
  #endif

  #ifdef SCI4_USED
    LIN_IFC_SCI4,
  #endif

  #ifdef SCI5_USED
    LIN_IFC_SCI5,
  #endif

  #ifdef SCI6_USED
    LIN_IFC_SCI6,
  #endif

  #ifdef SCI7_USED
    LIN_IFC_SCI7,
  #endif

  #ifdef SCI8_USED
    LIN_IFC_SCI8,
  #endif

  #ifdef SCI9_USED
    LIN_IFC_SCI9,
  #endif

    LIN_IFC_NR_MAX,
    LIN_INVALID_IFC_INDEX = 0xFF
} l_ifc_handle;


/*********************************************************************
 *
 * flag handling macros
 *
 *********************************************************************/

/* byte-wise */
#define LIN_SET_CHANGED_FLAG_BYTE(A, B)     \
  l_pChangedFlagData[(A)] = (B)

#define LIN_CLEAR_CHANGED_FLAG_BYTE(A)      \
  l_pChangedFlagData[(A)] = 0

#define LIN_TEST_CHANGED_FLAG_BYTE(A)       \
  (l_pChangedFlagData[(A)])

/* bit-wise */
#define LIN_SET_CHANGED_FLAG_BIT(A, B)      \
  l_pChangedFlagData[(A)] |= (l_u8) (1U << (B))

#define LIN_CLEAR_CHANGED_FLAG_BIT(A, B)    \
  l_pChangedFlagData[(A)] &= (l_u8) (~(1U << (B)))

#define LIN_TEST_CHANGED_FLAG_BIT(A, B)     \
  (l_pChangedFlagData[(A)] & (l_u8) (1U << (B)))


/*********************************************************************
 *
 * definition of function prototypes depending on interface
 *
 *********************************************************************/
#ifdef SCI0_USED
  #define l_ifc_init_SCI0()        l_ifc_init(LIN_IFC_SCI0)
	#ifndef LIN_21
    #define l_ifc_connect_SCI0()     l_ifc_connect(LIN_IFC_SCI0)
    #define l_ifc_disconnect_SCI0()  l_ifc_disconnect(LIN_IFC_SCI0)
  #endif /* !LIN_21 */
  #define l_sch_tick_SCI0()        l_sch_tick(LIN_IFC_SCI0)
  #define l_sch_set_SCI0(A,B)      l_sch_set(LIN_IFC_SCI0, (A), (B))
  #define l_ifc_goto_sleep_SCI0()  l_ifc_goto_sleep(LIN_IFC_SCI0)
  #define l_ifc_wake_up_SCI0()     l_ifc_wake_up(LIN_IFC_SCI0)
  #define l_ifc_ioctl_SCI0(A,B)    l_ifc_ioctl(LIN_IFC_SCI0, (A), (B))
  #define l_ifc_aux_SCI0()         l_ifc_aux(LIN_IFC_SCI0)
  #define l_ifc_read_status_SCI0() l_ifc_read_status(LIN_IFC_SCI0)
#endif

#ifdef SCI1_USED
  #define l_ifc_init_SCI1()        l_ifc_init(LIN_IFC_SCI1)
	#ifndef LIN_21
    #define l_ifc_connect_SCI1()     l_ifc_connect(LIN_IFC_SCI1)
    #define l_ifc_disconnect_SCI1()  l_ifc_disconnect(LIN_IFC_SCI1)
  #endif /* !LIN_21 */
  #define l_sch_tick_SCI1()        l_sch_tick(LIN_IFC_SCI1)
  #define l_sch_set_SCI1(A,B)      l_sch_set(LIN_IFC_SCI1, (A), (B))
  #define l_ifc_goto_sleep_SCI1()  l_ifc_goto_sleep(LIN_IFC_SCI1)
  #define l_ifc_wake_up_SCI1()     l_ifc_wake_up(LIN_IFC_SCI1)
  #define l_ifc_ioctl_SCI1(A,B)    l_ifc_ioctl(LIN_IFC_SCI1, (A), (B))
  #define l_ifc_aux_SCI1()         l_ifc_aux(LIN_IFC_SCI1)
  #define l_ifc_read_status_SCI1() l_ifc_read_status(LIN_IFC_SCI1)
#endif

#ifdef SCI2_USED
  #define l_ifc_init_SCI2()        l_ifc_init(LIN_IFC_SCI2)
  #ifndef LIN_21
    #define l_ifc_connect_SCI2()     l_ifc_connect(LIN_IFC_SCI2)
    #define l_ifc_disconnect_SCI2()  l_ifc_disconnect(LIN_IFC_SCI2)
  #endif /* !LIN_21 */
  #define l_sch_tick_SCI2()        l_sch_tick(LIN_IFC_SCI2)
  #define l_sch_set_SCI2(A,B)      l_sch_set(LIN_IFC_SCI2, (A), (B))
  #define l_ifc_goto_sleep_SCI2()  l_ifc_goto_sleep(LIN_IFC_SCI2)
  #define l_ifc_wake_up_SCI2()     l_ifc_wake_up(LIN_IFC_SCI2)
  #define l_ifc_ioctl_SCI2(A,B)    l_ifc_ioctl(LIN_IFC_SCI2, (A), (B))
  #define l_ifc_aux_SCI2()         l_ifc_aux(LIN_IFC_SCI2)
  #define l_ifc_read_status_SCI2() l_ifc_read_status(LIN_IFC_SCI2)
#endif

#ifdef SCI3_USED
  #define l_ifc_init_SCI3()        l_ifc_init(LIN_IFC_SCI3)
  #ifndef LIN_21
    #define l_ifc_connect_SCI3()     l_ifc_connect(LIN_IFC_SCI3)
    #define l_ifc_disconnect_SCI3()  l_ifc_disconnect(LIN_IFC_SCI3)
  #endif /* !LIN_21 */
  #define l_sch_tick_SCI3()        l_sch_tick(LIN_IFC_SCI3)
  #define l_sch_set_SCI3(A,B)      l_sch_set(LIN_IFC_SCI3, (A), (B))
  #define l_ifc_goto_sleep_SCI3()  l_ifc_goto_sleep(LIN_IFC_SCI3)
  #define l_ifc_wake_up_SCI3()     l_ifc_wake_up(LIN_IFC_SCI3)
  #define l_ifc_ioctl_SCI3(A,B)    l_ifc_ioctl(LIN_IFC_SCI3, (A), (B))
  #define l_ifc_aux_SCI3()         l_ifc_aux(LIN_IFC_SCI3)
  #define l_ifc_read_status_SCI3() l_ifc_read_status(LIN_IFC_SCI3)
#endif

#ifdef SCI4_USED
  #define l_ifc_init_SCI4()        l_ifc_init(LIN_IFC_SCI4)
	#ifndef LIN_21
    #define l_ifc_connect_SCI4()     l_ifc_connect(LIN_IFC_SCI4)
    #define l_ifc_disconnect_SCI4()  l_ifc_disconnect(LIN_IFC_SCI4)
  #endif /* !LIN_21 */
  #define l_sch_tick_SCI4()        l_sch_tick(LIN_IFC_SCI4)
  #define l_sch_set_SCI4(A,B)      l_sch_set(LIN_IFC_SCI4, (A), (B))
  #define l_ifc_goto_sleep_SCI4()  l_ifc_goto_sleep(LIN_IFC_SCI4)
  #define l_ifc_wake_up_SCI4()     l_ifc_wake_up(LIN_IFC_SCI4)
  #define l_ifc_ioctl_SCI4(A,B)    l_ifc_ioctl(LIN_IFC_SCI4, (A), (B))
  #define l_ifc_aux_SCI4()         l_ifc_aux(LIN_IFC_SCI4)
  #define l_ifc_read_status_SCI4() l_ifc_read_status(LIN_IFC_SCI4)
#endif

#ifdef SCI5_USED
  #define l_ifc_init_SCI5()        l_ifc_init(LIN_IFC_SCI5)
	#ifndef LIN_21
    #define l_ifc_connect_SCI5()     l_ifc_connect(LIN_IFC_SCI5)
    #define l_ifc_disconnect_SCI5()  l_ifc_disconnect(LIN_IFC_SCI5)
  #endif /* !LIN_21 */
  #define l_sch_tick_SCI5()        l_sch_tick(LIN_IFC_SCI5)
  #define l_sch_set_SCI5(A,B)      l_sch_set(LIN_IFC_SCI5, (A), (B))
  #define l_ifc_goto_sleep_SCI5()  l_ifc_goto_sleep(LIN_IFC_SCI5)
  #define l_ifc_wake_up_SCI5()     l_ifc_wake_up(LIN_IFC_SCI5)
  #define l_ifc_ioctl_SCI5(A,B)    l_ifc_ioctl(LIN_IFC_SCI5, (A), (B))
  #define l_ifc_aux_SCI5()         l_ifc_aux(LIN_IFC_SCI5)
  #define l_ifc_read_status_SCI5() l_ifc_read_status(LIN_IFC_SCI5)
#endif

#ifdef SCI6_USED
  #define l_ifc_init_SCI6()        l_ifc_init(LIN_IFC_SCI6)
	#ifndef LIN_21
    #define l_ifc_connect_SCI6()     l_ifc_connect(LIN_IFC_SCI6)
    #define l_ifc_disconnect_SCI6()  l_ifc_disconnect(LIN_IFC_SCI6)
  #endif /* !LIN_21 */
  #define l_sch_tick_SCI6()        l_sch_tick(LIN_IFC_SCI6)
  #define l_sch_set_SCI6(A,B)      l_sch_set(LIN_IFC_SCI6, (A), (B))
  #define l_ifc_goto_sleep_SCI6()  l_ifc_goto_sleep(LIN_IFC_SCI6)
  #define l_ifc_wake_up_SCI6()     l_ifc_wake_up(LIN_IFC_SCI6)
  #define l_ifc_ioctl_SCI6(A,B)    l_ifc_ioctl(LIN_IFC_SCI6, (A), (B))
  #define l_ifc_aux_SCI6()         l_ifc_aux(LIN_IFC_SCI6)
  #define l_ifc_read_status_SCI6() l_ifc_read_status(LIN_IFC_SCI6)
#endif

#ifdef SCI7_USED
  #define l_ifc_init_SCI7()        l_ifc_init(LIN_IFC_SCI7)
	#ifndef LIN_21
    #define l_ifc_connect_SCI7()     l_ifc_connect(LIN_IFC_SCI7)
    #define l_ifc_disconnect_SCI7()  l_ifc_disconnect(LIN_IFC_SCI7)
  #endif /* !LIN_21 */
  #define l_sch_tick_SCI7()        l_sch_tick(LIN_IFC_SCI7)
  #define l_sch_set_SCI7(A,B)      l_sch_set(LIN_IFC_SCI7, (A), (B))
  #define l_ifc_goto_sleep_SCI7()  l_ifc_goto_sleep(LIN_IFC_SCI7)
  #define l_ifc_wake_up_SCI7()     l_ifc_wake_up(LIN_IFC_SCI7)
  #define l_ifc_ioctl_SCI7(A,B)    l_ifc_ioctl(LIN_IFC_SCI7, (A), (B))
  #define l_ifc_aux_SCI7()         l_ifc_aux(LIN_IFC_SCI7)
  #define l_ifc_read_status_SCI7() l_ifc_read_status(LIN_IFC_SCI7)
#endif

#ifdef SCI8_USED
  #define l_ifc_init_SCI8()        l_ifc_init(LIN_IFC_SCI8)
	#ifndef LIN_21
    #define l_ifc_connect_SCI8()     l_ifc_connect(LIN_IFC_SCI8)
    #define l_ifc_disconnect_SCI8()  l_ifc_disconnect(LIN_IFC_SCI8)
  #endif /* !LIN_21 */
  #define l_sch_tick_SCI8()        l_sch_tick(LIN_IFC_SCI8)
  #define l_sch_set_SCI8(A,B)      l_sch_set(LIN_IFC_SCI8, (A), (B))
  #define l_ifc_goto_sleep_SCI8()  l_ifc_goto_sleep(LIN_IFC_SCI8)
  #define l_ifc_wake_up_SCI8()     l_ifc_wake_up(LIN_IFC_SCI8)
  #define l_ifc_ioctl_SCI8(A,B)    l_ifc_ioctl(LIN_IFC_SCI8, (A), (B))
  #define l_ifc_aux_SCI8()         l_ifc_aux(LIN_IFC_SCI8)
  #define l_ifc_read_status_SCI8() l_ifc_read_status(LIN_IFC_SCI8)
#endif

#ifdef SCI9_USED
  #define l_ifc_init_SCI9()        l_ifc_init(LIN_IFC_SCI9)
	#ifndef LIN_21
    #define l_ifc_connect_SCI9()     l_ifc_connect(LIN_IFC_SCI9)
    #define l_ifc_disconnect_SCI9()  l_ifc_disconnect(LIN_IFC_SCI9)
  #endif /* !LIN_21 */
  #define l_sch_tick_SCI9()        l_sch_tick(LIN_IFC_SCI9)
  #define l_sch_set_SCI9(A,B)      l_sch_set(LIN_IFC_SCI9, (A), (B))
  #define l_ifc_goto_sleep_SCI9()  l_ifc_goto_sleep(LIN_IFC_SCI9)
  #define l_ifc_wake_up_SCI9()     l_ifc_wake_up(LIN_IFC_SCI9)
  #define l_ifc_ioctl_SCI9(A,B)    l_ifc_ioctl(LIN_IFC_SCI9, (A), (B))
  #define l_ifc_aux_SCI9()         l_ifc_aux(LIN_IFC_SCI9)
  #define l_ifc_read_status_SCI9() l_ifc_read_status(LIN_IFC_SCI9)
#endif

#endif /* LIN_DEF_GEN_H */
/*
 * eof lin_def_gen.h
 */
