#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>
#include <math.h>
#include <time.h> 
#include <GL/glut.h>

#define PI 3.1415926535898
#define PIN 0
#define FLOOR 1
#define FLOOR1 2
#define BALL 3
#define SIDES 4
#define FLOOR_LINE 5
#define TEXTURE_COUNT 6

#define SPACEBAR 32

GLfloat windowW = 20;
GLfloat windowH = 20;
GLfloat rota = 0.0f;
GLfloat rotb = 0.0f;
GLfloat rotc = 0.0f;

GLfloat flag = 0.0;

GLfloat animateRotaion = 0.0f;

GLfloat ballx = 0.0f;
GLfloat ballz = 10.0f;

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

using namespace std;

GLUquadric* sphere;

struct BitMapFile
{
    int sizeX;
    int sizeY;
    unsigned char* data;
};

static unsigned int texture[TEXTURE_COUNT];

GLUquadricObj* qobj;

BitMapFile* getbmp(string filename)
{
    int offset, headerSize;

    BitMapFile* bmpRGB = new BitMapFile;
    BitMapFile* bmpRGBA = new BitMapFile;

    ifstream infile(filename.c_str(), ios::binary);

    infile.seekg(10);
    infile.read((char*)&offset, 4);

    infile.read((char*)&headerSize, 4);

    infile.seekg(18);
    infile.read((char*)&bmpRGB->sizeX, 4);
    infile.read((char*)&bmpRGB->sizeY, 4);

    int padding = (3 * bmpRGB->sizeX) % 4 ? 4 - (3 * bmpRGB->sizeX) % 4 : 0;
    int sizeScanline = 3 * bmpRGB->sizeX + padding;
    int sizeStorage = sizeScanline * bmpRGB->sizeY;
    bmpRGB->data = new unsigned char[sizeStorage];

    infile.seekg(offset);
    infile.read((char*)bmpRGB->data, sizeStorage);

    int startScanline, endScanlineImageData, temp;
    for (int y = 0; y < bmpRGB->sizeY; y++)
    {
        startScanline = y * sizeScanline;
        endScanlineImageData = startScanline + 3 * bmpRGB->sizeX;
        for (int x = startScanline; x < endScanlineImageData; x += 3)
        {
            temp = bmpRGB->data[x];
            bmpRGB->data[x] = bmpRGB->data[x + 2];
            bmpRGB->data[x + 2] = temp;
        }
    }

    bmpRGBA->sizeX = bmpRGB->sizeX;
    bmpRGBA->sizeY = bmpRGB->sizeY;
    bmpRGBA->data = new unsigned char[4 * bmpRGB->sizeX * bmpRGB->sizeY];

    for (int j = 0; j < 4 * bmpRGB->sizeY * bmpRGB->sizeX; j += 4)
    {
        bmpRGBA->data[j] = bmpRGB->data[(j / 4) * 3];
        bmpRGBA->data[j + 1] = bmpRGB->data[(j / 4) * 3 + 1];
        bmpRGBA->data[j + 2] = bmpRGB->data[(j / 4) * 3 + 2];
        bmpRGBA->data[j + 3] = 0xFF;
    }
    return bmpRGBA;
}

void loadExternalTextures()
{
    BitMapFile* image[TEXTURE_COUNT];
    image[PIN] = getbmp("Textures/pin.bmp");
    image[FLOOR] = getbmp("Textures/floor.bmp");
    image[FLOOR1] = getbmp("Textures/floor2.bmp");
    image[BALL] = getbmp("Textures/ball.bmp");
    image[SIDES] = getbmp("Textures/sides.bmp");
    image[FLOOR_LINE] = getbmp("Textures/floor_line.bmp");

    for (int i = 0; i < TEXTURE_COUNT; i++) {
        glBindTexture(GL_TEXTURE_2D, texture[i]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image[i]->sizeX, image[i]->sizeY, 0, GL_RGBA, GL_UNSIGNED_BYTE, image[i]->data);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    }
}

void init() {
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glEnable(GL_DEPTH_TEST);

    glGenTextures(TEXTURE_COUNT, texture);
    loadExternalTextures();
    
    glLineWidth(1.0);
    glPolygonMode(GL_FRONT, GL_FILL);
    glPolygonMode(GL_BACK, GL_FILL);

    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
}

void Timer(int x) {
    //animateRotaion += animateRotaion >= 360.0 ? -animateRotaion : 5;
    if (flag == 1.0) {
        if (ballz >= -8.0) {
            ballz = ballz - 1;
        }
    }
    
    
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
    glTranslatef(ballx, 1, ballz);
    glRotatef(-90.0f, 1.0, 0.0, 0.0);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture[BALL]);

    GLUquadric* qobj = gluNewQuadric();
    gluQuadricTexture(qobj, GL_TRUE);
    gluSphere(qobj, 1, 20, 20);
    gluDeleteQuadric(qobj);

    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
}

void drawPin(GLfloat x, GLfloat y, GLfloat z) {

    glPushMatrix();
    glColor3f(1.0f, 1.0f, 1.0f);
    GLdouble plane1[] = { 0, 1.1, 0, 0 };
    glClipPlane(GL_CLIP_PLANE0, plane1);
    glEnable(GL_CLIP_PLANE0);
    glTranslatef(0, 0, 0.2);
    glTranslatef(x, y, z);
    glRotatef(0, 0, 1, 0);
    glScalef(0.9, 1.75, 1);
    glRotatef(-90.0f, 1.0, 0.0, 0.0);
    //glutSolidSphere(0.5, 20, 20);
    
    GLUquadric* qobj = gluNewQuadric();
    gluQuadricTexture(qobj, GL_TRUE);
    gluSphere(qobj, 0.5, 20, 20);
    //gluDeleteQuadric(qobj);
    
    glDisable(GL_CLIP_PLANE0);
    glPopMatrix();

    glPushMatrix();
    glColor3f(1.0f, 0.0f, 1.0f);
    glClipPlane(GL_CLIP_PLANE0, plane1);
    glEnable(GL_CLIP_PLANE0);
    glTranslatef(0, 0, 0.2);
    glTranslatef(x, y + 1.05, z);
    glRotatef(0, 0, 1, 0);
    glScalef(0.8, 1.75, 1);
    glRotatef(-90.0f, 1.0, 0.0, 0.0);
    //glutSolidSphere(0.3, 20, 20);
    
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture[PIN]);

    gluQuadricTexture(qobj, GL_TRUE);
    gluSphere(qobj, 0.3, 20, 20);
    gluDeleteQuadric(qobj);

    glDisable(GL_TEXTURE_2D);
    glDisable(GL_CLIP_PLANE0);
    glPopMatrix();

}

void pins() {

    glPushMatrix();

    drawPin(0, 0.75, -10);

    drawPin(0.8, 0.75, -11);
    drawPin(-0.8, 0.75, -11);
    
    drawPin(0, 0.75, -12);
    drawPin(1.6, 0.75, -12);
    drawPin(-1.6, 0.75, -12);
    
    drawPin(0.8, 0.75, -13);
    drawPin(-0.8, 0.75, -13);
    drawPin(-2.4, 0.75, -13);
    drawPin(2.4, 0.75, -13);
    
    glPopMatrix();
}

void floor() {
    
    //Arena
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture[FLOOR]);

    glBegin(GL_QUADS);
    
    glTexCoord2f(0.0, 0.0); glVertex3f(-7, 0.001, -20);
    glTexCoord2f(1.0, 0.0); glVertex3f(-7, 0.001, 15);
    glTexCoord2f(1.0, 1.0); glVertex3f(7, 0.001, 15);
    glTexCoord2f(0.0, 1.0); glVertex3f(7, 0.001, -20);

    glEnd();

    //Sides
    glBindTexture(GL_TEXTURE_2D, texture[SIDES]);

    glBegin(GL_QUADS);

    glTexCoord2f(0.0, 0.0); glVertex3f(-9, 0.001, -20);
    glTexCoord2f(1.0, 0.0); glVertex3f(-9, 0.001, 15);
    glTexCoord2f(1.0, 1.0); glVertex3f(-7, 0.001, 15);
    glTexCoord2f(0.0, 1.0); glVertex3f(-7, 0.001, -20);

    glEnd();

    glBindTexture(GL_TEXTURE_2D, texture[SIDES]);

    glBegin(GL_QUADS);

    glTexCoord2f(0.0, 0.0); glVertex3f(7, 0.001, -20);
    glTexCoord2f(1.0, 0.0); glVertex3f(7, 0.001, 15);
    glTexCoord2f(1.0, 1.0); glVertex3f(9, 0.001, 15);
    glTexCoord2f(0.0, 1.0); glVertex3f(9, 0.001, -20);

    glEnd();

    //Floor
    glBindTexture(GL_TEXTURE_2D, texture[FLOOR1]);
    glBegin(GL_QUADS);
    
    glTexCoord2f(0.0, 0.0); glVertex3f(-20, 0, -20);
    glTexCoord2f(1.0, 0.0); glVertex3f(-20, 0, 20);
    glTexCoord2f(1.0, 1.0); glVertex3f(20, 0, 20);
    glTexCoord2f(0.0, 1.0); glVertex3f(20, 0, -20);

    glEnd();

    //Floor_Line
    glBindTexture(GL_TEXTURE_2D, texture[FLOOR_LINE]);
    glBegin(GL_QUADS);
    
    glTexCoord2f(0.0, 0.0); glVertex3f(-7, 0.01, 10);
    glTexCoord2f(1.0, 0.0); glVertex3f(-7, 0.01, 10.3);
    glTexCoord2f(1.0, 1.0); glVertex3f(7, 0.01, 10.3);
    glTexCoord2f(0.0, 1.0); glVertex3f(7, 0.01, 10);

    glEnd();

    glDisable(GL_TEXTURE_2D);
}

void wall() {

    glEnable(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, texture[FLOOR]);

    glBegin(GL_QUADS);

    //glColor4f(1.0, 1.0, 0.0, 0.0);

    glTexCoord2f(0.0, 0.0); glVertex3f(-20, 20, -20);
    glTexCoord2f(1.0, 0.0); glVertex3f(-20, 0, -20);
    glTexCoord2f(1.0, 1.0); glVertex3f(20, 0, -20);
    glTexCoord2f(0.0, 1.0); glVertex3f(20, 20, -20);

    glEnd();

    glDisable(GL_TEXTURE_2D);
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

    //wall();

    drawAxes();
    
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
    if (key == 'm')
        flag = 1.0; //fault
    if (key == 'g')
        ballx -= 1;
    if (key == 'h')
        ballx += 1;

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

    glutInitWindowSize(600, 600);

    glutCreateWindow("Bowling Animation");

    glutPositionWindow(150, 150);

    glutDisplayFunc(display);
    
    glutReshapeFunc(changeSize);

    glutSpecialFunc(keyboardSpecial);

    glutKeyboardFunc(keyBoadrd);
    
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    
    
    glutTimerFunc(60.0, Timer, 1);


    init();

    glutMainLoop();

    return 1;
}

