/*********************************************************************
 *
 * Project:      LIN 2.1 driver suite
 *
 * File name:    lin_def_stm8_gen.h
 *
 * Copyright:    (c) STMicroelectronics
 *
 * Author:       Jatin KHURANA
 *
 * Description:  Hardware specific configuration and settings for
 *               STM8L
 *
 * Target:       STM8AL
 *
 * Names:        jk  Jatin Khurana  STMicroelectronics
 *
 * TO BE CHANGED BY THE USER
 *
 *********************************************************************/

/*********************************************************************
 *
 * History:
 *
 * 20071213  v1.0   gsf  - Initial version for STM8
 *
 *********************************************************************/

/** \file lin_def_stm8_gen.h
 */

#ifndef LIN_DEF_STM8L_GEN_H
  #define LIN_DEF_STM8L_GEN_H

#include "lin_cfg_types.h" 
#include "lin_def.h"
#include "lin_def_gen.h"


#define LIN_DEF_STM8L_GEN_H_MAJOR_VERSION  1
#define LIN_DEF_STM8L_GEN_H_MINOR_VERSION  32
#define LIN_DEF_STM8L_GEN_H_PATCH_VERSION  1

#define LIN_DEF_STM8L_GEN_H_STM

/*********************************************************************
 * includes
 *********************************************************************/

#ifdef LIN_VERSION_CONTROL_H
  #error "lin_version_control.h shall not be included in h-files and shall be the last file to be included in a c-file!"
#endif

/*********************************************************************
 *
 * The LIN driver expects a real stack memory model for all functions
 * If a memory model is used that implements a non standard behaviour
 * for function calls, adjust the STACK macro to the token that
 * must be written before each function to guarantee use of real
 * stack for that function
 * For the COSMIC compiler this is @stack
 *
 *********************************************************************/
#ifdef __COSMIC__
	#define STACK    @stack
#elif __RAISONANCE__
	#define STACK    
#elif __IAR__
	#define STACK	
#else
	#error "This LIN driver is developed for the COSMIC, RAISONANCE and IAR compilers only"
#endif

/*********************************************************************
 * post function tag, currently void for STM8
 *********************************************************************/
#define REENTRANT

/*********************************************************************
 * COSMIC linker needs at least one symbol per file
 *********************************************************************/
#define LIN_NEED_LIB_PLACEBO

/*********************************************************************
 * definition of wakeup baudrate
 *********************************************************************/
#define LIN_WAKEUP_BAUDRATE                              20000

/*********************************************************************
 * global typedefs, demanded by the LIN standard
 *
 * these types cannot be defined within lin_types.h because
 * the length of the integer types is hardware dependent
 *********************************************************************/
typedef unsigned char  l_bool;
typedef unsigned char  l_u8;
typedef unsigned short l_u16;
typedef unsigned long  l_u32;
typedef signed long    l_s32;
typedef l_u16          l_ioctl_op;
typedef l_u16          l_irqmask;

/*********************************************************************
 *
 * definition of the functions for baudrate and protocol callbacks
 *
 * the user must implement the functions declared below for the
 * interface actually used
 *
 *********************************************************************/
#ifdef LIN_BAUDRATE_DETECT
  #ifdef SCI1_USED
    STACK extern void l_baudrateCallback_SCI1(l_u16 Baudrate);
  #endif 
  #ifdef SCI2_USED
    STACK extern void l_baudrateCallback_SCI2(l_u16 Baudrate);
  #endif 
#endif

#ifdef LIN_PROTOCOL_SWITCH
  #ifdef SCI1_USED
    STACK extern void l_protocolCallback_SCI1(void);
  #endif
  #ifdef SCI2_USED
    STACK extern void l_protocolCallback_SCI2(void);
  #endif
#endif

/*********************************************************************
 *
 * internal handling of zero page optimisation
 *
 *********************************************************************/
#ifdef LIN_TX_FLAGS_IN_ZERO_PAGE
  #ifdef __COSMIC__
	#define LIN_ZERO_PAGE11 @tiny
  #elif __RAISONANCE__
	#define LIN_ZERO_PAGE11 page0
  #elif __IAR__
	#define LIN_ZERO_PAGE11 __tiny
  #endif
#endif
#ifdef LIN_RX_FLAGS_IN_ZERO_PAGE
	#ifdef __COSMIC__
		#define LIN_ZERO_PAGE12 @tiny
	#elif __RAISONANCE__
		#define LIN_ZERO_PAGE12 page0
	#elif __IAR__
		#define LIN_ZERO_PAGE12 __tiny	
	#endif
#endif
#ifdef LIN_CHANGED_FLAGS_IN_ZERO_PAGE
	#ifdef __COSMIC__
		#define LIN_ZERO_PAGE13 @tiny
	#elif __RAISONANCE__
		#define LIN_ZERO_PAGE13 page0
	#elif __IAR__
		#define LIN_ZERO_PAGE13 __tiny	
	#endif
#endif
#ifdef LIN_FRAME_BUFFER_IN_ZERO_PAGE
	#ifdef __COSMIC__
		#define LIN_ZERO_PAGE14 @tiny
	#elif __RAISONANCE__
		#define LIN_ZERO_PAGE14 page0
	#elif __IAR__
		#define LIN_ZERO_PAGE14 __tiny	
	#endif
#endif
#ifdef LIN_FRAME_IDS_IN_ZERO_PAGE
	#ifdef __COSMIC__
		#define LIN_ZERO_PAGE15 @tiny
	#elif __RAISONANCE__
		#define LIN_ZERO_PAGE15 page0
	#elif __IAR__
		#define LIN_ZERO_PAGE15 __tiny	
	#endif
#endif

#ifdef LIN_MASTER_NODE
  #if (LIN_ZERO_PAGE_SIZE  >=  20 * LIN_IFC_NR_MAX)
  #ifdef __COSMIC__
    #define LIN_ZERO_PAGE1 @tiny
    #define LIN_ZERO_PAGE2 @tiny
    #define LIN_ZERO_PAGE3 @tiny
    #define LIN_ZERO_PAGE4 @tiny
    #define LIN_ZERO_PAGE5 @tiny
    #define LIN_ZERO_PAGE6 @tiny
    #define LIN_ZERO_PAGE7 @tiny
    #define LIN_ZERO_PAGE10 @tiny
  #elif __RAISONANCE__
	#define LIN_ZERO_PAGE1 page0
    #define LIN_ZERO_PAGE2 page0
    #define LIN_ZERO_PAGE3 page0
    #define LIN_ZERO_PAGE4 page0
    #define LIN_ZERO_PAGE5 page0
    #define LIN_ZERO_PAGE6 page0
    #define LIN_ZERO_PAGE7 page0
    #define LIN_ZERO_PAGE10 page0
  #elif __IAR__
	#define LIN_ZERO_PAGE1 __tiny
    #define LIN_ZERO_PAGE2 __tiny
    #define LIN_ZERO_PAGE3 __tiny
    #define LIN_ZERO_PAGE4 __tiny
    #define LIN_ZERO_PAGE5 __tiny
    #define LIN_ZERO_PAGE6 __tiny
    #define LIN_ZERO_PAGE7 __tiny
    #define LIN_ZERO_PAGE10 __tiny
  #endif
  
  #elif (LIN_ZERO_PAGE_SIZE  >=  19 * LIN_IFC_NR_MAX)
    
	
	#ifdef __COSMIC__
	#define LIN_ZERO_PAGE1 @tiny
    #define LIN_ZERO_PAGE2 @tiny
    #define LIN_ZERO_PAGE3 @tiny
    #define LIN_ZERO_PAGE4 @tiny
    #define LIN_ZERO_PAGE5 @tiny
    #define LIN_ZERO_PAGE6 @tiny
    #define LIN_ZERO_PAGE8 @tiny
    #define LIN_ZERO_PAGE10 @tiny
	#elif __RAISONANCE__
	#define LIN_ZERO_PAGE1 page0
    #define LIN_ZERO_PAGE2 page0
    #define LIN_ZERO_PAGE3 page0
    #define LIN_ZERO_PAGE4 page0
    #define LIN_ZERO_PAGE5 page0
    #define LIN_ZERO_PAGE6 page0
    #define LIN_ZERO_PAGE8 page0
    #define LIN_ZERO_PAGE10 page0
	#elif __IAR__
	#define LIN_ZERO_PAGE1 __tiny
    #define LIN_ZERO_PAGE2 __tiny
    #define LIN_ZERO_PAGE3 __tiny
    #define LIN_ZERO_PAGE4 __tiny
    #define LIN_ZERO_PAGE5 __tiny
    #define LIN_ZERO_PAGE6 __tiny
    #define LIN_ZERO_PAGE8 __tiny
    #define LIN_ZERO_PAGE10 __tiny
	#endif
	
	
	
	
	
  #elif (LIN_ZERO_PAGE_SIZE  >=  18 * LIN_IFC_NR_MAX)
  
  #ifdef __COSMIC__
	#define LIN_ZERO_PAGE1 @tiny
    #define LIN_ZERO_PAGE2 @tiny
    #define LIN_ZERO_PAGE3 @tiny
    #define LIN_ZERO_PAGE4 @tiny
    #define LIN_ZERO_PAGE5 @tiny
    #define LIN_ZERO_PAGE6 @tiny
    #define LIN_ZERO_PAGE10 @tiny
	#elif __RAISONANCE__
	#define LIN_ZERO_PAGE1 page0
    #define LIN_ZERO_PAGE2 page0
    #define LIN_ZERO_PAGE3 page0
    #define LIN_ZERO_PAGE4 page0
    #define LIN_ZERO_PAGE5 page0
    #define LIN_ZERO_PAGE6 page0
    #define LIN_ZERO_PAGE10 page0
	#elif __IAR__
	#define LIN_ZERO_PAGE1 __tiny
    #define LIN_ZERO_PAGE2 __tiny
    #define LIN_ZERO_PAGE3 __tiny
    #define LIN_ZERO_PAGE4 __tiny
    #define LIN_ZERO_PAGE5 __tiny
    #define LIN_ZERO_PAGE6 __tiny
    #define LIN_ZERO_PAGE10 __tiny
	#endif
	
    
  #elif (LIN_ZERO_PAGE_SIZE  >=  17 * LIN_IFC_NR_MAX)
  
  #ifdef __COSMIC__
	#define LIN_ZERO_PAGE1 @tiny
    #define LIN_ZERO_PAGE2 @tiny
    #define LIN_ZERO_PAGE3 @tiny
    #define LIN_ZERO_PAGE4 @tiny
    #define LIN_ZERO_PAGE5 @tiny
    #define LIN_ZERO_PAGE8 @tiny
    #define LIN_ZERO_PAGE10 @tiny
	#elif __RAISONANCE__
	#define LIN_ZERO_PAGE1 page0
    #define LIN_ZERO_PAGE2 page0
    #define LIN_ZERO_PAGE3 page0
    #define LIN_ZERO_PAGE4 page0
    #define LIN_ZERO_PAGE5 page0
    #define LIN_ZERO_PAGE8 page0
    #define LIN_ZERO_PAGE10 page0
	#elif __IAR__
	#define LIN_ZERO_PAGE1 __tiny
    #define LIN_ZERO_PAGE2 __tiny
    #define LIN_ZERO_PAGE3 __tiny
    #define LIN_ZERO_PAGE4 __tiny
    #define LIN_ZERO_PAGE5 __tiny
    #define LIN_ZERO_PAGE8 __tiny
    #define LIN_ZERO_PAGE10 __tiny
	#endif
	
    
  #elif (LIN_ZERO_PAGE_SIZE  >=  16 * LIN_IFC_NR_MAX)
  
  #ifdef __COSMIC__
	#define LIN_ZERO_PAGE1 @tiny
    #define LIN_ZERO_PAGE2 @tiny
    #define LIN_ZERO_PAGE3 @tiny
    #define LIN_ZERO_PAGE4 @tiny
    #define LIN_ZERO_PAGE8 @tiny
    #define LIN_ZERO_PAGE10 @tiny
	#elif __RAISONANCE__
	#define LIN_ZERO_PAGE1 page0
    #define LIN_ZERO_PAGE2 page0
    #define LIN_ZERO_PAGE3 page0
    #define LIN_ZERO_PAGE4 page0
    #define LIN_ZERO_PAGE8 page0
    #define LIN_ZERO_PAGE10 page0
	#elif __IAR__
	#define LIN_ZERO_PAGE1 __tiny
    #define LIN_ZERO_PAGE2 __tiny
    #define LIN_ZERO_PAGE3 __tiny
    #define LIN_ZERO_PAGE4 __tiny
    #define LIN_ZERO_PAGE8 __tiny
    #define LIN_ZERO_PAGE10 __tiny
	#endif
	
    
  #elif (LIN_ZERO_PAGE_SIZE  >=  15 * LIN_IFC_NR_MAX)
  
  #ifdef __COSMIC__
	#define LIN_ZERO_PAGE1 @tiny
    #define LIN_ZERO_PAGE2 @tiny
    #define LIN_ZERO_PAGE3 @tiny
    #define LIN_ZERO_PAGE4 @tiny
    #define LIN_ZERO_PAGE10 @tiny
	#elif __RAISONANCE__
	#define LIN_ZERO_PAGE1 page0
    #define LIN_ZERO_PAGE2 page0
    #define LIN_ZERO_PAGE3 page0
    #define LIN_ZERO_PAGE4 page0
    #define LIN_ZERO_PAGE10 page0
	#elif __IAR__
	#define LIN_ZERO_PAGE1 __tiny
    #define LIN_ZERO_PAGE2 __tiny
    #define LIN_ZERO_PAGE3 __tiny
    #define LIN_ZERO_PAGE4 __tiny
    #define LIN_ZERO_PAGE10 __tiny
	#endif
	
    
  #elif (LIN_ZERO_PAGE_SIZE  >=  14 * LIN_IFC_NR_MAX)
  
  #ifdef __COSMIC__
	#define LIN_ZERO_PAGE1 @tiny
    #define LIN_ZERO_PAGE4 @tiny
    #define LIN_ZERO_PAGE3 @tiny
    #define LIN_ZERO_PAGE10 @tiny
	#elif __RAISONANCE__
	#define LIN_ZERO_PAGE1 page0
    #define LIN_ZERO_PAGE4 page0
    #define LIN_ZERO_PAGE3 page0
    #define LIN_ZERO_PAGE10 page0
	#elif __IAR__
	#define LIN_ZERO_PAGE1 __tiny
    #define LIN_ZERO_PAGE4 __tiny
    #define LIN_ZERO_PAGE3 __tiny
    #define LIN_ZERO_PAGE10 __tiny
	#endif
	
    
  #elif (LIN_ZERO_PAGE_SIZE  >=  13 * LIN_IFC_NR_MAX)
  
  #ifdef __COSMIC__
	#define LIN_ZERO_PAGE1 @tiny
    #define LIN_ZERO_PAGE2 @tiny
    #define LIN_ZERO_PAGE3 @tiny
    #define LIN_ZERO_PAGE4 @tiny
    #define LIN_ZERO_PAGE5 @tiny
    #define LIN_ZERO_PAGE6 @tiny
    #define LIN_ZERO_PAGE7 @tiny
    #define LIN_ZERO_PAGE8 @tiny
    #define LIN_ZERO_PAGE9 @tiny
	#elif __RAISONANCE__
	#define LIN_ZERO_PAGE1 page0
    #define LIN_ZERO_PAGE2 page0
    #define LIN_ZERO_PAGE3 page0
    #define LIN_ZERO_PAGE4 page0
    #define LIN_ZERO_PAGE5 page0
    #define LIN_ZERO_PAGE6 page0
    #define LIN_ZERO_PAGE7 page0
    #define LIN_ZERO_PAGE8 page0
    #define LIN_ZERO_PAGE9 page0
	#elif __IAR__
	#define LIN_ZERO_PAGE1 __tiny
    #define LIN_ZERO_PAGE2 __tiny
    #define LIN_ZERO_PAGE3 __tiny
    #define LIN_ZERO_PAGE4 __tiny
    #define LIN_ZERO_PAGE5 __tiny
    #define LIN_ZERO_PAGE6 __tiny
    #define LIN_ZERO_PAGE7 __tiny
    #define LIN_ZERO_PAGE8 __tiny
    #define LIN_ZERO_PAGE9 __tiny
	#endif
	
    
  #elif (LIN_ZERO_PAGE_SIZE  >=  12 * LIN_IFC_NR_MAX)
  
  #ifdef __COSMIC__
	#define LIN_ZERO_PAGE1 @tiny
    #define LIN_ZERO_PAGE2 @tiny
    #define LIN_ZERO_PAGE3 @tiny
    #define LIN_ZERO_PAGE4 @tiny
    #define LIN_ZERO_PAGE5 @tiny
    #define LIN_ZERO_PAGE6 @tiny
    #define LIN_ZERO_PAGE7 @tiny
    #define LIN_ZERO_PAGE9 @tiny
	#elif __RAISONANCE__
	#define LIN_ZERO_PAGE1 page0
    #define LIN_ZERO_PAGE2 page0
    #define LIN_ZERO_PAGE3 page0
    #define LIN_ZERO_PAGE4 page0
    #define LIN_ZERO_PAGE5 page0
    #define LIN_ZERO_PAGE6 page0
    #define LIN_ZERO_PAGE7 page0
    #define LIN_ZERO_PAGE9 page0
	#elif __IAR__
	#define LIN_ZERO_PAGE1 __tiny
    #define LIN_ZERO_PAGE2 __tiny
    #define LIN_ZERO_PAGE3 __tiny
    #define LIN_ZERO_PAGE4 __tiny
    #define LIN_ZERO_PAGE5 __tiny
    #define LIN_ZERO_PAGE6 __tiny
    #define LIN_ZERO_PAGE7 __tiny
    #define LIN_ZERO_PAGE9 __tiny
	#endif
	
    
  #elif (LIN_ZERO_PAGE_SIZE  >=  11 * LIN_IFC_NR_MAX)
  
  #ifdef __COSMIC__
	#define LIN_ZERO_PAGE1 @tiny
    #define LIN_ZERO_PAGE2 @tiny
    #define LIN_ZERO_PAGE3 @tiny
    #define LIN_ZERO_PAGE4 @tiny
    #define LIN_ZERO_PAGE5 @tiny
    #define LIN_ZERO_PAGE6 @tiny
    #define LIN_ZERO_PAGE7 @tiny
    #define LIN_ZERO_PAGE8 @tiny
	#elif __RAISONANCE__
	#define LIN_ZERO_PAGE1 page0
    #define LIN_ZERO_PAGE2 page0
    #define LIN_ZERO_PAGE3 page0
    #define LIN_ZERO_PAGE4 page0
    #define LIN_ZERO_PAGE5 page0
    #define LIN_ZERO_PAGE6 page0
    #define LIN_ZERO_PAGE7 page0
    #define LIN_ZERO_PAGE8 page0
	#elif __IAR__
	#define LIN_ZERO_PAGE1 __tiny
    #define LIN_ZERO_PAGE2 __tiny
    #define LIN_ZERO_PAGE3 __tiny
    #define LIN_ZERO_PAGE4 __tiny
    #define LIN_ZERO_PAGE5 __tiny
    #define LIN_ZERO_PAGE6 __tiny
    #define LIN_ZERO_PAGE7 __tiny
    #define LIN_ZERO_PAGE8 __tiny
	#endif
	
    
  #elif (LIN_ZERO_PAGE_SIZE  >=  10 * LIN_IFC_NR_MAX)
  
  #ifdef __COSMIC__
	#define LIN_ZERO_PAGE1 @tiny
    #define LIN_ZERO_PAGE2 @tiny
    #define LIN_ZERO_PAGE3 @tiny
    #define LIN_ZERO_PAGE4 @tiny
    #define LIN_ZERO_PAGE5 @tiny
    #define LIN_ZERO_PAGE6 @tiny
    #define LIN_ZERO_PAGE7 @tiny
	#elif __RAISONANCE__
	#define LIN_ZERO_PAGE1 page0
    #define LIN_ZERO_PAGE2 page0
    #define LIN_ZERO_PAGE3 page0
    #define LIN_ZERO_PAGE4 page0
    #define LIN_ZERO_PAGE5 page0
    #define LIN_ZERO_PAGE6 page0
    #define LIN_ZERO_PAGE7 page0
	#elif __IAR__
	#define LIN_ZERO_PAGE1 __tiny
    #define LIN_ZERO_PAGE2 __tiny
    #define LIN_ZERO_PAGE3 __tiny
    #define LIN_ZERO_PAGE4 __tiny
    #define LIN_ZERO_PAGE5 __tiny
    #define LIN_ZERO_PAGE6 __tiny
    #define LIN_ZERO_PAGE7 __tiny
	#endif
	
    
  #elif (LIN_ZERO_PAGE_SIZE  >=  9 * LIN_IFC_NR_MAX)
  #ifdef __COSMIC__
	#define LIN_ZERO_PAGE1 @tiny
    #define LIN_ZERO_PAGE2 @tiny
    #define LIN_ZERO_PAGE3 @tiny
    #define LIN_ZERO_PAGE4 @tiny
    #define LIN_ZERO_PAGE5 @tiny
    #define LIN_ZERO_PAGE6 @tiny
    #define LIN_ZERO_PAGE8 @tiny
	#elif __RAISONANCE__
	#define LIN_ZERO_PAGE1 page0
    #define LIN_ZERO_PAGE2 page0
    #define LIN_ZERO_PAGE3 page0
    #define LIN_ZERO_PAGE4 page0
    #define LIN_ZERO_PAGE5 page0
    #define LIN_ZERO_PAGE6 page0
    #define LIN_ZERO_PAGE8 page0
	#elif __IAR__
	#define LIN_ZERO_PAGE1 __tiny
    #define LIN_ZERO_PAGE2 __tiny
    #define LIN_ZERO_PAGE3 __tiny
    #define LIN_ZERO_PAGE4 __tiny
    #define LIN_ZERO_PAGE5 __tiny
    #define LIN_ZERO_PAGE6 __tiny
    #define LIN_ZERO_PAGE8 __tiny
	#endif
	
    
  #elif (LIN_ZERO_PAGE_SIZE  >=  8 * LIN_IFC_NR_MAX)
  
  #ifdef __COSMIC__
	#define LIN_ZERO_PAGE1 @tiny
    #define LIN_ZERO_PAGE2 @tiny
    #define LIN_ZERO_PAGE3 @tiny
    #define LIN_ZERO_PAGE4 @tiny
    #define LIN_ZERO_PAGE5 @tiny
    #define LIN_ZERO_PAGE6 @tiny
	#elif __RAISONANCE__
	#define LIN_ZERO_PAGE1 page0
    #define LIN_ZERO_PAGE2 page0
    #define LIN_ZERO_PAGE3 page0
    #define LIN_ZERO_PAGE4 page0
    #define LIN_ZERO_PAGE5 page0
    #define LIN_ZERO_PAGE6 page0
	#elif __IAR__
	#define LIN_ZERO_PAGE1 __tiny
    #define LIN_ZERO_PAGE2 __tiny
    #define LIN_ZERO_PAGE3 __tiny
    #define LIN_ZERO_PAGE4 __tiny
    #define LIN_ZERO_PAGE5 __tiny
    #define LIN_ZERO_PAGE6 __tiny
	#endif
	
    
  #elif (LIN_ZERO_PAGE_SIZE  >=  7 * LIN_IFC_NR_MAX)
  
  #ifdef __COSMIC__
	#define LIN_ZERO_PAGE1 @tiny
    #define LIN_ZERO_PAGE2 @tiny
    #define LIN_ZERO_PAGE3 @tiny
    #define LIN_ZERO_PAGE4 @tiny
    #define LIN_ZERO_PAGE5 @tiny
    #define LIN_ZERO_PAGE8 @tiny
	#elif __RAISONANCE__
	#define LIN_ZERO_PAGE1 page0
    #define LIN_ZERO_PAGE2 page0
    #define LIN_ZERO_PAGE3 page0
    #define LIN_ZERO_PAGE4 page0
    #define LIN_ZERO_PAGE5 page0
    #define LIN_ZERO_PAGE8 page0
	#elif __IAR__
	#define LIN_ZERO_PAGE1 __tiny
    #define LIN_ZERO_PAGE2 __tiny
    #define LIN_ZERO_PAGE3 __tiny
    #define LIN_ZERO_PAGE4 __tiny
    #define LIN_ZERO_PAGE5 __tiny
    #define LIN_ZERO_PAGE8 __tiny
	#endif
	
    
  #elif (LIN_ZERO_PAGE_SIZE  >=  6 * LIN_IFC_NR_MAX)
  
  #ifdef __COSMIC__
	#define LIN_ZERO_PAGE1 @tiny
    #define LIN_ZERO_PAGE2 @tiny
    #define LIN_ZERO_PAGE3 @tiny
    #define LIN_ZERO_PAGE4 @tiny
    #define LIN_ZERO_PAGE8 @tiny
	#elif __RAISONANCE__
	#define LIN_ZERO_PAGE1 page0
    #define LIN_ZERO_PAGE2 page0
    #define LIN_ZERO_PAGE3 page0
    #define LIN_ZERO_PAGE4 page0
    #define LIN_ZERO_PAGE8 page0
	#elif __IAR__
	#define LIN_ZERO_PAGE1 __tiny
    #define LIN_ZERO_PAGE2 __tiny
    #define LIN_ZERO_PAGE3 __tiny
    #define LIN_ZERO_PAGE4 __tiny
    #define LIN_ZERO_PAGE8 __tiny
	#endif
	
    
  #elif (LIN_ZERO_PAGE_SIZE  >=  5 * LIN_IFC_NR_MAX)
  
  #ifdef __COSMIC__
	#define LIN_ZERO_PAGE1 @tiny
    #define LIN_ZERO_PAGE2 @tiny
    #define LIN_ZERO_PAGE3 @tiny
    #define LIN_ZERO_PAGE4 @tiny
	#elif __RAISONANCE__
	#define LIN_ZERO_PAGE1 page0
    #define LIN_ZERO_PAGE2 page0
    #define LIN_ZERO_PAGE3 page0
    #define LIN_ZERO_PAGE4 page0
	#elif __IAR__
	#define LIN_ZERO_PAGE1 __tiny
    #define LIN_ZERO_PAGE2 __tiny
    #define LIN_ZERO_PAGE3 __tiny
    #define LIN_ZERO_PAGE4 __tiny
	#endif
	
    
  #elif (LIN_ZERO_PAGE_SIZE  >=  4 * LIN_IFC_NR_MAX)
  
  #ifdef __COSMIC__
	#define LIN_ZERO_PAGE1 @tiny
    #define LIN_ZERO_PAGE4 @tiny
    #define LIN_ZERO_PAGE3 @tiny
	#elif __RAISONANCE__
	#define LIN_ZERO_PAGE1 page0
    #define LIN_ZERO_PAGE4 page0
    #define LIN_ZERO_PAGE3 page0
	#elif __IAR__
	#define LIN_ZERO_PAGE1 __tiny
    #define LIN_ZERO_PAGE4 __tiny
    #define LIN_ZERO_PAGE3 __tiny
	#endif
	
    
  #elif (LIN_ZERO_PAGE_SIZE  >=  3 * LIN_IFC_NR_MAX)
  
  #ifdef __COSMIC__
	#define LIN_ZERO_PAGE4 @tiny
    #define LIN_ZERO_PAGE3 @tiny
	#elif __RAISONANCE__
	#define LIN_ZERO_PAGE4 page0
    #define LIN_ZERO_PAGE3 page0
	#elif __IAR__
	#define LIN_ZERO_PAGE4 __tiny
    #define LIN_ZERO_PAGE3 __tiny
	#endif
	
    
  #elif (LIN_ZERO_PAGE_SIZE  >=  2 * LIN_IFC_NR_MAX)
  #ifdef __COSMIC__
	#define LIN_ZERO_PAGE1 @tiny
    #define LIN_ZERO_PAGE4 @tiny
	#elif __RAISONANCE__
	#define LIN_ZERO_PAGE1 page0
    #define LIN_ZERO_PAGE4 page0
	#elif __IAR__
	#define LIN_ZERO_PAGE1 __tiny
    #define LIN_ZERO_PAGE4 __tiny
	#endif
	
    
  #elif (LIN_ZERO_PAGE_SIZE  >=  1 * LIN_IFC_NR_MAX)
  
  #ifdef __COSMIC__
	#define LIN_ZERO_PAGE4 @tiny
	#elif __RAISONANCE__
	#define LIN_ZERO_PAGE4 page0
	#elif __IAR__
	#define LIN_ZERO_PAGE4 __tiny
	#endif
	
    
  #endif
#else /* slave node */
  #if (LIN_ZERO_PAGE_SIZE  >=  20 * LIN_IFC_NR_MAX)
  #ifdef __COSMIC__
	#define LIN_ZERO_PAGE1 @tiny
    #define LIN_ZERO_PAGE2 @tiny
    #define LIN_ZERO_PAGE3 @tiny
    #define LIN_ZERO_PAGE4 @tiny
    #define LIN_ZERO_PAGE5 @tiny
    #define LIN_ZERO_PAGE6 @tiny
    #define LIN_ZERO_PAGE7 @tiny
    #define LIN_ZERO_PAGE10 @tiny
	#elif __RAISONANCE__
	#define LIN_ZERO_PAGE1 page0
    #define LIN_ZERO_PAGE2 page0
    #define LIN_ZERO_PAGE3 page0
    #define LIN_ZERO_PAGE4 page0
    #define LIN_ZERO_PAGE5 page0
    #define LIN_ZERO_PAGE6 page0
    #define LIN_ZERO_PAGE7 page0
    #define LIN_ZERO_PAGE10 page0
	#elif __IAR__
	#define LIN_ZERO_PAGE1 __tiny
    #define LIN_ZERO_PAGE2 __tiny
    #define LIN_ZERO_PAGE3 __tiny
    #define LIN_ZERO_PAGE4 __tiny
    #define LIN_ZERO_PAGE5 __tiny
    #define LIN_ZERO_PAGE6 __tiny
    #define LIN_ZERO_PAGE7 __tiny
    #define LIN_ZERO_PAGE10 __tiny
	#endif
	
    
  #elif (LIN_ZERO_PAGE_SIZE  >=  19 * LIN_IFC_NR_MAX)
  
  #ifdef __COSMIC__
	#define LIN_ZERO_PAGE1 @tiny
    #define LIN_ZERO_PAGE2 @tiny
    #define LIN_ZERO_PAGE3 @tiny
    #define LIN_ZERO_PAGE4 @tiny
    #define LIN_ZERO_PAGE5 @tiny
    #define LIN_ZERO_PAGE6 @tiny
    #define LIN_ZERO_PAGE8 @tiny
    #define LIN_ZERO_PAGE10 @tiny
	#elif __RAISONANCE__
	#define LIN_ZERO_PAGE1 page0
    #define LIN_ZERO_PAGE2 page0
    #define LIN_ZERO_PAGE3 page0
    #define LIN_ZERO_PAGE4 page0
    #define LIN_ZERO_PAGE5 page0
    #define LIN_ZERO_PAGE6 page0
    #define LIN_ZERO_PAGE8 page0
    #define LIN_ZERO_PAGE10 page0
	#elif __IAR__
	#define LIN_ZERO_PAGE1 __tiny
    #define LIN_ZERO_PAGE2 __tiny
    #define LIN_ZERO_PAGE3 __tiny
    #define LIN_ZERO_PAGE4 __tiny
    #define LIN_ZERO_PAGE5 __tiny
    #define LIN_ZERO_PAGE6 __tiny
    #define LIN_ZERO_PAGE8 __tiny
    #define LIN_ZERO_PAGE10 __tiny
	#endif
	
    
  #elif (LIN_ZERO_PAGE_SIZE  >=  18 * LIN_IFC_NR_MAX)
  #ifdef __COSMIC__
	#define LIN_ZERO_PAGE1 @tiny
    #define LIN_ZERO_PAGE2 @tiny
    #define LIN_ZERO_PAGE3 @tiny
    #define LIN_ZERO_PAGE4 @tiny
    #define LIN_ZERO_PAGE5 @tiny
    #define LIN_ZERO_PAGE6 @tiny
    #define LIN_ZERO_PAGE10 @tiny
	#elif __RAISONANCE__
	#define LIN_ZERO_PAGE1 page0
    #define LIN_ZERO_PAGE2 page0
    #define LIN_ZERO_PAGE3 page0
    #define LIN_ZERO_PAGE4 page0
    #define LIN_ZERO_PAGE5 page0
    #define LIN_ZERO_PAGE6 page0
    #define LIN_ZERO_PAGE10 page0
	#elif __IAR__
	#define LIN_ZERO_PAGE1 __tiny
    #define LIN_ZERO_PAGE2 __tiny
    #define LIN_ZERO_PAGE3 __tiny
    #define LIN_ZERO_PAGE4 __tiny
    #define LIN_ZERO_PAGE5 __tiny
    #define LIN_ZERO_PAGE6 __tiny
    #define LIN_ZERO_PAGE8 __tiny
    #define LIN_ZERO_PAGE10 __tiny
	#endif
	
    
  #elif (LIN_ZERO_PAGE_SIZE  >=  17 * LIN_IFC_NR_MAX)
  #ifdef __COSMIC__
	#define LIN_ZERO_PAGE1 @tiny
    #define LIN_ZERO_PAGE2 @tiny
    #define LIN_ZERO_PAGE3 @tiny
    #define LIN_ZERO_PAGE4 @tiny
    #define LIN_ZERO_PAGE5 @tiny
    #define LIN_ZERO_PAGE8 @tiny
    #define LIN_ZERO_PAGE10 @tiny
	#elif __RAISONANCE__
	#define LIN_ZERO_PAGE1 page0
    #define LIN_ZERO_PAGE2 page0
    #define LIN_ZERO_PAGE3 page0
    #define LIN_ZERO_PAGE4 page0
    #define LIN_ZERO_PAGE5 page0
    #define LIN_ZERO_PAGE8 page0
    #define LIN_ZERO_PAGE10 page0
	#elif __IAR__
	#define LIN_ZERO_PAGE1 __tiny
    #define LIN_ZERO_PAGE2 __tiny
    #define LIN_ZERO_PAGE3 __tiny
    #define LIN_ZERO_PAGE4 __tiny
    #define LIN_ZERO_PAGE5 __tiny
    #define LIN_ZERO_PAGE8 __tiny
    #define LIN_ZERO_PAGE10 __tiny
	#endif
	
    
  #elif (LIN_ZERO_PAGE_SIZE  >=  16 * LIN_IFC_NR_MAX)
  #ifdef __COSMIC__
	#define LIN_ZERO_PAGE1 @tiny
    #define LIN_ZERO_PAGE2 @tiny
    #define LIN_ZERO_PAGE3 @tiny
    #define LIN_ZERO_PAGE4 @tiny
    #define LIN_ZERO_PAGE5 @tiny
    #define LIN_ZERO_PAGE10 @tiny
	#elif __RAISONANCE__
	#define LIN_ZERO_PAGE1 page0
    #define LIN_ZERO_PAGE2 page0
    #define LIN_ZERO_PAGE3 page0
    #define LIN_ZERO_PAGE4 page0
    #define LIN_ZERO_PAGE5 page0
    #define LIN_ZERO_PAGE10 page0
	#elif __IAR__
	#define LIN_ZERO_PAGE1 __tiny
    #define LIN_ZERO_PAGE2 __tiny
    #define LIN_ZERO_PAGE3 __tiny
    #define LIN_ZERO_PAGE4 __tiny
    #define LIN_ZERO_PAGE5 __tiny
    #define LIN_ZERO_PAGE10 __tiny
	#endif
	
   
  #elif (LIN_ZERO_PAGE_SIZE  >=  15 * LIN_IFC_NR_MAX)
  #ifdef __COSMIC__
	#define LIN_ZERO_PAGE1 @tiny
    #define LIN_ZERO_PAGE2 @tiny
    #define LIN_ZERO_PAGE3 @tiny
    #define LIN_ZERO_PAGE4 @tiny
    #define LIN_ZERO_PAGE10 @tiny
	#elif __RAISONANCE__
	#define LIN_ZERO_PAGE1 page0
    #define LIN_ZERO_PAGE2 page0
    #define LIN_ZERO_PAGE3 page0
    #define LIN_ZERO_PAGE4 page0
    #define LIN_ZERO_PAGE10 page0
	#elif __IAR__
	#define LIN_ZERO_PAGE1 __tiny
    #define LIN_ZERO_PAGE2 __tiny
    #define LIN_ZERO_PAGE3 __tiny
    #define LIN_ZERO_PAGE4 __tiny
    #define LIN_ZERO_PAGE10 __tiny
	#endif
	
   
  #elif (LIN_ZERO_PAGE_SIZE  >=  14 * LIN_IFC_NR_MAX)
  #ifdef __COSMIC__
	#define LIN_ZERO_PAGE1 @tiny
    #define LIN_ZERO_PAGE2 @tiny
    #define LIN_ZERO_PAGE3 @tiny
    #define LIN_ZERO_PAGE10 @tiny
	#elif __RAISONANCE__
	#define LIN_ZERO_PAGE1 page0
    #define LIN_ZERO_PAGE2 page0
    #define LIN_ZERO_PAGE3 page0
    #define LIN_ZERO_PAGE10 page0
	#elif __IAR__
	#define LIN_ZERO_PAGE1 __tiny
    #define LIN_ZERO_PAGE2 __tiny
    #define LIN_ZERO_PAGE3 __tiny
    #define LIN_ZERO_PAGE10 __tiny
	#endif
	
    
  #elif (LIN_ZERO_PAGE_SIZE  >=  13 * LIN_IFC_NR_MAX)
  
  #ifdef __COSMIC__
	#define LIN_ZERO_PAGE1 @tiny
    #define LIN_ZERO_PAGE3 @tiny
    #define LIN_ZERO_PAGE10 @tiny
	#elif __RAISONANCE__
	#define LIN_ZERO_PAGE1 page0
    #define LIN_ZERO_PAGE3 page0
    #define LIN_ZERO_PAGE10 page0
	#elif __IAR__
	#define LIN_ZERO_PAGE1 __tiny
    #define LIN_ZERO_PAGE3 __tiny
    #define LIN_ZERO_PAGE10 __tiny
	#endif
	
    
  #elif (LIN_ZERO_PAGE_SIZE  >=  12 * LIN_IFC_NR_MAX)
  
  #ifdef __COSMIC__
	 #define LIN_ZERO_PAGE1 @tiny
    #define LIN_ZERO_PAGE2 @tiny
    #define LIN_ZERO_PAGE3 @tiny
    #define LIN_ZERO_PAGE4 @tiny
    #define LIN_ZERO_PAGE5 @tiny
    #define LIN_ZERO_PAGE6 @tiny
    #define LIN_ZERO_PAGE7 @tiny
    #define LIN_ZERO_PAGE8 @tiny
    #define LIN_ZERO_PAGE9 @tiny
	#elif __RAISONANCE__
	#define LIN_ZERO_PAGE1 page0
    #define LIN_ZERO_PAGE2 page0
    #define LIN_ZERO_PAGE3 page0
    #define LIN_ZERO_PAGE4 page0
    #define LIN_ZERO_PAGE5 page0
    #define LIN_ZERO_PAGE6 page0
    #define LIN_ZERO_PAGE7 page0
    #define LIN_ZERO_PAGE8 page0
    #define LIN_ZERO_PAGE9 page0
	#elif __IAR__
	#define LIN_ZERO_PAGE1 __tiny
    #define LIN_ZERO_PAGE2 __tiny
    #define LIN_ZERO_PAGE3 __tiny
    #define LIN_ZERO_PAGE4 __tiny
    #define LIN_ZERO_PAGE5 __tiny
    #define LIN_ZERO_PAGE6 __tiny
    #define LIN_ZERO_PAGE7 __tiny
    #define LIN_ZERO_PAGE8 __tiny
    #define LIN_ZERO_PAGE9 __tiny
	#endif
	
   
  #elif (LIN_ZERO_PAGE_SIZE  >=  11 * LIN_IFC_NR_MAX)
  
  #ifdef __COSMIC__
	 #define LIN_ZERO_PAGE1 @tiny
    #define LIN_ZERO_PAGE2 @tiny
    #define LIN_ZERO_PAGE3 @tiny
    #define LIN_ZERO_PAGE4 @tiny
    #define LIN_ZERO_PAGE5 @tiny
    #define LIN_ZERO_PAGE6 @tiny
    #define LIN_ZERO_PAGE7 @tiny
    #define LIN_ZERO_PAGE8 @tiny
	#elif __RAISONANCE__
	#define LIN_ZERO_PAGE1 page0
    #define LIN_ZERO_PAGE2 page0
    #define LIN_ZERO_PAGE3 page0
    #define LIN_ZERO_PAGE4 page0
    #define LIN_ZERO_PAGE5 page0
    #define LIN_ZERO_PAGE6 page0
    #define LIN_ZERO_PAGE7 page0
    #define LIN_ZERO_PAGE8 page0
	#elif __IAR__
	#define LIN_ZERO_PAGE1 __tiny
    #define LIN_ZERO_PAGE2 __tiny
    #define LIN_ZERO_PAGE3 __tiny
    #define LIN_ZERO_PAGE4 __tiny
    #define LIN_ZERO_PAGE5 __tiny
    #define LIN_ZERO_PAGE6 __tiny
    #define LIN_ZERO_PAGE7 __tiny
    #define LIN_ZERO_PAGE8 __tiny
	#endif
	
   
  #elif (LIN_ZERO_PAGE_SIZE  >=  10 * LIN_IFC_NR_MAX)
  #ifdef __COSMIC__
	 #define LIN_ZERO_PAGE1 @tiny
    #define LIN_ZERO_PAGE2 @tiny
    #define LIN_ZERO_PAGE3 @tiny
    #define LIN_ZERO_PAGE4 @tiny
    #define LIN_ZERO_PAGE5 @tiny
    #define LIN_ZERO_PAGE6 @tiny
    #define LIN_ZERO_PAGE7 @tiny
	#elif __RAISONANCE__
	#define LIN_ZERO_PAGE1 page0
    #define LIN_ZERO_PAGE2 page0
    #define LIN_ZERO_PAGE3 page0
    #define LIN_ZERO_PAGE4 page0
    #define LIN_ZERO_PAGE5 page0
    #define LIN_ZERO_PAGE6 page0
    #define LIN_ZERO_PAGE7 page0
	#elif __IAR__
	#define LIN_ZERO_PAGE1 __tiny
    #define LIN_ZERO_PAGE2 __tiny
    #define LIN_ZERO_PAGE3 __tiny
    #define LIN_ZERO_PAGE4 __tiny
    #define LIN_ZERO_PAGE5 __tiny
    #define LIN_ZERO_PAGE6 __tiny
    #define LIN_ZERO_PAGE7 __tiny
	#endif
	
  
   
  #elif (LIN_ZERO_PAGE_SIZE  >=  9 * LIN_IFC_NR_MAX)
  #ifdef __COSMIC__
	#define LIN_ZERO_PAGE1 @tiny
    #define LIN_ZERO_PAGE2 @tiny
    #define LIN_ZERO_PAGE3 @tiny
    #define LIN_ZERO_PAGE4 @tiny
    #define LIN_ZERO_PAGE5 @tiny
    #define LIN_ZERO_PAGE6 @tiny
    #define LIN_ZERO_PAGE8 @tiny
	#elif __RAISONANCE__
	#define LIN_ZERO_PAGE1 page0
    #define LIN_ZERO_PAGE2 page0
    #define LIN_ZERO_PAGE3 page0
    #define LIN_ZERO_PAGE4 page0
    #define LIN_ZERO_PAGE5 page0
    #define LIN_ZERO_PAGE6 page0
    #define LIN_ZERO_PAGE8 page0
	#elif __IAR__
	#define LIN_ZERO_PAGE1 __tiny
    #define LIN_ZERO_PAGE2 __tiny
    #define LIN_ZERO_PAGE3 __tiny
    #define LIN_ZERO_PAGE4 __tiny
    #define LIN_ZERO_PAGE5 __tiny
    #define LIN_ZERO_PAGE6 __tiny
    #define LIN_ZERO_PAGE8 __tiny
	#endif
	
  
    
  #elif (LIN_ZERO_PAGE_SIZE  >=  8 * LIN_IFC_NR_MAX)
  #ifdef __COSMIC__
	#define LIN_ZERO_PAGE1 @tiny
    #define LIN_ZERO_PAGE2 @tiny
    #define LIN_ZERO_PAGE3 @tiny
    #define LIN_ZERO_PAGE4 @tiny
    #define LIN_ZERO_PAGE5 @tiny
    #define LIN_ZERO_PAGE6 @tiny
	#elif __RAISONANCE__
	#define LIN_ZERO_PAGE1 page0
    #define LIN_ZERO_PAGE2 page0
    #define LIN_ZERO_PAGE3 page0
    #define LIN_ZERO_PAGE4 page0
    #define LIN_ZERO_PAGE5 page0
    #define LIN_ZERO_PAGE6 page0
	#elif __IAR__
	#define LIN_ZERO_PAGE1 __tiny
    #define LIN_ZERO_PAGE2 __tiny
    #define LIN_ZERO_PAGE3 __tiny
    #define LIN_ZERO_PAGE4 __tiny
    #define LIN_ZERO_PAGE5 __tiny
    #define LIN_ZERO_PAGE6 __tiny
	#endif
	
    
  #elif (LIN_ZERO_PAGE_SIZE  >=  7 * LIN_IFC_NR_MAX)
  #ifdef __COSMIC__
	#define LIN_ZERO_PAGE1 @tiny
    #define LIN_ZERO_PAGE2 @tiny
    #define LIN_ZERO_PAGE3 @tiny
    #define LIN_ZERO_PAGE4 @tiny
    #define LIN_ZERO_PAGE5 @tiny
    #define LIN_ZERO_PAGE8 @tiny
	#elif __RAISONANCE__
	#define LIN_ZERO_PAGE1 page0
    #define LIN_ZERO_PAGE2 page0
    #define LIN_ZERO_PAGE3 page0
    #define LIN_ZERO_PAGE4 page0
    #define LIN_ZERO_PAGE5 page0
    #define LIN_ZERO_PAGE8 page0
	#elif __IAR__
	#define LIN_ZERO_PAGE1 __tiny
    #define LIN_ZERO_PAGE2 __tiny
    #define LIN_ZERO_PAGE3 __tiny
    #define LIN_ZERO_PAGE4 __tiny
    #define LIN_ZERO_PAGE5 __tiny
    #define LIN_ZERO_PAGE8 __tiny
	#endif
	
    
  #elif (LIN_ZERO_PAGE_SIZE  >=  6 * LIN_IFC_NR_MAX)
  #ifdef __COSMIC__
	#define LIN_ZERO_PAGE1 @tiny
    #define LIN_ZERO_PAGE2 @tiny
    #define LIN_ZERO_PAGE3 @tiny
    #define LIN_ZERO_PAGE4 @tiny
    #define LIN_ZERO_PAGE5 @tiny
	#elif __RAISONANCE__
	#define LIN_ZERO_PAGE1 page0
    #define LIN_ZERO_PAGE2 page0
    #define LIN_ZERO_PAGE3 page0
    #define LIN_ZERO_PAGE4 page0
    #define LIN_ZERO_PAGE5 page0
	#elif __IAR__
	#define LIN_ZERO_PAGE1 __tiny
    #define LIN_ZERO_PAGE2 __tiny
    #define LIN_ZERO_PAGE3 __tiny
    #define LIN_ZERO_PAGE4 __tiny
    #define LIN_ZERO_PAGE5 __tiny
	#endif
	
    
  #elif (LIN_ZERO_PAGE_SIZE  >=  5 * LIN_IFC_NR_MAX)
  
  #ifdef __COSMIC__
	#define LIN_ZERO_PAGE1 @tiny
    #define LIN_ZERO_PAGE2 @tiny
    #define LIN_ZERO_PAGE3 @tiny
    #define LIN_ZERO_PAGE4 @tiny
	#elif __RAISONANCE__
	#define LIN_ZERO_PAGE1 page0
    #define LIN_ZERO_PAGE2 page0
    #define LIN_ZERO_PAGE3 page0
    #define LIN_ZERO_PAGE4 page0
	#elif __IAR__
	#define LIN_ZERO_PAGE1 __tiny
    #define LIN_ZERO_PAGE2 __tiny
    #define LIN_ZERO_PAGE3 __tiny
    #define LIN_ZERO_PAGE4 __tiny
	#endif
	
    
  #elif (LIN_ZERO_PAGE_SIZE  >=  4 * LIN_IFC_NR_MAX)
  #ifdef __COSMIC__
	#define LIN_ZERO_PAGE1 @tiny
    #define LIN_ZERO_PAGE2 @tiny
    #define LIN_ZERO_PAGE3 @tiny
	#elif __RAISONANCE__
	#define LIN_ZERO_PAGE1 page0
    #define LIN_ZERO_PAGE2 page0
    #define LIN_ZERO_PAGE3 page0
	#elif __IAR__
	#define LIN_ZERO_PAGE1 __tiny
    #define LIN_ZERO_PAGE2 __tiny
    #define LIN_ZERO_PAGE3 __tiny
	#endif
	
    
  #elif (LIN_ZERO_PAGE_SIZE  >=  3 * LIN_IFC_NR_MAX)
  
  #ifdef __COSMIC__
	#define LIN_ZERO_PAGE1 @tiny
    #define LIN_ZERO_PAGE3 @tiny
	#elif __RAISONANCE__
	#define LIN_ZERO_PAGE1 page0
    #define LIN_ZERO_PAGE3 page0
	#elif __IAR__
	#define LIN_ZERO_PAGE1 __tiny
    #define LIN_ZERO_PAGE3 __tiny
	#endif
	
    
  #elif (LIN_ZERO_PAGE_SIZE  >=  2 * LIN_IFC_NR_MAX)
  
  #ifdef __COSMIC__
	#define LIN_ZERO_PAGE1 @tiny
    #define LIN_ZERO_PAGE2 @tiny
	#elif __RAISONANCE__
	#define LIN_ZERO_PAGE1 page0
    #define LIN_ZERO_PAGE2 page0
	#elif __IAR__
	#define LIN_ZERO_PAGE1 __tiny
    #define LIN_ZERO_PAGE2 __tiny
	#endif
	
    
  #elif (LIN_ZERO_PAGE_SIZE  >=  1 * LIN_IFC_NR_MAX)
  
  #ifdef __COSMIC__
	#define LIN_ZERO_PAGE1 @tiny
	#elif __RAISONANCE__
	#define LIN_ZERO_PAGE1 page0
	#elif __IAR__
	#define LIN_ZERO_PAGE1 __tiny
	#endif
	
    
  #endif
#endif

#ifndef LIN_ZERO_PAGE1
  #define LIN_ZERO_PAGE1
#endif

#ifndef LIN_ZERO_PAGE2
  #define LIN_ZERO_PAGE2
#endif

#ifndef LIN_ZERO_PAGE3
  #define LIN_ZERO_PAGE3
#endif

#ifndef LIN_ZERO_PAGE4
  #define LIN_ZERO_PAGE4
#endif

#ifndef LIN_ZERO_PAGE5
  #define LIN_ZERO_PAGE5
#endif

#ifndef LIN_ZERO_PAGE6
  #define LIN_ZERO_PAGE6
#endif

#ifndef LIN_ZERO_PAGE7
  #define LIN_ZERO_PAGE7
#endif

#ifndef LIN_ZERO_PAGE8
  #define LIN_ZERO_PAGE8
#endif

#ifndef LIN_ZERO_PAGE9
  #define LIN_ZERO_PAGE9
#endif

#ifndef LIN_ZERO_PAGE10
  #define LIN_ZERO_PAGE10
#endif

#ifndef LIN_ZERO_PAGE11
  #define LIN_ZERO_PAGE11
#endif

#ifndef LIN_ZERO_PAGE12
  #define LIN_ZERO_PAGE12
#endif

#ifndef LIN_ZERO_PAGE13
  #define LIN_ZERO_PAGE13
#endif

#ifndef LIN_ZERO_PAGE14
  #define LIN_ZERO_PAGE14
#endif

#ifndef LIN_ZERO_PAGE15
 #define LIN_ZERO_PAGE15
#endif

#endif /* LIN_DEF_STM8L_GEN_H */

/*
 * eof lin_def_stm8L_gen.h
 */
