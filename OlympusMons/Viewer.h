#pragma once
#include "Vector3D.h"

class Viewer
{

private:
	Vector3D forward;
	Vector3D up;
	Vector3D right;
	Vector3D position;

	double angle;

public:
	Viewer(	double px, double py, double poz, double fx, double fy, double fz, double upx, double upy, double upz);

	void moveForward(int distance);
	void moveRight(int distance);
	void moveUp(int distance);

	void lookUp();
	void lookDown();
	void lookLeft();
	void lookRight();

	void setViewer();
	void printPosition();
	double getPosition(int coordinate);

	~Viewer(void);
};

