#ifndef __MAIN__H
#define __MAIN__H

#ifdef __APPLE__
#include <GLFW/glfw3.h>
#else
#include <GLFW/glfw3.h>
#endif
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <stdbool.h>

#define WIDTH 600
#define HEIGHT 800
#define VIEW 2
#define STEP .1   //STEP IS HALF SHIPWID
#define SHIPINITX 0
#define SHIPINITY -.85
#define SHIPWIDTH .2
#define SHIPHEIGHT .05
#define ALIENCOL 7
#define ALIENROW 5
#define ALIENROWINT .2 // alien radius
#define ALIENSP 60 // second
#define FIRECOOLINGTIME 30 // half a second
#define DEATHREAD 5
#define READVALUEUNIT 20.0
#define LIFETOTAL 5
#define FAILREAD 5

void alienBullet(GLint t);
bool win();
bool lose();
void draw();
#endif
