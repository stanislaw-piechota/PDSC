#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
// #include "rand_malloc.h"

bool isEndOfFile = false;
char *allocateChar(char *, int *, int);
char *getLine(void);
void printLines(int);
char *reverseLine(char *, int);

int main(int argc, char *argv[])
{
	if ((argc == 2) && strcmp(argv[1], "rand")){
		printf("Random mode siwtched on\n");
	}
	printLines(1);
	return 0;
}

void printLines(int depth)
{
	char *str = NULL;
	if ((str = getLine()) == NULL)
		return;
	if (!isEndOfFile)
		printLines(depth + 1);
	printf("%s", str);
	free(str);
}

char *allocateChar(char *str, int *newSize, int c)
{
	char *temp = realloc(str, ++(*newSize) * sizeof(char));
	if (temp == NULL)
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

char *reverseLine(char *line, int size)
{
	char *newLine = malloc(size * sizeof(char));
	if (newLine == NULL)
		return NULL;

	char *newIndex = newLine, *wordStart = line + size - 3, *wordEnd = wordStart, *index;
	while (wordStart > line)
	{
		if (*wordStart == ' ')
		{
			index = wordStart+1;
			while (index <= wordEnd)
				*(newIndex++) = *(index++);
			*(newIndex++) = ' ';
			wordEnd = --wordStart;
		}
		else
			wordStart--;
	}
	index = wordStart;
	while ((index <= wordEnd) && (wordEnd >= line))
		*(newIndex++) = *(index++);
	*(newIndex++) = '\n';
	*newIndex = '\0';

	free(line);
	return newLine;
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
			if ((str = allocateChar(str, &size, '\n')) == NULL)
				return NULL;
			isEndOfFile = true;
			break;
		}
		if ((str = allocateChar(str, &size, character)) == NULL)
			return NULL;
	} while ((character != EOF) && (character != '\n'));
	str = allocateChar(str, &size, '\0');

	if ((str = reverseLine(str, size)) == NULL)
		return NULL;
	return str;
}