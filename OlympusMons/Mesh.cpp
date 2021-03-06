#include "stdio.h"
#include "stdlib.h"
#include "Math.h"
#include "Mesh.h"


Mesh::Mesh(void)
{
	pixelstep = 1.0;
	scale = 20.0;
	initColors();
}

void Mesh::initColors(void)
{
	riverColor = (float*)malloc(4*sizeof(float));
	if(riverColor==NULL) {printf("Eroare la alocare culori"); return;}
	dirtColor = (float*)malloc(4*sizeof(float));
	if(dirtColor==NULL) {printf("Eroare la alocare culori"); return;}
	grassColor = (float*)malloc(4*sizeof(float));
	if(grassColor==NULL) {printf("Eroare la alocare culori"); return;}
	snowColor = (float*)malloc(4*sizeof(float));
	if(snowColor==NULL) {printf("Eroare la alocare culori"); return;}

	riverColor[0] = 0; riverColor[1] = 0; riverColor[2] = 1; riverColor[3] = 1; 
	dirtColor [0] = 0.5; dirtColor [1] = 0.2; dirtColor [2] = 0.1; dirtColor [3] = 1;
	grassColor[0] = 0; grassColor[1] = 0.8; grassColor[2] = 0; grassColor[3] = 1;
	snowColor [0] = 1; snowColor [1] = 1; snowColor [2] = 1; snowColor [3] = 1;
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
	//glMaterialfv(GL_FRONT, GL_SPECULAR,specref);
	//glMaterialfv(GL_FRONT,GL_SHININESS, mat_shininess);

	//drawUndeformed(UNDEFORMED_X1, 1);
	//drawUndeformed(UNDEFORMED_X2, 2);
	//drawUndeformed(UNDEFORMED_X4, 4);
	//drawDeformed(DEFORMED_X1, 1);
	//drawDeformed(DEFORMED_X2, 2);
	//drawDeformed(DEFORMED_X4, 4);

}

int Mesh::getWidth() {
	if(!image)
		return -1;
	return image->width;
}

int Mesh::getHeight() {
	if(!image)
		return -1;
	return image->height;
}

point_t* Mesh::getVertices() {

	int width, height;
	width = image->width;
	height = image->height;

	point_t* ret = (point_t*)malloc(width * height * sizeof(point_t));
	if(!ret)
		return NULL;

	if(!image)
		return NULL;

	long i,j;
	for(i = 0; i < height; i++) {
		for(j = 0; j < width; j++) {
			ret[(i*width + j)].position[0] = i;
			ret[(i*width + j)].position[1] = image->imageData[i*width + j] / 255.0 * scale;
			ret[(i*width + j)].position[2] = j;
			setColor(ret,i,j);
		}
		//printf("(%f %f %f)\n",ret[i*width*3+width-3],ret[i*width*3+width-2],ret[i*width*3+width-1]);
	}
	
	return ret;
}
void Mesh::setColor(point_t * points, int i , int j)
{
	int width = image->width;
	float minRiver = 0.0 , maxRiver = 10.0/100.0 * scale, riverDif = maxRiver - minRiver;
	float minDirt = 10.0/100.0 * scale, maxDirt = 15.0/100.0 * scale, dirtDif = maxDirt - minDirt;
	float minGrass = 15.0/100.0 * scale, maxGrass = 50.0/100.0 * scale, grassDif = maxGrass - minGrass;
	float minSnow = 50.0/100.0 * scale, maxSnow = scale, snowDif = maxSnow - minSnow;
 
	float vertexHeight = points[i*width+j].position[1];
	float gap = 0.9;
	//setare culoare riverbed
	if(vertexHeight >= minRiver && vertexHeight < maxRiver)
	{
		float pondere = (vertexHeight - minRiver)*gap/riverDif;
		points[i*width+j].color[0]=riverColor[0]*pondere; points[i*width+j].color[1]=riverColor[1]*pondere; 
		points[i*width+j].color[2]=riverColor[2]*pondere; points[i*width+j].color[3]=riverColor[3]*pondere;
	}
	//portiune de pamant
	else if(vertexHeight >= minDirt && vertexHeight <maxDirt)
	{
		float pondere = (vertexHeight - minDirt)*gap/dirtDif;
		points[i*width+j].color[0]=dirtColor[0]*pondere; points[i*width+j].color[1]=dirtColor[1]*pondere; 
		points[i*width+j].color[2]=dirtColor[2]*pondere; points[i*width+j].color[3]=dirtColor[3]*pondere;
	}
	//pajiste
	else if(vertexHeight >= minGrass && vertexHeight < maxGrass)
	{
		float pondere = (vertexHeight - minGrass)*gap/grassDif;
		points[i*width+j].color[0]=grassColor[0]*pondere; points[i*width+j].color[1]=grassColor[1]*pondere; 
		points[i*width+j].color[2]=grassColor[2]*pondere; points[i*width+j].color[3]=grassColor[3]*pondere;
	}
	//zapada
	else if(vertexHeight >= minSnow)
	{
		float pondere = (vertexHeight - minSnow)*gap/snowDif;
		points[i*width+j].color[0]=snowColor[0]*1/pondere; points[i*width+j].color[1]=snowColor[1]*1/pondere; 
		points[i*width+j].color[2]=snowColor[2]*1/pondere; points[i*width+j].color[3]=snowColor[3]*1/pondere;
	}
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

	if(image == NULL)
		return;

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

void Mesh::drawDeformed(unsigned int minx, unsigned int maxx, unsigned int miny, unsigned int maxy, int div) {

	if(image == NULL)
		return;

	unsigned int i,j;
	float step = pixelstep / (float)div;
	float idiv;
	float jdiv;

	float y1, y2, y3, y4;
	float y1div, y2div, y3div, y4div;
	Vector3D normal1, normal2, normal;
	Vector3D color1,color2,color3,color4;

	int width, height;
	width = image->width;
	height = image->height;

	glBegin(GL_QUADS);
	for(i = minx; i < maxx-1; i++)
		for(j = miny; j < maxy-1; j++) {
			
			y1 = (float)image->imageData[i*width + j] / 255.0 * scale;
			y2 = (float)image->imageData[i*width + j+1] / 255.0 * scale;
			y3 = (float)image->imageData[(i+1)*width + j+1] / 255.0 * scale;
			y4 = (float)image->imageData[(i+1)*width + j] / 255.0 * scale;
			
			int tesHeight = pixelstep/step+1;
			int tesWidth = pixelstep/step+1;
			int tesX,tesY;
			
			float * xTes = (float*)malloc(tesWidth*tesHeight*sizeof(float));
			float * yTes = (float*)malloc(tesWidth*tesHeight*sizeof(float));
			float * zTes = (float*)malloc(tesWidth*tesHeight*sizeof(float));
			
			
			//precalcularea punctelor intermediare si a normalelor
			for(tesX=0,idiv = 0; idiv < pixelstep;tesX++,idiv+=step)
			{
				for(tesY=0,jdiv = 0; jdiv < pixelstep;tesY++,jdiv+=step)
				{
					
					y1div = inBetween(jdiv, inBetween(idiv, y1, y4), inBetween(idiv, y2, y3));
					y2div = inBetween(jdiv+step, inBetween(idiv, y1, y4), inBetween(idiv, y2, y3));
					y3div = inBetween(jdiv+step, inBetween(idiv+step, y1, y4), inBetween(idiv+step, y2, y3));
					y4div = inBetween(jdiv, inBetween(idiv+step, y1, y4), inBetween(idiv+step, y2, y3));

					xTes[tesX*tesWidth+tesY] = i+idiv;				yTes[tesX*tesWidth+tesY]= y1div;		  zTes[tesX*tesWidth+tesY] = j+jdiv;
					xTes[tesX*tesWidth+tesY+1] = i+idiv;			yTes[tesX*tesWidth+tesY+1] = y2div;	      zTes[tesX*tesWidth+tesY+1] = j+jdiv+step;
					xTes[(tesX+1)*tesWidth+tesY+1] = i+idiv+step;   yTes[(tesX+1)*tesWidth+tesY+1] = y3div;   zTes[(tesX+1)*tesWidth+tesY+1] = j+jdiv+step;
					xTes[(tesX+1)*tesWidth+tesY] = i+idiv+step;		yTes[(tesX+1)*tesWidth+tesY] = y4div;	  zTes[(tesX+1)*tesWidth+tesY] = j+jdiv;
				}
			}
			Vector3D * normals = computeNormalsForTesArea(tesHeight,tesWidth,xTes,yTes,zTes);
			

			for(tesX=0,idiv = 0; idiv < pixelstep; idiv+=step,tesX++)
			{
				for(tesY=0,jdiv = 0; jdiv < pixelstep; jdiv+=step,tesY++)
				{
					y1div = inBetween(jdiv, inBetween(idiv, y1, y4), inBetween(idiv, y2, y3));
					y2div = inBetween(jdiv+step, inBetween(idiv, y1, y4), inBetween(idiv, y2, y3));
					y3div = inBetween(jdiv+step, inBetween(idiv+step, y1, y4), inBetween(idiv+step, y2, y3));
					y4div = inBetween(jdiv, inBetween(idiv+step, y1, y4), inBetween(idiv+step, y2, y3));
					// TODO: adaugare normale si culori aici

					// astea sunt niste noromale calculate de mine, nu prea sunt decente pentru ca sunt per suprafata
					normal1 = calculateNormal(i+idiv, y1div, j+jdiv, i+idiv, y2div, j+jdiv+step, i+idiv+step, y3div, j+jdiv+step);
					normal2 = calculateNormal(i+idiv, y1div, j+jdiv, i+idiv+step, y3div, j+jdiv+step, i+idiv+step, y4div, j+jdiv);
					normal = (normal1 + normal2).Normalize();
					glNormal3f(normal.x,normal.y,normal.z);

					color1 = computeColorPerVertex(y1div);
					color2 = computeColorPerVertex(y2div);
					color3 = computeColorPerVertex(y3div);
					color4 = computeColorPerVertex(y4div);

					//glNormal3f(normals[tesX*tesWidth+tesY].x, normals[tesX*tesWidth+tesY].y, normals[tesX*tesWidth+tesY].z);	//1
					glColor3f(color1.x,color1.y,color1.z);
					glVertex3f(i+idiv, y1div, j+jdiv);			

					//glNormal3f(normals[tesX*tesWidth+tesY+1].x, normals[tesX*tesWidth+tesY+1].y, normals[tesX*tesWidth+tesY+1].z);	//2
					glColor3f(color2.x,color2.y,color2.z);
					glVertex3f(i+idiv, y2div, j+jdiv+step);		

					//glNormal3f(normals[(tesX+1)*tesWidth+tesY+1].x, normals[(tesX+1)*tesWidth+tesY+1].y, normals[(tesX+1)*tesWidth+tesY+1].z);	//3
					glColor3f(color3.x,color3.y,color3.z);
					glVertex3f(i+idiv+step, y3div, j+jdiv+step);

					//glNormal3f(normals[(tesX+1)*tesWidth+tesY].x, normals[(tesX+1)*tesWidth+tesY].y, normals[(tesX+1)*tesWidth+tesY].z);	//4
					glColor3f(color4.x,color4.y,color4.z);
					glVertex3f(i+idiv+step, y4div, j+jdiv);							
				}
				
			}

			free(xTes); free(yTes); free(zTes);
			free(normals);
		}
	glEnd();
}
Vector3D Mesh::computeColorPerVertex(float height)
{
	float minRiver = 0.0 , maxRiver = 10.0/100.0 * scale, riverDif = maxRiver - minRiver;
	float minDirt = 10.0/100.0 * scale, maxDirt = 15.0/100.0 * scale, dirtDif = maxDirt - minDirt;
	float minGrass = 15.0/100.0 * scale, maxGrass = 50.0/100.0 * scale, grassDif = maxGrass - minGrass;
	float minSnow = 50.0/100.0 * scale, maxSnow = scale, snowDif = maxSnow - minSnow;

	float gap = 0.9;
	//setare culoare riverbed
	if(height >= minRiver && height < maxRiver)
	{
		float pondere = (height - minRiver)*gap/riverDif;
		float red=riverColor[0]*pondere, green = riverColor[1]*pondere, blue = riverColor[2]*pondere;
		return Vector3D(red,green,blue);
	}
	//portiune de pamant
	else if(height >= minDirt && height <maxDirt)
	{
		float pondere = (height - minDirt)*gap/dirtDif;
		float red=dirtColor[0]*pondere, green = dirtColor[1]*pondere, blue = dirtColor[2]*pondere;
		return Vector3D(red,green,blue);
	}
	//pajiste
	else if(height >= minGrass && height < maxGrass)
	{
		float pondere = (height - minGrass)*gap/grassDif;
		float red=grassColor[0]*pondere, green = grassColor[1]*pondere, blue = grassColor[2]*pondere;
		return Vector3D(red,green,blue);
	}
	//zapada
	else
	{
		float pondere = (height - minSnow)*gap/snowDif;
		float red=snowColor[0]*1/pondere, green = snowColor[1]*1/pondere, blue = snowColor[2]*1/pondere;
		return Vector3D(red,green,blue);
	}
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

Vector3D addNormalsForEdges(Vector3D center,Vector3D px1,Vector3D px2,Vector3D px3)
{
	Vector3D v1 = px1-center;
	Vector3D v2 = px2-center;
	Vector3D v3 = px3-center;

	Vector3D norm1 = v1.CrossProduct(v2);
	norm1=norm1.Normalize();
	Vector3D norm2 = v3.CrossProduct(v2);
	norm2=norm2.Normalize();

	Vector3D sum = norm1+norm2;
	return sum.Normalize();
}

void computeNomalsForCorners(int i,int j,int length, int width, float*x,float*y,float*z,Vector3D * normals)
{
	//stanga sus
	if(i==0 && j==0)
	{
		Vector3D centru = Vector3D(x[0],y[0],z[0]);
		Vector3D dreapta = Vector3D(x[1],y[1],z[1]);
		Vector3D jos = Vector3D(x[width],y[width],z[width]);

		Vector3D v1 = dreapta-centru;
		Vector3D v2 = jos-centru;

		Vector3D norm = v1.CrossProduct(v2);
		norm = norm.Normalize();
		//printf("Primul X%f Y%f Z%f",norm.x,norm.y,norm.z);
		normals[0]=norm;

	}
	//dreapta sus
	if(i==0 && j==width-1)
	{
		Vector3D centru = Vector3D(x[width-1],y[width-1],z[width-1]);
		Vector3D stanga = Vector3D(x[width-2],y[width-2],z[width-2]);
		Vector3D jos = Vector3D(x[2*width-1],y[2*width-1],z[2*width-1]);

		Vector3D v1 = jos-centru;
		Vector3D v2 = stanga-centru;

		Vector3D norm = v1.CrossProduct(v2);
		norm = norm.Normalize();

		normals[width-1]=norm;

	}
	//dreapta jos
	if(i==length-1 && j==width-1)
	{
		Vector3D centru = Vector3D(x[i*width+j],y[i*width+j],z[i*width+j]);
		Vector3D sus = Vector3D(x[(i-1)*width+j],y[(i-1)*width+j],z[(i-1)*width+j]);
		Vector3D stanga = Vector3D(x[i*width+j-1],y[i*width+j-1],z[i*width+j-1]);

		Vector3D v1 = sus-centru;
		Vector3D v2 = stanga-centru;

		Vector3D norm = v1.CrossProduct(v2);
		norm = norm.Normalize();
		
		normals[i*width+j]=norm;
	}
	//stanga jos
	if(i==length-1 && j==0)
	{
		Vector3D centru = Vector3D(x[i*width+j],y[i*width+j],z[i*width+j]);
		Vector3D sus = Vector3D(x[(i-1)*width+j],y[(i-1)*width+j],z[(i-1)*width+j]);
		Vector3D dreapta = Vector3D(x[i*width+j+1],y[i*width+j+1],z[i*width+j+1]);

		Vector3D v1 = dreapta-centru;
		Vector3D v2 = sus-centru;
		
		Vector3D norm = v1.CrossProduct(v2);
		norm = norm.Normalize();

		normals[i*width+j]=norm;

	}
}

void computeNormalsForEdges(int i, int j,int length, int width, float*x,float*y,float*z,Vector3D * normals)
{
	//sus
	if(i==0 && j!=0 && j!= width-1)
	{
		Vector3D centru = Vector3D(x[i*width+j],y[i*width+j],z[i*width+j]);
		Vector3D stanga = Vector3D(x[i*width+j-1],y[i*width+j]-1,z[i*width+j-1]);
		Vector3D jos = Vector3D(x[(i+1)*width+j],y[(i+1)*width+j],z[(i+1)*width+j]);
		Vector3D dreapta = Vector3D(x[i*width+j+1],y[i*width+j+1],z[i*width+j+1]);
		
		normals[i*width+j]=addNormalsForEdges(centru,stanga,jos,dreapta);
	}
	//dreapta
	if(i!=0 && i!=length-1 && j==width-1)
	{
		Vector3D centru = Vector3D(x[i*width+j],y[i*width+j],z[i*width+j]);
		Vector3D sus = Vector3D(x[(i-1)*width+j-1],y[(i-1)*width+j]-1,z[(i-1)*width+j-1]);
		Vector3D jos = Vector3D(x[(i+1)*width+j],y[(i+1)*width+j],z[(i+1)*width+j]);
		Vector3D stanga = Vector3D(x[i*width+j-1],y[i*width+j-1],z[i*width+j-1]);
		

		normals[i*width+j]=addNormalsForEdges(centru,stanga,jos,sus);
	}
	//jos
	if(i==length-1 && j!= width-1 && j!=0)
	{
		Vector3D centru = Vector3D(x[i*width+j],y[i*width+j],z[i*width+j]);
		Vector3D sus = Vector3D(x[(i-1)*width+j-1],y[(i-1)*width+j]-1,z[(i-1)*width+j-1]);
		Vector3D stanga = Vector3D(x[i*width+j-1],y[i*width+j-1],z[i*width+j-1]);
		Vector3D dreapta = Vector3D(x[i*width+j+1],y[i*width+j+1],z[i*width+j+1]);

		normals[i*width+j]=addNormalsForEdges(centru,stanga,dreapta,sus);

	}
	//stanga
	if(i!=0 && i!=length-1 && j==0)
	{
		Vector3D centru = Vector3D(x[i*width+j],y[i*width+j],z[i*width+j]);
		Vector3D sus = Vector3D(x[(i-1)*width+j-1],y[(i-1)*width+j]-1,z[(i-1)*width+j-1]);
		Vector3D jos = Vector3D(x[(i+1)*width+j],y[(i+1)*width+j],z[(i+1)*width+j]);
		Vector3D dreapta = Vector3D(x[i*width+j+1],y[i*width+j+1],z[i*width+j+1]);


		normals[i*width+j]=addNormalsForEdges(centru,jos,dreapta,sus);
	}
}

void computeNormalsForInterior(int i, int j,int length, int width, float*x,float*y,float*z,Vector3D * normals)
{
	//compute interior
	if(i>0 && i<length-1 && j>0 && j<width-1)
	{
		Vector3D centru = Vector3D(x[i*width+j],y[i*width+j],z[i*width+j]);
		Vector3D sus = Vector3D(x[(i-1)*width+j-1],y[(i-1)*width+j]-1,z[(i-1)*width+j-1]);
		Vector3D jos = Vector3D(x[(i+1)*width+j],y[(i+1)*width+j],z[(i+1)*width+j]);
		Vector3D stanga = Vector3D(x[i*width+j-1],y[i*width+j-1],z[i*width+j-1]);
		Vector3D dreapta = Vector3D(x[i*width+j+1],y[i*width+j+1],z[i*width+j+1]);

		Vector3D v1 = sus-centru;
		Vector3D v2 = dreapta-centru;
		Vector3D v3 = jos-centru;
		Vector3D v4 = stanga-centru;

		Vector3D norm1 = v1.CrossProduct(v2);
		norm1 = norm1.Normalize();
		Vector3D norm2 = v2.CrossProduct(v3);
		norm2 = norm2.Normalize();
		Vector3D norm3 = v3.CrossProduct(v4);
		norm3 = norm3.Normalize();
		Vector3D norm4 = v4.CrossProduct(v1);
		norm4 = norm4.Normalize();

		Vector3D sum = norm1+norm2+norm3+norm4;
		sum = sum.Normalize();
			
		normals[i*width+j]=sum;
	}
}


Vector3D* Mesh::computeNormalsForTesArea(int testHeight, int tesWidth, float *x, float*y, float * z)
{
	Vector3D* normals = (Vector3D*)malloc(testHeight*tesWidth*sizeof(Vector3D));
	if(normals == NULL){printf("Eroare la alocarea spatiului pentru normale teselare");return NULL;}

	for(int i=0;i<testHeight;i++)
	{
		for(int j=0;j<tesWidth;j++)
		{
			computeNomalsForCorners(i,j,testHeight,tesWidth,x,y,z,normals);
			computeNormalsForEdges(i,j,testHeight,tesWidth,x,y,z,normals);
			computeNormalsForInterior(i,j,testHeight,tesWidth,x,y,z,normals);
		}
	}
	return normals;
	
}

void computeNormals(int length, int width, point_t* points)
{
	Vector3D* normals = (Vector3D*)malloc(length*width*sizeof(Vector3D));
	if(normals == NULL){printf("Eroare la alocarea spatiului pentru normale");return;}
	
	float * x = (float*)malloc(length*width*sizeof(float));
	if(x==NULL) {printf("Eroare la alocarea spatiului pt x"); return;}

	float * y = (float*)malloc(length*width*sizeof(float));
	if(y==NULL) {printf("Eroare la alocarea spatiului pt y"); return;}
	
	float * z = (float*)malloc(length*width*sizeof(float));
	if(z==NULL) {printf("Eroare la alocarea spatiului pt z"); return;}

	//copiez din vectorul mare in cele 3 mici
	for(int i=0;i<width*length;i++)
	{
		x[i]=points[i].position[0];
		y[i]=points[i].position[1];
		z[i]=points[i].position[2];
	}

	for(int i=0;i<length;i++)
	{
		for(int j=0;j<width;j++)
		{
			computeNomalsForCorners(i,j,length,width,x,y,z,normals);
			computeNormalsForEdges(i,j,length,width,x,y,z,normals);
			computeNormalsForInterior(i,j,length,width,x,y,z,normals);
		}
	}

	free(x);
	free(y);
	free(z);
	
	for (int i = 0; i < width*length; ++i)
	{
		points[i].normal[0] = normals[i].x;
		points[i].normal[1] = normals[i].y;
		points[i].normal[2] = normals[i].z;
	}
	free(normals);
}

