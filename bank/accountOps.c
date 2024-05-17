#include "accountOps.h"

int getLastAccountId()
{
    FILE *database;
    Account tempAccount;
    int highestId = -1, line = 0;

    if ((database = fopen("database.dat", "rb")) == NULL)
    {
        perror("Couldn\'t connect to the database");
        exit(1);
    }

    do
    {
        if (fseek(database, (line++) * sizeof(Account), SEEK_SET))
        {
            perror("Error while looking for account");
            exit(3);
        }

        if (fread(&tempAccount, sizeof(Account), 1, database) == 1)
        {
            if (tempAccount.accountId > highestId)
                highestId = tempAccount.accountId;
            printf("Account ID: %d\n", tempAccount.accountId);
        }

    } while (!feof(database));

    if (fclose(database))
    {
        perror("Error closing the database");
        exit(2);
    }

    return highestId;
}

void createNewAccount()
{
    Account newAccount = {getLastAccountId() + 1, "", "", "", "", 0.0, 0.0, 0.0};

    getString(newAccount.name, NAME_LENGTH, "Name", "%[a-zA-Z]s", MIN_STRING_LENGTH);
    getString(newAccount.surname, SURNAME_LENGTH, "Surname", "%[a-zA-Z-]s", MIN_STRING_LENGTH);
    getString(newAccount.address, ADDRESS_LENGTH, "Address", "%[a-zA-Z0-9,.- /]s", MIN_STRING_LENGTH);
    getString(newAccount.pesel, PESEL_LENGTH, "Nr identyfikacyjny (PESEL)", "%[0-9]s", PESEL_LENGTH-1);
    getDouble(&newAccount.balance, "What's the balance of your account");
    newAccount.loanBalance = 0;
    newAccount.interest = INTEREST_RATE;
    
    printAccountData(newAccount);
}

void printAccountData(Account account){
    printf("\nAccount ID: %d\n", account.accountId);
    printf("Name: %s\n", account.name);
    printf("Surname: %s\n", account.surname);
    printf("Address: %s\n", account.address);
    printf("PESEL: %s\n", account.pesel);
    printf("Balance: %" STR(MAX_DOUBLE_EXPONENT) "." STR(MANTISSA_LENGTH) "f\n", account.balance);
    printf("Loan balance: %" STR(MAX_DOUBLE_EXPONENT) "." STR(MANTISSA_LENGTH) "f\n", account.loanBalance);
    printf("Interest for this year: %" STR(MAX_DOUBLE_EXPONENT) "." STR(MANTISSA_LENGTH) "f\n\n", account.interest);
}