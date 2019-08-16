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

u8 FingerPack[8] 	= {0};
u8 FingerPackCount = 0;
u8 FingerPackOver = 0;

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
static void JTAG_Disable(void);
static void NVIC_Config(void);
static void RCC_Config(void);

static void Task_AdminCkeck(void);
static void Task_FingerIdentify(void);
static void Task_RFIDIdentify(void);
// static void Task_DoorMange(void);

void OLED_DrawX(void);
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
	
	NVIC_Config();
	
	RCC_Config();
	
	RTC_Init();
	
	/* IIC �� OLED ʹ��JTAG������Ҫ��ӳ�� */
	JTAG_Disable();
	
	/* ��ʼ��LED */
	LED_Init();
	
	/* ��ʼ������2(��������) */
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
	
	/* ��ʼ�����ݰ���ģ�飬�Լ���ʼ��MPR121 IRQ����Ϊ���븡��̬ */
	Key_Init();
	MPR_IRQ_Init();
	
	/* ����Ա�ж� */
	Task_AdminCkeck();
	
	/* ָ��ģ���ʼ�� */
	Fingerprint_Init();
	
	/* �������ģ���ʼ�� */
	Motor_Init();
	
	/* ��Ƶʶ��ģ��ʹ�� */
	RFID_Init();// SPI2 ��Ҫ��ǰ��ʼ��
	
	/* ���� ����MPR121 IRQ ���� �������ж�3 ��ʼ��*/
	EXTI3_Init();
	
	/* ��ʱ��3��ʼ����ʹ���������ж� */
	TIM3_Interrupt_Init();

	/* ����ȫ���ж� */
	__set_PRIMASK(0); 
	
	/* �ȴ�ϵͳ�ȶ� */
	Delay(1000);
	
	while(1)
	{
		Task_FingerIdentify();
		Task_RFIDIdentify();
		Task_WindowMain();
	}
	
	/* No Retval */
}

void RTC_DispClock(void)
{
	static TimeStu ZeroTime = {2019,8,4,8,0,0,0};
	
	TimeStu Time = {0};
	
	if(TimeDisplay == 1)
	{
		Read_RTCTime(&Time);
		
		if(Time.Year != ZeroTime.Year)
		{
			Set_RTCTime(&ZeroTime);
		}
		else
		{
		}
		
		GUI_DisplayNumber(0,0,Time.Year,2,2);
		GUI_DisplayNumber(0,16,Time.Month,2,2);
		GUI_DisplayNumber(0,32,Time.Date,2,2);
		GUI_DisplayNumber(0,64,Time.Hour,2,2);
		GUI_DisplayNumber(0,88,Time.Minute,2,2);
		GUI_DisplayNumber(0,112,Time.Second,2,2);
		
		TimeDisplay = 0;
	}
	else
	{
		
	}
	
}

static void JTAG_Disable(void)
{
	/* ��ӳ�� */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);// ��λ��ſ��ٴ���ӳ��
	
	return ;
}

static void NVIC_Config(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;

//	/* Configure one bit for preemption priority */
//	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

	/* Enable the RTC Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	return ;
}

void RCC_Config(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);	 
	PWR_BackupAccessCmd(ENABLE);
	
	return ;
}

static void Task_AdminCkeck(void)
{
	if(Admin_Check() == 1)
	{
		CurrentWindowMode = WindowMode_Admin;
	}
	else
	{
		CurrentWindowMode = WindowMode_User;
	}
}

static void Task_FingerIdentify(void)
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
}

static void Task_RFIDIdentify(void)
{
	GUI_RFID_CompareCard();
}

//static void Task_DoorMange(void)
//{
//	/* ��״̬���� */
//}

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
