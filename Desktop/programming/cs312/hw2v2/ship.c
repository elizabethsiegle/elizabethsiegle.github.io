#include "ship.h"
#include "main.h"
#include "list.h"

GLint alienHit[ALIENHEIGHT*ALIENWIDTH];
GLdouble leftPressed, rightPressed, spacePressed, downPressed, shipShouldMove;
GLdouble shipDX, shipDY;
GLdouble alienDX, alienDY;
GLdouble fireMissileV = SHIPHEIGHT;
extern GLint shipShoot;

extern Alien aliens[ALIENHEIGHT * ALIENWIDTH];
extern Node *bulletShot;
extern Node *shipBull;
GLint numTime;
GLint hitReadValue = 0;
extern GLint numHits;
GLint NextStepInDebug;
GLint numAliens = ALIENWIDTH * ALIENHEIGHT;
GLint previousFireTime;

void checkAndFire() {
  if (spacePressed) {
    Node* temp = makeNode(makeBullet(SHIPX, SHIPY, shipDX, 0));
    addNode(temp, &shipBull);
    spacePressed = false;
  }
  if(shipBull != NULL) {
    for (Node* temp = shipBull->h; temp != NULL; temp = temp->next) {
      GLdouble xHere = temp->data->xPos + STEP;
      GLdouble yHere = temp->data->yPos + SHIPHEIGHT;
      if(temp->data->active == true) {
        glPushMatrix();
        glColor3f(1, .6, .6);
        glTranslatef(temp->data->dx, temp->data->dy, 0);
        glBegin(GL_POLYGON);
        glVertex2f(xHere + SHIPHEIGHT/2, yHere + SHIPHEIGHT/2);
        glVertex2f(xHere + SHIPHEIGHT/2, yHere - SHIPHEIGHT/2);
        glVertex2f(xHere - SHIPHEIGHT/2, yHere - SHIPHEIGHT/2);
        glVertex2f(xHere - SHIPHEIGHT/2, yHere + SHIPHEIGHT/2);
        glEnd();
        glPopMatrix();
        glPushMatrix();
        for (int i = 0; i< numAliens; i++) {
          if (aliens[i].alive) {
            if((xHere + temp->data->dx - aliens[i].x - alienDX - STEP)*(xHere + temp->data->dx - aliens[i].x - alienDX - STEP) +
            (yHere +temp->data->dy - aliens[i].y - alienDY - SHIPHEIGHT/2)*(yHere + temp->data->dy - aliens[i].y - alienDY - SHIPHEIGHT/2)
            < STEP*STEP) {
              aliens[i].alive = false;
              alienHit[i] = 0;
              temp->data->active = false;
            }
          }
        }
      }
    }
  }
  if(bulletShot != NULL) {
      for (Node *temp = bulletShot->h; temp != NULL; temp = temp->next) {
          GLdouble xx = temp->data->xPos + STEP;
          GLdouble yy = temp->data->yPos + SHIPHEIGHT;
          if(temp->data->active == true) {
              glPushMatrix();
              if((xx + temp->data->dx - SHIPX - shipDX - STEP)*(xx + temp->data->dx - SHIPX - shipDX- STEP) +
                 (yy +temp->data->dy - SHIPY - SHIPHEIGHT/2)*(yy + temp->data->dy - SHIPY - SHIPHEIGHT/2)
                 < STEP^2) {
                  temp->data->active = false;
                  hitReadValue = READVALUEUNIT*DEATHREAD;
                  numHits++;
                  //exit(0);
              }
          }
      }
  }
}

void drawShip() {
  if(shipShouldMove && rightPressed && shipDX <= VIEWSZ - SHIPWIDTH) {
    shipDX += STEP;
    rightPressed -= STEP;
  }
  else if(shipShouldMove && leftPressed && shipDX > SHIPWIDTH - VIEWSZ) {
    shipDX -= STEP;
    leftPressed -= STEP;
  }
  else {
    shipShouldMove = false;
    rightPressed = false;
    leftPressed = false;
  }
  if (hitReadValue > 0) {
    if (numTime % ALIENSP == 0 ) {
      hitReadValue -= READVALUEUNIT;
    }
  }
  //ship
  glPushMatrix();
  glTranslatef(shipDX, 0, 0);
  glBegin(GL_POLYGON);
  glVertex2f(SHIPX + SHIPWIDTH*3/4, SHIPY - SHIPHEIGHT);
  glVertex2f(SHIPX + SHIPWIDTH/4, SHIPY - SHIPHEIGHT);
  glVertex2f(SHIPX + SHIPWIDTH/4, SHIPY + SHIPHEIGHT);
  glVertex2f(SHIPX + SHIPWIDTH*3/4, SHIPY + SHIPHEIGHT);
  glEnd();
  glPopMatrix();
}