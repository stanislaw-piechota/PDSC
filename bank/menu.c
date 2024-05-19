#include "menu.h"
#include "messages.inl"

short selectedOption = -1;

void printWelcomeMessage()
{
    system("clear");
    printf(welcomeMessage);
    while (getchar() != '\n')
        ;
    system("clear");
}

void printMenu()
{
    printf("\n");
    for (int i = 0; i < OPERATIONS; i++)
        printf("%s", menuMessages[i]);
    printf("\n");
}

bool changeOption()
{
    int operationBuffer = -1, result;

    printf("Select operation to perform: ");
    result = scanf("%1d", &operationBuffer);
    while (getchar() != '\n')
        return false;

    if (result && (operationBuffer >= 1) && (operationBuffer <= OPERATIONS))
    {
        selectedOption = operationBuffer - 1;
        return true;
    }

    return false;
}