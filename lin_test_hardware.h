/*********************************************************************
 *
 * Project:      LIN 2.1 driver suite - test suite
 *
 * File name:    lin_test_hardware.h
 *
 * Copyright:    (c) STMicroelectronics
 *
 * Author:       Giuseppe Stefano Fazio, STMicroelectronics
 *
 * Description:  test-hardware specific settings
 *
 * Target:       STM STM8
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
 * 20071218  v1.0   gsf  - Initial version for STM8
 * 20090420  v1.1   gsf  - BUGZILLA_472: LIN driver does not compile if 
 *                         Cosmic "strict" option is enabled.
 *
 *********************************************************************/

/*********************************************************************
 * includes
 *********************************************************************/
#include "lin_def_stm8.h" 

/*********************************************************************
 * register definitions
 *********************************************************************/

/********************************************
 * GPIOA   General Purpose I/O ports
 ********************************************/
#ifdef __COSMIC__
volatile l_u8 	GPIOA_ODR		@0x5000;	/* Output data register		*/
volatile l_u8 	GPIOA_IDR		@0x5001;	/* Pin input register		*/
volatile l_u8 	GPIOA_DDR		@0x5002;	/* Data direction register	*/
volatile l_u8 	GPIOA_CR1		@0x5003;	/* Option register		*/
volatile l_u8 	GPIOA_CR2 	        @0x5004;	/* Control register		*/
#endif

#ifdef __RAISONANCE__
at 0x5000 hreg 	GPIOA_ODR		;	/* Output data register		*/
at 0x5001 hreg 	GPIOA_IDR		;	/* Pin input register		*/
at 0x5002 hreg 	GPIOA_DDR		;	/* Data direction register	*/
at 0x5003 hreg 	GPIOA_CR1		;	/* Option register		*/
at 0x5004 hreg 	GPIOA_CR2 	        ;	/* Control register		*/
#endif

#ifdef __IAR__
__no_init volatile l_u8 	GPIOA_ODR		@0x5000;	/* Output data register		*/
__no_init volatile l_u8 	GPIOA_IDR		@0x5001;	/* Pin input register		*/
__no_init volatile l_u8 	GPIOA_DDR		@0x5002;	/* Data direction register	*/
__no_init volatile l_u8 	GPIOA_CR1		@0x5003;	/* Option register		*/
__no_init volatile l_u8 	GPIOA_CR2 	@0x5004;	/* Control register		*/
#endif

/********************************************
 * GPIOB   General Purpose I/O ports
 ********************************************/
#ifdef __COSMIC__
volatile l_u8 	GPIOB_ODR		@0x5005;	/* Output data register		*/
volatile l_u8 	GPIOB_IDR		@0x5006;	/* Pin input register		*/
volatile l_u8 	GPIOB_DDR		@0x5007;	/* Data direction register	*/
volatile l_u8 	GPIOB_CR1		@0x5008;	/* Option register		*/
volatile l_u8 	GPIOB_CR2		@0x5009;	/* Control register		*/
#endif

#ifdef __RAISONANCE__
at 0x5005 hreg 	GPIOB_ODR		;	/* Output data register		*/
at 0x5006 hreg 	GPIOB_IDR		;	/* Pin input register		*/
at 0x5007 hreg 	GPIOB_DDR		;	/* Data direction register	*/
at 0x5008 hreg 	GPIOB_CR1		;	/* Option register		*/
at 0x5009 hreg  GPIOB_CR2		;	/* Control register		*/
#endif

#ifdef __IAR__
__no_init volatile l_u8 	GPIOB_ODR		@0x5005;	/* Output data register		*/
__no_init volatile l_u8 	GPIOB_IDR		@0x5006;	/* Pin input register		*/
__no_init volatile l_u8 	GPIOB_DDR		@0x5007;	/* Data direction register	*/
__no_init volatile l_u8 	GPIOB_CR1		@0x5008;	/* Option register		*/
__no_init volatile l_u8 	GPIOB_CR2		@0x5009;	/* Control register		*/
#endif


/********************************************
 * GPIOC   General Purpose I/O ports
 ********************************************/
#ifdef __COSMIC__
volatile l_u8 	GPIOC_ODR		@0x500A;	/* Output data register		*/
volatile l_u8 	GPIOC_IDR		@0x500B;	/* Pin input register		*/
volatile l_u8 	GPIOC_DDR		@0x500C;	/* Data direction register	*/
volatile l_u8 	GPIOC_CR1		@0x500D;	/* Option register		*/
volatile l_u8 	GPIOC_CR2		@0x500E;	/* Control register		*/
#endif

#ifdef __RAISONANCE__
at 0x500A hreg 	GPIOC_ODR		;	/* Output data register		*/
at 0x500B hreg 	GPIOC_IDR		;	/* Pin input register		*/
at 0x500C hreg 	GPIOC_DDR		;	/* Data direction register	*/
at 0x500D hreg 	GPIOC_CR1		;	/* Option register		*/
at 0x500E hreg 	GPIOC_CR2		;	/* Control register		*/
#endif

#ifdef __IAR__
__no_init volatile l_u8 	GPIOC_ODR		@0x500A;	/* Output data register		*/
__no_init volatile l_u8 	GPIOC_IDR		@0x500B;	/* Pin input register		*/
__no_init volatile l_u8 	GPIOC_DDR		@0x500C;	/* Data direction register	*/
__no_init volatile l_u8 	GPIOC_CR1		@0x500D;	/* Option register		*/
__no_init volatile l_u8 	GPIOC_CR2		@0x500E;	/* Control register		*/
#endif

/********************************************
 * GPIOD   General Purpose I/O ports
 ********************************************/
#ifdef __COSMIC__
volatile l_u8 	GPIOD_ODR		@0x500F;	/* Output data register		*/
volatile l_u8 	GPIOD_IDR		@0x5010;	/* Pin input register		*/
volatile l_u8 	GPIOD_DDR		@0x5011;	/* Data direction register	*/
volatile l_u8 	GPIOD_CR1		@0x5012;	/* Option register		*/
volatile l_u8 	GPIOD_CR2		@0x5013;	/* Control register		*/
#endif

#ifdef __RAISONANCE__
at 0x500F hreg 	GPIOD_ODR		;	/* Output data register		*/
at 0x5010 hreg 	GPIOD_IDR		;	/* Pin input register		*/
at 0x5011 hreg 	GPIOD_DDR		;	/* Data direction register	*/
at 0x5012 hreg 	GPIOD_CR1		;	/* Option register		*/
at 0x5013 hreg 	GPIOD_CR2		;	/* Control register		*/
#endif

#ifdef __IAR__
__no_init volatile l_u8 	GPIOD_ODR		@0x500F;	/* Output data register		*/
__no_init volatile l_u8 	GPIOD_IDR		@0x5010;	/* Pin input register		*/
__no_init volatile l_u8 	GPIOD_DDR		@0x5011;	/* Data direction register	*/
__no_init volatile l_u8 	GPIOD_CR1		@0x5012;	/* Option register		*/
__no_init volatile l_u8 	GPIOD_CR2		@0x5013;	/* Control register		*/
#endif

/********************************************
 * GPIOE   General Purpose I/O ports
 ********************************************/
#ifdef __COSMIC__
volatile l_u8 	GPIOE_ODR		@0x5014;	/* Output data register		*/
volatile l_u8 	GPIOE_IDR		@0x5015;	/* Pin input register		*/
volatile l_u8 	GPIOE_DDR		@0x5016;	/* Data direction register	*/
volatile l_u8 	GPIOE_CR1		@0x5017;	/* Option register		*/
volatile l_u8 	GPIOE_CR2		@0x5018;	/* Control register		*/
#endif

#ifdef __RAISONANCE__
at 0x5014 hreg 	GPIOE_ODR		;	/* Output data register		*/
at 0x5015 hreg 	GPIOE_IDR		;	/* Pin input register		*/
at 0x5016 hreg 	GPIOE_DDR		;	/* Data direction register	*/
at 0x5017 hreg 	GPIOE_CR1		;	/* Option register		*/
at 0x5018 hreg 	GPIOE_CR2		;	/* Control register		*/
#endif

#ifdef __IAR__
__no_init volatile l_u8 	GPIOE_ODR		@0x5014;	/* Output data register		*/
__no_init volatile l_u8 	GPIOE_IDR		@0x5015;	/* Pin input register		*/
__no_init volatile l_u8 	GPIOE_DDR		@0x5016;	/* Data direction register	*/
__no_init volatile l_u8 	GPIOE_CR1		@0x5017;	/* Option register		*/
__no_init volatile l_u8 	GPIOE_CR2		@0x5018;	/* Control register		*/
#endif

/********************************************
 * GPIOF   General Purpose I/O ports
 ********************************************/
#ifdef __COSMIC__
volatile l_u8 	GPIOF_ODR		@0x5019;	/* Output data register		*/
volatile l_u8 	GPIOF_IDR		@0x501A;	/* Pin input register		*/
volatile l_u8 	GPIOF_DDR		@0x501B;	/* Data direction register	*/
volatile l_u8 	GPIOF_CR1		@0x501C;	/* Option register		*/
volatile l_u8 	GPIOF_CR2		@0x501D;	/* Control register		*/
#endif

#ifdef __RAISONANCE__
at 0x5019 hreg 	GPIOF_ODR		;	/* Output data register		*/
at 0x501A hreg 	GPIOF_IDR		;	/* Pin input register		*/
at 0x501B hreg 	GPIOF_DDR		;	/* Data direction register	*/
at 0x501C hreg 	GPIOF_CR1		;	/* Option register		*/
at 0x501D hreg 	GPIOF_CR2		;	/* Control register		*/
#endif

#ifdef __IAR__
__no_init volatile l_u8 	GPIOF_ODR		@0x5019;	/* Output data register		*/
__no_init volatile l_u8 	GPIOF_IDR		@0x501A;	/* Pin input register		*/
__no_init volatile l_u8 	GPIOF_DDR		@0x501B;	/* Data direction register	*/
__no_init volatile l_u8 	GPIOF_CR1		@0x501C;	/* Option register		*/
__no_init volatile l_u8 	GPIOF_CR2		@0x501D;	/* Control register		*/
#endif

/********************************************
 * GPIOG   General Purpose I/O ports
 ********************************************/
#ifdef __COSMIC__
volatile l_u8 	GPIOG_ODR		@0x501E;	/* Output data register		*/
volatile l_u8 	GPIOG_IDR		@0x501F;	/* Pin input register		*/
volatile l_u8 	GPIOG_DDR		@0x5020;	/* Data direction register	*/
volatile l_u8 	GPIOG_CR1		@0x5021;	/* Option register		*/
volatile l_u8 	GPIOG_CR2		@0x5022;	/* Control register		*/
#endif

#ifdef __RAISONANCE__
at 0x501E hreg 	GPIOG_ODR		;	/* Output data register		*/
at 0x501F hreg 	GPIOG_IDR		;	/* Pin input register		*/
at 0x5020 hreg 	GPIOG_DDR		;	/* Data direction register	*/
at 0x5021 hreg 	GPIOG_CR1		;	/* Option register		*/
at 0x5022 hreg 	GPIOG_CR2		;	/* Control register		*/
#endif

#ifdef __IAR__
__no_init volatile l_u8 	GPIOG_ODR		@0x501E;	/* Output data register		*/
__no_init volatile l_u8 	GPIOG_IDR		@0x501F;	/* Pin input register		*/
__no_init volatile l_u8 	GPIOG_DDR		@0x5020;	/* Data direction register	*/
__no_init volatile l_u8 	GPIOG_CR1		@0x5021;	/* Option register		*/
__no_init volatile l_u8 	GPIOG_CR2		@0x5022;	/* Control register		*/
#endif

/********************************************
 * GPIOH   General Purpose I/O ports
 ********************************************/
#ifdef __COSMIC__
volatile l_u8 	GPIOH_ODR		@0x5023;	/* Output data register		*/
volatile l_u8 	GPIOH_IDR		@0x5024;	/* Pin input register		*/
volatile l_u8 	GPIOH_DDR		@0x5025;	/* Data direction register	*/
volatile l_u8 	GPIOH_CR1		@0x5026;	/* Option register		*/
volatile l_u8 	GPIOH_CR2		@0x5027;	/* Control register		*/
#endif

#ifdef __RAISONANCE__
at 0x5023 hreg 	GPIOH_ODR		;	/* Output data register		*/
at 0x5024 hreg 	GPIOH_IDR		;	/* Pin input register		*/
at 0x5025 hreg 	GPIOH_DDR		;	/* Data direction register	*/
at 0x5026 hreg 	GPIOH_CR1		;	/* Option register		*/
at 0x5027 hreg 	GPIOH_CR2		;	/* Control register		*/
#endif

#ifdef __IAR__
__no_init volatile l_u8 	GPIOH_ODR		@0x5023;	/* Output data register		*/
__no_init volatile l_u8 	GPIOH_IDR		@0x5024;	/* Pin input register		*/
__no_init volatile l_u8 	GPIOH_DDR		@0x5025;	/* Data direction register	*/
__no_init volatile l_u8 	GPIOH_CR1		@0x5026;	/* Option register		*/
__no_init volatile l_u8 	GPIOH_CR2		@0x5027;	/* Control register		*/
#endif

/********************************************
 * GPIOI   General Purpose I/O ports
 ********************************************/
#ifdef __COSMIC__
volatile l_u8 	GPIOI_ODR		@0x5028;	/* Output data register		*/
volatile l_u8 	GPIOI_IDR		@0x5029;	/* Pin input register		*/
volatile l_u8 	GPIOI_DDR		@0x502A;	/* Data direction register	*/
volatile l_u8 	GPIOI_CR1		@0x502B;	/* Option register		*/
volatile l_u8 	GPIOI_CR2		@0x502C;	/* Control register		*/

volatile l_u8 	EXTI_CR1	  @0x50A0;	/* External interrupt		*/
volatile l_u8 	EXTI_CR2    @0x50A1;	/* External interrupt		*/

volatile l_u8 	CLK_ICKC		@0x50C0;	/** Internal Clock Control  **/  
volatile l_u8 	CLK_ECKC		@0x50C1;	/* External Clock Control	*/
volatile l_u8 	CLK_SWI		        @0x50C4;	/* Switch Control		*/
volatile l_u8 	CLK_SWIC		@0x50C5;	/* Switch Control		*/
volatile l_u8 	CLK_CKDIV		@0x50C6;
volatile l_u8 	CLK_CCO			@0x50C9;	/* select fcpu for CCO output */


#endif

#ifdef __RAISONANCE__
at 0x5028 hreg 	GPIOI_ODR		;	/* Output data register		*/
at 0x5029 hreg 	GPIOI_IDR		;	/* Pin input register		*/
at 0x502A hreg 	GPIOI_DDR		;	/* Data direction register	*/
at 0x502B hreg 	GPIOI_CR1		;	/* Option register		*/
at 0x502C hreg 	GPIOI_CR2		;	/* Control register		*/
 
at 0x50A0 hreg 	EXTI_CR1	  ;	/* External interrupt		*/
at 0x50A1 hreg 	EXTI_CR2    ;	/* External interrupt		*/

at 0x50C0 hreg 	CLK_ICKC		;	/* Internal Clock Control	*/
at 0x50C1 hreg 	CLK_ECKC		;	/* External Clock Control	*/
at 0x50C4 hreg 	CLK_SWI		  ;	/* Switch Control		*/
at 0x50C5 hreg 	CLK_SWIC		;	/* Switch Control		*/
at 0x50C6 hreg 	CLK_CKDIV		;
at 0x50C9 hreg 	CLK_CCO			;	/* select fcpu for CCO output */
#endif

#ifdef __IAR__
__no_init volatile l_u8 	GPIOI_ODR		@0x5028;	/* Output data register		*/
__no_init volatile l_u8 	GPIOI_IDR		@0x5029;	/* Pin input register		*/
__no_init volatile l_u8 	GPIOI_DDR		@0x502A;	/* Data direction register	*/
__no_init volatile l_u8 	GPIOI_CR1		@0x502B;	/* Option register		*/
__no_init volatile l_u8 	GPIOI_CR2		@0x502C;	/* Control register		*/

__no_init volatile l_u8 	EXTI_CR1	  @0x50A0;	/* External interrupt		*/
__no_init volatile l_u8 	EXTI_CR2    @0x50A1;	/* External interrupt		*/
 
__no_init volatile l_u8 	CLK_ICKC		@0x50C0;	/* Internal Clock Control	*/ 
__no_init volatile l_u8 	CLK_ECKC		@0x50C1;	/* External Clock Control	*/
__no_init volatile l_u8 	CLK_SWI		  @0x50C4;	/* Switch Control		*/
__no_init volatile l_u8 	CLK_SWIC		@0x50C5;	/* Switch Control		*/
__no_init volatile l_u8 	CLK_CKDIV		@0x50C6;
__no_init volatile l_u8 	CLK_CCO			@0x50C9;	/* select fcpu for CCO output */
#endif



__no_init volatile l_u8 	CLK_PCKENR1			@0x50C7;
__no_init volatile l_u8 	TIM1_EGR			@0x5257;
__no_init volatile l_u8 	TIM1_CR1			@0x5250;
__no_init volatile l_u8 	TIM1_RCR			@0x5264;
__no_init volatile l_u8 	TIM1_PSCRH			@0x5260;
__no_init volatile l_u8 	TIM1_PSCRL			@0x5261;
__no_init volatile l_u8 	TIM1_ARRH			@0x5262;
__no_init volatile l_u8 	TIM1_ARRL			@0x5263;
__no_init volatile l_u8 	TIM1_CCER2			@0x525D;
__no_init volatile l_u8 	TIM1_CCMR3			@0x525A;
__no_init volatile l_u8 	TIM1_CCR3H			@0x5269;
__no_init volatile l_u8 	TIM1_CCR3L			@0x526A;
__no_init volatile l_u8 	TIM1_BKR			@0x526D;


/********************************************
 * SPI
 ********************************************/
#ifdef __COSMIC__
volatile l_u8 	SPI_CR1			@0x5200;
volatile l_u8 	SPI_CR2			@0x5201;
volatile l_u8 	SPI_ICR			@0x5202;
volatile l_u8 	SPI_SR			@0x5203;
volatile l_u8 	SPI_DR			@0x5204;
volatile l_u8 	SPI_CRCPR		@0x5205;
volatile l_u8 	SPI_RXCRCR	@0x5206;
volatile l_u8 	SPI_TXCRCR	@0x5207;
#endif

#ifdef __RAISONANCE__
at 0x5200 hreg 	SPI_CR1			;
at 0x5201 hreg 	SPI_CR2			;
at 0x5202 hreg 	SPI_ICR			;
at 0x5203 hreg 	SPI_SR			;
at 0x5204 hreg 	SPI_DR			;
at 0x5205 hreg 	SPI_CRCPR		;
at 0x5206 hreg 	SPI_RXCRCR	;
at 0x5207 hreg 	SPI_TXCRCR	;
#endif

#ifdef __IAR__
__no_init volatile l_u8 	SPI_CR1			@0x5200;
__no_init volatile l_u8 	SPI_CR2			@0x5201;
__no_init volatile l_u8 	SPI_ICR			@0x5202;
__no_init volatile l_u8 	SPI_SR			@0x5203;
__no_init volatile l_u8 	SPI_DR			@0x5204;
__no_init volatile l_u8 	SPI_CRCPR		@0x5205;
__no_init volatile l_u8 	SPI_RXCRCR	@0x5206;
__no_init volatile l_u8 	SPI_TXCRCR	@0x5207;
#endif

/*********************************************************************
 * global functions
 *********************************************************************/
STACK void linIrqDisable
(
   void
);

STACK void linIrqEnable
(
   void
);


#ifdef STM8AF622x        /***** STM8AF622x acting like a slave  ****/

/************************ALED1642 PIN definision************************/    
#define SPI_CLK_HIGH            GPIOC_ODR |= 0x20    
#define SPI_CLK_LOW             GPIOC_ODR &= 0xdf

#define SPI_LE_HIGH             GPIOC_ODR |= 0x08
#define SPI_LE_LOW              GPIOC_ODR &= 0xF7

#define SPI_DAT_HIGH            GPIOC_ODR |= 0x40
#define SPI_DAT_LOW             GPIOC_ODR &= 0xBF
     

/*********************************************************************
 * handling of LED
 *********************************************************************/
/* Begin BUGZILLA_472 */

     #define LIN_LEDS_INIT()                \
      do                                    \
      {                                     \
       (GPIOD_ODR &= ((volatile char) ~(0x0D))); \
        GPIOD_DDR |= 0x0D;                  \
        GPIOD_CR1 |= 0x0D;                  \
      } while (0)
/* End BUGZILLA_472 */

#define LIN_LEDS_SET(A)                     \
      do                                    \
      {                                     \
        (GPIOD_ODR = (A));                  \
      } while (0)


#define LIN_LEDS_SET_MASK(A)                \
      do                                    \
      {                                     \
        (GPIOD_ODR |= (A));                 \
      } while (0)

/* Begin BUGZILLA_472 */
#define LIN_LEDS_CLEAR_MASK(A)              \
      do                                    \
      {                                     \
        (GPIOD_ODR &= ((volatile char) ~(A))); \
      } while (0)
/* End BUGZILLA_472 */

#define LIN_LEDS_TOGGLE_MASK(A)             \
       do                                   \
       {                                    \
         (GPIOD_ODR ^= (A));                \
       } while(0)

#define LIN_LEDS_TICK_MASK(A)               \
       do                                   \
       {                                    \
         (GPIOD_ODR ^= (A));                \
         (GPIOD_ODR ^= (A));                \
       } while(0)

#define LIN_LEDS_GET(A)    ((l_u8) (GPIOD_ODR))

#define LIN_DIPS_GET()    ((l_u8) (GPIOB_IDR))         


/*********************************************************************
 * watchdog handling
 *********************************************************************/
#define LIN_WATCHDOG_TRIGGER()

/*********************************************************************
 * LIN enable on STM8-EVAL board
 *********************************************************************/
/* Begin BUGZILLA_472 */
#define LIN_MULTIPLEXER_1()                 \
       do                                   \
       {                                    \
         (GPIOG_ODR &= ((volatile char) ~(0x40))); \
         (GPIOG_DDR |= 0x40);               \
         (GPIOG_CR1 |= 0x40);               \
       } while(0)
       
#define USART_MULTIPLEXER_1_2()             \
       do                                   \
       {                                    \
         (GPIOF_ODR &= ((volatile char) ~(0x60))); \
         (GPIOF_DDR |= 0x60);               \
         (GPIOF_CR1 |= 0x60);               \
       } while(0)
/* End BUGZILLA_472 */

#define LIN_EN_1()                          \
       do                                   \
       {                                    \
         (GPIOF_ODR |= 0x80);               \
         (GPIOF_DDR |= 0x80);               \
         (GPIOF_CR1 |= 0x80);               \
       } while(0)


/*****************  PC3  ********************/           

#define LIN_EN_2()                          \
       do                                   \
       {                                    \
         (GPIOC_ODR |= 0x08);               \
         (GPIOC_DDR |= 0x08);               \
         (GPIOC_CR1 |= 0x08);               \
       } while(0)  


/*********************************************************************
 * External Interrupt enable on 'Joystick' (B7..B4 ports)
 *********************************************************************/
#define JOYSTICK_INIT()                     \
       do                                   \
       {                                    \
         (GPIOB_DDR &= 0x0F);               \
         (GPIOB_CR1 |= 0xF0);               \
         (GPIOB_CR2 |= 0xF0);               \
         (EXTI_CR1  |= 0x04);               \
       } while(0)

/*********************************************************************
 * External Interrupt enable on button 'Reset' (PC4 port)
 *********************************************************************/    

#define BUTTON_INIT()                       \
       do                                   \
       {                                    \
         (GPIOC_DDR &= 0xEF);               \
         (GPIOC_CR1 |= 0x10);               \
         (GPIOC_CR2 |= 0x10);               \
         (EXTI_CR1  |= 0x10);               \
       } while(0)

#endif

#ifndef STM8AF622x                 /***** STM8/128-Eval Board acting like slave ****/



/*********************************************************************
 * handling of LED
 *********************************************************************/
/* Begin BUGZILLA_472 */
#define LIN_LEDS_INIT()                     \
      do                                    \
      {                                     \
       (GPIOH_ODR &= ((volatile char) ~(0x0F))); \
        GPIOH_DDR |= 0x0F;                  \
        GPIOH_CR1 |= 0x0F;                  \
      } while (0)
/* End BUGZILLA_472 */

#define LIN_LEDS_SET(A)                     \
      do                                    \
      {                                     \
        (GPIOH_ODR = (A));                  \
      } while (0)


#define LIN_LEDS_SET_MASK(A)                \
      do                                    \
      {                                     \
        (GPIOH_ODR |= (A));                 \
      } while (0)

/* Begin BUGZILLA_472 */
#define LIN_LEDS_CLEAR_MASK(A)              \
      do                                    \
      {                                     \
        (GPIOH_ODR &= ((volatile char) ~(A))); \
      } while (0)
/* End BUGZILLA_472 */

#define LIN_LEDS_TOGGLE_MASK(A)             \
       do                                   \
       {                                    \
         (GPIOH_ODR ^= (A));                \
       } while(0)

#define LIN_LEDS_TICK_MASK(A)               \
       do                                   \
       {                                    \
         (GPIOH_ODR ^= (A));                \
         (GPIOH_ODR ^= (A));                \
       } while(0)

#define LIN_LEDS_GET(A)    ((l_u8) (GPIOH_ODR))

#define LIN_DIPS_GET()    ((l_u8) (GPIOI_IDR))

/*********************************************************************
 * watchdog handling
 *********************************************************************/
#define LIN_WATCHDOG_TRIGGER()

/*********************************************************************
 * LIN enable on STM8-EVAL board
 *********************************************************************/
/* Begin BUGZILLA_472 */
#define LIN_MULTIPLEXER_1()                 \
       do                                   \
       {                                    \
         (GPIOG_ODR &= ((volatile char) ~(0x40))); \
         (GPIOG_DDR |= 0x40);               \
         (GPIOG_CR1 |= 0x40);               \
       } while(0)
       
#define USART_MULTIPLEXER_1_2()             \
       do                                   \
       {                                    \
         (GPIOF_ODR &= ((volatile char) ~(0x60))); \
         (GPIOF_DDR |= 0x60);               \
         (GPIOF_CR1 |= 0x60);               \
       } while(0)
/* End BUGZILLA_472 */

#define LIN_EN_1()                          \
       do                                   \
       {                                    \
         (GPIOF_ODR |= 0x80);               \
         (GPIOF_DDR |= 0x80);               \
         (GPIOF_CR1 |= 0x80);               \
       } while(0)

#define LIN_EN_2()                          \
       do                                   \
       {                                    \
         (GPIOG_ODR |= 0x08);               \
         (GPIOG_DDR |= 0x08);               \
         (GPIOG_CR1 |= 0x08);               \
       } while(0)

/*********************************************************************
 * External Interrupt enable on 'Joystick' (B7..B4 ports)
 *********************************************************************/
#define JOYSTICK_INIT()                     \
       do                                   \
       {                                    \
         (GPIOB_DDR &= 0x0F);               \
         (GPIOB_CR1 |= 0xF0);               \
         (GPIOB_CR2 |= 0xF0);               \
         (EXTI_CR1  |= 0x04);               \
       } while(0)

/*********************************************************************
 * External Interrupt enable on button 'Reset' (C0 port)
 *********************************************************************/
#define BUTTON_INIT()                       \
       do                                   \
       {                                    \
         (GPIOC_DDR &= 0xFE);               \
         (GPIOC_CR1 |= 0x01);               \
         (GPIOC_CR2 |= 0x01);               \
         (EXTI_CR1  |= 0x10);               \
       } while(0)

#endif


/*********************************************************************
 * Top Level interrupt enable
 *********************************************************************/
#define TLI_ENABLE()      (EXTI_CR2  |= 0x04)
