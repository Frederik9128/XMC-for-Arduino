/**
 * @file xmc_ledts.c
 *
 * @cond
 *****************************************************************************
 * XMClib - XMC Peripheral Driver Library
 *
 * Copyright (c) 2015-2020, Infineon Technologies AG
 * All rights reserved.
 *
 * Boost Software License - Version 1.0 - August 17th, 2003
 *
 * Permission is hereby granted, free of charge, to any person or organization
 * obtaining a copy of the software and accompanying documentation covered by
 * this license (the "Software") to use, reproduce, display, distribute,
 * execute, and transmit the Software, and to prepare derivative works of the
 * Software, and to permit third-parties to whom the Software is furnished to
 * do so, all subject to the following:
 *
 * The copyright notices in the Software and this entire statement, including
 * the above license grant, this restriction and the following disclaimer,
 * must be included in all copies of the Software, in whole or in part, and
 * all derivative works of the Software, unless such copies or derivative
 * works are solely in the form of machine-executable object code generated by
 * a source language processor.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
 * SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
 * FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 *
 * To improve the quality of the software, users are encouraged to share
 * modifications, enhancements or bug fixes with Infineon Technologies AG
 * at XMCSupport@infineon.com.
 *****************************************************************************
 *
 * @endcond
 *
 */

/*********************************************************************************************************************
 * HEADER FILES
 ********************************************************************************************************************/
#include "xmc_ledts.h"

#if defined(LEDTS0)
#include "xmc_scu.h"

/*********************************************************************************************************************
 * MACROS
 ********************************************************************************************************************/
#define XMC_LEDTS_CLOCK_NOT_RUNNING 0U

/*********************************************************************************************************************
 * ENUMS
 ********************************************************************************************************************/

/*********************************************************************************************************************
 * DATA STRUCTURES
 ********************************************************************************************************************/

/*********************************************************************************************************************
 * GLOBAL DATA
 ********************************************************************************************************************/

/*********************************************************************************************************************
 * LOCAL/UTILITY ROUTINES
 ********************************************************************************************************************/

/*********************************************************************************************************************
 * API IMPLEMENTATION
 ********************************************************************************************************************/

/**
  * Initialization of global register
  */
XMC_LEDTS_STATUS_t XMC_LEDTS_InitGlobal(XMC_LEDTS_t *const ledts, const XMC_LEDTS_GLOBAL_CONFIG_t *config)
{
  XMC_ASSERT("XMC_LEDTS_InitGlobal:Wrong Module Pointer", XMC_LEDTS_CHECK_KERNEL_PTR(ledts));
  XMC_ASSERT("XMC_LEDTS_InitGlobal:Null Pointer", (config != (XMC_LEDTS_GLOBAL_CONFIG_t *)NULL));

  if (ledts == XMC_LEDTS0)
  {
#if defined(CLOCK_GATING_SUPPORTED)
    XMC_SCU_CLOCK_UngatePeripheralClock(XMC_SCU_PERIPHERAL_CLOCK_LEDTS0);
#endif
#if defined(PERIPHERAL_RESET_SUPPORTED)
    XMC_SCU_RESET_DeassertPeripheralReset(XMC_SCU_PERIPHERAL_RESET_LEDTS0);
#endif
  }
#if defined(LEDTS1)
  else if (ledts == XMC_LEDTS1)
  {
#if defined(CLOCK_GATING_SUPPORTED)
    XMC_SCU_CLOCK_UngatePeripheralClock(XMC_SCU_PERIPHERAL_CLOCK_LEDTS1);
#endif
#if defined(PERIPHERAL_RESET_SUPPORTED)
    XMC_SCU_RESET_DeassertPeripheralReset(XMC_SCU_PERIPHERAL_RESET_LEDTS1);
#endif
  }
#endif
#if defined(LEDTS2)
  else if (ledts == XMC_LEDTS2)
  {
#if defined(CLOCK_GATING_SUPPORTED)
    XMC_SCU_CLOCK_UngatePeripheralClock(XMC_SCU_PERIPHERAL_CLOCK_LEDTS2);
#endif
#if defined(PERIPHERAL_RESET_SUPPORTED)
    XMC_SCU_RESET_DeassertPeripheralReset(XMC_SCU_PERIPHERAL_RESET_LEDTS2);
#endif
  }
#endif
  else
  {
    XMC_ASSERT("XMC_LEDTS_InitGlobal:Invalid Module Pointer", 0);
  }

  if ((ledts->GLOBCTL & LEDTS_GLOBCTL_CLK_PS_Msk) != XMC_LEDTS_CLOCK_NOT_RUNNING)
  {
    return XMC_LEDTS_STATUS_RUNNING;
  }

  ledts->GLOBCTL = config->globctl;

  return XMC_LEDTS_STATUS_SUCCESS;
}

/**
  * Initialization of registers for LED-driving function
  */
XMC_LEDTS_STATUS_t XMC_LEDTS_InitLED(XMC_LEDTS_t *const ledts, const XMC_LEDTS_LED_CONFIG_t *config)
{
  XMC_ASSERT("XMC_LEDTS_LED_Init:Wrong Module Pointer", XMC_LEDTS_CHECK_KERNEL_PTR(ledts));
  XMC_ASSERT("XMC_LEDTS_LED_Init:Null Pointer", (config != (XMC_LEDTS_LED_CONFIG_t *)NULL));

  if ((ledts->GLOBCTL & LEDTS_GLOBCTL_CLK_PS_Msk) != XMC_LEDTS_CLOCK_NOT_RUNNING)
  {
    return XMC_LEDTS_STATUS_RUNNING;
  }

  ledts->FNCTL &= ~(LEDTS_FNCTL_COLLEV_Msk | LEDTS_FNCTL_NR_LEDCOL_Msk);
  ledts->FNCTL |= (config->fnctl);

  /* Enable LED function */
  ledts->GLOBCTL |= LEDTS_GLOBCTL_LD_EN_Msk;

  return XMC_LEDTS_STATUS_SUCCESS;
}

/**
  * Initialization of registers for basic Touch-Sense control function
  */
XMC_LEDTS_STATUS_t XMC_LEDTS_InitTSBasic(XMC_LEDTS_t *const ledts, const XMC_LEDTS_TS_CONFIG_BASIC_t *config)
{
  uint32_t reg;

  XMC_ASSERT("XMC_LEDTS_TS_Basic_Init:Wrong Module Pointer", XMC_LEDTS_CHECK_KERNEL_PTR(ledts));
  XMC_ASSERT("XMC_LEDTS_TS_Basic_Init:Null Pointer", (config != (XMC_LEDTS_TS_CONFIG_BASIC_t *)NULL));

  if ((ledts->GLOBCTL & LEDTS_GLOBCTL_CLK_PS_Msk) != XMC_LEDTS_CLOCK_NOT_RUNNING)
  {
    return XMC_LEDTS_STATUS_RUNNING;
  }

  reg = ~(LEDTS_FNCTL_ACCCNT_Msk | LEDTS_FNCTL_TSCCMP_Msk | LEDTS_FNCTL_TSCTRR_Msk | LEDTS_FNCTL_TSCTRSAT_Msk |
          LEDTS_FNCTL_NR_TSIN_Msk);
  ledts->FNCTL &= (reg);
  ledts->FNCTL |= (config->fnctl);

  /* Enable TS function */
  ledts->GLOBCTL |= LEDTS_GLOBCTL_TS_EN_Msk;

  return XMC_LEDTS_STATUS_SUCCESS;
}

/**
  * Initialization of registers for advanced Touch-Sense control function
  */
XMC_LEDTS_STATUS_t XMC_LEDTS_InitTSAdvanced (XMC_LEDTS_t *const ledts, const XMC_LEDTS_TS_CONFIG_ADVANCED_t *config)
{
  uint32_t reg;

  XMC_ASSERT("XMC_LEDTS_TS_Advanced_Init:Wrong Module Pointer", XMC_LEDTS_CHECK_KERNEL_PTR(ledts));
  XMC_ASSERT("XMC_LEDTS_TS_Advanced_Init:Null Pointer", (config != (XMC_LEDTS_TS_CONFIG_ADVANCED_t *)NULL));

  if ((ledts->GLOBCTL & LEDTS_GLOBCTL_CLK_PS_Msk) != XMC_LEDTS_CLOCK_NOT_RUNNING)
  {
    return XMC_LEDTS_STATUS_RUNNING;
  }

  reg = ~(LEDTS_GLOBCTL_MASKVAL_Msk | LEDTS_GLOBCTL_FENVAL_Msk);
  ledts->GLOBCTL &= (reg);
  ledts->GLOBCTL |= (config->globctl);

  reg = ~(LEDTS_FNCTL_PADT_Msk | LEDTS_FNCTL_PADTSW_Msk | LEDTS_FNCTL_EPULL_Msk | LEDTS_FNCTL_TSOEXT_Msk);
  ledts->FNCTL &= (reg);
  ledts->FNCTL |= (config->fnctl);

  return XMC_LEDTS_STATUS_SUCCESS;
}

/**
  * Starts LEDTS-counter
  */
void XMC_LEDTS_StartCounter(XMC_LEDTS_t *const ledts, const uint16_t prescaler)
{
  XMC_ASSERT("XMC_LEDTS_Start_Counter:Wrong Module Pointer", XMC_LEDTS_CHECK_KERNEL_PTR(ledts));

  ledts->GLOBCTL |= prescaler << 16U;
}

/**
  * Stops LEDTS-counter
  */
void XMC_LEDTS_StopCounter(XMC_LEDTS_t *const ledts)
{
  XMC_ASSERT("XMC_LEDTS_Stop_Counter:Wrong Module Pointer", XMC_LEDTS_CHECK_KERNEL_PTR(ledts));

  ledts->GLOBCTL &= 0x0000FFFF;
}

/**
  * Reads time interrupt flags
  */
uint32_t XMC_LEDTS_ReadInterruptFlag(XMC_LEDTS_t *const ledts)
{
  XMC_ASSERT("XMC_LEDTS_ReadInterruptFlag:Wrong Module Pointer", XMC_LEDTS_CHECK_KERNEL_PTR(ledts));

  return (ledts->EVFR & 0xF);
}

/**
  * Set the active pad number
  */
void XMC_LEDTS_SetActivePADNo(XMC_LEDTS_t *const ledts, XMC_LEDTS_NUMBER_TS_INPUT_t pad_num)
{
  uint32_t reg;

  XMC_ASSERT("XMC_LEDTS_SetActivePADNo:Wrong Module Pointer", XMC_LEDTS_CHECK_KERNEL_PTR(ledts));

  reg = ledts->FNCTL;
  reg &= ~(LEDTS_FNCTL_PADT_Msk);
  reg |= (uint32_t)pad_num;
  ledts->FNCTL = reg;
}

/**
  * Clears interrupt indication flags
  */
void XMC_LEDTS_ClearInterruptFlag(XMC_LEDTS_t *const ledts, uint32_t interrupt_mask)
{
  XMC_ASSERT("XMC_LEDTS_ClearInterruptFlag:Wrong Module Pointer", XMC_LEDTS_CHECK_KERNEL_PTR(ledts));

  ledts->EVFR = (interrupt_mask << LEDTS_EVFR_CTSF_Pos);
}

/**
  * Programming of registers to output pattern on an LED column in LED matrix
  */
void XMC_LEDTS_SetLEDLinePattern(XMC_LEDTS_t *const ledts, XMC_LEDTS_LED_COLUMN_t column, const uint8_t pattern)
{
  uint32_t reg;
  uint8_t reg_index = ((uint8_t)column) >> 2;
  uint8_t bit_shift_count = ((uint8_t)column & 0x03) * 8;

  XMC_ASSERT("XMC_LEDTS_Set_LED_Line_Pattern:Wrong Module Pointer", XMC_LEDTS_CHECK_KERNEL_PTR(ledts));

  reg = ledts->LINE[reg_index];
  reg &= (~(0xff << bit_shift_count));
  reg |= pattern << bit_shift_count;
  ledts->LINE[reg_index] = reg;

}

/**
  * Programming of registers to adjust brightness of an LED column in LED matrix
  */
void XMC_LEDTS_SetColumnBrightness(XMC_LEDTS_t *const ledts, XMC_LEDTS_LED_COLUMN_t column, const uint8_t brightness)
{
  uint32_t reg;
  uint8_t reg_index = ((uint8_t)column) >> 2;
  uint8_t bit_shift_count = ((uint8_t)column & 0x03) * 8;

  XMC_ASSERT("XMC_LEDTS_Set_Column_Brightness:Wrong Module Pointer", XMC_LEDTS_CHECK_KERNEL_PTR(ledts));

  reg = ledts->LDCMP[reg_index];
  reg &= (~(0xff << bit_shift_count));
  reg |= brightness << bit_shift_count;
  ledts->LDCMP[reg_index] = reg;
}

/**
  * Programming of registers to set common oscillation window size for touch-sense inputs
  */
void XMC_LEDTS_SetCommonOscillationWindow(XMC_LEDTS_t *const ledts, const uint8_t common_size)
{
  uint32_t reg;

  XMC_ASSERT("XMC_LEDTS_Set_Common_Oscillation_Window:Wrong Module Pointer", XMC_LEDTS_CHECK_KERNEL_PTR(ledts));

  reg = ledts->LDCMP[1];
  reg &= ~LEDTS_LDCMP1_CMP_LDA_TSCOM_Msk;
  reg |= (common_size << LEDTS_LDCMP1_CMP_LDA_TSCOM_Pos);
  ledts->LDCMP[1] = reg;
}

/**
  * Checking the previous active function or LED column status
  */
uint32_t XMC_LEDTS_ReadFNCOL(XMC_LEDTS_t *const ledts)
{
  uint32_t fncol_read;

  XMC_ASSERT("XMC_LEDTS_Read_FNCOL:Wrong Module Pointer", XMC_LEDTS_CHECK_KERNEL_PTR(ledts));

  fncol_read = ledts->FNCTL & LEDTS_FNCTL_FNCOL_Msk;
  fncol_read >>= LEDTS_FNCTL_FNCOL_Pos;

  return fncol_read;
}

/**
  * Set the number of LED column Enabled
  */
void XMC_LEDTS_SetNumOfLEDColumns(XMC_LEDTS_t *const ledts, uint8_t count)
{

  XMC_ASSERT("XMC_LEDTS_SetNumOfLEDColumns:Wrong Module Pointer", XMC_LEDTS_CHECK_KERNEL_PTR(ledts));

  ledts->FNCTL &= ~(LEDTS_FNCTL_NR_LEDCOL_Msk);
  ledts->FNCTL |= (count << LEDTS_FNCTL_NR_LEDCOL_Pos);
}

/**
  * Reading recorded number of oscillation counts
  */
uint16_t XMC_LEDTS_ReadTSVAL(XMC_LEDTS_t *const ledts)
{
  uint16_t no_of_oscillations;

  XMC_ASSERT("XMC_LEDTS_Read_TSVAL:Wrong Module Pointer", XMC_LEDTS_CHECK_KERNEL_PTR(ledts));

  no_of_oscillations = (ledts->TSVAL & 0xFFFF);

  return no_of_oscillations;
}

/**
  * Programming of registers to adjust the size of oscillation window
  */
void XMC_LEDTS_SetOscillationWindow(XMC_LEDTS_t *const ledts, XMC_LEDTS_NUMBER_TS_INPUT_t touchpad, const uint8_t size)
{
  uint32_t reg;
  uint8_t reg_index = ((uint8_t)touchpad) >> 2;
  uint8_t bit_shift_count = ((uint8_t)touchpad & 0x03) * 8;

  XMC_ASSERT("XMC_LEDTS_Set_Oscillation_Window:Wrong Module Pointer", XMC_LEDTS_CHECK_KERNEL_PTR(ledts));

  reg = ledts->TSCMP[reg_index];
  reg &= (~(0xff << bit_shift_count));
  reg |= size << bit_shift_count;
  ledts->TSCMP[reg_index] = reg;
}

#endif /* LEDTS0 */
