/****************************************************************************
 *   $Id:: systicktest.c 3635 2010-06-02 00:31:46Z usb00423                 $
 *   Project: NXP LPC11xx Systick example
 *
 *   Description:
 *     This file contains System tick test modules, main entry, to test 
 *     systick APIs.
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
#include "LPC11xx.h"                        /* LPC11xx definitions */
#include "gpio.h"

#define SYSTICK_DELAY		5000

volatile uint32_t TimeTick = 0;

/* SysTick interrupt happens every 10 ms */
void SysTick_Handler(void)
{
  TimeTick++;
}

void delaySysTick(uint32_t tick)
{
  uint32_t timetick;

  /* Clear SysTick Counter */
  SysTick->VAL = 0;
  /* Enable the SysTick Counter */
  SysTick->CTRL |= SYSTICK_ENABLE;

  timetick = TimeTick;
  while ((TimeTick - timetick) < tick);
  
  /* Disable SysTick Counter */
  SysTick->CTRL &= ~SYSTICK_ENABLE;
  /* Clear SysTick Counter */
  SysTick->VAL = 0;
  return;
}

/* Main Program */

int main (void) {
  SystemInit();

  /* Called for system library in core_cmx.h(x=0 or 3). */
  SysTick_Config( SYSTICK_DELAY );
  
  /* Enable AHB clock to the GPIO domain. */
  LPC_SYSCON->SYSAHBCLKCTRL |= (1<<6);
  
  /* Set port 2_0 to output */
  GPIOSetDir( 2, 0, 1 );

  while (1)                                /* Loop forever */
  {
	delaySysTick(SYSTICK_DELAY);
	GPIOSetValue( 2, 0, 0 );
	delaySysTick(SYSTICK_DELAY);
	GPIOSetValue( 2, 0, 1 );
  }
}
