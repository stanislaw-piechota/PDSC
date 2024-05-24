#ifndef __PRINT_MENU__

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define OPERATIONS 9
#define SEARCH_OPERATIONS 5

void printWelcomeMessage(void);
void printMenu(short length, char messages[][40]);
bool changeOption(short length, short *optionVar);
void printFindMenu(void);
void printAccountData(void *accountData);
void printAccountHeader(void);

extern short selectedOption;
extern short selectedSearchOption;
extern const char welcomeMessage[];
extern char menuMessages[OPERATIONS][40];
extern char searchMessages[SEARCH_OPERATIONS][40];


#endif