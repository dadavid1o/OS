#include <iostream>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <sstream>
#include <string>
#include <climits>

struct BankCell {
    int balance = 0;
    int min_balance = 0;
    int max_balance = INT_MAX;
    bool frozen = false;
};

struct Bank {
    int size;
    BankCell cells[1];
};

bool is_valid_account(const Bank* bank, int account) {
    return account >= 0 && account < bank->size;
}

void show_balance(const Bank* bank, int account) {
    std::cout << "Account " << account << " balance: " 
              << bank->cells[account].balance << " units\n";
}

void show_min_balance(const Bank* bank, int account) {
    std::cout << "Account " << account << " minimum balance: " 
              << bank->cells[account].min_balance << " units\n";
}

void show_max_balance(const Bank* bank, int account) {
    std::cout << "Account " << account << " maximum balance: " 
              << bank->cells[account].max_balance << " units\n";
}

void process_command(Bank* bank, const std::string& cmd) {
    std::istringstream iss(cmd);
    std::string action;
    iss >> action;

    try {
        if (action == "balance") {
            int account;
            iss >> account;
            if (!is_valid_account(bank, account)) {
                std::cout << "Sorry, account " << account << " doesn't exist\n";
            } else {
                show_balance(bank, account);
            }
        }
        else if (action == "min_balance") {
            int account;
            iss >> account;
            if (!is_valid_account(bank, account)) {
                std::cout << "Sorry, account " << account << " doesn't exist\n";
            } else {
                show_min_balance(bank, account);
            }
        }
        else if (action == "max_balance") {
            int account;
            iss >> account;
            if (!is_valid_account(bank, account)) {
                std::cout << "Sorry, account " << account << " doesn't exist\n";
            } else {
                show_max_balance(bank, account);
            }
        }
        else if (action == "transfer") {
            int from, to, amount;
            iss >> from >> to >> amount;
            
            if (!is_valid_account(bank, from) || !is_valid_account(bank, to)) {
                std::cout << "Sorry, one of the specified accounts doesn't exist\n";
            } 
            else if (bank->cells[from].frozen) {
                std::cout << "Sorry, the sender account is frozen\n";
            }
            else if (bank->cells[to].frozen) {
                std::cout << "Sorry, the recipient account is frozen\n";
            }
            else if (amount <= 0) {
                std::cout << "Sorry, the transfer amount must be positive\n";
            }
            else if (bank->cells[from].balance - amount < bank->cells[from].min_balance) {
                std::cout << "Sorry, insufficient funds in the sender account\n";
            }
            else if (bank->cells[to].balance + amount > bank->cells[to].max_balance) {
                std::cout << "Sorry, the transfer would exceed the recipient's maximum balance\n";
            }
            else {
                bank->cells[from].balance -= amount;
                bank->cells[to].balance += amount;
                std::cout << "Successfully transferred " << amount << " units from account " 
                          << from << " to account " << to << "\n";
            }
        }
        else if (action == "freeze") {
            int account;
            iss >> account;
            if (!is_valid_account(bank, account)) {
                std::cout << "Sorry, account " << account << " doesn't exist\n";
            } else {
                bank->cells[account].frozen = true;
                std::cout << "Account " << account << " has been frozen\n";
            }
        }
        else if (action == "unfreeze") {
            int account;
            iss >> account;
            if (!is_valid_account(bank, account)) {
                std::cout << "Sorry, account " << account << " doesn't exist\n";
            } else {
                bank->cells[account].frozen = false;
                std::cout << "Account " << account << " has been unfrozen\n";
            }
        }
        else if (action == "set_min") {
            int account, value;
            iss >> account >> value;
            if (!is_valid_account(bank, account)) {
                std::cout << "Sorry, account " << account << " doesn't exist\n";
            } else if (value > bank->cells[account].max_balance) {
                std::cout << "Sorry, minimum balance cannot exceed maximum balance\n";
            } else {
                bank->cells[account].min_balance = value;
                std::cout << "Account " << account << " minimum balance set to: " 
                          << value << " units\n";
            }
        }
        else if (action == "set_max") {
            int account, value;
            iss >> account >> value;
            if (!is_valid_account(bank, account)) {
                std::cout << "Sorry, account " << account << " doesn't exist\n";
            } else if (value < bank->cells[account].min_balance) {
                std::cout << "Sorry, maximum balance cannot be less than minimum balance\n";
            } else {
                bank->cells[account].max_balance = value;
                std::cout << "Account " << account << " maximum balance set to: " 
                          << value << " units\n";
            }
        }
        else if (action == "deposit_all") {
            int amount;
            iss >> amount;
            if (amount <= 0) {
                std::cout << "Sorry, amount must be positive\n";
                return;
            }
            for (int i = 0; i < bank->size; ++i) {
                if (!bank->cells[i].frozen) {
                    bank->cells[i].balance += amount;
                }
            }
            std::cout << "Successfully deposited " << amount << " units to all accounts\n";
        }
        else if (action == "withdraw_all") {
            int amount;
            iss >> amount;
            if (amount <= 0) {
                std::cout << "Sorry, amount must be positive\n";
                return;
            }
            bool can_withdraw = true;
            for (int i = 0; i < bank->size; ++i) {
                if (!bank->cells[i].frozen && 
                    (bank->cells[i].balance - amount < bank->cells[i].min_balance)) {
                    can_withdraw = false;
                    break;
                }
            }
            if (can_withdraw) {
                for (int i = 0; i < bank->size; ++i) {
                    if (!bank->cells[i].frozen) {
                        bank->cells[i].balance -= amount;
                    }
                }
                std::cout << "Successfully withdrew " << amount << " units from all accounts\n";
            } else {
                std::cout << "Sorry, the operation would make some account balances negative\n";
            }
        }
        else if (action == "help") {
            std::cout << "\nAvailable commands:\n"
                      << "  balance <account>         - show current balance\n"
                      << "  min_balance <account>     - show minimum balance\n"
                      << "  max_balance <account>     - show maximum balance\n"
                      << "  transfer <from> <to> <amount> - transfer funds\n"
                      << "  freeze <account>          - freeze account\n"
                      << "  unfreeze <account>        - unfreeze account\n"
                      << "  set_min <account> <X>     - set minimum balance\n"
                      << "  set_max <account> <X>     - set maximum balance\n"
                      << "  deposit_all <amount>      - deposit to all accounts\n"
                      << "  withdraw_all <amount>     - withdraw from all accounts\n"
                      << "  help                      - show this help\n"
                      << "  exit                      - exit the program\n\n";
        }
        else if (action != "exit") {
            std::cout << "Sorry, command not recognized. Type 'help' for the command list\n";
        }
    } catch (...) {
        std::cout << "Sorry, an error occurred while processing the command. Please check your input\n";
    }
}
