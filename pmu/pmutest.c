/****************************************************************************
 *   $Id:: pmutest.c 3635 2010-06-02 00:31:46Z usb00423                     $
 *   Project: NXP LPC11xx PMU (Power Management Unit) example
 *
 *   Description:
 *     This file contains PMU slave test modules, main entry, to test PMU 
 *     slave APIs.
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
#include "timer16.h"
#include "pmu.h"

extern volatile uint32_t timer16_0_counter;

/*****************************************************************************
**   Main Function  main()
******************************************************************************/
int main (void)
{
#if !TEST_POWERDOWN
  uint32_t regVal;
#endif

  SystemInit();

  /* Enable AHB clock to the GPIO domain. */
  LPC_SYSCON->SYSAHBCLKCTRL |= (1<<6);

#if !TEST_POWERDOWN
  /* Setup wakeup source before sleep or deep sleep. Current,
  port0.1 is configured as wakeup source, falling edge trigger. */    
  PMU_Init();
  /* Put X/Y/Z into sleep mode. */
  /* For LPC11xx, USB_PLL and USB_PHY bits should not be 
  touched as they don't exist in the current silicon. Depending 
  on the configuration, these bits need to be carefully selected. */
//  regVal = USB_PHY_PD|USB_PLL_PD|SYS_PLL_PD|WDT_OSC_PD|ADC_PD|BOD_PD;
  /* Please note: LP_REGUL_PD and MAIN_REGUL_PD should be set one at 
  a time, not both at the same time. */
  regVal = SYS_PLL_PD|WDT_OSC_PD|ADC_PD|BOD_PD|LP_REGUL_PD;
  PMU_Sleep( MCU_DEEP_SLEEP, regVal );
#else
  /* Config PMU_PCON to let MCU to deep power down mode, retain 
  pattern in general-purpose register. A low on P1.4 will wake up
  the part from deep power down. */
  PMU_PowerDown();
#endif

  /* Using 16-bit timer 0 for LED blinky rate. */
  init_timer16(0, TIME_INTERVAL);
  enable_timer16(0);
  
  /* Set port 2_0 to output as LED control */
  GPIOSetDir( 2, 0, 1 );

  while (1)                                /* Loop forever */
  {
	/* I/O configuration and LED setting pending. */
	if ( (timer16_0_counter > 0) && (timer16_0_counter <= 200) )
	{
	  GPIOSetValue( 2, 0, 0 );
	}
	if ( (timer16_0_counter > 200) && (timer16_0_counter <= 400) )
	{
	  GPIOSetValue( 2, 0, 1 );
	}
	else if ( timer16_0_counter > 400 )
	{
	  timer16_0_counter = 0;
	}
  }
}

/*********************************************************************************
**                            End Of File
*********************************************************************************/
