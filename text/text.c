#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define END_OF_FILE 1

short flags = 0;

char *allocateChar(char *str, int *newSize, int c)
{
	char *temp = realloc(str, ++(*newSize) * sizeof(char));
	if (temp == NULL)
	{
		printf("Couldn't allocate memory\nUsing old address: %p\n", str);
	}
	else
	{
		str = temp;
		// printf("Allocation succesful, new address: %p\n", str);
		*(str + *newSize - 1) = (char)c;
	}

	return str;
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
			flags |= END_OF_FILE;
			break;
		}

		str = allocateChar(str, &size, character);
	} while ((character != EOF) && (character != '\n'));
	str = allocateChar(str, &size, '\0');

	return str;
}

int main(int argc, char *argv[])
{
	char *str = NULL;

	do {
		str = getLine();
		printf("%s", str);
		free(str);
	} while(!(flags & END_OF_FILE));

	return 0;
}
