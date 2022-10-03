#include<stdio.h>
#include<GL/glut.h>

void init();
void draw();
void draw_axis();
void resize(int, int);
void idle();
void main_menu_function(int);

bool spin_state = 0; 

float sunAngle = 0; // 태양 자전
float earthAngle1 = 0; // 지구 자전
float earthAngle2 = 0; // 지구 공전
float moonAngle1 = 0; // 달 자전
float moonAngle2 = 0; // 달 공전
float marsAngle1 = 0; // 화성 자전
float marsAngle2 = 0; // 화성 공전

int main(int argc, char** argv) {
	/* Window 초기화 */
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(500, 100);
	glutCreateWindow("12201933 이승은 Lab7");
	init(); // 사용자 초기화 함수

	/* Callback 함수 정의 */
	glutReshapeFunc(resize);
	glutDisplayFunc(draw);
	glutIdleFunc(idle);

	/* Popup menu 생성 및 추가 */
	glutCreateMenu(main_menu_function);
	glutAddMenuEntry("Quit", 999);
	glutAddMenuEntry("Spin ON/OFF", 1);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	/* Looping 시작 */
	glutMainLoop();

	return 0;
}

/* 초기화 함수 */
void init() {
	/* 화면의 기본색으로 검정색 설정 */
	glClearColor(0.f, 0.f, 0.f, 0.f);

	printf("init 함수 호출\n");
}

/* 윈도우 생성 및 크기 변화 호출 */
void resize(int width, int height) {
	glViewport(0, 0, 500, 500); // viewport T.F
	glMatrixMode(GL_PROJECTION); 
	glLoadIdentity();
	gluPerspective(45, (float)width / (float)height, 1, 500); // projection T.F
	glMatrixMode(GL_MODELVIEW);

	printf("resize 함수 호출\n");
}

void idle(void) {
	if (spin_state) {
		/* 태양의 자전 각도 변화 */
		sunAngle = sunAngle + 0.01;
		if (sunAngle > 360)
			sunAngle -= 360;
		
		/* 지구의 자전, 공전 각도 변화 */
		earthAngle1 = earthAngle1 + 0.1;
		if (earthAngle1 > 360)
			earthAngle1 -= 360;
		earthAngle2 = earthAngle2 + 2;
		if (earthAngle2 > 360)
			earthAngle2 -= 360;

		/* 달의 자전, 공전 각도 변화 */
		moonAngle1 = moonAngle1 + 0.1;
		if (moonAngle1 > 360)
			moonAngle1 -= 360;
		moonAngle2 = moonAngle2 + 0.5;
		if (moonAngle2 > 360)
			moonAngle2 -= 360;

		/* 화성의 자전, 공전 각도 변화 */
		marsAngle1 = marsAngle1 + 0.1;
		if (marsAngle1 > 360)
			marsAngle1 -= 360;
		marsAngle2 = marsAngle2 + 3;
		if (marsAngle2 > 360)
			marsAngle2 -= 360;
	}
	glutPostRedisplay();
}

void draw_axis() {
	glLineWidth(2); // 좌표축의 두께
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

void draw() {
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(10, 10, 10, 0, 0, 0, 0, 1, 0);

	glRotatef(sunAngle, 0, 1, 0); // 태양 자전
	glColor3f(1, 0, 0);
	glutWireSphere(3, 50, 50); // 태양 그리기
	draw_axis(); // World 좌표계 그리기

	glPushMatrix(); // 현재의 행렬 stack에 저장

	glRotatef(earthAngle1, 0, 1, 0); // 지구 자전
	glTranslatef(4, 0, 3);
	glRotatef(earthAngle2, 0, 1, 0); // 지구 공전
	glColor3f(0, 0, 1);
	glutWireSphere(1, 50, 50); // 지구 그리기

	glRotatef(moonAngle1, 0, 1, 0); // 달 자전
	glTranslatef(1.2, 0, 1.2);
	glRotatef(moonAngle2, 0, 1, 0); // 달 공전
	glColor3f(1, 1, 1); 
	glutWireSphere(0.3, 50, 50); // 달 그리기

	glPopMatrix(); // 태양만 그렸을 때의 상태로 행렬 복귀

	glRotatef(marsAngle1, 0, 1, 0); // 화성 자전
	glTranslatef(6, 0, 3);
	glRotatef(marsAngle2, 0, 1, 0); // 화성 공전
	glColor3f(0, 1, 1);
	glutWireSphere(0.5, 50, 50); // 화성 그리기

	glFlush();
	glutSwapBuffers();
}

void main_menu_function(int option) {
	printf("Main menu %d has been selected\n", option);

	if (option == 999) {
		exit(0);
	}
	else if (option == 1) {
		glClear(GL_COLOR_BUFFER_BIT);
		spin_state = !spin_state;
	}
}
