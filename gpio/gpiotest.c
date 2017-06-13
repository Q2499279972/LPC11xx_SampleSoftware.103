/****************************************************************************
 *   $Id:: gpiotest.c 3635 2010-06-02 00:31:46Z usb00423                    $
 *   Project: NXP LPC11xx GPIO example
 *
 *   Description:
 *     This file contains GPIO test modules, main entry, to test GPIO APIs.
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
#include "gpio.h"

/*****************************************************************************
**   Main Function  main()
******************************************************************************/


void delay(volatile uint32_t nTime)
{
	while(nTime--);
}
int main (void)
{
  SystemInit();

  GPIOInit();

  GPIOSetDir( PORT2, 6, 1 );
  GPIOSetDir( PORT2, 0, 1 );
  GPIOSetDir( PORT0, 1, 1 );
  GPIOSetDir( PORT3, 4, 1 );
  GPIOSetDir( PORT1, 9, 1 );
  GPIOSetDir( PORT0, 5, 1 );
  GPIOSetDir( PORT0, 4, 1 );
  GPIOSetDir( PORT2, 1, 1 );

  while( 1 )
  {
	  GPIOSetValue(2,6,0);
	  delay(1000000);
	  GPIOSetValue(2,6,1);
	  GPIOSetValue(2,0,0);
	  delay(1000000);
	  GPIOSetValue(2,0,1);
	  GPIOSetValue(0,1,0);
	  delay(1000000);
	  GPIOSetValue(0,1,1);
	  GPIOSetValue(3,4,0);
	  delay(1000000);
	  GPIOSetValue(3,4,1);
	  GPIOSetValue(1,9,0);
	  delay(1000000);
	  GPIOSetValue(1,9,1);
	  GPIOSetValue(0,5,0);
	  delay(1000000);
	  GPIOSetValue(0,5,1);
	  GPIOSetValue(0,4,0);
	  delay(1000000);
	  GPIOSetValue(0,4,1);
	  GPIOSetValue(2,1,0);
	  delay(1000000);
	  GPIOSetValue(2,1,1);
	  	  delay(10000000);
	  GPIOSetValue(2,6,0);
	  GPIOSetValue(2,0,0);
	  GPIOSetValue(0,1,0);
	  GPIOSetValue(3,4,0);
	  GPIOSetValue(1,9,0);
	  GPIOSetValue(0,5,0);
	  GPIOSetValue(0,4,0);
	  GPIOSetValue(2,1,0);
	 	  	  delay(10000000);
	  GPIOSetValue(2,6,1);
	  GPIOSetValue(2,0,1);
	  GPIOSetValue(0,1,1);
	  GPIOSetValue(3,4,1);
	  GPIOSetValue(1,9,1);
	  GPIOSetValue(0,5,1);
	  GPIOSetValue(0,4,1);
	  GPIOSetValue(2,1,1);
	  delay(10000000);


  }
}

/*********************************************************************************
**                            End Of File
*********************************************************************************/
