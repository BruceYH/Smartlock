/**
******************************************************************************
  * @file       main.h
  * @brief      ������ͷ�ļ�
  * @note       ��global.h ������
  * @version    1.1
  * @date       Tue 06-08-2019
******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
 extern "C" {
#endif
/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include <stdio.h>
#include <string.h>
#include "systick.h"
	 
#include "led.h"
#include "time.h"
#include "uart.h"
#include "voice.h"
#include "iic.h"
#include "mpr121.h"
#include "key.h"
	 
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

#ifdef __cplusplus
}
#endif

#endif