#include "primlib.h"
#include <math.h>
#include <stdlib.h>
#include <unistd.h>

#define NUMBER_OF_VERTICIES 5
#define ROTATING_ANGLE 0.25
#define MAX_ANGLE 360
#define MIN_RADIUS 100
#define MAX_RADIUS 300
#define RADIUS_STEP 0.2

typedef struct {
	int x;
	int y;
	float alpha; // tilt angle from positive Y axis
} Vertex;

void drawScreen(Vertex* verticies)
{
	gfx_filledRect(0, 0, gfx_screenWidth() - 1, gfx_screenHeight() - 1, BLACK);
	for (int i = 0; i < NUMBER_OF_VERTICIES; i++) {
		gfx_line(verticies[i].x, verticies[i].y,
				 verticies[(i + 1) % NUMBER_OF_VERTICIES].x,
				 verticies[(i + 1) % NUMBER_OF_VERTICIES].y, YELLOW);
	}
	gfx_updateScreen();
	SDL_Delay(10);
}

float convertDegToRad(float angle)
{
	return angle / 180 * M_PI;
}

void updatePosition(float radius, Vertex* vertex)
{
	vertex->x = gfx_screenWidth() / 2 +
				(int)(sinf(convertDegToRad(vertex->alpha)) * radius);
	vertex->y = gfx_screenHeight() / 2 -
				(int)(cosf(convertDegToRad(vertex->alpha)) * radius);
}

void createVerticies(Vertex* verticies)
{
	for (int i = 0; i < NUMBER_OF_VERTICIES; i++) {
		Vertex newVertex;
		newVertex.alpha = (float)MAX_ANGLE / NUMBER_OF_VERTICIES * i;
		updatePosition(MIN_RADIUS, &newVertex);
		verticies[i] = newVertex;
	}
}

void updateVerticies(float* radius, float* radiusStep, Vertex* verticies)
{
	for (int i = 0; i < NUMBER_OF_VERTICIES; i++) {
		verticies[i].alpha =
			fmodf(verticies[i].alpha + ROTATING_ANGLE, MAX_ANGLE);

		if ((*radius > MAX_RADIUS) || (*radius < MIN_RADIUS))
			*radiusStep *= -1;
		*radius += *radiusStep;

		updatePosition(*radius, verticies + i);
	}
}

int main(int argc, char* argv[])
{
	if (gfx_init()) {
		exit(3);
	}

	float radius = MIN_RADIUS, radiusStep = RADIUS_STEP;
	Vertex verticies[NUMBER_OF_VERTICIES];
	createVerticies(verticies);

	while (1) {
		drawScreen(verticies);
		updateVerticies(&radius, &radiusStep, verticies);
		if (gfx_pollkey() == SDLK_ESCAPE)
			break;
	}

	return 0;
}
