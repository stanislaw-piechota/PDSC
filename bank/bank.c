#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include "messages.inl"

#define MAX_DOUBLE_EXPONENT 10
#define MAX_DOUBLE_VALUE (pow(10, MAX_DOUBLE_EXPONENT + 1) - 1)
#define MANTISSA_LENGTH 2
#define DOUBLE_LENGTH (MAX_DOUBLE_EXPONENT + MANTISSA_LENGTH + 1 + 2)

#define INTEREST_RATE 0.05
#define MIN_STRING_LENGTH 2
#define NAME_LENGTH 21
#define SURNAME_LENGTH 51
#define ADDRESS_LENGTH 101
#define PESEL_LENGTH 12

#define _STR(x) #x
#define STR(X) _STR(X)

short selectedOption = 0;

typedef struct
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

void printWelcomeMessage(void);
void printMenu(void);
void printAccountData(Account account);
bool changeOption(void);
bool createNewAccount(void);
int getLastAccountId();
void getString(char stringPtr[], int length, char *prompt, char* format, int minLength);

int main(int argc, char const *argv[])
{
    printWelcomeMessage();

    // while (true) {
    //     printMenu();
    //     if(changeOption()){
    //     } else {
    //         printf("Couldnt execute any operation\n");
    //     }

    //     if (selectedOption == OPERATIONS - 1)
    //         exit(0);
    // }

    createNewAccount();

    return 0;
}

int getLastAccountId()
{
    FILE *database;
    Account tempAccount;
    int highestId = -1, line = 0;

    if ((database = fopen("database.dat", "rb")) == NULL)
    {
        perror("Couldnt connect to the database");
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

void clearBuffer(char string[]){
    while (*string) string++;
    if (*(string - 1) != '\n')
        while (getchar() != '\n');
}

void trimTrailingWhitespaces(char string[]){
    while (*string) string++;
    string--; // \n
    while (isspace(*(--string))){
        *string = '\n';
        *(string+1) = '\0';
    }
}

void getString(char stringPtr[], int length, char *prompt, char* format, int minLength)
{
    int result;
    char buffer[length];
    do {
        printf("%s (%d-%d characters): ", prompt, minLength, length-1);
        fgets(buffer, length, stdin);
        clearBuffer(buffer);
        trimTrailingWhitespaces(buffer);
        result = sscanf(buffer, format, stringPtr);
    }
    while (!result || (strlen(stringPtr) < strlen(buffer)-1) || (strlen(stringPtr) <= minLength - 1));
}

double getDouble(char *prompt){
    char number[DOUBLE_LENGTH];
    double result;
    do {
        getString(number, DOUBLE_LENGTH, prompt, "%[0-9.+-]s", 1);
        result = strtod(number, NULL);
    } while(result < 0);
    return result;
}

bool createNewAccount()
{
    Account newAccount = {getLastAccountId() + 1, "", "", "", "", 0.0, 0.0, 0.0};

    getString(newAccount.name, NAME_LENGTH, "Name", "%[a-zA-Z]s", MIN_STRING_LENGTH);
    getString(newAccount.surname, SURNAME_LENGTH, "Surname", "%[a-zA-Z-]s", MIN_STRING_LENGTH);
    getString(newAccount.address, ADDRESS_LENGTH, "Address", "%[a-zA-Z0-9,.- /]s", MIN_STRING_LENGTH);
    getString(newAccount.pesel, PESEL_LENGTH, "Nr identyfikacyjny (PESEL)", "%[0-9]s", PESEL_LENGTH-1);
    newAccount.balance = getDouble("What's the balance of your account");
    printAccountData(newAccount);
    return true;
}

void printWelcomeMessage()
{
    // system("clear");
    printf(welcomeMessage);
    while (getchar() != '\n')
        ;
}

void printMenu()
{
    // system("clear");
    printf("\n");
    for (int i = 0; i < OPERATIONS; i++)
        printf("%s", menuMessages[i]);
    printf("\n");
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

bool changeOption()
{
    int operationBuffer = -1, result;

    printf("Select operation to perform: ");
    result = scanf("%d", &operationBuffer);
    while (getchar() != '\n')
        ;

    if (result && (operationBuffer >= 1) && (operationBuffer <= OPERATIONS))
    {
        selectedOption = operationBuffer - 1;
        return true;
    }

    return false;
}