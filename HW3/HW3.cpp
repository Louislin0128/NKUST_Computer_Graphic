#include <iostream>
#include <cmath>
#include <windows.h>
#include "glut.h"
using namespace std;

// Control parameter
double Base_X = 0.0;
double Base_Z = 0.0;
double Link1_Rotate = 0.0;
double Gripper_OpenLength = 0.0;

// function prototype
void display();
void init();
void mainmenu(int id);
void Base_XSubmenu(int id);
void Base_ZSubmenu(int id);
void Link1_RotateSubmenu(int id);
void Link4_HeightSubmenu(int id);
void Gripper_Submenu(int id);
void DrawGround();
void DrawCube(float scaleX, float scaleY, float scaleZ);

// Arm Link Setting
double Base_Width = 0.4;
double Base_Length = 0.4;
double Base_Height = 0.4;

double Link1_Width = 0.2;
double Link1_Length = 0.2;
double Link1_Height = 0.6;

double Link2_Width = 0.2;
double Link2_Length = 1.0;
double Link2_Height = 0.2;

double Link3_Width = 0.2;
double Link3_Length = 0.2;
double Link3_Height = 0.2;

double Link4_Width = 0.1;
double Link4_Length = 0.1;
double Link4_Height = 0.2;

double Link5_Width = 0.1;
double Link5_Length = 0.6;
double Link5_Height = 0.1;

double Gripper_Width = 0.1;
double Gripper_Length = 0.1;
double Gripper_Height = 0.2;

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("simple");

	glutDisplayFunc(display);

	init();
	glutMainLoop();
	return 0;
}

void display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	/** 
	gluLookAt函式：
		第一組是腦袋的位置
		第二組是眼睛看物體的位置
		第三組是頭頂朝向的方向
	*/

	//正常的視角
	gluLookAt(0.0, 1.0, 1.0,  0.5, -1.0, 0.0,  0.0, 1.0, 0.0);

	//不正常的視角，測試Base_所用
	//gluLookAt(1.0, 1.0, 1.0, 0.0, -1.0, 0.0, 0.0, 1.0, 0.0);
	//gluLookAt(0.0, 10.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, -1.0);

	DrawGround();

	//draw base
	glTranslatef(Base_X, 0.2, Base_Z);
	//glRotatef(0.0, 0.0, 1.0, 0.0);
	DrawCube(Base_Length, Base_Height, Base_Width);

	//draw link1
	glTranslatef(0.0, Base_Height / 2.0, 0.0);
	glRotatef(Link1_Rotate, 0.0, 1.0, 0.0);
	glTranslatef(0.0, Link1_Height / 2.0, 0.0);
	DrawCube(Link1_Length, Link1_Height, Link1_Width);

	//draw link2
	glTranslatef(Link1_Length / 2.0 + Link2_Length / 2.0, 
		Link1_Height / 2.0 - Link2_Height / 2.0, 0.0);
	DrawCube(Link2_Length, Link2_Height, Link2_Width);

	//draw link3
	glTranslatef(Link2_Length / 2.0 - Link3_Length / 2.0, 
		-(Link2_Height / 2.0 + Link3_Height / 2.0), 0.0);
	DrawCube(Link3_Length, Link3_Height, Link3_Width);

	//draw link4
	glTranslatef(Link3_Length / 4.0 - Link4_Length / 2.0, 
		-(Link3_Height / 2.0 + Link4_Height / 2.0), 0.0);
	DrawCube(Link4_Length, Link4_Height, Link4_Width);

	//draw link5
	glTranslatef(Link4_Length - Link5_Length / 6.0, 
		-(Link4_Height / 2.0 + Link5_Height / 2.0), 0.0);
	DrawCube(Link5_Length, Link5_Height, Link5_Width);

	//draw Gripper left (x軸往負走)
	glPushMatrix();
	glTranslatef(-(Gripper_Length / 2.0 + Gripper_OpenLength), -(Link5_Height / 2.0 + Gripper_Height / 2.0), 0.0);
	DrawCube(Gripper_Length, Gripper_Height, Gripper_Width);
	glPopMatrix();

	//draw Gripper right (x軸往正走)
	glTranslatef(+(Gripper_Length / 2.0 + Gripper_OpenLength), -(Link5_Height / 2.0 + Gripper_Height / 2.0), 0.0);
	DrawCube(Gripper_Length, Gripper_Height, Gripper_Width);

	glFlush();
	glutSwapBuffers();
}

void init() {
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glColor3f(1.0, 1.0, 1.0);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-2.0, 2.0, -2.0, 2.0, -2.0, 2.0);

	int submenu1 = glutCreateMenu(Base_XSubmenu);
	glutAddMenuEntry("Incease", 1);
	glutAddMenuEntry("Decrease", 2);

	int submenu2 = glutCreateMenu(Base_ZSubmenu);
	glutAddMenuEntry("Incease", 1);
	glutAddMenuEntry("Decrease", 2);

	//整體逆時鐘轉或順時鐘轉
	int submenu3 = glutCreateMenu(Link1_RotateSubmenu);
	glutAddMenuEntry("Clockwise", 1);
	glutAddMenuEntry("CounterClockwise", 2);

	//控制夾子的伸長與縮短
	int submenu4 = glutCreateMenu(Link4_HeightSubmenu);
	glutAddMenuEntry("Up", 1);
	glutAddMenuEntry("Down", 2);

	//夾子
	int submenu5 = glutCreateMenu(Gripper_Submenu);
	glutAddMenuEntry("Open", 1);
	glutAddMenuEntry("Close", 2);

	//set main menu
	glutCreateMenu(mainmenu);
	glutAddSubMenu("Base X Position", submenu1);
	glutAddSubMenu("Base Z Position", submenu2);
	glutAddSubMenu("Link 1 Rotation", submenu3);
	glutAddSubMenu("Link 4 Height", submenu4);
	glutAddSubMenu("Gripper", submenu5);

	glutAddMenuEntry("Home", 1);
	glutAddMenuEntry("Exit", 2);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

void mainmenu(int id) {
	switch (id) {
	case 1:
		Base_X = 0.0;
		Base_Z = 0.0;
		Link1_Rotate = 0.0;
		Link4_Height = 0.2;
		Gripper_OpenLength = 0.0;
		break;
	case 2:
		exit(0);
		break;
	default:
		cout << "mainmenu(): Invalid id "<<id<< endl;
		break;
	}
	glutPostRedisplay();
}

void Base_XSubmenu(int id) {
	switch (id) {
	case 1:
		Base_X += 0.05;
		break;
	case 2:
		Base_X -= 0.05;
		break;
	default:
		cout << "Base_XSubmenu(): Invalid id " << id << endl;
		break;
	}
	glutPostRedisplay();
}

void Base_ZSubmenu(int id) {
	switch (id) {
	case 1:
		Base_Z += 0.05;
		break;
	case 2:
		Base_Z -= 0.05;
		break;
	default:
		cout << "Base_ZSubmenu(): Invalid id " << id << endl;
		break;
	}
	glutPostRedisplay();
}

void Link1_RotateSubmenu(int id) {
	switch (id) {
	case 1:
		Link1_Rotate += 5;
		break;
	case 2:
		Link1_Rotate -= 5;
		break;
	default:
		cout << "Link1_RotateSubmenu(): Invalid id " << id << endl;
		break;
	}
	glutPostRedisplay();
}

void Link4_HeightSubmenu(int id) {
	switch (id) {
	case 1:
		Link4_Height -= 0.05;
		if (Link4_Height < 0.1) {
			Link4_Height = 0.1;
		}
		break;
	case 2:
		Link4_Height += 0.05;
		if (Link4_Height > 0.25) {
			Link4_Height = 0.25;
		}
		break;
	default:
		cout << "Link4_HeightSubmenu(): Invalid id "<<id<< endl;
		break;
	}
	cout << "Link4_HeightSubmenu(): " << Link4_Height << endl;
	glutPostRedisplay();
}

void Gripper_Submenu(int id) {
	switch (id) {
	case 1:
		Gripper_OpenLength += 0.05;
		if (Gripper_OpenLength > 0.15) {
			Gripper_OpenLength = 0.15;
		}
		break;
	case 2:
		Gripper_OpenLength -= 0.05;
		if (Gripper_OpenLength < 0.0) {
			Gripper_OpenLength = 0.0;
		}
		break;
	default:
		cout << "Gripper_Submenu(): Invalid id " << id << endl;
		break;
	}
	glutPostRedisplay();
}

void DrawGround() {
	glColor3f(1.0, 1.0, 1.0);
	glBegin(GL_LINE_LOOP);
	glVertex3f(2.0, 0.0, -2.0);
	glVertex3f(-2.0, 0.0, -2.0);
	glVertex3f(-2.0, 0.0, 2.0);
	glVertex3f(2.0, 0.0, 2.0);
	glEnd();
}

void DrawCube(float scaleX, float scaleY, float scaleZ) {
	glPushMatrix();
	glScalef(scaleX, scaleY, scaleZ);
	glutWireCube(1.0);
	glPopMatrix();
}
