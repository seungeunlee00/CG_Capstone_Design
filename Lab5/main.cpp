#include <gl/glut.h>
#include <stdio.h>
#include <windows.h>

bool spin_state = true; // 회전 상태
double spin = 0; // 회전 각도
int draw_mode = 0; // 도형 선택
int x_trans = 0, y_trans = 0; // x, y 좌표 방향으로 평행 이동량
bool scale_state = false; // scale 상태
double scale = 1.0; // scale 배수

void init(void) {
    // 화면의 기본색으로 주황색 설정
    glClearColor(1.0f, 0.5f, 0.0f, 0.0f);

    // 화면 좌표 정보 설정
    glMatrixMode(GL_PROJECTION); // 3차원을 2차원으로 투영
    glLoadIdentity();
    gluOrtho2D(0.0f, 500.0f, 0.0f, 500.0f);
}

void idle(void) {
    if (spin_state) {
        spin += 0.1; // 회전각도 0.1 증가
        if (spin > 360)
            spin -= 360;
    }
    glutPostRedisplay();
}

// 삼각형 그리기 함수
void draw_triangle(void) {
    // 면의 색상을 노란색으로
    glColor4f(1.0f, 1.0f, 0.0f, 0.75f);
    glBegin(GL_TRIANGLES);

    glVertex2i(100, 300);
    glVertex2i(100, 100);
    glVertex2i(300, 200);

    glEnd();
}

// Polygon 그리기 함수
void draw_polygon(void) {
    // 면의 색상을 노란색으로
    glColor4f(1.0f, 1.0f, 0.0f, 0.75f);
    glBegin(GL_POLYGON);

    glVertex2i(100, 300);
    glVertex2i(100, 100);
    glVertex2i(300, 100);
    glVertex2i(300, 200);

    glEnd();
}

void draw(void) {
    // 화면을 깨끗하게 지우기
    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glTranslatef(250 + x_trans, 250 + y_trans, 0); // 평행 이동
    glRotatef(spin, 0, 0, 1); // 회전
    glScalef(scale, scale, scale); // scale
    glTranslatef(-250 - x_trans, -250 - y_trans, 0); // 평행 이동

    switch (draw_mode) {
    case 1:
        draw_triangle();      
        break;
    case 2:
        draw_polygon();
        break;
    default:
        break;
    }

    glFlush();
    glutSwapBuffers();
}

// Menu 처리 함수 - 메뉴 선택시 수행됨
void sub_menu_function(int option) {
    // Triangle 서브메뉴 선택 시
    if (option == 1) {
        printf("Submenu Triangle has been selected\n");
        draw_mode = 1;
    }
    // Polygon 서브메뉴 선택 시
    else if (option == 2) {
        printf("Submenu Polygon has been selected\n");
        draw_mode = 2;
    }
}

void main_menu_function(int option) {
    // Spin 메뉴 선택 시
    if (option == 1) {
        printf("You selected Spin menu\n");
        if (spin_state)
            spin_state = false;
        else
            spin_state = true;
    }
    // X Trans 메뉴 선택 시
    else if (option == 2) {
        printf("You selected X Trans menu\n");
        x_trans += 5;
    }
    // Y Trans 메뉴 선택 시
    else if (option == 3) {
        printf("You selected Y Trans menu\n");
        y_trans += 5;
    }
    // Scale 메뉴 선택 시
    else if (option == 4) {
        printf("You selected Scale menu\n");
        if (scale_state) {
            scale = 1.0;
            scale_state = false;
        }
        else {
            scale = 0.7;
            scale_state = true;
        }
    }
    // Quit 메뉴 선택 시
    else if (option == 999) {
        printf("You selected Quit menu\n");
        exit(0);
    }
    glutPostRedisplay(); // 윈도우 다시 그리기
}

// 키보드 입력 콜백 함수
void keyboard(unsigned char key, int x, int y) {
    printf("You pressed %c\n", key);
    if (key == 's')
        if (spin_state)
            spin_state = false;
        else
            spin_state = true;
    glutPostRedisplay();
}

int main(int argc, char** argv) {
    int submenu;

    // 윈도우 초기화
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(500, 100);
    glutCreateWindow("12201933 이승은 Lab5");
    init(); // 사용자 초기화 함수

    // 서브 메뉴 생성 및 추가
    submenu = glutCreateMenu(sub_menu_function);
    glutAddMenuEntry("Triangle", 1);
    glutAddMenuEntry("Polygon", 2);

    // 메인 메뉴 생성
    glutCreateMenu(main_menu_function);
    glutAddSubMenu("Shape", submenu);
    glutAddMenuEntry("Spin", 1);
    glutAddMenuEntry("X Trans", 2);
    glutAddMenuEntry("Y Trans", 3);
    glutAddMenuEntry("Scale", 4);
    glutAddMenuEntry("Quit", 999);
    glutAttachMenu(GLUT_RIGHT_BUTTON);

    // Idle 콜백 함수
    glutIdleFunc(idle); // 컴퓨터의 유휴시간에 호출

    // 디스플레이 콜백 함수
    glutDisplayFunc(draw); // draw() : 실제 그리기 함수

    // 키보드 콜백 함수
    glutKeyboardFunc(keyboard);

    // Looping의 시작
    glutMainLoop();

    return 0;
}
