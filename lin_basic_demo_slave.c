/*********************************************************************
 *
 * Project:      LIN 2.1 driver suite - test suite
 *
 * File name:    lin_basic_demo_slave.c
 *
 * Copyright:    (c) STMicroelectronics
 *
 * Author:       Giuseppe Stefano Fazio, STMicroelectronics
 *
 * Description:  test code for lin_basic_demo (slave)
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
 * 20090420  v1.1   gsf  - BUGZILLA_472: LIN driver does not compile if 
 *                         Cosmic "strict" option is enabled.
 *
 *********************************************************************/

/*********************************************************************
 * include for LIN driver
 *********************************************************************/
#include "lin.h"
#include "lin_test_hardware.h"
#include "lin_slave.h"
#include "lin_def.h"
#include "lin_def_stm8.h"
/*********************************************************************
 * global defines
 *********************************************************************/

#define ENABLE 1
#define DISABLE 0

extern l_ifc_handle ifc_used;
#define dimension 0x1F
unsigned char my_buffer_1[dimension];
#ifndef STM8AF622x
 #if (defined(LIN_INCLUDE_TP) && defined(LIN_21))
   void TP_state_machine(void);
 #endif
#endif

/*********************************************************************
 * global variables
 *********************************************************************/
unsigned char *ReceivedData = my_buffer_1;

#ifdef __COSMIC__

@tiny unsigned char temp;
@tiny static unsigned char ledCount = 0;
@tiny static unsigned char l_counterISRdisable = 0;
@tiny static unsigned char Toggle = 0;
@tiny static unsigned char TPState = 0;
@tiny unsigned char TPError = 0;
@tiny unsigned char RxStatus = 0;
@tiny unsigned char TxStatus = 0;
@tiny l_u16  RecLength = 0;
@tiny unsigned char RecNAD = 0;
@tiny unsigned char DataToSend[16]= {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
@tiny unsigned char KeyOnOff=0;
@tiny unsigned char PreviousKeyOnOff=0;
@tiny unsigned char NewJoystickState=0;
@tiny unsigned char Joystick=0;

#elif __RAISONANCE__

page0 unsigned char temp;
page0  static unsigned char ledCount = 0;
page0  static unsigned char l_counterISRdisable = 0;
page0  static unsigned char Toggle = 0;
page0  static unsigned char TPState = 0;
page0  unsigned char TPError = 0;
page0  unsigned char RxStatus = 0;
page0  unsigned char TxStatus = 0;
page0  l_u16  RecLength = 0;
page0  unsigned char RecNAD = 0;
page0  unsigned char DataToSend[16]= {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
page0  unsigned char KeyOnOff=0;
page0  unsigned char PreviousKeyOnOff=0;
page0  unsigned char NewJoystickState=0;
page0  unsigned char Joystick=0;

#elif __IAR__

__tiny  unsigned char last_IndicationLampMode;
__tiny  unsigned char IndicationLampMode;
//__tiny  static unsigned char ledCount = 0;
//__tiny  static unsigned char l_counterISRdisable = 0;
//__tiny  static unsigned char Toggle = 0;
//__tiny  static unsigned char TPState = 0;
__tiny  unsigned char TPError = 0;
__tiny  unsigned char RxStatus = 0;
__tiny  unsigned char TxStatus = 0;
__tiny  l_u16  RecLength = 0;
__tiny  unsigned char RecNAD = 0;
__tiny  unsigned char DataToSend[16]= {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
__tiny  unsigned char KeyOnOff=0;
__tiny  unsigned char PreviousKeyOnOff=0;
__tiny  unsigned char NewJoystickState=0;
__tiny  unsigned char Joystick=0;
#endif


/*********************************************************************
 * tasks
 *********************************************************************/


/*********************************************************************
 *
 * l_initTask()
 *
 * Test init task, must be called from the application at startup
 *
 * parameters:
 * - none
 *
 * return value:
 * - none
 *
 */
void l_initTask(void)
{
    /*
     * bring up LIN driver
     */
   l_sys_init();
   l_ifc_init(ifc_used);
#ifndef LIN_21
    l_ifc_connect(ifc_used);
#endif /* !LIN_21 */
#ifndef STM8AF622x
  #if (defined(LIN_INCLUDE_TP) && defined(LIN_21))
	 ld_init(ifc_used);
  #endif
#endif
    l_changeFrameId(0x0011U,0x11);
    l_changeFrameId(0x0020U,0x20);

    //l_ifc_wake_up(ifc_used);
    return ;
}
    
    
    
//定义18组led灯的亮暗状态    
unsigned int LEDS_bit[18]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

//aled1642处理跟配置
void ALED_Write(unsigned int data,unsigned char bit_len)
{
  unsigned int temp2;
  unsigned char i,temp1;
  temp1=16-bit_len;
  temp2=data;
  SPI_LE_LOW;
  for(i=0;i<16;i++)
  {
    SPI_CLK_LOW;
    if(i==temp1)
      SPI_LE_HIGH;
    if(temp2&0x8000)
    {
        SPI_DAT_HIGH;
    }
    else
    {
        SPI_DAT_LOW;
    }
    temp2<<=1;
    SPI_CLK_HIGH;
  } 
  SPI_LE_LOW;
}

//aled1642开关LED灯
void ALED_TurnOnOff()   //18bit
{
  unsigned char i;
  SPI_LE_LOW;
  for(i=0;i<18;i++)
  {
    SPI_CLK_LOW;
    if(i==17)
      SPI_LE_HIGH;
    if(LEDS_bit[i])
    {
        SPI_DAT_HIGH;
    }
    else
    {
        SPI_DAT_LOW;
    }
    SPI_CLK_HIGH;
  } 
  SPI_LE_LOW;
}

//aled1642设置亮度值
void ALED_Brightness_Set(unsigned int bright)
{
  unsigned char i;
  for(i=0;i<17;i++)
     ALED_Write(bright,3);
   ALED_Write(bright,5);
}

unsigned char timeCnt;
unsigned char timeFlag;
unsigned char led_flow_num=17;
unsigned char led_flow_bit;
unsigned char led_flow_dir;
unsigned int brightness=0;
unsigned char allon_flag;

void Display_Led(unsigned char mode)
{
  unsigned char i;
  switch(mode)
  {
  case 0://默认自动循环模式
    if(led_flow_dir==0)
    {
      if(led_flow_bit<=17)
      {
          LEDS_bit[led_flow_bit]=1;
      }
      else if(led_flow_bit==18)
      {
          for(i=0;i<18;i++)
            LEDS_bit[i]=0;
      }
      led_flow_bit++;
      if(led_flow_bit>18) {led_flow_bit=0;led_flow_dir=1;}
    }
    else if(led_flow_dir==1)
    {
      if(led_flow_bit<=17)
      {
          LEDS_bit[17-led_flow_bit]=1;
      }
      else if(led_flow_bit==18)
      {
          for(i=0;i<18;i++)
            LEDS_bit[i]=0;
      }
      led_flow_bit++;   
      if(led_flow_bit>18) {led_flow_bit=0;led_flow_dir=2;ALED_Brightness_Set(0);}
    }
    else if(led_flow_dir==2)
    {
      if(!allon_flag)
      {
        for(i=0;i<18;i++)
          LEDS_bit[i]=1;
        allon_flag=1;
      }
      brightness+=4000;
      ALED_Brightness_Set(brightness);
      if(brightness>=50000) {led_flow_dir=3;}    
    }
    else if(led_flow_dir==3)
    {
      brightness-=4000;
      ALED_Brightness_Set(brightness);
      if(brightness<=5000) {led_flow_dir=4;}   
    }
    else if(led_flow_dir==4)
    {
        if(allon_flag)
        {
          for(i=0;i<18;i++)
            LEDS_bit[i]=0;
          allon_flag=0;
        } 
        led_flow_dir=5;  
    }
    else if(led_flow_dir==5)
    {
      ALED_Brightness_Set(0xffff);
    }
    break;
  case 1://从左到右再从右到做
    if(allon_flag)
    {
      for(i=0;i<18;i++)
        LEDS_bit[i]=0;
      allon_flag=0;
    }
    if(!led_flow_dir)
    {
      if(led_flow_bit<=22)
      {
        if(led_flow_bit>=led_flow_num)
          LEDS_bit[led_flow_bit-led_flow_num]=0;
        if(led_flow_bit<18) 
          LEDS_bit[led_flow_bit]=1;
      }
      led_flow_bit++;
      if(led_flow_bit>30) {led_flow_bit=0;led_flow_dir=!led_flow_dir;}
    }
    else
    {
      if(led_flow_bit<=22)
      {
        if(led_flow_bit>=led_flow_num)
          LEDS_bit[17+led_flow_num-led_flow_bit]=0;
        if(led_flow_bit<18)       
          LEDS_bit[17-led_flow_bit]=1;
      }
      led_flow_bit++;   
      if(led_flow_bit>30) {led_flow_bit=0;led_flow_dir=!led_flow_dir;}
    }
    break;
  case 2://从中间向两边四散开来流水再从两边向中间合拢
    if(allon_flag)
    {
      for(i=0;i<18;i++)
        LEDS_bit[i]=0;
      allon_flag=0;
    }
    if(!led_flow_dir)
    {
      if(led_flow_bit<=13)
      {
        if(led_flow_bit>=led_flow_num)
          {LEDS_bit[led_flow_bit-led_flow_num]=0;LEDS_bit[17+led_flow_num-led_flow_bit]=0;}
        if(led_flow_bit<9)
          {LEDS_bit[led_flow_bit]=1;LEDS_bit[17-led_flow_bit]=1;}
      }
      led_flow_bit++;
      if(led_flow_bit>20) {led_flow_bit=0;led_flow_dir=!led_flow_dir;}
    }
    else
    {
      if(led_flow_bit<=13)
      {
        if(led_flow_bit>=led_flow_num)
          {LEDS_bit[8+led_flow_num-led_flow_bit]=0;LEDS_bit[9+led_flow_bit-led_flow_num]=0;}
        if(led_flow_bit<9)
          {LEDS_bit[8-led_flow_bit]=1;LEDS_bit[9+led_flow_bit]=1;}
      }
      led_flow_bit++;   
      if(led_flow_bit>20) {led_flow_bit=0;led_flow_dir=!led_flow_dir;}
    }
    break;
  case 3://呼吸效果
    if(!allon_flag)
    {
      for(i=0;i<18;i++)
        LEDS_bit[i]=1;
      allon_flag=1;
    }
    if(!led_flow_dir)
    {
      brightness+=500;
      ALED_Brightness_Set(brightness);
      if(brightness>50000) {led_flow_dir=!led_flow_dir;}
    }
    else
    {
      brightness-=500;
      ALED_Brightness_Set(brightness);
      if(brightness<5000) {led_flow_dir=!led_flow_dir;}
    }
    break;
  case 4:
    if(allon_flag)
    {
      for(i=0;i<18;i++)
        LEDS_bit[i]=0;
      allon_flag=0;
      ALED_Brightness_Set(50000);
    }
    if(led_flow_bit<=17)
    {
        LEDS_bit[led_flow_bit]=1;
    }
    else if(led_flow_bit==18)
    {
        for(i=0;i<18;i++)
          LEDS_bit[i]=0;
    }
    led_flow_bit++;
    if(led_flow_bit>18) {led_flow_bit=0;}
    break;
  case 5:
    if(allon_flag)
    {
      for(i=0;i<18;i++)
        LEDS_bit[i]=0;
      allon_flag=0;
      ALED_Brightness_Set(50000);
    }
    if(led_flow_bit<=17)
    {
        LEDS_bit[17-led_flow_bit]=1;
    }
    else if(led_flow_bit==18)
    {
        for(i=0;i<18;i++)
          LEDS_bit[i]=0;
        allon_flag=0;
    }
    led_flow_bit++;
    if(led_flow_bit>18) {led_flow_bit=0;}
    break;
  case 6://中间向两边流水
    if(allon_flag)
    {
      for(i=0;i<18;i++)
        LEDS_bit[i]=0;
      allon_flag=0;
      ALED_Brightness_Set(50000);
    }
    
    if(led_flow_bit<=8)
    {
      LEDS_bit[8-led_flow_bit]=1;
      LEDS_bit[9+led_flow_bit]=1;
    }
    else if(led_flow_bit==9)
    {
         for(i=0;i<18;i++)
          LEDS_bit[i]=0;  
         allon_flag=0;
    }
    led_flow_bit++;
    if(led_flow_bit>9) {led_flow_bit=0;}
    break;
  case 7:
    if(allon_flag)
    {
      for(i=0;i<18;i++)
        LEDS_bit[i]=0;
      allon_flag=0;
      ALED_Brightness_Set(50000);
    }
    
    if(led_flow_bit==1)
    {
      for(i=0;i<18;i++)
        LEDS_bit[i]=1;
    }
    else if(led_flow_bit==2)
    {
         for(i=0;i<18;i++)
          LEDS_bit[i]=0;  
    }
    else if(led_flow_bit==3)
    {
         for(i=0;i<18;i++)
          LEDS_bit[i]=1;  
    }
    else if(led_flow_bit==4)
    {
         for(i=0;i<18;i++)
          LEDS_bit[i]=0;  
    }
    if(led_flow_bit<=4)
      led_flow_bit++;
    break;
  case 8://两边向中间流水  待修改
    if(allon_flag)
    {
      for(i=0;i<18;i++)
        LEDS_bit[i]=0;
      allon_flag=0;
      ALED_Brightness_Set(50000);
    }
    
    if(led_flow_bit<=8)
    {
      LEDS_bit[17-led_flow_bit]=1;
      LEDS_bit[led_flow_bit]=1;
    }
    else if(led_flow_bit==9)
    {
         for(i=0;i<18;i++)
          LEDS_bit[i]=0;  
         allon_flag=0;
    }
    led_flow_bit++;
    if(led_flow_bit>9) {led_flow_bit=0;}
    break;
  default:
    break;
  }
}
/*********************************************************************
 *
 * l_cyclicTask()
 *
 * Cyclic test task, must be called periodically from the test
 * application
 *
 * parameters:
 * - none
 *
 * return value:
 * - none
 *
 */
void l_cyclicTask(void)
{
unsigned char i;
   linIrqDisable();

#ifndef STM8AF622x
  #if (defined(LIN_INCLUDE_TP) && defined(LIN_21))
	 TP_state_machine();
  #endif
#endif
   
         timeCnt=timeCnt%6+1;
   
/*****************流水处理*******************/
         switch(timeCnt)
         {
         case 1:
           if(IndicationLampMode!=last_IndicationLampMode)
           {
              led_flow_bit=0;led_flow_dir=0;
              for(i=0;i<18;i++)
                LEDS_bit[i]=0;
           }
            if(IndicationLampMode==0)
              Display_Led(0);
            else if(IndicationLampMode==2)
              Display_Led(4);
            else if(IndicationLampMode==3)
              Display_Led(5);
            else if(IndicationLampMode==4)
              Display_Led(7);
            else if(IndicationLampMode==5)//吧台打开
              Display_Led(6);//中间向两边流水
            else if(IndicationLampMode==6)//吧台关闭
              Display_Led(8);//全亮
            else if(IndicationLampMode==7)//全亮
              for(i=0;i<18;i++)
                LEDS_bit[i]=1;
            ALED_TurnOnOff();
          break;
         case 2:
           break;
         case 3:
           break;
         case 4:
           break;
         }
/********************************************/

   //更新LIN的报文数据
   if(l_flg_tst_LIN_FLAG_RxCCU_ConsoleIndicationLampMode())
   {
      l_flg_clr_LIN_FLAG_RxCCU_ConsoleIndicationLampMode();
      last_IndicationLampMode=IndicationLampMode;
      IndicationLampMode=l_u8_rd_CCU_ConsoleIndicationLampMode(); 
   }
	 
   linIrqEnable();

   return ;
}

#ifndef STM8AF622x
  #if (defined(LIN_INCLUDE_TP) && defined(LIN_21))
void TP_state_machine(void)
{
    unsigned char idx;
    RxStatus = ld_rx_status(ifc_used);
    TxStatus = ld_tx_status(ifc_used);

    switch (TPState)
    {
      case 0:
      {
        if (RxStatus==LD_COMPLETED)
        {
          RecLength = dimension; /* This is important for correct internal flags management ! */
    
          RecNAD = 0x71;
          ld_receive_message(ifc_used, &RecLength, &RecNAD, ReceivedData); /* to set status to LD_IN_PROGRESS */
          TPState = 1;
        }
      }
      break;

      case 1:
      {
        switch (RxStatus)
        {
          case LD_N_CR_TIMEOUT:
          case LD_WRONG_SN:
          case LD_FAILED:

            TPError = 1;
            ld_init(ifc_used);
            TPState = 0;

          break;

          case LD_IN_PROGRESS:
					switch (TxStatus)
          {
            case LD_N_AS_TIMEOUT:
            case LD_FAILED:
						{
   						TPError = 1;
              ld_init(ifc_used);
              TPState = 0;
						}
            break;

            case LD_IN_PROGRESS:
            break;

            case LD_COMPLETED:          	
            break;

            default:

            break;
          }        
          break;

          case LD_COMPLETED:
          {

            RecNAD = 0x71;
            DataToSend[0] = ReceivedData[0] + 0x40;
            if(ReceivedData[0] == 0x2E)
            {
              for(idx=0;idx<10;idx++)
                DataToSend[idx+1]= ReceivedData[idx+1];
              RecLength = 3;  // 6E,55,55							
            }
            else if(ReceivedData[0] == 0x22 || ReceivedData[0] == 0x23)
            {
              RecLength = 11;
            }
            else
              RecLength = 6;

            ld_send_message(ifc_used, RecLength, RecNAD,  DataToSend);
            RecLength = dimension;
            ld_receive_message(ifc_used, &RecLength, &RecNAD, ReceivedData); /* to set status to LD_IN_PROGRESS */     
          }
          break;

          default:
					break;
        }
      }	
      break; //case 1      
      default:
      break;
    } 
}
#endif
#endif