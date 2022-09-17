#include "CustomTime.h"

#include <time.h>

int getMSMtime(int month, int year)
{
	if (year >= 2000) {
		return (year - 2000) * 12 + month;
	}
	return 0;
}

int getMonthfromMSM(int msmDate)
{
	return ((msmDate % 12)+1);
}

int getYearfromMSM(int msmDate)
{
	return ((msmDate / 12) + 2000);
}

std::string getMonthfromMSM_s(int msmDate)
{
	return std::to_string((msmDate % 12)+1);
}

std::string getYearfromMSM_s(int msmDate)
{
	return std::to_string((msmDate / 12)+2000);
}

double getUNIXtime(int month, int year)
{
	/* get current timeinfo: */
	const time_t rawtime = time(0);
	/* convert to struct: */
    tm* timeinfo = localtime(&rawtime);

	/* now modify the timeinfo to the given date: */
	timeinfo->tm_year = year - 1900;
	timeinfo->tm_mon = month - 1;    //months since January - [0,11]
	timeinfo->tm_mday = 1;          //day of the month - [1,31]
	timeinfo->tm_hour = 0;         //hours since midnight - [0,23]
	timeinfo->tm_min = 0;          //minutes after the hour - [0,59]
	timeinfo->tm_sec = 0;          //seconds after the minute - [0,59]

	/* call mktime: create unix time stamp from timeinfo struct */
	return (double)mktime(timeinfo);
}

double fromMSMtoUNIXtime(int msmDate)
{
	int month = getMonthfromMSM(msmDate);
	int year = getYearfromMSM(msmDate);

	return getUNIXtime(month, year);
}

int getCurrentYear()
{
	/* get current timeinfo: */
    const time_t rawtime = time(0);
	/* convert to struct: */
	tm* timeinfo = localtime(&rawtime);
	/* now modify the timeinfo to the given date: */
	timeinfo->tm_year += 1900;
	return timeinfo->tm_year;
}