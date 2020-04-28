/*********************************************************************
 *
 * Project:      LIN 2.1 driver suite
 *
 * File name:    lin_def.c
 *
 * Copyright:    (c) STMicroelectronics
 *
 * Author:       Giuseppe Stefano Fazio, STMicroelectronics
 *
 * Description:  User definitions regarding the driver hardware
 *               configuration
 *
 * Target:       all
 *
 * Names:        gsf  Giuseppe Stefano Fazio  STMicroelectronics
 *
 * TO BE CHANGED BY THE USER
 *
 *********************************************************************/

/*********************************************************************
 *
 * History:
 *
 * 20071218  v1.0   gsf  - Initial version for STM8
 *
 *********************************************************************/

/** \file lin_def.c
 */

#define LIN_DEF_C

#define LIN_DEF_C_MAJOR_VERSION  1
#define LIN_DEF_C_MINOR_VERSION  31
#define LIN_DEF_C_PATCH_VERSION  1

#define LIN_DEF_C_STM

/*********************************************************************
 * includes
 *********************************************************************/

#include "lin_types.h"
#include "lin_def.h"
#include "lin_def_arch_include.h"
#include "lin_general.h"
#include "lin_cfg_types.h"
#include "lin_arch_include.h"
#include "lin_diag.h"
#include "lin_version_control.h"


/*********************************************************************
 * local macros
 *********************************************************************/


/*********************************************************************
 * local typedefs
 *********************************************************************/


/*********************************************************************
 * global constants
 *********************************************************************/

/*********************************************************************
 * function declarations
 *********************************************************************/
STACK extern void      linIrqDisable (void) REENTRANT;
STACK extern void      linIrqEnable (void) REENTRANT;

/*********************************************************************
 *
 * Implementation
 *
 *********************************************************************/

/*********************************************************************
 *
 * l_sys_irq_disable()
 *
 */
/**
 * \brief function to disable interrupts
 *
 * This function is required by the standard to put the controler
 * in a state in which no interrupts can occur.
 * Refer to the user docu for more information!
 *
 * - standard API
 * - slave only
 *
 * \param  none
 *
 * \return an interrupt mask that may be used to re-enable the interrupts
 *
 */
STACK l_irqmask l_sys_irq_disable (void) REENTRANT
{
  /*
   * the user should add the implementation of the interrupt
   * locking/releasing functions here. It is not needed to use
   * another level of function calls here, the implementation
   * can go here
   * The function call here is preserved to provoke a compilation
   * error if the user fails to provide an implementation
   */
  linIrqDisable();

  return 0;
}

/*********************************************************************
 *
 * l_sys_irq_restore()
 *
 */
/**
 * \brief function to enable interrupts
 *
 * Function to reenable the interrupts, also demanded by the standard
 *
 * - standard API
 * - slave only
 *
 * \param  irqMask    state of interrupt register before disabling the
 *                    interrupts (highly architecture specific)
 *
 * \retval none
 *
 */
STACK void l_sys_irq_restore (l_irqmask irqMask) REENTRANT
{
  /*
   * the user should add the implementation of the interrupt
   * locking/releasing functions here. It is not needed to use
   * another level of function calls here, the implementation
   * can go here
   * The function call here is preserved to provoke a compilation
   * error if the user fails to provide an implementation
   */
  linIrqEnable();

  return ;
}

#if (defined(LIN_INCLUDE_READ_BY_ID) && defined(LIN_SLAVE_NODE))
  /*********************************************************************
   *
   * ld_readByIdCallback()
   *
   */
  /**
   * \brief callback function for read by id service
   *
   * Function is called by diagnostic module when read by id service
   * request is received by slave node with user defined id
   *
   * - diagnostic callback function
   * - slave only
   *
   * \param id       id of read by id request
   *        pBuffer  buffer to compose answer to
   *
   * \retval 1 if application has filled response buffer
   *
   */
  STACK l_bool ld_readByIdCallback(l_u8 id, l_u8 * pBuffer)
  {
     return 0;
  } /* end ld_readByIdCallback() */
#endif

#if (defined(LIN_21) && defined(LIN_SLAVE_NODE) && (defined(LIN_INCLUDE_READ_BY_ID) || defined(LIN_INCLUDE_COND_CHANGE_NAD)))
  /*********************************************************************
   *
   * ld_read_by_id_callout()
   *
   */
  /**
   * \brief function to send to the master a read by identifier response
   * with an identifier in the user defined area.
   * This implementation is an example, user should replace with its
   * application.
   *
   * This callout is used when the master node transmits a read by 
   * identifier request with an identifier in the user defined area. 
   * The slave node application will be called from the driver when such 
   * request is received.
   * The id parameter is the identifier in the user defined area (32 to 63), 
   * see Table 4.19 of LIN 2.1 specifications, from the read by identifier 
   * configuration request.
   * The data pointer points to a data area with 5 bytes. This area will be 
   * used by the application to set up the positive response, see the user 
   * defined area in Table 4.20 of LIN 2.1 specifications.
   *
   * - diagnostic callback function
   * - slave only
   *
   * \param  ifc                    handle of the interface
   * \param  id                     id of the requested item (in the range 
   *                                32 to 63, user defined ids)
   * \param  data                   buffer to hold the response message
   *
   * \retval LD_NEGATIVE_RESPONSE   The slave node will respond with a negative 
   *                                response as defined in Table 4.21 of LIN 2.1
   *                                specifications. In this case the data area is
   *                                not considered.
   * \retval LD_POSITIVE_RESPONSE   The slave node will setup a positive response
   *                                using the data provided by the application.
   * \retval LD_NO_RESPONSE         The slave node will not answer.
   *
   *
   */
  STACK l_u8 ld_read_by_id_callout(l_u8 id, l_u8* mydata)
  {
    l_u8 i;
    l_u8 retval = LD_NO_RESPONSE;
    
    /* Following code is an example - Real implementation is application-dependent */
    
    /*
     * the handling does essentially depend on the id of the
     * requested item
     */
    if (id == LIN_DIAG2_READ_USR_DEF_MIN)
    {
      /*
       * id received is user defined 32
       */


       for(i=0;i<5;i++)                                     
       {
         mydata[LIN_DIAG_SF_FRAME_DATA_POS + i] = (l_u8) (id + i + 1);  
       }
      
      retval = LD_POSITIVE_RESPONSE;
    }
    else if ((id > LIN_DIAG2_READ_USR_DEF_MIN)
             &&
             (id < LIN_DIAG2_READ_USR_DEF_MAX))
    {
      /*
       * id received is included between ]32..63[ (extremes excluded)
       * command failed, compose neg response       
       */
      mydata[LIN_DIAG_SF_FRAME_DATA_POS + 0] = LIN_DIAG_SID_READ_BY_ID;

      mydata[LIN_DIAG_SF_FRAME_DATA_POS + 1] = LIN_DIAG_NAK_ERROR_CODE;

      retval = LD_NEGATIVE_RESPONSE;
    }
    else 
    {
      /*
       * id received is user defined 63 - no response
       */
    }

  	return retval;

  } /* end ld_read_by_id_callout() */
#endif

#if (defined(LIN_INCLUDE_DATA_DUMP) && defined(LIN_SLAVE_NODE))
  /*********************************************************************
   *
   * ld_dataDumpCallback()
   *
   */
  /**
   * \brief callback function for data dump service
   *
   * Function is called by diagnostic module when data dump service
   * request is received by slave node
   *
   * - diagnostic callback function
   * - slave only
   *
   * \param sendBuf buffer holding master request
   *        recBuf  buffer to compose answer to
   *
   * \retval 1 if application has filled response buffer
   *
   */
  STACK l_bool ld_dataDumpCallback(const l_u8 * sendBuf, l_u8 * recBuf)
  {
    return 0;
  }
#endif

/*
 * eof lin_def.c
 */
