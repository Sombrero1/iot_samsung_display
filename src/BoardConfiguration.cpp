/******************************************************************************
 *
 * @brief     This file is part of the TouchGFX 4.8.0 evaluation distribution.
 *
 * @author    Draupner Graphics A/S <http://www.touchgfx.com>
 *
 ******************************************************************************
 *
 * @section Copyright
 *
 * Copyright (C) 2014-2016 Draupner Graphics A/S <http://www.touchgfx.com>.
 * All rights reserved.
 *
 * TouchGFX is protected by international copyright laws and the knowledge of
 * this source code may not be used to write a similar product. This file may
 * only be used in accordance with a license and should not be re-
 * distributed in any way without the prior permission of Draupner Graphics.
 *
 * This is licensed software for evaluation use, any use must strictly comply
 * with the evaluation license agreement provided with delivery of the
 * TouchGFX software.
 *
 * The evaluation license agreement can be seen on www.touchgfx.com
 *
 * @section Disclaimer
 *
 * DISCLAIMER OF WARRANTY/LIMITATION OF REMEDIES: Draupner Graphics A/S has
 * no obligation to support this software. Draupner Graphics A/S is providing
 * the software "AS IS", with no express or implied warranties of any kind,
 * including, but not limited to, any implied warranties of merchantability
 * or fitness for any particular purpose or warranties against infringement
 * of any proprietary rights of a third party.
 *
 * Draupner Graphics A/S can not be held liable for any consequential,
 * incidental, or special damages, or any other relief, or for any claim by
 * any third party, arising from your use of this software.
 *
 *****************************************************************************/
#include <STM32F746GTouchController.hpp>
#include <STM32F7DMA.hpp>
#include <STM32F7HAL.hpp>
//#include <STM32F7Instrumentation.hpp>
#include <common/TouchGFXInit.hpp>
#include <platform/driver/lcd/LCD16bpp.hpp>
#include <platform/driver/lcd/LCD24bpp.hpp>
#include <platform/driver/touch/NoTouchController.hpp>
#include <touchgfx/hal/BoardConfiguration.hpp>
#include <touchgfx/hal/GPIO.hpp>
#include <touchgfx/hal/NoDMA.hpp>
#include <touchgfx/hal/OSWrappers.hpp>

/***********************************************************
 ******   Single buffer in internal RAM              *******
 ***********************************************************
 * On this platform, TouchGFX is able to run using a single
 * frame buffer in internal SRAM, thereby avoiding the need
 * for external SDRAM.
 * This feature was introduced in TouchGFX 4.7.0. To enable it,
 * uncomment the define below. The function touchgfx_init()
 * later in this file will check for this define and configure
 * TouchGFX accordingly.
 * For details on the single buffer strategy, please refer to
 * the knowledge base article "Single vs double buffering in TouchGFX"
 * on our support site.
 */
//#define SINGLE_FRAME_BUFFER_INTERNAL

/***********************************************************
 ******         24 Bits Per Pixel Support            *******
 ***********************************************************
 *
 * The default bit depth of the framebuffer is 16bpp. If you want 24bpp support, define the symbol "USE_BPP" with a value
 * of "24", e.g. "USE_BPP=24". This symbol affects the following:
 *
 * 1. Type of TouchGFX LCD (16bpp vs 24bpp)
 * 2. Bit depth of the framebuffer(s)
 * 3. TFT controller configuration.
 *
 * WARNING: Remember to modify your image formats accordingly in app/config/. Please see the following knowledgebase article
 * for further details on how to choose and configure the appropriate image formats for your application:
 *
 * https://touchgfx.zendesk.com/hc/en-us/articles/206725849
 */

extern "C" {
#include "stm32746g_discovery_lcd.h"
#include "stm32746g_discovery_qspi.h"
#include "stm32746g_discovery_sdram.h"
#include "stm32f7xx_hal_dma.h"
#include "stm32f7xx_hal_rcc_ex.h"
#include "stm32f7xx_hal_tim.h"

//void OS_SysTick(void);
//void SystemClock_Config(void);

//bool os_inited = false;
//void SysTick_Handler(void)
//{
//    HAL_IncTick();
//    if (os_inited) {
//        OS_SysTick();
//    }
//}
//#define SINGLE_FRAME_BUFFER_INTERNAL
#define USE_BPP 24

extern LTDC_HandleTypeDef hLtdcHandler;
}

static void BSP_LCD_LayerInit(uint16_t LayerIndex, uint32_t FB_Address);

#ifdef SINGLE_FRAME_BUFFER_INTERNAL
uint16_t frameBuffer[480 * 272];
static uint32_t frameBuf0 = (uint32_t)&frameBuffer[0];
#else
// Use SDRAM for frame buffers
static uint32_t frameBuf0 = (uint32_t)(0xC0000000);
#endif
namespace touchgfx {
void hw_init()
{
    //    HAL_Init();

    //    SystemClock_Config(); //200MHz overdrive, FL6

    //Enable CRC engine for STM32 Lock check
    __HAL_RCC_CRC_CLK_ENABLE();

    /* Initialize the QSPI */
    //    BSP_QSPI_Init();
    //    BSP_QSPI_MemoryMappedMode();
    HAL_NVIC_DisableIRQ(QUADSPI_IRQn);

#ifndef SINGLE_FRAME_BUFFER_INTERNAL
    BSP_SDRAM_Init();
#endif

    BSP_LCD_Init();
    BSP_LCD_LayerInit(0, frameBuf0);
    BSP_LCD_DisplayOn();

    GPIO::init();
    //    SCB_EnableDCache();
    //    SCB_EnableICache();

    /* Configure unused area of QSPI region as strongly ordered.
     * This is *important* to avoid unintentional fetches from illegal
     * addresses due to cache/speculation which would halt the MCU.
     */
    HAL_MPU_Disable();
    MPU_Region_InitTypeDef MPU_InitStruct;
    MPU_InitStruct.Enable = MPU_REGION_ENABLE;
    MPU_InitStruct.BaseAddress = 0x90000000;
    MPU_InitStruct.Size = MPU_REGION_SIZE_256MB;
    MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
    MPU_InitStruct.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE;
    MPU_InitStruct.IsCacheable = MPU_ACCESS_NOT_CACHEABLE;
    MPU_InitStruct.IsShareable = MPU_ACCESS_NOT_SHAREABLE;
    MPU_InitStruct.Number = MPU_REGION_NUMBER2;
    MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL0;
    MPU_InitStruct.SubRegionDisable = 0x00;
    MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_ENABLE;

    HAL_MPU_ConfigRegion(&MPU_InitStruct);

    /* Configure the MPU attributes as WT for QSPI (used 16Mbytes) */
    MPU_InitStruct.Enable = MPU_REGION_ENABLE;
    MPU_InitStruct.BaseAddress = 0x90000000;
    MPU_InitStruct.Size = MPU_REGION_SIZE_16MB; /* NOTE! Change this if you change QSPI flash size! */
    MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
    MPU_InitStruct.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE;
    MPU_InitStruct.IsCacheable = MPU_ACCESS_CACHEABLE;
    MPU_InitStruct.IsShareable = MPU_ACCESS_NOT_SHAREABLE;
    MPU_InitStruct.Number = MPU_REGION_NUMBER3;
    MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL0;
    MPU_InitStruct.SubRegionDisable = 0x00;
    MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_ENABLE;

    HAL_MPU_ConfigRegion(&MPU_InitStruct);
    HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);

    //Deactivate speculative/cache access to first FMC Bank to save FMC bandwidth
    FMC_Bank1->BTCR[0] = 0x000030D2;
}

STM32F7DMA dma;
STM32F746GTouchController tc;
//STM32F7Instrumentation mcuInstr;

#if !defined(USE_BPP) || USE_BPP == 16
LCD16bpp display;
#elif USE_BPP == 24
LCD24bpp display;
#else
#error Unknown USE_BPP
#endif

void touchgfx_init()
{
    HAL& hal = touchgfx_generic_init<STM32F7HAL>(dma, display, tc, 480, 272, 0, 0);
    //    os_inited = true;

#if !defined(USE_BPP) || USE_BPP == 16
#ifdef SINGLE_FRAME_BUFFER_INTERNAL
    //setup for single buffering
    //    hal.setFrameBufferStartAddress((uint16_t*)frameBuf0, 16, false, false);
    hal.setFrameBufferStartAddresses((uint16_t*)frameBuf0, 0, 0);
    // The optimized strategy for single buffering requires the presence of a
    // task delay function.
    hal.registerTaskDelayFunction(&OSWrappers::taskDelay);

    // Enable strategy.
    hal.setFrameRefreshStrategy(HAL::REFRESH_STRATEGY_OPTIM_SINGLE_BUFFER_TFT_CTRL);
#else
    //setup for double buffering.
    //    hal.setFrameBufferStartAddress((uint16_t*)frameBuf0);
    hal.setFrameBufferStartAddresses((uint16_t*)frameBuf0, 0, 0);
#endif
#elif USE_BPP == 24
#ifdef SINGLE_FRAME_BUFFER_INTERNAL
#error Single frame buffer in internal is only possible in 16bpp due to memory constraints.
#endif
    hal.setFrameBufferStartAddress((uint16_t*)frameBuf0, 24);
//    hal.setFrameBufferStartAddresses((uint16_t*)frameBuf0, NULL, NULL);
#else
#error Unknown USE_BPP
#endif

    hal.setTouchSampleRate(2);
    hal.setFingerSize(1);

    // By default frame rate compensation is off.
    // Enable frame rate compensation to smooth out animations in case there is periodic slow frame rates.
    hal.setFrameRateCompensation(false);

    // This platform can handle simultaneous DMA and TFT accesses to SDRAM, so disable lock to increase performance.
    hal.lockDMAToFrontPorch(false);

    //    mcuInstr.init();

    //Set MCU instrumentation and Load calculation
    //    hal.setMCUInstrumentation(&mcuInstr);
    //    hal.enableMCULoadCalculation(true);
}
}

/**
  * @brief  Initializes the LCD layer in RGB565 format (16 bits per pixel) or RGB888(24 bits per pixel) depending on definitionof USE_BPP
  * @param  LayerIndex: Layer foreground or background
  * @param  FB_Address: Layer frame buffer
  * @retval None
  */
static void BSP_LCD_LayerInit(uint16_t LayerIndex, uint32_t FB_Address)
{
    LCD_LayerCfgTypeDef layer_cfg;

    /* Layer Init */
    layer_cfg.WindowX0 = 0;
    layer_cfg.WindowX1 = BSP_LCD_GetXSize();
    layer_cfg.WindowY0 = 0;
    layer_cfg.WindowY1 = BSP_LCD_GetYSize();
    layer_cfg.FBStartAdress = FB_Address;
    layer_cfg.Alpha = 255;
    layer_cfg.Alpha0 = 0;
    layer_cfg.Backcolor.Blue = 0;
    layer_cfg.Backcolor.Green = 0;
    layer_cfg.Backcolor.Red = 0;
    layer_cfg.ImageWidth = BSP_LCD_GetXSize();
    layer_cfg.ImageHeight = BSP_LCD_GetYSize();

#if !defined(USE_BPP) || USE_BPP == 16
    layer_cfg.PixelFormat = LTDC_PIXEL_FORMAT_RGB565;
    layer_cfg.BlendingFactor1 = LTDC_BLENDING_FACTOR1_CA;
    layer_cfg.BlendingFactor2 = LTDC_BLENDING_FACTOR2_CA;
#elif USE_BPP == 24
    layer_cfg.PixelFormat = LTDC_PIXEL_FORMAT_RGB888;
    layer_cfg.BlendingFactor1 = LTDC_BLENDING_FACTOR1_PAxCA;
    layer_cfg.BlendingFactor2 = LTDC_BLENDING_FACTOR2_PAxCA;
#else
#error Unknown USE_BPP
#endif

    HAL_LTDC_ConfigLayer(&hLtdcHandler, &layer_cfg, LayerIndex);
}
