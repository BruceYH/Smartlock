/**
******************************************************************************
  * @file       gui_finger.c
  * @brief      ָ��ͼ�ν���
  * @version    1.0
  * @date       Aug-14-2019 Wed
******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "gui_finger.h"

/** @addtogroup GUI Fingerprint
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

void GUI_Finger_EnrollNewUser(void)
{
	u8 IDTemp;
	
	GUI_ClearScreen();
	
	GUI_DisplayString(0,24,&FingerMangeString1_16x16[0][0],5);
	
	/* �ȴ���ָ���� */
	while(MG200_DETECT_Status() != SET)
	{
	}

	IDTemp = 0x00;// ʹ���Զ�����
	if(Finger_EnrollNewUser(IDTemp) == 0)
	{
		GUI_DisplayString(4,32+0,&EnrollString_16x16[0][0],2);
		GUI_DisplayString(4,32+32,&SuccessString_16x16[0][0],2);
		printf("Add new user success\r\n");
	}
	else
	{
		GUI_DisplayString(4,32+0,&EnrollString_16x16[0][0],2);
		GUI_DisplayString(4,32+32,&FailString_16x16[0][0],2);
		printf("Could new a user\r\n");
	}
	
	Delay(1000);
	
	GUI_ClearScreen();
	
	return ;
}

void GUI_Finger_EraseAllUser(void)
{
	u8 Temp;
	
	GUI_ClearScreen();
	GUI_DisplayString(0,32+0,&DeleteString_16x16[0][0],2);
	GUI_DisplayString(0,32+32,&MenuString3_16x16[4][0],2);
	
	
	if(Finger_EraseAllUser(&Temp) == 0)
	{
		GUI_DisplayString(4,32+0,&DeleteString_16x16[0][0],2);
		GUI_DisplayString(4,24+32,&SuccessString_16x16[0][0],2);
	}
	else
	{
		GUI_DisplayString(4,32+0,&DeleteString_16x16[0][0],2);
		GUI_DisplayString(4,32+32,&FailString_16x16[0][0],2);
	}

	Delay(1000);
	
	GUI_ClearScreen();
}

void GUI_Finger_Compare(void)
{
	/* ʵʱ�Բ��ߣ��Ῠס���� */
	
	u8 Finger_ID;
	
	Voice_Play(VoiceCmd_Di);
	
	GUI_ClearScreen();
	GUI_DisplayPicture(32,0,64,64,&Fingerprint_64px[0][0]);
	
	if(Finger_Compare(&Finger_ID) == 0)
	{
		Voice_Play(VoiceCmd_DOOROPEN_SUCCESS);
	}
	else
	{
		GUI_DisplayString(2,48,&IdentifyString_16x16[0][0],2);
		GUI_DisplayString(4,48,&FailString_16x16[0][0],2);
		
		Delay(1000);
	}
	
	return ;
}


/**
  * @}
  */
