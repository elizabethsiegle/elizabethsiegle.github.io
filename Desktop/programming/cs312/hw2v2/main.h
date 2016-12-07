#ifndef __MAIN__H
#define __MAIN__H

#include <stdio.h>
#include <stdlib.h>

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
#define VIEWSZ 2
#define STEP .1
#define SHIPX 0
#define SHIPY -.85
#define SHIPWIDTH .2
#define SHIPHEIGHT .05
#define ALIENWIDTH 7
#define ALIENHEIGHT 5
#define ALIENROWINT .2 // alien radius
#define ALIENSP 40 // second
#define FIRECOOLINGTIME 30 // half a second
#define DEATHREAD 5
#define READVALUEUNIT 20.0
#define NUMLIVES 6

void alienShoot(GLint t);
bool win();
bool lose();
void draw();

#endif