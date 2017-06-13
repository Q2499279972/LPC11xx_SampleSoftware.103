/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2007
 *
 *    File name   : main.c
 *    DESCRIPTION
 *    ===========
 *      This example project shows how to use the IAR Embedded Workbench for ARM
 *     to develop code for IAR-LPC-1114-SK board. It shows basic use of I/O,
 *     timer, ADC and interrupt controllers.
 *     It shows the Temperature and the Trimer position no LCD and UART
 *     Press B1 to see the temperature on the LCD.
 *     Press B2 to see the trimer position on the LCD
 *
 *    COMPATIBILITY
 *    =============
 *
 *      The example project is compatible with, on IAR-LPC-1343-SK evaluation
 *       board. By default, the project is configured to use the J-Link SWD
 *     interface.
 *
 *    CONFIGURATION
 *    =============
 *
 *     Jumpers:
 *     RST_E   - unfilled
 *     ISP_E   - unfilled
 *     TMS/SWD - SWD (2-3)
 *     TDO/SWO - SWO (2-3)
 *
 *     J-Lind_D - filled - to use with an external J-Link
 *              - unfilled - to use with the on board J-Link
 *
 *     The GettingStarted application is downloaded to the iFlash.
 *
 *    GETTING STARTED
 *     ===============
 *
 *       1) Start the IAR Embedded Workbench for ARM.
 *
 *       2) Select File->Open->Workspace...
 *        Open the following workspace:
 *
 *         <installation-root>\arm\examples\NXP\
 *          LPC13xx\IAR-LPC-1114-SK\GettingStarted\GettingStarted.eww
 *
 *       3) Run the program.
 *
 *    1. Date        : September 7, 2009
 *       Author      : Stoyan Choynev
 *       Description : Initial Revision
 *
 *
 *    $Revision: 36578 $
 **************************************************************************/
#include "includes.h"
#include <yfuns.h>

/*variable for clitical section entry control*/
Int32U CriticalSecCntr;
volatile Int32U Ticks;
/** external data **/

//** internal functions **/
static int MyLowLevelGetchar(void);
/** public functions **/
/*************************************************************************
 * Function Name: NVIC_IntEnable
 * Parameters: IntNumber - Interrup number
 * Return: void
 *
 * Description: Enable interrup at NVIC
 *
 *
 *************************************************************************/
void NVIC_IntEnable(Int32U IntNumber)
{
volatile unsigned long * pNVIC_SetEn = &SETENA0;

  assert((NVIC_WAKE_UP0 <= IntNumber) && (NVIC_PIO_0 >= IntNumber));
  IntNumber -= NVIC_WAKE_UP0;

  pNVIC_SetEn += IntNumber/32;
  *pNVIC_SetEn = (1UL<<(IntNumber%32));

}

/*************************************************************************
 * Function Name: NVIC_IntDisable
 * Parameters: IntNumber - Interrup number
 * Return: void
 *
 * Description: Disables interrup at NVIC
 *
 *
 *************************************************************************/
void NVIC_IntDisable(Int32U IntNumber)
{
volatile unsigned long * pNVIC_ClrEn = &CLRENA0;

  assert((NVIC_WAKE_UP0 <= IntNumber) && (NVIC_PIO_0 >= IntNumber));
  IntNumber -= NVIC_WAKE_UP0;

  pNVIC_ClrEn += IntNumber/32;
  *pNVIC_ClrEn = (1UL<<(IntNumber%32));

}

/*************************************************************************
 * Function Name: NVIC_ClrPend
 * Parameters: IntNumber - Interrup number
 * Return: void
 *
 * Description:Clear pending interrupt at NVIC
 *
 *
 *************************************************************************/
void NVIC_ClrPend(Int32U IntNumber)
{
volatile unsigned long * pNVIC_ClrPend = &CLRPEND0;

  assert((NVIC_WAKE_UP0 <= IntNumber) && (NVIC_PIO_0 >= IntNumber));
  IntNumber -= NVIC_WAKE_UP0;

  pNVIC_ClrPend += IntNumber/32;
  *pNVIC_ClrPend = (1UL<<(IntNumber%32));

}

/*************************************************************************
 * Function Name: NVIC_ClrPend
 * Parameters: IntNumber - Interrup number, Interrupt Priority
 * Return: void
 *
 * Description:Sets Interrupt priority
 *
 *
 *************************************************************************/
void NVIC_IntPri(Int32U IntNumber, Int8U Priority)
{
volatile Int8U * pNVIC_IntPri = (Int8U *)&IP0;

  assert((NVIC_WAKE_UP0 <= IntNumber) && (NVIC_PIO_0 >= IntNumber));
  IntNumber -= NVIC_WAKE_UP0;
  pNVIC_IntPri += IntNumber;
  *pNVIC_IntPri = Priority;
}

/*************************************************************************
 * Function Name: InitClock
 * Parameters: clock, AHB devider
 * Return: void
 *
 * Description: Initialize PLL to desired clock and  AHB devider
 *              Sys clock is Sys PLL output
 *
 *
 *************************************************************************/
void InitClock(Int32U clock, Int32U ahbdiv)
{
  /*Sys Oscilator Enable*/
  SYSOSCCLTRL = (MAIN_OSC_FREQ>(20MHZ))?(0x2):(0x0);
  /*Power Up SYS Oscilator*/
  PDRUNCFG_bit.SYSOSC_PD = 0;
  /*Enable Internal RC oscilator*/
  PDRUNCFG_bit.IRC_PD = 0;
  /*Select internal RC oscilator for
    Sys clock source*/
  MAINCLKUEN = 0;
  MAINCLKSEL = 0;
  MAINCLKUEN = 1;
  /*Configure SYS PLL*/
  /*Power Down SYS PLL*/
  PDRUNCFG_bit.SYSPLL_PD = 1;
  /*Select Sys Oscilator for
    SYS PLL source*/
  SYSPLLCLKUEN = 0;
  SYSPLLCLKSEL = 1;
  SYSPLLCLKUEN = 1;
  /*Calc M*/
  Int32U m = clock/MAIN_OSC_FREQ - 1;

  assert(m<32);
  /*Configure PLL frequency*/
  SYSPLLCTRL =  (m)    /*MSEL*/
             |  (0<<5) /*PSEL = 1*/
             |  (0<<7) /*DIRECT = 0*/
             |  (0<<8); /*BYPASS=0*/

  /*Power Up PLL*/
  PDRUNCFG_bit.SYSPLL_PD = 0;
  /*Set Sys AHB Clock devider*/
  SYSAHBCLKDIV_bit.DIV = ahbdiv;
  /*Wain until PLL locks*/
  while(!(SYSPLLSTAT_bit.LOCK));
  /*Select Sys PLL Output for
    Sys clock source*/
  MAINCLKUEN = 0;
  MAINCLKSEL = 3;
  MAINCLKUEN = 1;
}

/*************************************************************************
 * Function Name: SYS_GetMainClk
 * Parameters: none
 * Return: Int32U
 *
 * Description: return Main Clock [Hz]
 *
 *************************************************************************/
Int32U SYS_GetMainClk(void)
{
Int32U Clk;
  switch(MAINCLKSEL_bit.SEL)
  {
    case 0:
       Clk = I_RC_OSC_FREQ;
    break;
    case 1:
      Clk = MAIN_OSC_FREQ;
    break;
    case 2:
      Clk = WDT_OSC_FREQ;
    break;
    case 3:
      switch(SYSPLLCLKSEL_bit.SEL)
      {
        case 0:
          Clk = I_RC_OSC_FREQ;
        break;
        case 1:
          Clk = MAIN_OSC_FREQ;
        break;
        case 2:
          Clk = WDT_OSC_FREQ;
        break;

        default:
          Clk = 0;
        break;
      }

      if(!SYSPLLCTRL_bit.BYPASS)
        Clk *= (SYSPLLCTRL_bit.MSEL+1);
    break;

    default:
      Clk = 0;
    break;
  }
  return Clk;
}

void UART_Init (Int32U BaudRate)
{
Int32U Pclk;
  /*Enable Clocks*/
  UARTCLKDIV = 1;
  /**/
  while((Pclk=SYS_GetMainClk()/UARTCLKDIV)> 70MHZ)
  {
    UARTCLKDIV ++;
  }
  /*Enable UART register clock*/
  SYSAHBCLKCTRL_bit.UART = 1;
  /*Enable I/O Configuration registers*/
  SYSAHBCLKCTRL_bit.IOCON = 1;

  IOCON_PIO1_6 = 1;         //Uart RX function select
  IOCON_PIO1_7 = 1;         //Uart RX function select

  U0LCR_bit.WLS = 3;        //Serlect 8-Bits
  U0LCR_bit.SBS = 0;        //One Stop Bit
  U0LCR_bit.PE = 0;         //No parity

  U0LCR_bit.DLAB = 1;       //Devisor Latch Access ebabled
  //Set UART0 BAUDRATE
  U0DLL = ((((Pclk+8)/16)+BaudRate/2)/BaudRate)%256;  //
  U0DLM = ((((Pclk+8)/16)+BaudRate/2)/BaudRate)/256;  //
  U0LCR_bit.DLAB = 0;           //Clear Devisor Latch Access bit

  U0FCR = (1<<0);     //Enable UART0 FIFOs.
  U0TER = (1<<7);     //Enable Transmission
}

/*************************************************************************
 * Function Name: __write
 * Parameters: Low Level cahracter output
 *
 * Return:
 *
 * Description:
 *
 *************************************************************************/
size_t __write(int Handle, const unsigned char * Buf, size_t Bufsize)
{
size_t nChars = 0;

  for (/*Empty */; Bufsize > 0; --Bufsize)
  {
    while( !U0LSR_bit.THRE );  //Wait
    U0THR = * Buf++;
    ++nChars;
  }
  return nChars;
}
/*************************************************************************
 * Function Name: __read
 * Parameters: Low Level cahracter input
 *
 * Return:
 *
 * Description:
 *
 *************************************************************************/
size_t __read(int handle, unsigned char * buffer, size_t size)
{
  int nChars = 0;

  /* This template only reads from "standard in", for all other file
   * handles it returns failure. */
  if (handle != _LLIO_STDIN)
  {
    return _LLIO_ERROR;
  }

  for (/* Empty */; size > 0; --size)
  {
    int c = MyLowLevelGetchar();
    if (c < 0)
      break;

    *buffer++ = c;
    ++nChars;
  }

  return nChars;
}

/*************************************************************************
 * Function Name: Dly100us
 * Parameters: void *arg
 * Return: void
 *
 * Description: Delay [100us]
 *		
 *************************************************************************/
void Dly100us(Int32U Dly)
{
volatile Int32U  Dly100;
  for(;Dly;Dly--)
    for(Dly100 = 550; Dly100; Dly100--);
}


/*************************************************************************
 * Function Name: GpioInit
 * Parameters: void
 * Return: void
 *
 * Description: Reset all GPIO pins to default: primary function
 *
 *************************************************************************/
void GpioInit(void)
{
  // Set to inputs
  GPIO0DIR = \
  GPIO1DIR = \
  GPIO2DIR = \
  GPIO3DIR = 0;

  // Reset all GPIO pins to default primary function
  IOCON_PIO2_6 = \
  IOCON_PIO2_0 = \
  IOCON_RESET_PIO0_0 = \
  IOCON_PIO0_1 = \
  IOCON_PIO1_8 = \
  IOCON_PIO0_2 = \
  IOCON_PIO2_7 = \
  IOCON_PIO2_8 = \
  IOCON_PIO2_1 = \
  IOCON_PIO0_3 = \
  IOCON_PIO1_9 = \
  IOCON_PIO2_4 = \
  IOCON_PIO2_5 = \
  IOCON_PIO0_6 = \
  IOCON_PIO0_7 = \
  IOCON_PIO2_9 = \
  IOCON_PIO2_10 = \
  IOCON_PIO2_2 = \
  IOCON_PIO0_8 = \
  IOCON_SWCLK_PIO0_10 = \
  IOCON_PIO2_11 = \
  IOCON_PIO3_0 = \
  IOCON_PIO3_1 = \
  IOCON_PIO2_3 = \
  IOCON_PIO3_2 = \
  IOCON_PIO1_5 = \
  IOCON_PIO3_3 = 0x50;

  IOCON_PIO1_10 = \
  IOCON_R_PIO0_11 = \
  IOCON_R_PIO1_0 = \
  IOCON_R_PIO1_1 = \
  IOCON_R_PIO1_2 = \
  IOCON_SWDIO_PIO1_3 = \
  IOCON_PIO1_4 = \
  IOCON_PIO1_11 = 0xD0;

  IOCON_PIO0_4 = \
  IOCON_PIO0_5 = 0x00;
}
/*************************************************************************
 * Function Name: TMR0_IRQHandler
 * Parameters: none
 *
 * Return: none
 *
 * Description: Timer 0 interrupt handler
 *
 *************************************************************************/
void CT32B0_IRQHandler (void)
{
  ++Ticks;
  // clear interrupt
  TMR32B0IR_bit.MR0INT = 1;
  /**/
  NVIC_ClrPend(NVIC_CT32B0);
}

void CT32B0_Init(Int32U tps)
{
  Ticks = 0;
  // Enable TIM0 clocks
  SYSAHBCLKCTRL_bit.CT32B0 = 1;

  // Init Time0
  TMR32B0TCR_bit.CE = 0;     // counting  disable
  TMR32B0TCR_bit.CR = 1;     // set reset
  TMR32B0TCR_bit.CR = 0;     // release reset
  TMR32B0CTCR_bit.CTM = 0;   // Timer Mode: every rising PCLK edge
  TMR32B0MCR_bit.MR0I = 1;   // Enable Interrupt on MR0
  TMR32B0MCR_bit.MR0R = 1;   // Enable reset on MR0
  TMR32B0MCR_bit.MR0S = 0;   // Disable stop on MR0
  // set timer 0 period
  TMR32B0PR = 0;
  TMR32B0MR0 = (SYS_GetMainClk()/(SYSAHBCLKDIV))/(tps);
  // init timer 0 interrupt
  TMR32B0IR_bit.MR0INT = 1;  // clear pending interrupt
  TMR32B0TCR_bit.CE = 1;     // counting Enable
   /*Enable NVIC TMR0 Interrupt*/
  NVIC_IntEnable(NVIC_CT32B0);
  NVIC_IntPri(NVIC_CT32B0,16);
}

void CT32B0_Stop(void)
{
  TMR32B0TCR_bit.CE = 0;     // counting  disable

  NVIC_IntDisable(NVIC_CT32B0);

  SYSAHBCLKCTRL_bit.CT32B0 = 0; // disable clock
}

/*************************************************************************
 * Function Name: ADC_INIT
 * Parameters: none
 *
 * Return: none
 *
 * Description: ADC Initialzation.
 *
 *************************************************************************/
void ADC_Init(void)
{
  /*Enable ADC*/
  SYSAHBCLKCTRL_bit.ADC = 1;
  PDRUNCFG_bit.ADC_PD = 0;
  /*Selcet ADC pin function*/
  ANALOG_TRIM_IOCON = 1;
  /*Set ADC Devider*/
  AD0CR_bit.CLKDIV = ((SYS_GetMainClk()/(SYSAHBCLKDIV)) / 4500000) + 1;

  AD0CR_bit.SEL = 0;
  // Disable all interrupts
  AD0INTEN = 0;

  AD0CR_bit.START = 0;
  // Select ANALOG_TRIM_CHANNEL
  AD0CR_bit.SEL = 1<<ANALOG_TRIM_CHANNEL;
  //Start ADC
  AD0CR_bit.START = 1;
}

/*************************************************************************
 * Function Name: main
 * Parameters: none
 *
 * Return: none
 *
 * Description: main
 *
 *************************************************************************/
int main(void)
{
  Int32U tmp;

  /*Init clock*/
  InitClock(132MHZ, 2);
  /*Enable GPIO Clock*/
  SYSAHBCLKCTRL_bit.GPIO = 1;
  /*Enable IOCON Clock*/
  SYSAHBCLKCTRL_bit.IOCON = 1;
  /*Set all pins as input ports*/
  GpioInit();
   /*Init LED Ports*/
  LED1_DIR |= LED1_MASK;
  LED2_DIR |= LED2_MASK;
  LED3_DIR |= LED3_MASK;
  LED4_DIR |= LED4_MASK;
  LED5_DIR |= LED5_MASK;
  LED6_DIR |= LED6_MASK;
  LED7_DIR |= LED7_MASK;
  LED8_DIR |= LED8_MASK;
  /*UART Init: 115200 bps, 8-bit,
    1-Stop bit, Parity - None*/
  UART_Init(115200);
  /*Enable Timer. Ticks 10 times per second*/
  CT32B0_Init(10);
  
  printf("\n\r");
  printf("\n\r");
  printf("***************************\n\r");
  printf("*         TE1114-EK       *\n\r");
  printf("* Getting Started example *\n\r");
  printf("***************************\n\r");

  /*Init ADC*/
  ADC_Init();
   
  while(1)
  {   
    //LED_ON(LED1_PIN);
    //LED_ON(LED2_PIN);
    //LED_ON(LED3_PIN);
    //LED_ON(LED4_PIN);
    LED_ON(LED5_PIN);
    LED_ON(LED6_PIN);
    LED_ON(LED7_PIN);
    LED_ON(LED8_PIN);
    Dly100us(1000);
    //LED_OFF(LED1_PIN);
    //LED_OFF(LED2_PIN);
    //LED_OFF(LED3_PIN); 
    //LED_OFF(LED4_PIN);
    LED_OFF(LED5_PIN);
    LED_OFF(LED6_PIN);
    LED_OFF(LED7_PIN); 
    LED_OFF(LED8_PIN);
    Dly100us(1000);
          
    if(!B1_PIN)   //If Button 1 Is Pressed 
    {
      LED_ON(LED1_PIN);     
    }     
    else     
    {
      LED_OFF(LED1_PIN);    
    }               
    if(!B2_PIN)   //If Button 2 Is Pressed 
    {
      LED_ON(LED2_PIN);
    }    
    else     
    {LED_OFF(LED2_PIN);}     
    
    if(!B3_PIN)   //If Button 3 Is Pressed 
    {
      LED_ON(LED3_PIN);
    }     
    else     
    {LED_OFF(LED3_PIN);}  
    
    if(!B4_PIN)   //If Button 4 Is Pressed 
    {
      LED_ON(LED4_PIN); 
    }      
    else     
    {LED_OFF(LED4_PIN);} 
    
    AD0CR_bit.START = 0;
    // Select ANALOG_TRIM_CHANNEL
    AD0CR_bit.SEL = 1<<ANALOG_TRIM_CHANNEL;
    AD0CR_bit.START = 1;
    /**/
    tmp = AD0DR0_bit.RESULT;
    printf("Temp %d       \r",tmp);
    
    while(!Ticks);

    Ticks = 0;
   }
        
}

/** private functions **/
static int MyLowLevelGetchar(void)
{
int ch;

  if(U0LSR_bit.DR)
  {
    ch = U0RBR;
    if(U0LSR_bit.OE || U0LSR_bit.PE || U0LSR_bit.FE )
    {
      return (ch | 0x10000000);
    }
    return (ch & 0xff );
  }
  return -1;
}
