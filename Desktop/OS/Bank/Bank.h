#pragma once
#include <string>

struct BankCell {
    int balance;
    int min_balance;
    int max_balance;
    bool frozen;
};

struct Bank {
    int size;
    BankCell cells[1]; 
};

bool is_valid_account(const Bank* bank, int account);

void show_balance(const Bank* bank, int account);
void show_min_balance(const Bank* bank, int account);
void show_max_balance(const Bank* bank, int account);

bool process_command(Bank* bank, const std::string& cmd, std::ostream& out);
