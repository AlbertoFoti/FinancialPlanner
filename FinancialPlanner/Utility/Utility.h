#pragma once

#include <iostream>
#include <time.h>
#include <string>

// Months Since Millenium (months since 1 jan 2000)
int getMSMtime(int month, int year);

int getMonthfromMSM(int msmDate);
int getYearfromMSM(int msmDate);

std::string getMonthfromMSM_s(int msmDate);
std::string getYearfromMSM_s(int msmDate);

// Seconds since Epoch
double getUNIXtime(int month, int year);

// Conversion between MSM and UNIX time
double fromMSMtoUNIXtime(int msmDate);