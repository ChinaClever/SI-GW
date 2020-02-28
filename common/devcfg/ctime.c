/*
 * ctime.c
 *
 *  Created on: 2019��6��6��
 *      Author: luozhiyong
 */
#include "ctime.h"

// ��������򷵻�1�����С�ڷ���0��������ڷ���2��
int dt_cmpDt(const char* time1,const char* time2)
{
	int year1,month1,day1,hour1,min1,sec1;
	int year2,month2,day2,hour2,min2,sec2;
	sscanf(time1,"%d-%d-%d %d:%d:%d",&year1,&month1,&day1,&hour1,&min1,&sec1);
	sscanf(time2,"%d-%d-%d %d:%d:%d",&year2,&month2,&day2,&hour2,&min2,&sec2);
	int tm1 = year1*10000+month1*100+day1;
	int tm2 = year2*10000+month2*100+day2;
	if(tm1!=tm2) return (tm1>tm2)?1:0;//�����ȣ��󷵻�1��С����0
	tm1 = hour1*3600+min1*60+sec1;
	tm2 = hour2*3600+min2*60+sec2;//��ʱ����ת��Ϊ����
	if(tm1!=tm2) return (tm1>tm2)?1:0;//�����ȣ��󷵻�1��С����0
	return 2;//�������Ȼ�������
}

int dt_cmpCurrentDt(const char* time1)
{
	time_t t;
	time (&t);//��ȡUnixʱ�����
	struct tm *lt = localtime (&t);//תΪʱ��ṹ��
	if(strlen(time1)<12) return -1;

	char time2[42] = {0};
	sprintf (time2, "%d-%d-%d %d:%d:%d\n",lt->tm_year+1900, lt->tm_mon, lt->tm_mday, lt->tm_hour, lt->tm_min, lt->tm_sec);//������
	return dt_cmpDt(time1, time2);
}

// ��������򷵻�1�����С�ڷ���0��������ڷ���2��
int dt_cmpTime(const char* time1,const char* time2)
{
	int hour1,min1,sec1;
	int hour2,min2,sec2;
	sscanf(time1,"%d:%d:%d",&hour1,&min1,&sec1);
	sscanf(time2,"%d:%d:%d",&hour2,&min2,&sec2);
	int tm1 = hour1*3600+min1*60+sec1;
	int tm2 = hour2*3600+min2*60+sec2;//��ʱ����ת��Ϊ����
	if(tm1!=tm2) return (tm1>tm2)?1:0;//�����ȣ��󷵻�1��С����0
	return 2;//�������Ȼ�������
}

int dt_cmpCurrentTime(const char* time1)
{
	time_t t;
	time (&t);//��ȡUnixʱ�����
	struct tm *lt = localtime (&t);//תΪʱ��ṹ��
	if(strlen(time1)<6) return -1;

	char time2[32] = {0};
	sprintf (time2, "%d:%d:%d\n",lt->tm_hour, lt->tm_min, lt->tm_sec);//������
	return dt_cmpTime(time1, time2);
}


//��ʱ���ת���ַ��� strfmt
void dt_datetimeToStr(time_t t, char *pcTime)
{
	struct tm *lt;
	lt = localtime(&t);
//	strftime(pcTime,128,"%F %T",lt);
	sprintf (pcTime, "%d-%02d-%02dT%02d:%02d:%02d\n",lt->tm_year+1900, lt->tm_mon, lt->tm_mday, lt->tm_hour, lt->tm_min, lt->tm_sec);//������
}

// ���ַ���ת��ʱ���
long dt_strToDatetime(const char *str_time)
{
	struct tm stm;
//	strptime(str_time, "%Y-%m-%d %H:%M:%S",&stm);
	int year, month, day, hour, minute,second;
	sscanf(str_time,"%d-%02d-%02dT%02d:%02d:%02d", &year, &month, &day, &hour, &minute, &second);
	stm.tm_year  = year-1900;
	stm.tm_mon   = month;
	stm.tm_mday  = day;
	stm.tm_hour  = hour;
	stm.tm_min   = minute;
	stm.tm_sec   = second;
	stm.tm_isdst = 0;

	long t = mktime(&stm);
	return t;
}

// ʱ����Ƚ�
int dt_cmpTimestamp(time_t t)
{
	time_t tm;
	time (&tm);//��ȡUnixʱ���

	return t-tm;
}

// ��ȡ�����ѹ�������
int dt_secStartOfDay()
{
	time_t t;
	time (&t);//��ȡUnixʱ�����
	struct tm *lt = localtime (&t);//תΪʱ��ṹ��

	int sec = lt->tm_hour*3600 + lt->tm_min*60 + lt->tm_sec;
	return sec;
}

// ʱ��ת��������
int dt_timeToSec(const char* time)
{
	int hour,min,sec;
	sscanf(time,"%02d:%02d:%02d",&hour,&min,&sec);
	int tm = hour*3600+min*60+sec;
	return tm;
}

// ����ת����ʱ��
void dt_secToTime(int t, char *pcTime)
{
	int hour,min,sec;

	sec = (int)(t % 60); t /= 60;//ȡ��ʱ��
	min =(int)(t % 60); t /= 60;//ȡ����ʱ��
	hour=(int)(t % 24);
	sprintf (pcTime, "%02d:%02d:%02d\n",hour, min, sec);//������
}

// �����Ƚ�
int dt_cmpSec(int sec)
{
	int ret = dt_secStartOfDay();
	ret = sec - ret;

	return ret;
}