#include "accountOps.h"
#include "fileOps.h"
#include "inputOps.h"

long getLastAccountId()
{
    FILE *database;
    Account tempAccount;
    long highestId = -1, line = 0;

    connectToDatabse(&database);
    do
    {
        if (readAccount(database, &tempAccount, line++) && (tempAccount.accountId > highestId))
            highestId = tempAccount.accountId;

    } while (!feof(database));

    disconnectFromDatabase(&database);
    return highestId;
}

Account getAccountData()
{
    Account newAccount = {getLastAccountId() + 1, "", "", "", "", 0.0, 0.0, 0.0};

    getString(newAccount.name, NAME_LENGTH, "Name", "%[a-zA-Z]s", MIN_STRING_LENGTH);
    getString(newAccount.surname, SURNAME_LENGTH, "Surname", "%[a-zA-Z-]s", MIN_STRING_LENGTH);
    getString(newAccount.address, ADDRESS_LENGTH, "Address", "%[a-zA-Z0-9,.- /]s", MIN_STRING_LENGTH);
    getString(newAccount.pesel, PESEL_LENGTH, "ID number (PESEL)", "%[0-9]s", PESEL_LENGTH - 1);
    getDouble(&newAccount.balance, "What's the balance of your account");
    newAccount.loanBalance = 0;
    newAccount.interest = INTEREST_RATE;

    return newAccount;
}

void createNewAccount()
{
    Account newAccount = getAccountData();
    
    printf("\n");
    printAccountHeader();
    printAccountData(newAccount);
    printf("\n");
    if (getConfirmation())
    {
        saveAccountData(&newAccount);
        printf("Account saved successfully with ID: %ld\n", newAccount.accountId);
    }
}

void printAccountData(Account account)
{   
    printf("%-" STR(SHORT_FIELD) "ld|", account.accountId);
    printf("%-" STR(SHORT_FIELD) "." STR(SHORT_FIELD) "s|", account.name);
    printf("%-" STR(SHORT_FIELD) "." STR(SHORT_FIELD) "s|", account.surname);
    printf("%-" STR(SHORT_FIELD) "." STR(SHORT_FIELD) "s|", account.address);
    printf("%-" STR(SHORT_FIELD) "." STR(SHORT_FIELD) "s|", account.pesel);
    printf("%-" STR(SHORT_FIELD) "." STR(MANTISSA_LENGTH) "lf|", account.balance);
    printf("%-" STR(SHORT_FIELD) "." STR(MANTISSA_LENGTH) "lf|", account.loanBalance);
    printf("%-.2lf\n", account.interest);
}

void printAccountHeader(){
    printf("ID%*c|Name%*c|Surname%*c|Address%*c|PESEL%*c|Balance%*c|Loan%*c|Interest\n",
           SHORT_FIELD - 2, ' ', SHORT_FIELD - 4, ' ', SHORT_FIELD - 7, ' ', SHORT_FIELD - 7, ' ', SHORT_FIELD - 5, ' ',
           SHORT_FIELD - 7, ' ', SHORT_FIELD - 4, ' ');
    for (int i = 0; i < 8 * SHORT_FIELD; i++)
        printf("-");
    printf("\n");
}

void printAccounts()
{
    FILE *database;
    Account currentAccount;
    long line = 0;

    connectToDatabse(&database);
    printAccountHeader();

    do
    {
        if (readAccount(database, &currentAccount, line++))
            printAccountData(currentAccount);
    } while (!feof(database));

    disconnectFromDatabase(&database);
}