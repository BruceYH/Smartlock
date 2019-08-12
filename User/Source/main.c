/**
******************************************************************************
  * @file       main.c
  * @brief      ������Դ�ļ�
  * @version    0.6
  * @date       Aug-10-2019 Sat
  * @update     ���OLED Drive
******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/** @addtogroup 
  * @{
  */

/* Global varaiables ---------------------------------------------------------*/
u16 gTouchStatus = 0; // ��¼ÿ����MPR121��ȡ���İ���״̬
u8 CurrentWindowMode = WindowMode_AllClear;

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
extern u8 Password[7]; // ��ʼ����
extern u8 StringBuff[BUFF_LENGTH];

/* Private function prototypes -----------------------------------------------*/
static void JTAG_Disable(void);

extern void OLED_ShowTextBox(u8 CowNumber,u8 CowHeight,u8 *Str);


/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main function
  * @param  NULL
  * @return NULL
  */
int main(void)
{
	/* ��ֹ�ⲿ�ж� */
	NVIC_DisableIRQ(EXTI3_IRQn);
	
	/* IIC �� OLED ʹ��JTAG������Ҫ��ӳ�� */
	JTAG_Disable();
	
	/* ��ʼ��LED */
	LED_Init();
	
	/* ��ʼ�����ڣ����ڵ��� */
	USART1_Init(115200);
	USART_Cmd(USART1,ENABLE);
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
	
	/* ����ģ�� IRQ Pin Init */
	EXTI3_Init();
	
	/* �ȴ�ϵͳ�ȶ� */
	
	while(1)
	{
		Window_MainTask(&CurrentWindowMode);
		Delay(1000);
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


/* Exported functions --------------------------------------------------------*/

void Password_Input(void)
{
	u32 i;
	u8 KeyValue;
	
	for(i = 0; i < (BUFF_LENGTH - 1) ; )
	{
		LED3_OFF();
		
		KeyValue = Key_Scan();
			
		if(KeyValue != 0)/* ����ģ���⵽�����仯 */
		{
#ifdef DEBUG
			printf("KeyValue = %c\r\n",KeyValue);
#endif
			
			Voice_Play(VoiceCmd_Di);
			
			if(KeyValue == '#')
			{
				break;
			}
			else if(KeyValue == '*')
			{
				if(i == 0)
				{
					// Don't do anything
				}
				else
				{
					i -= 1;
				}
				StringBuff[i] = 0; // ����i�����һλ
			}
			else
			{
				StringBuff[i] = KeyValue;// ��ǰ���뱻��¼����������Ӧλ��
				i++; // i ǰ��һλ��Ϊ�´�����׼��
			}
			
			
		}
		else
		{
		}
		
		OLED_ShowTextBox(4,2,StringBuff);
		OLED_ShowTextBox(6,2,StringBuff + 8);
	}
	StringBuff[i] = 0;
	
	return ;
}

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
