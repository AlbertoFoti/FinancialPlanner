#pragma once

#include <string>

/**
 * @brief Months Since Millenium (months since 1 jan 2000)
 * 
 * @param month Current Month
 * @param year Current Year
 * @return int Months since Millenium
 */
int getMSMtime(int month, int year);

/**
 * @brief Get the Month from MSM time
 * 
 * @param msmDate Months Since Millenium Date
 * @return int Month
 */
int getMonthfromMSM(int msmDate);

/**
 * @brief Get the Year from MSM time
 * 
 * @param msmDate Months Since Millenium Date
 * @return int Year
 */
int getYearfromMSM(int msmDate);

/**
 * @brief Get the Month from MSM time as a string
 * 
 * @param msmDate Months Since Millenium Date
 * @return std::string Month 
 */
std::string getMonthfromMSM_s(int msmDate);

/**
 * @brief Get the Year from MSM time as a string
 * 
 * @param msmDate Months Since Millenium Date
 * @return std::string Year 
 */
std::string getYearfromMSM_s(int msmDate);

/**
 * @brief Get Seconds Since Epoch (UNIX time)
 * 
 * @param month Current Month
 * @param year Current Year
 * @return double UNIX time 
 */
double getUNIXtime(int month, int year);

/**
 * @brief MSM (months since millenium) to UNIX (seconds since epoch)
 * 
 * @param msmDate MSM time
 * @return double UNIX time 
 */
double fromMSMtoUNIXtime(int msmDate);

/**
 * @brief Get the Current Year
 * 
 * @return int current year
 */
int getCurrentYear();