
#include "list.h"
Bullet *makeBullet(GLdouble xPos, GLdouble yPos, GLdouble dx, GLdouble dy){
    Bullet *newBullet;
    if ( (newBullet = (Bullet *) malloc( sizeof(Bullet) ) )!= 0) {
        newBullet->xPos = xPos;
        newBullet->yPos = yPos;
        newBullet->dx = dx;
        newBullet->dy =  dy;
        newBullet->active = true;
        //newBullet->shootAngle = ang;
    }
    else {
        exit(0);
    }
    return newBullet;
}

Node *makeNode (Bullet* b) {
    Node *newNode;
    if ( (newNode = (Node *) malloc(sizeof(Node)) )!= 0) {
        newNode->data =  b;
        newNode->next = NULL;
        newNode->h = NULL;
        newNode->t = NULL;
    }
    else {
        exit(0);
    }
    return newNode;
}

void addNode(Node *n, Node **llist) {
    if (*llist == NULL) {
        (*llist) = n;
        (*llist)->h = n;
        (*llist)->t = n;
    }
    else {
        (*llist)->t->next = n;
        (*llist)->t = n;
    }
}

void removeNode(Node *n, Node **llist) {
    Node *temp1, *temp2;

    if ((n == (*llist)->h) && (n ==(*llist)->t)) {
        (*llist)->h = (*llist)->t = NULL;
    }
    else if(n == (*llist)->h) {
        (*llist)->h = n->next;
    }
    else {
        for (temp2 = (*llist)->h, temp1=NULL; temp2 != n; temp2 = temp2->next);
        if (n == (*llist)->t)
            (*llist)->t = temp1;
        temp1->next = n->next;
    }


}


void clearNodes(Node** llist) {
    Node *temp1, *temp2;
    for (temp1 = (*llist)->h; temp1 != NULL; temp1 = temp2) {
        temp2 = temp1->next;
        free(temp1->data);
        free(temp1);
    }
    (*llist)->h = (*llist)->t = NULL;
}