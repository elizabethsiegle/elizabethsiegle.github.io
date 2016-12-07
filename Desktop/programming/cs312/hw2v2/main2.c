#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "objects.h"

#ifdef __APPLE__
#include <GLFW/glfw3.h>
#include <GL/gl.h>
#include <GL/glu.h>
#else
#include <GLFW/glfw3.h>
#include <GL/gl.h>
#include <GL/glu.h>
#endif

#define WIDCanvas 400
#define HEICanvas 400
#define VIEWSZIE 1
#define STEP .1   //STEP IS HALF SHIPWID
#define SHIPINIX 0
#define SHIPINIY -.85
#define SHIPWID .2
#define SHIPHEI .05
#define COLOFALIEN 7
#define ROWOFALIEN 5
#define INTERVALOFALIENROW .2 // alien radius
#define ALIENSPEED 60 // second
#define FIRECOOLINGTIME 30 // half a second
#define DEATHREAD 5
#define READVALUEUNIT 20.0
#define LIFETOTAL 5
#define FAILREAD 5




GLint angle =30;
GLint alienDeathAnimationValue[COLOFALIEN*ROWOFALIEN];
GLint numberOfgrids = VIEWSZIE/STEP;
GLdouble isLeftKeyPressed, isRightKeyPressed, isSpacePressed, isDownKeyPressed, shipShouldMove;
GLdouble shipXtrans, shipYtrans;
GLdouble alienXtrans = SHIPWID,alienYtrans = INTERVALOFALIENROW, alienGroupV = SHIPWID;
GLdouble fireMissileV = SHIPHEI;
GLint shipShoot = NORMALSHOOT;
GLdouble  failTimeCount = FAILREAD;

GLfloat shipColor[3] = {1, 1, 1};

Alien alienGroup[COLOFALIEN*ROWOFALIEN];
Node* AttackingMissile;
Node* shipMissile;
GLint timeCount = 0, hitReadValue = 0, hitTotal = 0;
GLint freezeForDebug = FALSE, NextStepInDebug;
GLint failFlag = FALSE;

GLint level = 7;  // define the interval of bumb releasing, best using a prime
GLint previousFireTime;



void dropBumb(int interval){
    //drop a bumb, twice the time of interval
    int enemyColIndex[COLOFALIEN];  // record the bottom most alive alien index
    
    for (int i=0; i<COLOFALIEN; i++) {
        enemyColIndex[i] = -1;
    }
    
    
    for (int i=0; i<COLOFALIEN*ROWOFALIEN; i++) {
        if (alienGroup[i].ifAlive) {
            enemyColIndex[i%COLOFALIEN] = i;
        }
    }
    
    if (timeCount % (interval) == 0){
        for (int i=0; i<COLOFALIEN; i++) {
            if((angle+i) % level == 2 && enemyColIndex[i] > -1){  //randomize bumb droping
                Node* tmp = makeNode(make_Missile(alienGroup[enemyColIndex[i]].centerX, alienGroup[enemyColIndex[i]].centerY, alienXtrans, alienYtrans, NORMALSHOOT));
                addNode(tmp, &AttackingMissile);
            }
        }
        
    }
    if(AttackingMissile != NULL){   //move attacking missile
        for (Node* tmp = AttackingMissile->head; tmp != NULL; tmp = tmp->next) {
            tmp->data->transY -= INTERVALOFALIENROW;
        }
    }
    
    
}

int ifWin(){
    for (int i=0; i<COLOFALIEN*ROWOFALIEN; i++) {
        if (alienGroup[i].ifAlive == TRUE) {
            return 0;
        }
    }
    return 1;
}

int ifFail(){
    for (int i=COLOFALIEN*ROWOFALIEN; i>0; --i) {
        if(alienGroup[i].ifAlive == TRUE){
            
            if(alienGroup[i].centerY + alienYtrans <= -VIEWSZIE  || hitTotal == LIFETOTAL){
                //printf("fail");
                return 1;
            }
        }
    }
    return 0;
}

void draw(){
    // clear to background color
    glClear(GL_COLOR_BUFFER_BIT);
    
    //
    //
    //  check if no alien left(WIN)
    //
    //
    
    
    if (ifWin()) {
        glPushMatrix();
        glColor3f(timeCount%3, timeCount%3+1, timeCount%3+2);
        
        glRotatef(timeCount, 0, 0, 1);
        
        glBegin(GL_POLYGON);
        
        glVertex2f(-VIEWSZIE/2.0, 0);
        glVertex2f(0, -VIEWSZIE/2.0);
        glVertex2f(VIEWSZIE/2.0, 0);
        glVertex2f(0, VIEWSZIE/2.0);
        glEnd();
        glPopMatrix();
    }
    
    //
    //
    //  draw Aliens
    //
    //
    
    glPushMatrix();
    
    
    for (int i=0; i<COLOFALIEN*ROWOFALIEN; i++) {
        
        if(alienGroup[i].ifAlive == TRUE || (alienGroup[i].ifAlive == FALSE && alienDeathAnimationValue[i] > -1)){
            
            for (float j = 3.0; j>0; j--) {
                glPushMatrix();
                
                glLoadIdentity();
                glTranslatef(alienXtrans, alienYtrans, 0);
                
                glColor3f(j/3, j/3, j/3);
                glTranslatef(alienGroup[i].centerX + SHIPWID/2, alienGroup[i].centerY + SHIPWID/2, 1);
                glScalef(j/3, j/3, 1);
                glTranslatef(-alienGroup[i].centerX - SHIPWID/2, -alienGroup[i].centerY - SHIPWID/2, -1);
                
                if(alienGroup[i].ifAlive == FALSE && alienDeathAnimationValue[i] > -1){
                    
                    glTranslatef(alienGroup[i].centerX + SHIPWID/2, alienGroup[i].centerY + SHIPWID/2, 0);
                    glScalef(READVALUEUNIT/alienDeathAnimationValue[i], READVALUEUNIT/alienDeathAnimationValue[i], 1);
                    glTranslatef(-(alienGroup[i].centerX + SHIPWID/2), -(alienGroup[i].centerY + SHIPWID/2), 0);
                }
                
                
                glTranslatef(alienGroup[i].centerX + SHIPWID/2, alienGroup[i].centerY + SHIPWID/2, 0);
                glRotatef(angle, 0, 0, 1);
                glTranslatef(-(alienGroup[i].centerX + SHIPWID/2), -(alienGroup[i].centerY + SHIPWID/2), 0);
                
                glBegin(GL_POLYGON);
                glVertex2f(alienGroup[i].centerX + SHIPWID/2 - STEP*2/3, alienGroup[i].centerY + SHIPWID/2 - STEP/6);
                glVertex2f(alienGroup[i].centerX + SHIPWID/2 - STEP*2/3, alienGroup[i].centerY + SHIPWID/2 + STEP/6);
                glVertex2f(alienGroup[i].centerX + SHIPWID/2 - STEP/6, alienGroup[i].centerY + SHIPWID/2 + STEP*2/3);
                glVertex2f(alienGroup[i].centerX + SHIPWID/2 + STEP/6, alienGroup[i].centerY + SHIPWID/2 + STEP*2/3);
                glVertex2f(alienGroup[i].centerX + SHIPWID/2 + STEP*2/3, alienGroup[i].centerY + SHIPWID/2 + STEP/6);
                glVertex2f(alienGroup[i].centerX + SHIPWID/2 + STEP*2/3, alienGroup[i].centerY + SHIPWID/2 - STEP/6);
                glVertex2f(alienGroup[i].centerX + SHIPWID/2 + STEP/6, alienGroup[i].centerY + SHIPWID/2 - STEP*2/3);
                glVertex2f(alienGroup[i].centerX + SHIPWID/2 - STEP/6, alienGroup[i].centerY + SHIPWID/2 - STEP*2/3);
                glEnd();
                
                glPopMatrix();
            }
            
            if(alienGroup[i].ifAlive == FALSE && alienDeathAnimationValue[i] > -1){
                
                if(timeCount % (ALIENSPEED/2) == 0){
                    alienDeathAnimationValue[i] += READVALUEUNIT;
                }
                if (alienDeathAnimationValue[i] >= READVALUEUNIT*DEATHREAD) {
                    alienDeathAnimationValue[i] = -1;
                }
                
                
            }
            
        }
        
    }
    glPopMatrix();
    
    
    
    //
    //
    //  check if fire and draw shipMissile
    //
    //
    
    if (isSpacePressed) {
        Node* tmp = makeNode(make_Missile(SHIPINIX, SHIPINIY, shipXtrans, 0, shipShoot));
        addNode(tmp, &shipMissile);
        isSpacePressed = FALSE;
    }
    if(shipMissile != NULL){
        for (Node* tmp = shipMissile->head; tmp != NULL; tmp = tmp->next) {
            GLdouble centerXofThis = tmp->data->centerX + STEP;
            GLdouble centerYofThis = tmp->data->centerY + SHIPHEI;

            if(tmp->data->ifValid == TRUE){
                if(freezeForDebug){
                    
                    printf("shipMissile x:%f, y:%f \n", tmp->data->centerX+tmp->data->transX, tmp->data->centerY+tmp->data->transY);
                }
                glPushMatrix();
                
                glColor3f(1, .6, .6);
                glTranslatef(tmp->data->transX, tmp->data->transY, 0);
                
                glBegin(GL_POLYGON);
                glVertex2f(centerXofThis - SHIPHEI*2/3, centerYofThis - SHIPHEI/3);
                glVertex2f(centerXofThis - SHIPHEI*2/3, centerYofThis + SHIPHEI/3);
                glVertex2f(centerXofThis - SHIPHEI/3, centerYofThis + SHIPHEI*2/3);
                glVertex2f(centerXofThis + SHIPHEI/3, centerYofThis + SHIPHEI*2/3);
                glVertex2f(centerXofThis + SHIPHEI*2/3, centerYofThis + SHIPHEI/3);
                glVertex2f(centerXofThis + SHIPHEI*2/3, centerYofThis - SHIPHEI/3);
                glVertex2f(centerXofThis + SHIPHEI/3, centerYofThis - SHIPHEI*2/3);
                glVertex2f(centerXofThis - SHIPHEI/3, centerYofThis - SHIPHEI*2/3);
                glEnd();
                
                glPopMatrix();
                glPushMatrix();
                
                glColor3f(1, 1, 0);
                glTranslatef(tmp->data->transX, tmp->data->transY, 0);
                glTranslatef(centerXofThis, centerYofThis, 1);
                glScalef(.4, .4, 1);
                glTranslatef(-centerXofThis, -centerYofThis, -1);
                
                glBegin(GL_POLYGON);
                glVertex2f(centerXofThis - SHIPHEI*2/3, centerYofThis - SHIPHEI/3);
                glVertex2f(centerXofThis - SHIPHEI*2/3, centerYofThis + SHIPHEI/3);
                glVertex2f(centerXofThis - SHIPHEI/3, centerYofThis + SHIPHEI*2/3);
                glVertex2f(centerXofThis + SHIPHEI/3, centerYofThis + SHIPHEI*2/3);
                glVertex2f(centerXofThis + SHIPHEI*2/3, centerYofThis + SHIPHEI/3);
                glVertex2f(centerXofThis + SHIPHEI*2/3, centerYofThis - SHIPHEI/3);
                glVertex2f(centerXofThis + SHIPHEI/3, centerYofThis - SHIPHEI*2/3);
                glVertex2f(centerXofThis - SHIPHEI/3, centerYofThis - SHIPHEI*2/3);
                glEnd();
                
                glPopMatrix();
                
                
                for (int i=0; i<COLOFALIEN*ROWOFALIEN; i++) {
                    if (alienGroup[i].ifAlive) {
                        
                        if((centerXofThis + tmp->data->transX - alienGroup[i].centerX - alienXtrans - STEP)*(centerXofThis + tmp->data->transX - alienGroup[i].centerX - alienXtrans- STEP) +
                           (centerYofThis +tmp->data->transY - alienGroup[i].centerY -alienYtrans- SHIPHEI/2)*(centerYofThis + tmp->data->transY - alienGroup[i].centerY - alienYtrans - SHIPHEI/2)
                           < STEP*STEP){
                            alienGroup[i].ifAlive = FALSE;
                            alienDeathAnimationValue[i] = 0;
                            tmp->data->ifValid = FALSE;
                        }
                    }
                    
                }
                
                
                
            }
            
        }
    }
    
    
    
    //
    //
    //  draw Attacking missile in the linked list
    //
    //
    
    if(AttackingMissile != NULL){
        glColor3f(.6, 1, .6);
        for (Node* tmp = AttackingMissile->head; tmp != NULL; tmp = tmp->next) {
            GLdouble centerXofThis = tmp->data->centerX + STEP;
            GLdouble centerYofThis = tmp->data->centerY + SHIPHEI;
            
            if(freezeForDebug)
                printf("AttackingMissile x:%f, y:%f \n", tmp->data->centerX+tmp->data->transX, tmp->data->centerY+tmp->data->transY);
            
            
            if(tmp->data->ifValid == TRUE){
                glPushMatrix();
                if((centerXofThis + tmp->data->transX - SHIPINIX - shipXtrans - STEP)*(centerXofThis + tmp->data->transX - SHIPINIX - shipXtrans- STEP) +
                   (centerYofThis +tmp->data->transY - SHIPINIY - SHIPHEI/2)*(centerYofThis + tmp->data->transY - SHIPINIY - SHIPHEI/2)
                   < STEP*STEP){
                    tmp->data->ifValid = FALSE;
                    hitReadValue = READVALUEUNIT*DEATHREAD;
                    hitTotal++;
                    //printf("HIT!!\n");
                    //exit(0);
                }
                
                
                glTranslatef(tmp->data->transX, tmp->data->transY, 0);
                
                glBegin(GL_POLYGON);
                glVertex2f(centerXofThis - SHIPHEI*2/3, centerYofThis - SHIPHEI/6);
                glVertex2f(centerXofThis - SHIPHEI*2/3, centerYofThis + SHIPHEI/6);
                glVertex2f(centerXofThis - SHIPHEI/6, centerYofThis + SHIPHEI*2/3);
                glVertex2f(centerXofThis + SHIPHEI/6, centerYofThis + SHIPHEI*2/3);
                glVertex2f(centerXofThis + SHIPHEI*2/3, centerYofThis + SHIPHEI/6);
                glVertex2f(centerXofThis + SHIPHEI*2/3, centerYofThis - SHIPHEI/6);
                glVertex2f(centerXofThis + SHIPHEI/6, centerYofThis - SHIPHEI*2/3);
                glVertex2f(centerXofThis - SHIPHEI/6, centerYofThis - SHIPHEI*2/3);
                glEnd();
                
                glPopMatrix();
            }
            
        }
    }
    
    //
    //
    //  draw SHIP
    //
    //
    glColor3f(1, 1, 1);
    glPushMatrix();
    
    //
    //  update if key pressed,   ship move horizontally animation
    //
    
    if (!freezeForDebug) {
        
        if(shipShouldMove && isRightKeyPressed && shipXtrans <= VIEWSZIE - SHIPWID - STEP){
            shipXtrans += STEP;
            isRightKeyPressed -= STEP;
        }
        else if(shipShouldMove && isLeftKeyPressed && shipXtrans > SHIPWID - VIEWSZIE - STEP){
            shipXtrans -= STEP;
            isLeftKeyPressed -= STEP;
        }else{
            shipShouldMove = FALSE;
            isRightKeyPressed = FALSE;
            isLeftKeyPressed = FALSE;
        }
    }else{
        printf("ship x:%f, y:%f \n", SHIPINIX+shipXtrans, SHIPINIY+shipYtrans);
        
    }
    
    if (hitReadValue > 0) {
        
        if (timeCount % (ALIENSPEED/3) == 0 ) {
            hitReadValue -= READVALUEUNIT;
        }
        shipColor[0]=1;
        shipColor[1]=1*(1-(hitReadValue+0.0)/(READVALUEUNIT*DEATHREAD));
        shipColor[2]=1*(1-(hitReadValue+0.0)/(READVALUEUNIT*DEATHREAD));
    }
    
    
    //tail flame
    glPushMatrix();
    glTranslatef(shipXtrans, 0, 0);
    
    glColor3f(.6, .8, 1);
    glBegin(GL_POLYGON);
    glVertex2f(SHIPINIX + SHIPWID*2/5, SHIPINIY+ SHIPHEI);
    glVertex2f(SHIPINIX + SHIPWID*2/5, SHIPINIY- SHIPHEI*2);
    glVertex2f(SHIPINIX + SHIPWID/2, SHIPINIY- SHIPHEI*2.5);
    glVertex2f(SHIPINIX + SHIPWID*3/5, SHIPINIY- SHIPHEI*2);
    glVertex2f(SHIPINIX + SHIPWID*3/5, SHIPINIY + SHIPHEI);
    glEnd();
    glPopMatrix();
    
    //ship body
    glPushMatrix();
    glTranslatef(shipXtrans, 0, 0);
    if(hitReadValue <= 0){
        glColor3f(1, 1-(0.0+hitTotal)/LIFETOTAL, 1-(0.0+hitTotal)/LIFETOTAL);
    }else{
        glColor3f(shipColor[0], shipColor[1], shipColor[2]);
    }
    glBegin(GL_POLYGON);
    glVertex2f(SHIPINIX + SHIPWID*4/5, SHIPINIY + 1.5*SHIPHEI);
    glVertex2f(SHIPINIX + SHIPWID*3.5/5, SHIPINIY + 2.5*SHIPHEI);
    glVertex2f(SHIPINIX + SHIPWID*1.5/5, SHIPINIY + 2.5*SHIPHEI);
    glVertex2f(SHIPINIX + SHIPWID/5, SHIPINIY + 1.5*SHIPHEI);
    glVertex2f(SHIPINIX + SHIPWID/5, SHIPINIY - SHIPHEI);
    glVertex2f(SHIPINIX + SHIPWID*4/5, SHIPINIY - SHIPHEI);
    glEnd();
    
    glColor3f(.7, .7, .7);
    glBegin(GL_POLYGON);
    glVertex2f(SHIPINIX + SHIPWID/3, SHIPINIY + SHIPWID/3 );
    glVertex2f(SHIPINIX + SHIPWID/3, SHIPINIY + SHIPWID/6 );
    glVertex2f(SHIPINIX + SHIPWID*2/5, SHIPINIY);
    glVertex2f(SHIPINIX + SHIPWID*3/5, SHIPINIY);
    glVertex2f(SHIPINIX + SHIPWID*2/3, SHIPINIY + SHIPWID/6 );
    glVertex2f(SHIPINIX + SHIPWID*2/3, SHIPINIY + SHIPWID/3 );
    glVertex2f(SHIPINIX + SHIPWID*3/5, SHIPINIY + SHIPWID/2);
    glVertex2f(SHIPINIX + SHIPWID*2/5, SHIPINIY + SHIPWID/2);
    glEnd();
    
    glPopMatrix();
    
    glColor3f(.5, .5, .5);
    
    //cannon
    glPushMatrix();
    glTranslatef(shipXtrans, 0, 0);
    glTranslatef(SHIPINIX + SHIPWID/2, SHIPINIY + SHIPHEI/2, 0);
    glRotatef(30*(-shipShoot+1), 0, 0, 1);
    glTranslatef(-SHIPINIX - SHIPWID/2, -SHIPINIY - SHIPHEI/2, 0);
    
    
    glBegin(GL_POLYGON);
    
    glVertex2f(SHIPINIX + SHIPWID*2/5, SHIPINIY + SHIPHEI );
    glVertex2f(SHIPINIX + SHIPWID*3/7, SHIPINIY + SHIPHEI/2 );
    glVertex2f(SHIPINIX + SHIPWID*4/7, SHIPINIY + SHIPHEI/2 );
    glVertex2f(SHIPINIX + SHIPWID*3/5, SHIPINIY + SHIPHEI );
    glVertex2f(SHIPINIX + SHIPWID*3/5, SHIPINIY + 4*SHIPHEI );
    glVertex2f(SHIPINIX + SHIPWID*2/5, SHIPINIY + 4*SHIPHEI);
    glEnd();
    glPopMatrix();
    
    
    //lowest ship body
    
    glPushMatrix();
    glTranslatef(shipXtrans, 0, 0);
    if(hitReadValue <= 0){
        glColor3f(1, 1-(0.0+hitTotal)/LIFETOTAL, 1-(0.0+hitTotal)/LIFETOTAL);
    }else{
        glColor3f(shipColor[0], shipColor[1], shipColor[2]);
    }
    glBegin(GL_POLYGON);
    glVertex2f(SHIPINIX + SHIPWID/11, SHIPINIY);
    glVertex2f(SHIPINIX + SHIPWID*10/11, SHIPINIY);
    glVertex2f(SHIPINIX + SHIPWID, SHIPINIY - SHIPHEI);
    glVertex2f(SHIPINIX, SHIPINIY - SHIPHEI);
    glEnd();
    
    glColor3f(.7, .7, .7);
    glBegin(GL_POLYGON);
    glVertex2f(SHIPINIX, SHIPINIY- SHIPHEI*4/5);
    glVertex2f(SHIPINIX + SHIPWID, SHIPINIY- SHIPHEI*4/5);
    glVertex2f(SHIPINIX + SHIPWID, SHIPINIY - SHIPHEI);
    glVertex2f(SHIPINIX, SHIPINIY - SHIPHEI);
    glEnd();
    glPopMatrix();
    
    
    //
    //
    //  check if alien reaches bottom or no life left
    //
    //
    
    
    if(ifFail())
        failFlag = TRUE;
    
    if (failFlag == TRUE) {
        
        if (timeCount % 10 == 0 && failTimeCount > 0) {
            failTimeCount --;
        }
        
        glPushMatrix();
        glBegin(GL_POLYGON);
        glColor3f(failTimeCount/FAILREAD, failTimeCount/FAILREAD, failTimeCount/FAILREAD);
        glVertex2f(-VIEWSZIE, -VIEWSZIE);
        glVertex2f(VIEWSZIE, -VIEWSZIE);
        glVertex2f(VIEWSZIE, VIEWSZIE);
        glVertex2f(-VIEWSZIE, VIEWSZIE);
        glEnd();
        glPopMatrix();
        
    }
    
    
    
    //
    //
    //  update alien movements
    //
    //
    //
    
    
    if (freezeForDebug == TRUE) {
        printf("\n DEBUG INFOR \n");
        printf("timeCount: %d\n", timeCount);
        printf("angle: %d\n", angle);
        printf("numberOfgrids: %d\n", numberOfgrids);
        printf("isLeftKeyPressed: %f, \nisRightKeyPressed: %f, \nisDownKeyPressed: %f, \nshipShouldMove: %f\n",
               isLeftKeyPressed, isRightKeyPressed, isDownKeyPressed, shipShouldMove);
        printf("shipXtrans: %f, shipYtrans: %f\n", shipXtrans, shipYtrans);
        printf("alienXtrans: %f, alienYtrans: %f, alienGroupV: %f\n \n \n",
               alienXtrans, alienYtrans, alienGroupV);
        for (int i=0; i<COLOFALIEN*ROWOFALIEN; i++) {
            printf("alienGroup[%d], alienGroup[i].centerX: %f, alienGroup[i].centerY: %f, alienGroup[i].ifAlive: %d\n",
                   i, alienGroup[i].centerX, alienGroup[i].centerY, alienGroup[i].ifAlive);
            
        }
        
        
        
        if (NextStepInDebug == TRUE) {
            timeCount ++;
            if (timeCount % (ALIENSPEED*2) < ALIENSPEED)
                angle += 1;
            NextStepInDebug = FALSE;
        }
        
        
    }else{
        
        timeCount++;
        if (timeCount % (ALIENSPEED*2) < ALIENSPEED)
            angle += 1;
        
    }
    
    
    //
    //  MOVE ALIEN
    //
    
    if(alienYtrans > 0){
        if (timeCount % ALIENSPEED == 0){
            if (alienXtrans < STEP || alienXtrans >= COLOFALIEN*STEP - STEP) {
                alienGroupV *= -1;
                alienYtrans -= SHIPHEI;
            }
            dropBumb(ALIENSPEED*2);
            
            alienXtrans += alienGroupV;
        }
        
        
    }else if(alienYtrans > -3*INTERVALOFALIENROW){
        if (timeCount % (ALIENSPEED/2) == 0){
            if (alienXtrans < STEP || alienXtrans >= COLOFALIEN*STEP - STEP) {
                alienGroupV *= -1;
                alienYtrans -= SHIPHEI;
            }
            
            dropBumb(ALIENSPEED);
            
            alienXtrans += alienGroupV;
        }
    }else{
        if (timeCount % (ALIENSPEED/5) == 0){
            if (alienXtrans < STEP || alienXtrans >= COLOFALIEN*STEP - STEP) {
                alienGroupV *= -1;
                alienYtrans -= SHIPHEI;
            }
            
            dropBumb(ALIENSPEED*2/5);
            
            alienXtrans += alienGroupV;
        }
    }
    
    //
    //  MOVE SHIP MISSILE
    //
    
    if (timeCount % (ALIENSPEED/10) == 0){
        if(shipMissile != NULL){
            glColor3f(0, 1, 0);
            for (Node* tmp = shipMissile->head; tmp != NULL; tmp = tmp->next) {
                tmp->data->transY += SHIPHEI;
                
                if (tmp->data->shootAngle == LEFTSHOOT) {
                    tmp->data->transX -= SHIPHEI/3;
                }else if (tmp->data->shootAngle == RIGHTSHOOT) {
                    tmp->data->transX += SHIPHEI/3;
                }
            }
        }
    }
    
    
    if(shipMissile != NULL){
        Node *tmp, *tmp2;
        for (tmp = shipMissile->head; tmp != NULL; tmp = tmp2) {
            tmp2 = tmp->next;
            if ((tmp->data->centerY + tmp->data->transY >VIEWSZIE) ||
                (tmp->data->centerY + tmp->data->transY <-VIEWSZIE) ||
                (tmp->data->centerX + tmp->data->transX >VIEWSZIE) ||
                (tmp->data->centerX + tmp->data->transX <-VIEWSZIE))
            {
                tmp->data->ifValid = FALSE;
                deleteNode(tmp, &shipMissile);
            }
        }
    }

    
    
    
    
    if (timeCount % 360 == 0) {
        timeCount = 0;
    }
    
    if (angle % 360 == 0) {
        timeCount = 0;
    }
    
    
}


void init() {
    // Set the clear color to black
    glClearColor(0.0, 0.0, 0.0, 1.0);
    AttackingMissile = NULL;
    shipMissile = NULL;
    
    
    for (int i=0; i<COLOFALIEN*ROWOFALIEN; i++) {
        alienGroup[i].centerX = -VIEWSZIE + SHIPWID*(i%COLOFALIEN);
        alienGroup[i].centerY = VIEWSZIE - INTERVALOFALIENROW*(i/COLOFALIEN) - SHIPHEI*(ROWOFALIEN+1);
        alienGroup[i].ifAlive = TRUE;
        alienGroup[i].index = i;
        
        alienDeathAnimationValue[i] = -1;
    }
    
}


void framebuffer_resize(GLFWwindow* w, GLint width, GLint height){
    glViewport(0, 0, width, height);
}

void keyboard(GLFWwindow *w, int key, int scancode, int action, int mods) {
    switch(action) {
        case GLFW_PRESS :
            shipShouldMove = TRUE;
            break;
        case GLFW_RELEASE :
            break;
        case GLFW_REPEAT :
            shipShouldMove = TRUE;
            break;
        default :
            printf("shouldn't happen\t");
            break;
    }
    
    switch(mods) {
        case GLFW_MOD_SHIFT :
            break;
        case GLFW_MOD_ALT :
            break;
        case GLFW_MOD_CONTROL :
            break;
        default :
            break;
    }
    
    switch(key) {
        case GLFW_KEY_ESCAPE :
        case 'q' :
        case 'Q':
            if(AttackingMissile != NULL){
                clearNode(&AttackingMissile);
            }
            if(shipMissile != NULL){
                clearNode(&shipMissile);
            }
            glfwSetWindowShouldClose(w, GL_TRUE);
            break;
        case GLFW_KEY_LEFT:
            isLeftKeyPressed = SHIPWID;
            isRightKeyPressed = FALSE;
            break;
        case GLFW_KEY_RIGHT:
            isRightKeyPressed = SHIPWID;
            isLeftKeyPressed = FALSE;
            break;
        case GLFW_KEY_DOWN:
            isDownKeyPressed = TRUE;
            if (freezeForDebug == TRUE) {
                NextStepInDebug = TRUE;
            }
            freezeForDebug = TRUE;
            break;
        case GLFW_KEY_UP:
            break;
        case GLFW_KEY_SPACE:
            if(timeCount - previousFireTime > FIRECOOLINGTIME || previousFireTime - timeCount > FIRECOOLINGTIME){
                isSpacePressed = TRUE;
                previousFireTime = timeCount;
            }
            break;
        case 'r' :
        case 'R':
            freezeForDebug = FALSE;
            break;
        case 'w' :
        case 'W':
            shipShoot = NORMALSHOOT;
            break;
        case 'a' :
        case 'A':
            shipShoot = LEFTSHOOT;
            break;
        case 'd' :
        case 'D':
            shipShoot = RIGHTSHOOT;
            break;
        default:
            break;
    }
}


int main(int argc, char **argv) {
    GLFWwindow* window;
    
    // Initialize the library
    if (!glfwInit())
        exit(EXIT_FAILURE);
    
    // Create a window and its OpenGL context
    window = glfwCreateWindow(WIDCanvas, HEICanvas, "Hello GL!", NULL, NULL);
    if (!window){
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    
    glfwSetWindowPos(window, WIDCanvas, HEICanvas);
    // Make the window's context current
    glfwMakeContextCurrent(window);
    
    
    // Assign reshape() to be the function called whenever
    // a reshape event occurs
    glfwSetFramebufferSizeCallback(window, framebuffer_resize);
    
    // Assign keyboard() to be the function called whenever
    // a key is pressed or released
    glfwSetKeyCallback(window, keyboard);
    
    init();
    // Loop until the user closes the window
    while (!glfwWindowShouldClose(window)) {
        
        draw();
        
        // Swap front and back buffers
        glfwSwapBuffers(window);
        
        // Poll for and process events
        glfwPollEvents();
    }
    
    glfwTerminate();
    exit(EXIT_SUCCESS);
}