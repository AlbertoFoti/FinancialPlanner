#pragma once

// Transactions
struct Transaction {
    int day;
    std::string category {};
    std::string sub_category {};
    std::string type {};
    int account_id;
    int sub_account_id;
    int account_to;
    int sub_account_id_to;
    double amount;
    std::string comment {};

    Transaction() {

    }

    Transaction(int _day, std::string _cat, std::string _sub_cat, std::string _type, int _acc_id, int _sub_acc_id, int _acc_to, int _sub_acc_to, double _amount, std::string _comment) {
        this->day = _day;
        this->category = _cat;
        this->sub_category = _sub_cat;
        this->type = _type;
        this->account_id = _acc_id;
        this->sub_account_id = _sub_acc_id;
        this->account_to = _acc_to;
        this->sub_account_id_to = _sub_acc_to;
        this->amount = _amount;
        this->comment = _comment;
    }
};
using Transaction_p = std::shared_ptr<Transaction>;

// Monthly Transactions Summary
struct MonthlyTransactions {
    int month;
    int year;
    std::vector<Transaction_p> transactions {};

    MonthlyTransactions(int _month, int _year) {
        this->month = _month;
        this->year = _year;
        this->transactions = std::vector<Transaction_p>({});
    }

    void set_month_and_year(int _month, int _year) {
        this->month = _month;
        this->year = _year;
    }
};
using MonthlyTransactions_p = std::shared_ptr<MonthlyTransactions>;