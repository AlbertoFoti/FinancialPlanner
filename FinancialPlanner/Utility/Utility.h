#pragma once

#include <iostream>
#include <time.h>
#include <string>
#include "json/json.h"

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
 * @brief Bubble Sort for Transactions
 * 
 * @param root Json::Value root not sorted
 * @return Json::Value json root sorted
 */
Json::Value BubbleSortTransactions(Json::Value root);

/**
 * @brief Swap Function for bubble sort (of transactions)
 * 
 * @param root Json::Value root not sorted
 * @param i 
 * @param j 
 * @param k 
 * @return Json::Value 
 */
Json::Value swapBubbleSortTransaction(Json::Value root, int i, int j, int k);
Json::Value BubbleSortAccountDetails(Json::Value root);
Json::Value swapBubbleSortAccountDetails(Json::Value root, int i, int j);
Json::Value BubbleSortNetWorth(Json::Value root);
Json::Value swapBubbleSortNetWorth(Json::Value root, int i, int j);