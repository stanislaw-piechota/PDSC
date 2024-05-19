#ifndef __FILE_OPERATIONS__

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

void connectToDatabse(FILE **database);
void setAccountPtr(FILE *database, long size);
void disconnectFromDatabase(FILE **database);
void saveAccountData(void *account);
bool readAccount(FILE *database, void *account, long id);

#endif