#include "aliens.h"
GLdouble alienDX;
GLdouble alienDY;
GLdouble alienGroupV;
void drawAliens() {
  for (int x = 0; x < 3; x++) {
      glPushMatrix();
      glLoadIdentity();
      glTranslatef(alienDX, alienDY, 0);
      glColor3f(x/3, x/3, x/3);
      glTranslatef(aliens[i].x + SHIPWIDTH/2, aliens[i].y + SHIPWIDTH/2, 1);
      glScalef(x/3, x/3, 1);
      glTranslatef(-aliens[i].x - SHIPWIDTH/2, -aliens[i].y - SHIPWIDTH/2, -1);

      if(aliens[x].alive == false && alienDeathAnimationValue[x] > -1) {
        glTranslatef(aliens[i].x + SHIPWIDTH/2, aliens[i].y + SHIPWIDTH/2, 0);
        glScalef(READVALUEUNIT/alienDeathAnimationValue[x], READVALUEUNIT/alienDeathAnimationValue[x], 1);
        glTranslatef(-(aliens[i].x + SHIPWIDTH/2), -(aliens[i].y + SHIPWIDTH/2), 0);
      }

      glTranslatef(aliens[i].x + SHIPWIDTH/2, aliens[i].y + SHIPWIDTH/2, 0);
      glRotatef(angle, 0, 0, 1);
      glTranslatef(-(aliens[i].x + SHIPWIDTH/2), -(aliens[i].y + SHIPWIDTH/2), 0);

      glBegin(GL_POLYGON);
      glVertex2f(aliens[i].x + SHIPWIDTH/2 - STEP*2/3, aliens[i].y + SHIPWIDTH/2 - STEP/6);
      glVertex2f(aliens[i].x + SHIPWIDTH/2 - STEP*2/3, aliens[i].y + SHIPWIDTH/2 + STEP/6);
      glVertex2f(aliens[i].x + SHIPWIDTH/2 - STEP/6, aliens[i].y + SHIPWIDTH/2 + STEP*2/3);
      glVertex2f(aliens[i].x + SHIPWIDTH/2 + STEP/6, aliens[i].y + SHIPWIDTH/2 + STEP*2/3);
      glVertex2f(aliens[i].x + SHIPWIDTH/2 + STEP*2/3, aliens[i].y + SHIPWIDTH/2 + STEP/6);
      glVertex2f(aliens[i].x + SHIPWIDTH/2 + STEP*2/3, aliens[i].y + SHIPWIDTH/2 - STEP/6);
      glVertex2f(aliens[i].x + SHIPWIDTH/2 + STEP/6, aliens[i].y + SHIPWIDTH/2 - STEP*2/3);
      glVertex2f(aliens[i].x+ SHIPWIDTH/2 - STEP/6, aliens[i].y + SHIPWIDTH/2 - STEP*2/3);
      glEnd();
      glPopMatrix();
    }
}
