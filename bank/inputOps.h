#ifndef __INPUT_OPERATIONS__

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_DOUBLE_EXPONENT 10
#define MAX_DOUBLE_VALUE (pow(10, MAX_DOUBLE_EXPONENT + 1) - 1)
#define MANTISSA_LENGTH 2
#define DOUBLE_LENGTH (MAX_DOUBLE_EXPONENT + MANTISSA_LENGTH + 1 + 2)

void getString(char stringPtr[], int length, char *prompt, char* format, int minLength);
void getDouble(double *field, char *prompt);
bool getConfirmation(void);

#endif