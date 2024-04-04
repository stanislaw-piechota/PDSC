#include "primlib.h"
#include "pieces.inl"
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

#define SCREEN_WIDTH gfx_screenWidth()
#define SCREEN_HEIGHT gfx_screenHeight()

#define BOARD_WIDTH 10
#define BOARD_HEIGHT 20
#define HEIGHT_OFFSET 1
#define SQAURE_SIZE ((float)SCREEN_HEIGHT / (BOARD_HEIGHT + 2 * HEIGHT_OFFSET))
#define BOARD_START_X ((SCREEN_WIDTH - BOARD_WIDTH * SQAURE_SIZE) / 2)
#define DELAY_TIME 10
#define FALLING_TIME 0.4
#define ITERS_TO_MOVE(POINTS) (int)(10 * (33.1 / pow((double)POINTS / 10000 + 5.2, 1) - 1.4))
#define POINTS_PER_ROW 300
#define UPDATE_TRESHOLD 1000
#define UPDATE_DELTA 3
#define PIXELS_PER_LETTER 4

#define PIECE_SIZE 4
#define PIECES 7
#define NEXT_PIECES 3
#define ROTATIONS 4
#define PIECE_EMPTY 0
#define PIECE_ACTIVE 1
#define PIECE_DUMPED 2
#define DUMPED_COLOR RED

#define END_PROGRAM 1
#define LOST_GAME 2
#define CONTINUE_PROGRAM 4

short flags = 0;
int board[BOARD_HEIGHT][BOARD_WIDTH] = {{0}}, points = 0;
const int smashPoints[PIECES] = {25, 10, 50, 50, 15, 15, 10};

typedef struct
{
	// field with coordinates of each "active sqaure" of piece;
	// as there are only 4 "active squares" in each piece
	int fields[4][2];
	int rotation;
	int kind;
	int colorIndex;
} Piece;

void drawSquare(int xInd, int yInd, int color)
{
	gfx_filledRect(BOARD_START_X + xInd * SQAURE_SIZE, (HEIGHT_OFFSET + yInd) * SQAURE_SIZE,
				   BOARD_START_X + (xInd + 1) * SQAURE_SIZE, (HEIGHT_OFFSET + yInd + 1) * SQAURE_SIZE, color);
}

int findLeft(Piece piece)
{
	int left = BOARD_WIDTH;
	for (int j = 0; j < PIECE_SIZE; j++)
		if (piece.fields[j][1] < left)
			left = piece.fields[j][1];
	return left;
}

int findTop(Piece piece)
{
	int top = BOARD_HEIGHT;
	for (int i = 0; i < PIECE_SIZE; i++)
	{
		if (piece.fields[i][0] < top)
			top = piece.fields[i][0];
	}
	return top;
}

int findRight(Piece piece)
{
	int right = 0;
	for (int i = 0; i < PIECE_SIZE; i++)
	{
		if (piece.fields[i][1] > right)
			right = piece.fields[i][1];
	}
	return right;
}

void drawNextPieces(Piece *pieces)
{
	for (int i = 0; i < NEXT_PIECES; i++)
	{
		for (int j = 0; j < PIECE_SIZE; j++)
		{
			drawSquare(BOARD_WIDTH + HEIGHT_OFFSET + pieces[i].fields[j][1] - findLeft(pieces[i]),
					   HEIGHT_OFFSET + i * (PIECE_SIZE + HEIGHT_OFFSET) + pieces[i].fields[j][0],
					   pieces[i].colorIndex);
		}
	}
}

void drawHoldPiece(Piece *holdPiece)
{
	for (int i = 0; i < PIECE_SIZE; i++)
	{
		drawSquare(-HEIGHT_OFFSET - PIECE_SIZE + holdPiece->fields[i][1],
				   HEIGHT_OFFSET + holdPiece->fields[i][0], holdPiece->colorIndex);
	}
}

void drawCurrentPiece(Piece piece)
{
	for (int i = 0; i < PIECE_SIZE; i++)
	{
		drawSquare(piece.fields[i][1], piece.fields[i][0], piece.colorIndex);
	}
}

void drawLines()
{
	// intersecting lines
	for (int i = 0; i < BOARD_HEIGHT; i++)
	{
		gfx_line(BOARD_START_X, (HEIGHT_OFFSET + i) * SQAURE_SIZE,
				 BOARD_START_X + BOARD_WIDTH * SQAURE_SIZE - 1, (HEIGHT_OFFSET + i) * SQAURE_SIZE, WHITE);
	}
	for (int j = 0; j < BOARD_WIDTH; j++)
	{
		gfx_line(BOARD_START_X + j * SQAURE_SIZE, HEIGHT_OFFSET * SQAURE_SIZE,
				 BOARD_START_X + j * SQAURE_SIZE, (HEIGHT_OFFSET + BOARD_HEIGHT) * SQAURE_SIZE - 1, WHITE);
	}

	// white bounds
	gfx_rect(BOARD_START_X, HEIGHT_OFFSET * SQAURE_SIZE,
			 BOARD_START_X + BOARD_WIDTH * SQAURE_SIZE,
			 SCREEN_HEIGHT - HEIGHT_OFFSET * SQAURE_SIZE, WHITE);
}

void drawBoard()
{
	for (int i = 0; i < BOARD_HEIGHT; i++)
	{
		for (int j = 0; j < BOARD_WIDTH; j++)
		{
			if (board[i][j] == PIECE_DUMPED)
				drawSquare(j, i, DUMPED_COLOR);
		}
	}
}

void drawScore()
{
	// buff = "SCORE: XXXXXX"
	char buff[7 + 6];
	snprintf(buff, 7 + 6, "SCORE: %d", points);
	gfx_textout(SCREEN_WIDTH / 2 - strlen(buff) * PIXELS_PER_LETTER, (HEIGHT_OFFSET * SQAURE_SIZE - PIXELS_PER_LETTER) / 2, buff, WHITE);
}

void drawScreen(Piece piece, Piece *holdPiece, Piece *nextPieces)
{
	gfx_filledRect(0, 0, gfx_screenWidth() - 1, gfx_screenHeight() - 1,
				   BLACK);
	drawCurrentPiece(piece);
	drawNextPieces(nextPieces);
	drawHoldPiece(holdPiece);
	drawBoard();
	drawLines();
	drawScore();

	gfx_updateScreen();
	SDL_Delay(DELAY_TIME);
}

void drawEnding()
{
	int key;
	while ((key = gfx_pollkey()) != SDLK_RETURN)
	{
		if (key == SDLK_ESCAPE)
			exit(3);
		gfx_filledRect(0, 0, gfx_screenWidth() - 1, gfx_screenHeight() - 1,
					   BLACK);

		// up to 6 digits of score
		char message[25 + 6];
		snprintf(message, 31, "YOU LOST! YOUR SCORE IS: %d", points);
		gfx_textout(SCREEN_WIDTH / 2 - strlen(message) * PIXELS_PER_LETTER, SCREEN_HEIGHT / 2 - PIXELS_PER_LETTER / 2,
					message, WHITE);

		gfx_updateScreen();
	}
}

bool checkPieceCollision(Piece piece)
{
	for (int i = 0; i < PIECE_SIZE; i++)
	{
		if ((piece.fields[i][0] < 0) || (piece.fields[i][0] >= BOARD_HEIGHT))
			return true;
		if ((piece.fields[i][1] < 0) || (piece.fields[i][1] >= BOARD_WIDTH))
			return true;
		if (board[piece.fields[i][0]][piece.fields[i][1]] == PIECE_DUMPED)
			return true;
	}
	return false;
}

void blitPiece(Piece piece)
{
	for (int i = 0; i < PIECE_SIZE; i++)
		board[piece.fields[i][0]][piece.fields[i][1]] = PIECE_ACTIVE;
}

Piece createPiece(int x, int y, int pc, int rot, int color)
{
	Piece piece;
	piece.kind = pc;
	piece.rotation = rot;
	piece.colorIndex = color;
	int squareCount = 0;
	for (int i = 0; i < PIECE_SIZE; i++)
	{
		for (int j = 0; j < PIECE_SIZE; j++)
		{
			if (pieces[pc][rot][i][j])
			{
				piece.fields[squareCount][0] = y + i;	// y
				piece.fields[squareCount++][1] = x + j; // x
			}
		}
	}
	return piece;
}

void rotatePiece(Piece *piece)
{
	int newRotation = (piece->rotation + 1) % ROTATIONS;
	Piece rotatedPiece = createPiece(findLeft(*piece), findTop(*piece), piece->kind, newRotation, piece->colorIndex);
	if (!checkPieceCollision(rotatedPiece))
	{
		*piece = rotatedPiece;
		piece->rotation = newRotation;
	}
}

void randPiece(Piece *piece)
{
	int pcIndex = rand() % PIECES, rotIndex = rand() % ROTATIONS,
		x = rand() % (BOARD_WIDTH - PIECE_SIZE + 1), color = rand() % (COLOR_MAX - GREEN) + GREEN;
	*piece = createPiece(x, 0, pcIndex, rotIndex, color);
}

bool checkVerticalCollision(Piece *piece)
{
	for (int i = 0; i < PIECE_SIZE; i++)
	{
		// vetical collision with bottom border
		if (piece->fields[i][0] == BOARD_HEIGHT - 1)
			return true;
		// vertical collision with another piece
		if (board[piece->fields[i][0] + 1][piece->fields[i][1]] == PIECE_DUMPED)
			return true;
	}
	return false;
}

bool checkHorizontalCollisionsRight(Piece *piece)
{
	for (int i = 0; i < PIECE_SIZE; i++)
	{
		// horizontal collision with borders
		if (piece->fields[i][1] == BOARD_WIDTH - 1)
			return true;
		// horizontal collision with piece on the right
		if (board[piece->fields[i][0]][piece->fields[i][1] + 1] == PIECE_DUMPED)
			return true;
	}
	return false;
}

bool checkHorizontalCollisionsLeft(Piece *piece)
{
	for (int i = 0; i < PIECE_SIZE; i++)
	{
		// horizontal collision with borders
		if (!piece->fields[i][1])
			return true;
		// horizontal collision with piece on the left
		if (board[piece->fields[i][0]][piece->fields[i][1] - 1] == PIECE_DUMPED)
			return true;
	}
	return false;
}

bool checkRowCoplete(int row)
{
	for (int i = 0; i < BOARD_WIDTH; i++)
	{
		if (!board[row][i])
			return false;
	}
	return true;
}

bool checkRowEmpty(int row)
{
	for (int i = 0; i < BOARD_WIDTH; i++)
	{
		if (board[row][i] == PIECE_DUMPED)
			return false;
	}
	return true;
}

void removeRow(int row)
{
	for (int j = 0; j < BOARD_WIDTH; j++)
	{
		board[row][j] = board[row - 1][j] == PIECE_ACTIVE ? PIECE_EMPTY : board[row - 1][j];
		board[row - 1][j] = PIECE_EMPTY;
	}
}

void deleteCompleteRows(Piece piece, Piece *holdPiece, Piece *nextPieces)
{
	int i = BOARD_HEIGHT - 1, rowsCounter = 0;
	while (!checkRowEmpty(i) && (i > 0))
	{
		if (checkRowCoplete(i))
		{
			for (int k = i; k > 0; k--)
			{
				removeRow(k);
				drawScreen(piece, holdPiece, nextPieces);
			}
			SDL_Delay(25);
			points += (++rowsCounter) * POINTS_PER_ROW;
		}
		else
			i--;
	}
}

void updatePieces(Piece *piece, Piece *nextPieces)
{
	for (int i = 0; i < NEXT_PIECES - 1; i++)
		nextPieces[i] = nextPieces[i + 1];
	randPiece(nextPieces + NEXT_PIECES - 1);
}

void finishMove(Piece *piece, Piece *holdPiece, Piece *nextPieces)
{
	*piece = nextPieces[0];
	if (checkPieceCollision(*piece))
	{
		flags |= LOST_GAME;
		return;
	}
	updatePieces(piece, nextPieces);
	deleteCompleteRows(*piece, holdPiece, nextPieces);
}

void moveY(Piece *piece, Piece *holdPiece, Piece *nextPieces)
{
	bool toDump = checkVerticalCollision(piece);
	for (int i = PIECE_SIZE - 1; i >= 0; i--)
	{
		if (!toDump)
		{
			board[piece->fields[i][0]][piece->fields[i][1]] = PIECE_EMPTY;
			board[piece->fields[i][0] + 1][piece->fields[i][1]] = PIECE_ACTIVE;
			piece->fields[i][0]++;
		}
		else
		{
			board[piece->fields[i][0]][piece->fields[i][1]] = PIECE_DUMPED;
		}
	}
	if (toDump)
		finishMove(piece, holdPiece, nextPieces);
}

void fastForward(Piece *piece, Piece *holdPiece, Piece *nextPieces)
{
	while (!checkVerticalCollision(piece))
		moveY(piece, holdPiece, nextPieces);
	points += smashPoints[piece->kind];
	moveY(piece, holdPiece, nextPieces);
}

void moveX(short deltaX, Piece *piece)
{
	if ((checkHorizontalCollisionsLeft(piece) && (deltaX < 0)) ||
		(checkHorizontalCollisionsRight(piece) && (deltaX > 0)))
		return;
	for (int i = PIECE_SIZE - 1; i >= 0; i--)
	{
		board[piece->fields[i][0]][piece->fields[i][1]] = PIECE_EMPTY;
		board[piece->fields[i][0]][piece->fields[i][1] + deltaX] = PIECE_ACTIVE;
		piece->fields[i][1] += deltaX;
	}
}

bool tryToPlacePiece(Piece* piece, Piece* holdPiece, int left, int top){
	Piece switchedPiece = createPiece(left, top, holdPiece->kind, holdPiece->rotation,
										holdPiece->colorIndex);
	if (checkPieceCollision(switchedPiece))
		return false;
	*piece = *holdPiece;
	for (int i = 0; i < PIECE_SIZE; i++)
	{
		piece->fields[i][0] = top + holdPiece->fields[i][0];
		piece->fields[i][1] = left + holdPiece->fields[i][1];
	}
	return true;
}

void switchCurrent(Piece copy, Piece* holdPiece, int left, int top){
	*holdPiece = copy;
	for (int i = 0; i < PIECE_SIZE; i++)
	{
		holdPiece->fields[i][0] -= top;
		holdPiece->fields[i][1] -= left;
	}
}

void switchHold(Piece *piece, Piece *holdPiece, Piece *nextPieces)
{
	Piece currentCopy = *piece;
	int left = findLeft(*piece), top = findTop(*piece);
	if (holdPiece->colorIndex == PIECE_EMPTY)
	{
		*piece = nextPieces[0];
		updatePieces(piece, nextPieces);
		switchCurrent(currentCopy, holdPiece, left, top);
	}
	else if (tryToPlacePiece(piece, holdPiece, left, top)){
		switchCurrent(currentCopy, holdPiece, left, top);
	}
}

void handleKeyEvents(Piece *piece, Piece *holdPiece, Piece *nextPieces)
{
	int key = gfx_pollkey();
	switch (key)
	{
	case SDLK_ESCAPE:
		flags |= END_PROGRAM;
		break;
	case SDLK_DOWN:
		fastForward(piece, holdPiece, nextPieces);
		break;
	case SDLK_LEFT:
		moveX(-1, piece);
		break;
	case SDLK_RIGHT:
		moveX(1, piece);
		break;
	case SDLK_SPACE:
		rotatePiece(piece);
		break;
	case 'c':
		switchHold(piece, holdPiece, nextPieces);
		break;
	}
}

void drawIntroduction()
{
	int key;
	const char message[] = "PRESS ENTER TO START";
	while ((key = gfx_pollkey()) != SDLK_RETURN)
	{
		if (key == SDLK_ESCAPE)
			exit(3);
		gfx_filledRect(0, 0, SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1, BLACK);

		gfx_textout(SCREEN_WIDTH / 2 - strlen(message) * PIXELS_PER_LETTER, SCREEN_HEIGHT / 2 - PIXELS_PER_LETTER / 2,
					message, WHITE);

		gfx_updateScreen();
	}
}

void resetGame(){
	for (int i = 0; i < BOARD_HEIGHT; i++)
	{
		for (int j = 0; j < BOARD_WIDTH; j++)
			board[i][j] = 0;
	}
	points = 0;
}

void gameLoop(Piece* currentPiece, Piece* holdPiece, Piece* nextPieces){
	short iterCounter = 0;
	while (!(flags & END_PROGRAM))
	{
		drawScreen(*currentPiece, holdPiece, nextPieces);
		handleKeyEvents(currentPiece, holdPiece, nextPieces);

		if (++iterCounter >= ITERS_TO_MOVE(points))
		{
			iterCounter = 0;
			moveY(currentPiece, holdPiece, nextPieces);
		}

		if (flags & END_PROGRAM)
			exit(3);
		if (flags & LOST_GAME)
			break;
	}
}

void drawGame()
{
	resetGame();
	Piece currentPiece;
	Piece holdPiece = createPiece(0, 0, 0, 0, PIECE_EMPTY);
	Piece nextPieces[NEXT_PIECES];
	randPiece(&currentPiece);
	blitPiece(currentPiece);
	for (int i = 0; i < NEXT_PIECES; i++)
		randPiece(nextPieces + i);

	gameLoop(&currentPiece, &holdPiece, nextPieces);
	drawScreen(currentPiece, &holdPiece, nextPieces);
	SDL_Delay(1000);
}

int main(int argc, char *argv[])
{
	srand(time(NULL));
	if (gfx_init())
	{
		exit(3);
	}

	while (true)
	{
		drawIntroduction();
		drawGame();
		if (flags & LOST_GAME)
		{
			drawEnding();
			flags ^= LOST_GAME;
		}
	}
	return 0;
}
