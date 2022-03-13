#pragma once

#include <iostream>
#include <time.h>
#include <string>
#include "imgui.h"
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
 * @return Json::Value root sorted
 */
Json::Value BubbleSortTransactions(Json::Value root);

/**
 * @brief Swap Function for bubble sort (of transactions)
 * 
 * @param root Json::Value root not sorted
 * @param i index of specific Month/Year records
 * @param j Record 1
 * @param k Record 2
 * @return Json::Value root records swapped 
 */
Json::Value swapBubbleSortTransaction(Json::Value root, int i, int j, int k);

/**
 * @brief Bubble Sort for Accounts
 * 
 * @param root Json::Value root not sorted
 * @return Json::Value root sorted
 */
Json::Value BubbleSortAccountDetails(Json::Value root);

/**
 * @brief Swap Function for bubble sort (of accounts)
 * 
 * @param root Json::Value root not sorted
 * @param i Account 1
 * @param j Account 2
 * @return Json::Value root accounts swapped
 */
Json::Value swapBubbleSortAccountDetails(Json::Value root, int i, int j);

/**
 * @brief Bubble Sort for New Worth
 * 
 * @param root Json::Value root not sorted
 * @return Json::Value root sorted
 */
Json::Value BubbleSortNetWorth(Json::Value root);

/**
 * @brief Swap Function for bubble sort (of accounts)
 * 
 * @param root Json::Value root not sorted
 * @param i Record 1
 * @param j Record 2
 * @return Json::Value root net worth records swapped
 */
Json::Value swapBubbleSortNetWorth(Json::Value root, int i, int j);

struct Date_format {
    int Day;
    int Month;
    int Year;
};
using Date_format_p = std::shared_ptr<Date_format>;

/**
 * @brief Calendar Selection Widget
 * 
 * @return Date_format_p 
 */
Date_format_p calendarSelection();