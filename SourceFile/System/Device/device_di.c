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
*�ļ���     : device_di.c
*����       : PLC����������ӿڣ�PLC��ʶΪX��
*ԭ��       : ��
********************************************************************************
*�汾     ����            ����            ˵��
*V0.1    Wangsw        2015/07/17       ��ʼ�汾
********************************************************************************
*/

#include "drive.h"
#include "system.h"


void DiSystick10000Routine(void)
{
    AppDataPointer->DI.X0 = *AppDataPointer->DI.pX0;
    AppDataPointer->DI.X1 = *AppDataPointer->DI.pX1;
    AppDataPointer->DI.X2 = *AppDataPointer->DI.pX2;
    AppDataPointer->DI.X3 = *AppDataPointer->DI.pX3;
}

void InitDI(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    AppDataPointer->DI.pX0 = (bool *)&PcIn->Bit0;
    AppDataPointer->DI.pX1 = (bool *)&PcIn->Bit1;
    AppDataPointer->DI.pX2 = (bool *)&PcIn->Bit2;
    AppDataPointer->DI.pX3 = (bool *)&PcIn->Bit3;
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
}

