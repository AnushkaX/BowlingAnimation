#include <stdlib.h>
#include <GL/glut.h>
#include <cmath>
#include <stdio.h>
#include <iostream>
#define PI 3.1415926535898

GLfloat windowW = 20;
GLfloat windowH = 20;
GLfloat rota = 0.0f;
GLfloat rotb = 0.0f;
GLfloat rotc = 0.0f;


float posX = 0.01, posY = -0.1, posZ = 0,

bx1 = 0.01, by1 = 0.1,
bx2 = 0.06, by2 = 0.1,
bx3 = 0.10, by3 = 0.1;

GLfloat rotation = 90.0;
double x, y, angle;

GLint circle_points = 50;

void init() {
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glEnable(GL_DEPTH_TEST);
    glLineWidth(1.0);
}

void ball() {
    glPushMatrix();
    glColor3f(1.0f, 0.0f, 1.0f);
    glutSolidSphere(0.25, 100, 100);
    glPopMatrix();
}

void bottle() {
    glColor3f(0.0, 0.0, 1.0);
    glPointSize(9.0);
    glBegin(GL_POINTS);
    glVertex3f(bx1, by1, 0.0);
    glEnd();

    glBegin(GL_POINTS);
    glVertex3f(bx2, by2, 0.0);

    glEnd();
    glBegin(GL_POINTS);
    glVertex3f(bx3, by3, 0.0);

    glEnd();

    glFlush();

}

void circ() {
    glColor3f(1.0, 0.0, 1.0);
    glBegin(GL_TRIANGLE_FAN);
    for (int i = 0; i <= 300; i++) {
        angle = 2 * PI * i / 300;
        x = cos(angle) / 20;
        y = sin(angle) / 20;
        glVertex2d(x, y);
    }
    glEnd();
}

void ball(float yloc)
{
    int b;
    static float balls[100][3];
    static short first = true;

    if (first) {            // Initialize 100 spheres with random x,y coordinates.
        first = false;
        for (b = 0; b < 100; b++) {
            balls[b][0] = float(rand() % 50 - 25) / 10.0;
            balls[b][1] = float(rand() % 50 - 25) / 10.0;
            balls[b][2] = 0.0;
        }
    }

    for (b = 0; b < 100; b++) {    // Translate balls towards and away from front plane.
        glPushMatrix();
        glTranslatef(balls[b][0], balls[b][1], yloc);
        glutSolidSphere(0.2, 20, 50);
        glPopMatrix();
    }
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    ball();

    glutSwapBuffers();

    glFlush();
}

void display1() {
    glClearColor(1.0, 1.0, 1.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glPushMatrix();
    glColor3f(1.0, 0.0, 0.0);

    
    glLoadIdentity();
    gluOrtho2D(-1.0, 1.0, -1.0, 1.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glPushMatrix();
    bottle();
    glPopMatrix();

    //drawSphere(5);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(posX, posY, posZ);
    circ();
    glPopMatrix();
    glutSwapBuffers();
}

float move_unit = 0.02f;
void keyboardown(int key, int x, int y) {
    switch (key) {
    case GLUT_KEY_RIGHT:
        posX += move_unit;
        break;
    case GLUT_KEY_LEFT:
        posX -= move_unit;
        break;
    case GLUT_KEY_UP:
        posY += move_unit;
        break;
    case GLUT_KEY_DOWN:
        posY -= move_unit;
        break;
    default:
        break;
    }
    if (posX == bx1 || posX == bx2) {

        bx1 -= 0.02, by1 += 0.06;
        bx2 = 0.02,
            by2 += 0.08;
        bx3 = 0.04,
            by3 += 0.04;

    }

    glutPostRedisplay();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitWindowSize(500, 500);
    glutCreateWindow("Assignment 3");
    glutPositionWindow(100, 100);
    glutDisplayFunc(display);
    //glutReshapeFunc(resize);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    //glutTimerFunc(20, Timer, 1);
    init();
    glutMainLoop();

    return 1;
}

