#include "fileOps.h"
#include "accountOps.h"

void connectToDatabse(FILE **database)
{
    if (!(*database = fopen("database.dat", "rb+")))
    {
        perror("Couldn\'t connect to the database");
        exit(1);
    }
}

void setAccountPtr(FILE *database, long size)
{
    if (fseek(database, size, SEEK_SET))
    {
        perror("Error while looking for account");
        exit(3);
    }
}

void disconnectFromDatabase(FILE **database)
{
    if (fclose(*database))
    {
        perror("Error closing the database");
        exit(2);
    }
}

void saveAccountData(void *account)
{
    Account *accountCopy = (Account *)account;
    FILE *database;
    connectToDatabse(&database);
    setAccountPtr(database, accountCopy->accountId * sizeof(Account));
    if (fwrite(accountCopy, sizeof(Account), 1, database) != 1)
    {
        perror("Error during writing to database");
        exit(2);
    }
    disconnectFromDatabase(&database);
}

bool readAccount(FILE *database, void *account, long id){

    setAccountPtr(database, id * sizeof(Account));

    return (fread(account, sizeof(Account), 1, database) == 1);
}