#ifndef __ACCOUNT_OPTIONS__

#include <stdio.h>
#include <stdlib.h>

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

void printAccountData(Account account);
void createNewAccount(void);
long getLastAccountId(void);
void printAccounts(void);
void printAccountHeader(void);
void searchByID(void);
void searchByString(int resultLength, Account *searchResult, char compareField[], char format[]);

#endif