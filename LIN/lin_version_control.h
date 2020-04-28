/*********************************************************************
 *
 * Project:      LIN 2.0 driver suite
 *
 * File name:    lin_version_control.h
 *
 * Copyright:    (c) STMicroelectronics
 *
 * Author:       Giuseppe Stefano Fazio
 *
 * Description:  Version control of all used c and h files for a given
 *               LIN version
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
 * 20090420  v1.1   gsf  - Modified lin_cfg*.* file version due to the
 *                         new release of LINGEN v.3.2.0.
 * 20120522  v1.2   jkh  - Modified due to STM8AL 
 *
 *********************************************************************/

/** \file lin_version_control.h
 */

#ifndef LIN_VERSION_CONTROL_H
  #define LIN_VERSION_CONTROL_H

/**********************************************************************
 *  List with all valid versions of c files
 **********************************************************************/

#define LIN_TIMER_C_MAJOR_VERSION_EXPECTED                  1
#define LIN_TIMER_C_MINOR_VERSION_EXPECTED                  31
#define LIN_TIMER_C_PATCH_VERSION_EXPECTED                  2

#define LIN_SLAVE_C_MAJOR_VERSION_EXPECTED                  1
#define LIN_SLAVE_C_MINOR_VERSION_EXPECTED                  33
#define LIN_SLAVE_C_PATCH_VERSION_EXPECTED                  1

#define LIN_MASTER_C_MAJOR_VERSION_EXPECTED                 1
#define LIN_MASTER_C_MINOR_VERSION_EXPECTED                 31
#define LIN_MASTER_C_PATCH_VERSION_EXPECTED                 1

#define LIN_GENERAL_C_MAJOR_VERSION_EXPECTED                1
#define LIN_GENERAL_C_MINOR_VERSION_EXPECTED                32
#define LIN_GENERAL_C_PATCH_VERSION_EXPECTED                2

#define LIN_DIAG_C_MAJOR_VERSION_EXPECTED                   1
#define LIN_DIAG_C_MINOR_VERSION_EXPECTED                   32
#define LIN_DIAG_C_PATCH_VERSION_EXPECTED                   2

#define LIN_DIAG_MASTER_C_MAJOR_VERSION_EXPECTED            1
#define LIN_DIAG_MASTER_C_MINOR_VERSION_EXPECTED            31
#define LIN_DIAG_MASTER_C_PATCH_VERSION_EXPECTED            1

#define LIN_DIAG_SLAVE_C_MAJOR_VERSION_EXPECTED             1
#define LIN_DIAG_SLAVE_C_MINOR_VERSION_EXPECTED             33
#define LIN_DIAG_SLAVE_C_PATCH_VERSION_EXPECTED             1

#define LIN_DEF_C_MAJOR_VERSION_EXPECTED                    1
#define LIN_DEF_C_MINOR_VERSION_EXPECTED                    31
#define LIN_DEF_C_PATCH_VERSION_EXPECTED                    1

#define LIN_CFG_C_MAJOR_VERSION_EXPECTED                    3
#define LIN_CFG_C_MINOR_VERSION_EXPECTED                    5
#define LIN_CFG_C_PATCH_VERSION_EXPECTED                    0


#define LIN_STM8_C_MAJOR_VERSION_EXPECTED               		1
#define LIN_STM8_C_MINOR_VERSION_EXPECTED               		33
#define LIN_STM8_C_PATCH_VERSION_EXPECTED               		4

#define LIN_STM8L_C_MAJOR_VERSION_EXPECTED               		1
#define LIN_STM8L_C_MINOR_VERSION_EXPECTED               		32
#define LIN_STM8L_C_PATCH_VERSION_EXPECTED               		4

/**********************************************************************
 *  List with all valid versions of h files
 **********************************************************************/

#define LIN_H_MAJOR_VERSION_EXPECTED                        1
#define LIN_H_MINOR_VERSION_EXPECTED                        31
#define LIN_H_PATCH_VERSION_EXPECTED                        1

#define LIN_TIMER_H_MAJOR_VERSION_EXPECTED                  1
#define LIN_TIMER_H_MINOR_VERSION_EXPECTED                  31
#define LIN_TIMER_H_PATCH_VERSION_EXPECTED                  2

#define LIN_SLAVE_H_MAJOR_VERSION_EXPECTED                  1
#define LIN_SLAVE_H_MINOR_VERSION_EXPECTED                  31
#define LIN_SLAVE_H_PATCH_VERSION_EXPECTED                  1

#define LIN_MASTER_H_MAJOR_VERSION_EXPECTED                 1
#define LIN_MASTER_H_MINOR_VERSION_EXPECTED                 31
#define LIN_MASTER_H_PATCH_VERSION_EXPECTED                 1

#define LIN_GENERAL_H_MAJOR_VERSION_EXPECTED                1
#define LIN_GENERAL_H_MINOR_VERSION_EXPECTED                32
#define LIN_GENERAL_H_PATCH_VERSION_EXPECTED                1

#define LIN_TYPES_H_MAJOR_VERSION_EXPECTED                  1
#define LIN_TYPES_H_MINOR_VERSION_EXPECTED                  31
#define LIN_TYPES_H_PATCH_VERSION_EXPECTED                  1

#define LIN_DIAG_H_MAJOR_VERSION_EXPECTED                   1
#define LIN_DIAG_H_MINOR_VERSION_EXPECTED                   31
#define LIN_DIAG_H_PATCH_VERSION_EXPECTED                   2

#define LIN_DIAG_API_H_MAJOR_VERSION_EXPECTED               1
#define LIN_DIAG_API_H_MINOR_VERSION_EXPECTED               31
#define LIN_DIAG_API_H_PATCH_VERSION_EXPECTED               1

#define LIN_DIAG_MASTER_H_MAJOR_VERSION_EXPECTED            1
#define LIN_DIAG_MASTER_H_MINOR_VERSION_EXPECTED            31
#define LIN_DIAG_MASTER_H_PATCH_VERSION_EXPECTED            1

#define LIN_DIAG_SLAVE_H_MAJOR_VERSION_EXPECTED             1
#define LIN_DIAG_SLAVE_H_MINOR_VERSION_EXPECTED             32
#define LIN_DIAG_SLAVE_H_PATCH_VERSION_EXPECTED             1

#define LIN_CFG_H_MAJOR_VERSION_EXPECTED                    3
#define LIN_CFG_H_MINOR_VERSION_EXPECTED                    5
#define LIN_CFG_H_PATCH_VERSION_EXPECTED                    0

#define LIN_CFG_TYPES_H_MAJOR_VERSION_EXPECTED              3
#define LIN_CFG_TYPES_H_MINOR_VERSION_EXPECTED              5
#define LIN_CFG_TYPES_H_PATCH_VERSION_EXPECTED              0

#define LIN_ARCH_INCLUDE_H_MAJOR_VERSION_EXPECTED           1
#define LIN_ARCH_INCLUDE_H_MINOR_VERSION_EXPECTED           32
#define LIN_ARCH_INCLUDE_H_PATCH_VERSION_EXPECTED           1

#define LIN_STM8_H_MAJOR_VERSION_EXPECTED               1
#define LIN_STM8_H_MINOR_VERSION_EXPECTED               32
#define LIN_STM8_H_PATCH_VERSION_EXPECTED               1

#define LIN_STM8L_H_MAJOR_VERSION_EXPECTED               1
#define LIN_STM8L_H_MINOR_VERSION_EXPECTED               32
#define LIN_STM8L_H_PATCH_VERSION_EXPECTED               1

#define LIN_DEF_H_MAJOR_VERSION_EXPECTED                    1
#define LIN_DEF_H_MINOR_VERSION_EXPECTED                    31
#define LIN_DEF_H_PATCH_VERSION_EXPECTED                    1

#define LIN_DEF_GEN_H_MAJOR_VERSION_EXPECTED                1
#define LIN_DEF_GEN_H_MINOR_VERSION_EXPECTED                31
#define LIN_DEF_GEN_H_PATCH_VERSION_EXPECTED                1

#define LIN_DEF_ARCH_INCLUDE_H_MAJOR_VERSION_EXPECTED       1
#define LIN_DEF_ARCH_INCLUDE_H_MINOR_VERSION_EXPECTED       32
#define LIN_DEF_ARCH_INCLUDE_H_PATCH_VERSION_EXPECTED       1

#define LIN_DEF_STM8_H_MAJOR_VERSION_EXPECTED           1
#define LIN_DEF_STM8_H_MINOR_VERSION_EXPECTED           32
#define LIN_DEF_STM8_H_PATCH_VERSION_EXPECTED           1

#define LIN_DEF_STM8_GEN_H_MAJOR_VERSION_EXPECTED       1
#define LIN_DEF_STM8_GEN_H_MINOR_VERSION_EXPECTED       31
#define LIN_DEF_STM8_GEN_H_PATCH_VERSION_EXPECTED       1

#define LIN_DEF_STM8L_H_MAJOR_VERSION_EXPECTED           1
#define LIN_DEF_STM8L_H_MINOR_VERSION_EXPECTED           32
#define LIN_DEF_STM8L_H_PATCH_VERSION_EXPECTED           1

#define LIN_DEF_STM8L_GEN_H_MAJOR_VERSION_EXPECTED       1
#define LIN_DEF_STM8L_GEN_H_MINOR_VERSION_EXPECTED       32
#define LIN_DEF_STM8L_GEN_H_PATCH_VERSION_EXPECTED       1
/**********************************************************************
 *  Prepare check
 **********************************************************************/

#undef H_CHECKED
#undef C_CHECKED


/**********************************************************************
 *  First check the c file
 **********************************************************************/

#ifdef LIN_TIMER_C
  #define C_CHECKED
  #if ((LIN_TIMER_C_MAJOR_VERSION != LIN_TIMER_C_MAJOR_VERSION_EXPECTED) ||      \
       (LIN_TIMER_C_MINOR_VERSION != LIN_TIMER_C_MINOR_VERSION_EXPECTED) ||      \
       (LIN_TIMER_C_PATCH_VERSION != LIN_TIMER_C_PATCH_VERSION_EXPECTED)   )
    #error "Version conflict in file lin_timer.c!"
  #endif
  #ifndef LIN_TIMER_C_STM
    #error "Version conflict in lin_timer.c: STM tag not defined"
  #endif
#endif

#ifdef LIN_SLAVE_C
  #ifdef C_CHECKED
    #error "lin_slave.c: a c-file is probably included, this is not allowed!"
  #else
    #define C_CHECKED
  #endif
  #if ((LIN_SLAVE_C_MAJOR_VERSION != LIN_SLAVE_C_MAJOR_VERSION_EXPECTED) ||      \
       (LIN_SLAVE_C_MINOR_VERSION != LIN_SLAVE_C_MINOR_VERSION_EXPECTED) ||      \
       (LIN_SLAVE_C_PATCH_VERSION != LIN_SLAVE_C_PATCH_VERSION_EXPECTED)   )
    #error "Version conflict in file lin_slave.c!"
  #endif
  #ifndef LIN_SLAVE_C_STM
    #error "Version conflict in lin_slave.c: STM tag not defined"
  #endif
#endif

#ifdef LIN_MASTER_C
  #ifdef C_CHECKED
    #error "lin_master.c: a c-file is probably included, this is not allowed!"
  #else
    #define C_CHECKED
  #endif
  #if ((LIN_MASTER_C_MAJOR_VERSION != LIN_MASTER_C_MAJOR_VERSION_EXPECTED) ||      \
       (LIN_MASTER_C_MINOR_VERSION != LIN_MASTER_C_MINOR_VERSION_EXPECTED) ||      \
       (LIN_MASTER_C_PATCH_VERSION != LIN_MASTER_C_PATCH_VERSION_EXPECTED)   )
    #error "Version conflict in file lin_master.c!"
  #endif
  #ifndef LIN_MASTER_C_STM
    #error "Version conflict in lin_master.c: STM tag not defined"
  #endif
#endif

#ifdef LIN_GENERAL_C
  #ifdef C_CHECKED
    #error "lin_general.c: a c-file is probably included, this is not allowed!"
  #else
    #define C_CHECKED
  #endif
  #if ((LIN_GENERAL_C_MAJOR_VERSION != LIN_GENERAL_C_MAJOR_VERSION_EXPECTED) ||      \
       (LIN_GENERAL_C_MINOR_VERSION != LIN_GENERAL_C_MINOR_VERSION_EXPECTED) ||      \
       (LIN_GENERAL_C_PATCH_VERSION != LIN_GENERAL_C_PATCH_VERSION_EXPECTED)   )
    #error "Version conflict in file lin_general.c!"
  #endif
  #ifndef LIN_GENERAL_C_STM
    #error "Version conflict in lin_general.c: STM tag not defined"
  #endif
#endif

#ifdef LIN_DIAG_C
  #ifdef C_CHECKED
    #error "lin_diag.c: a c-file is probably included, this is not allowed!"
  #else
    #define C_CHECKED
  #endif
  #if ((LIN_DIAG_C_MAJOR_VERSION != LIN_DIAG_C_MAJOR_VERSION_EXPECTED) ||      \
       (LIN_DIAG_C_MINOR_VERSION != LIN_DIAG_C_MINOR_VERSION_EXPECTED) ||      \
       (LIN_DIAG_C_PATCH_VERSION != LIN_DIAG_C_PATCH_VERSION_EXPECTED)   )
    #error "Version conflict in file lin_diag.c!"
  #endif
  #ifndef LIN_DIAG_C_STM
    #error "Version conflict in lin_diag.c: STM tag not defined"
  #endif
#endif

#ifdef LIN_DIAG_MASTER_C
  #ifdef C_CHECKED
    #error "lin_diag_master.c: a c-file is probably included, this is not allowed!"
  #else
    #define C_CHECKED
  #endif
  #if ((LIN_DIAG_MASTER_C_MAJOR_VERSION != LIN_DIAG_MASTER_C_MAJOR_VERSION_EXPECTED) ||      \
       (LIN_DIAG_MASTER_C_MINOR_VERSION != LIN_DIAG_MASTER_C_MINOR_VERSION_EXPECTED) ||      \
       (LIN_DIAG_MASTER_C_PATCH_VERSION != LIN_DIAG_MASTER_C_PATCH_VERSION_EXPECTED)   )
    #error "Version conflict in file lin_diag_master.c!"
  #endif
  #ifndef LIN_DIAG_MASTER_C_STM
    #error "Version conflict in lin_diag_master.c: STM tag not defined"
  #endif
#endif

#ifdef LIN_DIAG_SLAVE_C
  #ifdef C_CHECKED
    #error "lin_diag_slave.c: a c-file is probably included, this is not allowed!"
  #else
    #define C_CHECKED
  #endif
  #if ((LIN_DIAG_SLAVE_C_MAJOR_VERSION != LIN_DIAG_SLAVE_C_MAJOR_VERSION_EXPECTED) ||      \
       (LIN_DIAG_SLAVE_C_MINOR_VERSION != LIN_DIAG_SLAVE_C_MINOR_VERSION_EXPECTED) ||      \
       (LIN_DIAG_SLAVE_C_PATCH_VERSION != LIN_DIAG_SLAVE_C_PATCH_VERSION_EXPECTED)   )
    #error "Version conflict in file lin_diag_slave.c!"
  #endif
  #ifndef LIN_DIAG_SLAVE_C_STM
    #error "Version conflict in lin_diag_slave.c: STM tag not defined"
  #endif
#endif

#ifdef LIN_DEF_C
  #ifdef C_CHECKED
    #error "lin_def.c: a c-file is probably included, this is not allowed!"
  #else
    #define C_CHECKED
  #endif
  #if ((LIN_DEF_C_MAJOR_VERSION != LIN_DEF_C_MAJOR_VERSION_EXPECTED) ||      \
       (LIN_DEF_C_MINOR_VERSION != LIN_DEF_C_MINOR_VERSION_EXPECTED) ||      \
       (LIN_DEF_C_PATCH_VERSION != LIN_DEF_C_PATCH_VERSION_EXPECTED)   )
    #error "Version conflict in file lin_def.c!"
  #endif
  #ifndef LIN_DEF_C_STM
    #error "Version conflict in lin_def.c: STM tag not defined"
  #endif
#endif

#ifdef LIN_CFG_C
  #ifdef C_CHECKED
    #error "lin_cfg.c: a c-file is probably included, this is not allowed!"
  #else
    #define C_CHECKED
  #endif
  #if ((LIN_CFG_C_MAJOR_VERSION != LIN_CFG_C_MAJOR_VERSION_EXPECTED) ||      \
       (LIN_CFG_C_MINOR_VERSION != LIN_CFG_C_MINOR_VERSION_EXPECTED) ||      \
       (LIN_CFG_C_PATCH_VERSION != LIN_CFG_C_PATCH_VERSION_EXPECTED)   )
    #error "Version conflict in file lin_cfg.c!"
  #endif
  #ifndef LIN_CFG_C_STM
    #error "Version conflict in lin_cfg.c: STM tag not defined"
  #endif
#endif

#ifdef LIN_STM8_C
  #ifdef C_CHECKED
    #error "lin_stm8.c: a c-file is probably included, this is not allowed!"
  #else
    #define C_CHECKED
  #endif
  #if ((LIN_STM8_C_MAJOR_VERSION != LIN_STM8_C_MAJOR_VERSION_EXPECTED) ||      \
       (LIN_STM8_C_MINOR_VERSION != LIN_STM8_C_MINOR_VERSION_EXPECTED) ||      \
       (LIN_STM8_C_PATCH_VERSION != LIN_STM8_C_PATCH_VERSION_EXPECTED)   )
    #error "Version conflict in file lin_stm8.c!"
  #endif
  #ifndef LIN_STM8_C_STM
    #error "Version conflict in lin_stm8.c: STM tag not defined"
  #endif
#endif

#ifdef LIN_STM8L_C
  #ifdef C_CHECKED
    #error "lin_stm8L.c: a c-file is probably included, this is not allowed!"
  #else
    #define C_CHECKED
  #endif
  #if ((LIN_STM8L_C_MAJOR_VERSION != LIN_STM8L_C_MAJOR_VERSION_EXPECTED) ||      \
       (LIN_STM8L_C_MINOR_VERSION != LIN_STM8L_C_MINOR_VERSION_EXPECTED) ||      \
       (LIN_STM8L_C_PATCH_VERSION != LIN_STM8L_C_PATCH_VERSION_EXPECTED)   )
    #error "Version conflict in file lin_stm8L.c!"
  #endif
  #ifndef LIN_STM8L_C_STM
    #error "Version conflict in lin_stm8L.c: STM tag not defined"
  #endif
#endif

#ifndef C_CHECKED
  #error "No c-file identifier <LIN_FILE_C> found!"
#endif


/**********************************************************************
 *  Then check the h-files
 **********************************************************************/

#ifdef LIN_H
  #ifndef H_CHECKED
    #define H_CHECKED
  #endif
  #if ((LIN_H_MAJOR_VERSION != LIN_H_MAJOR_VERSION_EXPECTED) ||          \
       (LIN_H_MINOR_VERSION != LIN_H_MINOR_VERSION_EXPECTED) ||          \
       (LIN_H_PATCH_VERSION != LIN_H_PATCH_VERSION_EXPECTED)   )
    #error "Version conflict in file lin.h!"
  #endif
  #ifndef LIN_H_STM
    #error "Version conflict in lin.h: STM tag not defined"
  #endif
#endif

#ifdef LIN_TIMER_H
  #ifndef H_CHECKED
    #define H_CHECKED
  #endif
  #if ((LIN_TIMER_H_MAJOR_VERSION != LIN_TIMER_H_MAJOR_VERSION_EXPECTED) ||          \
       (LIN_TIMER_H_MINOR_VERSION != LIN_TIMER_H_MINOR_VERSION_EXPECTED) ||          \
       (LIN_TIMER_H_PATCH_VERSION != LIN_TIMER_H_PATCH_VERSION_EXPECTED)   )
    #error "Version conflict in file lin_timer.h!"
  #endif
  #ifndef LIN_TIMER_H_STM
    #error "Version conflict in lin_timer.h: STM tag not defined"
  #endif
#endif

#ifdef LIN_SLAVE_H
  #ifndef H_CHECKED
    #define H_CHECKED
  #endif
  #if ((LIN_SLAVE_H_MAJOR_VERSION != LIN_SLAVE_H_MAJOR_VERSION_EXPECTED) ||          \
       (LIN_SLAVE_H_MINOR_VERSION != LIN_SLAVE_H_MINOR_VERSION_EXPECTED) ||          \
       (LIN_SLAVE_H_PATCH_VERSION != LIN_SLAVE_H_PATCH_VERSION_EXPECTED)   )
    #error "Version conflict in file lin_slave.h!"
  #endif
  #ifndef LIN_SLAVE_H_STM
    #error "Version conflict in lin_slave.h: STM tag not defined"
  #endif
#endif

#ifdef LIN_MASTER_H
  #ifndef H_CHECKED
    #define H_CHECKED
  #endif
  #if ((LIN_MASTER_H_MAJOR_VERSION != LIN_MASTER_H_MAJOR_VERSION_EXPECTED) ||          \
       (LIN_MASTER_H_MINOR_VERSION != LIN_MASTER_H_MINOR_VERSION_EXPECTED) ||          \
       (LIN_MASTER_H_PATCH_VERSION != LIN_MASTER_H_PATCH_VERSION_EXPECTED)   )
    #error "Version conflict in file lin_master.h!"
  #endif
  #ifndef LIN_MASTER_H_STM
    #error "Version conflict in lin_master.h: STM tag not defined"
  #endif
#endif

#ifdef LIN_GENERAL_H
  #ifndef H_CHECKED
    #define H_CHECKED
  #endif
  #if ((LIN_GENERAL_H_MAJOR_VERSION != LIN_GENERAL_H_MAJOR_VERSION_EXPECTED) ||          \
       (LIN_GENERAL_H_MINOR_VERSION != LIN_GENERAL_H_MINOR_VERSION_EXPECTED) ||          \
       (LIN_GENERAL_H_PATCH_VERSION != LIN_GENERAL_H_PATCH_VERSION_EXPECTED)   )
    #error "Version conflict in file lin_general.h!"
  #endif
  #ifndef LIN_GENERAL_H_STM
    #error "Version conflict in lin_general.h: STM tag not defined"
  #endif
#endif

#ifdef LIN_TYPES_H
  #ifndef H_CHECKED
    #define H_CHECKED
  #endif
  #if ((LIN_TYPES_H_MAJOR_VERSION != LIN_TYPES_H_MAJOR_VERSION_EXPECTED) ||          \
       (LIN_TYPES_H_MINOR_VERSION != LIN_TYPES_H_MINOR_VERSION_EXPECTED) ||          \
       (LIN_TYPES_H_PATCH_VERSION != LIN_TYPES_H_PATCH_VERSION_EXPECTED)   )
    #error "Version conflict in file lin_types.h!"
  #endif
  #ifndef LIN_TYPES_H_STM
    #error "Version conflict in lin_types.h: STM tag not defined"
  #endif
#endif

#ifdef LIN_DIAG_H
  #ifndef H_CHECKED
    #define H_CHECKED
  #endif
  #if ((LIN_DIAG_H_MAJOR_VERSION != LIN_DIAG_H_MAJOR_VERSION_EXPECTED) ||          \
       (LIN_DIAG_H_MINOR_VERSION != LIN_DIAG_H_MINOR_VERSION_EXPECTED) ||          \
       (LIN_DIAG_H_PATCH_VERSION != LIN_DIAG_H_PATCH_VERSION_EXPECTED)   )
    #error "Version conflict in file lin_diag.h!"
  #endif
  #ifndef LIN_DIAG_H_STM
    #error "Version conflict in lin_diag.h: STM tag not defined"
  #endif
#endif

#ifdef LIN_DIAG_API_H
  #ifndef H_CHECKED
    #define H_CHECKED
  #endif
  #if ((LIN_DIAG_API_H_MAJOR_VERSION != LIN_DIAG_API_H_MAJOR_VERSION_EXPECTED) ||          \
       (LIN_DIAG_API_H_MINOR_VERSION != LIN_DIAG_API_H_MINOR_VERSION_EXPECTED) ||          \
       (LIN_DIAG_API_H_PATCH_VERSION != LIN_DIAG_API_H_PATCH_VERSION_EXPECTED)   )
    #error "Version conflict in file lin_diag_api.h!"
  #endif
  #ifndef LIN_DIAG_API_H_STM
    #error "Version conflict in lin_diag_api.h: STM tag not defined"
  #endif
#endif

#ifdef LIN_DIAG_MASTER_H
  #ifndef H_CHECKED
    #define H_CHECKED
  #endif
  #if ((LIN_DIAG_MASTER_H_MAJOR_VERSION != LIN_DIAG_MASTER_H_MAJOR_VERSION_EXPECTED) ||          \
       (LIN_DIAG_MASTER_H_MINOR_VERSION != LIN_DIAG_MASTER_H_MINOR_VERSION_EXPECTED) ||          \
       (LIN_DIAG_MASTER_H_PATCH_VERSION != LIN_DIAG_MASTER_H_PATCH_VERSION_EXPECTED)   )
    #error "Version conflict in file lin_diag_master.h!"
  #endif
  #ifndef LIN_DIAG_MASTER_H_STM
    #error "Version conflict in lin_diag_master.h: STM tag not defined"
  #endif
#endif

#ifdef LIN_DIAG_SLAVE_H
  #ifndef H_CHECKED
    #define H_CHECKED
  #endif
  #if ((LIN_DIAG_SLAVE_H_MAJOR_VERSION != LIN_DIAG_SLAVE_H_MAJOR_VERSION_EXPECTED) ||          \
       (LIN_DIAG_SLAVE_H_MINOR_VERSION != LIN_DIAG_SLAVE_H_MINOR_VERSION_EXPECTED) ||          \
       (LIN_DIAG_SLAVE_H_PATCH_VERSION != LIN_DIAG_SLAVE_H_PATCH_VERSION_EXPECTED)   )
    #error "Version conflict in file lin_diag_slave.h!"
  #endif
  #ifndef LIN_DIAG_SLAVE_H_STM
    #error "Version conflict in lin_diag_slave.h: STM tag not defined"
  #endif
#endif

#ifdef LIN_CFG_H
  #ifndef H_CHECKED
    #define H_CHECKED
  #endif
  #if ((LIN_CFG_H_MAJOR_VERSION != LIN_CFG_H_MAJOR_VERSION_EXPECTED) ||          \
       (LIN_CFG_H_MINOR_VERSION != LIN_CFG_H_MINOR_VERSION_EXPECTED) ||          \
       (LIN_CFG_H_PATCH_VERSION != LIN_CFG_H_PATCH_VERSION_EXPECTED)   )
    #error "Version conflict in file lin_cfg.h!"
  #endif
  #ifndef LIN_CFG_H_STM
    #error "Version conflict in lin_cfg.h: STM tag not defined"
  #endif
#endif

#ifdef LIN_CFG_TYPES_H
  #ifndef H_CHECKED
    #define H_CHECKED
  #endif
  #if ((LIN_CFG_TYPES_H_MAJOR_VERSION != LIN_CFG_TYPES_H_MAJOR_VERSION_EXPECTED) ||          \
       (LIN_CFG_TYPES_H_MINOR_VERSION != LIN_CFG_TYPES_H_MINOR_VERSION_EXPECTED) ||          \
       (LIN_CFG_TYPES_H_PATCH_VERSION != LIN_CFG_TYPES_H_PATCH_VERSION_EXPECTED)   )
    #error "Version conflict in file lin_cfg_types.h!"
  #endif
  #ifndef LIN_CFG_TYPES_H_STM
    #error "Version conflict in lin_cfg_types.h: STM tag not defined"
  #endif
#endif

#ifdef LIN_ARCH_INCLUDE_H
  #ifndef H_CHECKED
    #define H_CHECKED
  #endif
  #if ((LIN_ARCH_INCLUDE_H_MAJOR_VERSION != LIN_ARCH_INCLUDE_H_MAJOR_VERSION_EXPECTED) ||          \
       (LIN_ARCH_INCLUDE_H_MINOR_VERSION != LIN_ARCH_INCLUDE_H_MINOR_VERSION_EXPECTED) ||          \
       (LIN_ARCH_INCLUDE_H_PATCH_VERSION != LIN_ARCH_INCLUDE_H_PATCH_VERSION_EXPECTED)   )
    #error "Version conflict in file lin_arch_include.h!"
  #endif
  #ifndef LIN_ARCH_INCLUDE_H_STM
    #error "Version conflict in lin_arch_include.h: STM tag not defined"
  #endif
#endif

#ifdef LIN_STM8_H
  #ifndef H_CHECKED
    #define H_CHECKED
  #endif
  #if ((LIN_STM8_H_MAJOR_VERSION != LIN_STM8_H_MAJOR_VERSION_EXPECTED) ||          \
       (LIN_STM8_H_MINOR_VERSION != LIN_STM8_H_MINOR_VERSION_EXPECTED) ||          \
       (LIN_STM8_H_PATCH_VERSION != LIN_STM8_H_PATCH_VERSION_EXPECTED)   )
    #error "Version conflict in file lin_stm8.h!"
  #endif
  #ifndef LIN_STM8_H_STM
    #error "Version conflict in lin_stm8.h: STM tag not defined"
  #endif
#endif

#ifdef LIN_STM8L_H
  #ifndef H_CHECKED
    #define H_CHECKED
  #endif
  #if ((LIN_STM8L_H_MAJOR_VERSION != LIN_STM8L_H_MAJOR_VERSION_EXPECTED) ||          \
       (LIN_STM8L_H_MINOR_VERSION != LIN_STM8L_H_MINOR_VERSION_EXPECTED) ||          \
       (LIN_STM8L_H_PATCH_VERSION != LIN_STM8L_H_PATCH_VERSION_EXPECTED)   )
    #error "Version conflict in file lin_stm8L.h!"
  #endif
  #ifndef LIN_STM8L_H_STM
    #error "Version conflict in lin_stm8L.h: STM tag not defined"
  #endif
#endif

#ifdef LIN_DEF_H
  #ifndef H_CHECKED
    #define H_CHECKED
  #endif
  #if ((LIN_DEF_H_MAJOR_VERSION != LIN_DEF_H_MAJOR_VERSION_EXPECTED) ||          \
       (LIN_DEF_H_MINOR_VERSION != LIN_DEF_H_MINOR_VERSION_EXPECTED) ||          \
       (LIN_DEF_H_PATCH_VERSION != LIN_DEF_H_PATCH_VERSION_EXPECTED)   )
    #error "Version conflict in file lin_def.h!"
  #endif
  #ifndef LIN_DEF_H_STM
    #error "Version conflict in lin_def.h: STM tag not defined"
  #endif
#endif

#ifdef LIN_DEF_GEN_H
  #ifndef H_CHECKED
    #define H_CHECKED
  #endif
  #if ((LIN_DEF_GEN_H_MAJOR_VERSION != LIN_DEF_GEN_H_MAJOR_VERSION_EXPECTED) ||          \
       (LIN_DEF_GEN_H_MINOR_VERSION != LIN_DEF_GEN_H_MINOR_VERSION_EXPECTED) ||          \
       (LIN_DEF_GEN_H_PATCH_VERSION != LIN_DEF_GEN_H_PATCH_VERSION_EXPECTED) )
    #error "Version conflict in file lin_def_gen.h!"
  #endif
  #ifndef LIN_DEF_GEN_H_STM
    #error "Version conflict in lin_def_gen.h: STM tag not defined"
  #endif
#endif

#ifdef LIN_DEF_ARCH_INCLUDE_H
  #ifndef H_CHECKED
    #define H_CHECKED
  #endif
  #if ((LIN_DEF_ARCH_INCLUDE_H_MAJOR_VERSION != LIN_DEF_ARCH_INCLUDE_H_MAJOR_VERSION_EXPECTED) ||          \
       (LIN_DEF_ARCH_INCLUDE_H_MINOR_VERSION != LIN_DEF_ARCH_INCLUDE_H_MINOR_VERSION_EXPECTED) ||          \
       (LIN_DEF_ARCH_INCLUDE_H_PATCH_VERSION != LIN_DEF_ARCH_INCLUDE_H_PATCH_VERSION_EXPECTED)   )
    #error "Version conflict in file lin_def_arch_include.h!"
  #endif
  #ifndef LIN_DEF_ARCH_INCLUDE_H_STM
    #error "Version conflict in lin_def_arch_include.h: STM tag not defined"
  #endif
#endif

#ifdef LIN_DEF_STM8_H
  #ifndef H_CHECKED
    #define H_CHECKED
  #endif
  #if ((LIN_DEF_STM8_H_MAJOR_VERSION != LIN_DEF_STM8_H_MAJOR_VERSION_EXPECTED) ||          \
       (LIN_DEF_STM8_H_MINOR_VERSION != LIN_DEF_STM8_H_MINOR_VERSION_EXPECTED) ||          \
       (LIN_DEF_STM8_H_PATCH_VERSION != LIN_DEF_STM8_H_PATCH_VERSION_EXPECTED)   )
    #error "Version conflict in file lin_def_stm8.h!"
  #endif
  #ifndef LIN_DEF_STM8_H_STM
    #error "Version conflict in lin_def_stm8.h: STM tag not defined"
  #endif
#endif

#ifdef LIN_DEF_STM8_GEN_H
  #ifndef H_CHECKED
    #define H_CHECKED
  #endif
  #if ((LIN_DEF_STM8_GEN_H_MAJOR_VERSION != LIN_DEF_STM8_GEN_H_MAJOR_VERSION_EXPECTED) ||          \
       (LIN_DEF_STM8_GEN_H_MINOR_VERSION != LIN_DEF_STM8_GEN_H_MINOR_VERSION_EXPECTED) ||          \
       (LIN_DEF_STM8_GEN_H_PATCH_VERSION != LIN_DEF_STM8_GEN_H_PATCH_VERSION_EXPECTED)   )
    #error "Version conflict in file lin_def_stm8_gen.h!"
  #endif
  #ifndef LIN_DEF_STM8_GEN_H_STM
    #error "Version conflict in lin_def_stm8_gen.h: STM tag not defined"
  #endif
#endif

#ifdef LIN_DEF_STM8L_H
  #ifndef H_CHECKED
    #define H_CHECKED
  #endif
  #if ((LIN_DEF_STM8L_H_MAJOR_VERSION != LIN_DEF_STM8L_H_MAJOR_VERSION_EXPECTED) ||          \
       (LIN_DEF_STM8L_H_MINOR_VERSION != LIN_DEF_STM8L_H_MINOR_VERSION_EXPECTED) ||          \
       (LIN_DEF_STM8L_H_PATCH_VERSION != LIN_DEF_STM8L_H_PATCH_VERSION_EXPECTED)   )
    #error "Version conflict in file lin_def_stm8L.h!"
  #endif
  #ifndef LIN_DEF_STM8L_H_STM
    #error "Version conflict in lin_def_stm8.h: STM tag not defined"
  #endif
#endif

#ifdef LIN_DEF_STM8L_GEN_H
  #ifndef H_CHECKED
    #define H_CHECKED
  #endif
  #if ((LIN_DEF_STM8L_GEN_H_MAJOR_VERSION != LIN_DEF_STM8L_GEN_H_MAJOR_VERSION_EXPECTED) ||          \
       (LIN_DEF_STM8L_GEN_H_MINOR_VERSION != LIN_DEF_STM8L_GEN_H_MINOR_VERSION_EXPECTED) ||          \
       (LIN_DEF_STM8L_GEN_H_PATCH_VERSION != LIN_DEF_STM8L_GEN_H_PATCH_VERSION_EXPECTED)   )
    #error "Version conflict in file lin_def_stm8_gen.h!"
  #endif
  #ifndef LIN_DEF_STM8L_GEN_H_STM
    #error "Version conflict in lin_def_stm8L_gen.h: STM tag not defined"
  #endif
#endif

#ifndef H_CHECKED
  #error "No h-file checked!"
#endif


#endif /*  LIN_VERSION_CONTROL_H */









