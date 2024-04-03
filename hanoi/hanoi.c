#include "primlib.h"
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>

#define SCREEN_WIDTH gfx_screenWidth()
#define SCREEN_HEIGHT gfx_screenHeight()
#define POLES_NO 3
#define DISCS_NO 3
#define EMPTY -1
#define POLE_COLOR YELLOW
#define DISC_COLOR BLUE
#define FLOOR_COLOR RED
#define FLOOR_HEIGHT 50
#define DISC_HEIGHT 15
#define POLE_WIDTH 20
#define POLE_HEIGHT (DISCS_NO * DISC_HEIGHT + DISC_HEIGHT)
#define DISC_LENGTH_PER_UNIT \
	((SCREEN_WIDTH / (POLES_NO + 1) - POLE_WIDTH) / DISCS_NO)
#define POLE_CENTER_X(index) (SCREEN_WIDTH / (POLES_NO + 1) * (index))
#define ANIMATION_Y_POS (SCREEN_HEIGHT - FLOOR_HEIGHT - POLE_HEIGHT - DISC_HEIGHT)
#define WIN_TEXT "Congratulations! You won"
#define LOSE_TEXT "You lose! Try again"
#define PIXELS_PER_LETTER 4
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
} Disc;

typedef struct
{
	Disc discs[DISCS_NO];
	int top;
} Pole;

void push(Pole *stack, Disc value)
{
	assert(stack->top != DISCS_NO - 1);
	stack->discs[++stack->top] = value;
}

Disc pop(Pole *stack)
{
	assert(stack->top != EMPTY);
	return stack->discs[stack->top--];
}

void init_lib()
{
	if (gfx_init())
		exit(3);
}

Disc *topDisc(int poleIndex);
int topValue(int poleIndex);
int signum(int val);
void initializePoles();
void initializeDiscs();
void drawPole(int index);
void drawDisc(Disc *currentDisc);
void drawTexts(int src, int dest);
void drawObjects(int src, int dest);
void drawEnding();
void drawScreen(int src, int dest);
void switchScopes(int **current, int *src, int *dest, Disc **animatedDisc);
void mapInput(int *scope);
void processInput(int *scope, int keyVal);
void animateLeftRight(Disc *disc, int *src, int *dest);
void animateUp(Disc *disc, int *src, int *dest);
void animateDown(Disc *disc, int *src, int *dest);
void endMove(Disc *disc, int *src, int *dest);
void resetScopes(int *src, int *dest);
bool isMoveLegal(int src, int dest);
bool checkWin();

Pole poles[POLES_NO];
short flags = 0;
void (*animationFunction)(Disc *, int *, int *) = animateUp;

int main(int argc, char *argv[])
{
	int src = EMPTY, dest = EMPTY;
	int *currentScope = &src;
	Disc *animatedDisc;
	init_lib();
	initializePoles();
	initializeDiscs();

	while (true)
	{
		drawScreen(src, dest);
		mapInput(currentScope);

		if ((flags & CONFIRM_CHOICE) && !(flags & ANIMATION_RUNNING))
			switchScopes(&currentScope, &src, &dest, &animatedDisc);
		else if (flags & ANIMATION_RUNNING)
			animationFunction(animatedDisc, &src, &dest);
		if (flags & END_PROGRAM)
			break;
	}

	drawEnding();
	return 0;
}

void mapInput(int *scope)
{
	char key = gfx_pollkey();
	switch (key)
	{
	case SDLK_ESCAPE:
		flags |= END_PROGRAM;
		break;
	case SDLK_RETURN:
		flags |= CONFIRM_CHOICE;
		break;
	case EMPTY:
		break;
	default:
		processInput(scope, key - '0');
		break;
	}
}

void processInput(int *scope, int keyVal)
{
	if ((keyVal < 0) || (keyVal > POLES_NO) || (flags & ANIMATION_RUNNING))
		return;
	if ((!keyVal) && (POLES_NO == 10))
		*scope = 9;
	else if (keyVal)
		*scope = keyVal - 1;
}

void switchScopes(int **current, int *src, int *dest, Disc **animatedDisc)
{
	flags ^= CONFIRM_CHOICE;
	if ((*current == src) && (*src != EMPTY))
		*current = dest;
	else if ((*current == dest) && (*dest != EMPTY))
	{
		*current = src;
		if (isMoveLegal(*src, *dest))
		{
			flags |= ANIMATION_RUNNING;
			*animatedDisc = topDisc(*src);
		}
		else
			resetScopes(src, dest);
	}
}

Disc *topDisc(int poleIndex)
{
	if (poles[poleIndex].top == EMPTY)
		return NULL;
	return &poles[poleIndex].discs[poles[poleIndex].top];
}

int topValue(int poleIndex)
{
	Disc *disc = topDisc(poleIndex);
	return disc == NULL ? EMPTY : disc->value;
}

void initializePoles()
{
	for (int i = 0; i < POLES_NO; i++)
		poles[i].top = EMPTY;
}

void initializeDiscs()
{
	for (int i = 0; i < DISCS_NO; i++)
	{
		Disc newDisc;
		int discShift =
			((DISCS_NO - i) * DISC_LENGTH_PER_UNIT + POLE_WIDTH) / 2;
		newDisc.left = POLE_CENTER_X(1) - discShift;
		newDisc.right = POLE_CENTER_X(1) + discShift;
		newDisc.top = SCREEN_HEIGHT - FLOOR_HEIGHT - DISC_HEIGHT * (i + 1);
		newDisc.bottom = SCREEN_HEIGHT - FLOOR_HEIGHT - DISC_HEIGHT * i;
		newDisc.value = DISCS_NO - i;
		push(poles, newDisc);
	}
}

void drawDisc(Disc *currentDisc)
{
	gfx_filledRect(currentDisc->left, currentDisc->top, currentDisc->right,
				   currentDisc->bottom, DISC_COLOR);
}

void drawPole(int index)
{
	gfx_filledRect(POLE_CENTER_X(index + 1) - POLE_WIDTH / 2,
				   SCREEN_HEIGHT - FLOOR_HEIGHT - POLE_HEIGHT,
				   POLE_CENTER_X(index + 1) + POLE_WIDTH / 2,
				   SCREEN_HEIGHT - FLOOR_HEIGHT, POLE_COLOR);
}

void drawTexts(int src, int dest)
{
	char buffer[8];
	snprintf(buffer, 8, "SRC: %d", src + 1);
	gfx_textout(10, 10, buffer, WHITE);
	snprintf(buffer, 8, "DST: %d", dest + 1);
	gfx_textout(10, 20, buffer, WHITE);
}

void drawObjects(int src, int dest)
{
	for (int i = 0; i < POLES_NO; i++)
		drawPole(i);
	for (int i = 0; i < POLES_NO; i++)
	{
		for (int j = 0; j <= poles[i].top; j++)
			drawDisc(poles[i].discs + j);
	}
	drawTexts(src, dest);
}

void drawScreen(int src, int dest)
{
	gfx_filledRect(0, 0, SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1, BLACK);
	gfx_filledRect(0, SCREEN_HEIGHT - FLOOR_HEIGHT + 1, SCREEN_WIDTH - 1,
				   SCREEN_HEIGHT - 1, FLOOR_COLOR);
	drawObjects(src, dest);
	gfx_updateScreen();
	SDL_Delay(10);
}

void drawEnding()
{
	char *endText = flags & WIN ? WIN_TEXT : LOSE_TEXT;
	while (gfx_pollkey() == EMPTY)
	{
		gfx_filledRect(0, 0, SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1, BLACK);
		gfx_textout(SCREEN_WIDTH / 2 - PIXELS_PER_LETTER * strlen(endText), SCREEN_HEIGHT / 2, endText, WHITE);
		gfx_updateScreen();
		SDL_Delay(10);
	}
}

void animateUp(Disc *disc, int *src, int *dest)
{
	disc->top--;
	disc->bottom--;
	if (disc->bottom <= ANIMATION_Y_POS)
		animationFunction = animateLeftRight;
}

void animateDown(Disc *disc, int *src, int *dest)
{
	disc->top++;
	disc->bottom++;

	Disc *destDisc = topDisc(*dest);
	if (disc->bottom >= (destDisc == NULL ? SCREEN_HEIGHT - FLOOR_HEIGHT : destDisc->top))
		animationFunction = endMove;
}

void animateLeftRight(Disc *disc, int *src, int *dest)
{
	int direction = signum(*dest - *src);
	disc->right += direction;
	disc->left += direction;
	if ((disc->left + disc->right) / 2 == POLE_CENTER_X(*dest + 1))
		animationFunction = animateDown;
}

void endMove(Disc *disc, int *src, int *dest)
{
	push(&poles[*dest], pop(&poles[*src]));
	resetScopes(src, dest);
	animationFunction = animateUp;
	if (checkWin())
	{
		flags |= END_PROGRAM;
		flags |= WIN;
	}
}

bool isMoveLegal(int src, int dest)
{
	if (topValue(src) == EMPTY)
		return false;
	if ((topValue(src) >= topValue(dest)) && (topValue(dest) != EMPTY))
		return false;
	if (src == dest)
		return false;
	return true;
}

int signum(int val)
{
	return (val > 0) - (val < 0);
}

void resetScopes(int *src, int *dest)
{
	*src = EMPTY;
	*dest = EMPTY;
	if (flags & ANIMATION_RUNNING)
		flags ^= ANIMATION_RUNNING;
}

bool checkWin()
{
	return (poles[POLES_NO - 1].top == DISCS_NO - 1);
}