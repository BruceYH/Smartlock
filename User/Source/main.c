/**
******************************************************************************
  * @file       main.c
  * @brief      ������Դ�ļ�
  * @version    1.1
  * @date       Aug-10-2019 Sat
  * @update     ���ָ�ƽ�������
******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/** @addtogroup 
  * @{
  */

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
static void Task_DoorMange(void);

static u8 Admin_Check(void);

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
	
	/* �ж����ȼ����� */
	NVIC_Config();
	
	/* ����ʱ��ʹ�� */
	RCC_Config();
	
	/* ʵʱʱ��ʹ�� */
	RTC_Init();
	
	/* ��ӳ��JTAG */
	JTAG_Disable();// IIC �� OLED ռ����JTAG����
	
	/* ��ʼ��LED */
	LED_Init();
	
#ifdef DEBUG	
	/* ��ʼ������2(��������) */
	USART1_Init(115200);

	printf("\r\nDebug mode\r\n");
#endif		

	/* ��ʼ����������ģ�� */
	VoiceModule_Init();
	
	/* ��ʼ��SP2 */
	SPI2_Init();
	
	/* ��ʼ��OLEDģ�鲢������ */
	OLED_Init();
	OLED_Config();
	OLED_Clear();
	
	/* ��ʼ�����ݰ���ģ�飬�Լ���ʼ��MPR121 IRQ����Ϊ���븡��̬ */
	Key_Init();
	MPR_IRQ_Init();
		
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
	
	/* ����Ա�ж� */
	Task_AdminCkeck();
	
	/* �ȴ�ϵͳ�ȶ� */
	Delay(1000);
	
	while(1)
	{
		Task_FingerIdentify();
		Task_RFIDIdentify();
		Task_WindowMain();
		Task_DoorMange();
	}
	
	/* No Retval */
}

<<<<<<< HEAD
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
		GUI_ClearPart(0,2,0,128);
		
		GUI_DisplayNumber(0,0,Time.Year,2,2);
		GUI_DisplayNumber(0,16,Time.Month,2,2);
		GUI_DisplayNumber(0,32,Time.Date,2,2);
		GUI_DisplayNumber(0,64,Time.Hour,2,2);
		GUI_DisplayNumber(0,88,Time.Minute,2,2);
		GUI_DisplayNumber(0,112,Time.Second,2,2);
		
		TimeDisplay = 0;
		
		Delay(500);
	}
	else
	{
		
	}
	
}

=======
>>>>>>> Rebuild_GUI_Logic
static void JTAG_Disable(void)
{
	/* ��ӳ�� */
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);// ��λ��ſ��ٴ���ӳ��
	
	return ;
}

static void NVIC_Config(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;

	/* Configure two bit for preemption priority */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

	/* Enable the RTC Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	/* Enable and set EXTI3 Interrupt to the highest priority */
	NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	/* Enable the USART2 Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	/* Enable the TIM3 global Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	return ;
}

static void RCC_Config(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);	 
	PWR_BackupAccessCmd(ENABLE);
	
	/* Enable AFIO clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	
	/* GPIOA Periph clock enable */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

	/* GPIOB Periph clock enable */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	/* GPIC Periph clock enable */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	
	/* USART1 Periph clock enable */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	
	/* USART2 Periph clock enable */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	
	/* SPI2 Periph clock enable */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
	
	/* TIM2 clock enable */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	
	/* TIM3 clock enable */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	
	/* TIM4 clock enable */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	
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
	if(MG200_DETECT_Status() == SET && CurrentWindowMode != WindowMode_Setting)
	{
		CurrentWindowMode = WindowMode_User;
		CurrentUserMode = UserSubMode_Finger;
		UnBusy_Count = 0;
	}
	else
	{
		CurrentUserMode = UserSubMode_Password;
	}
}

static void Task_RFIDIdentify(void)
{
	if(CurrentWindowMode != WindowMode_Setting)
	{
		GUI_RFID_CompareCard();
	}
	else
	{
		
	}
	
}

static void Task_DoorMange(void)
{
	/* ��״̬���� */
	static u8 LockFlag = 0x01;
	
	if(LockFlag != SmartLock.LockFlag)
	{
		LockFlag = SmartLock.LockFlag;
		
		if(SmartLock.LockFlag == 0x00)
		{
			Motor_OpenLock();
		}
		else
		{
			Motor_CloseLock();
		}
	}
	else
	{
		
	}
	
}

static u8 Admin_Check(void)
{
	u32 i = 0;

	NVIC_DisableIRQ(EXTI3_IRQn);

	Delay(100);
#ifdef DEBUG
	printf("MPR_IRQ = %d\r\n",MPR_IRQ());
#endif	
	
	gTouchStatus = MPR_TouchStatus();
	
#ifdef DEBUG	
	printf("TouchStatus = %#x\r\n",gTouchStatus);
#endif	
	
	while(1)
	{
		Delay(100);
		gTouchStatus = MPR_TouchStatus();
#ifdef DEBUG
		printf("TouchStatus = %#x\r\n",gTouchStatus);
#endif	
		if(i>20 || (gTouchStatus != 0x80f && gTouchStatus != 0x8))
		{
			break;
		}
		i++;
	}
	
	// �ȴ�MPR121�����Żָ��ߵ�ƽ�Է�ֹ���жϰ�����ȡ�Ĺ����ͻ
	do
	{
		gTouchStatus = MPR_TouchStatus();
#ifdef DEBUG
		printf("TouchStatus = %#x\r\n",gTouchStatus);
#endif	
	}while(MPR_IRQ() != SET);
	
	if(gTouchStatus != 0x80f)
	{
		gTouchStatus = MPR_TouchStatus();
	}
	else
	{
	}
	
	NVIC_EnableIRQ(EXTI3_IRQn);

#ifdef DEBUG	
	printf("TouchStatus = %#X\r\n",gTouchStatus);
#endif	

#ifdef DEBUG
	printf("Out the while,i = %d\r\n",i);
#endif	
	
	if(i < 15)
	{
		return 0;
	}
	else
	{
		return 1;
	}
}

/* Exported functions --------------------------------------------------------*/

#ifdef DEBUG
//Redirect fputc function
int fputc(int ch,FILE *stream)
{
	USART_SendData(USART1,ch);
	while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET)
	{
	}
	
	return ch;
}

#endif
    
/**
  * @}
  */
