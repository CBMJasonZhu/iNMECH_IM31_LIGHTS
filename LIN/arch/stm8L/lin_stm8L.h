/*********************************************************************
 *
 * Project:      LIN 2.1 driver suite
 *
 * File name:    lin_stm8L.h
 *
 * Copyright:    (c) STMicroelectronics
 *
 * Author:       Jatin KHURANA
 *
 * Description:  Hardware dependend parts of the LIN driver:
 *               - Timer
 *               - Serial interfaces
 *
 * Target:       STM8AL
 *
 * Names:        jk  Jatin Khurana STMicroelectronics
 *
 * NOT TO BE CHANGED BY THE USER
 *
 *********************************************************************/

/*********************************************************************
 *
 * History:
 *
 * 20071213  v1.0   gsf  - Initial version for STM8
 * 20090420  v1.1   gsf  - BUGZILLA_472: LIN driver does not compile if 
 *                         Cosmic "strict" option is enabled.
 *                         BUGZILLA_478: “Mute Mode” must be disabled 
 *                         for a LINSCI Slave node, when this is in 
 *                         Sleep State mode.
 *
 *********************************************************************/

/** \file lin_stm8.h
 */

#ifndef LIN_STM8L_H
  #define LIN_STM8L_H

#define LIN_STM8L_H_MAJOR_VERSION  1
#define LIN_STM8L_H_MINOR_VERSION  32
#define LIN_STM8L_H_PATCH_VERSION  1

#define LIN_STM8L_H_STM

#ifndef __ARCH_STM8L__
  #error Error: This file supports only STM8L
#endif


/*********************************************************************
 * includes
 *********************************************************************/

#include "lin_cfg_types.h"
#include "lin_def_arch_include.h"
#include "lin_def.h"

#include "lin_general.h"


#ifdef LIN_VERSION_CONTROL_H
  #error "lin_version_control.h shall not be included in h-files and shall be the last file to be included in a c-file!"
#endif

//#ifndef __COSMIC__
//  #error "The driver for the STM8 supports only the cosmic compiler"
//#endif

#ifndef QAC_CHECK
  #define QAC_IGNORE
#endif

/*********************************************************************
 * global macros
 *********************************************************************/


/***********************************************************
 * register definitions
 ***********************************************************/

#ifdef SCI1_USED

  /********************************************
   * SCI2   USART Master/Slave
   ********************************************/
#ifdef __COSMIC__
  volatile l_u8   USART1_SR          @0x5230;  /* SCI2 Status register                        */
  volatile l_u8   USART1_DR          @0x5231;  /* SCI2 Data register                          */
  volatile l_u8   USART1_BRR1        @0x5232;  /* SCI2 Baud rate register 1 SCIDIV mantissa[7:0] */
  volatile l_u8   USART1_BRR2        @0x5233;  /* SCI2 Baud rate register 2 SCIDIV mantissa[11:8] SCIDIV fraction[3:0] */
  volatile l_u8   USART1_CR1         @0x5234;  /* SCI2 Control register 1                     */
  volatile l_u8   USART1_CR2         @0x5235;  /* SCI2 Control register 2                     */
  volatile l_u8   USART1_CR3         @0x5236;  /* SCI2 Control register 3                     */
  volatile l_u8   USART1_CR4         @0x5237;  /* SCI2 Control register 4                     */
  volatile l_u8   USART1_CR5         @0x5238;  /* SCI2 Control register 5                     */
  volatile l_u8   USART1_GTR         @0x5239;  /* SCI2 Guardtime register 3                   */
  volatile l_u8   USART1_PSCR        @0x523A;  /* SCI2 Prescaler register                     */
#endif
#ifdef __RAISONANCE__
  at 0x5230 hreg   USART1_SR          ;  /* SCI2 Status register                        */
  at 0x5231 hreg   USART1_DR          ;  /* SCI2 Data register                          */
  at 0x5232 hreg   USART1_BRR1        ;  /* SCI2 Baud rate register 1 SCIDIV mantissa[7:0] */
  at 0x5233 hreg   USART1_BRR2        ;  /* SCI2 Baud rate register 2 SCIDIV mantissa[11:8] SCIDIV fraction[3:0] */
  at 0x5234 hreg   USART1_CR1         ;  /* SCI2 Control register 1                     */
  at 0x5235 hreg   USART1_CR2         ;  /* SCI2 Control register 2                     */
  at 0x5236 hreg   USART1_CR3         ;  /* SCI2 Control register 3                     */
  at 0x5237 hreg   USART1_CR4         ;  /* SCI2 Control register 4                     */
  at 0x5238 hreg   USART1_CR5         ;  /* SCI2 Control register 5                     */
  at 0x5239 hreg   USART1_GTR         ;  /* SCI2 Guardtime register 3                */
  at 0x523A hreg   USART1_PSCR        ;  /* SCI2 Prescaler register                     */
#endif
#ifdef __IAR__
  __no_init	volatile l_u8   USART1_SR          @0x5230;  /* SCI2 Status register                        */
  __no_init	volatile l_u8   USART1_DR          @0x5231;  /* SCI2 Data register                          */
  __no_init	volatile l_u8   USART1_BRR1        @0x5232;  /* SCI2 Baud rate register 1 SCIDIV mantissa[7:0] */
  __no_init	volatile l_u8   USART1_BRR2        @0x5233;  /* SCI2 Baud rate register 2 SCIDIV mantissa[11:8] SCIDIV fraction[3:0] */
  __no_init	volatile l_u8   USART1_CR1         @0x5234;  /* SCI2 Control register 1                     */
  __no_init	volatile l_u8   USART1_CR2         @0x5235;  /* SCI2 Control register 2                     */
  __no_init	volatile l_u8   USART1_CR3         @0x5236;  /* SCI2 Control register 3                     */
  __no_init	volatile l_u8   USART1_CR4         @0x5237;  /* SCI2 Control register 4                     */
  __no_init	volatile l_u8   USART1_CR5         @0x5238;  /* SCI2 Control register 5                     */
  __no_init	volatile l_u8   USART1_GTR         @0x5239;  /* SCI2 Guardtime register 3                   */
  __no_init	volatile l_u8   USART1_PSCR        @0x523A;  /* SCI2 Prescaler register                     */
#endif

/********************************************
   * bit definitions for SCI1 registers
   ********************************************/
  /********************************************
   * SCI1 status register 
   ********************************************/
  #define USART1_SR_TXE          ((l_u8)0x80)  /* Transmit Data Register Empty Mask           */
  #define USART1_SR_TC           ((l_u8)0x40)  /* Transmission Complete Mask                  */
  #define USART1_SR_RXNE         ((l_u8)0x20)  /* Read Data Register Not Empty Mask           */
  #define USART1_SR_IDLE         ((l_u8)0x10)  /* IDLE line detected Mask                     */
  #define USART1_SR_OR           ((l_u8)0x08)  /* OverRun error Mask                          */
  #define USART1_SR_NF           ((l_u8)0x04)  /* Noise Flag Mask                             */
  #define USART1_SR_FE           ((l_u8)0x02)  /* Framing Error Mask                          */
  #define USART1_SR_PE           ((l_u8)0x01)  /* Parity Error Mask                           */

  /********************************************
   * SCI1 Baudrate register 1 
   ********************************************/
  #define USART1_BRR1_DIVM       ((l_u8)0xFF)  /* LSB mantissa of USARTDIV [7:0] Mask         */

  /********************************************
   * SCI1 Baudrate register 2
   ********************************************/
  #define USART1_BRR2_DIVM       ((l_u8)0xF0)  /* MSB mantissa of USARTDIV [11:8] Mask        */
  #define USART1_BRR2_DIVF       ((l_u8)0x0F)  /* Fraction bits of USARTDIV [3:0] Mask        */

  /********************************************
   * SCI1 Control register 1 
   ********************************************/
  #define USART1_CR1_R8          ((l_u8)0x80)  /* Receive Data bit 8                          */
  #define USART1_CR1_T8          ((l_u8)0x40)  /* Transmit data bit 8                         */
  #define USART1_CR1_USARTD      ((l_u8)0x20)  /* USART Disable (for low power consumption)   */
  #define USART1_CR1_M           ((l_u8)0x10)  /* Word length Mask                            */
  #define USART1_CR1_WAKE        ((l_u8)0x08)  /* Wake-up method Mask                         */
  #define USART1_CR1_PCEN        ((l_u8)0x04)  /* Parity Control Enable Mask                  */
  #define USART1_CR1_PS          ((l_u8)0x02)  /* USART LINBreakLength Mask                   */
  #define USART1_CR1_PIEN        ((l_u8)0x01)  /* USART Parity Enable Mask                    */

  /********************************************
   * SCI1 Control register 2 
   ********************************************/
  #define USART1_CR2_TIEN        ((l_u8)0x80)  /* Transmitter Interrupt Enable Mask           */
  #define USART1_CR2_TCIEN       ((l_u8)0x40)  /* TransmissionComplete Interrupt Enable Mask  */
  #define USART1_CR2_RIEN        ((l_u8)0x20)  /* Receiver Interrupt Enable Mask              */
  #define USART1_CR2_ILIEN       ((l_u8)0x10)  /* IDLE Line Interrupt Enable Mask             */
  #define USART1_CR2_TEN         ((l_u8)0x08)  /* Transmitter Enable Mask                     */
  #define USART1_CR2_REN         ((l_u8)0x04)  /* Receiver Enable Mask                        */
  #define USART1_CR2_RWU         ((l_u8)0x02)  /* Receiver Wake-Up Mask                       */
  #define USART1_CR2_SBK         ((l_u8)0x01)  /* Send Break Mask                             */

  /********************************************
   * SCI1 Control register 3
   ********************************************/
  #define USART1_CR3_Reserved    ((l_u8)0xC0)  /* RESERVED bit Mask                           */
 // #define USART1_CR3_LINEN       ((l_u8)0x40)  /* Alternate Function outpu Mask               */
  #define USART1_CR3_STOP        ((l_u8)0x30)  /* STOP bits [1:0] Mask                        */
  #define USART1_CR3_CLKEN       ((l_u8)0x08)  /* Clock Enable Mask                           */
  #define USART1_CR3_CPOL        ((l_u8)0x04)  /* Clock Polarity Mask                         */
  #define USART1_CR3_CPHA        ((l_u8)0x02)  /* Clock Phase Mask                            */
  #define USART1_CR3_LBCL        ((l_u8)0x01)  /* Last Bit Clock pulse Mask                   */

  /********************************************
   * SCI1 Control register 4
   ********************************************/
  #define USART1_CR4_Reserved    ((l_u8)0xF0)  /* RESERVED bit Mask                           */
 // #define USART1_CR4_LBDIEN      ((l_u8)0x40)  /* LIN Break Detection Interrupt Enable Mask   */
 // #define USART1_CR4_LBDL        ((l_u8)0x20)  /* LIN Break Detection Length Mask             */
 // #define USART1_CR4_LBDF        ((l_u8)0x10)  /* LIN Break Detection Flag Mask               */
  #define USART1_CR4_ADD         ((l_u8)0x0F)  /* Address of the USART node Mask              */

  /********************************************
   * SCI1 Control register 5
   ********************************************/
  #define USART1_CR5_Reserved    ((l_u8)0xC1)  /* RESERVED bit Mask                           */
  #define USART1_CR5_SCEN        ((l_u8)0x20)  /* Smart Card Enable Mask                      */
  #define USART1_CR5_NACK        ((l_u8)0x10)  /* Smart Card Nack Enable Mask                 */
  #define USART1_CR5_HDSEL       ((l_u8)0x08)  /* Half-Duplex Selection Mask                  */
  #define USART1_CR5_IRLP        ((l_u8)0x04)  /* Irda Low Power Selection Mask               */
  #define USART1_CR5_IREN        ((l_u8)0x02)  /* Irda Enable Mask                            */
#endif

#if (defined(SCI0_USED) || defined (SCI2_USED)|| defined(SCI3_USED) || defined(SCI4_USED) ||defined(SCI5_USED) || defined(SCI6_USED) || defined(SCI7_USED) || defined(SCI8_USED) || defined(SCI9_USED))
  #error "For STM8L only SCI1 is valid"
#endif

#ifdef LIN_USE_HARDWARE_TIMER
  /********************************************
  * 8 bit timer module
  ********************************************/
#ifdef __COSMIC__
  volatile l_u8   TIM4_CR1          @0x52E0;  /* Control register 1                          */
  volatile l_u8   TIM4_IER          @0x52E4;  /* Interrupt enable register                   */
  volatile l_u8   TIM4_SR1          @0x52E5;  /* Status register 1                           */
  volatile l_u8   TIM4_EGR          @0x52E6;  /* Event generation register                   */
  volatile l_u8   TIM4_CNTR         @0x52E7;  /* Counter high                                */
  volatile l_u8   TIM4_PSCL         @0x52E8;  /* Prescaler register                          */	
  volatile l_u8   TIM4_ARR          @0x52E9;  /* Auto-reload register high                   */
#endif

#ifdef __RAISONANCE__
  at 0x52E0 hreg   TIM4_CR1;         /* Control register 1                          */
  at 0x52E4 hreg   TIM4_IER;         /* Interrupt enable register                   */
  at 0x52E5 hreg   TIM4_SR1;         /* Status register 1                           */
  at 0x52E6 hreg   TIM4_EGR;         /* Event generation register                   */
  at 0x52E7 hreg   TIM4_CNTR;        /* Counter high                                */
  at 0x52E8 hreg   TIM4_PSCL;        /* Prescaler register                          */
  at 0x52E9 hreg   TIM4_ARR;         /* Auto-reload register high                   */
#endif

#ifdef __IAR__
  __no_init	volatile l_u8   TIM4_CR1          @0x52E0;  /* Control register 1                          */
  __no_init	volatile l_u8   TIM4_IER          @0x52E4;  /* Interrupt enable register                   */
  __no_init	volatile l_u8   TIM4_SR1          @0x52E5;  /* Status register 1                           */
  __no_init	volatile l_u8   TIM4_EGR          @0x52E6;  /* Event generation register                   */
  __no_init	volatile l_u8   TIM4_CNTR         @0x52E7;  /* Counter high                                */
  __no_init	volatile l_u8   TIM4_PSCL         @0x52E8;  /* Prescaler register                          */
  __no_init	volatile l_u8   TIM4_ARR          @0x52E9;  /* Auto-reload register high                   */
#endif

   
  /********************************************
   * bit definitions for SCI1 registers
   ********************************************/
  /********************************************
   * TIM4 Control register 1 
   ********************************************/
  #define TIM4_CR1_ARPE         ((l_u8)0x80)
  #define TIM4_CR1_Reserved     ((l_u8)0x70)
  #define TIM4_CR1_OPM          ((l_u8)0x08)
  #define TIM4_CR1_URS          ((l_u8)0x04)
  #define TIM4_CR1_UDIS         ((l_u8)0x02)
  #define TIM4_CR1_CEN          ((l_u8)0x01)

  /********************************************
   * TIM4 Interrupt enable register
   ********************************************/
  #define TIM4_IER_TIE          ((l_u8)0x40)
//  #define TIM4_IER_Reserved     ((l_u8)0xA0)
//  #define TIM4_IER_CC4IE        ((l_u8)0x10)
 // #define TIM4_IER_CC3IE        ((l_u8)0x08)
//  #define TIM4_IER_CC2IE        ((l_u8)0x04)
//  #define TIM4_IER_CC1IE        ((l_u8)0x02)
  #define TIM4_IER_UIE          ((l_u8)0x01)

  /********************************************
   * TIM4 Status register 1
   ********************************************/
  #define TIM4_SR1_TIF          ((l_u8)0x40)
 // #define TIM4_SR1_Reserved     ((l_u8)0xA0)
 // #define TIM4_SR1_CC4IF        ((l_u8)0x10)
 // #define TIM4_SR1_CC3IF        ((l_u8)0x08)
 // #define TIM4_SR1_CC2IF        ((l_u8)0x04)
 // #define TIM4_SR1_CC1IF        ((l_u8)0x02)
  #define TIM4_SR1_UIF          ((l_u8)0x01)

  /********************************************
   * TIM4 Event generation register
   ********************************************/
  #define TIM4_EGR_TG           ((l_u8)0x40)
 // #define TIM4_EGR_Reserved     ((l_u8)0xA0)
 // #define TIM4_EGR_CC4G         ((l_u8)0x10)
 // #define TIM4_EGR_CC3G         ((l_u8)0x08)
 // #define TIM4_EGR_CC2G         ((l_u8)0x04)
 // #define TIM4_EGR_CC1G         ((l_u8)0x02)
  #define TIM4_EGR_UG           ((l_u8)0x01)
#endif

#ifdef LIN_USE_HARDWARE_TIMER
  /*
   * the timer counter register
   * Note: there are two registers on STM8 giving the timer value
   *       LIN will use the register that does clear the
   *       overflow flag while reading
   */
  
  /* An update interrupt request is sent only when counter reaches the overflow/underflow */
  #define LIN_TIMER_UPDATE_REQ_SOURCE()         (TIM4_CR1 |= TIM4_CR1_URS)
  
  /* enable the 8-bit timer module */
  #define LIN_TIMER_ENABLE()                    (TIM4_CR1 |= TIM4_CR1_CEN)

  /* disable the 8-bit timer module */
  /* Begin BUGZILLA_472 */
  #define LIN_TIMER_DISABLE()                   (TIM4_CR1 &= ((volatile l_u8) ~(TIM4_CR1_CEN)))
  /* End BUGZILLA_472 */
  #define LIN_START_TIMER_ARCH(A) \
    do \
    {  \
      LIN_SET_PRESCALER(); \
      LIN_RESTART_TIMER_ARCH(A); \
      LIN_TIMER_INTERRUPT_ENABLE(); \
    } while (0)
  
#if ((LIN_BOARD_CPU_FREQ_HZ == 16000000) && (LIN_TIME_BASE_IN_MS == 2))
  /* 16MHz / 128 / 250 => 2ms timebase */
  #define LIN_SET_PRESCALER()           (TIM4_PSCL = 7)
  #define LIN_RESTART_TIMER_ARCH(A)     (TIM4_ARR = 250)
#elif ((LIN_BOARD_CPU_FREQ_HZ == 8000000) && (LIN_TIME_BASE_IN_MS == 2))
  /* 8MHz / 128 / 125 => 2ms timebase */
  #define LIN_SET_PRESCALER()           (TIM4_PSCL = 7)  
  #define LIN_RESTART_TIMER_ARCH(A)     (TIM4_ARR = 125)
#elif ((LIN_BOARD_CPU_FREQ_HZ == 16000000) && (LIN_TIME_BASE_IN_MS == 1))
  /* 16MHz / 64 / 250 => 1ms timebase used only for conformance tests */
  #define LIN_SET_PRESCALER()           (TIM4_PSCL = 6)
  #define LIN_RESTART_TIMER_ARCH(A)     (TIM4_ARR = 250)
#elif ((LIN_BOARD_CPU_FREQ_HZ == 8000000) && (LIN_TIME_BASE_IN_MS == 1))
  /* 8MHz / 32 / 250 => 1ms timebase used only for conformance tests */
  #define LIN_SET_PRESCALER()           (TIM4_PSCL = 5)  
  #define LIN_RESTART_TIMER_ARCH(A)     (TIM4_ARR = 250)
#else
  #error "Prescaler values are calculated only for a frequency of 8 or 16 MHZ!"
#endif

  /* 2MHz / 16 / 125 => 1ms timebase */
//  #define LIN_SET_PRESCALER()           (TIM4_PSCL = 4)  
//  #define LIN_RESTART_TIMER_ARCH(A)     (TIM4_ARR = 125)

  /* 10MHz / 64 / 156 => 1ms timebase */
//  #define LIN_SET_PRESCALER()           (TIM4_PSCL = 6)
//  #define LIN_RESTART_TIMER_ARCH(A)     (TIM4_ARR = 156)
  
  /* Interrupt enable */
  #define LIN_TIMER_INTERRUPT_ENABLE()  (TIM4_IER |= TIM4_IER_UIE)

  /* Clear flag */
  /* Begin BUGZILLA_472 */
  #define LIN_TIMER_CLEAR_IRQ_FLAG()    (TIM4_SR1 &= ((volatile l_u8) ~(TIM4_SR1_UIF)))
  /* End BUGZILLA_472 */
  #define LIN_CHECK_HARDWARE_TIMER()

#endif /* LIN_USE_HARDWARE_TIMER */

#ifdef SCI1_USED
  #define LIN_SEND_BREAK()              (USART1_CR2 |= USART1_CR2_SBK)
  #define LIN_PARITY_CHECK()            (USART1_SR & USART1_SR_PE)
  #define LIN_PARITY_EN()               (USART1_SR & USART1_CR1_PCEN) /* Parity control enable */
  

	#define LIN_RIE()	(USART1_CR2 |= USART1_CR2_RIEN)
	#define LIN_CLEAR_LDUM()
  /* Begin BUGZILLA_478 */
  #define LIN_MUTE_MODE_DISABLE()       (USART_CR2 &= ((volatile l_u8) ~(USART_CR2_RWU)))
  /* End BUGZILLA_478 */
  
#endif

// #endif 

/*
 * macros for SCI1
 */
/*
 * LINSCI slave functionality is only available on SCI1
 */
#if (defined(SCI1_USED) && defined(LIN_SLAVE_NODE))
  #if (defined(LIN_SLAVE_LINSCI_BREAK_DETECTION) || \
       defined(LIN_SLAVE_LINSCI_FULL)            || \
       defined(LIN_SLAVE_LINSCI_AUTOSYNC)        || \
       defined(LIN_SLAVE_LINSCI_MUTE_MODE))
    #define LIN_SLAVE_LINSCI
    #undef LIN_BAUDRATE_DETECT
    #undef LIN_BAUDRATE_MODE_SLOPPY
  #else
    #define LIN_SLAVE_STANDARD_UART
  #endif
#else
  #undef LIN_SLAVE_LINSCI_BREAK_DETECTION
  #undef LIN_SLAVE_LINSCI_FULL
  #undef LIN_SLAVE_LINSCI_AUTOSYNC
  #undef LIN_SLAVE_LINSCI_MUTE_MODE

  #define LIN_SLAVE_STANDARD_UART
#endif /* SCI2_USED */

/* macro to reset nominal baudrate for LINSCI */
#ifdef LIN_SLAVE_LINSCI_AUTOSYNC
  #define LIN_SLAVE_RESET_NOMINAL_BAUDRATE() \
          do {                               \
               LINUART_BRR1 = l_storeBRR1;    \
             } while (0)
#else
  #define LIN_SLAVE_RESET_NOMINAL_BAUDRATE()
#endif /* LIN_SLAVE_LINSCI_AUTOSYNC */

/*
 * macros for SCI1
 */
/* byte received? */
#define l_byteReceivedArch_SCI1()       (USART1_SR & USART1_SR_RXNE)

/* serial error: OR (overrun) or FE (framing) error */
#define l_serialErrorArch_SCI1()        ((USART1_SR & USART1_SR_FE) || (USART1_SR & USART1_SR_OR))
/*********************************************************************
 * global typedefs
 *********************************************************************/

/*********************************************************************
 * global constants
 *********************************************************************/

/*********************************************************************
 * global variables
 *********************************************************************/
#ifdef LIN_SLAVE_LINSCI_AUTOSYNC
  extern l_u8                   l_storeBRR1;
#endif /* LIN_SLAVE_LINSCI_AUTOSYNC */

/*********************************************************************
 * global functions
 *********************************************************************/

#ifdef SCI1_USED

  extern STACK l_bool l_ifc_initArch_SCI1(void);

  extern STACK l_bool l_ifc_connectArch_SCI1(void);

  extern STACK l_bool l_ifc_disconnectArch_SCI1(void);

  extern STACK void   l_txCharArch_SCI1(l_u8 txChar);

  extern STACK void   l_ifc_rx_SCI1(void);

  extern STACK void   l_ifc_tx_SCI1(void);

  extern STACK void   l_changeBaudrateArch_SCI1(l_u16 baudrate);
#endif


#if (defined(SCI0_USED) || defined (SCI2_USED) || defined(SCI3_USED) || defined(SCI4_USED) ||defined(SCI5_USED) || defined(SCI6_USED) || defined(SCI7_USED) || defined(SCI8_USED) || defined(SCI9_USED))
  #error "For STM8L only SCI1 is valid"
#endif

#endif  /* LIN_STM8_H */

/*
 * eof lin_stm8.h
 */




