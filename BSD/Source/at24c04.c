/**
******************************************************************************
  * @file       at24c04.c
  * @brief      AT24C04 drive source file
  * @version    1.1
  * @date       Aug-15-2019 Thu
******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "at24c04.h"

/** @addtogroup AT24C04 Drive
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

void AT24C04_Init(void)
{
	/* �����ݲ�ʹ�ã��ú���ִ��ǰ���ȳ�ʼ��KEY */
	
	return ;
}

u8 AT24C04_ReadByte(const u8 ByteAddress,u8 *ReadData)
{
	IIC_Start();
	IIC_SendByte(AT24C04_WRITE_ADDR);
	if(IIC_CheckACK()!= 1)
	{
		return 1;
	}
	else
	{
	}
	IIC_SendByte(ByteAddress);
	if(IIC_CheckACK()!= 1)
	{
		return 1;
	}
	else
	{
	}
	
	IIC_Start();
	IIC_SendByte(AT24C04_READ_ADDR);
	if(IIC_CheckACK()!= 1)
	{
		return 1;
	}
	else
	{
	}
	*ReadData = IIC_RecvByte(0);
	IIC_CheckACK();
	
	IIC_Stop();
	
	return 0;
}


// ����ִ�к���ʱһ��ʱ��
u8 AT24C04_WriteByte(const u8 ByteAddress,const u8 WriteData)
{
	IIC_Start();
	IIC_SendByte(AT24C04_WRITE_ADDR);
	if(IIC_CheckACK()!= 1)
	{
		return 1;
	}
	else
	{
	}
	IIC_SendByte(ByteAddress);
	if(IIC_CheckACK()!= 1)
	{
		return 1;
	}
	else
	{
	}
	IIC_SendByte(WriteData);
	if(IIC_CheckACK()!= 1)
	{
		return 1;
	}
	else
	{
	}
	IIC_Stop();
	
	/* ����ȴ�һ��ʱ�� */
	Delay(10);
	
	return 0;
}


u8 AT24C04_WriteBuff(u8 Addr,const u8 *Buff,u8 Size)
{
	for(u32 i = 0; i < Size;i++)
	{
		if(AT24C04_WriteByte(Addr++,*(Buff + i)) != 0)
		{
			return 1;
		}
		else
		{
		}
	}
	
	return 0;
}

u8 AT24C04_ReadBuff(u8 Addr,u8 *Buff,u8 Size)
{
	for(u32 i = 0; i < Size;i++)
	{
		if(AT24C04_ReadByte(Addr++,Buff + i) != 0)
		{
			return 1;
		}
		else
		{
		}
	}
	
	return 0;
}

// ����д��0 �ظ�д��1 ��ȡʧ��2 д��ʧ��3 д�����ݴ���4
u8 AT24C04_Save16Byte(const u8 Data[16])
{
	u8 ReadTemp[16] = {0};
	if(AT24C04_ReadBuff(0x32,ReadTemp,16) != 0)
	{
		return 2;
	}
	else
	{
		if(HashCompare(Data,ReadTemp,16) == 0)
		{
			// ����һ�²����ظ�д��
			return 1;
		}
		else
		{
			if(AT24C04_WriteBuff(0x32,Data,16) != 0)
			{
				return 3;
			}
			else
			{
				if(AT24C04_ReadBuff(0x32,ReadTemp,16) != 0)
				{
					return 2;
				}
				else
				{
					/* д�����ݲ�һ�� */
					if(HashCompare(Data,ReadTemp,16) != 0)
					{
						return 4;
					}
					else
					{
						return 0;
					}
				}
			}
		}
	}
}


/**
  * @}
  */
