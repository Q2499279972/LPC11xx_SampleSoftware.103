/****************************************************************************
 *   $Id:: wdttest.c 3635 2010-06-02 00:31:46Z usb00423                     $
 *   Project: NXP LPC11xx WDT example
 *
 *   Description:
 *     This file contains WDT test modules, main entry, to test WDT APIs.
 *
 ****************************************************************************
 * Software that is described herein is for illustrative purposes only
 * which provides customers with programming information regarding the
 * products. This software is supplied "AS IS" without any warranties.
 * NXP Semiconductors assumes no responsibility or liability for the
 * use of the software, conveys no license or title under any patent,
 * copyright, or mask work right to the product. NXP Semiconductors
 * reserves the right to make changes in the software without
 * notification. NXP Semiconductors also make no representation or
 * warranty that such application will be suitable for the specified
 * use without further testing or modification.
****************************************************************************/
#include "LPC11xx.h"			/* LPC11xx Peripheral Registers */
#include "timer32.h"
#include "clkconfig.h"
#include "wdt.h"
#include "gpio.h"

extern volatile uint32_t timer32_0_counter;

/*****************************************************************************
**   Main Function  main()
******************************************************************************/
int main (void)
{
  SystemInit();
  
  /* Config WDT clock */
  WDT_CLK_Setup(WDTCLK_SRC_MAIN_CLK);
   
  /*** The main Function is an endless loop ****/
  init_timer32( 0, TIME_INTERVAL );
    
  WDTInit();
  enable_timer32( 0 );

  /* Set port 2_0 to output */
  GPIOSetDir( 2, 0, 1 );

  /* Set port 2_1 to output */
  GPIOSetDir( 2, 1, 1 );

  if ((LPC_SYSCON->SYSRESSTAT & 0x4) == 0x4)
  {
    LPC_SYSCON->SYSRESSTAT |= 0x4;
    while( 1 ) 
    {
	  /* I/O configuration and LED setting pending. */
	  if ( (timer32_0_counter > 0) && (timer32_0_counter <= 200) )
	  {
	    GPIOSetValue( 2, 1, 0 );
	  }
	  if ( (timer32_0_counter > 200) && (timer32_0_counter <= 400) )
	  {
	    GPIOSetValue( 2, 1, 1 );
	  }
	  else if ( timer32_0_counter > 400 )
	  {
	    timer32_0_counter = 0;
	  }
    }
  }
  else
  {
    while( 1 ) 
    {
	  /* I/O configuration and LED setting pending. */
	  if ( (timer32_0_counter > 0) && (timer32_0_counter <= 200) )
	  {
	    GPIOSetValue( 2, 0, 0 );
	  }
	  if ( (timer32_0_counter > 200) && (timer32_0_counter <= 400) )
	  {
	    GPIOSetValue( 2, 0, 1 );
	  }
	  else if ( timer32_0_counter > 400 )
	  {
	    timer32_0_counter = 0;
	  }
    }
  }
}

/*********************************************************************************
**                            End Of File
*********************************************************************************/
