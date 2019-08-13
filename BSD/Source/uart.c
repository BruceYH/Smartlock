/**
******************************************************************************
  * @file       uart.c
  * @brief      ������ز�������Դ�ļ�
  * @version    1.0
  * @date       Tue 06-08-2019
******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "uart.h"

/** @addtogroup UART
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
/**
  * @brief  USART1��ʼ��
  * @param  BaudRate
  * @return NULL
  */
void USART1_Init(u32 BaudRate)
{
	/* GPIO���� PA9 USART1_TX PA10 USART1_RX */
	//ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	GPIO_InitTypeDef GPIO_InitStruct;
	
	GPIO_InitStruct.GPIO_Pin		= GPIO_Pin_9;
	GPIO_InitStruct.GPIO_Speed		= GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_Mode		= GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Pin		= GPIO_Pin_10;
	GPIO_InitStruct.GPIO_Speed		= GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_Mode		= GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	
	/* USART1���� */
	//ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	
	USART_InitTypeDef USART_InitStruct;
	
	USART_InitStruct.USART_BaudRate				= BaudRate;
	USART_InitStruct.USART_HardwareFlowControl	= USART_HardwareFlowControl_None;
	USART_InitStruct.USART_Mode					= USART_Mode_Rx | USART_Mode_Tx;
	USART_InitStruct.USART_Parity				= USART_Parity_No;
	USART_InitStruct.USART_StopBits				= USART_StopBits_1;
	USART_InitStruct.USART_WordLength			= USART_WordLength_8b;
	
	USART_Init(USART1,&USART_InitStruct);
	
	USART_Cmd(USART1,ENABLE);
}

/**
  * @brief  USART2��ʼ��
  * @param  BaudRate
  * @return NULL
  */
void USART2_Init(u32 BaudRate)
{
	/* GPIO���� PA2 USART2_TX PA3 USART2_RX */
	GPIO_InitTypeDef GPIO_InitStruct;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitStruct.GPIO_Pin		= GPIO_Pin_2;
	GPIO_InitStruct.GPIO_Speed		= GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_Mode		= GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Pin		= GPIO_Pin_3;
	GPIO_InitStruct.GPIO_Speed		= GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_Mode		= GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	
	/* USART2���� */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	
	USART_InitTypeDef USART_InitStruct;
	
	USART_InitStruct.USART_BaudRate				= BaudRate;
	USART_InitStruct.USART_HardwareFlowControl	= USART_HardwareFlowControl_None;
	USART_InitStruct.USART_Mode					= USART_Mode_Rx | USART_Mode_Tx;
	USART_InitStruct.USART_Parity				= USART_Parity_No;
	USART_InitStruct.USART_StopBits				= USART_StopBits_1;
	USART_InitStruct.USART_WordLength			= USART_WordLength_8b;
	
	USART_Init(USART2,&USART_InitStruct);
	
	
	NVIC_InitTypeDef NVIC_InitStructure;
	
	/* Enable the USART2 Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	USART_ITConfig(USART2,USART_IT_RXNE, ENABLE);
	USART_ITConfig(USART2,USART_IT_TXE , DISABLE);
	// ʹ�� USART1

	USART_Cmd(USART2, ENABLE);//ʹ��֮�� TX TXE ��λ
}


void UART2_SendByte(u8 Data)
{
	USART_SendData(USART2,Data);
	while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET)
	{
	}
	
	return ;
}

/**
  * @}
  */
