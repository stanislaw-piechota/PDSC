#include "accountOps.h"
#include "menu.h"
#include "inputOps.h"
#include "fileOps.h"

void searchOperation(){
    Account searchResult;
    printFindMenu();
    system("clear");
    switch (selectedSearchOption) {
        case 0:
            searchByID();
            break;
        case 1:
            searchByString(NAME_LENGTH, &searchResult, searchResult.name, "%[a-zA-Z]s");
            break;
        case 2:
            searchByString(SURNAME_LENGTH, &searchResult, searchResult.surname, "%[a-zA-Z-]s");
            break;
        case 3:
            searchByString(ADDRESS_LENGTH, &searchResult, searchResult.address, "%[a-zA-Z0-9,.- /]s");
            break;
        case 4:
            searchByString(PESEL_LENGTH, &searchResult, searchResult.pesel, "%[0-9]s");
            break;
        default:
            printf("No option selected. Continuing...\n");
            break;
    }
}

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
    case 2:
        searchOperation();
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
        printMenu(OPERATIONS, menuMessages);

        if (!changeOption(OPERATIONS, &selectedOption))
        {
            printf("Couldnt execute any operation\n");
            continue;
        }

        system("clear");
        executeOperation();
        waitForEnter();
    }

    return 0;
}