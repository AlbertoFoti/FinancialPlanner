#include "Utility.h"

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

double fromMSMtoUNIXtime(int msmDate)
{
	int month = getMonthfromMSM(msmDate);
	int year = getYearfromMSM(msmDate);

	return getUNIXtime(month, year);
}

Json::Value BubbleSortTransactions(Json::Value root)
{

	bool swapped = true;
	for (int i = 0; i != root["records"].size(); i++) {
		swapped = true;
		while (swapped) {
			swapped = false;
			for (int j = 0; j < root["records"][i]["data"].size() - 1; j++) {
				if (root["records"][i]["data"][j]["Day"] > root["records"][i]["data"][j + 1]["Day"]) {
					root = swapBubbleSortTransaction(root, i, j, j + 1);
					swapped = true;
				}
			}
		}
	}
	return root;
}

Json::Value swapBubbleSortTransaction(Json::Value root, int i, int j, int k) {
	Json::Value root2 = root["records"][i]["data"];
	Json::Value tmp = root2[j];
	root2[j] = root2[k];
	root2[k] = tmp;
	root["records"][i]["data"] = root2;

	return root;
}
