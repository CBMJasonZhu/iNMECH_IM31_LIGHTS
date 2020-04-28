/*********************************************************************
 *
 * Project:      LIN 2.1 driver suite - test suite
 *
 * File name:    interrupt_vector.c
 *
 * Copyright:    (c) STMicroelectronics
 *
 * Author:       Giuseppe Stefano Fazio, STMicroelectronics
 *
 * Description:  target specific application frame
 *
 * Target:       STM STM8
 *
 * Names:        gsf  Giuseppe Stefano Fazio  STMicroelectronics
 *
 * NOT TO BE CHANGED BY THE USER
 *
 *********************************************************************/

/*********************************************************************
 * History:
 *
 * 20071219  gsf  v1.0  initial version for STM8
 *
 *********************************************************************/
 
 
/*********************************************************************
 * includes
 *********************************************************************/

#include "lin_def_stm8.h"


/*  should be the name of a dummy interrupt routine
 *  in order to catch properly unexpected interrupts
 */
#define NULL 0

extern @near void _stext();  /* startup routine */
extern void l_timer4ISR(void);  /* TIMER 4 */
extern void l_serial2ISR(void); /* LINUART RX */
extern void l_serial1ISR(void); /* USART RX */
extern void PortB(void);
extern void PortC(void);

extern void DummyInt(void);

#pragma section const {vector}

#ifdef STM8AF622x
void (* const @vector _vectab[32])() = {
  (void (*)())_stext,  /* RESET       */
  DummyInt,                /* TRAP        */
  DummyInt,                /* TLI         */
  DummyInt,                /* AWU         */
  DummyInt,                /* CLK         */
  DummyInt,                /* EXTI PORTA  */
  PortB,                   /* EXTI PORTB  */
  PortC,                   /* EXTI PORTC  */
  DummyInt,                /* EXTI PORTD  */
  DummyInt,                /* EXTI PORTE  */
  DummyInt,                /* EXTI PORTF  */
  DummyInt,                /* Reserved    */
  DummyInt,                /* SPI END of Transfer*/
  DummyInt,                /* TIM1 OVF    */
  DummyInt,                /* TIM1 CAP    */
  DummyInt,                /* TIM5 OVF    */
  DummyInt,                /* TIM5 CAP    */
  DummyInt,                /* Reserved    */
  DummyInt,                /* Reserved    */
  DummyInt,                /* LINUART TX    */
  l_serial1ISR,            /* LINUART RX */
  DummyInt,                /* I2C         */
  DummyInt,                /* Reserved  */
  DummyInt,                /* Reserved */
  DummyInt,                /* ADC         */
  l_timer4ISR,             /* TIM4 OVF    */
  DummyInt,                /* EEPROM ECC  */
  DummyInt,                /* Reserved    */
  DummyInt,                /* Reserved    */
  DummyInt,                /* Reserved    */
  DummyInt,                /* Reserved    */
  DummyInt,                /* Reserved    */        
  };

#else /******* MCU other than STM8AF 8K DIE *****/

void (* const @vector _vectab[32])() = {
  (void (*)())_stext,  /* RESET       */
  DummyInt,                /* TRAP        */
  DummyInt,                /* TLI         */
  DummyInt,                /* AWU         */
  DummyInt,                /* CLK         */
  DummyInt,                /* EXTI PORTA  */
  PortB,               /* EXTI PORTB  */
  PortC,               /* EXTI PORTC  */
  DummyInt,                /* EXTI PORTD  */
  DummyInt,                /* EXTI PORTE  */
  DummyInt,                /* CAN RX      */
  DummyInt,                /* CAN TX      */
  DummyInt,                /* SPI         */
  DummyInt,                /* TIM1 OVF    */
  DummyInt,                /* TIM1 CAP    */
  DummyInt,                /* TIM2 OVF    */
  DummyInt,                /* TIM2 CAP    */
  DummyInt,                /* TIM3 OVF    */
  DummyInt,                /* TIM3 CAP    */
  DummyInt,                /* USART TX    */
  l_serial2ISR,        /* USART RX    */  
  DummyInt,                /* I2C         */
  DummyInt,                /* LINUART TX  */
  l_serial1ISR,        /* LINUART RX  */  
  DummyInt,                /* ADC         */
  l_timer4ISR,         /* TIM4 OVF    */
  DummyInt,                /* EEPROM ECC  */
  DummyInt,                /* Reserved    */
  DummyInt,                /* Reserved    */
  DummyInt,                /* Reserved    */
  DummyInt,                /* Reserved    */
  DummyInt,                /* Reserved    */
  };
#endif