#include<stdio.h>
#include<GL/glut.h>
#include <math.h>
#define PI 3.14159265358979

void init();
double radian(int);
void special_keyboard(int, int, int);
void draw();
void draw_axis();
void resize(int, int);
void main_menu_function(int);

double radius = 10; // 구 좌표 radius
double theta = 45, phi = 45;
double cam[3];
double center[3] = { 0,0,0 };
double up[3] = { 0,1,0 };

int main(int argc, char** argv) {

	cam[0] = (radius * sin(radian(theta)) * cos(radian(phi)));
	cam[1] = (radius * sin(radian(theta)) * sin(radian(phi)));
	cam[2] = (radius * cos(radian(theta)));

	/* Window 초기화 */
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(500, 100);
	glutCreateWindow("12201933 이승은 Lab6");
	init(); // 사용자 초기화 함수

	/* 메인 메뉴 생성 */
	glutCreateMenu(main_menu_function);
	glutAddMenuEntry("init", 1);
	glutAddMenuEntry("Quit", 999);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	/* Callback 함수 정의 */
	glutReshapeFunc(resize);
	glutDisplayFunc(draw);
	glutSpecialFunc(special_keyboard);
	
	/* Looping 시작 */
	glutMainLoop();

	return 0;
}

void init() {
	glClearColor(0.f, 0.f, 0.f, 0.f);
}

double radian(int value) {
	return (value * (PI / 180));
}

void special_keyboard(int key, int x, int y) {
	if (key == GLUT_KEY_LEFT) {
		phi -= 5;
		if (phi <= 0) {
			phi = 360;
		}
	}
	if (key == GLUT_KEY_RIGHT) {
		phi += 5;
		if (phi >= 360) {
			phi = 0;
		}
	}
	if (key == GLUT_KEY_DOWN) {
		theta += 5;
		if (int(theta) % 180 == 0)
			up[1] = -1;
		if (theta > 360) {
			theta = 0;
			up[1] = 1;
		}
	}
	if (key == GLUT_KEY_UP) {
		theta -= 5;
		if (int(theta) % 180 == 0)
			up[1] = 1;
		if (theta < 0) {
			theta = 360;
			up[1] = -1;
		}
	}
	double radianTheta = radian(theta);
	double radianPhi = radian(phi);
	cam[0] = radius * sin(radianTheta) * sin(radianPhi);
	cam[1] = radius * cos(radianTheta);
	cam[2] = radius * sin(radianTheta) * cos(radianPhi);
	printf("theta : %d, phi : %d\n", int(theta), int(phi));
	glutPostRedisplay();
}

void draw() {
	glClear(GL_COLOR_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(cam[0], cam[1], cam[2], center[0], center[1], center[2], up[0], up[1], up[2]);
	// resize() 함수에서 viewport와 projection T.F는 이미 수행
	glColor3f(1, 1, 0);
	glutWireTeapot(3);
	draw_axis();

	glFlush();
}

void draw_axis() {
	glLineWidth(3); // 좌표축의 두께
	glBegin(GL_LINES);

	glColor3f(1, 0, 0); // x축은 red
	glVertex3f(0, 0, 0);
	glVertex3f(4, 0, 0);

	glColor3f(0, 1, 0); // y축은 green
	glVertex3f(0, 0, 0);
	glVertex3f(0, 4, 0);

	glColor3f(0, 0, 1);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 0, 4); // z축은 blue

	glEnd();
	glLineWidth(1); // 두께 다시 환원
}

void resize(int width, int height) {
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION); // viewport T.F
	glLoadIdentity();
	gluPerspective(45, (float)width / (float)height, 1, 500); // projection T.F
	glMatrixMode(GL_MODELVIEW);
}

void main_menu_function(int option) {
	if (option == 1) {
		printf("You selected Init menu\n");
		glClear(GL_COLOR_BUFFER_BIT);
		theta = 45, phi = 45;
		glutPostRedisplay();
	}
	else if (option == 999) {
		printf("You selected Quit menu\n");
		exit(0);
	}
}
