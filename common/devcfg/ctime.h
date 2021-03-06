/*
 * ctime.h
 *
 *  Created on: 2019年6月6日
 *      Author: luozhiyong
 */

#ifndef BSP_STM32_STM32F407_ATK_EXPLORER_APPLICATIONS_TIMER_CTIME_H_
#define BSP_STM32_STM32F407_ATK_EXPLORER_APPLICATIONS_TIMER_CTIME_H_

#include <time.h>
#include <stdio.h>

// 如果大于则返回1，如果小于返回0，如果等于返回2。
int dt_cmpDt(const char* time1,const char* time2);
int dt_cmpCurrentDt(const char* time1);

// 如果大于则返回1，如果小于返回0，如果等于返回2。
int dt_cmpTime(const char* time1,const char* time2);
int dt_cmpCurrentTime(const char* time1);

//将时间戳转成字符串
void dt_datetimeToStr(time_t t, char *pcTime);

// 将字符串转成时间戳
long dt_strToDatetime(const char *str_time);

// 时间戳比较
int dt_cmpTimestamp(time_t t);

// 时间转换成秒数
int dt_timeToSec(const char* time1);

// 秒数转换成时间
void dt_secToTime(int t, char *pcTime);

// 秒数比较
int dt_cmpSec(int sec);

#endif /* BSP_STM32_STM32F407_ATK_EXPLORER_APPLICATIONS_TIMER_CTIME_H_ */
