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
u16 gTouchStatus = 0;

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define BUFF_LENGTH 256
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
u8 Password[] = "201988";
u8 StringBuff[BUFF_LENGTH] = {0};

/* Private function prototypes -----------------------------------------------*/
static void JTAG_Disable(void);
static void Password_Input(void);
static u8 Password_Check(void);

//static s8 String_Compare(const u8 *String1,const u8 *String2);
s32 String_ViolentMatch(const u8 *TargetString,const u8 *MatchingString);


/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main function
  * @param  NULL
  * @return NULL
  */
int main(void)
{
	/* IIC �� OLED ʹ��JTAG������Ҫ��ӳ�� */
	JTAG_Disable();
	
	LED_Init();
	
	LED2_OFF();
	LED3_OFF();
	
	USART1_Init(115200);
	USART_Cmd(USART1,ENABLE);
	
	VoiceModule_Init();
	//delay_ms(500);//�ȴ��ȶ�
	
	Key_Init();
	
	delay_ms(500);//�ȴ��ȶ�
	
#ifdef DEBUG
		printf("\r\nDebug mode\r\n");
#endif	

	OLED_Init();
	OLED_Config();
	OLED_Clear();
	
	Voice_Play( VoiceCmd_INPUT_ADMIN_PASSWORD);
	
	while(1)
	{
		
		for(u32 i = 0;i < 4;i++)
		{
			for(u32 j = 0;j < 16;j+=2)
			{
				OLED_FullWidthCharacter(j,i);
				Delay(500);
			}
		}
		OLED_Clear();
		
		Password_Input();
		
#ifdef DEBUG
		printf("StringBuff = \"%s\"\r\n",StringBuff);
#endif		
		
		if(Password_Check() == 1)
		{
			printf("Password right\r\n");
//			Voice_Play(VoiceCmd_DOOROPEN_SUCCESS);
		}
		else
		{
			printf("Password wrong\r\n");
//			Voice_Play(VoiceCmd_PASSWORD_INCONFORMITY);
		}
		OLED_Clear();

	}
}

void JTAG_Disable(void)
{
	/* ��ӳ�� */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);// ��λ��ſ��ٴ���ӳ��
	return ;
}


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
				i = 0;
			}
			else
			{
				StringBuff[i] = KeyValue;
				i++;
			}
		}
		else
		{
		}
	}
	StringBuff[i] = 0;
	
	return ;
}

u8 Password_Check(void)
{
//	if(String_Compare(StringBuff,Password) == 0)
//	{
//		return 1;
//	}
//	
//	return 0;
	
	s32 Place;
	Place = String_ViolentMatch(StringBuff,Password);
	
#ifdef DEBUG
	printf("Place = %d\r\n",Place);
#endif
	
	if(Place == -1)
	{
		return 0;
	}
	else
	{
		return 1;
	}
	
}

s8 String_Compare(const u8 *String1,const u8 *String2)
{
	while(*String1 != '\0'){
        if(*String1 < *String2){
            return -1;
        }
        else if(*String1 > *String2){
            return 1;
        }
        else{}
        String1++;
        String2++;
    }
    if(*String2 != '\0'){
        return -1;
    }
    else{
        return 0;
    }
}

s32 String_ViolentMatch(const u8 *TargetString,const u8 *MatchingString)
{
	u32 T_Length = strlen((const char *)TargetString);
	u32 M_Length = strlen((const char *)MatchingString);
	
	u32 i,j;
	
	if((T_Length < M_Length) ||  (T_Length > 0xFFFFFFF) || (M_Length > 0xFFFFFFF))
	{
		return -1;// Ŀ���ַ������ȹ��̡�
	}
	else
	{
		for(i = 0;i < (T_Length - M_Length + 1);i++)
		{
			for(j = 0;j < M_Length; j++)
			{
				if(TargetString[i + j] != MatchingString[j])//��Ӧλ�ò�ƥ�䣬����һλ�����¿�ʼƥ��
				{
					break;
				}
				else// ����ƥ��
				{
				}
			}
			if(j == M_Length)// ��ȫƥ��
			{
				break;
			}
			else// ʧ��
			{
			}
		}
		
		if(i > (T_Length - M_Length))// ���λ����Ȼ��ƥ��
		{
			return -1;
		}
		else// ƥ��ɹ������ض�ӦĿ���ַ���ƥ��λ��
		{
			return i;
		}
	}
	
	
	
}

/* Exported functions --------------------------------------------------------*/

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
