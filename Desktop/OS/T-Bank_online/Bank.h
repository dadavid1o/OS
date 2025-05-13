#pragma once
#include <map>
#include <string>
#include <pthread.h>

struct Bank {
    std::map<std::string, int> accounts;
    pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
};

std::string BankExecute(Bank& bank, const std::string& command);