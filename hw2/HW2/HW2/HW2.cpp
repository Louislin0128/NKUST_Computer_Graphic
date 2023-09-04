#include <iostream>
#include <cmath>
#include <windows.h>
#include "glut.h"

int n_points = 50;
double delta_theta = 1.0;

// ellipse parameter
double ellipse_a = 2.0;
double ellipse_b = 1.0;
double cx = 0.0, cy = 0.0, theta = 0.0;

// function prototype
void display();
void init();
void idle();
void mainmenu(int id);
void menu_movement(int id);
void keyboard(unsigned char key, GLint x, GLint y);
void mouse(GLint button, GLint state, GLint x, GLint y);

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("simple");

	glutDisplayFunc(display);
	glutIdleFunc(idle);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouse);

	init();
	glutMainLoop();
	return 0;
}

void display() {
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0, 1.0, 1.0);

	glBegin(GL_POLYGON);
	double degree = 360.0f / (double)n_points;
	double x = 0.0, y = 0.0;

	for (int i = 0; i < n_points; i++) {
		double dg = ((double) i * degree) * 3.14159 / 180.0;
		x = ellipse_a * cos(dg);
		y = ellipse_b * sin(dg);

		dg = ((double) theta) * 3.14159 / 180.0;
		double x_prime = x * cos(dg) - y * sin(dg);
		double y_prime = x * sin(dg) + y * cos(dg);

		x_prime += cx;
		y_prime += cy;
		glVertex2f(x_prime, y_prime);
	}
	glEnd();

	glFlush();
	glutSwapBuffers();
}

void init() {
	int menubar_control = glutCreateMenu(menu_movement);
	glutAddMenuEntry("Start", 2);
	glutAddMenuEntry("Abort", 3);

	int menubar_rotation = glutCreateMenu(menu_movement);
	glutAddMenuEntry("Clockwise", 4);
	glutAddMenuEntry("Conterclockwise", 5);

	int menubar_translation = glutCreateMenu(menu_movement);
	glutAddMenuEntry("Increase x", 6);
	glutAddMenuEntry("Decrease x", 7);
	glutAddMenuEntry("Increase y", 8);
	glutAddMenuEntry("Decrease y", 9);

	//set main menu
	glutCreateMenu(mainmenu);
	glutAddSubMenu("Rotation", menubar_control);
	glutAddSubMenu("Direction", menubar_rotation);
	glutAddSubMenu("Translation", menubar_translation);
	glutAddMenuEntry("Quit", 11);

	glutAttachMenu(GLUT_RIGHT_BUTTON);

	glClearColor(0.0, 0.0, 0.0, 0.0);
	glColor3f(1.0, 1.0, 1.0);
	
	/* initialize viewing values  */
	glLoadIdentity();
	gluOrtho2D(-3.0, 3.0, -3.0, 3.0);
}

void idle() {
	theta = theta + delta_theta;

	//out of range process
	if (theta >= 360.0) theta = theta - 360.0;
	if (theta < 0.0) theta = theta + 360.0;

	Sleep(1);
	glutPostRedisplay();
}

void mainmenu(int id) {
	switch (id) {
		case 11: exit(0);
	}
}

void menu_movement(int id) {
	/* button "Start" */ 
	if (id == 2) glutIdleFunc(idle);

	/* button "Abort" */
	if (id == 3) glutIdleFunc(NULL);

	/* button "Clockwise" */
	if (id == 4) delta_theta = -1.0;

	/* button "Counterclockwise" */
	if (id == 5) delta_theta = 1.0;

	/* button "inc/dec x" */
	if (id == 6) cx = cx + 0.2;
	if (id == 7) cx = cx - 0.2;

	/* button "inc/dec y" */
	if (id == 8) cy = cy + 0.2;
	if (id == 9) cy = cy - 0.2;
	glutPostRedisplay();
}

void keyboard(unsigned char key, GLint x, GLint y) {
	if (key == 'q' || key == 'Q') {
		exit(0);
	}
}

void mouse(GLint button, GLint state, GLint x, GLint y) {
	switch (button) {
		case GLUT_LEFT_BUTTON:
			if (state == GLUT_DOWN) {
				cx = ((double)x / 500.0) * 6.0 - 3.0;
				cy = ((double)(500.0 - y) / 500.0) * 6.0 - 3.0;
			}
			break;

		case GLUT_RIGHT_BUTTON:
			if (state == GLUT_DOWN)
				glutIdleFunc(NULL);
			break;

		default:
			break;
	}
}
