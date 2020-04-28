/*********************************************************************
 *
 * Project:      LIN 2.1 driver suite
 *
 * File name:    lin_stm8L.c
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
 * 20120522	 v1.3   Jkh	 - Modified for STM8L
 * 20120822	 v1.3   Jkh	 - default and break added inside "l_changeBaudrateArch_SCI1" 
 *												 incase "#define LIN_BOARD_CPU_FREQ_HZ" is other than 8MHz or 16MHz 
 *												 to resolve the compiler issue
 *********************************************************************/

/** \file lin_stm8l.c
 */

#define LIN_STM8L_C

#define LIN_STM8L_C_MAJOR_VERSION  1
#define LIN_STM8L_C_MINOR_VERSION  32
#define LIN_STM8L_C_PATCH_VERSION  4

#define LIN_STM8L_C_STM

/*********************************************************************
 * includes
 *********************************************************************/
#include <limits.h>
#include "lin_cfg_types.h"
#include "lin_stm8L.h"
#include "lin_def_arch_include.h"
#include "lin_def.h"
#include "lin_def_gen.h"
#include "lin_general.h"
#include "lin_cfg.h"

#ifdef LIN_MASTER_NODE
  #include "lin_master.h"
#else
  #include "lin_slave.h"
#endif

#include "lin_version_control.h"

/*********************************************************************
 * environmental checks
 *********************************************************************/

#if !defined(__ARCH_STM8L__)
  #error "Invalid architecture"
#endif

/*********************************************************************
 * global variables
 *********************************************************************/
#ifdef LIN_SLAVE_LINSCI_AUTOSYNC
  l_u8                        l_storeBRR1;
#endif /* LIN_SLAVE_LINSCI_AUTOSYNC */

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
    USART1_CR1 = 0;


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
    USART1_CR1 = 0;

    USART1_CR2 |= USART1_CR2_RIEN;/* Receiver Interrupt Enable Mask */
    USART1_CR2 |= USART1_CR2_TEN; /* Transmitter Enable */
    USART1_CR2 |= USART1_CR2_REN; /* Receiver Enable */	
	

    l_changeBaudrateArch_SCI1(IFCCTRL_NORMALBAUDRATE(LIN_IFC_SCI1));

    /*
     * we clear the relevant status bits here
     */
    USART1_SR;
    USART1_DR;	

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

    USART1_CR1 = 0;
    USART1_CR2 = 0;

    /*
     * The fifth control register is relevant for LIN mode only
     */	

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
    USART1_DR = txChar;

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
    l_u16        calc_mantissa;
    l_u8         calc_fraction;
    l_u16        tmp;
    l_u8         i;
    l_u8         calc = 0x00;
    volatile l_u8 prescaler = 0;
    volatile l_u8 fraction  = 0;

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

           case 19200:         /* Normal baudrate */
             prescaler  = 0x1A;
             fraction   = 0x01;

           break;

           case 20000:         /* Wake-up/Normal baudrate */
             prescaler  = 0x19;

           break;

           default:            /* For all other cases let's calculate the correct values  */
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
					 default:
             calc = 0x01;
					 break;
        #endif /* #if (LIN_BOARD_CPU_FREQ_HZ == 8000000) */
    }

    if (calc)
    {
      calc_mantissa = (l_u16)(divider / (baudrate>>4));

      calc_fraction = 0;

      for (i = 0; i < 16; ++i)
      {
         tmp = (l_u16)(LIN_BOARD_CPU_FREQ_HZ / ((calc_mantissa<<4) + i));
         if (tmp >= baudrate)
         {
            fraction = i;
         }
      }

      /* Begin BUGZILLA_472 */
      fraction = ((l_u8) (calc_fraction | ((l_u8) calc_mantissa>>8))); /* DIV_Mantissa[11:8]+DIV_Fraction[3:0] */
      /* End BUGZILLA_472 */
      prescaler = ((l_u8) calc_mantissa);           /* DIV_Mantissa[7:0] */
    }
	
	USART1_BRR2   = fraction;
  USART1_BRR1   = prescaler;


  } /* l_changeBaudrateArch_SCI1() */
  /* Begin BUGZILLA_475 */
#endif /* SCI1_USED */

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

    rxChar  = USART1_DR;

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
          /*
           * if we have a serial error make sure that readback
           * is also invalid
           */
          /* Begin BUGZILLA_472 */
          rxChar = (l_u8) (TRANSFERCTRL_CHARSENT(LIN_IFC_SCI1) - 1);
          /* End BUGZILLA_472 */          
		  
					IFCCTRL_SERIALERROR(LIN_IFC_SCI1) = 1;
         }
      } /* if (serial error) */      
     
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
            USART1_DR = TRANSFERCTRL_CHARSENT(LIN_IFC_SCI1);

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
            
            /*
             * calculate checksum
             */
            sum = (l_u16) TRANSFERCTRL_CHECKSUM(LIN_IFC_SCI1) + (l_u16) rxChar;
            
            TRANSFERCTRL_CHARSENT(LIN_IFC_SCI1) =
              (l_u8) ~((l_u8) (sum + (sum >> 8)));

            /*
             * send checksum byte
             */
            USART1_DR = TRANSFERCTRL_CHARSENT(LIN_IFC_SCI1);

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
            USART1_DR = TRANSFERCTRL_CHARSENT(LIN_IFC_SCI1);

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
              
              USART1_DR = TRANSFERCTRL_CHARSENT(LIN_IFC_SCI1);

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
      USART1_SR;     
      USART1_DR;
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

/*
 * eof lin_stm8.c
 */
