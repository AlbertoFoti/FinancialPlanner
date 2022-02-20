#pragma once

#include <iostream>
#include <string>
#include <cstdlib>
#include <fstream>
#include "json/json.h"

struct Account {
	int id;
	std::string name;
	float AmountStored;
};
using Account_p = Account*;

class Backend {
public:
	void init();

	// Accounts
	std::vector<Account_p> getAccounts();

	// Testing
	std::string sayHello();
};