#include "inputOps.h"

int clearBuffer(char string[])
{
    while (*string)
        string++;
    if (*(string - 1) != '\n')
    {
        if (getchar() == '\n')
            return 1;
        while (getchar() != '\n')
            ;
        return 0;
    }
    return 1;
}

void trimTrailingWhitespaces(char string[])
{
    while (*string)
        string++;
    string--; // \n
    while (isspace(*(--string)))
    {
        *string = '\n';
        *(string + 1) = '\0';
    }
}

void getString(char stringPtr[], int length, char *prompt, char *format, int minLength)
{
    int scanResult, clearResult;
    char buffer[length];
    do
    {
        printf("%s (%d-%d characters): ", prompt, minLength, length - 1);
        fgets(buffer, length, stdin);
        clearResult = clearBuffer(buffer);
        trimTrailingWhitespaces(buffer);
        scanResult = sscanf(buffer, format, stringPtr);
    } while (!scanResult || !clearResult || (strlen(stringPtr) < strlen(buffer) - 1) || (strlen(stringPtr) <= minLength - 1));
}

void getDouble(double *field, char *prompt)
{
    char number[DOUBLE_LENGTH], *endPtr;
    double result;
    do
    {
        getString(number, DOUBLE_LENGTH, prompt, "%[0-9.+-]s", 1);
        result = strtod(number, &endPtr);
    } while ((result < 0) || (*endPtr != '\0') || (number[0] == '.') || (number[strlen(number)-1] == '.'));
    *field = result;
}

bool getConfirmation()
{
    char answer;
    int result;
    do
    {
        printf("Are you sure to perform this action (Y/N): ");
        result = scanf("%1[YN]c", &answer);
        while (getchar() != '\n')
            ;
    } while (!result);

    return (answer == 'Y' ? true : false);
}
