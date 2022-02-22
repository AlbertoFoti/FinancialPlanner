#pragma once

#include <iostream>
#include <string>
#include "Backend.h"
#include "../Utility/Utility.h"

class Core {
	Backend back_end;

	std::vector<NW_record_p> NW_records;
public:
	Core();

	// Financial Functions =======
	float CompoundInterestCalculate(float initialNW, float interestRate, float annualDeposits, int investmentYears, float* y_data);
	
	// Backend Functions =========

	// Accounts
	std::vector<Account_p> getAccounts();
	void pushAccount(Account_p x);
	// Net Worth (NW)
	std::vector<NW_record_p> getNWdata();
	std::vector<NW_record_p> getNWdataFromDb(double from, double to);

	// Testing
	std::string testBackend();
};