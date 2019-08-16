/**
******************************************************************************
  * @file       gui_rfid.h
  * @brief      RFIDͼ�ν���ͷ�ļ�
  * @version    1.0
  * @date       Aug-15-2019 Thu
******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __GUI_RFID_H
#define __GUI_RFID_H

#ifdef __cplusplus
 extern "C" {
#endif
/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "stdio.h"
	 
#include "user_misc.h"

#include "gui_baseelmt.h"
#include "rfid.h"
#include "voice.h"
#include "time.h"
#include "motor.h"
	 
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
extern u8 PasswordA[6];// Ĭ�������ݲ��޸�
	 
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void GUI_RFID_AuthorizeNewCard(void);
void GUI_RFID_CompareCard(void);
	 
#ifdef __cplusplus
}
#endif

#endif