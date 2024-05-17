#include "inputOps.h"

void clearBuffer(char string[]){
    while (*string) string++;
    if (*(string - 1) != '\n')
        while (getchar() != '\n');
}

void trimTrailingWhitespaces(char string[]){
    while (*string) string++;
    string--; // \n
    while (isspace(*(--string))){
        *string = '\n';
        *(string+1) = '\0';
    }
}

void getString(char stringPtr[], int length, char *prompt, char* format, int minLength)
{
    int result;
    char buffer[length];
    do {
        printf("%s (%d-%d characters): ", prompt, minLength, length-1);
        fgets(buffer, length, stdin);
        clearBuffer(buffer);
        trimTrailingWhitespaces(buffer);
        result = sscanf(buffer, format, stringPtr);
    }
    while (!result || (strlen(stringPtr) < strlen(buffer)-1) || (strlen(stringPtr) <= minLength - 1));
}

void getDouble(double *field, char *prompt){
    char number[DOUBLE_LENGTH], *endPtr;
    double result;
    do {
        getString(number, DOUBLE_LENGTH, prompt, "%[0-9.+-]s", 1);
        result = strtod(number, &endPtr);
    } while((result < 0) || (*endPtr != '\0'));
    *field = result;
}