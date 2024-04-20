#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <limits.h>
#include <errno.h>

#define MAX_BASE 36
#define MIN_BASE 2
#define SPECIAL_BASE 0

int getDigit(int c, int base)
{
	int digitValue = -1;
	if (isdigit(c))
		digitValue = c - '0';
	else if (isalpha(c) && (base > 10))
	{
		if (islower(c))
			digitValue = c - 'a' + 10;
		else
			digitValue = c - 'A' + 10;
	}

	if (digitValue < base)
		return digitValue;
	return -1;
}

void checkOverload(long *number, bool negative)
{
	if ((*number >> 63 == -1) && !negative)
	{
		errno = 34;
		*number = LONG_MAX;
	}

	if ((*number >> 63 == 0) && *number && negative)
	{
		errno = 34;
		*number = LONG_MIN;
	}
}

bool checkCriticalErrors(char **endPtr, int base)
{
	if (!endPtr)
		return false;
	if (((base < MIN_BASE) || (base > MAX_BASE)) && (base != SPECIAL_BASE))
	{
		errno = 22;
		return false;
	}
	return true;
}

bool parseSign(char **endPtr)
{
	if (**endPtr == '-')
	{
		(*endPtr)++;
		return true;
	}
	else if (**endPtr == '+')
		(*endPtr)++;
	return false;
}

int recognizeBase(char **endPtr, int base)
{
	int digit = getDigit(**endPtr, 10);
	if (digit > 0)
		return 10;

	else if (digit == 0)
	{
		(*endPtr)++;
		if (**endPtr == 'x'){
			(*endPtr)++;
			return 16;
		}
		return 8;
	}

	return -1;
}

void skipWhitespaces(char **endPtr){
	while (isspace(**endPtr))
		(*endPtr)++;
}

long convertString(char **endPtr, int base, bool negative){
	long number = 0;
	int digit;

	while ((digit = getDigit(**endPtr, base)) >= 0)
	{
		(*endPtr)++;
		number *= base;
		number += (negative ? -digit : digit);
	}

	return number;
}

void reversePtrIfNan(const char *nPtr, char **endPtr, int base, long number){
	if ((getDigit(*(*endPtr - 1), base) < 0) && !number)
		while ((getDigit(*(*endPtr-1), base) < 0) && (*endPtr != nPtr))
			(*endPtr)--;
}

long strtol(const char *nPtr, char **endPtr, int base)
{
	if (!checkCriticalErrors(endPtr, base))
		return 0;
	
	skipWhitespaces(endPtr);
	bool negative = parseSign(endPtr);
	int detectedBase = recognizeBase(endPtr, base);
	if (!base) base = detectedBase;

	long number = convertString(endPtr, base, negative);
	checkOverload(&number, negative);

	reversePtrIfNan(nPtr, endPtr, base, number);
	return number;
}

// int main()
// {
// 	char *string = strdup("-0x");
// 	char **end = &string;
// 	long result = strtol(string, end, 0);

// 	printf("%ld\n", result);
// 	printf("End points at: %s\n", *end);

// 	return 0;
// }