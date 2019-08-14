/**
******************************************************************************
  * @file       ui.c
  * @brief      User Interface
  * @version    1.0
  * @date       Aug-12-2019 Mon
******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "ui.h"


/** @addtogroup User Interface
  * @{
  */

/* Extern variables ----------------------------------------------------------*/
extern u8 ReversalFlag;	 // ��ʾ��ת��־
extern u8 CurrentWindowMode;

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

u8 Password[7] = "201988"; // ��ʼ����
u8 StringBuff[BUFF_LENGTH] = {0}; // ���뻺����

/* Private function prototypes -----------------------------------------------*/

void Password_Input(void);

extern u8 Admin_Check(void);

static void Admin_Mode(void);
static void User_Mode(void);

void ArrayForward(u8 *Array,u8 Length);
void ArrayBackward(u8 *Array,u8 Length);

static void Password_Input(void);
static u8 Password_Check(void);
static s32 String_ViolentMatch(const u8 *TargetString,const u8 *MatchingString);

static void OLED_Show1stRow(u8 RowNumber,u8 RowHeight);// ��ʾ���� ��#ȷ�� ��*ȡ��
static void OLED_Show2ndRow(u8 RowNumber,u8 RowHeight);// ��ʾ���� ����������
static void OLED_ShowTextBox(u8 RowNumber,u8 RowHeight,u8 *Str);
static void OLED_ShowMenuRow(u8 RowNumber,u8 RowHeight);

/* Private functions ---------------------------------------------------------*/

void OLED_Show1stRow(u8 RowNumber,u8 RowHeight)
{
	u32 j[] = {2,1,2,2,1,1,2,1,2,2}; // ��¼�ֿ������
	u32 k = 0; // ��¼��Ӧ����λ��
	
	for(u32 i = 0;i < 10 ;i++) // ��ʾ10����
	{
		OLED_Show_XxN8_Character(RowNumber,k * 8,RowHeight,j[i] * 8,&(String1_16xN[k][0]));
		k += j[i];
	}
	
	return ;
}

void OLED_Show2ndRow(u8 RowNumber,u8 RowHeight)
{
	for(u32 j = 0;j < 10;j+=2)
	{
		OLED_Show_XxN8_Character(RowNumber,(j + 3) * 8,RowHeight,16,&(String2_16xN[j][0]));
	}
	return ;
}

void OLED_ShowTextBox(u8 RowNumber,u8 RowHeight,u8 *Str)
{
	for(u32 i = 0; i < 8; i++)
	{
		if(Str[i] == 0) // ������ַ����ж�Ӧ�ַ�ֵΪ0
		{
			// ��ʾ��_��
			OLED_Show_XxN8_Character(RowNumber,(i * 16) + 4,RowHeight,8,&Char_8x16[0][0]);
		}
		else // ������ַ����ж�Ӧ�ַ�ֵ��0
		{
			// ��ʾ��*��
			OLED_Show_XxN8_Character(RowNumber,(i * 16) + 4,RowHeight,8,&Char_8x16[1][0]);
		}
	}
	return ;
}

void OLED_ShowMenuRow(u8 RowNumber,u8 RowHeight)
{
	
	for(u32 i = 0;i < 4 ;i++) // ��ʾ4����
	{
		OLED_Show_XxN8_Character(RowNumber,(i + 2) * 16,RowHeight,16,&(String3_16xN[2*i][0]));
	}
	
	return ;
}

void OLED_ShowWaitRow(u8 RowNumber,u8 RowHeight)
{
	
	for(u32 i = 0;i < 5 ;i++) // ��ʾ5����
	{
		OLED_Show_XxN8_Character(RowNumber,i * 16 + 24,RowHeight,16,&(String4_16xN[2*i][0]));
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
		
		if(CurrentWindowMode == WindowMode_AllClear)
		{
			return ;
		}
		else
		{
		}
		
		
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


u8 Menu_Move(u8 *MenuList,u8 MenuListLenth)
{
	u8 KeyValue;
	
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
			return MenuPlace_Check;
		}
		else if(KeyValue == '*')
		{
			return MenuPlace_Back;
		}
		else if(KeyValue == '2')
		{
			ArrayForward(MenuList,MenuListLenth);
			return MenuPlace_Shift;
			
		}
		else if(KeyValue == '8')
		{
			ArrayBackward(MenuList,MenuListLenth);
			return MenuPlace_Shift;
		}
		else
		{
		}
	}
	else
	{
	}
	
	return MenuPlace_NoEnter;;
}


u8 Password_Check(void)
{	
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

void ArrayBackward(u8 *Array,u8 Length)
{
	u8 First = Array[0];
	u32 i;
	for(i = 0;i < Length - 1;i++)
	{
		Array[i] = Array[i + 1];
	}
	Array[i] = First;
}

void ArrayForward(u8 *Array,u8 Length)
{
	u8 Last = Array[Length - 1];
	u32 i;
	for(i = Length - 1;i > 0;i--)
	{
		Array[i] = Array[i - 1];
	}
	Array[i] = Last;
}

void Admin_Mode(void)
{	
	OLED_ShowMenuRow(0,2);
	OLED_Show2ndRow(2,2);
	
	Voice_Play(VoiceCmd_INPUT_ADMIN_PASSWORD);
	
	Password_Input();
	
	if(CurrentWindowMode == WindowMode_AllClear)
	{
	return ;
	}
	else
	{
	}
	
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
		
	return ;
}

void User_Mode(void)
{
	static u8 ErrorCount = 0;
	// TODO �����������
	
	OLED_Show1stRow(0,2);
	OLED_Show2ndRow(2,2);
	
	// TODO ���ݰ������������������ + ˢ������������
	
	Password_Input(); // �ڲ�������OLED_ShowTextBox()����ʵʱ��ʾ����״��
	
	if(CurrentWindowMode == WindowMode_AllClear)
	{
	return ;
	}
	else
	{
	}
	
#ifdef DEBUG
	printf("StringBuff = \"%s\"\r\n",StringBuff);
#endif		
	
	if(Password_Check() == 1)
	{
		printf("Password right\r\n");
		Voice_Play(VoiceCmd_DOOROPEN_SUCCESS);
		ErrorCount = 0;
	}
	else
	{
		printf("Password wrong\r\n");
		Voice_Play(VoiceCmd_PASSWORD_INCONFORMITY);
		ErrorCount++;
	}
	
	if(ErrorCount > 2)
	{
		for(u32 i = 0 ;i<10;i++)
		{
			OLED_ShowWaitRow(2,2);
			OLED_Show_XxN8_Character(2,80,2,8,&(Number_8x16[9-i][0]));
			TIM2_Delay_ms(1000);
		}
		ErrorCount = 1;
	}
	else
	{
	}
	
	return ;
}

void SubMenu_Change(u32 *NextSubMenu,u8 MenuCurrentPlace)
{
	switch(MenuCurrentPlace)
	{
		case 0:
			*NextSubMenu = SubMenu_ChangePassword;
			break;
		case 1:
			*NextSubMenu = SubMenu_MuteSetting;
			break;
		default:
			*NextSubMenu = SubMenu_Start;
	}
}

void Menu_Start(u32 *LastMenu)
{
	const u8 *Str[4] = 
	{
		&(MenuString1_16x16[0][0]),
		&(MenuString2_16x16[0][0]),
		&(MenuString3_16x16[0][0]),
		&(MenuString4_16x16[0][0]),
	};
	const u8 StrLenArray[4] = {6,4,4,4};
	
	static u8 CurrentPlace[4] = {0,1,2,3};
	
	switch (Menu_Move(CurrentPlace,4))
	{
		case MenuPlace_Check:
			SubMenu_Change(LastMenu,CurrentPlace[1]);
			OLED_Clear();
			break;
		case MenuPlace_Back:
			OLED_Clear();
			CurrentWindowMode = WindowMode_User;
			break;
		case MenuPlace_Shift:
			OLED_Clear();
			break;
		default:
			break;
	}
	
	OLED_ShowMenuRow(0,2);
	OLED_ShowString(2,2,0,Str[CurrentPlace[0]],StrLenArray[CurrentPlace[0]]);
	ReversalFlag = 1;
	OLED_ShowString(4,2,0,Str[CurrentPlace[1]],StrLenArray[CurrentPlace[1]]);
	ReversalFlag = 0;
	OLED_ShowString(6,2,0,Str[CurrentPlace[2]],StrLenArray[CurrentPlace[2]]);
	
	if(*LastMenu != SubMenu_Start)
	{
		OLED_Clear();
	}
	else
	{
	}
	
	return ;
}

void Mute_Setting(u8 MenuElmt)
{
	// �˵���һ���Ǿ���
	if(MenuElmt == 0)
	{
		MuteMode = 1;
	}
	else
	{
		MuteMode = 0;
	}
}

void Menu_MuteSetting(u32 *LastMenu)
{
	const u8 *Str[2] = 
	{
		&(MuteString1_16x16[0][0]),
		&(MuteString2_16x16[0][0])
	};
	const u8 StrLenArray[2] = {2,2};
	
	static u8 CurrentPlace[2] = {0,1};
	
	switch (Menu_Move(CurrentPlace,2))
	{
		case MenuPlace_Check:
			Mute_Setting(CurrentPlace[0]);
			OLED_Clear();
			break;
		case MenuPlace_Back:
			*LastMenu = SubMenu_Start;
			OLED_Clear();
			break;
		case MenuPlace_Shift:
			OLED_Clear();
			break;
		default:
			break;
	}
	
	
	
	OLED_ShowString(0,2,32,&MenuString2_16x16[0][0],4); // ������ʾ������ģʽ��
	
	ReversalFlag = 1;
	OLED_ShowString(CurrentPlace[0] * 2 + 2,2,0,Str[CurrentPlace[0]],StrLenArray[CurrentPlace[0]]);
	ReversalFlag = 0;
	OLED_ShowString(CurrentPlace[1] * 2 + 2,2,0,Str[CurrentPlace[1]],StrLenArray[CurrentPlace[1]]);
	
	if(*LastMenu != SubMenu_MuteSetting)
	{
		OLED_Clear();
	}
	else
	{
	}
	
	return ;
}

void Window_AdminMode(void)
{
	Admin_Mode();
	memset(StringBuff,0,sizeof(StringBuff));// ������뻺����
	OLED_Clear();
	
	CurrentWindowMode = WindowMode_Setting;
	
	return ;
}

void Window_UserMode(void)
{
	static u8 FirstFlag = 0;
	
	if(FirstFlag == 0)
	{
		OLED_Clear();
		OLED_ShowPicture(28,0,72,64,&(Logo_72x64[0][0]));
		Delay(1000);
		FirstFlag = 1;
	}
	else
	{
		OLED_Clear();
		User_Mode();
		
		memset(StringBuff,0,sizeof(StringBuff));// ������뻺����
	}
	
	return ;
}

void Window_SettingMode(void)
{
	static u32 CurrentMenu = SubMenu_Start; 
	
	switch(CurrentMenu)
	{
		case SubMenu_Start:
			Menu_Start(&CurrentMenu);
			break;
		case SubMenu_MuteSetting:
			Menu_MuteSetting(&CurrentMenu);
			break;
		default:
			CurrentMenu = SubMenu_Start;
	}
	
	
	return ;
}

/* Exported functions --------------------------------------------------------*/

void Window_MainTask(void)
{
	switch(CurrentWindowMode)
	{	
		case WindowMode_Admin:
			Window_AdminMode();
//			*WindowMode = WindowMode_User;
			break;
		case WindowMode_User:
			Window_UserMode();
			break;
		case WindowMode_Setting:
			Window_SettingMode();
			break;
		default:
			OLED_Clear();
	}
}

/**
  * @}
  */
