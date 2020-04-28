/*********************************************************************
 *
 * Project:      LIN 2.1 driver suite
 *
 * File name:    lin_basic_demo_slave.h
 *
 * Copyright:    (c) STMicroelectronics
 *
 * Author:       Giuseppe Stefano Fazio, STMicroelectronics
 *
 * Description:  header file for lin_basic_demo (slave)
 *
 * Target:       any
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
extern void LCD_Clear(void);
extern void LCD_PrintString(unsigned char Line, unsigned char AutoComplete, unsigned char Append, unsigned char *ptr);
extern void LCD_Demo_Init (void);
extern void ALED_Write(unsigned int data,unsigned char bit_len);
extern void ALED_TurnOnOff();
extern unsigned int LEDS_bit[18];


extern void l_initTask
(
   void
);

extern void l_cyclicTask
(
   void
);

#ifdef __COSMIC__                                           
 @tiny extern unsigned char KeyOnOff;
 @tiny extern unsigned char PreviousKeyOnOff;
 @tiny extern unsigned char NewJoystickState;
 @tiny extern unsigned char Joystick;
 
#elif __RAISONANCE__
 page0 extern unsigned char KeyOnOff;
 page0 extern unsigned char PreviousKeyOnOff;
 page0 extern unsigned char NewJoystickState;
 page0 extern unsigned char Joystick;
#elif __IAR__
 __tiny extern unsigned char KeyOnOff;
 __tiny extern unsigned char PreviousKeyOnOff;
 __tiny extern unsigned char NewJoystickState;
 __tiny extern unsigned char Joystick;
#endif
