/**
******************************************************************************
  * @file       main.c
  * @brief      ������Դ�ļ�
  * @version    1.0
  * @date       Aug-10-2019 Sat
  * @update     ���ָ�ƽ�������
******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/** @addtogroup 
  * @{
  */

/* Global varaiables ---------------------------------------------------------*/

u16 gTouchStatus = 0; // ��¼ÿ����MPR121��ȡ���İ���״̬

u32 UnBusy_Count = 0;

u8 FingerPack[8] 	= {0};
u8 FingerPackCount = 0;
u8 FingerPackOver = 0;

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
static void JTAG_Disable(void);

void Fingerprint_NewTask(void);

void RFID_CardSearch(void);
	
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main function
  * @param  NULL
  * @return NULL
  */
int main(void)
{
	/* ����ȫ���ж� */
	__set_PRIMASK(1); 
	
	/* IIC �� OLED ʹ��JTAG������Ҫ��ӳ�� */
	JTAG_Disable();
	
	/* ��ʼ��LED */
	LED_Init();
	
	/* ��ʼ�����ڣ����ڵ��� */
	USART1_Init(115200);
#ifdef DEBUG
		printf("\r\nDebug mode\r\n");
#endif		
	
	/* ��ʼ����������ģ�� */
	VoiceModule_Init();
	
	/* ��ʼ��OLEDģ�鲢������ */
	OLED_Init();
	OLED_Config();
	OLED_Clear();
	
	/* ��ʼ�����ݰ���ģ�� */
	Key_Init();
	MPR_IRQ_Init();
	
	/* ����Ա�ж� */
	if(Admin_Check() == 1)
	{
		CurrentWindowMode = WindowMode_Admin;
	}
	else
	{
		CurrentWindowMode = WindowMode_User;
	}
	
	Fingerprint_Init();
	
	Motor_Init();
	
	// SPI2 ��Ҫ����
	RFID_Init();
	
	/* ����ģ�� IRQ Pin Init */
	EXTI3_Init();
	
	/* ��ʱ��3��ʼ����ʹ���������ж� */
	TIM3_Interrupt_Init();

	/* ����ȫ���ж� */
	__set_PRIMASK(0); 
	
	/* �ȴ�ϵͳ�ȶ� */	
	
	while(1)
	{
		if(MG200_DETECT_Status() == SET)
		{
			CurrentWindowMode = WindowMode_User;
			CurrentUserMode = UserSubMode_Finger;
		}
		else
		{
			CurrentUserMode = UserSubMode_Password;
		}
		Window_MainTask();
	}
	
	/* No Retval */
}

void JTAG_Disable(void)
{
	/* ��ӳ�� */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);// ��λ��ſ��ٴ���ӳ��
	
	return ;
}


// RFID init

void RFID_CardSearch(void)
{
	u8 CardType[2] = {0};
	u8 SerialNum[4] = {0};
	
	/* Ѱ�� */
	if(PcdRequest(PICC_REQALL,CardType) == MI_OK)
	{
		printf("CardType = %d%d\r\n",CardType[0],CardType[1]);
		Voice_Play(VoiceCmd_Di);
		/* ����ײ */
		if(PcdAnticoll(SerialNum) == MI_OK)
		{
			printf("SerialNum = %2x%2x%2x%2x\r\n",SerialNum[0],SerialNum[1],SerialNum[2],SerialNum[3]);
			/* ѡ�� */
			if(PcdSelect(SerialNum) == MI_OK)
			{	
			}
		}
	}
}


void Fingerprint_NewTask(void)
{
	u8 IDTemp,Result;
	
	if(MG200_DETECT_Status() == SET)
	{
		Finger_EraseAllUser(&Result);
		
		Voice_Play(VoiceCmd_Di);
		IDTemp = 0x00;// ʹ���Զ�����
		if(Finger_EnrollNewUser(IDTemp) == 0)
		{
			printf("Add new user success\r\n");
		}
		else
		{
			printf("Could new a user\r\n");
		}
		
		while(MG200_DETECT_Status() != RESET)
		{
		}
		
		while(MG200_DETECT_Status() != SET)
		{
		}
		
		if(Finger_Compare(&IDTemp) != 0)
		{
			printf("No find user\r\n");
		}
		else
		{
			printf("User ID is %d\r\n",IDTemp);
		}
		
		while(MG200_DETECT_Status() != RESET)
		{
		}
		
//		if(Finger_CaptureAndExtract(3) == 0)
//		{
//			Voice_Play(VoiceCmd_Di);
//			
//			
//		}
//		else
//		{
//		}
		
	}
	else
	{
		
	}
}


/* Exported functions --------------------------------------------------------*/

u8 Admin_Check(void)
{
	u32 i = 0;

	Delay(100);
	printf("MPR_IRQ = %d\r\n",MPR_IRQ());
	gTouchStatus = MPR_TouchStatus();
	printf("TouchStatus = %#x\r\n",gTouchStatus);
	
	while(1)
	{
		Delay(100);
		gTouchStatus = MPR_TouchStatus();
		printf("TouchStatus = %#x\r\n",gTouchStatus);
		if(i>25 || (gTouchStatus != 0x80f && gTouchStatus != 0x8))
		{
			break;
		}
		i++;
	}
	
	while(MPR_IRQ() == RESET)
	{
		gTouchStatus = MPR_TouchStatus();
	}
	
	printf("Out the while,i = %d\r\n",i);
	
	if(i < 20)
	{
		return 0;
	}
	else
	{
		return 1;
	}
}


//Redirect fputc function
int fputc(int ch,FILE *stream)
{
	USART_SendData(USART1,ch);
	while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET)
	{
	}
	
	return ch;
}
    
/**
  * @}
  */
