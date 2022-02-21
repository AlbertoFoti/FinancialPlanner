#include "Core.h"
#include "Core.h"

Core::Core()
{
	this->back_end.init();
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

std::vector<Account_p> Core::getAccounts()
{
	return this->back_end.getAccounts();
}

void Core::pushAccount(Account_p x)
{
	this->back_end.pushAccount(x);
}

std::string Core::testBackend() 
{
	return back_end.sayHello();
}
