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
*�ļ���     : device_rtc.c
*����       : 32.768KHzʱ�Ӿ��������������ʱ�Ӵ�2013��1��1�����ڶ���ʼ,����Ҫ�������һ�꿪ʼ
*ԭ��       : ����������ֻ���ʺ�2013~2099�꣬2100�������
********************************************************************************
*�汾     ����            ����            ˵��
*V0.1    Wangsw        2015/11/25       ��ʼ�汾
********************************************************************************
*/

#include "drive.h"
#include "system.h"

#define SecondsInDay            86400       // 1���ܹ�86400��
#define DayInFourYear           1461        // 4���ܹ�1461�죬365*3+366
                                                                       
static const byte DaysInNonLeapMonthTable[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};  // ������·����ڱ�
static const byte DaysInLeapMonthTable[12] =    {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};  // ƽ����·����ڱ�


/*******************************************************************************
* ����	    : �������ڣ�����Ӧ�����õ��ǵ�ǰʱ��
* �������  : ����
* ���ز���  : ����״̬
*******************************************************************************/
static void SetDate(void)
{
    int i;
    uint second;
    uint readSecond;
    uint year;
    uint month;
    uint day;


    year = AppDataPointer->Rtc.Year - 2013;

    day = (year / 4) * DayInFourYear + (year % 4) * 365;

    month = AppDataPointer->Rtc.Month - 1;
    if (AppDataPointer->Rtc.Year % 4 == 0)
    {
        for (i = 0; i < month; i++)
            day = day + DaysInLeapMonthTable[i];
    }
    else
    {
        for (i = 0; i < month; i++)
            day = day + DaysInNonLeapMonthTable[i];
    }

    day = day + AppDataPointer->Rtc.Day - 1;

    second = day * SecondsInDay + AppDataPointer->Rtc.Hour * 3600 + AppDataPointer->Rtc.Minute * 60 + AppDataPointer->Rtc.Second;
    readSecond = RTC_GetCounter();
    if (second > readSecond + 10 || second + 10 < readSecond)
    {
        PWR_BackupAccessCmd(ENABLE);                           //��ʼ�޸�����
        RTC_WaitForLastTask();
        RTC_SetCounter(second);
        RTC_WaitForLastTask(); 
    }                                                                                     
}


/*******************************************************************************
* ����	    : ��ȡ��ǰ���ڣ���ȡ��������ֱ�Ӵ浽���ݿ���
* �������  : ��
* ���ز���  : ��ȡ״̬
*******************************************************************************/
static void GetDate(void)
{     
    uint second;  
    uint day;
    uint dayInFourYear;
    
    uint year = 2013;
    uint month = 0;



    second = RTC_GetCounter();
    day = second / SecondsInDay;
    AppDataPointer->Rtc.Week = (day + 2) % 7;
        
    dayInFourYear = day % DayInFourYear;

    if (AppDataPointer->Rtc.Year % 4 == 0)
        AppDataPointer->Rtc.Year = year + (day / DayInFourYear) * 4 + dayInFourYear / 366;
    else 
        AppDataPointer->Rtc.Year = year + (day / DayInFourYear) * 4 + dayInFourYear / 365;

    if (dayInFourYear > 1095)
    {
        day = dayInFourYear - 1095;
        while (day >= DaysInLeapMonthTable[month])
        {
            day = day - DaysInLeapMonthTable[month];
            month++;
        }
    }
    else
    {
        day = dayInFourYear % 365;
        while (day >= DaysInNonLeapMonthTable[month])
        {
            day = day - DaysInNonLeapMonthTable[month];
            month++;
        }
    }

    AppDataPointer->Rtc.Month = month + 1;               
    AppDataPointer->Rtc.Day   = day + 1;                 

    second = second % SecondsInDay;                  
    AppDataPointer->Rtc.Hour   = second / 3600;                        
    AppDataPointer->Rtc.Minute = (second % 3600) / 60;        
    AppDataPointer->Rtc.Second = (second % 3600) % 60;    
}    

static void RTC_Config(void)
{
    int counter = 0;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP,ENABLE);
    PWR_BackupAccessCmd(ENABLE);
    BKP_DeInit();
    RCC_LSEConfig(RCC_LSE_ON);
    while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)
    {
        if(++counter == 2000000)
            break;
    }
    if(counter < 2000000)
        RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
    else
    {
        RCC_LSICmd(ENABLE);
        while(RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET);
        RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);
    }
    RCC_RTCCLKCmd(ENABLE);
    RTC_WaitForSynchro();
    RTC_WaitForLastTask();

    if(counter < 2000000) 
        RTC_SetPrescaler(32767);
    else
        RTC_SetPrescaler(40000 - 1);
    RTC_WaitForLastTask();
    RTC_ITConfig(RTC_IT_SEC,ENABLE);
    RTC_WaitForLastTask();
}

void RtcSystick100Routine(void)
{
    static byte Counter;
    if (++Counter == 100)
    {
        Counter = 0;

        SetDate();
        GetDate();
    }
}

/*******************************************************************************
* ����	    : ��ʼ��RTC�豸
* �������  : ��
* ���ز���  : ��
*******************************************************************************/
void InitRtc(void)
{
    if(BKP_ReadBackupRegister(BKP_DR1) != 0x1234)
    {
        RTC_Config();
        BKP_WriteBackupRegister(BKP_DR1,0x1234);
    }
    else
    {
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP,ENABLE);
        PWR_BackupAccessCmd(ENABLE);
        if((RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET) && (RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET))
            RTC_Config();
        
        RTC_WaitForSynchro();
        RTC_WaitForLastTask();
        RTC_ITConfig(RTC_IT_SEC,ENABLE);
        RTC_WaitForLastTask();
    }

    GetDate();
}


