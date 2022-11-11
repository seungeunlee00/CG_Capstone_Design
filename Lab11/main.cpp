#include <GL/glut.h>
#include <stdio.h>
#include <windows.h>
#include <math.h>
#include "bmpfuncs.h"
#define PI 3.14159265358979

volatile int radius = 15;
volatile double theta = 45;
volatile double phi = 45;
volatile double upVector = cos(phi * PI / 180);
volatile double eyex, eyey, eyez; // 시야 설정

volatile unsigned char lightMode = 0; // 광원 설정 모드
int axisMode = 0; // 좌표축 설정 모드

/* texture mapping set variable */
GLuint texName[6]; // make cube
GLuint texName2[3]; // make cylinder
GLuint* texName3 = new GLuint(); 

/* quadric object 객체 생성 */
GLUquadricObj* qobj = gluNewQuadric(); 

void init();
void draw();
void keyboard(unsigned char, int, int);
void specialkeyboard(int, int, int);
void resize(int, int);
void main_menu_function(int);
void light_default();
void printInstruction();

/*Texture mapping 함수*/
void cubeTextureMapping();
void cylinderTextureMapping();
void sphereTextureMapping();
void draw_textureCube();
void draw_Cylinder();
void draw_Sphere();

int main(int argc, char** argv) {
    /* Window 초기화 */
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutInitWindowPosition(400, 100);
    glutCreateWindow("12201933 이승은 Lab11");
    init(); // -> 사용자 초기화 함수

    /* Callback 함수 정의 */
    glutReshapeFunc(resize);
    glutDisplayFunc(draw);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialkeyboard);

    /* menu 생성 및 추가 */
    glutCreateMenu(main_menu_function);
    glutAddMenuEntry("Quit", 999);
    glutAttachMenu(GLUT_RIGHT_BUTTON);

    /* Looping 시작 */
    glutMainLoop();
    return 0;
}

void init() {
    /* 화면, Object의 기본색으로 Black 설정 */
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glColor3f(1.0f, 1.0f, 1.0f);

    glutReshapeFunc(resize); // resize 콜백 함수
    light_default(); // light setting
    printInstruction(); // 조작법 출력 

    /* Texture Mapping Set */
    gluQuadricTexture(qobj, GL_TRUE);
    cubeTextureMapping();
    cylinderTextureMapping();
    sphereTextureMapping();

    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);    
    // polygon의 원래 색상과 texture 색상을 곱하여 덮음
    glEnable(GL_TEXTURE_2D);
}

void light_default() {
    GLfloat ambientLight[] = { 0.2f, 0.2f, 0.2f, 1.0f };
    GLfloat diffuseLight[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat specularLight[] = { 1.0f, 1.0f, 1.0f, 1.0f };

    glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    GLfloat diffuse_Sun[] = { 1.0f, 0.0f, 0.0f, 1.0f };
    GLfloat ambient_Sun[] = { 0.2f, 0.2f, 0.2f, 1.0f };
    GLfloat specular_Sun[] = { 1.0f, 1.0f, 1.0f, 1.0f };

    glMaterialfv(GL_FRONT, GL_AMBIENT, ambient_Sun);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_Sun);
    glMaterialfv(GL_FRONT, GL_SPECULAR, specular_Sun);
    glMateriali(GL_FRONT, GL_SHININESS, 60);

    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
}

void draw() {
    /* 화면을 깨끗하게 지우기 */
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // 물체의 앞뒤 관계를 위한 DEPTH 추가
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glClearDepth(1.0);
    glEnable(GL_DEPTH_TEST); // 뒤에 있는 object가 앞으로 보이는 현상 제거

    eyex = radius * sin(theta * PI / 180) * cos(phi * PI / 180);
    eyey = radius * sin(phi * PI / 180);
    eyez = radius * cos(theta * PI / 180) * cos(phi * PI / 180);
    upVector = cos(phi * PI / 180);

    if (lightMode == 0) { // 조명 camera에 위치
        GLfloat light_position[] = { 0.0, 0.0, 0.0, 1.0 };
        glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    }
    gluLookAt(eyex, eyey, eyez, 0, 0, 0, 0, upVector, 0);

    /* Texture 큐브 */
    glPushMatrix();
    glTranslatef(2, 0, 2);
    glColor3f(1, 1, 1);
    GLfloat light_position[] = { 0.0, 0.0, 0.0, 1.0 };
    if (lightMode == 1) { // 조명 cube에 위치
        glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    }
    draw_textureCube();
    glPopMatrix();

    /* Cylinder */
    glPushMatrix();
    glTranslatef(5, 0, -5);
    if (lightMode == 2) { // 조명 cylinder에 위치
        glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    }
    draw_Cylinder();
    glPopMatrix();

    /* Sphere */
    glPushMatrix();
    glTranslatef(-5, 0, 0);
    if (lightMode == 3) { // 조명 sphere에 위치
        glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    }
    draw_Sphere();
    glPopMatrix();

    glFlush();
    glutSwapBuffers();
}

void keyboard(unsigned char key, int x, int y) {
    printf("You pressed %c\n", key);
    if (key == '0') {
        lightMode = 0;
        glutPostRedisplay();
    } else if (key == '1') {
        lightMode = 1;
        glutPostRedisplay();
    } else if (key == '2') {
        lightMode = 2;
        glutPostRedisplay();
    } else if (key == '3') {
        lightMode = 3;
        glutPostRedisplay();
    } else if (key == 'o') {
        radius++;
        glutPostRedisplay();
    } else if (key == 'i') {
        radius--;
        glutPostRedisplay();
    }
}

void specialkeyboard(int key, int x, int y) {
    if (key == GLUT_KEY_DOWN) {
        if (phi == 0) phi = 360;
        phi = phi - 1;
        glutPostRedisplay();
    } else if (key == GLUT_KEY_UP) {
        if (phi == 360) phi = 0;
        phi = phi + 1;
        glutPostRedisplay();
    } else if (key == GLUT_KEY_LEFT) {
        if (theta == 0) theta = 360;
        theta = theta - 1;
        glutPostRedisplay();
    } else if (key == GLUT_KEY_RIGHT) {
        if (theta == 360) theta = 0;
        theta = theta + 1;
        glutPostRedisplay();
    }
}

/* 윈도우 생성 및 크기 변화 호출 */
void resize(int width, int height) {
    glViewport(0, 0, width, height); // viewport T.F
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45, (float)width / (float)height, 1, 500); // projection T.F
    glMatrixMode(GL_MODELVIEW);
}

void printInstruction() {
    /* 조작법 console 출력 */
    printf("\n-----------keyboard navigation-----------\n\n");
    printf("0 : 광원 Camera\n");
    printf("1 : 광원 Cube\n");
    printf("2 : 광원 Cylinder\n");
    printf("3 : 광원 Sphere\n");
    printf("'o' : Zoom out\n");
    printf("'i' : Zoom in\n");
    printf("방향키 : camera 위치\n\n");
}

void main_menu_function(int option) {
    printf("Main menu %d has been selected\n", option);
    if (option == 999) exit(0);
}

void cubeTextureMapping() {
    glGenTextures(6, texName);
    int imgWidth, imgHeight, channels;
    for (int i = 0; i < 6; i++) {
        glBindTexture(GL_TEXTURE_2D, texName[i]);
        char buf[100];
        sprintf(buf, "img/TexImage%d.bmp", i);
        buf[strlen(buf)] = 0;
        unsigned char* img = readImageData(buf, &imgWidth, &imgHeight, &channels);
        glTexImage2D(GL_TEXTURE_2D, 0, /*INPUT CHANNEL*/3, imgWidth, imgHeight, 0, /*TEXEL CHANNEL*/GL_RGB, GL_UNSIGNED_BYTE, img);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    }
}

void cylinderTextureMapping() {
    glGenTextures(3, texName2);

    glBindTexture(GL_TEXTURE_2D, texName2[0]);
    int width, height, channels;
    unsigned char* img = readImageData("img/CIDER_T.bmp", &width, &height, &channels);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, img);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    glBindTexture(GL_TEXTURE_2D, texName2[1]);
    img = readImageData("img/coke.bmp", &width, &height, &channels);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, img);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    glBindTexture(GL_TEXTURE_2D, texName2[2]);
    img = readImageData("img/CIDER_B.bmp", &width, &height, &channels);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, img);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
}

void sphereTextureMapping() {
    glGenTextures(1, texName3); 
    glBindTexture(GL_TEXTURE_2D, *texName3);
    int width, height, channels;
    unsigned char* img = readImageData("img/EARTH.bmp", &width, &height, &channels);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, img);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
}

void draw_textureCube() {
    glColor3f(1.0, 1.0, 1.0);
    // white로 color를 set해주어야 texture 색상이 제대로 적용됨
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    // polygon의 원래 색상과 texture 색상을 곱하여 덮음. texture가 입혀진 표면에 광원 효과 설정 가능 

    glBindTexture(GL_TEXTURE_2D, texName[0]);
    glBegin(GL_QUADS);
    glNormal3f(-1.0, 0, 0);   // -x axis
    glTexCoord2f(0, 0); glVertex3f(-1.0, 1.0, -1.0);
    glTexCoord2f(1, 0); glVertex3f(-1.0, -1.0, -1.0);
    glTexCoord2f(1, 1); glVertex3f(-1.0, -1.0, 1.0);
    glTexCoord2f(0, 1); glVertex3f(-1.0, 1.0, 1.0);
    glEnd();

    glBindTexture(GL_TEXTURE_2D, texName[1]);
    glBegin(GL_QUADS);
    glNormal3f(1.0, 0, 0);   // x axis
    glTexCoord2f(0, 0); glVertex3f(1.0, 1.0, 1.0);
    glTexCoord2f(1, 0); glVertex3f(1.0, -1.0, 1.0);
    glTexCoord2f(1, 1); glVertex3f(1.0, -1.0, -1.0);
    glTexCoord2f(0, 1); glVertex3f(1.0, 1.0, -1.0);
    glEnd();

    glBindTexture(GL_TEXTURE_2D, texName[2]);
    glBegin(GL_QUADS);
    glNormal3f(0, -1.0, 0);   // -y axis
    glTexCoord2f(0, 0); glVertex3f(-1.0, -1.0, -1.0);
    glTexCoord2f(1, 0); glVertex3f(1.0, -1.0, -1.0);
    glTexCoord2f(1, 1); glVertex3f(1.0, -1.0, 1.0);
    glTexCoord2f(0, 1); glVertex3f(-1.0, -1.0, 1.0);
    glEnd();

    glBindTexture(GL_TEXTURE_2D, texName[3]);
    glBegin(GL_QUADS);
    glNormal3f(0, 1.0, 0);   // y axis
    glTexCoord2f(0, 0); glVertex3f(-1.0, 1.0, 1.0);
    glTexCoord2f(1, 0); glVertex3f(1.0, 1.0, 1.0);
    glTexCoord2f(1, 1); glVertex3f(1.0, 1.0, -1.0);
    glTexCoord2f(0, 1); glVertex3f(-1.0, 1.0, -1.0);
    glEnd();

    glBindTexture(GL_TEXTURE_2D, texName[4]);
    glBegin(GL_QUADS);
    glNormal3f(0, 0, 1.0);   // z axis
    glTexCoord2f(0, 0); glVertex3f(1.0, 1.0, 1.0);
    glTexCoord2f(1, 0); glVertex3f(-1.0, 1.0, 1.0);
    glTexCoord2f(1, 1); glVertex3f(-1.0, -1.0, 1.0);
    glTexCoord2f(0, 1); glVertex3f(1.0, -1.0, 1.0);
    glEnd();

    glBindTexture(GL_TEXTURE_2D, texName[5]);
    glBegin(GL_QUADS);
    glNormal3f(0, 0, -1.0);   // -z axis
    glTexCoord2f(0, 0); glVertex3f(1.0, 1.0, -1.0);
    glTexCoord2f(1, 0); glVertex3f(-1.0, 1.0, -1.0);
    glTexCoord2f(1, 1); glVertex3f(-1.0, -1.0, -1.0);
    glTexCoord2f(0, 1); glVertex3f(1.0, -1.0, -1.0);
    glEnd();
}

void draw_Cylinder() {
    glColor3f(1.0, 1.0, 1.0);

    /* 윗면 - Disk */
    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D, texName2[0]);
    glTranslatef(-2, 1, 0);
    glRotatef(-90, 1, 0, 0);
    gluDisk(qobj, 0, 1, 12, 1);
    glPopMatrix();

    /* 옆면 - Cylinder */
    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D, texName2[1]);
    glTranslatef(-2, -2, 0);
    glRotatef(-90, 1, 0, 0);
    gluCylinder(qobj, 1, 1, 3, 12, 1);
    glPopMatrix();

    /* 아랫면 - Disk */
    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D, texName2[2]);
    glTranslatef(-2, -2, 0);
    glRotatef(90, 1, 0, 0);
    gluDisk(qobj, 0, 1, 12, 1);
    glPopMatrix();
}

void draw_Sphere() {
    glColor3f(1.0, 1.0, 1.0);
    glBindTexture(GL_TEXTURE_2D, *texName3);
    glRotatef(-90, 1, 0, 0);
    gluSphere(qobj, 1, 24, 24);
    glRotatef(90, 1, 0, 0);
}
