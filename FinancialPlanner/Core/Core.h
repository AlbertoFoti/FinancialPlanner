#pragma once

#include <iostream>
#include <string>
#include "Backend.h"

class Core {
	Backend back_end;
public:
	Core();
	double CompoundInterestCalculate(double initialNW, double interestRate, double annualDeposits, int investmentYears, float* y_data);
	
	void testBackend();
};