#include "list.h"
#include "main.h"
#include "ship.h"

GLint angle = 90;
extern GLint numAliens;
extern GLint alienHit[ALIENWIDTH * ALIENHEIGHT];
extern GLdouble leftPressed, rightPressed, spacePressed, downPressed, shipShouldMove;
extern GLdouble shipXtrans, shipYtrans;
GLdouble alienDX = SHIPWIDTH, alienDY = ALIENROWINT, alienGroupV = SHIPWIDTH;
GLdouble fireBull = SHIPHEIGHT;
GLfloat loseTime = 8;

Alien aliens[ALIENHEIGHT * ALIENWIDTH];
Node *bulletShot = NULL;
Node *shipBull = NULL;
GLint numTime = 0;
GLint numHits = 0;
bool pause = false;
GLint nextStep = 0;
bool lost = false;

GLint sendBomb = 7;
GLint previousFireTime;

void alienShoot(GLint t) {
    //alien shoots
    int enemyColInd[ALIENHEIGHT];  //bottom most alive alien index

    for (int i = 0; i < ALIENHEIGHT; i++) {
        enemyColInd[i] = -1;
    }

    for (int i = 0; i < numAliens; i++) {
        if (aliens[i].alive) {
            enemyColInd[i%ALIENHEIGHT] = i;
        }
    }
    if (numTime % t <= 2) {
        for (int i = 0; i < ALIENHEIGHT; i++) {
            if((angle+i) % sendBomb == 2 && enemyColInd[i] - i > -8) {  //random bomb
                Bullet *b = makeBullet(aliens[enemyColInd[i]].x, aliens[enemyColInd[i]].y, alienDX, alienDY);
                Node *temp = makeNode(b);
                addNode(temp, &bulletShot);
            }
        }
    }
    if(bulletShot != NULL) {   //move attacking bullet
        for (Node *temp = bulletShot->h; temp != NULL; temp = temp->next) { //shot->h
            temp->data->dy -= ALIENROWINT;
        }
    }
}

bool win() {
    for (int i = 0; i < numAliens; i++) {
        if (aliens[i].alive) {
            return false;
        }
    }
    return true;
}

bool lose() {
    for (int i = 0; i < ALIENHEIGHT * ALIENWIDTH; i++) {
        if(aliens[i].alive) {
            if(aliens[i].x + alienDY <= -VIEWSZ  || numHits == NUMLIVES) {
                return true;
            }
        }
    }
    return false;
}

void draw() {
    glClear(GL_COLOR_BUFFER_BIT);
    //check if no alien left(WIN)
    if (win()) {
        glPushMatrix();
        glColor3f(numTime % 3, numTime %3+1, numTime%3+2);
        glRotatef(numTime, 0, 0, 1);
        glBegin(GL_POLYGON);
        glVertex2f(-VIEWSZ/2.0, 0);
        glVertex2f(0, -VIEWSZ/2.0);
        glVertex2f(VIEWSZ/2.0, 0);
        glVertex2f(0, VIEWSZ/2.0);
        glEnd();
        glPopMatrix();
    }

    //draw Aliens
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
    // glPushMatrix();
    // for (int i = 0; i < numAliens; i++) {

    //     if(aliens[i].alive || (!aliens[i].alive && alienHit[i] > -1)) {
    //         for (GLint x = 0; x < 3; x++) {
    //           printf("ai ya");
    //             glPushMatrix();
    //             glLoadIdentity();
    //             glTranslatef(alienDX, alienDY, 0);
    //             glColor3f(x/3, x/3, x/3);
    //             glTranslatef(aliens[i].x + SHIPWIDTH/2, aliens[i].y + SHIPWIDTH/2, 1);
    //             glScalef(x/3, x/3, 1);
    //             glTranslatef(-aliens[i].x - SHIPWIDTH/2, -aliens[i].y - SHIPWIDTH/2, -1);

    //             if(!aliens[x].alive && alienHit[x] > -1) {

    //                 glTranslatef(aliens[i].x + SHIPWIDTH/2, aliens[i].y + SHIPWIDTH/2, 0);
    //                 glScalef(READVALUEUNIT/alienHit[i], READVALUEUNIT/alienHit[i], 1);
    //                 glTranslatef(-(aliens[x].x + SHIPWIDTH/2), -(aliens[x].y + SHIPWIDTH/2), 0);
    //             }
    //             glTranslatef(aliens[i].x + SHIPWIDTH/2, aliens[x].y + SHIPWIDTH/2, 0);
    //             glRotatef(angle, 0, 0, 1);
    //             glTranslatef(-(aliens[x].x + SHIPWIDTH/2), -(aliens[x].y + SHIPWIDTH/2), 0);

    //             glBegin(GL_POLYGON);
    //             glVertex2f(aliens[x].x + SHIPWIDTH/2 - STEP*2/3, aliens[x].y + SHIPWIDTH/2 - STEP/6);
    //             glVertex2f(aliens[i].x + SHIPWIDTH/2 - STEP*2/3, aliens[x].y + SHIPWIDTH/2 + STEP/6);
    //             glVertex2f(aliens[x].x + SHIPWIDTH/2 - STEP/6, aliens[x].y + SHIPWIDTH/2 + STEP*2/3);
    //             glVertex2f(aliens[x].x + SHIPWIDTH/2 + STEP/6, aliens[x].y + SHIPWIDTH/2 + STEP*2/3);
    //             glVertex2f(aliens[x].x + SHIPWIDTH/2 + STEP*2/3, aliens[x].y + SHIPWIDTH/2 + STEP/6);
    //             glVertex2f(aliens[x].x + SHIPWIDTH/2 + STEP*2/3, aliens[x].y + SHIPWIDTH/2 - STEP/6);
    //             glVertex2f(aliens[x].x + SHIPWIDTH/2 + STEP/6, aliens[x].y + SHIPWIDTH/2 - STEP*2/3);
    //             glVertex2f(aliens[x].x+ SHIPWIDTH/2 - STEP/6, aliens[x].y + SHIPWIDTH/2 - STEP*2/3);
    //             glEnd();

    //             glPopMatrix();
    //         }

    //         if(aliens[i].alive == false && alienHit[i] > -1){

    //             if(numTime % (ALIENSP/2) == 0){
    //                 alienHit[i] += READVALUEUNIT;
    //             }
    //             if (alienHit[i] >= READVALUEUNIT*DEATHREAD) {
    //                 alienHit[i] = -1;
    //             }
    //         }
    //     }
    // }
    // glPopMatrix();
    // //check if fire and draw shipBullet
    // checkAndFire(SHIPX, SHIPY, SHIPWIDTH, SHIPHEIGHT);
    // drawShip();
    // //  check if alien reaches bottom or no life left
    // if(lose()) {
    //   lost = true;
    // }

    // if (lost) {
    //     if (numTime % 10 == 0 && loseTime > 0) {
    //         loseTime--;
    //     }

    //     glPushMatrix();
    //     glBegin(GL_POLYGON);
    //     glVertex2f(-VIEWSZ, -VIEWSZ);
    //     glVertex2f(VIEWSZ, -VIEWSZ);
    //     glVertex2f(VIEWSZ, VIEWSZ);
    //     glVertex2f(-VIEWSZ, VIEWSZ);
    //     glEnd();
    //     glPopMatrix();

    // }

    // //  update alien movements

    // if (pause) {
    //     if (nextStep) {
    //         numTime++;
    //         if (numTime % (ALIENSP*2) < ALIENSP) {
    //           angle += 1;
    //         }
    //         nextStep = false;
    //     }
    // }
    // if (!pause) {
    //     numTime++;
    //     if (numTime % (ALIENSP*2) < ALIENSP) {
    //       angle += 1;
    //     }
    // }
    // //MOVE ALIEN
    // if(alienDY > 0) {
    //     if (numTime % ALIENSP == 0) {
    //         if (alienDX < STEP || alienDX >= ALIENHEIGHT * STEP - STEP) {
    //             alienGroupV *= -1;
    //             alienDY -= SHIPHEIGHT;
    //         }
    //         alienShoot(ALIENSP*2);

    //         alienDX += alienGroupV;
    //     }

    // }
    // else if(alienDY > -3* ALIENROWINT) {
    //     if (numTime % (ALIENSP/2) == 0){
    //         if (alienDX < STEP || alienDX >= ALIENHEIGHT * STEP - STEP) {
    //             alienGroupV *= -1;
    //             alienDY -= SHIPHEIGHT;
    //         }
    //         alienShoot(ALIENSP);
    //         alienDX += alienGroupV;
    //     }
    // }
    // else {
    //     if (numTime% (ALIENSP/5) == 0){
    //         if (alienDX < STEP || alienDX >= ALIENHEIGHT * STEP - STEP) {
    //             alienGroupV *= -1;
    //             alienDY -= SHIPHEIGHT;
    //         }

    //         alienShoot(ALIENSP*2/5);
    //         alienDX += alienGroupV;
    //     }
    // }
    // //MOVE BULLET
    // if (numTime % (ALIENSP/10) == 0) {
    //     if(shipBull != NULL) {
    //         glColor3f(0, 1, 0);
    //         for (Node *temp = shipBull->h; temp != NULL; temp = temp->next) {
    //             temp->data->dy += SHIPHEIGHT;

    //             // if (temp->data->shootAngle == LEFTSHOOT) {
    //             //     temp->data->dx -= SHIPHEIGHT/3;
    //             // }
    //             // else if (temp->data->shootAngle == RIGHTSHOOT) {
    //             //     temp->data->dx += SHIPHEIGHT/3;
    //             // }
    //         }
    //     }
    // }
    // if(shipBull != NULL){
    //     Node *temp1, *temp2;
    //     for (temp1 = shipBull->h; temp1 != NULL; temp1 = temp2) {
    //         temp2 = temp1->next;
    //         if ((temp1->data->yPos + temp1->data->dy >VIEWSZ) ||
    //             (temp1->data->yPos + temp1->data->dy <-VIEWSZ) ||
    //             (temp1->data->xPos + temp1->data->dx >VIEWSZ) ||
    //             (temp1->data->xPos + temp1->data->dx <-VIEWSZ))
    //         {
    //             temp1->data->active = false;
    //             removeNode(temp1, &shipBull);
    //         }
    //     }
    // }
    // if ((numTime % 360 == 0) || (angle % 360 == 0)) {
    //     numTime = 0;
    // }
}

void init() {
    // Set to black
    glClearColor(0.0, 0.0, 0.0, 1.0);
    bulletShot = NULL;
    shipBull = NULL;


    for (int i = 0; i < numAliens; i++) {
        aliens[i].x = -VIEWSZ + SHIPWIDTH *(i % ALIENHEIGHT);
        aliens[i].y = VIEWSZ - ALIENROWINT * (i/ALIENHEIGHT) - SHIPHEIGHT *(ALIENWIDTH +1);
        aliens[i].alive = true;
        aliens[i].index = i;

        alienHit[i] = false;
    }
}

void framebuffer_resize(GLFWwindow* w, GLint width, GLint height){
    glViewport(0, 0, width, height);
}

void keyboard(GLFWwindow *w, int key, int scancode, int action, int mods) {
    switch(action) {
        case GLFW_PRESS :
            shipShouldMove = true;
            break;
        case GLFW_RELEASE :
            break;
        case GLFW_REPEAT :
            shipShouldMove = true;
            break;
        default :
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
            if(bulletShot != NULL){
                clearNodes(&bulletShot);
            }
            if(shipBull != NULL){
                clearNodes(&shipBull);
            }
            glfwSetWindowShouldClose(w, GL_TRUE);
            break;
        case GLFW_KEY_LEFT:
            leftPressed = SHIPWIDTH;
            rightPressed = false;
            printf("here");
            break;
        case GLFW_KEY_RIGHT:
            rightPressed = SHIPWIDTH;
            leftPressed = false;
            break;
        case GLFW_KEY_DOWN:
            downPressed = true;
            if (pause) {
                nextStep = true;
            }
            pause = true;
            break;
        case GLFW_KEY_UP:
            break;
        case GLFW_KEY_SPACE:
            if(numTime - previousFireTime > FIRECOOLINGTIME || previousFireTime - numTime > FIRECOOLINGTIME){
                spacePressed = true;
                previousFireTime = numTime;
            }
            break;
        case 'p' :
        case 'P':
            pause = !pause; //false
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
    window = glfwCreateWindow(WIDTH, HEIGHT, "hw2", NULL, NULL);
    if (!window){
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwSetWindowPos(window, WIDTH, HEIGHT);
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