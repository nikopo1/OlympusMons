#include "Viewer.h"
#include "glut.h"
#include "stdio.h"
#include <math.h>

#define PI	3.14159

Viewer::Viewer(	double px, double py, double pz, double fx, double fy, double fz, double upx, double upy, double upz)
{
	Vector3D ceva(px, py, pz);
	position =	Vector3D(px, py, pz);
	forward =	Vector3D(fx, fy, fz);
	up =		Vector3D(upx, upy, upz);
	right =		forward.CrossProduct(up);
	angle = 5.0*PI/180.0;
}


Viewer::~Viewer(void)
{
}

void Viewer::moveForward(int distance) {
	//position = position + forward * distance;
	position.x = position.x + forward.x * distance;
	position.z = position.z + forward.z * distance;
	setViewer();
}
void Viewer::moveRight(int distance) {
	//position = position - right * distance;
	position.x = position.x + right.x * distance;
	position.z = position.z + right.z * distance;
	setViewer();
}

void Viewer::lookUp() {
	Vector3D oldforward = forward;
	forward = oldforward*cos(angle) - up*sin(angle);
	up = oldforward*sin(angle) + up*cos(angle);
	right = forward.CrossProduct(up).Normalize();

	setViewer();
}
void Viewer::lookDown() {
	Vector3D oldforward = forward;
	forward = oldforward*cos(angle) + up*sin(angle);
	up = -oldforward*sin(angle) + up*cos(angle);
	right = forward.CrossProduct(up);

	setViewer();
}

void Viewer::lookLeft() {
	Vector3D oldforward = forward;
	forward = oldforward*cos(angle) - right*sin(angle);
	right = oldforward*sin(angle) + right*cos(angle);
	up = right.CrossProduct(forward);

	setViewer();
}
void Viewer::lookRight() {
	Vector3D oldforward = forward;
	forward = oldforward*cos(angle) + right*sin(angle);
	right = -oldforward*sin(angle) + right*cos(angle);
	up = right.CrossProduct(forward);

	setViewer();
}



void Viewer::setViewer() {

	forward = forward.Normalize();
	up = up.Normalize();
	right = right.Normalize();

	int w,h;
	w = glutGet(GLUT_WINDOW_WIDTH);
	h = glutGet(GLUT_WINDOW_HEIGHT);

	glViewport(0,0, (GLsizei) w, (GLsizei) h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(90, (float)w/h, 1.0, 200.0); 
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// vedere de deasupra
	gluLookAt(position.x, position.y, position.z,   //observatorul este in origine departat pe Y
		      position.x + forward.x, position.y + forward.y, position.z + forward.z,   //si priveste in directia negativa a axei oy
			  up.x, up.y, up.z);

	printf("*****************************\n");
	printf("position=[%lf, %lf, %lf]\n", position.x, position.y, position.z);
	printf("forward=[%lf, %lf, %lf]\n", forward.x, forward.y, forward.z);
	printf("right=[%lf, %lf, %lf]\n", right.x, right.y, right.z);
	printf("up=[%lf, %lf, %lf]\n", up.x, up.y, up.z);
}