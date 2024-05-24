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
    Account newAccount = {getLastAccountId() + 1, "", "", "", "", 0.0, 0.0, INTEREST_RATE};

    getString(newAccount.name, NAME_LENGTH, "Name", "%[a-zA-Z]s", MIN_STRING_LENGTH);
    getString(newAccount.surname, SURNAME_LENGTH, "Surname", "%[a-zA-Z-]s", MIN_STRING_LENGTH);
    getString(newAccount.address, ADDRESS_LENGTH, "Address", "%[a-zA-Z0-9,.- /]s", MIN_STRING_LENGTH);
    getString(newAccount.pesel, PESEL_LENGTH, "ID number (PESEL)", "%[0-9]s", PESEL_LENGTH - 1);
    getDouble(&newAccount.balance, "What's the balance of your account");

    return newAccount;
}

void createNewAccount()
{
    Account newAccount = getAccountData();
    
    printf("\n");
    printAccountHeader();
    printAccountData(&newAccount);
    printf("\n");
    if (getConfirmation())
    {
        saveAccountData(&newAccount);
        printf("Account saved successfully with ID: %ld\n", newAccount.accountId);
    } else {
        printf("Something went wrong while saving the account\n");
    }
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
            printAccountData(&currentAccount);
    } while (!feof(database));

    disconnectFromDatabase(&database);
}

void searchByString(int resultLength, Account *searchResult, char compareField[], char format[]){
    FILE *database;
    char searchField[resultLength];
    long id=0, results = 0;

    getString(searchField, resultLength, "Value to search for", format, 1);
    connectToDatabse(&database);
    do{
        if (readAccount(database, searchResult, id++) && !strcmp(compareField, searchField))
        {
            if (!results){
                system("clear");
                printAccountHeader();
            }
            printAccountData(searchResult);
            results++;
        }
    } while (!feof(database));
    if (!results)
        printf("Account with value %s not found\n", searchField);
    disconnectFromDatabase(&database);
}

void changeBalance(Account *depositAccount, double *balance, double newBalance){
    FILE *database;
    
    connectToDatabse(&database);
    *balance = newBalance;
    saveAccountData(depositAccount);
    disconnectFromDatabase(&database);
}

bool accountExists(Account *searchResult, long accountID){
    bool result = false;
    FILE *database;

    connectToDatabse(&database);
    if (readAccount(database, searchResult, accountID))
        result = true;
    disconnectFromDatabase(&database);
    return result;
}

bool getAccount(Account* account, const char prompt[]){
    long accountID;
    char message[50] = "\nID of an account to ";
    getLong(&accountID, strcat(message, prompt));
    if (!accountExists(account, accountID))
    {
        printf("Account with ID %ld does not exist\n", accountID);
        return false;
    }
    printf("\n");
    printAccountHeader();
    printAccountData(account);
    return true;
}

double getRightAmount(double field, bool deposit){
    double amount;

    getDouble(&amount, "\nSelect an amount to proceed with");
    if (deposit && (field + amount > MAX_DOUBLE_VALUE)){
        printf("Your updated balance would exceed the limit. Aborting...\n");
        return -1;
    }
    if (!deposit && (field - amount < 0))
    {
        printf("You dont have enough funds to withdraw\n");
        return -1;
    }
    return field + (deposit ? amount : -amount);
}

void makeDepositWithdraw(bool deposit)
{
    double newBalance;
    Account operationAccount;

    if (!getAccount(&operationAccount, (deposit ? "make a deposit" : "withdraw from")))
        return;
    if ((newBalance = getRightAmount(operationAccount.balance, deposit)) < 0)
        return;
    
    printf("Your new balance would be %." STR(MANTISSA_LENGTH) "lf\n", newBalance);
    if (getConfirmation()){
        printf("The operation was successful\n");
        changeBalance(&operationAccount, &operationAccount.balance, newBalance);
    }
}

void makeTransfer(){
    double newWithdraw, newDeposit;
    Account withdrawAccount, depositAccount;

    if (!getAccount(&withdrawAccount, "widthdraw from"))
        return;
    if (!getAccount(&depositAccount, "make a deposit"))
        return;
    if ((newWithdraw = getRightAmount(withdrawAccount.balance, false)) < 0)
        return;
    if ((newDeposit = depositAccount.balance + (withdrawAccount.balance - newWithdraw)) > MAX_DOUBLE_VALUE){
        printf("Your updated balance would exceed the limit. Aborting...\n");
        return;
    }

    if (getConfirmation())
    {
        printf("Your transferred from was successful \n");
        changeBalance(&withdrawAccount, &withdrawAccount.balance, newWithdraw);
        changeBalance(&depositAccount, &depositAccount.balance, newDeposit);
    }
}

void takeLoan(){
    double newLoan, newBalance;
    Account operationAccount;

    if (!getAccount(&operationAccount, "take a loan"))
        return;
    
    if ((newLoan = getRightAmount(operationAccount.loanBalance, true)) < 0){
        printf("Your loan would exceed the limit after this operation. Aborting...\n");
        return;
    }

    if ((newBalance = operationAccount.balance + newLoan - operationAccount.loanBalance) > MAX_DOUBLE_VALUE)
    {
        printf("Your main balance would exceed the limit. Aborting...\n");
        return;
    }

    printf("Your main balance would be %." STR(MANTISSA_LENGTH) "lf\n", newBalance);
    printf("Your loan balance would be %." STR(MANTISSA_LENGTH) "lf\n", newLoan);
    if(getConfirmation()){
        changeBalance(&operationAccount, &operationAccount.loanBalance, newLoan);
        changeBalance(&operationAccount, &operationAccount.balance, newBalance);
        printf("Loan has been taken successfully\n");
    };
}

void payDebt()
{
    double amount, realAmount, newBalance, newLoan;
    Account operationAccount;

    if (!getAccount(&operationAccount, "pay off debt"))
        return;
    if (!operationAccount.loanBalance){
        printf("\nThis account has no debt to pay off. Aborting...\n");
        return;
    }

    getDouble(&amount, "\nSelect an amount to proceed with");
    realAmount = amount * (1 - operationAccount.interest);
    printf("\nDue to %.0lf%% interest rate your %.2lf charge will pay off only %.2f of your debt\n", 
           operationAccount.interest*100, amount, realAmount);

    if ((newBalance = operationAccount.balance - amount) < 0)
    {
        printf("You dont have enough money for this amount. Aborting...");
        return;
    }
    if ((newLoan = operationAccount.loanBalance - realAmount) < 0){
        printf("You will overpay %.2lf, this money cannot be received back\n", fabs(newLoan));
        newLoan = 0;
    }

    printf("Your main balance would be %." STR(MANTISSA_LENGTH) "lf\n", newBalance);
    printf("Your loan balance would be %." STR(MANTISSA_LENGTH) "lf\n", newLoan);
    if (getConfirmation())
    {
        changeBalance(&operationAccount, &operationAccount.loanBalance, newLoan);
        changeBalance(&operationAccount, &operationAccount.balance, newBalance);
        printf("Part of the debt has been paid off successfully");
    };
}