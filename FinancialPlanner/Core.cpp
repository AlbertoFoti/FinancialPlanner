#include "Core.h"

double Core::CompoundInterestCalculate(double initialNW, double interestRate, double annualDeposits, int investmentYears, float* y_data)
{
	static int i = 0;

	if (investmentYears < 0 || interestRate < 0 || initialNW < 0) {
		return 0.00;
	}

	y_data[i] = initialNW;

	if (investmentYears > 0) {
		i++;
		double curr_nw = initialNW + annualDeposits + ((initialNW + annualDeposits) * (interestRate / 100));
		return this->CompoundInterestCalculate(curr_nw, interestRate, annualDeposits, investmentYears - 1, &y_data[0]);
	}
	else {
		i = 0;
		return initialNW;
	}

	return 0.00;
}
