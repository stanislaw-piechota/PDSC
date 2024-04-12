#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "rand_malloc.h"

bool isEndOfFile = false;
char *allocateChar(char *, int *, int);
char *getLine(void);
void printLines(void);
void printReversedLine(char *line);

int main(int argc, char *argv[])
{
	printLines();
	return 0;
}

void printLines()
{
	char *str = NULL;
	if ((str = getLine()) == NULL)
		return;
	if (!isEndOfFile)
		printLines();
	printReversedLine(str);
	free(str);
}

char *allocateChar(char *str, int *newSize, int c)
{
	char *temp = realloc(str, ++(*newSize) * sizeof(char));
	if (!temp)
	{
		free(str);
		return NULL;
	}
	else
	{
		str = temp;
		*(str + *newSize - 1) = (char)c;
	}

	return str;
}

int stringLen(char *str)
{
	int len = 0;
	while (*str++)
		len++;
	return len;
}

void printReversedLine(char *line)
{
	char *wordStart = line + stringLen(line) - 2, *wordEnd = wordStart, *index;
	while (wordStart >= line)
	{
		if ((*wordStart == ' ') || (wordStart == line))
		{
			index = wordStart + (wordStart != line);
			while (index <= wordEnd)
				printf("%c", *index++);
			if (wordStart != line)
				printf(" ");
			wordEnd = --wordStart;
		}
		else
			wordStart--;
	}
	printf("\n");
}

char *getLine()
{
	char *str = NULL;
	int size = 0, character;

	do
	{
		character = getchar();
		if (character == EOF)
		{
			if (!(str = allocateChar(str, &size, '\n')))
				return NULL;
			isEndOfFile = true;
			break;
		}
		if (!(str = allocateChar(str, &size, character)))
			return NULL;
	} while ((character != EOF) && (character != '\n'));
	str = allocateChar(str, &size, '\0');

	return str;
}