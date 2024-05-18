#ifndef __FILE_OPERATIONS__

#include <stdio.h>
#include <stdlib.h>

void connectToDatabse(FILE **database);
void setAccountPtr(FILE *database, long size);
void disconnectFromDatabase(FILE **database);
void saveAccountData(void *account);

#endif