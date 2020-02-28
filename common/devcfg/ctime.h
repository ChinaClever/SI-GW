/*
 * ctime.h
 *
 *  Created on: 2019��6��6��
 *      Author: luozhiyong
 */

#ifndef BSP_STM32_STM32F407_ATK_EXPLORER_APPLICATIONS_TIMER_CTIME_H_
#define BSP_STM32_STM32F407_ATK_EXPLORER_APPLICATIONS_TIMER_CTIME_H_

#include <time.h>
#include <stdio.h>

// ��������򷵻�1�����С�ڷ���0��������ڷ���2��
int dt_cmpDt(const char* time1,const char* time2);
int dt_cmpCurrentDt(const char* time1);

// ��������򷵻�1�����С�ڷ���0��������ڷ���2��
int dt_cmpTime(const char* time1,const char* time2);
int dt_cmpCurrentTime(const char* time1);

//��ʱ���ת���ַ���
void dt_datetimeToStr(time_t t, char *pcTime);

// ���ַ���ת��ʱ���
long dt_strToDatetime(const char *str_time);

// ʱ����Ƚ�
int dt_cmpTimestamp(time_t t);

// ʱ��ת��������
int dt_timeToSec(const char* time1);

// ����ת����ʱ��
void dt_secToTime(int t, char *pcTime);

// �����Ƚ�
int dt_cmpSec(int sec);

#endif /* BSP_STM32_STM32F407_ATK_EXPLORER_APPLICATIONS_TIMER_CTIME_H_ */