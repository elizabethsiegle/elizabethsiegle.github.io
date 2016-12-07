#include "list.h"
#include "main.h"
#include "aliens.h"
GLint angle = 30;
GLint alienDeathAnimationValue[ALIENCOL*ALIENROW];
GLdouble isLeftKeyPressed, isRightKeyPressed, isSpacePressed, isDownKeyPressed, shipShouldMove;
GLdouble shipdx, shipdy;
extern GLdouble alienDX = SHIPWIDTH, alienDY = ALIENROWINT, alienGroupV = SHIPWIDTH;
GLdouble fireMissileV = SHIPHEIGHT;
GLint shipShoot = NORMALSHOOT;
GLdouble failTimeCount = FAILREAD;

GLfloat shipCol[3] = {0.0, 0.0, 0.0};

Alien aliens[ALIENCOL*ALIENROW];
Node *sentBullet;
Node* shipBull;
GLint numTime = 0, hitReadValue = 0, numHits = 0;
GLint freezeForDebug = false, NextStepInDebug;
GLint failFlag = false;

GLint level = 7;  // define the interval of bumb releasing, best using a prime
GLint previousFireTime;

void alienShoot(GLint t) {
    //
    int enemyColInd[ALIENCOL];  // record the bottom most alive alien index
    for (int i = 0; i < ALIENCOL; i++) {
        enemyColInd[i] = -1;
    }
    for (int i = 0; i < ALIENCOL*ALIENROW; i++) {
        if (aliens[i].alive) {
            enemyColInd[i%ALIENCOL] = i;
        }
    }
    if (numTime % (t) == 2){
        for (int i = 0; i < ALIENCOL; i++) {
            if((angle+i) % level == 2 && enemyColInd[i] > -1) {
                Bullet *b = makeBullet(aliens[enemyColInd[i]].x, aliens[enemyColInd[i]].y, alienDX, alienDY, 1); //NORMALSHOOT
                Node *temp = makeNode(b);
                add(temp, &sentBullet);
            }
        }

    }
    if(sentBullet != NULL) {   //move attacking missile
        for (Node* temp = sentBullet->h; temp != NULL; temp = temp->next) {
            temp->data->dy -= ALIENROWINT;
        }
    }
}

bool win() {
    for (int i = 0; i < ALIENCOL * ALIENROW; i++) {
        if (aliens[i].alive == true) {
            return false;
        }
    }
    return true;
}

bool lose() {
    for (int i = 0; i < ALIENCOL * ALIENROW; i++) {
        if(aliens[i].alive == true) {
            if(aliens[i].x + alienDY <= -VIEW  || numHits == LIFETOTAL) {
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
        glVertex2f(-VIEW/2.0, 0);
        glVertex2f(0, -VIEW/2.0);
        glVertex2f(VIEW/2.0, 0);
        glVertex2f(0, VIEW/2.0);
        glEnd();
        glPopMatrix();
    }

    //  draw Aliens
    glPushMatrix();
    for (int i = 0; i < ALIENCOL * ALIENROW; i++) {
        if(aliens[i].alive == true || (aliens[i].alive == false && alienDeathAnimationValue[i] > -1)) {
          drawAliens();

            if(aliens[i].alive == false && alienDeathAnimationValue[i] > -1) {
                if(numTime % (ALIENSP/2) == 0){
                    alienDeathAnimationValue[i] += READVALUEUNIT;
                }
                if (alienDeathAnimationValue[i] >= READVALUEUNIT*DEATHREAD) {
                    alienDeathAnimationValue[i] = -1;
                }
            }
        }
    }
    glPopMatrix();
    //  check if fire and draw shipMissile

    if (isSpacePressed) {
      Bullet *tempBull = makeBullet(SHIPINITX, SHIPINITY, shipdx, 0, shipShoot);
      Node *temp = makeNode(tempBull);
      add(temp, &shipBull);
      isSpacePressed = false;
    }
    if(shipBull != NULL){
        for (Node* temp = shipBull->h; temp != NULL; temp = temp->next) {
            GLdouble xMid = temp->data->xPos + STEP;
            GLdouble yMid = temp->data->yPos + SHIPHEIGHT;

            if(temp->data->active == true) {
                if(freezeForDebug) {
                    printf("shipMissile x:%f, y:%f \n", temp->data->xPos + temp->data->dx, temp->data->yPos + temp->data->dy);
                }
                glPushMatrix();

                glColor3f(1, .6, .6);
                glTranslatef(temp->data->dx, temp->data->dy, 0);

                glBegin(GL_POLYGON);
                glVertex2f(xMid - SHIPHEIGHT*2/3, yMid - SHIPHEIGHT/3);
                glVertex2f(xMid - SHIPHEIGHT*2/3, yMid + SHIPHEIGHT/3);
                glVertex2f(xMid - SHIPHEIGHT/3, yMid + SHIPHEIGHT*2/3);
                glVertex2f(xMid + SHIPHEIGHT/3, yMid + SHIPHEIGHT*2/3);
                glVertex2f(xMid + SHIPHEIGHT*2/3, yMid + SHIPHEIGHT/3);
                glVertex2f(xMid + SHIPHEIGHT*2/3, yMid - SHIPHEIGHT/3);
                glVertex2f(xMid + SHIPHEIGHT/3, yMid - SHIPHEIGHT*2/3);
                glVertex2f(xMid - SHIPHEIGHT/3, yMid - SHIPHEIGHT*2/3);
                glEnd();

                glPopMatrix();
                glPushMatrix();

                glColor3f(1, 1, 0);
                glTranslatef(temp->data->dx, temp->data->dy, 0);
                glTranslatef(xMid, yMid, 1);
                glScalef(.4, .4, 1);
                glTranslatef(-xMid, -yMid, -1);

                glBegin(GL_POLYGON);
                glVertex2f(xMid - SHIPHEIGHT*2/3, yMid - SHIPHEIGHT/3);
                glVertex2f(xMid - SHIPHEIGHT*2/3, yMid + SHIPHEIGHT/3);
                glVertex2f(xMid - SHIPHEIGHT/3, yMid + SHIPHEIGHT*2/3);
                glVertex2f(xMid + SHIPHEIGHT/3, yMid + SHIPHEIGHT*2/3);
                glVertex2f(xMid + SHIPHEIGHT*2/3, yMid + SHIPHEIGHT/3);
                glVertex2f(xMid + SHIPHEIGHT*2/3, yMid - SHIPHEIGHT/3);
                glVertex2f(xMid + SHIPHEIGHT/3, yMid - SHIPHEIGHT*2/3);
                glVertex2f(xMid - SHIPHEIGHT/3, yMid - SHIPHEIGHT*2/3);
                glEnd();
                glPopMatrix();
                for (int i = 0; i < ALIENCOL*ALIENROW; i++) {
                    if (aliens[i].alive) {
                        if((xMid + temp->data->dx - aliens[i].x - alienDX - STEP)*(xMid + temp->data->dx - aliens[i].x - alienDX- STEP) +
                           (yMid +temp->data->dy - aliens[i].y -alienDY- SHIPHEIGHT/2)*(yMid + temp->data->dy - aliens[i].y - alienDY - SHIPHEIGHT/2)
                           < STEP*STEP){
                            aliens[i].alive = false;
                            alienDeathAnimationValue[i] = 0;
                            temp->data->active = false;
                        }
                    }
                }
            }
        }
    }
    //bullet in LL
    if(sentBullet != NULL){
        glColor3f(.2, .4, .6);
        for (Node *temp = sentBullet->h; temp != NULL; temp = temp->next) {
            GLdouble xMid = temp->data->xPos + STEP;
            GLdouble yMid = temp->data->yPos + SHIPHEIGHT;
            if(freezeForDebug) {
                printf("AttackingMissile x:%f, y:%f \n", temp->data->xPos + temp->data->dx, temp->data->dy +temp->data->dy);
            }
            if(temp->data->active == true) {
                glPushMatrix();
                if((xMid + temp->data->dx - SHIPINITX - shipdx - STEP)*(xMid + temp->data->dx - SHIPINITX - shipdx- STEP) +
                   (yMid +temp->data->dy - SHIPINITY - SHIPHEIGHT/2)*(yMid + temp->data->dy - SHIPINITY - SHIPHEIGHT/2)
                   < STEP*STEP){
                    temp->data->active = false;
                    hitReadValue = READVALUEUNIT*DEATHREAD;
                    numHits++;
                }
                glTranslatef(temp->data->dx, temp->data->dy, 0);
                glBegin(GL_POLYGON);
                glVertex2f(xMid - SHIPHEIGHT*2/3, yMid - SHIPHEIGHT/6);
                glVertex2f(xMid - SHIPHEIGHT*2/3, yMid + SHIPHEIGHT/6);
                glVertex2f(xMid - SHIPHEIGHT/6, yMid + SHIPHEIGHT*2/3);
                glVertex2f(xMid + SHIPHEIGHT/6, yMid + SHIPHEIGHT*2/3);
                glVertex2f(xMid + SHIPHEIGHT*2/3, yMid + SHIPHEIGHT/6);
                glVertex2f(xMid + SHIPHEIGHT*2/3, yMid - SHIPHEIGHT/6);
                glVertex2f(xMid + SHIPHEIGHT/6, yMid - SHIPHEIGHT*2/3);
                glVertex2f(xMid - SHIPHEIGHT/6, yMid - SHIPHEIGHT*2/3);
                glEnd();
                glPopMatrix();
            }
        }
    }
    //  draw SHIP
    glColor3f(0, 0, 1);
    glPushMatrix();

    //  update if key pressed,   ship move horizontally animation

    if (!freezeForDebug) {
        if(shipShouldMove && isRightKeyPressed && shipdx <= VIEW - SHIPWIDTH - STEP) {
            shipdx += STEP;
            isRightKeyPressed -= STEP;
        }
        else if(shipShouldMove && isLeftKeyPressed && shipdx > SHIPWIDTH - VIEW - STEP) {
            shipdx -= STEP;
            isLeftKeyPressed -= STEP;
        }
        else {
            shipShouldMove = false;
            isRightKeyPressed = false;
            isLeftKeyPressed = false;
        }
    }
    else {
        printf("ship x:%f, y:%f \n", SHIPINITX + shipdx, SHIPINITY + shipdy);
    }

    if (hitReadValue > 0) {
        if (numTime % (ALIENSP/3) == 0 ) {
            hitReadValue -= READVALUEUNIT;
        }
        shipCol[0] = 1;
        shipCol[1] = 1*(1-(hitReadValue+0.0)/(READVALUEUNIT*DEATHREAD));
        shipCol[2] = 1*(1-(hitReadValue+0.0)/(READVALUEUNIT*DEATHREAD));
    }

    //ship body
    glPushMatrix();
    glTranslatef(shipdx, 0, 0);
    if(hitReadValue <= 0) {
        glColor3f(0, 1-(0 + numHits)/LIFETOTAL, 1-(0 + numHits)/LIFETOTAL);
    }
    else {
        glColor3f(shipCol[0], shipCol[1], shipCol[2]);
    }

    glBegin(GL_POLYGON);
    glVertex2f(SHIPINITX + SHIPWIDTH*4/5, SHIPINITY + 1.5*SHIPHEIGHT);
    glVertex2f(SHIPINITX + SHIPWIDTH*3.5/5, SHIPINITY + 2.5*SHIPHEIGHT);
    glVertex2f(SHIPINITX + SHIPWIDTH*1.5/5, SHIPINITY + 2.5*SHIPHEIGHT);
    glVertex2f(SHIPINITX + SHIPWIDTH/5, SHIPINITY + 1.5*SHIPHEIGHT);
    glVertex2f(SHIPINITX + SHIPWIDTH/5, SHIPINITY - SHIPHEIGHT);
    glVertex2f(SHIPINITX + SHIPWIDTH*4/5, SHIPINITY - SHIPHEIGHT);
    glEnd();

    glColor3f(.2, .4, .6);
    glBegin(GL_POLYGON);
    glVertex2f(SHIPINITX + SHIPWIDTH/3, SHIPINITY + SHIPWIDTH/3 );
    glVertex2f(SHIPINITX + SHIPWIDTH/3, SHIPINITY + SHIPWIDTH/6 );
    glVertex2f(SHIPINITX + SHIPWIDTH*2/5, SHIPINITY);
    glVertex2f(SHIPINITX + SHIPWIDTH*3/5, SHIPINITY);
    glVertex2f(SHIPINITX + SHIPWIDTH*2/3, SHIPINITY + SHIPWIDTH/6 );
    glVertex2f(SHIPINITX + SHIPWIDTH*2/3, SHIPINITY + SHIPWIDTH/3 );
    glVertex2f(SHIPINITX + SHIPWIDTH*3/5, SHIPINITY + SHIPWIDTH/2);
    glVertex2f(SHIPINITX + SHIPWIDTH*2/5, SHIPINITY + SHIPWIDTH/2);
    glEnd();

    glPopMatrix();

    glColor3f(.3, .5, .7);
    //check if alien reaches bottom or no life left
    if(lose()) {
      failFlag = true;
    }

    if (failFlag == true) {

        if (numTime % 10 == 0 && failTimeCount > 0) {
            failTimeCount --;
        }

        glPushMatrix();
        glBegin(GL_POLYGON);
        glColor3f(failTimeCount/FAILREAD, failTimeCount/FAILREAD, failTimeCount/FAILREAD);
        glVertex2f(-VIEW, -VIEW);
        glVertex2f(VIEW, -VIEW);
        glVertex2f(VIEW, VIEW);
        glVertex2f(-VIEW, VIEW);
        glEnd();
        glPopMatrix();
    }

    //move alien
    if (freezeForDebug == true) {
        printf("\n DEBUG INFOR \n");
        printf("numTime: %d\n", numTime);
        // printf("angle: %d\n", angle);
        // printf("numberOfgrids: %d\n", numberOfgrids);
        // printf("isLeftKeyPressed: %f, \nisRightKeyPressed: %f, \nisDownKeyPressed: %f, \nshipShouldMove: %f\n",
        //        isLeftKeyPressed, isRightKeyPressed, isDownKeyPressed, shipShouldMove);
        // printf("shipXtrans: %f, shipYtrans: %f\n", shipXtrans, shipYtrans);
        // printf("alienXtrans: %f, alienYtrans: %f, alienGroupV: %f\n \n \n",
        //        alienXtrans, alienYtrans, alienGroupV);
        for (int i = 0; i < ALIENCOL * ALIENROW; i++) {
            printf("aliens[%d], aliens[i].x: %f, aliens[i].y: %f, aliens[i].alive: %d\n",
                   i, aliens[i].x, aliens[i].y, aliens[i].alive);
        }
        if (NextStepInDebug == true) {
            numTime++;
            if (numTime % (ALIENSP*2) < ALIENSP) {
              angle += 2;
            }
            NextStepInDebug = false;
        }
    }
    else {
        numTime++;
        if (numTime % (ALIENSP*2) < ALIENSP) {
          angle += 1;
        } //if
    } //else

    // MOVE ALIEN

    if(alienDY > 0) {
        if (numTime % ALIENSP == 0){
            if (alienDX < STEP || alienDX >= ALIENCOL * STEP - STEP) {
                alienGroupV *= -1;
                alienDY -= SHIPHEIGHT;
            }
            alienShoot(ALIENSP*2);
            alienDX += alienGroupV;
        }
    }
    else if(alienDY > -3 * ALIENROWINT){
        if (numTime % (ALIENSP/2) == 0){
            if (alienDX < STEP || alienDX >= ALIENCOL * STEP - STEP) {
                alienGroupV *= -1;
                alienDY -= SHIPHEIGHT;
            }
            alienShoot(ALIENSP);
            alienDX += alienGroupV;
        }
    }
    else {
        if (numTime % (ALIENSP/5) == 0){
            if (alienDX < STEP || alienDX >= ALIENCOL * STEP - STEP) {
                alienGroupV *= -1;
                alienDY -= SHIPHEIGHT;
            }
            alienShoot(ALIENSP*2/5);
            alienDX += alienGroupV;
        }
    }

    //ship bullet

    if (numTime % (ALIENSP/10) == 0){
        if(shipBull != NULL){
            glColor3f(0, 1, 0);
            for (Node *temp = shipBull->h; temp != NULL; temp = temp->next) {
                temp->data->dy += SHIPHEIGHT;
                if (temp->data->shootAngle == LEFTSHOOT) {
                    temp->data->dx -= SHIPHEIGHT/3;
                }
                else if (temp->data->shootAngle == RIGHTSHOOT) {
                    temp->data->dx += SHIPHEIGHT/3;
                }
            }
        }
    }
    if(shipBull != NULL) {
        Node *temp1, *temp2;
        for (temp1 = shipBull->h; temp1 != NULL; temp1 = temp2) {
            temp2 = temp1->next;
            if ((temp1->data->yPos + temp1->data->dy >VIEW) ||
                (temp1->data->yPos + temp1->data->dy <-VIEW) ||
                (temp1->data->xPos + temp1->data->dx >VIEW) ||
                (temp1->data->xPos + temp1->data->dx < -VIEW)) {
                temp1->data->active = false;
                removeNode(temp1, &shipBull);
            }
        }
    }
    if (numTime % 360 == 0) {
        numTime = 0;
    }

    if (angle % 360 == 0) {
        numTime = 0;
    }
}

void init() {
    //set to black
    glClearColor(0.0, 0.0, 0.0, 1.0);
    shipBull = NULL;
    sentBullet = NULL;
    for (int i = 0; i < ALIENCOL*ALIENROW; i++) {
        aliens[i].x = -VIEW + SHIPWIDTH*(i%ALIENCOL);
        aliens[i].y = VIEW - ALIENROWINT*(i/ALIENCOL) - SHIPHEIGHT*(ALIENROW + 1);
        aliens[i].alive = true;
        aliens[i].index = i;
        alienDeathAnimationValue[i] = -1;
    }
}

void framebuffer_resize(GLFWwindow* wind, GLint width, GLint height){
    glViewport(0, 0, width, height);
}

void keyboard(GLFWwindow *wind, int key, int scancode, int action, int mods) {
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
            if(sentBullet != NULL){
                clearNodes(&sentBullet);
            }
            if(shipBull != NULL){
                clearNodes(&shipBull);
            }
            glfwSetWindowShouldClose(wind, GL_TRUE);
            break;
        case GLFW_KEY_LEFT:
            isLeftKeyPressed = SHIPWIDTH;
            isRightKeyPressed = false;
            break;
        case GLFW_KEY_RIGHT:
            isRightKeyPressed = SHIPWIDTH;
            isLeftKeyPressed = false;
            break;
        case GLFW_KEY_DOWN:
            isDownKeyPressed = true;
            if (freezeForDebug == true) {
                NextStepInDebug = true;
            }
            freezeForDebug = true;
            break;
        case GLFW_KEY_UP:
            break;
        case GLFW_KEY_SPACE:
            if(numTime - previousFireTime > FIRECOOLINGTIME || previousFireTime - numTime > FIRECOOLINGTIME){
                isSpacePressed = true;
                previousFireTime = numTime;
            }
            break;
        case 'r' :
        case 'R':
            freezeForDebug = false;
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
    window = glfwCreateWindow(WIDTH, HEIGHT, "Hw2 v2!", NULL, NULL);
    if (!window) {
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
