#include <iostream>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <climits>
#include <unistd.h>
#include <sstream>
#include <string>
struct BankCell {
    int balance = 0;
    int min_balance = 0;
    int max_balance = INT_MAX;
    bool frozen = false;
};
struct Bank {
    int size;
    BankCell cels[1];
};

bool is_valid_account(const Bank* bank, int account) {
    return account >= 0 && account < bank->size;
}

void show_balance(const Bank* bank, int account) {
    std::cout << "Account " << account << " balance: "
              << bank->cels[account].balance;
}

void show_min_balance(const Bank* bank, int account) {
    std::cout << "Account: " << account << " min_balance: "
              << bank->cels[account].min_balance;
}
void show_max_balance(const Bank* bank, int account) {
    std::cout << "Account: " << account << " min_balance: "
              << bank->cels[account].max_balance;
}





int main()
{




    return 0;
}