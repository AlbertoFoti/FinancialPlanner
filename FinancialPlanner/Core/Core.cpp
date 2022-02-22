#include "Core.h"

Core::Core()
{
	this->back_end.init();

	double from = getUNIXtime(1, 2019);
	double to = getUNIXtime(1, 2022);
	this->NW_records = this->back_end.getNWdata(from, to);
}

float Core::CompoundInterestCalculate(float initialNW, float interestRate, float annualDeposits, int investmentYears, float* y_data)
{
	static int i = 0;

	if (investmentYears < 0 || interestRate < 0 || initialNW < 0) {
		return 0.00;
	}

	y_data[i] = initialNW;

	if (investmentYears > 0) {
		i++;
		float curr_nw = initialNW + annualDeposits + ((initialNW + annualDeposits) * (interestRate / 100));
		return this->CompoundInterestCalculate(curr_nw, interestRate, annualDeposits, investmentYears - 1, &y_data[0]);
	}
	else {
		i = 0;
		return initialNW;
	}

	return 0.00;
}

// Account =================================================================

std::vector<Account_p> Core::getAccounts()
{
	return this->back_end.getAccounts();
}

void Core::pushAccount(Account_p x)
{
	this->back_end.pushAccount(x);
}

// Net Worth ===============================================================

std::vector<NW_record_p> Core::getNWdata()
{
	return this->NW_records;
}

std::vector<NW_record_p> Core::getNWdataFromDb(double from, double to)
{
	this->NW_records = this->back_end.getNWdata(from, to);
	return this->NW_records;
}

std::string Core::testBackend() 
{
	return back_end.sayHello();
}
