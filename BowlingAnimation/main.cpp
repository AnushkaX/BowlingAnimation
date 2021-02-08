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

GLfloat animateRotaion = 0.0f;

//Move the exvironment
GLfloat moveX = 0.0f;
GLfloat moveY = 0.0f;
GLfloat moveZ = 0.0f;

//Rotate the environment
GLfloat rotateX = 0.0f;
GLfloat rotateY = 0.0f;
GLfloat rotateZ = 0.0f;

//Move cameras
GLfloat camX = 0.0f;
GLfloat camY = 0.0f;
GLfloat camZ = 0.0f;


GLfloat rotation = 90.0;
double x, y, angle;

GLint circle_points = 50;

void init() {
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glEnable(GL_DEPTH_TEST);
    glLineWidth(1.0);
    glPolygonMode(GL_FRONT, GL_FILL);
    glPolygonMode(GL_BACK, GL_FILL);
}

void Timer(int x) {
    animateRotaion += animateRotaion >= 360.0 ? -animateRotaion : 5;
    glutPostRedisplay();

    glutTimerFunc(60, Timer, 1);
}

void DrawGrid() {
    GLfloat ext = 20.0f;
    GLfloat step = 1.0f;
    GLfloat yGrid = -0.4f;
    GLint line;

    glBegin(GL_LINES);
    glColor3f(1.0, 1.0, 1.0);

    for (line = -ext; line <= ext; line += step) {
        glVertex3f(line, yGrid, ext);
        glVertex3f(line, yGrid, -ext);

        glVertex3f(ext, yGrid, line);
        glVertex3f(-ext, yGrid, line);
    }
    glEnd();

}

void drawAxes() {
    glBegin(GL_LINES);

    glColor3f(1.0, 0.0, 0.0);
    glVertex3f(-20, 0, 0);
    glVertex3f(20, 0, 0);

    glColor3f(0.0, 1.0, 0.0);
    glVertex3f(0, -20, 0);
    glVertex3f(0, 20, 0);

    glColor3f(0.0, 0.0, 1.0);
    glVertex3f(0, 0, -20);
    glVertex3f(0, 0, 20);

    glEnd();

}

void ball() {
    glPushMatrix();
    glColor3f(0.0f, 1.0f, 1.0f);
    glTranslatef(0.0, 0.5, 0.0);
    glutSolidSphere(0.5, 100, 100);
    glPopMatrix();
}

void pins() {
    glPushMatrix();
    glColor3f(1.0f, 0.0f, 1.0f);
    GLdouble plane1[] = { 0, 1.1, 0, 0 };
    glClipPlane(GL_CLIP_PLANE0, plane1);
    glEnable(GL_CLIP_PLANE0);
    glTranslatef(0, 0, 0.2);
    glTranslatef(0.0, 0.75, -9.0);
    glRotatef(0, 0, 1, 0);
    glScalef(1, 1.75, 1);
    glRotatef(-90.0f, 1.0, 0.0, 0.0);
    glutSolidSphere(0.5, 20, 20);
    glDisable(GL_CLIP_PLANE0);
    glPopMatrix();

    glPushMatrix();
    glColor3f(1.0f, 0.0f, 1.0f);
    GLdouble plane2[] = { 0, 1.1, 0, 0 };
    glClipPlane(GL_CLIP_PLANE0, plane2);
    glEnable(GL_CLIP_PLANE0);
    glTranslatef(0, 0, 0.2);
    glTranslatef(1.0, 0.75, -9.0);
    glRotatef(0, 0, 1, 0);
    glScalef(1, 1.75, 1);
    glRotatef(-90.0f, 1.0, 0.0, 0.0);
    glutSolidSphere(0.5, 20, 20);
    glDisable(GL_CLIP_PLANE0);
    glPopMatrix();

    glPushMatrix();
    glColor3f(1.0f, 0.0f, 1.0f);
    GLdouble plane3[] = { 0, 1.1, 0, 0 };
    glClipPlane(GL_CLIP_PLANE0, plane3);
    glEnable(GL_CLIP_PLANE0);
    glTranslatef(0, 0, 0.2);
    glTranslatef(-1.0, 0.75, -9.0);
    glRotatef(0, 0, 1, 0);
    glScalef(1, 1.75, 1);
    glRotatef(-90.0f, 1.0, 0.0, 0.0);
    glutSolidSphere(0.5, 20, 20);
    glDisable(GL_CLIP_PLANE0);
    glPopMatrix();
}

void floor() {
    //glLineWidth(12.0);

    glBegin(GL_QUADS);

    glColor4f(1.0, 1.0, 0.0, 0.0);

    glVertex3f(-2, 0, -10);
    glVertex3f(-2, 0, 1);
    glVertex3f(2, 0, 1);
    glVertex3f(2, 0, -10);

    glEnd();
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


void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //glRotatef(180.0, 0.0, 1.0, 0.0);
    //glRotatef(-30.0f, 0.0, 1.0, 0.0);
    
    glPushMatrix();

    //camera movements
    gluLookAt(0.0 + camX, 2.0f + camY, 5.0f + camZ, 0, 0, 0, 0, 1.0, 0);

    //move the frame
    glTranslatef(moveX, moveY, moveZ);

    glRotatef(rotateX, 1.0f, 0.0f, 0.0f);
    glRotatef(rotateY, 0.0f, 1.0f, 0.0f);
    glRotatef(rotateZ, 0.0f, 0.0f, 1.0f);

    floor();

    //drawAxes();
    
    DrawGrid();
    
    ball();
    
    pins();

    glPopMatrix();

    glutSwapBuffers();

    glFlush();
}

void keyboardSpecial(int key, int x, int y) {
    if (key == GLUT_KEY_UP) {
        moveZ += 1;
    }
    if (key == GLUT_KEY_DOWN) {
        moveZ -= 1;
    }

    if (key == GLUT_KEY_LEFT) {
        rotateY += 5;
    }
    if (key == GLUT_KEY_RIGHT) {
        rotateY -= 5;
    }
 
    glutPostRedisplay();
}

void keyBoadrd(unsigned char key, int x, int y) {
    if (key == 'w')
        camY += 0.5f;
    if (key == 's')
        camY -= 0.5;
    if (key == 'a')
        camX += 0.5f;
    if (key == 'd')
        camX -= 0.5;

    glutPostRedisplay();
}

void changeSize(GLsizei w, GLsizei h) {
    glViewport(0, 0, w, h);
    GLfloat aspectRatio = h == 0 ? w / 1 : (GLfloat)w / (GLfloat)h;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluPerspective(120.0, aspectRatio, 1.0, 100.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitWindowSize(500, 500);
    glutCreateWindow("Assignment 3");
    glutPositionWindow(150, 150);

    glutDisplayFunc(display);
    
    glutReshapeFunc(changeSize);

    glutSpecialFunc(keyboardSpecial);
    glutKeyboardFunc(keyBoadrd);
    
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    
    //glutTimerFunc(60.0, Timer, 1);
    
    init();
    glutMainLoop();

    return 1;
}

