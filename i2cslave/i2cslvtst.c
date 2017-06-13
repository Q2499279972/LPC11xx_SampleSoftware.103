/****************************************************************************
 *   $Id:: i2cslvtst.c 3635 2010-06-02 00:31:46Z usb00423                   $
 *   Project: NXP LPC11xx I2C example
 *
 *   Description:
 *     This file contains I2C slave test modules, main entry, to test I2C 
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
#include "type.h"
#include "i2cslave.h"

extern volatile uint8_t I2CWrBuffer[BUFSIZE];
extern volatile uint8_t I2CRdBuffer[BUFSIZE];
extern volatile uint32_t I2CSlaveState;
extern volatile uint32_t I2CReadLength, I2CWriteLength;

/*******************************************************************************
**   Main Function  main()
*******************************************************************************/
int main (void)
{
  uint32_t i;

  SystemInit();

  for ( i = 0; i < BUFSIZE; i++ )
  {
	I2CRdBuffer[i] = 0x00;
  }
  
  I2CSlaveInit();			/* initialize I2c */
 
  /* When the NACK occurs, the master has stopped the 
  communication. Just check the content of I2CRd/WrBuffer. */
  while ( I2CSlaveState != DATA_NACK );
  return 0;
}

/******************************************************************************
**                            End Of File
******************************************************************************/
