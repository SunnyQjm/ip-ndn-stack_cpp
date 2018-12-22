/*
 * ttime.c
 *
 *  Created on: Mar 9, 2018
 *      Author: root
 */
#include "ttime.h"
#include "packet.h"

void time_val_to_Time(struct timeval timess,formal_time_t * timed)
{
	struct tm *timetemp;
	time_t timet=timess.tv_sec;
	timetemp=localtime(&timet);
	timed->year=timetemp->tm_year+1900;
	timed->month=timetemp->tm_mon+1;
	timed->day=timetemp->tm_mday;
	timed->minute=timetemp->tm_min;
	timed->sec=timetemp->tm_sec;
	timed->hour=timetemp->tm_hour;
	timed->m_sec=timess.tv_usec/1000;
}//timeval to Time

void double_time_to_Time(double times,formal_time_t * timed)
{
	struct timeval temp;
	dbl2time(times,&temp);
	time_val_to_Time(temp,timed);
}

double Time_to_double(formal_time_t  timed)
{
	struct tm tm1;
	time_t t1;
	double tm2;
	tm1.tm_year=timed.year;
	tm1.tm_mon=timed.month;
	tm1.tm_mday=timed.day;
	tm1.tm_hour=timed.hour;
	tm1.tm_min=timed.minute;
	tm1.tm_sec=timed.sec;
	t1=mktime(&tm1);
	tm2=(double)(t1)+(double)(timed.m_sec)/1000.0;
	return tm2;
}
