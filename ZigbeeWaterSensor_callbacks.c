/***************************************************************************//**
 * @file
 * @brief Callback implementation for ZigbeeMinimal sample application.
 *******************************************************************************
 * # License
 * <b>Copyright 2019 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * The licensor of this software is Silicon Laboratories Inc. Your use of this
 * software is governed by the terms of Silicon Labs Master Software License
 * Agreement (MSLA) available at
 * www.silabs.com/about-us/legal/master-software-license-agreement. This
 * software is distributed to you in Source Code format and is governed by the
 * sections of the MSLA applicable to Source Code.
 *
 ******************************************************************************/

// This callback file is created for your convenience. You may add application
// code to this file. If you regenerate this file over a previous version, the
// previous version will be overwritten and any code you have added will be
// lost.

#include "app/framework/include/af.h"
#include "gpiointerrupt.h"
#include "gpio-button.h"

#define GPIO_INTERRUPT_PORT gpioPortB                                           //水浸传感
#define GPIO_INTERRUPT_PIN  (1U)



/** @brief Stack Status
 *
 * This function is called by the application framework from the stack status
 * handler.  This callbacks provides applications an opportunity to be notified
 * of changes to the stack status and take appropriate action.  The return code
 * from this callback is ignored by the framework.  The framework will always
 * process the stack status after the callback returns.
 *
 * @param status   Ver.: always
 */
bool emberAfStackStatusCallback(EmberStatus status)
{
  // This value is ignored by the framework.
  return false;
}

/** @brief Complete
 *
 * This callback is fired when the Network Steering plugin is complete.
 *
 * @param status On success this will be set to EMBER_SUCCESS to indicate a
 * network was joined successfully. On failure this will be the status code of
 * the last join or scan attempt. Ver.: always
 * @param totalBeacons The total number of 802.15.4 beacons that were heard,
 * including beacons from different devices with the same PAN ID. Ver.: always
 * @param joinAttempts The number of join attempts that were made to get onto
 * an open Zigbee network. Ver.: always
 * @param finalState The finishing state of the network steering process. From
 * this, one is able to tell on which channel mask and with which key the
 * process was complete. Ver.: always
 */
void emberAfPluginNetworkSteeringCompleteCallback(EmberStatus status,
                                                  uint8_t totalBeacons,
                                                  uint8_t joinAttempts,
                                                  uint8_t finalState)
{
  emberAfCorePrintln("%p network %p: 0x%X", "Join", "complete", status);
}

/** @brief Ok To Sleep
 *
 * This function is called by the Idle/Sleep plugin before sleeping. It is
 * called with interrupts disabled. The application should return true if the
 * device may sleep or false otherwise.
 *
 * @param durationMs The maximum duration in milliseconds that the device will
 * sleep. Ver.: always
 */
bool emberAfPluginIdleSleepOkToSleepCallback(uint32_t durationMs)
{
  /*关闭看门狗，去睡觉*/
  halInternalDisableWatchDog(MICRO_DISABLE_WATCH_DOG_KEY);


  //NVIC_EnableIRQ(USART1_RX_IRQn);
  return true;
}


/** @brief Wake Up
 *
 * This function is called by the Idle/Sleep plugin after sleeping.
 *
 * @param durationMs The duration in milliseconds that the device slept.
 * Ver.: always
 */
void emberAfPluginIdleSleepWakeUpCallback(uint32_t durationMs)
{
  /**/
  /*醒来，开启看门狗*/
  halInternalEnableWatchDog();

  //NVIC_DisableIRQ(USART1_RX_IRQn);

}

//初始化
void emberAfMainInitCallback(void)
{
  halGpioButtonInitialize();
  GPIO_PinModeSet(GPIO_INTERRUPT_PORT, 0,gpioModeInputPull,1); //1:DOUT  1:默认高电平  0：默认低电平

  GPIO_PinModeSet(GPIO_INTERRUPT_PORT, GPIO_INTERRUPT_PIN,gpioModeInputPull,1); //1:DOUT  1:默认高电平  0：默认低电平
  GPIOINT_Init();//在此中断初始化 中断插件才能生效，，
  CMU_ClockEnable(cmuClock_GPIO, true);

}


//水浸信息上报函数
void emberAfPluginGpioSensorStateChangedCallback(uint8_t newSensorState)
{
  emberAfCorePrintln("GpioSensorStateChangedCallback %d",newSensorState);
}

void emberAfCustomGpioButtonStateChangedCallback(uint8_t newSensorState)
{
  emberAfCorePrintln("GpioButtonStateChangedCallback %d",newSensorState);
}
