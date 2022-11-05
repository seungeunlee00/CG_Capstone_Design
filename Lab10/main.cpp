#include<stdio.h>
#include<GL/glut.h>

void init();
void draw();
void draw_axis();
void resize(int, int);
void idle();
void main_menu_function(int);
void keyboard(unsigned char, int, int);
void draw_string(void* font, const char* str, int x, int y, int z);

bool spin_state = 0;

int cameraLight = 0;
int solarLight = 0;
int earthLight = 0;
int moonLight = 0;
int worldLight = 0;

float sunAngle = 0; // 태양 자전
float earthAngle1 = 0; // 지구 자전
float earthAngle2 = 0; // 지구 공전
float moonAngle1 = 0; // 달 자전
float moonAngle2 = 0; // 달 공전

int main(int argc, char** argv) {
	/* Window 초기화 */
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(500, 100);
	glutCreateWindow("12201933 이승은 Lab10");
	init(); // 사용자 초기화 함수

	/* Callback 함수 정의 */
	glutReshapeFunc(resize);
	glutDisplayFunc(draw);
	glutIdleFunc(idle);
	glutKeyboardFunc(keyboard);

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

	/* 0번 조명 관련 설정 */
	GLfloat light_ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
	GLfloat light_diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	// 재질의 반사 특성 설정 – init()에 추가
	GLfloat ambient_Sun[] = { 0.2f, 0.2f, 0.2f, 1.0f };
	GLfloat diffuse_Sun[] = { 1.0f, 0.0f, 0.0f, 1.0f };
	GLfloat specular_Sun[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glMaterialfv(GL_FRONT, GL_AMBIENT, ambient_Sun);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_Sun);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular_Sun);
	glMaterialf(GL_FRONT, GL_SHININESS, 64);

	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

	glEnable(GL_DEPTH_TEST);
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
		earthAngle1 = earthAngle1 + 0.05;
		if (earthAngle1 > 360)
			earthAngle1 -= 360;
		earthAngle2 = earthAngle2 + 0.05;
		if (earthAngle2 > 360)
			earthAngle2 -= 360;

		/* 달의 자전, 공전 각도 변화 */
		moonAngle1 = moonAngle1 + 0.05;
		if (moonAngle1 > 360)
			moonAngle1 -= 360;
		moonAngle2 = moonAngle2 + 0.05;
		if (moonAngle2 > 360)
			moonAngle2 -= 360;
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
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	/* 조명 카메라에 위치 */
	if (cameraLight == 1) { 
		GLfloat light_position[] = { 0.0, 0.0, 0.0, 1.0 };
		glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	}
	gluLookAt(10, 10, 10, 0, 0, 0, 0, 1, 0);

	/* 조명 (-15, -15, 15)에 위치 */
	glTranslatef(-15, -15, 15);
	if (worldLight == 1) {
		GLfloat light_position[] = { 0.0, 0.0, 0.0, 1.0 };
		glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	}
	glTranslatef(15, 15, -15);

	/* String 그리기 */
	glColor3f(1.0f, 1.0f, 0.0f);
	draw_string(GLUT_BITMAP_HELVETICA_18, "12201933 Seungeun Lee", -11, -17, 5);

	/* 태양 */
	glRotatef(sunAngle, 0, 1, 0); // 태양 자전
	glColor3f(1, 0, 0);

	GLfloat emission[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	if (solarLight == 1) {
		emission[0] = 1.0f;
		glMaterialfv(GL_FRONT, GL_EMISSION, emission);
	}

	glutSolidSphere(3, 50, 50); // 태양 그리기
	draw_axis(); // World 좌표계 그리기

	if (solarLight == 1) {
		GLfloat light_position[] = { 0.0, 0.0, 0.0, 1.0 };
		glLightfv(GL_LIGHT0, GL_POSITION, light_position);
		
		emission[0] = 0.0f;
		glMaterialfv(GL_FRONT, GL_EMISSION, emission);
	}

	/* 지구 */
	glRotatef(earthAngle1, 0, 1, 0); // 지구 자전
	glTranslatef(4, 0, 3);
	glRotatef(earthAngle2, 0, 1, 0); // 지구 공전
	glColor3f(0, 0, 1);

	if (earthLight == 1) {
		emission[2] = 1.0f;
		glMaterialfv(GL_FRONT, GL_EMISSION, emission);
	}

	glutSolidSphere(1, 50, 50); // 지구 그리기

	if (earthLight == 1) {
		GLfloat light_position[] = { 0.0, 0.0, 0.0, 1.0 };
		glLightfv(GL_LIGHT0, GL_POSITION, light_position);
		
		emission[2] = 0.0f;
		glMaterialfv(GL_FRONT, GL_EMISSION, emission);
	}

	/* 달 */
	glRotatef(moonAngle1, 0, 1, 0); // 달 자전
	glTranslatef(1.2, 0, 1.2);
	glRotatef(moonAngle2, 0, 1, 0); // 달 공전
	glColor3f(0, 1, 1);

	if (moonLight == 1) {
		emission[1] = 1.0f;
		emission[2] = 1.0f;
		glMaterialfv(GL_FRONT, GL_EMISSION, emission);
	}

	glutSolidSphere(0.3, 50, 50); // 달 그리기

	if (moonLight == 1) {
		GLfloat light_position[] = { 0.0, 0.0, 0.0, 1.0 };
		glLightfv(GL_LIGHT0, GL_POSITION, light_position);
		
		emission[1] = 0.0f;
		emission[2] = 0.0f;
		glMaterialfv(GL_FRONT, GL_EMISSION, emission);
	}

	glFlush();
}

void draw_string(void* font, const char* str, int x, int y, int z) {
	unsigned int i;
	glRasterPos3i(x, y, z);
	for (i = 0; i < strlen(str); i++) {
		glutBitmapCharacter(font, str[i]);
	}
}

void keyboard(unsigned char key, int x, int y) {
	if (key == '0') {
		printf("Camera Light\n");

		cameraLight = 1;
		solarLight = 0;
		earthLight = 0;
		moonLight = 0;
		worldLight = 0;
		glutPostRedisplay();
	}
	else if (key == '1') {
		printf("Solar Light\n");

		cameraLight = 0;
		solarLight = 1;
		earthLight = 0;
		moonLight = 0;
		worldLight = 0;
		glutPostRedisplay();
	}
	else if (key == '2') {
		printf("Earth Light\n");

		cameraLight = 0;
		solarLight = 0;
		earthLight = 1;
		moonLight = 0;
		worldLight = 0;
		glutPostRedisplay();
	}
	else if (key == '3') {
		printf("Moon Light\n");

		cameraLight = 0;
		solarLight = 0;
		earthLight = 0;
		moonLight = 1;
		worldLight = 0;
		glutPostRedisplay();
	}
	else if (key == '4') {
		printf("(-15, -15, 15) Light\n");

		cameraLight = 0;
		solarLight = 0;
		earthLight = 0;
		moonLight = 0;
		worldLight = 1;
		glutPostRedisplay();
	}
}

void main_menu_function(int option) {
	if (option == 999) {
		printf("exit has been selected\n");
		exit(0);
	}
	else if (option == 1) {
		printf("Spin ON/OFF has been selected\n");
		glClear(GL_COLOR_BUFFER_BIT);
		spin_state = !spin_state;
	}
}
