#pragma once

#include "json/json.h"

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