#include "accountOps.h"
#include "menu.h"

int main(int argc, char const *argv[])
{
    printWelcomeMessage();

    while (true) {
        printMenu();
        if(changeOption()){
        } else {
            printf("Couldnt execute any operation\n");
        }

        if (selectedOption == 0)
            createNewAccount();
        if (selectedOption == OPERATIONS - 1)
            exit(0);
    }


    return 0;
}