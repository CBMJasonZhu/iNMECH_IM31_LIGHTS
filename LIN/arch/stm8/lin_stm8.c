/*********************************************************************
 *
 * Project:      LIN 2.1 driver suite
 *
 * File name:    lin_stm8.c
 *
 * Copyright:    (c) STMicroelectronics
 *
 * Author:       Giuseppe Stefano Fazio
 *
 * Description:  Hardware dependend parts of the LIN driver:
 *               - Timer
 *               - Serial interfaces
 *
 * Target:       STM8
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
 * 20071217  v1.0   gsf  - Initial version for STM8
 *                         BUGZILLA_376: LBDL bit must be set in LINUART,
 *                         in both master or slave modes.
 *                         BUGZILLA_377: Uncommented some code.
 *                         BUGZILLA_378: Wrong driver implementation
 *                         for Break and Del readback.
 * 20090420  v1.1   gsf  - BUGZILLA_472: LIN driver does not compile if 
 *                         Cosmic "strict" option is enabled.
 *                         BUGZILLA_474: LIN driver restart communication 
 *                         with wrong baudrate after wakeup.
 *                         BUGZILLA_475: The values for baudrate 
 *                         registres are not correctly calculate for 
 *                         differents baudrates and clocks.
 * 20110706  v1.2   JKh  - BUGZILLA_1166: LIN bus shorted to GND - LBDF flag issue
 * 20120822	 v1.3   Jkh	 - default and break added inside "l_changeBaudrateArch_SCI1" 
 *												 incase "#define LIN_BOARD_CPU_FREQ_HZ" is other than 8MHz or 16MHz 
 *												 to resolve the compiler issue
 *
 *********************************************************************/

/** \file lin_stm8.c
 */

#define LIN_STM8_C

#define LIN_STM8_C_MAJOR_VERSION  1
#define LIN_STM8_C_MINOR_VERSION  33
#define LIN_STM8_C_PATCH_VERSION  4

#define LIN_STM8_C_STM

/*********************************************************************
 * includes
 *********************************************************************/
#include <limits.h>
#include "lin_cfg_types.h"
#include "lin_stm8.h"
#include "lin_def_arch_include.h"
#include "lin_def.h"
#include "lin_def_gen.h"
#include "lin_general.h"
#include "lin_cfg.h"
#include "lin_test_hardware.h" 


#ifdef LIN_MASTER_NODE
  #include "lin_master.h"
#else
  #include "lin_slave.h"
#endif

#include "lin_version_control.h"

/*********************************************************************
 * environmental checks
 *********************************************************************/

#if !defined(__ARCH_STM8__)
  #error "Invalid architecture"
#endif

/*********************************************************************
 * global variables
 *********************************************************************/
                                                       
#ifdef __COSMIC__                                           
 #ifdef LIN_SLAVE_LINSCI_AUTOSYNC
  @tiny l_u8                         l_storeBRR1 = 0;
 #endif
@tiny static  volatile l_u8 prescaler = 0;
@tiny static  volatile l_u8 fraction  = 0;
@tiny static  l_u16        calc_mantissa;
@tiny static l_u8         calc_fraction;
@tiny static l_u8         calc = 0x00;

#elif __RAISONANCE__
 #ifdef LIN_SLAVE_LINSCI_AUTOSYNC
  page0 l_u8                         l_storeBRR1 = 0;
 #endif
page0 static  volatile l_u8 prescaler = 0;
page0 static  volatile l_u8 fraction  = 0;
page0 static  l_u16        calc_mantissa;
page0 static l_u8         calc_fraction;
page0 static l_u8         calc = 0x00;

#elif __IAR__
 #ifdef LIN_SLAVE_LINSCI_AUTOSYNC
   __tiny l_u8                        l_storeBRR1 = 0;
 #endif
__tiny static  volatile l_u8 prescaler = 0;
__tiny static  volatile l_u8 fraction  = 0;
__tiny static  l_u16        calc_mantissa;
__tiny static l_u8         calc_fraction;
__tiny static l_u8         calc = 0x00;

#endif


/*********************************************************************
 * functions
 *********************************************************************/
/*********************************************************************
 *
 * Implementation
 *
 *********************************************************************/

/*********************************************************************
 *
 * System specific parts
 *
 *********************************************************************/

/*********************************************************************
 *
 * l_sys_init_arch()
 *
 */
/**
 * \brief hardware initialization function
 *
 * This function performs system specific initialization
 *
 * - standard API
 * - master and slave
 *
 * \param  none
 *
 * \retval 0 if initialization succeeded (currently always)
 *
 */
STACK l_bool l_sys_init_arch (void)
{

  return 0;

} /* end l_sys_init_arch() */


/*********************************************************************
 *
 * interface related stuff
 *
 *********************************************************************/


/*********************************************************************
 *
 * SCI1 interface
 *
 *********************************************************************/

#ifdef SCI1_USED

  /*********************************************************************
   *
   * l_ifc_initArch_SCI1()
   *
   */
  /**
   * \brief interface specific initialization function
   *
   * This function performs the initialization of the SCI1 interface
   *
   * - internal function
   * - slave only
   *
   * \param  none
   *
   * \retval none
   *
   */
  STACK l_bool l_ifc_initArch_SCI1 (void)
  {
    /*
     * it is recommended to set the M bit prior to setting the baudrate
     * (we set the complete register to 0)
     */
    LINUART_CR1 = 0;


    /* always success */

    return 0;

  } /* end l_ifc_initArch_SCI1() */

  /*********************************************************************
   *
   * l_ifc_connectArch_SCI1()
   *
   */
  /**
   * \brief interface specific connect function
   *
   * Initialization function for the SCI1 interface
   *
   * - internal function
   * - slave only
   *
   * \param  none
   *
   * \retval 0 if succeeded (currently always)
   *
   */
  STACK l_bool l_ifc_connectArch_SCI1 (void)
  {
   
    /*
     * initialize serial interface sc1 for reading and writing
     */
    LINUART_CR1 = 0;

    /***Enable Receiver Interrupt Mask,Transmitter & Receiver ****/
    LINUART_CR2 |= (LINUART_CR2_RIEN | LINUART_CR2_TEN | LINUART_CR2_REN);

    /* Begin BUGZILLA_376 */
     LINUART_CR4 |= LINUART_CR4_LBDL;/* LIN Break Detection Length Mask Enable */
    /* End BUGZILLA_376 */

    /*
     * it seems as if enabling receiver/transmitter suffices to
     * switch the pins of PORTD to SCI usage
     */
    #ifdef LIN_MASTER_NODE
      LINUART_CR3 |= LINUART_CR3_LINEN; /* Alternate function output control - LIN mode enable */
    #endif

    #ifdef LIN_SLAVE_NODE

      #ifdef LIN_SLAVE_LINSCI
        LINUART_CR3 |= LINUART_CR3_LINEN;  /* Alternate function output control - LIN mode enable */
 
        /******Enable LIN Slave Mode & LIN Header Interrupt********/
        LINUART_CR5 |= (LINUART_CR5_LSLV | LINUART_CR5_LHDIEN);

        #ifdef LIN_SLAVE_LINSCI_AUTOSYNC
          LINUART_CR5 |= LINUART_CR5_LASE; /* LIN slave autosync */
        #endif

        #ifdef LIN_SLAVE_LINSCI_MUTE_MODE
          LINUART_CR2 |= LINUART_CR2_RWU;
        #endif

      #endif /* LIN_SLAVE_LINSCI */

    #endif /* LIN_SLAVE_NODE */

    l_changeBaudrateArch_SCI1(IFCCTRL_NORMALBAUDRATE(LIN_IFC_SCI1));

    /*
     * we clear the relevant status bits here
     */
    LINUART_SR;
    LINUART_DR;
    /* Begin BUGZILLA_472 */
    LINUART_CR5 &= ((volatile l_u8) ~(LINUART_CR5_LSF));  /* Clear LSF flag */
    LINUART_CR5 &= ((volatile l_u8) ~(LINUART_CR5_LHDF)); /* Reset LHDF flag */
    /* End BUGZILLA_472 */

    return 0; /* always success */

  } /* end l_ifc_connectArch_SCI1() */

  /*********************************************************************
   *
   * l_ifc_disconnectArch_SCI1()
   *
   */
  /**
   * \brief interface specific disconnect function
   *
   * This function disconnects the SCI1 interface
   *
   * - internal function
   * - slave only
   *
   * \param  none
   *
   * \retval 0 if succeeded (currently always)
   *
   */
  STACK l_bool l_ifc_disconnectArch_SCI1 (void)
  {
    /*
     * set both control registers to 0, this disables
     * both receiver and transmitter, as well as all related
     * interrupts
     */

    LINUART_CR1 = 0;
    LINUART_CR2 = 0;

    /*
     * The fifth control register is relevant for LIN mode only
     */

    #if (defined(LIN_SLAVE_LINSCI) && defined(LIN_SLAVE_NODE))
      LINUART_CR5 = 0;
    #endif

    return 0;

  } /* end l_ifc_disconnectArch_SCI1() */

  /*********************************************************************
   *
   * l_txCharArch_SCI1()
   *
   */
  /**
   * \brief interface specific character send function
   *
   * This function transmits a given char over the SCI1 of the chip
   *
   * - internal function
   * - slave only
   *
   * \param  txChar         the character to be transmitted
   *
   * \retval none
   *
   */
  STACK void l_txCharArch_SCI1 (l_u8 txChar)
  {

    /*
     * move transmit character to transmit register
     */
    LINUART_DR = txChar;

    return ;

  } /* end l_txCharArch_SCI1() */

  /*********************************************************************
   *
   * l_changeBaudrateArch_SCI1()
   *
   */
  /**
   * \brief interface specific baudrate changing function
   *
   * This function changes the baudrate for SCI1
   *
   * - internal function
   * - slave only
   *
   * \param  baudrate       the value to set the baudrate to
   *
   * \retval the character received
   *
   */
  /* Begin BUGZILLA_475 */
  STACK void l_changeBaudrateArch_SCI1 (l_u16 baudrate)
  {
      l_u16        divider = (LIN_BOARD_CPU_FREQ_HZ >> 8);
      l_u16        tmp;
      l_u8         count;
      calc = 0x00;
      prescaler = 0;
      fraction  = 0;

  switch (baudrate)
    {
      #if (LIN_BOARD_CPU_FREQ_HZ == 8000000)      /* Fcpu = 8 MHz  */                  

           case 2400:          /* Normal baudrate */
             prescaler  = 0xD0;
             fraction   = 0x05;

           break;

           case 9600:          /* Normal baudrate */
             prescaler  = 0x34;
             fraction   = 0x01;

           break;

           case 10417:          /* Normal baudrate */
             prescaler  = 0x30;

           break;

           case 19200:          /* Normal baudrate */
             prescaler  = 0x1A;
             fraction   = 0x01;

           break;

           case 20000:          /* Wake-up/Normal baudrate */
             prescaler  = 0x19;

           break;

           default:             /* For all other cases let's calculate the correct values  */
             calc = 0x01;
               
           break;

      #elif (LIN_BOARD_CPU_FREQ_HZ == 16000000)    /* Fcpu = 16 MHz  */

          
           case 2400:          /* Normal baudrate */
             prescaler = 0xA0;
             fraction  = 0x1B;

           break;

           case 9600:          /* Normal baudrate */
             prescaler = 0x68;
             fraction  = 0x03;

           break;

           case 10417:         /* Normal baudrate */
             prescaler = 0x60;

           break;

           case 19200:         /* Normal baudrate */
             prescaler = 0x34;
             fraction  = 0x01;

           break;

           case 20000:         /* Wake-up/Normal baudrate */
             prescaler = 0x32;

           break;

           default:            /* For all other cases let's calculate the correct values  */
             calc = 0x01;
               
           break;     

       #else
           calc = 0x01;
       #endif /* #if (LIN_BOARD_CPU_FREQ_HZ == 8000000) */
    }



    if (calc)
    {
      calc_mantissa = (l_u16)(divider / (baudrate>>4));

      calc_fraction = 0;

      for (count = 0; count < 16; ++count)
      {
         tmp = (l_u16)(LIN_BOARD_CPU_FREQ_HZ / ((calc_mantissa<<4) + count));
         if (tmp >= baudrate)
         {
            fraction = count;
         }
      }

      /* Begin BUGZILLA_472 */
      fraction = ((l_u8) (calc_fraction | ((l_u8) calc_mantissa>>8))); /* DIV_Mantissa[11:8]+DIV_Fraction[3:0] */
      /* End BUGZILLA_472 */
      prescaler = ((l_u8) calc_mantissa);           /* DIV_Mantissa[7:0] */
    }

    #ifdef LIN_SLAVE_LINSCI
      #ifdef LIN_SLAVE_LINSCI_AUTOSYNC
        /*
         * the nominal value must be stored for aut because
         * the nominal value will be set again after every frame
         */

        if (l_storeBRR1 == 0)
        {
          l_storeBRR1   = prescaler;
        }

        LINUART_BRR2   = fraction;
        if (l_storeBRR1 == 0)
        {
          LINUART_BRR1   = 0;
        }
        else
        {
          LINUART_BRR1   = prescaler;
        }
      

      #else
        LINUART_BRR2   = fraction;
        LINUART_BRR1   = prescaler;

      #endif /* LIN_SLAVE_LINSCI_AUTOSYNC */
    #else
        LINUART_BRR2   = fraction;
        LINUART_BRR1   = prescaler;
    #endif /* LIN_SLAVE_LINSCI */

    return;

  } /* l_changeBaudrateArch_SCI1() */
  /* Begin BUGZILLA_475 */
#endif /* SCI1_USED */


/*********************************************************************
 *
 * SCI2 interface
 *
 *********************************************************************/

#ifdef SCI2_USED

  /*********************************************************************
   *
   * l_ifc_initArch_SCI2()
   *
   */
  /**
   * \brief interface specific initialization function
   *
   * This function performs the initialization of the SCI2 interface
   *
   * - internal function
   * - slave only
   *
   * \param  none
   *
   * \retval none
   *
   */
  STACK l_bool l_ifc_initArch_SCI2 (void)
  {
    /*
     * it is recommended to set the M bit prior to setting the baudrate
     * (we set the complete register to 0)
     */
    USART_CR1 = 0;


    /* always success */

    return 0;

  } /* end l_ifc_initArch_SCI2() */

  /*********************************************************************
   *
   * l_ifc_connectArch_SCI2()
   *
   */
  /**
   * \brief interface specific connect function
   *
   * Initialization function for the SCI2 interface
   *
   * - internal function
   * - slave only
   *
   * \param  none
   *
   * \retval 0 if succeeded (currently always)
   *
   */
  STACK l_bool l_ifc_connectArch_SCI2 (void)
  {
    volatile l_u8 tempVar;

    /*
     * initialize serial interface sc1 for reading and writing
     */
    USART_CR1 = 0;

    /****Enable Receiver Interrupt Mask ,Transmitter & Receiver ****/
    USART_CR2 |= (USART_CR2_RIEN |USART_CR2_TEN |USART_CR2_REN);

  /* Begin BUGZILLA_376 */
    USART_CR4 |= USART_CR4_LBDL;/* LIN Break Detection Length Mask Enable */
  /* End BUGZILLA_376 */
    /*
     * it seems as if enabling receiver/transmitter suffices to
     * switch the pins of PORTD to SCI usage
     */
    #ifdef LIN_MASTER_NODE
      USART_CR3 |= USART_CR3_LINEN; /* Alternate function output control - LIN mode enable */
    #endif

    l_changeBaudrateArch_SCI2(IFCCTRL_NORMALBAUDRATE(LIN_IFC_SCI2));

    /*
     * we clear the relevant status bits here
     */
    USART_SR;
    USART_DR;

    return 0; /* always success */

  } /* end l_ifc_connectArch_SCI2() */

  /*********************************************************************
   *
   * l_ifc_disconnectArch_SCI2()
   *
   */
  /**
   * \brief interface specific disconnect function
   *
   * This function disconnects the SCI2 interface
   *
   * - internal function
   * - slave only
   *
   * \param  none
   *
   * \retval 0 if succeeded (currently always)
   *
   */
  STACK l_bool l_ifc_disconnectArch_SCI2 (void)
  {
    /*
     * set both control registers to 0, this disables
     * both receiver and transmitter, as well as all related
     * interrupts
     */

    USART_CR1 = 0;
    USART_CR2 = 0;

    return 0;

  } /* end l_ifc_disconnectArch_SCI2() */

  /*********************************************************************
   *
   * l_txCharArch_SCI2()
   *
   */
  /**
   * \brief interface specific character send function
   *
   * This function transmits a given char over the SCI2 of the chip
   *
   * - internal function
   * - slave only
   *
   * \param  txChar         the character to be transmitted
   *
   * \retval none
   *
   */
  STACK void l_txCharArch_SCI2 (l_u8 txChar)
  {

    /*
     * move transmit character into the data register
     */

    USART_DR = txChar;

    return ;

  } /* end l_txCharArch_SCI2() */

  /*********************************************************************
   *
   * l_changeBaudrateArch_SCI2()
   *
   */
  /**
   * \brief interface specific baudrate changing function
   *
   * This function changes the baudrate for SCI2
   *
   * - internal function
   * - slave only
   *
   * \param  baudrate       the value to set the baudrate to
   *
   * \retval the character received
   *
   */
  /* Begin BUGZILLA_475 */
  STACK void l_changeBaudrateArch_SCI2 (l_u16 baudrate)
  {
    l_u16        divider = (LIN_BOARD_CPU_FREQ_HZ >> 8);
    l_u16        tmp;
    l_u8         count;
    l_u8         calc = 0x00;

    switch (baudrate)
    {
      #if (LIN_BOARD_CPU_FREQ_HZ == 8000000)      /* Fcpu = 8 MHz  */

           case 2400:          /* Normal baudrate */
             USART_BRR2 = 0x05;
             USART_BRR1 = 0xD0;
             
           break;

           case 9600:          /* Normal baudrate */
             USART_BRR2 = 0x01;
             USART_BRR1 = 0x34;

           break;

           case 10417:          /* Normal baudrate */
             USART_BRR1 = 0x30;

           break;

           case 19200:         /* Normal baudrate */
             USART_BRR2 = 0x01;
             USART_BRR1 = 0x1A;

           break;

           case 20000:         /* Wake-up/Normal baudrate */
             USART_BRR1 = 0x19;

           break;

           default:            /* For all other cases let's calculate the correct values  */
             calc = 0x01;
               
           break;
      #elif (LIN_BOARD_CPU_FREQ_HZ == 16000000)    /* Fcpu = 16 MHz  */

           case 2400:          /* Normal baudrate */
             USART_BRR2 = 0x1B;
             USART_BRR1 = 0xA0;

           break;

           case 9600:          /* Normal baudrate */
             USART_BRR2 = 0x03;
             USART_BRR1 = 0x68;

           break;

           case 10417:         /* Normal baudrate */
             USART_BRR1 = 0x60;

           break;

           case 19200:         /* Normal baudrate */
             USART_BRR2 = 0x01;
             USART_BRR1 = 0x34;

           break;

           case 20000:         /* Wake-up/Normal baudrate */
             USART_BRR1 = 0x32;

           break;

           default:            /* For all other cases let's calculate the correct values  */
             calc = 0x01;
               
           break;             
        #else
					 default:
             calc = 0x01;
					 break;
        #endif /* #if (LIN_BOARD_CPU_FREQ_HZ == 8000000) */
    }

    if (calc)
    {
      mantissa = (l_u16)(divider / (baudrate>>4));

      fraction = 0;

      for (count = 0; count < 16; ++count)
      {
         tmp = (l_u16)(LIN_BOARD_CPU_FREQ_HZ / ((mantissa<<4) + count));
         if (tmp >= baudrate)
         {
            fraction = count;
         }
      }

      /* Begin BUGZILLA_472 */
      USART_BRR2 =  ((l_u8) (fraction | ((l_u8) mantissa>>8))); /* DIV_Mantissa[11:8]+DIV_Fraction[3:0] */
      /* End BUGZILLA_472 */
      USART_BRR1 = (l_u8)mantissa;           /* DIV_Mantissa[7:0] */
    }

    return;

} /* l_changeBaudrateArch_SCI2() */
/* End BUGZILLA_475 */

#endif /* SCI2_USED */

/*********************************************************************
 *
 * ISRs
 *
 *********************************************************************/

/*********************************************************************
 * SCI 1
 *********************************************************************/

#ifdef SCI1_USED

  /*********************************************************************
   *
   * l_ifc_rx_SCI1()
   *
   */
  /**
   * \brief interface specific Rx notification function
   *
   * ISR for the SCI1 of the STM8
   *
   * - internal function
   * - slave only
   *
   * \param  none
   *
   * \retval none
   *
   */
  STACK void l_ifc_rx_SCI1 (void)
  {
    l_u8          rxChar;
    l_irqmask     irqMask;

    irqMask = l_sys_irq_disable();

    rxChar  = LINUART_DR;

    #ifdef LIN_PROTOCOL_SWITCH
      if (IFCCTRL_FLAGS(LIN_IFC_SCI1) & LIN_PROTOCOL_OTHER)
      {
        PROTOCOLCALLBACK(LIN_IFC_SCI1);
      }
      else
    #endif /* LIN_PROTOCOL_SWITCH */
    {
      /*
       * the sequence to clear rx interrupt flag is
       *  - read status register
       *  - read data register
       * status register is read when testing for error
       * data register is read when obtaining the character received
       */

      if (l_serialErrorArch_SCI1() != 0)
      {
         {
          #ifdef LIN_SLAVE_LINSCI
            if (LINUART_SR & LINUART_SR_LHE)
            {
              /* Begin BUGZILLA_472 */
              LINUART_CR5 &= ((volatile l_u8) ~(LINUART_CR5_LSF));  /* Clear LSF flag to solve possible sync lock */
              /* End BUGZILLA_472 */
            }
          #endif


          /*
           * if we have a serial error make sure that readback
           * is also invalid
           */
          /* Begin BUGZILLA_472 */
          rxChar = (l_u8) (TRANSFERCTRL_CHARSENT(LIN_IFC_SCI1) - 1);
          /* End BUGZILLA_472 */

          #ifdef LIN_MASTER_NODE
            if (IFCCTRL_STATE(LIN_IFC_SCI1) != LIN_STATE_SEND_SYNC)
            {
              IFCCTRL_SERIALERROR(LIN_IFC_SCI1) = 1;
            } 
            /* Begin BUGZILLA_1166 */
            /* End BUGZILLA_1166 */              
          #else
              IFCCTRL_SERIALERROR(LIN_IFC_SCI1) = 1;
          #endif
         }
      } /* if (serial error) */
      
      /* Begin BUGZILLA_1166 */
      if (LIN_RECEIVED_BREAK())
      {
        LIN_CLEAR_BREAK();
        LIN_RIE();
      }
			/* End BUGZILLA_1166 */
			
      #ifdef LIN_SLAVE_LINSCI
        if ((LINUART_CR5 & LINUART_CR5_LHDF) != 0) /* header detection flag */
        {
           /*
            * set status bit if a transfer is ongoing
            */
           /* Begin BUGZILLA_472 */
           LINUART_CR5 &= ((volatile l_u8) ~(LINUART_CR5_LHDF)); /* Reset LHDF flag */
           /* End BUGZILLA_472 */
           
           if ((IFCCTRL_STATE(LIN_IFC_SCI1) == LIN_STATE_SEND_DATA)
               ||
               (IFCCTRL_STATE(LIN_IFC_SCI1) == LIN_STATE_RECEIVE_DATA))
           {
              l_serialErrorFrameBody(__OPTIONAL_IFC_PARAMETER__ 1);
           }
           
           if (IFCCTRL_STATE(LIN_IFC_SCI1) != LIN_STATE_IDLE)
           {
             #ifdef __MORE_IFCS__
               l_terminateTransfer(LIN_IFC_SCI1);
             #else
               l_terminateTransfer();
             #endif
           }

           l_TimerStart(__OPTIONAL_TIMER_IFC_PARAMETER_COMMA__
                        LIN_FRAME_SUBCHANNEL,
                        LIN_FRAME_TIMER_VALUE(__MANDATORY_IFC_PARAMETER__));
           /* Begin BUGZILLA_474 */
           #ifdef LIN_SLAVE_LINSCI_FULL
             if (IFCCTRL_STATE(LIN_IFC_SCI1) == LIN_STATE_BUSSLEEP)
             {
               IFCCTRL_SERIALERROR(LIN_IFC_SCI1) = 1;
             }
             else
             {
               IFCCTRL_STATE(LIN_IFC_SCI1) = LIN_STATE_RECEIVE_IDENTIFIER;
             }
           #endif /* header detection method */
					 
           /* End BUGZILLA_474 */
        } /* if (header detection flag) */

      #endif

      /*
       * enable fast sending
       */
      if (IFCCTRL_STATE(LIN_IFC_SCI1) == LIN_STATE_SEND_DATA)
      {
        l_u16                          sum;
        if (rxChar == TRANSFERCTRL_CHARSENT(LIN_IFC_SCI1)) 
        {
          if (TRANSFERCTRL_BYTESSENT(LIN_IFC_SCI1) < 
              (TRANSFERCTRL_BYTESTOSEND(LIN_IFC_SCI1) - 1))
          {
            /*
             * frame is not complete, send next data byte
             */
            
            /*
             * copy next char into charSent
             */
            TRANSFERCTRL_CHARSENT(LIN_IFC_SCI1) =
              TRANSFERCTRL_BUFFER(LIN_IFC_SCI1)
              [TRANSFERCTRL_BYTESSENT(LIN_IFC_SCI1)];
            TRANSFERCTRL_BYTESSENT(LIN_IFC_SCI1)++;

            /*
             * send next character */
            LINUART_DR = TRANSFERCTRL_CHARSENT(LIN_IFC_SCI1);

            /*
             * calculate checksum
             */
            sum = (l_u16) TRANSFERCTRL_CHECKSUM(LIN_IFC_SCI1)
              + (l_u16) rxChar;
            TRANSFERCTRL_CHECKSUM(LIN_IFC_SCI1)=
              (l_u8) (sum + (sum >> 8));
          } /* if (data bytes) */
          
          else if  (TRANSFERCTRL_BYTESSENT(LIN_IFC_SCI1)
                    == (TRANSFERCTRL_BYTESTOSEND(LIN_IFC_SCI1) - 1))
          {
            /*
             * next byte is checksum byte
             */
            #ifdef LIN_DEBUG
              #ifdef LIN_MASTER_NODE            
                l_dbgMasterSendBuffer(TRANSFERCTRL_BUFFER(LIN_IFC_SCI1),
                                      TRANSFERCTRL_BYTESTOSEND(LIN_IFC_SCI1));
              #else
                l_dbgSlaveSendBuffer(TRANSFERCTRL_BUFFER(LIN_IFC_SCI1),
                                 TRANSFERCTRL_BYTESTOSEND(LIN_IFC_SCI1));
              #endif /* LIN_MASTER_NODE */
            #endif /* LIN_DEBUG */

            #ifndef LIN_MASTER_NODE
              #ifdef LIN_SLAVE_LINSCI_AUTOSYNC
                /*
                 * set LDUM bit here to allow hardware to reset
                 * nominal baudrate automatically
                 */
                LINUART_CR5 |= LINUART_CR5_LDUM;
                
              #endif /* LIN_SLAVE_LINSCI_AUTOSYNC*/
            #endif /* LIN_MASTER_NODE */

            /*
             * calculate checksum
             */
            sum = (l_u16) TRANSFERCTRL_CHECKSUM(LIN_IFC_SCI1) + (l_u16) rxChar;
            
            TRANSFERCTRL_CHARSENT(LIN_IFC_SCI1) =
              (l_u8) ~((l_u8) (sum + (sum >> 8)));

            /*
             * send checksum byte
             */
            LINUART_DR = TRANSFERCTRL_CHARSENT(LIN_IFC_SCI1);

            TRANSFERCTRL_BUFFER(LIN_IFC_SCI1)
              [TRANSFERCTRL_BYTESSENT(LIN_IFC_SCI1)] =
              TRANSFERCTRL_CHARSENT(LIN_IFC_SCI1);
            TRANSFERCTRL_BYTESSENT(LIN_IFC_SCI1)++;


          } /* if (checksum byte) */
          else
          {
            /*
             * frame complete
             */
            #ifdef __MORE_IFCS__
              l_stateSendData(LIN_IFC_SCI1, rxChar);
            #else
              l_stateSendData(rxChar);
            #endif
          } /* if (frame complete) */             
        } /* if (readback ok) */
        else
        {
          /* readback failed */
          /* handle it as serial error */
          #ifdef __MORE_IFCS__
            l_serialError(LIN_IFC_SCI1);
          #else
            l_serialError();
          #endif
        }
      } /* if (state send data) */

      #ifdef LIN_MASTER_NODE
        else if (IFCCTRL_STATE(LIN_IFC_SCI1) == LIN_STATE_SEND_IDENTIFIER)
        {
          if (rxChar == TRANSFERCTRL_CHARSENT(LIN_IFC_SCI1)) 
          {
            TRANSFERCTRL_CHARSENT(LIN_IFC_SCI1) =
              TRANSFERCTRL_BUFFER(LIN_IFC_SCI1)[LIN_FRAME_ID_POS];

            /*
             * send next byte
             */
            LINUART_DR = TRANSFERCTRL_CHARSENT(LIN_IFC_SCI1);

            TRANSFERCTRL_BYTESSENT(LIN_IFC_SCI1) = 1;
            
            IFCCTRL_STATE(LIN_IFC_SCI1) = LIN_STATE_RECEIVE_IDENTIFIER;
          }
          else
          {
            #ifdef __MORE_IFCS__
              l_serialError(LIN_IFC_SCI1);
            #else
              l_serialError();
            #endif
          }
        } /* if (send_identifier) */
      #endif /* LIN_MASTER_NODE */
      #ifdef LIN_MASTER_NODE
        else if (IFCCTRL_STATE(LIN_IFC_SCI1) == LIN_STATE_RECEIVE_IDENTIFIER)
        {
          if (rxChar == TRANSFERCTRL_CHARSENT(LIN_IFC_SCI1)) 
          {
            if (TRANSFERCTRL_STATE(LIN_IFC_SCI1) == LIN_TRANSFER_SEND)
            {
              /*
               * this frame is published by the master node
               * so start sending data here
               */
              TRANSFERCTRL_CHARSENT(LIN_IFC_SCI1) =
                TRANSFERCTRL_BUFFER(LIN_IFC_SCI1)[LIN_FRAME_DATA_POS];
              
              LINUART_DR = TRANSFERCTRL_CHARSENT(LIN_IFC_SCI1);

              TRANSFERCTRL_BYTESSENT(LIN_IFC_SCI1) = 2;
              
              IFCCTRL_STATE(LIN_IFC_SCI1) = LIN_STATE_SEND_DATA;
            }
            else
            {
              /*
               * the frame is published by a slave node,
               * so the master will only listen to the frame
               */
              
              IFCCTRL_STATE(LIN_IFC_SCI1) = LIN_STATE_RECEIVE_DATA;
              
            } /* if (frame published by master) */
            
          }
          else
          {
            #ifdef __MORE_IFCS__
              l_serialError(LIN_IFC_SCI1);
            #else
              l_serialError();
            #endif
          }
        }
      #endif /* LIN_MASTER_MODE */
      else
      {
        /*
         * set LDUM bit in autosync mode
         */
        #ifndef LIN_MASTER_NODE
          #ifdef LIN_SLAVE_LINSCI_AUTOSYNC
            if (IFCCTRL_STATE(LIN_IFC_SCI1) == LIN_STATE_RECEIVE_DATA)
            {
              if  ((TRANSFERCTRL_BYTESSENT(LIN_IFC_SCI1) + 1)
                   == (TRANSFERCTRL_BYTESTOSEND(LIN_IFC_SCI1) - 1))
              {
                /*
                 * set LDUM bit here to allow hardware to reset
                 * nominal baudrate automatically
                 */
                LINUART_CR5 |= LINUART_CR5_LDUM;
              }
              
            } /* if (STATE_SEND_DATA) */
      
          #endif /* LIN_SLAVE_LINSCI_AUTOSYNC*/
        #endif /* LIN_MASTER_NODE */

        #ifdef __MORE_IFCS__
          l_evaluateData(LIN_IFC_SCI1, rxChar);
        #else
          l_evaluateData(rxChar);
        #endif
      }
      /*
       * now clear the status flags
       */
  /* Begin BUGZILLA_377 */
      LINUART_SR;
      LINUART_CR5;
      LINUART_DR;

      #if (defined(LIN_SLAVE_LINSCI) && defined(LIN_SLAVE_LINSCI_MUTE_MODE))

        if (IFCCTRL_STATE(LIN_IFC_SCI1) == LIN_STATE_IDLE)
        {
           /*
            * enable mute mode
            */
           LINUART_CR2 |= LINUART_CR2_RWU;
  /* End BUGZILLA_377 */
        }
      #endif /* MUTE MODE */


    }

    l_sys_irq_restore(irqMask);

    return ;

  } /* end l_ifc_rx_SCI1() */

  /*********************************************************************
   *
   * l_ifc_tx_SCI1()
   *
   */
  /**
   * \brief interface specific Tx notification function
   *
   * The function to be called by the application if a character has
   * been sent. Currently empty
   *
   * - internal function
   * - slave only
   *
   * \param  none
   *
   * \retval none
   *
   */
  STACK void l_ifc_tx_SCI1 (void)
  {
    /*
     * currently nothing to be done...
     */

    return ;

  } /* end l_ifc_tx_SCI1() */


#endif /* SCI1_USED */

/*********************************************************************
 * SCI 2
 *********************************************************************/

#ifdef SCI2_USED

  /*********************************************************************
   *
   * l_ifc_rx_SCI2()
   *
   */
  /**
   * \brief interface specific Rx notification function
   * 
   * ISR for the SCI2 of the STM8
   *
   * - internal function
   * - slave only
   *
   * \param  none
   *
   * \retval none
   *
   */
  STACK void l_ifc_rx_SCI2 (void)
  {
    l_u8          rxChar;
    l_irqmask     irqMask;

    irqMask = l_sys_irq_disable();

    rxChar  = USART_DR;
    
    #ifdef LIN_PROTOCOL_SWITCH
      if (IFCCTRL_FLAGS(LIN_IFC_SCI2) & LIN_PROTOCOL_OTHER)
      {
        PROTOCOLCALLBACK(LIN_IFC_SCI2);
      }
      else
    #endif /* LIN_PROTOCOL_SWITCH */
    {
      /*
       * the sequence to clear rx interrupt flag is
       *  - read status register
       *  - read data register
       * status register is read when testing for error
       * data register is read when obtaining the character received
       */

      if (l_serialErrorArch_SCI2() != 0)
      {
         {
          /*
           * if we have a serial error make sure that readback
           * is also invalid
           */
          /* Begin BUGZILLA_472 */
          rxChar = (l_u8) (TRANSFERCTRL_CHARSENT(LIN_IFC_SCI2) - 1);
          /* End BUGZILLA_472 */

          #ifdef LIN_MASTER_NODE
            if (IFCCTRL_STATE(LIN_IFC_SCI2) != LIN_STATE_SEND_SYNC)
            {
              IFCCTRL_SERIALERROR(LIN_IFC_SCI2) = 1;
            }
   /* Begin BUGZILLA_378 */
            if (LIN_RECEIVED_BREAK())
            {
              LIN_CLEAR_BREAK();
              LIN_RIE();
            }
   /* End BUGZILLA_378 */
          #else
              IFCCTRL_SERIALERROR(LIN_IFC_SCI2) = 1;
          #endif
         }
      } /* if (serial error) */


      /*
       * enable fast sending
       */
      if (IFCCTRL_STATE(LIN_IFC_SCI2) == LIN_STATE_SEND_DATA)
      {
        l_u16                          sum;
        if (rxChar == TRANSFERCTRL_CHARSENT(LIN_IFC_SCI2)) 
        {
          if (TRANSFERCTRL_BYTESSENT(LIN_IFC_SCI2) < 
              (TRANSFERCTRL_BYTESTOSEND(LIN_IFC_SCI2) - 1))
          {
            /*
             * frame is not complete, send next data byte
             */
            
            /*
             * copy next char into charSent
             */
            TRANSFERCTRL_CHARSENT(LIN_IFC_SCI2) =
              TRANSFERCTRL_BUFFER(LIN_IFC_SCI2)
              [TRANSFERCTRL_BYTESSENT(LIN_IFC_SCI2)];
              TRANSFERCTRL_BYTESSENT(LIN_IFC_SCI2)++;

            /*
             * send next character */
            USART_SR;
            USART_DR = TRANSFERCTRL_CHARSENT(LIN_IFC_SCI2);

            /*
             * calculate checksum
             */
            sum = (l_u16) TRANSFERCTRL_CHECKSUM(LIN_IFC_SCI2)
              + (l_u16) rxChar;
            TRANSFERCTRL_CHECKSUM(LIN_IFC_SCI2)=
              (l_u8) (sum + (sum >> 8));
          } /* if (data bytes) */
          
          else if  (TRANSFERCTRL_BYTESSENT(LIN_IFC_SCI2)
                    == (TRANSFERCTRL_BYTESTOSEND(LIN_IFC_SCI2) - 1))
          {
            /*
             * next byte is checksum byte
             */
            #ifdef LIN_DEBUG
              #ifdef LIN_MASTER_NODE            
                l_dbgMasterSendBuffer(TRANSFERCTRL_BUFFER(LIN_IFC_SCI2),
                                      TRANSFERCTRL_BYTESTOSEND(LIN_IFC_SCI2));
              #else
                l_dbgSlaveSendBuffer(TRANSFERCTRL_BUFFER(LIN_IFC_SCI2),
                                 TRANSFERCTRL_BYTESTOSEND(LIN_IFC_SCI2));
              #endif /* LIN_MASTER_NODE */
            #endif /* LIN_DEBUG */

            /*
             * calculate checksum
             */
            sum = (l_u16) TRANSFERCTRL_CHECKSUM(LIN_IFC_SCI2) + (l_u16) rxChar;
            
            TRANSFERCTRL_CHARSENT(LIN_IFC_SCI2) =
              (l_u8) ~((l_u8) (sum + (sum >> 8)));

            /*
             * send checksum byte
             */
            USART_SR;
            USART_DR = TRANSFERCTRL_CHARSENT(LIN_IFC_SCI2);

            TRANSFERCTRL_BUFFER(LIN_IFC_SCI2)
              [TRANSFERCTRL_BYTESSENT(LIN_IFC_SCI2)] =
              TRANSFERCTRL_CHARSENT(LIN_IFC_SCI2);
            TRANSFERCTRL_BYTESSENT(LIN_IFC_SCI2)++;


          } /* if (checksum byte) */
          else
          {
            /*
             * frame complete
             */
            #ifdef __MORE_IFCS__
              l_stateSendData(LIN_IFC_SCI2, rxChar);
            #else
              l_stateSendData(rxChar);
            #endif
          } /* if (frame complete) */             
        } /* if (readback ok) */
        else
        {
          /* readback failed */
          /* handle it as serial error */
          #ifdef __MORE_IFCS__
            l_serialError(LIN_IFC_SCI2);
          #else
            l_serialError();
          #endif
        }
      } /* if (state send data) */        
      #ifdef LIN_MASTER_NODE
        else if (IFCCTRL_STATE(LIN_IFC_SCI2) == LIN_STATE_SEND_IDENTIFIER)
        {
          if (rxChar == TRANSFERCTRL_CHARSENT(LIN_IFC_SCI2)) 
          {
            TRANSFERCTRL_CHARSENT(LIN_IFC_SCI2) =
              TRANSFERCTRL_BUFFER(LIN_IFC_SCI2)[LIN_FRAME_ID_POS];

            /*
             * send next byte
             */
            USART_SR;
            USART_DR = TRANSFERCTRL_CHARSENT(LIN_IFC_SCI2);

            TRANSFERCTRL_BYTESSENT(LIN_IFC_SCI2) = 1;
            
            IFCCTRL_STATE(LIN_IFC_SCI2) = LIN_STATE_RECEIVE_IDENTIFIER;
          }
          else
          {
            #ifdef __MORE_IFCS__
              l_serialError(LIN_IFC_SCI2);
            #else
              l_serialError();
            #endif
          }
        } /* if (send_identifier) */
      #endif /* LIN_MASTER_NODE */
      #ifdef LIN_MASTER_NODE
        else if (IFCCTRL_STATE(LIN_IFC_SCI2) == LIN_STATE_RECEIVE_IDENTIFIER)
        {
          if (rxChar == TRANSFERCTRL_CHARSENT(LIN_IFC_SCI2)) 
          {
            if (TRANSFERCTRL_STATE(LIN_IFC_SCI2) == LIN_TRANSFER_SEND)
            {
              /*
               * this frame is published by the master node
               * so start sending data here
               */
              TRANSFERCTRL_CHARSENT(LIN_IFC_SCI2) =
                TRANSFERCTRL_BUFFER(LIN_IFC_SCI2)[LIN_FRAME_DATA_POS];
              
              USART_SR;
              USART_DR = TRANSFERCTRL_CHARSENT(LIN_IFC_SCI2);
              
              TRANSFERCTRL_BYTESSENT(LIN_IFC_SCI2) = 2;
              
              IFCCTRL_STATE(LIN_IFC_SCI2) = LIN_STATE_SEND_DATA;
            }
            else
            {
              /*
               * the frame is published by a slave node,
               * so the master will only listen to the frame
               */
              
              IFCCTRL_STATE(LIN_IFC_SCI2) = LIN_STATE_RECEIVE_DATA;
              
            } /* if (frame published by master) */
            
          }
          else
          {
            #ifdef __MORE_IFCS__
              l_serialError(LIN_IFC_SCI2);
            #else
              l_serialError();
            #endif
          }
        }
      #endif /* LIN_MASTER_MODE */
      else
      {
        
        #ifdef __MORE_IFCS__
          l_evaluateData(LIN_IFC_SCI2, rxChar);
        #else
          l_evaluateData(rxChar);
        #endif
      }

      /*
       * now clear the status flags
       */
      USART_SR;
      USART_CR3;
      USART_DR;

    }

    l_sys_irq_restore(irqMask);

    return ;

  } /* end l_ifc_rx_SCI2() */

  /*********************************************************************
   *
   * l_ifc_tx_SCI2()
   *
   */
  /**
   * \brief interface specific Rx notification function
   *
   * The function to be called by the application if a character has
   * been sent. Currently empty
   *
   * - internal function
   * - slave only
   *
   * \param  none
   *
   * \retval none
   *
   */
  STACK void l_ifc_tx_SCI2 (void)
  {
    /*
     * currently nothing to be done...
     */

    return ;

  } /* end l_ifc_tx_SCI2() */


#endif /* SCI2_USED */

/*
 * eof lin_stm8.c
 */
