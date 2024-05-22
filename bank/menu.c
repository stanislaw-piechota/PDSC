#include "menu.h"

short selectedOption = -1, selectedSearchOption = -1;
const char welcomeMessage[] = "Welcome to the banking system\nPress ENTER to continue...";
char menuMessages[OPERATIONS][40] = {
    "1: Create new account",
    "2: List all accounts",
    "3: Search for an account",
    "4: Make a deposit",
    "5: Make a withdrawal",
    "6: Make a money transfer",
    "7: Take a loan",
    "8: Pay the debt",
    "9: Exit"};
char searchMessages[SEARCH_OPERATIONS][40] = {
    "1: Account ID",
    "2: Name",
    "3: Surname",
    "4: Address",
    "5: Identification number (PESEL)"};

void printWelcomeMessage()
{
    system("clear");
    printf(welcomeMessage);
    while (getchar() != '\n')
        ;
    system("clear");
}

void printMenu(short length, char messages[][40])
{
    system("clear");
    for (int i = 0; i < length; i++)
        printf("%s\n", messages[i]);
    printf("\n");
}

bool changeOption(short length, short *optionVar)
{
    int operationBuffer = -1, result;

    printf("Select operation to perform: ");
    result = scanf("%1d", &operationBuffer);
    while (getchar() != '\n')
        return false;

    if (result && (operationBuffer >= 1) && (operationBuffer <= length))
    {
        *optionVar = operationBuffer - 1;
        return true;
    }

    return false;
}

void printFindMenu()
{
    do
    {
        printMenu(SEARCH_OPERATIONS, searchMessages);
    } while (!changeOption(SEARCH_OPERATIONS, &selectedSearchOption));
}