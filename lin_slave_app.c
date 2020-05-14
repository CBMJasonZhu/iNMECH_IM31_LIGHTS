/*********************************************************************
 *
 * Project:      LIN 2.1 driver suite - test suite
 *
 * File name:    lin_slave_app.c
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
 *
 * History:
 *
 * 20071218  v1.0   gsf  - Initial version for STM8
 * 20090319  v1.1   gsf  - BUGZILLA_472: LIN driver does not compile if 
 *                         Cosmic "strict" option is enabled.
 *
 *********************************************************************/

/*********************************************************************
 * include for LIN driver
 *********************************************************************/
//#include <processor.h>
#include "lin.h"
#include "lin_stm8.h"
#include "lin_test_hardware.h"
#include "lin_basic_demo_slave.h"
#include "lin_timer.h"
#include "lin_def_stm8.h" 

#ifdef __RAISONANCE__
 #include <intrist7.h>
#endif

#ifdef __IAR__
 #include <intrinsics.h>
#endif
 

/*********************************************************************
 * functions
 *********************************************************************/
void lin_osHWInit(void);
void lin_osCyclic(void);

/*********************************************************************
 * global defines
 *********************************************************************/
#ifdef __COSMIC__
#define enableInterrupts() {_asm("rim\n");} /* enable interrupts */
#define disableInterrupts() {_asm("sim\n");} /* disable interrupts */
#define NoOperation() {_asm("nop\n");} /* no operation */
#elif __RAISONANCE__
#define enableInterrupts()  _rim_()  /* enable interrupts */
#define disableInterrupts() _sim_()  /* disable interrupts */
#define NoOperation() _nop_()  /* no operation */
#elif __IAR__
#define enableInterrupts() {asm("rim\n");} /* enable interrupts */
#define disableInterrupts() {asm("sim\n");} /* disable interrupts */
#define NoOperation() {asm("nop\n");} /* no operation */
#else
	#error "No compiler defined"
#endif

/*********************************************************************
 * LCD defines
 *********************************************************************/

/* LCD managed as 2 Lines of 15 characters (2Lines * 15Char) */
#define LCD_LINE_MAX_CHAR ((unsigned char)15)
#define LCD_LINE1         ((unsigned char)0x80)
#define LCD_LINE2         ((unsigned char)0x90)

#define ENABLE 1
#define DISABLE 0

#define DOWN	0x80
#define UP 		0x40
#define RIGHT 0x20
#define LEFT 	0x10

/*********************************************************************
 * global variables
 *********************************************************************/
#ifdef __COSMIC__
@tiny unsigned char  l_counterISRdisable = 1;
@tiny unsigned char  irqMask = 0;
@tiny unsigned char  Cyclic = 5;  
@tiny unsigned int   nCount;
@tiny unsigned char  UpdateScreen = 0;
#endif

#ifdef __RAISONANCE__
page0  unsigned char  l_counterISRdisable = 1;
page0 unsigned char   irqMask = 0;
page0 unsigned char   Cyclic = 5; 
page0 unsigned int    nCount;
page0 unsigned char   UpdateScreen = 0;
#endif

#ifdef __IAR__
__tiny unsigned char   l_counterISRdisable = 1;
__tiny unsigned char   irqMask = 0;
__tiny unsigned char   Cyclic = 5; 
__tiny unsigned int    nCount;
__tiny unsigned char   UpdateScreen = 0;
#endif

#if defined(SCI1_USED)
    l_ifc_handle                  ifc_used = LIN_IFC_SCI1;
#elif defined(SCI2_USED)
    l_ifc_handle                  ifc_used = LIN_IFC_SCI2;
#else
    #error "Incorrect SCI_USED! For STM8 only interface 1..2 are supported!"
#endif /* #if defined(SCI1_USED) */

/*********************************************************************
 * LIN callbacks
 *********************************************************************/

/*********************************************************************
 * The LIN standard requires that the application must supply two
 * functions to start and stop system interrupts.
 * The functions below are the implementation of these functions.
 * The defined interface functions are declared in lin_def.h
 */
STACK void linIrqDisable(void)
{
   if (l_counterISRdisable == 0)
   {
	   disableInterrupts();
   }
   l_counterISRdisable++;
}

STACK void linIrqEnable(void)
{
   if (l_counterISRdisable)
   {
     l_counterISRdisable--;
   }

   if (l_counterISRdisable == 0)
   {
	   enableInterrupts();
   }
}

/*********************************************************************
 * ISRs
 *********************************************************************/
/*********************************************************************
 * 
 * The serial ISR of the LIN port
 *
 * The LIN ISR will only call the function l_ifc_rx(), as defined
 * in the LIN standard documentation
 *
 */
/*********************************************************************/
#ifdef __COSMIC__
@interrupt void PortB(void)
{
  /* Max 15 characters per line */
  /*if ((GPIOB_IDR&DOWN)==0) 
	{
		Joystick=DOWN;
		NewJoystickState=1;
	}
	else if ((GPIOB_IDR&UP)==0)
	{
		Joystick=UP; 
		NewJoystickState=1;		
	}
	else if ((GPIOB_IDR&RIGHT)==0)
	{
		Joystick=RIGHT; 
		NewJoystickState=1;
	}
	else if ((GPIOB_IDR&LEFT)==0)
	{
		Joystick=LEFT; 
		NewJoystickState=1;
	}
	else
	{
		NewJoystickState=0;
	}*/
	_asm("nop");
}

#endif


#ifdef __RAISONANCE__
void PortB(void) interrupt 4
{
return ;
}
#endif


#ifdef __IAR__
#pragma vector=0x06
__interrupt void PortB(void)
{
  /* Max 15 characters per line */
  /*if ((GPIOB_IDR&DOWN)==0) 
	{
		Joystick=DOWN;
		NewJoystickState=1;
	}
	else if ((GPIOB_IDR&UP)==0)
	{
		Joystick=UP; 
		NewJoystickState=1;		
	}
	else if ((GPIOB_IDR&RIGHT)==0)
	{
		Joystick=RIGHT; 
		NewJoystickState=1;
	}
	else if ((GPIOB_IDR&LEFT)==0)
	{
		Joystick=LEFT; 
		NewJoystickState=1;
	}
	else
	{
		NewJoystickState=0;
	}*/
     asm("rim\n");

}

#endif

/*********************************************************/
/*******  PORT C Interrupt Routine ***********************/

#ifdef __COSMIC__
@interrupt void PortC(void)
{
  UpdateScreen=1;	
	KeyOnOff ^= 0x01; 
    return ;
}
#endif

#ifdef __RAISONANCE__
void PortC(void) interrupt 5
{
	UpdateScreen=1;	
	KeyOnOff ^= 0x01;    
 return ;
}
#endif

#ifdef __IAR__
#pragma vector=0x07
__interrupt void PortC(void)
{
	UpdateScreen=1;	
	KeyOnOff ^= 0x01;                      
    return ;
}
#endif
/**********************************************************/

/*********************************************************/
/*******  SCI1 Rx Interrupt *************************/

#ifdef __COSMIC__
@interrupt void l_serial1ISR(void)
{
#ifdef SCI1_USED   
   linIrqDisable();
   l_ifc_rx_SCI1();
   linIrqEnable();
#endif
}
#endif

#ifdef __RAISONANCE__
#ifdef STM8AF622x
  void l_serial1ISR(void) interrupt 18
#else
  void l_serial1ISR(void) interrupt 21
#endif
{
#ifdef SCI1_USED
   linIrqDisable();
   l_ifc_rx_SCI1();
   linIrqEnable();
#endif
}
#endif

#ifdef __IAR__
#ifdef STM8AF622x
  #pragma vector=0x14
__interrupt void l_serial1ISR(void)  
#else
  #pragma vector=0x17
__interrupt void l_serial1ISR(void)
#endif
{
#ifdef SCI1_USED
   linIrqDisable();
   l_ifc_rx_SCI1();
   linIrqEnable();
#endif
}
#endif
/***************************************************/


/*********************************************************/
/*******  SCI2 Rx Interrupt *************************/

#ifdef __COSMIC__
#ifndef STM8AF622x
@interrupt void l_serial2ISR(void)
{
#ifdef SCI2_USED
   linIrqDisable();
   l_ifc_rx_SCI2();
   linIrqEnable();
#endif
}
#endif
#endif

#ifdef __RAISONANCE__
#ifndef STM8AF622x
void l_serial2ISR(void) interrupt 18
{
#ifdef SCI2_USED
   linIrqDisable();
   l_ifc_rx_SCI2();
   linIrqEnable();
#endif
}
#endif
#endif

#ifdef __IAR__
#ifndef STM8AF622x
#pragma vector=0x14
__interrupt void l_serial2ISR(void)
{
#ifdef SCI2_USED
   linIrqDisable();
   l_ifc_rx_SCI2();
   linIrqEnable();
#endif
}
#endif
#endif

/*********************************************************************
 *
 * The ISR for the selected timer interrupt for LIN
 *
 * The function must call the l_timerISR() provided by the LIN driver
 * The selected timer must be exclusively reserved for the LIN driver
 *
 */
#ifdef __COSMIC__
@interrupt void l_timer4ISR(void)
{
#ifdef LIN_USE_HARDWARE_TIMER
  linIrqDisable();

  l_timerISR();
                                         
	if (!(--Cyclic))       
	{
		Cyclic = 5;
		lin_osCyclic();
	}
	
	linIrqEnable();

  /* Begin BUGZILLA_472 */
  /* removed the "return" instruction */
  /* End BUGZILLA_472 */
#endif
}
#endif
#ifdef __RAISONANCE__
void l_timer4ISR(void) interrupt 23
{
#ifdef LIN_USE_HARDWARE_TIMER
  linIrqDisable();

  l_timerISR();

	if (!(--Cyclic))      
	{
		Cyclic = 5;
		lin_osCyclic();
	}
	
  linIrqEnable();

  /* Begin BUGZILLA_472 */
  /* removed the "return" instruction */
  /* End BUGZILLA_472 */
#endif
}
#endif

#ifdef __IAR__
#pragma vector=0x19
__interrupt void l_timer4ISR(void)
{
#ifdef LIN_USE_HARDWARE_TIMER
  linIrqDisable();

  l_timerISR();
    if (!(--Cyclic))    
     {
      Cyclic = 5;
      lin_osCyclic();
     }
  linIrqEnable();

  /* Begin BUGZILLA_472 */
  /* removed the "return" instruction */
  /* End BUGZILLA_472 */
#endif
}
#endif


#ifdef __COSMIC__

@interrupt void DummyInt(void)
{
 _asm("nop");
}

#elif __RAISONANCE__

void DummyInt(void)
{
	
}

#elif __IAR__

__interrupt void DummyInt(void)
{
asm("rim\n");
}
#endif


/*                                         

*/
/*********************************************************************
 *
 * Delay()
 * Decreament the value of nCount until it reaches to zero.
 * This function is used to give delay.
 * Return value:
 * - none
 */
void Delay(unsigned int nCount)
{
  /* Decrement nCount value */
  while (--nCount != 0);
}


/*********************************************************************
 *
 * lin_osHWInit()
 *
 * This function is called during system startup.
 * LEDs and timers will be initialized here
 *
 * Parameters:
 * - none
 *
 * Return value:
 * - none
 *
 */
void lin_osHWInit(void)
{	 
   //CLK_CKDIV = 0x08; /* HSI = 8MHz*/
   CLK_CKDIV = 0x00; /* HSI = 16MHz*/
   CLK_ICKC = 0x01;
   while(!(CLK_ICKC&0x02));  /**Wait until HSI is Ready**/ 

#ifdef EXT_CLK  /* External Clock at 8 MHz */
   CLK_ECKC |= 0x01; /* To  work with external clock */
   CLK_SWIC |= 0x02;
   CLK_SWI = 0xB4;
   while (!CLK_ECKC&0x02); /* wait HSE is ready */
#endif
	 

#ifdef LIN_USE_HARDWARE_TIMER    
   LIN_TIMER_ENABLE(); /* Enable the 8-bit timer module */
#endif
   return;
}

/*********************************************************************
 *
 * lin_osInit()
 *
 * Initialization task of the system. Will set up system timer
 * and call init function of test application
 *
 * Parameters:
 * - none
 *
 * Return value:
 * - none
 *
 */
void lin_osInit(void)
{
  /***********************************************************
   * perform hw initialization
   ***********************************************************/
  lin_osHWInit();

  /***********************************************************
   * call init task
   ***********************************************************/
  l_initTask();

  return ;
}

/*********************************************************************
 *
 * lin_osCyclic()
 *
 * Periodic task of the systyem. Is called periodically by the timer
 * ISR and will call LIN test application
 *
 * Parameters:
 * - none
 *
 * Return value:
 * - none
 *
 */
void lin_osCyclic (void)
{
  /***********************************************************
   * call cyclic test task
   ***********************************************************/

   l_cyclicTask();

   return ;
}




/*********************************************************************
 *
 * main()
 *
 * main function of demo application, bring demo up
 *
 * parameters:
 * - none
 *
 * return value:
 * - none
 *
 */


void main
(
   void
)
{
  
   unsigned int temp1=0xffff;
    unsigned int temp2=0x5555;
  unsigned char i,j;
   Delay(20000);
   lin_osInit();
   
/*  
CLK_PCKENR1 |= 0x80;//开启定时器1外设时钟
TIM1_EGR = 0x01; //重新初始化TIM1

TIM1_CR1 = 0x00; //B7(0)可以直接写入 B65(00)边缘对齐模式B4(0)向上计数B3(0)计数器不停止发生更新事件
TIM1_RCR = 0x00; 

//设定预分频为,16分频 16M
TIM1_PSCRH =0x00; //PWM的时钟 影响周期
TIM1_PSCRL =0x00; //PWM的时钟 影响周期

TIM1_ARRH = 0x00; //设定重装载值
TIM1_ARRL = 0x02; //PWM的周期

TIM1_CCER2 = 0x03; //开启CH3,高电平有效? ?
TIM1_CCMR3 = 0x68; //PWM模式1,CC1配置入输出
TIM1_CCR3H = 0x00;
TIM1_CCR3L = 0x01; // 占空比值

TIM1_BKR |= 0x80; //刹车
TIM1_CR1 |= 0x01; //使能TIM1计数器
*/
 
    GPIOD_ODR &= ~0x10; //PD4
    GPIOD_DDR |= 0x10;
    GPIOD_CR1 |= 0x10;
    GPIOD_CR2 |= 0x10;
    GPIOD_ODR |= 0x10;  

#ifdef __STM8AF6223A__   
    GPIOC_ODR &= ~0x70; //PC 6 5 4
    GPIOC_DDR |= 0x70;
    GPIOC_CR1 |= 0x70;
    GPIOC_CR2 |= 0x70;
    GPIOC_ODR |= 0x70;     

    GPIOB_ODR &= ~0x01; //PB 0
    GPIOB_DDR |= 0x01;
    GPIOB_CR1 |= 0x01;
    GPIOB_CR2 |= 0x01;
    GPIOB_ODR |= 0x01; 
#else
    GPIOC_ODR &= ~0x78; //PC 6 5 3 4
    GPIOC_DDR |= 0x78;
    GPIOC_CR1 |= 0x78;
    GPIOC_CR2 |= 0x78;
    GPIOC_ODR |= 0x78;  
#endif
     
    CLK_CCO |= 0x08; /* select fcpu for CCO output */
    CLK_CCO |= 0x01; /* CCO output enable*/
    while (!CLK_CCO&0x20); /* wait HSE is ready */
/**************初始化ALED1642***************/
ALED_Write(0x007f,7);//写控制寄存器
ALED_Write(0x007f,7);//连续写两边，板子有连个驱动芯片
//重置亮度值
for(i=0;i<17;i++)
  ALED_Write(0xffff,3);//latch brightness
ALED_Write(0xffff,5);//global brightness
//关闭所有的灯
ALED_TurnOnOff();//switch
/*******************************************/ 

   #ifndef STM8AF622x
    //LCD_Demo_Init();
   #endif
   /*
    * enable interrupts
    */
   
   linIrqEnable();	 
	 
   while(1)
   {    
      /* do nothing */
      #ifndef STM8AF622x
      if (UpdateScreen==1)
		  {
			 UpdateScreen=0;
			if (KeyOnOff==0) 
			  LCD_PrintString(LCD_LINE2, ENABLE, DISABLE, "Mst LED1 OFF"); // Max 15 characters per line
			else if (KeyOnOff==1) 
				LCD_PrintString(LCD_LINE2, ENABLE, DISABLE, "Mst LED1 ON");
		  }
      #endif
   }
    
}


/*
 * eof lin_slave_app.c
 */