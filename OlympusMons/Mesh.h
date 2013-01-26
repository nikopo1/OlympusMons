#pragma once
#ifndef _MESH_H
#define _MESH_H

#include "Vector3D.h"
#include "tga.h"
#include "glut.h"

#define UNDEFORMED_X1	1
#define UNDEFORMED_X2	2
#define UNDEFORMED_X4	3

#define DEFORMED_X1		4
#define DEFORMED_X2		5
#define DEFORMED_X4		6

#define NO_MESH			6

typedef struct _point_t {
	GLfloat position[3];
	GLfloat normal[3];
	GLfloat texture[2];
} point_t;


class Mesh
{
private:
	float pixelstep;
	float scale;
	tgaInfo* image;

public:

	Mesh(void);

	bool loadImage(char* path);
	void drawUndeformed(GLuint list, int div);
	void drawDeformed(GLuint list, int div);

	int getWidth();
	int getHeight();
	point_t* getVertices();

	inline float inBetween(float idiv, float y1, float y2);
	Vector3D calculateNormal(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3);

	~Mesh(void);
};

void computeNormals(int length, int width, point_t* points);

#endif