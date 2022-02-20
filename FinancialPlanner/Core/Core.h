#pragma once

#include <iostream>
#include <string>
#include "Backend.h"

class Core {
	Backend back_end;
public:
	Core();

	// Financial Functions
	float CompoundInterestCalculate(float initialNW, float interestRate, float annualDeposits, int investmentYears, float* y_data);
	
	// Backend Functions
	std::vector<Account_p> getAccounts();
	std::string testBackend();
};