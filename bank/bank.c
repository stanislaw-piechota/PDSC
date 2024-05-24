#include "accountOps.h"
#include "menu.h"
#include "inputOps.h"
#include "fileOps.h"

void makeDeposit(){
    makeDepositWithdraw(true);
}

void makeWithdraw(){
    makeDepositWithdraw(false);
}

void stopSystem(){
    exit(0);
}

void searchOperation(){
    Account searchResult;
    printFindMenu();
    system("clear");
    switch (selectedSearchOption) {
        case 0:
            getAccount(&searchResult, "search for");
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
    void (*functions[OPERATIONS])(void);
    functions[0] = createNewAccount;
    functions[1] = printAccounts;
    functions[2] = searchOperation;
    functions[3] = makeDeposit;
    functions[4] = makeWithdraw;
    functions[5] = makeTransfer;
    functions[6] = takeLoan;
    functions[7] = payDebt;
    functions[8] = stopSystem;
    functions[selectedOption]();
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