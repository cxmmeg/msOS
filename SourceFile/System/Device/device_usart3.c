/*
********************************************************************************
*                                Ƕ��ʽ΢ϵͳ
*                                   msOS
*
*                            Ӳ��ƽ̨:msPLC DEMO
*                          ��оƬ:STM32F103R8T6/RBT6
*                           ��������οƼ����޹�˾
*
*                                ����:����ΰ
*                                ����:������
*                                ��ʶ:Wangsw
*
*                                QQ:26033613
*                               QQȺ:291235815
*                        ��̳:http://bbs.huayusoft.com
*                        �Ա���:http://52edk.taobao.com
*                ����:http://forum.eet-cn.com/BLOG_wangsw317_1268.HTM
********************************************************************************
*�ļ���     : device_usart3.c
*����       : �����豸,����HMI��
********************************************************************************
*�汾     ����            ����            ˵��
*V0.1    Wangsw        2015/11/20       ��ʼ�汾
********************************************************************************
*/
#include "drive.h"
#include "system.h"

#define Direct PbOut->Bit8

static void Usart3Dummy(byte data){;}
typedef void (*Rxd3Function)(byte data);
Rxd3Function Rxd3 = Usart3Dummy;

static void Register(uint rxdFucntion)
{
    Rxd3 = (Rxd3Function)rxdFucntion;
}

void USART3_IRQHandler(void)
{ 
    byte data;  
    
#if 0
    if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
    {    
        data = USART_ReceiveData(USART3);
        Rxd3(data);
    }

    if (USART_GetITStatus(USART3, USART_IT_TC) != RESET)    // ����֡���   
    {
        Direct = 0;
        USART_ITConfig(USART3, USART_IT_TC, DISABLE);
    }
    
 
	if(USART_GetFlagStatus(USART3,USART_FLAG_ORE) == SET)
		USART_ReceiveData(USART3);
#else  
    uint sr;
    sr = USART3->SR;

    if (sr & USART_FLAG_TC)
    {
            Direct = 0;
            USART_ITConfig(USART3, USART_IT_TC, DISABLE);
    }

    if (sr & USART_FLAG_RXNE)
    {
        data = USART_ReceiveData(USART3);
        Rxd3(data);
    }
    
    if (sr & USART_FLAG_ORE)
    {
        USART_ReceiveData(USART3);
    }
#endif 
}



static void WriteToUsart3(byte * dataPointer, int sum)
{
    Direct = 1;
    DMA_Cmd(DMA1_Channel2, DISABLE);
    DMA1_Channel2->CMAR = (uint)dataPointer;
    DMA1_Channel2->CNDTR = sum;
    USART_ITConfig(USART3, USART_IT_TC, DISABLE);
    DMA_Cmd(DMA1_Channel2, ENABLE);  
}

void DMA1_Channel2_IRQHandler(void)
{
    if (DMA_GetITStatus(DMA1_IT_TC2) == SET)
    {
        DMA_ClearITPendingBit(DMA1_IT_TC2);
        USART_ClearFlag(USART3, USART_FLAG_TC);
        USART_ITConfig(USART3, USART_IT_TC, ENABLE);
    }
}

void InitUsart3(void)
{
// GPIO�˿�����
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;   
    DMA_InitTypeDef DMA_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO,ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);  

// RS485_DIR   PB.8
    Direct = 0;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    
// USART3_TX   PB.10
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
   
// USART3_RX   PB.11
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOB, &GPIO_InitStructure);  

// Usart3 NVIC ����
    NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;      
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;         //IRQͨ��ʹ��
    NVIC_Init(&NVIC_InitStructure); 

// USART ��ʼ������
    USART_InitStructure.USART_BaudRate = 115200;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

    USART_Init(USART3, &USART_InitStructure);
    USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);   //�����ж�
    USART_Cmd(USART3, ENABLE);                       //ʹ�ܴ���
    
// DMA NVIC ����
    NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2; 
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;      
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;         //IRQͨ��ʹ��
    NVIC_Init(&NVIC_InitStructure); 
// DMA ��ʼ��
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);  
    DMA_DeInit(DMA1_Channel2);  

    DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)(&(USART3->DR));  
    DMA_InitStructure.DMA_MemoryBaseAddr = 0;  

    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;  
    DMA_InitStructure.DMA_BufferSize = 8;  
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;  
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;  
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;  
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;   
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;  
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;  
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;  
    DMA_Init(DMA1_Channel2,&DMA_InitStructure);  
    
    DMA_ITConfig(DMA1_Channel2, DMA_IT_TC,ENABLE);
    
    USART_DMACmd(USART3,USART_DMAReq_Tx,ENABLE);

  
    //DMA_Cmd(DMA1_Channel2, ENABLE); 

    System.Device.Usart3.Register = Register;
    System.Device.Usart3.Write = WriteToUsart3; 
}

