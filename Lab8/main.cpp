#include<stdio.h>
#include<GL/glut.h>

void init();
void draw();
void idle();
void resize(int, int);
void main_menu_function(int);

double m_RotateAngle = 0;

int main(int argc, char** argv) {
	/* Window 초기화 */
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(500, 100);
	glutCreateWindow("12201933 이승은 Lab8");
	init(); // 사용자 초기화 함수

	/* Popup menu 생성 및 추가 */
	glutCreateMenu(main_menu_function);
	glutAddMenuEntry("Quit", 999);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	/* Callback 함수 정의 */
	glutReshapeFunc(resize);
	glutDisplayFunc(draw);
	glutIdleFunc(idle);

	/* Looping 시작 */
	glutMainLoop();

	return 0;
}

/* 초기화 함수 */
void init() {
	glClearColor(0.f, 0.f, 0.f, 0.f); // 화면의 기본색 -> 검정색
	glClearDepth(1.0f); // Buffer clearing시 이용할 값

	// Light 설정
	GLfloat ambientLight[] = { 0.2f, 0.2f, 0.2f, 1.0f };
	GLfloat diffuseLight[] = { 0.7f, 0.7f, 0.7f, 1.0f };
	GLfloat specularLight[] = { 0.9f, 0.9f, 0.9f, 1.0f };
	GLfloat specularMaterial[] = { 1.0f, 1.0f, 1.0f, 1.0f };

	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);

	glMaterialfv(GL_FRONT, GL_SPECULAR, specularMaterial);
	glMateriali(GL_FRONT, GL_SHININESS, 20);

	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	/* Depth - Test Enable */ 
	glFrontFace(GL_CW); // CW로 정의 - Default는 CCW
	glEnable(GL_DEPTH_TEST); // Depth buffer 활성화

	/* 비교 */
	// glPolygonMode(GL_BACK, GL_LINE); // Teapot 내부 wireframe
	glEnable(GL_CULL_FACE); // Back-face culling

	printf("init 함수 호출\n");
}

void draw() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(10, 10, 10, 0, 0, 0, 0, 1, 0);

	glColor3f(1.0f, 1.0f, 0.0f);

	glPushMatrix();
	glRotatef(m_RotateAngle, 0, 1, 0);
	GLdouble eq[4] = { 1.0, 0.0, 0.0, 0.0 };
	glClipPlane(GL_CLIP_PLANE0, eq);
	glEnable(GL_CLIP_PLANE0);
	glPopMatrix();
	
	glutSolidTeapot(4);
	glFlush();
}

void idle() {
	m_RotateAngle += 0.05;
	if (m_RotateAngle > 360)
		m_RotateAngle -= 360;
	glutPostRedisplay();
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

void main_menu_function(int option) {
	printf("Main menu %d has been selected\n", option);

	if (option == 999) {
		exit(0);
