#ifndef __ACCOUNT_OPTIONS__

#include <stdio.h>
#include <stdlib.h>

#define _STR(x) #x
#define STR(X) _STR(X)

#define INTEREST_RATE 0.05
#define MIN_STRING_LENGTH 2
#define NAME_LENGTH 21
#define SURNAME_LENGTH 51
#define ADDRESS_LENGTH 101
#define PESEL_LENGTH 12

typedef struct Account
{
    int accountId;
    char name[NAME_LENGTH];
    char surname[SURNAME_LENGTH];
    char address[ADDRESS_LENGTH];
    char pesel[PESEL_LENGTH];
    double balance;
    double loanBalance;
    double interest;
} Account;

void printAccountData(Account account);
void createNewAccount(void);
int getLastAccountId();

#endif