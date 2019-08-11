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

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define BUFF_LENGTH 256  // �������뻺����״̬

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
u8 Password[] = "201988"; // ��ʼ����
u8 StringBuff[BUFF_LENGTH] = {0}; // ���뻺����


/* Private function prototypes -----------------------------------------------*/
static void JTAG_Disable(void);

static u8 Admin_Check(void);

static void Admin_Mode(void);
static void User_Mode(void);

static void Password_Input(void);
static u8 Password_Check(void);
static s32 String_ViolentMatch(const u8 *TargetString,const u8 *MatchingString);

void OLED_Show1stCow(u8 CowNumber,u8 CowHeight);// ��ʾ���� ��#ȷ�� ��*ȡ��
void OLED_Show2ndCow(u8 CowNumber,u8 CowHeight);// ��ʾ���� ����������
void OLED_ShowTextBox(u8 CowNumber,u8 CowHeight,u8 *Str);

void OLED_ShowMenuCow(u8 CowNumber,u8 CowHeight);


/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main function
  * @param  NULL
  * @return NULL
  */
int main(void)
{
	u8 AdminFlag = 0;
	
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
	AdminFlag = 0;
	AdminFlag = Admin_Check();
	
	/* ����ģ�� IRQ Pin Init */
	EXTI3_Init();
	
	/* �ȴ�ϵͳ�ȶ� */
	delay_ms(500);//�ȴ��ȶ�
	
	while(1)
	{
		
		if(AdminFlag == 1)
		{
			Admin_Mode();
			AdminFlag = 0;
		}
		else
		{
			User_Mode();
		}

		Delay(1000);
		OLED_Clear();
		
		memset(StringBuff,0,sizeof(StringBuff));// ������뻺����
	}
}

void JTAG_Disable(void)
{
	/* ��ӳ�� */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);// ��λ��ſ��ٴ���ӳ��
	
	return ;
}


void OLED_Show1stCow(u8 CowNumber,u8 CowHeight)
{
	u32 j[] = {2,1,2,2,1,1,2,1,2,2}; // ��¼�ֿ������
	u32 k = 0; // ��¼��Ӧ����λ��
	
	for(u32 i = 0;i < 10 ;i++) // ��ʾ10����
	{
		OLED_Show_XxN8_Character(CowNumber,k * 8,CowHeight,j[i] * 8,&(String1_16xN[k][0]));
		k += j[i];
	}
	
	return ;
}

void OLED_Show2ndCow(u8 CowNumber,u8 CowHeight)
{
	for(u32 j = 0;j < 10;j+=2)
	{
		OLED_Show_XxN8_Character(CowNumber,(j + 3) * 8,CowHeight,16,&(String2_16xN[j][0]));
	}
	return ;
}

void OLED_ShowTextBox(u8 CowNumber,u8 CowHeight,u8 *Str)
{
	for(u32 i = 0; i < 8; i++)
	{
		if(Str[i] == 0) // ������ַ����ж�Ӧ�ַ�ֵΪ0
		{
			// ��ʾ��_��
			OLED_Show_XxN8_Character(CowNumber,(i * 16) + 4,CowHeight,8,&Char_8x16[0][0]);
		}
		else // ������ַ����ж�Ӧ�ַ�ֵ��0
		{
			// ��ʾ��*��
			OLED_Show_XxN8_Character(CowNumber,(i * 16) + 4,CowHeight,8,&Char_8x16[1][0]);
		}
	}
	return ;
}

void OLED_ShowMenuCow(u8 CowNumber,u8 CowHeight)
{
	
	for(u32 i = 0;i < 4 ;i++) // ��ʾ10����
	{
		OLED_Show_XxN8_Character(CowNumber,(i + 2) * 16,CowHeight,16,&(String3_16xN[2*i][0]));
	}
	
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

u8 Admin_Check(void)
{
	u32 i = 0;

	Delay(1000);
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

void Admin_Mode(void)
{
	OLED_ShowMenuCow(0,2);
	OLED_Show2ndCow(2,2);
	
	Voice_Play(VoiceCmd_INPUT_ADMIN_PASSWORD);
	
	Password_Input();
	
	if(Password_Check() == 1)
	{
		printf("Password right\r\n");
		Voice_Play(VoiceCmd_SETTING_SUCCESS);
	}
	else
	{
		printf("Password wrong\r\n");
		Voice_Play(VoiceCmd_PASSWORD_INCONFORMITY);
	}

}

void User_Mode(void)
{
	// TODO �����������
	
	OLED_Show1stCow(0,2);
	OLED_Show2ndCow(2,2);
	
	// TODO ���ݰ������������������ + ˢ������������
	
	Password_Input(); // �ڲ�������OLED_ShowTextBox()����ʵʱ��ʾ����״��
	
	
#ifdef DEBUG
	printf("StringBuff = \"%s\"\r\n",StringBuff);
#endif		
	
	if(Password_Check() == 1)
	{
		printf("Password right\r\n");
		Voice_Play(VoiceCmd_DOOROPEN_SUCCESS);
	}
	else
	{
		printf("Password wrong\r\n");
		Voice_Play(VoiceCmd_PASSWORD_INCONFORMITY);
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
