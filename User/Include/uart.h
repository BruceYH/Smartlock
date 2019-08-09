/**
******************************************************************************
  * @file       uart.h
  * @brief      ���ڲ�����غ���ͷ�ļ�
  * @version    1.0
  * @date       Tue 06-08-2019
******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __UART_H
#define __UART_H

#ifdef __cplusplus
 extern "C" {
#endif
/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

void USART1_Init(u32 BaudRate);

#ifdef __cplusplus
}
#endif

#endif