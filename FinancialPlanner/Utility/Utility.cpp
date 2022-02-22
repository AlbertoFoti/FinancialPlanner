#include "Utility.h"

// from double (months since Jan 2000) to UNIX_time (seconds since Epoch)
double getUNIXtime(int month, int year)
{
	time_t rawtime;
	struct tm* timeinfo;

	/* get current timeinfo: */
	time(&rawtime); //or: rawtime = time(0);
	/* convert to struct: */
	timeinfo = localtime(&rawtime);

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