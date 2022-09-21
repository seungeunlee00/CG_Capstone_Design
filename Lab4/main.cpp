#include<gl/glut.h>
#include<stdio.h>
#include<string.h>

void init(void);
void draw(void);
void mouse(int button, int state, int x, int y);
void keyboard(unsigned char key, int x, int y);
void draw_point(void);
void draw_line(void);
void draw_triangle(void);
void draw_string(void* font, const char* str, int x, int y);
void anti_aliasing(void);
void alpha_blending(void);
void main_menu_function(int option);
void sub_menu_function1(int option);

int draw123 = 0; // 1 : point, 2 : line, 3: triangle
int count = 0;
int temp = 0;
int x1, x2, x3 = 0; // (x1,y1), (x2,y2), (x3,y3) 
int y1, y2, y3 = 0;
bool To_draw_point = false;
bool To_draw_line = false;
bool To_draw_triangle = false;
bool anti_on = 0;
bool alpha_on = 0;

int main(int argc, char** argv) {
	int submenu1;

	/* window 초기화 */
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(300, 300);
	glutCreateWindow("12201933 이승은 Lab4");
	init(); // 사용자 초기화 함수

	/* Popup menu 생성 및 추가 */
	submenu1 = glutCreateMenu(sub_menu_function1);
	glutAddMenuEntry("Point", 1);
	glutAddMenuEntry("Line", 2);
	glutAddMenuEntry("Triangle", 3);

	/* 메인 메뉴 */
	glutCreateMenu(main_menu_function);
	glutAddMenuEntry("Quit", 999);
	glutAddMenuEntry("Clear", 10);
	glutAddSubMenu("draw", submenu1);
	glutAttachMenu(GLUT_RIGHT_BUTTON); // 오른쪽 마우스 눌렀을 때 메뉴 뜨게하기

	/* Callback 함수 정의 */
	glutDisplayFunc(draw); // 실제 그리기 함수
	glutMouseFunc(mouse); // 마우스 입력
	glutKeyboardFunc(keyboard); // 키보드 입력 

	/* Looping 시작 */
	glutMainLoop();

	return 0;
}

/* 사용자 초기화 함수 */
void init(void) {
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // 화면의 기본색으로 black 설정

	/* 화면 좌표 정보 설정 */
	glMatrixMode(GL_PROJECTION); // 3차원을 2차원으로 투영
	glLoadIdentity();
	gluOrtho2D(0.0f, 500.0f, 500.0f, 0.0f); // 우리가 보는 방향!

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
	glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
}

/* 그리기 함수 */
void draw(void) {
	if (draw123 == 1 && To_draw_point == true) {
		draw_point();
		To_draw_point = false;
	}
	else if (draw123 == 2 && To_draw_line == true) {
		draw_line();
		To_draw_line = false;
	}
	else if (draw123 == 3 && To_draw_triangle == true) {
		draw_triangle();
		To_draw_triangle = false;
	}
	glColor3f(1.0f, 1.0f, 0);
	draw_string(GLUT_BITMAP_HELVETICA_18, "I Like Computer Graphics!!", 50, 450);

	glFlush();
}

/* 점 그리기 함수 */
void draw_point(void) {
	glColor4f(1.0f, 0.0f, 0.0f, 0.75f); // 점의 색상을 붉은 색으로
	glPointSize(6.0f);

	glBegin(GL_POINTS);
	glVertex2i(x1, y1);
	glEnd();

	glutPostRedisplay();
	count = 0;
}

/* 선 그리기 함수 */
void draw_line(void) {
	glColor4f(1.0f, 1.0f, 0.0f, 0.75f); // 선의 색상을 노란색으로 
	glLineWidth(4.0f);

	glBegin(GL_LINES);
	glVertex2i(x1, y1);
	glVertex2i(x2, y2);
	glEnd();

	glutPostRedisplay();
	count = 0;
}

/*삼각형 그리기 함수 */
void draw_triangle(void) {
	glColor4f(0.3f, 0.5f, 0.0f, 0.75f); // 면의 색상을 초록색으로 

	glBegin(GL_TRIANGLES);
	glVertex2i(x1, y1);
	glVertex2i(x2, y2);
	glVertex2i(x3, y3);
	glEnd();

	glutPostRedisplay();
	count = 0;
}

/* string 그리기 */
void draw_string(void* font, const char* str, int x, int y) {
	unsigned int i;
	glRasterPos2i(x, y);
	for (i = 0; i < strlen(str); i++) {
		glutBitmapCharacter(font, str[i]);
	}
}

/* 마우스 입력 좌표 처리 */
void mouse(int button, int state, int x, int y) {
	if (state == 1)
		printf("button(%d), state(%d), x(%d), y(%d)\n", button, state, x, y);

	if (state == 0 && temp == 1)
		count++;
	temp = state;

	if (draw123 == 1) { // 점
		x1 = x; y1 = y;
		To_draw_point = true;
	}
	else if (draw123 == 2) { // 선
		if (count == 1) {
			x1 = x; y1 = y;
		}
		else if (count == 2) {
			x2 = x; y2 = y;
			To_draw_line = true;
		}
	}
	else if (draw123 == 3) { // 삼각형
		if (count == 1) {
			x1 = x; y1 = y;
		}
		else if (count == 2) {
			x2 = x; y2 = y;
		}
		else if (count == 3) {
			x3 = x; y3 = y;
			To_draw_triangle = true;
		}
	}
}

/* 키보드 입력 처리 */
void keyboard(unsigned char key, int x, int y) {
	if (key == 'a' || key =='A') {
		if (anti_on == true) {
			anti_on = false;
		}
		else if (anti_on == false) {
			anti_on = true;
		}
		anti_aliasing();
	}
	else if (key == 'b' || key =='B') {
		if (alpha_on == true) {
			alpha_on = false;
		}
		else if (alpha_on == false) {
			alpha_on = true;
		}
		alpha_blending();
	}
}

/* anti aliasing on/off */
void anti_aliasing(void) {
	if (anti_on == true) {
		printf("Anti-alising ON\n");
		glEnable(GL_BLEND);
		glEnable(GL_POINT_SMOOTH);
		glEnable(GL_LINE_SMOOTH);
		glEnable(GL_POLYGON_SMOOTH);
		glutPostRedisplay();
	}
	else if (anti_on == false) {
		printf("Anti-alising OFF\n");
		glDisable(GL_POINT_SMOOTH);
		glDisable(GL_LINE_SMOOTH);
		glDisable(GL_POLYGON_SMOOTH);
		glutPostRedisplay();
	}
}

/* alpha blending on/off */
void alpha_blending(void) {
	if (alpha_on == true) {
		printf("Alpha-blending ON\n");
		glEnable(GL_BLEND);
		glutPostRedisplay();
	}
	else if (alpha_on == false && anti_on == false) {
		printf("Alpha-blending OFF\n");
		glDisable(GL_BLEND);
		glutPostRedisplay();
	}
}

/* MENU 처리 함수 – 메뉴 선택시 수행됨 */
void sub_menu_function1(int option) {
	printf("Submenu %d has been selected\n", option);

	if (option == 1) {
		draw123 = 1; // 점 그리기
	}
	else if (option == 2) {
		draw123 = 2; // 선 그리기
	}
	else if (option == 3) {
		draw123 = 3; // 삼각형 그리기
	}
}

void main_menu_function(int option) {
	printf("Main menu %d has been selected\n", option);

	if (option == 999) {
		exit(0);
	}
	else if (option == 10) {
		glClear(GL_COLOR_BUFFER_BIT);
	}
}
