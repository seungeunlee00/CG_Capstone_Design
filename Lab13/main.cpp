#include <windows.h>
#include <gl/glut.h>
#include <stdio.h>

float g_pPosition[6] = { 0.0f, 144.0f, -100.0f, -100.0f, 100.0f, -100.0f };
int g_nX, g_nY;
int g_nSelect = 0;
int g_nGLWidth = 500, g_nGLHeight = 500;

void init(void) {
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	GLfloat light_ambient[] = { 0.1, 0.1, 0.1, 1.0 };
	GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	GLfloat light_position[] = { 1.0, 1.0, 1.0, 0.0 };
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	GLfloat mat_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialf(GL_FRONT, GL_SHININESS, 64);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-g_nGLWidth / 2.0f, g_nGLWidth / 2.0f, -g_nGLHeight / 2.0f,
		g_nGLHeight / 2.0f, -100, 100);
}

void DrawBackground() {
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glDisable(GL_LIGHTING);
	glColor3f(1.0f, 1.0f, 1.0f);
	int i;
	int offset = 50;
	glBegin(GL_LINES);
	for (i = -g_nGLWidth / 2.0f; i < g_nGLWidth / 2.0f; i += offset) {
		glVertex3f(i, -g_nGLHeight / 2.0f, 0.0f);
		glVertex3f(i, g_nGLHeight / 2.0f, 0.0f);
	}
	for (i = -g_nGLHeight / 2.0f; i < g_nGLHeight / 2.0f; i += offset) {
		glVertex3f(-g_nGLWidth / 2.0f, i, 0.0f);
		glVertex3f(g_nGLWidth / 2.0f, i, 0.0f);
	}
	glEnd();
	glPopAttrib();
}

void DrawSphere() {
	glLoadName(1);
	glPushMatrix();
	glTranslatef(g_pPosition[0], g_pPosition[1], 0.0f);
	glColor3f(1, 0, 0);
	glutSolidSphere(25.0f, 30, 30);
	glPopMatrix();

	glLoadName(2);
	glPushMatrix();
	glTranslatef(g_pPosition[2], g_pPosition[3], 0.0f);
	glColor3f(0, 1, 0);
	glutSolidSphere(25.0f, 30, 30);
	glPopMatrix();

	glLoadName(3);
	glPushMatrix();
	glTranslatef(g_pPosition[4], g_pPosition[5], 0.0f);
	glColor3f(0, 0, 1);
	glutSolidSphere(25.0f, 30, 30);
	glPopMatrix();
}

void draw(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	DrawBackground();
	DrawSphere();
	glFlush();
	glutSwapBuffers();
}

void resize(int width, int height) {
	g_nGLWidth = width;
	g_nGLHeight = height;
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-width / 2.0f, width / 2.0f, -height / 2.0f, height / 2.0f,
		-100, 100);
	glMatrixMode(GL_MODELVIEW);
}


void Picking(int x, int y) {
	// first step
	GLuint selectBuf[256];
	glSelectBuffer(256, selectBuf); // buffer에 GLuint 형의 배열에 picking한 개체에 대한 정보 저장

	// second step
	glRenderMode(GL_SELECT); // GL_SELECT를 사용하여 선택 모드 설정

	// third step
	glInitNames(); 
	// 개체의 이름(정수)를 등록해두면 이름 스택을 초기화
	// 반드시 glRenderMode(GL_SELECT)를 호출 후 실행해야 한다.
	glPushName(-1);
	// glPushName에 들어가는 parameter는 개체의 이름을 나타낸다.
	// glInitName() 직후 호출하면 이름 스택의 선두에 임시로 -1을 넣어둔다.

	// fourth step
	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);
	glMatrixMode(GL_PROJECTION);  // 선택의 처리는 시점 좌표계에서 실시하므로 투시 변환 행렬 설정
	glPushMatrix(); // 투시 변환 행렬(glMatrix(GL_PROJECTION))을 matrix stack에 저장
	glLoadIdentity(); // 투시 변환 행렬 초기화
	gluPickMatrix(x, y, 0.1, 0.1, viewport);
	// 표시 영역이 마우스 포인터 주위(viewport)만 사용하도록 변환 행렬 설정 
    // 마우스 클릭시 x, y가 선택 영역의 중심 좌표가 된다. 
	// 그 뒤 인자는 dx, dy로 작아질 수록 세밀하게 선택 가능하다

	// five step
	glOrtho(-g_nGLWidth / 2.0f, g_nGLWidth / 2.0f, -g_nGLHeight / 2.0f,
		g_nGLHeight / 2.0f, -100, 100); // glOrtho 3D를 투영하기 위한 ORTHO 생성
	glMatrixMode(GL_MODELVIEW); // object를 화면에서 보기위한 시각 변환 행렬
	glPushMatrix();
	DrawSphere(); // 내부에서 glLoadName(1),(2),(3)을 호출해가며 차례대로 구 생성
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glFlush();

	// sixth step
	GLint hits = glRenderMode(GL_RENDER);
	if (hits <= 0) return;

	// seven step
	int stack = selectBuf[0];
	unsigned int zMin = selectBuf[1];
	unsigned int zMax = selectBuf[2];
	g_nSelect = selectBuf[3];
	int index = 3 + stack;
	int i;
	for (i = 1; i < hits; i++) {
		stack = selectBuf[index];
		if (zMax > selectBuf[index + 2]) {
			zMax = selectBuf[index + 2];
			g_nSelect = selectBuf[index + 3];
		}
		index += 3 + stack;
	}
	printf("hits : %d\n", hits);

	for (int i = 0; i < 10; i++) {
		if (selectBuf[i] == 0) break;
		printf("%u ", selectBuf[i]);
	}
	printf("\ng_nSelect : %d\n", g_nSelect);
}

void mouse(int button, int state, int x, int y) {
	y = g_nGLHeight - y;
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		Picking(x, y);
		g_nX = x;
		g_nY = y;
	} else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
		g_nSelect = 0;
	}
}
void motion(int x, int y) {
	y = g_nGLHeight - y;
	if (g_nSelect > 0) {
		g_pPosition[(g_nSelect - 1) * 2 + 0] += x - g_nX;
		g_pPosition[(g_nSelect - 1) * 2 + 1] += y - g_nY;
		g_nX = x;
		g_nY = y;
		glutPostRedisplay();
	}
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(g_nGLWidth, g_nGLHeight);
	glutInitWindowPosition(300, 300);
	glutCreateWindow("12201933 이승은 Lab13");
	init();
	glutDisplayFunc(draw);
	glutReshapeFunc(resize);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);
	glutMainLoop();
	return 0;
}
