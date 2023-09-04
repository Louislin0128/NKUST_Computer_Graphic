#include <iostream>
#include "glut.h"
#include <cmath>
using namespace std;
#define Type 4

void display();
void init();

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("simple");

	glutDisplayFunc(display);
	init();

	glutMainLoop();
}

void display() {
	glClear(GL_COLOR_BUFFER_BIT);

	glBegin(GL_POLYGON);
	double degree = 2 * 3.1415926 / (double)Type, dg = 0.0;
	double x = 0.0, y = 0.0;

	for (int i = 0; i < Type; i++) {
		x = 1.0 * sin(dg);
		y = 1.0 * cos(dg);

		glVertex2f(x, y);

		dg += degree;
	}
	glEnd();

	glFlush();
}

void init() {
	glColor3f(1.0, 1.0, 1.0);
	glClearColor(0.0, 0.0, 0.0, 0.0);

	/* initialize viewing values  */
	glLoadIdentity();
	gluOrtho2D(-2.0, 2.0, -2.0, 2.0);
}
