#include "stdio.h"
#include "stdlib.h"
#include "Math.h"
#include "Mesh.h"


Mesh::Mesh(void)
{
	pixelstep = 1.0;
	scale = 15.0;
}


Mesh::~Mesh(void)
{
	if(image != NULL)
		tgaDestroy(image);
	glDeleteLists(1,6);
}

bool Mesh::loadImage(char* path) {
	
	GLuint ret;

	if(image != NULL) {
		tgaDestroy(image);
		image = NULL;
	}

	image = tgaLoad(path);

	if(image->status != TGA_OK)
		return false;

	ret = glGenLists(6);
	if( !ret )
		return false;

	// Light values and coordinates
	GLfloat specref[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat mat_shininess[] = { 255.0 };
	// Enable color tracking
	glEnable(GL_COLOR_MATERIAL);
	// Set Material properties to follow glColor values
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	// All materials hereafter have full specular reflectivity
	// with a high shine
	glMaterialfv(GL_FRONT, GL_SPECULAR,specref);
	glMaterialfv(GL_FRONT,GL_SHININESS, mat_shininess);

	drawUndeformed(UNDEFORMED_X1, 1);
	drawUndeformed(UNDEFORMED_X2, 2);
	drawUndeformed(UNDEFORMED_X4, 4);
	drawDeformed(DEFORMED_X1, 1);
	drawDeformed(DEFORMED_X2, 2);
	drawDeformed(DEFORMED_X4, 4);

}

void Mesh::drawUndeformed(GLuint list, int div) {

	int i,j;
	float step = pixelstep / (float)div;
	float idiv;
	float jdiv;

	int width, height;
	float y = 0;

	width = image->width;
	height = image->height;

	if(image == NULL)
		return;

	glNewList(list,GL_COMPILE);
	glColor3f( 0.25, 1.0, 0);
	glBegin(GL_QUADS);
	for(i = 0; i < height; i++)
		for(j = 0; j < width; j++) {
			for(idiv = 0; idiv < pixelstep; idiv+=step)
				for(jdiv = 0; jdiv < pixelstep; jdiv+=step)
				{
					glNormal3f(0, 1, 0);
					glVertex3f(i+idiv, 0, j+jdiv);
					glVertex3f(i+idiv, 0, j+jdiv+step);
					glVertex3f(i+idiv+step, 0, j+jdiv+step);
					glVertex3f(i+idiv+step, 0, j+jdiv);		
				}
		}
	glEnd();
	glEndList();
}

void Mesh::drawDeformed(GLuint list, int div) {

	int i,j;
	float step = pixelstep / (float)div;
	float idiv;
	float jdiv;

	float y1, y2, y3, y4;
	float y1div, y2div, y3div, y4div;
	Vector3D normal1, normal2, normal;
	GLfloat green[] = { 0.0f, 0.75f, 0.0f, 1.0f };

	int width, height;

	width = image->width;
	height = image->height;

	if(image == NULL)
		return;

	glNewList(list,GL_COMPILE);
	glColor3f( 0.25, 1.0, 0);

	glBegin(GL_QUADS);
	for(i = 0; i < height - 1; i++)
		for(j = 0; j < width -1; j++) {
			
			y1 = (float)image->imageData[i*width + j] / 255.0 * scale;
			y2 = (float)image->imageData[i*width + j+1] / 255.0 * scale;
			y3 = (float)image->imageData[(i+1)*width + j+1] / 255.0 * scale;
			y4 = (float)image->imageData[(i+1)*width + j] / 255.0 * scale;

			for(idiv = 0; idiv < pixelstep; idiv+=step)
				for(jdiv = 0; jdiv < pixelstep; jdiv+=step)
				{
					y1div = inBetween(jdiv, inBetween(idiv, y1, y4), inBetween(idiv, y2, y3));
					y2div = inBetween(jdiv+step, inBetween(idiv, y1, y4), inBetween(idiv, y2, y3));
					y3div = inBetween(jdiv+step, inBetween(idiv+step, y1, y4), inBetween(idiv+step, y2, y3));
					y4div = inBetween(jdiv, inBetween(idiv+step, y1, y4), inBetween(idiv+step, y2, y3));

					normal1 = calculateNormal(i+idiv, y1div, j+jdiv, i+idiv, y2div, j+jdiv+step, i+idiv+step, y3div, j+jdiv+step);
					normal2 = calculateNormal(i+idiv, y1div, j+jdiv, i+idiv+step, y3div, j+jdiv+step, i+idiv+step, y4div, j+jdiv);

					normal = (normal1 + normal2).Normalize();
					glNormal3f(normal.x, normal.y, normal.z);
					
					glVertex3f(i+idiv, y1div, j+jdiv);			//1
					glVertex3f(i+idiv, y2div, j+jdiv+step);		//2
					glVertex3f(i+idiv+step, y3div, j+jdiv+step);//3
					glVertex3f(i+idiv+step, y4div, j+jdiv);		//4					
				}
		}
	glEnd();
	glEndList();
}

inline float Mesh::inBetween(float idiv, float y1, float y2) {

	return (y1 * (pixelstep-idiv) + y2 * idiv)/pixelstep;
}

Vector3D Mesh::calculateNormal(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3) {
		Vector3D a;
		Vector3D b;
		Vector3D normal;
 
		a.x = x1 - x2;
		a.y = y1 - y2;
		a.z = z1 - z2;
 
		b.x = x2 - x3;
		b.y = y2 - y3;
		b.z = z2 - z3;
 
		return a.CrossProduct(b).Normalize();
}