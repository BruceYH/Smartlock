/**
******************************************************************************
  * @file       global.h
  * @brief      ȫ�����ͱ�������
  * @version    1.0 
  * @date       Aug-14-2019 Wed
******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __GLOBAL_H
#define __GLOBAL_H

#ifdef __cplusplus
 extern "C" {
#endif
/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"


/* Exported types ------------------------------------------------------------*/
typedef struct SmartLockStu_
{
	u8 AdminPassword[7];
	u8 UserPassword[7];
	u8 MuteMode;
	u8 LockFlag;
	u8 IDCardHash[16];
}SmartLockStu;

typedef struct TimeStu_
{
	u32 Year;
	u8 Month;
	u8 Day;
	u8 Date;
	u8 Hour;
	u8 Minute;
	u8 Second;
}TimeStu;

/* Extern variables ----------------------------------------------------------*/
extern u8 ReversalFlag;	 

extern u8 CurrentWindowMode;

extern u8 CurrentUserMode;

extern SmartLockStu SmartLock;

extern u32 UnBusy_Count;

extern u32 TimeDisplay;

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

#ifdef __cplusplus
}
#endif

#endif