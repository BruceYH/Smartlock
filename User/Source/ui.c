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

void ArrayForward(u8 *Array,u8 Length);
void ArrayBackward(u8 *Array,u8 Length);

static void Password_Input(void);

/* Private functions ---------------------------------------------------------*/


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
		
		GUI_Show8StrTextBox(4,2,StringBuff);
		GUI_Show8StrTextBox(6,2,StringBuff + 8);
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

void Admin_Mode(void)
{	
	GUI_DisplayString(0,32,&String3_16xN[0][0],4);// ������ʾ�����ò˵���
	GUI_DisplayString(2,24,&String2_16xN[0][0],5);// �����������롱
	
	Voice_Play(VoiceCmd_INPUT_ADMIN_PASSWORD);
	
	Password_Input();
	
	if(CurrentWindowMode == WindowMode_AllClear)
	{
	return ;
	}
	else
	{
	}
	
	if(FakePassword_Check(StringBuff,SmartLock.UserPassword) == 1)
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

void User_PasswordMode(void)
{
	static u8 FirstFlag = 0;
	static u8 ErrorCount = 0;
	// TODO �����������
	
	if(Key_Scan() != 0 || FirstFlag == 0)
	{
		
		if(FirstFlag == 0)
		{
			FirstFlag = 1;
		}
		else
		{
		}
		
		GUI_ShowOperationTipRow(0,2);// ��ʾ"��#ȷ�� ��*ɾ��"
		GUI_DisplayString(2,24,&String2_16xN[0][0],5);// �����������롱

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

		if(FakePassword_Check(StringBuff,SmartLock.UserPassword) == 1)
		{
			printf("Password right\r\n");
			Voice_Play(VoiceCmd_DOOROPEN_SUCCESS);
			Motor_OpenLock();
			ErrorCount = 0;
		}
		else
		{
			printf("Password wrong\r\n");
			Voice_Play(VoiceCmd_PASSWORD_INCONFORMITY);
			Motor_CloseLock();
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
		case SubMenu_PasswordChange-1:
			*NextSubMenu = SubMenu_PasswordChange;
			break;
		case SubMenu_MuteSetting-1:
			*NextSubMenu = SubMenu_MuteSetting;
			break;
		case SubMenu_FingerMange-1:
			*NextSubMenu = SubMenu_FingerMange;
			break;
		case SubMenu_IDCardMange-1:
			*NextSubMenu = SubMenu_IDCardMange;
			break;
		default:
			*NextSubMenu = SubMenu_Start;
	}
}

void Menu_Start(u32 *LastMenu)
{
	// �ַ���˳������û����ò˵�˳������ʹ�õ������ò˵��Ӳ˵�ö�ٽṹ���˳��
	const u8 *Str[4] = 
	{
		&(MenuString1_16x16[0][0]),// ���޸��û����롱
		&(MenuString2_16x16[0][0]),// ������ģʽ��
		&(MenuString3_16x16[0][0]),// ���Ǽ�ָ�ơ�
		&(MenuString4_16x16[0][0]),// ���Ǽǿ�Ƭ��
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
	
	GUI_DisplayString(0,32,&String3_16xN[0][0],4);// ������ʾ�����ò˵���
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
		SmartLock.MuteMode = 1;
	}
	else
	{
		SmartLock.MuteMode = 0;
	}
}

//void Password_Change(u8 MenuElmt)
//{
//	
//}

void Menu_PasswordChange(u32 *LastMenu)
{
	if(GUI_Password_Check(SmartLock.AdminPassword,6) == 0)
	{
		GUI_Password_Enroll(SmartLock.UserPassword,6);
	}
	else
	{
		
	}
	Delay(1000);
	
	GUI_ClearScreen();
	
	*LastMenu = SubMenu_Start;
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

void Finger_Mange(u8 MenuElmt)
{
	// �˵���һ���������û�
	if(MenuElmt == 0)
	{
		// �����û�����
		GUI_Finger_EnrollNewUser();
	}
	else
	{
		// ɾ���û�����
		GUI_Finger_EraseAllUser();
	}
}


void Menu_FingerMange(u32 *LastMenu)
{
	const u8 *Str[2] = 
	{
		&(UserMangeString1_16x16[0][0]),// �����û�
		&(UserMangeString2_16x16[0][0]) // ɾ���û�
	};
	const u8 StrLenArray[2] = {4,4};// ��¼�ַ�������
	
	static u8 CurrentPlace[2] = {0,1};

	switch (Menu_Move(CurrentPlace,2))
	{
		case MenuPlace_Check:
			Finger_Mange(CurrentPlace[0]);// ִ�в˵�λ�ü�¼�����Ӧѡ������
			GUI_ClearScreen();
			break;
		case MenuPlace_Back:
			*LastMenu = SubMenu_Start;
			GUI_ClearScreen();
			break;
		case MenuPlace_Shift:
			GUI_ClearScreen();
			break;
		default:
			break;
	}
	
	GUI_DisplayString(0,32,&MenuString3_16x16[0][0],4);// ������ʾ���Ǽ�ָ�ơ�
	
	ReversalFlag = 1;
	OLED_ShowString(CurrentPlace[0] * 2 + 2,2,0,Str[CurrentPlace[0]],StrLenArray[CurrentPlace[0]]);
	ReversalFlag = 0;
	OLED_ShowString(CurrentPlace[1] * 2 + 2,2,0,Str[CurrentPlace[1]],StrLenArray[CurrentPlace[1]]);
	
	if(*LastMenu != SubMenu_FingerMange)
	{
		GUI_ClearScreen();
	}
	else
	{
	}
	
	return ;
}

void IDCard_Mange(u8 MenuElmt)
{
	// �˵���һ���������û�
	if(MenuElmt == 0)
	{
		// �����û�����
		GUI_RFID_AuthorizeNewCard();
	}
	else
	{
		// ɾ���û�����
	}
}

void Menu_IDCardMange(u32 *LastMenu)
{
	const u8 *Str[2] = 
	{
		&(UserMangeString1_16x16[0][0]),// �����û�
		&(UserMangeString2_16x16[0][0]) // ɾ���û�
	};
	const u8 StrLenArray[2] = {4,4};// ��¼�ַ�������
	
	static u8 CurrentPlace[2] = {0,1};

	switch (Menu_Move(CurrentPlace,2))
	{
		case MenuPlace_Check:
			IDCard_Mange(CurrentPlace[0]);// ִ�в˵�λ�ü�¼�����Ӧѡ������
			GUI_ClearScreen();
			break;
		case MenuPlace_Back:
			*LastMenu = SubMenu_Start;
			GUI_ClearScreen();
			break;
		case MenuPlace_Shift:
			GUI_ClearScreen();
			break;
		default:
			break;
	}
	
	GUI_DisplayString(0,32,&MenuString4_16x16[0][0],4);// ������ʾ���Ǽǿ�Ƭ��
	
	ReversalFlag = 1;
	OLED_ShowString(CurrentPlace[0] * 2 + 2,2,0,Str[CurrentPlace[0]],StrLenArray[CurrentPlace[0]]);
	ReversalFlag = 0;
	OLED_ShowString(CurrentPlace[1] * 2 + 2,2,0,Str[CurrentPlace[1]],StrLenArray[CurrentPlace[1]]);
	
	if(*LastMenu != SubMenu_IDCardMange)
	{
		GUI_ClearScreen();
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
	// Ĭ���û���ģʽ CurrentUserMode = UserSubMode_Password;
	
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
		switch(CurrentUserMode)
		{
			case UserSubMode_Password:
				OLED_Clear();
				User_PasswordMode();
				break;
			case UserSubMode_Finger:
				GUI_Finger_Compare();
				break;
			default:
				CurrentUserMode = UserSubMode_Password;
		}
		
		
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
		case SubMenu_PasswordChange:
			Menu_PasswordChange(&CurrentMenu);
			break;
		case SubMenu_MuteSetting:
			Menu_MuteSetting(&CurrentMenu);
			break;
		case SubMenu_FingerMange:
			Menu_FingerMange(&CurrentMenu);
			break;
		case SubMenu_IDCardMange:
			Menu_IDCardMange(&CurrentMenu);
			break;
		default:
			CurrentMenu = SubMenu_Start;
	}
	
	
	return ;
}

/* Exported functions --------------------------------------------------------*/

void Task_WindowMain(void)
{
	switch(CurrentWindowMode)
	{	
		case WindowMode_Admin:
			Window_AdminMode();
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
