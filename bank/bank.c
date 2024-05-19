#include "accountOps.h"
#include "menu.h"
#include "inputOps.h"

void executeOperation()
{
    switch (selectedOption)
    {
    case 0:
        createNewAccount();
        break;
    case 1:
        printAccounts();
        break;
    case 8:
        exit(0);
        break;
    }
}

int main(int argc, char const *argv[])
{
    printWelcomeMessage();

    while (true)
    {
        system("clear");
        printMenu();

        if (!changeOption())
        {
            printf("Couldnt execute any operation\n");
            continue;
        }

        system("clear");
        executeOperation();
        waitForEnter();

        // printAccounts();
        // while (getchar() != '\n');
    }

    return 0;
}