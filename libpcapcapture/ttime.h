/*
 * ttime.h
 *
 *  Created on: Mar 9, 2018
 *      Author: root
 */
#include <ctime>

#ifndef TTIME_H_
#define TTIME_H_

typedef struct Time
{
	int year;
	int month;
	int day;
	int hour;
	int minute;
	int sec;
	int m_sec;
}formal_time_t;

double Time_to_double(formal_time_t  timed);

void time_val_to_Time(struct timeval times,formal_time_t * timed);

void double_time_to_Time(double times,formal_time_t * timed);

#endif /* TTIME_H_ */


