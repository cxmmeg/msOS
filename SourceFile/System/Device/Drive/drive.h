/*
******************************************************************************
*                                Ƕ��ʽ΢ϵͳ
*                                   msOS
*
*                            Ӳ��ƽ̨:msPLC DEMO
*                          ��оƬ:STM32F103R8T6/RBT6
*                           ��������οƼ����޹�˾
*
*                                ����:����ΰ
*                                ����:������
*                                ��ʶ:wangsw
*
*                                QQ:26033613
*                               QQȺ:291235815
*                        ��̳:http://bbs.huayusoft.com
*                        �Ա���:http://52edk.taobao.com
*                ����:http://forum.eet-cn.com/BLOG_wangsw317_1268.HTM
******************************************************************************
*�ļ���   : drive.h
*����     : ������ͷ�ļ�
*����ʱ�� : 2013.7.21
******************************************************************************
*/

#ifndef __DRIVE_H
#define __DRIVE_H
#include "stm32f10x.h"

//IO�ڵ�ַӳ��
#define GPIOA_IDR_ADDR    (GPIOA_BASE+8) //0x40010808 
#define GPIOB_IDR_ADDR    (GPIOB_BASE+8) //0x40010C08 
#define GPIOC_IDR_ADDR    (GPIOC_BASE+8) //0x40011008 
#define GPIOD_IDR_ADDR    (GPIOD_BASE+8) //0x40011408 
#define GPIOE_IDR_ADDR    (GPIOE_BASE+8) //0x40011808 
#define GPIOF_IDR_ADDR    (GPIOF_BASE+8) //0x40011A08 
#define GPIOG_IDR_ADDR    (GPIOG_BASE+8) //0x40011E08 

#define GPIOA_ODR_ADDR    (GPIOA_BASE+12) //0x4001080C 
#define GPIOB_ODR_ADDR    (GPIOB_BASE+12) //0x40010C0C 
#define GPIOC_ODR_ADDR    (GPIOC_BASE+12) //0x4001100C 
#define GPIOD_ODR_ADDR    (GPIOD_BASE+12) //0x4001140C 
#define GPIOE_ODR_ADDR    (GPIOE_BASE+12) //0x4001180C 
#define GPIOF_ODR_ADDR    (GPIOF_BASE+12) //0x40011A0C    
#define GPIOG_ODR_ADDR    (GPIOG_BASE+12) //0x40011E0C    

//IO�ڲ���,ֻ�Ե�һ��IO��!ȷ��n��ֵС��16!
#define BitBand(ioAddress, pinNumber) ((ioAddress & 0xF0000000)+0x2000000+((ioAddress&0xFFFFF)<<5)+(pinNumber<<2)) 

#define PaIn(n)    *((volatile unsigned int  *)BitBand(GPIOA_IDR_ADDR,n))
#define PaOut(n)   *((volatile unsigned int  *)BitBand(GPIOA_ODR_ADDR,n))

#define PbIn(n)    *((volatile unsigned int  *)BitBand(GPIOB_IDR_ADDR,n))
#define PbOut(n)   *((volatile unsigned int  *)BitBand(GPIOB_ODR_ADDR,n))

#define PcIn(n)    *((volatile unsigned int  *)BitBand(GPIOC_IDR_ADDR,n))
#define PcOut(n)   *((volatile unsigned int  *)BitBand(GPIOC_ODR_ADDR,n))

#define PdIn(n)    *((volatile unsigned int  *)BitBand(GPIOD_IDR_ADDR,n))
#define PdOut(n)   *((volatile unsigned int  *)BitBand(GPIOD_ODR_ADDR,n))

#define PeIn(n)    *((volatile unsigned int  *)BitBand(GPIOE_IDR_ADDR,n))
#define PeOut(n)   *((volatile unsigned int  *)BitBand(GPIOE_ODR_ADDR,n))

#define PfIn(n)    *((volatile unsigned int  *)BitBand(GPIOF_IDR_ADDR,n))
#define PfOut(n)   *((volatile unsigned int  *)BitBand(GPIOF_ODR_ADDR,n))

#define PgIn(n)    *((volatile unsigned int  *)BitBand(GPIOG_IDR_ADDR,n))
#define PgOut(n)   *((volatile unsigned int  *)BitBand(GPIOG_ODR_ADDR,n))





#endif

