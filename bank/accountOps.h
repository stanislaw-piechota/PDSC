#ifndef __ACCOUNT_OPTIONS__

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "menu.h"

#define _STR(x) #x
#define STR(X) _STR(X)

#define INTEREST_RATE 0.05
#define MIN_STRING_LENGTH 2
#define NAME_LENGTH 21
#define SURNAME_LENGTH 31
#define ADDRESS_LENGTH 61
#define PESEL_LENGTH 12

#define SHORT_FIELD 20

typedef struct Account
{
    long accountId;
    char name[NAME_LENGTH];
    char surname[SURNAME_LENGTH];
    char address[ADDRESS_LENGTH];
    char pesel[PESEL_LENGTH];
    double balance;
    double loanBalance;
    double interest;
} Account;

void createNewAccount(void);
long getLastAccountId(void);
void makeDepositWithdraw(bool deposit);
void makeTransfer(void);
void takeLoan(void);
void payDebt(void);
void printAccounts(void);
bool getAccount(Account* account, const char prompt[]);
void changeBalance(Account *deposit, double *balance, double newBalance);
void searchByString(int resultLength, Account *searchResult, char compareField[], char format[]);
bool accountExists(Account *searchResult, long accountID);

#endif