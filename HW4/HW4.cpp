#include <iostream>
#include <cmath>
#include <windows.h>
#include <time.h>
#include "glut.h"
#include "DisjointSet.h"
using namespace std;

#define MAZE_ROWS  10
#define MAZE_COLS  10

/* parameter */
int	maze[MAZE_ROWS][MAZE_COLS][4];
int view_mode = 0;	/*0: side view;	1: bird - eye*/ 

int user_row = 0;
int	user_col = 0;
int	user_face = 0;

/* gluLookAt的相關設定 */
double eye_x = -4.5, eye_z = -4.5, at_x = -3.5, at_z = -4.5;

/* function prototype */
void init();
void init_maze();
void display();
void display_maze();
void display_user();
void display_side();
void keyboard(unsigned char key, int x, int y);
void mouse(GLint button, GLint state, GLint x, GLint y);
void idle();

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("simple");

	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouse);
	//glutIdleFunc(idle);

	init();
	glutMainLoop();
	return 0;
}

void init() {
	init_maze();
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.0, 0.0, 0.0, 0.0);
}

void init_maze() {
	DisjointSet* p_set = new DisjointSet(MAZE_ROWS * MAZE_COLS);

	for (int r = 0; r < MAZE_ROWS; r++)
		for (int c = 0; c < MAZE_COLS; c++)
			for (int s = 0; s < 4; s++)
				maze[r][c][s] = 1;

	// randomly remove the side until all cells are connected
	srand(time(NULL));
	while (p_set->get_no_set() != 1) {
		bool is_found = false;
		int cur_row, cur_col, cur_side;
		int nei_row = 0, nei_col = 0, nei_side = 0;

		while (!is_found) {
			// randomly take an existing side
			cur_row = rand() % MAZE_ROWS;
			cur_col = rand() % MAZE_COLS;
			cur_side = rand() % 4;
			if (maze[cur_row][cur_col][cur_side] != 1)
				continue;

			// write the code to set nei_row, nei_col, // and nei_side 自己寫
			switch (cur_side) {
			case 0:
				nei_row = cur_row;
				nei_col = cur_col + 1;
				nei_side = 2;
				break;
			case 1:
				nei_row = cur_row - 1;
				nei_col = cur_col;
				nei_side = 3;
				break;
			case 2:
				nei_row = cur_row;
				nei_col = cur_col - 1;
				nei_side = 0;
				break;
			default:
				nei_row = cur_row + 1;
				nei_col = cur_col;
				nei_side = 1;
				break;
			}

			// check the valid of the neighbor cell
			if (nei_row >= 0 && nei_row < MAZE_ROWS && nei_col >= 0 && nei_col < MAZE_COLS) {
				is_found = true;
			}
		}

		int x = cur_row * MAZE_COLS + cur_col;
		int y = nei_row * MAZE_COLS + nei_col;

		if (p_set->find(x) != p_set->find(y)) {
			// remove the sides at the two cells
			maze[cur_row][cur_col][cur_side] = 0;
			maze[nei_row][nei_col][nei_side] = 0;

			// union x and y
			p_set->union_(x, y);
		}
	}
}

void display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// set the projection
	switch (view_mode) {
	case 0: 
		glOrtho(-1.0, +1.0, 0.0, 10.0, 0.0, 1.0);
		break;
	case 1: 
		glOrtho(-10.0, +10.0, -10.0, +10.0,	0.0, 10.0);
		break;
	}

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	// draw the eye view
	switch (view_mode) {
	case 0:
		gluLookAt(eye_x, 0.0, eye_z, at_x, 0.0, at_z, 0.0, 1.0, 0.0);
		break;
	case 1: 
		gluLookAt(0.0, 10.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, -1.0);
		break;
	}

	// draw the 3D maze
	display_maze();
	if (view_mode == 1)  display_user();

	glFlush();
	glutSwapBuffers();
}

void display_maze() {
	glPushMatrix();
	glTranslatef(-5.0, 0.0, -5.0);
	for (int r = 0; r < MAZE_ROWS; r++)
		for (int c = 0; c < MAZE_COLS; c++)
			for (int s = 0; s < 4; s++) {
				glPushMatrix();

				glTranslatef((float)c * 1.0 + 0.5, 0.0, (float)r * 1.0 + 0.5);
				glRotatef((float)s * 90, 0.0, 1.0, 0.0);

				/* 設置一面牆 */
				if (maze[r][c][s] == 1) display_side();

				glPopMatrix();
			}
	glPopMatrix();
}

void display_user() {
	glPushMatrix();

	glTranslatef((float)user_col * 1.0 + 0.5 - 5.0, 0.0, (float)user_row * 1.0 + 0.5 - 5.0);
	glRotatef((float) user_face * 90, 0.0, 1.0, 0.0);

	glColor3f(1.0, 1.0, 1.0);
	glBegin(GL_POLYGON);
	glVertex3f(+0.1, 5.0, +0.1);
	glVertex3f(+0.1, 5.0, -0.1);
	glVertex3f(-0.1, 5.0, -0.1);
	glVertex3f(-0.1, 5.0, +0.1);
	glEnd();

	glColor3f(0.0, 1.0, 0.0);
	glBegin(GL_LINES);
	glVertex3f(0.0, 5.0, 0.0);
	glVertex3f(+0.5, 5.0, 0.0);
	glEnd();
	glPopMatrix();
}

/*蓋一面牆，自己寫*/
void display_side() {
	glColor3f(1.0, 1.0, 1.0);
	glBegin(GL_POLYGON);
	glVertex3f(0.5, 0.0, -0.5);
	glVertex3f(0.5, 5.0, -0.5);
	glVertex3f(0.5, 5.0, +0.5);
	glVertex3f(0.5, 0.0, +0.5);
	glEnd();
	glDisable(GL_POLYGON_OFFSET_FILL);

	glColor3f(1.0, 0.0, 0.0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glBegin(GL_POLYGON);
	glVertex3f(0.5, 0.0, -0.5);
	glVertex3f(0.5, 5.0, -0.5);
	glVertex3f(0.5, 5.0, +0.5);
	glVertex3f(0.5, 0.0, +0.5);
	glEnd();
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

/* 使用鍵盤調整視角，"0"為第一人稱，"1"為鳥瞰角度 */
void keyboard(unsigned char key, int x, int y) {
	if (key == 'q' || key == 'Q') {
		exit(0);
	}else if (key == 'f' || key == 'F') {
		view_mode = 0;
		glutPostRedisplay();
	}else if (key == 'b' || key == 'B') {
		view_mode = 1;
		glutPostRedisplay();
	}
}

/* 使用滑鼠決定行走方向 */
void mouse(GLint button, GLint state, GLint x, GLint y) {
	if (button == GLUT_RIGHT_BUTTON	&& state == GLUT_DOWN) {
		user_face = user_face - 1;
		if (user_face < 0) 
			user_face = 3;
	}else if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		user_face = (user_face + 1) % 4;
	}else if (button == GLUT_MIDDLE_BUTTON && state == GLUT_DOWN) {
		// move forward，要在迷宮範圍內，所以要記得防呆
		if (maze[user_row][user_col][user_face] != 1) {
			switch (user_face) {
			case 0:
				user_col++;//面東
				break;
			case 1:
				user_row--;//面北
				break;
			case 2:
				user_col--;//面西
				break;
			case 3:
				user_row++;//面南
				break;
			default:
				cout << "wrong face direction\n" << endl;
				break;
			}
		}
	}
	eye_x = (float)user_col * 1.0 + 0.5 - 5.0;
	eye_z = (float)user_row * 1.0 + 0.5 - 5.0;

	switch (user_face) {
	case 0:
		at_x = eye_x + 1.0;
		at_z = eye_z;
		break;
	case 1:
		at_x = eye_x;
		at_z = eye_z - 1.0;
		break;
	case 2:
		at_x = eye_x - 1.0;
		at_z = eye_z;
		break;
	case 3:
		at_x = eye_x;
		at_z = eye_z + 1.0;
		break;
	default:
		break;
	}
	glutPostRedisplay();
}

void idle() {
	/*increase eye_x*/
	eye_x = eye_x + 0.01;

	Sleep(1);
	glutPostRedisplay();
}




