/**
******************************************************************************
  * @file       user_string.c
  * @brief      �û��Զ����ַ�������
  * @version    1.0
  * @date       Aug-14-2019 Wed
******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "user_string.h"

/** @addtogroup 
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

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

/**
  * @}
  */
