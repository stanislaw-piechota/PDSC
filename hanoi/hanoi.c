#include "primlib.h"
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define SCREEN_WIDTH gfx_screenWidth()
#define SCREEN_HEIGHT gfx_screenHeight()
#define POLES_NO 3 // up to 10
#define DISKS_NO 3 // works best up to 25
#define EMPTY (-1)

#define POLE_COLOR YELLOW
#define DISK_COLOR BLUE
#define FLOOR_COLOR RED

#define FLOOR_HEIGHT 50
#define DISK_HEIGHT 20
#define POLE_OFFSET DISK_HEIGHT
#define POLE_WIDTH 20
#define POLE_HEIGHT (DISKS_NO * DISK_HEIGHT + POLE_OFFSET)
#define DISK_SPACE 10
#define DISK_LENGTH_PER_UNIT \
	((SCREEN_WIDTH / (POLES_NO + 1) - DISK_SPACE - POLE_WIDTH) / DISKS_NO)

#define ANIMATION_Y_POS SCREEN_HEIGHT - FLOOR_HEIGHT - POLE_HEIGHT - POLE_OFFSET
#define WIN_TEXT "Congratulations! You won"
#define LOSE_TEXT "You lose! Try again"
#define END_TIME 3000

// flags
#define END_PROGRAM 1
#define CONFIRM_CHOICE 2
#define ANIMATION_RUNNING 4
#define WIN 8

typedef struct
{
	int left;
	int top;
	int right;
	int bottom;
	int value;
} Disk;

// START OF STACK IMPLEMENTATION
typedef struct
{
	Disk disks[DISKS_NO];
	int top;
} Pole;

bool push(Pole *stack, Disk value)
{
	if (stack->top == DISKS_NO - 1)
		return false;

	stack->top++;
	stack->disks[stack->top] = value;
	return true;
}

Disk pop(Pole *stack)
{
	Disk diskToPop;
	diskToPop.value = EMPTY;
	if (stack->top == EMPTY)
		return diskToPop;

	diskToPop = stack->disks[stack->top];
	stack->top--;
	return diskToPop;
}
// END OF STACK IMPLEMENTATION

Disk *topDisk(Pole *poles, int poleIndex)
{
	if (poles[poleIndex].top == EMPTY)
		return NULL;
	return &poles[poleIndex].disks[poles[poleIndex].top];
}

int topValue(Pole *poles, int poleIndex)
{
	Disk *disk = topDisk(poles, poleIndex);
	return disk == NULL ? EMPTY : disk->value;
}

int getPoleCenterX(int index)
{
	return SCREEN_WIDTH / (POLES_NO + 1) * index;
}

void initializePoles(Pole *poles)
{
	for (int i = 0; i < POLES_NO; i++)
	{
		Pole newPole;
		newPole.top = EMPTY;
		poles[i] = newPole;
	}

	for (int i = 0; i < DISKS_NO; i++)
	{
		Disk newDisk;
		int diskShift =
			((DISKS_NO - i) * DISK_LENGTH_PER_UNIT + POLE_WIDTH) / 2;
		newDisk.left = getPoleCenterX(1) - diskShift;
		newDisk.right = getPoleCenterX(1) + diskShift;
		newDisk.top = SCREEN_HEIGHT - FLOOR_HEIGHT - DISK_HEIGHT * (i + 1);
		newDisk.bottom = SCREEN_HEIGHT - FLOOR_HEIGHT - DISK_HEIGHT * i;
		newDisk.value = DISKS_NO - i;
		push(poles, newDisk);
	}
}

void drawDisks(Pole *poles)
{
	for (int i = 0; i < POLES_NO; i++)
	{
		for (int j = 0; j <= poles[i].top; j++)
		{
			Disk *current = poles[i].disks + j;
			gfx_filledRect(current->left, current->top, current->right,
						   current->bottom, DISK_COLOR);
		}
	}
}

void drawPoles(Pole *poles)
{
	for (int i = 0; i < POLES_NO; i++)
	{
		gfx_filledRect(getPoleCenterX(i + 1) - POLE_WIDTH / 2,
					   SCREEN_HEIGHT - FLOOR_HEIGHT - POLE_HEIGHT,
					   getPoleCenterX(i + 1) + POLE_WIDTH / 2,
					   SCREEN_HEIGHT - FLOOR_HEIGHT, POLE_COLOR);
	}
}

void drawTexts(int src, int dest)
{
	// 8 characters to be able to display "10"
	char buffer[8];
	snprintf(buffer, 8, "SRC: %d", src + 1);
	gfx_textout(10, 10, buffer, WHITE);
	snprintf(buffer, 8, "DST: %d", dest + 1);
	gfx_textout(10, 20, buffer, WHITE);
}

void drawScreen(Pole *poles, int src, int dest)
{
	gfx_filledRect(0, 0, SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1, BLACK);
	gfx_filledRect(0, SCREEN_HEIGHT - FLOOR_HEIGHT, SCREEN_WIDTH - 1,
				   SCREEN_HEIGHT - 1, FLOOR_COLOR);
	drawPoles(poles);
	drawDisks(poles);
	drawTexts(src, dest);
	gfx_updateScreen();
	SDL_Delay(10);
}

void mapInput(short *flags, int *scope)
{
	char key = gfx_pollkey();
	int keyVal;
	switch (key)
	{
	case SDLK_ESCAPE:
		*flags |= END_PROGRAM;
		break;
	case SDLK_RETURN:
		*flags |= CONFIRM_CHOICE;
		break;
	case EMPTY:
		break;
	default:
		keyVal = key - '0';
		if ((keyVal < 0) || (keyVal > POLES_NO) || (*flags & ANIMATION_RUNNING))
			break;
		if ((!keyVal) && (POLES_NO == 10))
			*scope = 9;
		else if (keyVal)
			*scope = keyVal - 1;
		break;
	}
}

bool isMoveLegal(Pole *poles, int src, int dest)
{
	if (topValue(poles, src) == EMPTY)
		return false;
	if ((topValue(poles, src) >= topValue(poles, dest)) && (topValue(poles, dest) != EMPTY))
		return false;
	if (src == dest)
		return false;
	printf("src: %d, dest: %d\n", src, dest);
	return true;
}

int sgn(int val)
{
	if (val > 0)
		return 1;
	return -1;
}

void resetScopes(int *src, int *dest, short *flags)
{
	*src = EMPTY;
	*dest = EMPTY;
	if (*flags & ANIMATION_RUNNING)
		*flags ^= ANIMATION_RUNNING;
}

bool checkWin(Pole *poles, int src, int dest)
{
	push(&poles[dest], pop(&poles[src]));
	if (poles[POLES_NO - 1].top == DISKS_NO - 1)
	{
		return true;
	}
	return false;
}

void animatePiece(Disk *disk, int *src, int *dest, Pole *poles, short *flags)
{
	int diskCenterX = (disk->left + disk->right) / 2, destVal = *dest, srcVal = *src;
	if ((disk->bottom > ANIMATION_Y_POS) && (diskCenterX == getPoleCenterX(srcVal + 1)))
	{
		disk->top--;
		disk->bottom--;
	}
	else if ((disk->bottom == ANIMATION_Y_POS) && (diskCenterX != getPoleCenterX(destVal + 1)))
	{
		disk->right += sgn(destVal - srcVal);
		disk->left += sgn(destVal - srcVal);
	}
	else if ((diskCenterX == getPoleCenterX(destVal + 1)) && (disk->bottom < (topDisk(poles, destVal) == NULL ? SCREEN_HEIGHT - FLOOR_HEIGHT : topDisk(poles, destVal)->top)))
	{
		disk->top++;
		disk->bottom++;
	}
	else
	{
		if (checkWin(poles, srcVal, destVal))
		{
			*flags |= END_PROGRAM;
			*flags |= WIN;
		}
		resetScopes(src, dest, flags);
	}
}

void drawEnding(short flags)
{
	char *endText = flags & WIN ? WIN_TEXT : LOSE_TEXT;
	gfx_filledRect(0, 0, SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1, BLACK);
	// 4*strlen deduced by trying
	gfx_textout(SCREEN_WIDTH / 2 - 4 * strlen(endText), SCREEN_HEIGHT / 2, endText, WHITE);
	gfx_updateScreen();
}

int main(int argc, char *argv[])
{
	if (gfx_init())
	{
		exit(3);
	}

	Pole poles[POLES_NO];
	int src = EMPTY, dest = EMPTY;
	int *currentScope = &src;
	short flags = 0;
	Disk *animatedDisk;
	initializePoles(poles);

	while (true)
	{
		drawScreen(poles, src, dest);
		mapInput(&flags, currentScope);

		// handling the events
		if ((flags & CONFIRM_CHOICE) && !(flags & ANIMATION_RUNNING))
		{
			flags ^= CONFIRM_CHOICE;
			if ((currentScope == &src) && (src != EMPTY))
				currentScope = &dest;
			else if ((currentScope == &dest) && (dest != EMPTY))
			{
				currentScope = &src;
				if (isMoveLegal(poles, src, dest))
				{
					flags |= ANIMATION_RUNNING;
					animatedDisk = topDisk(poles, src);
				}
				else
					resetScopes(&src, &dest, &flags);
			}
		}
		else if (flags & ANIMATION_RUNNING)
		{
			animatePiece(animatedDisk, &src, &dest, poles, &flags);
		}

		if (flags & END_PROGRAM)
			break;
	}
	drawEnding(flags);
	SDL_Delay(END_TIME);

	return 0;
}
