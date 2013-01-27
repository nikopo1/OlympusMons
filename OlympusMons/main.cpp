// 
// Laborator 2 -- SPG
// 
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")
#pragma comment(lib, "glut32.lib")
#pragma comment(lib, "glew32.lib")

#include <stdarg.h>
#include <stdlib.h>
#include "glew\glew.h"
#include <glut.h>
#include <stdio.h>


#include "Viewer.h"
#include "Mesh.h"
#include "texture.h"

#define DISTANCE	1

#define WIRE	0
#define SOLID	1

// modul initial in care sunt desenate poligoanele
int mode = SOLID;

Viewer view(230,6,210, 0.0, 0.0, 1.0, 0.0, 1.0, 0.0);
int mainWindow;
Mesh mesh;
int mesh_type = 1;

Vector3D spotlight(0.0, 15.0, 0.0);

GLint textura_iarba[4];
point_t* vertices;
GLuint*  indexes;
unsigned int numindexes;

unsigned int width, height;
unsigned int perimeter = 16;

GLuint vertex_id, index_id;


void doLighting() {
	GLfloat light1_position[] = { spotlight.x, spotlight.y, spotlight.z, 1.0 };
	glLightfv(GL_LIGHT1, GL_POSITION, light1_position);
}


// functia de animatie ( functia de idle a GLUT-ului -- similara cu un thread separat)
void spinAnimation()
{
	glutPostRedisplay();
}

// functia pentru procesarea evenimentelor de la tastatura
void keyboard(unsigned char key, int x, int y)
{
	switch(key)
	{
		case 27:
			exit(0);
			break;
		case 'o':
			mode = WIRE;
			break;
		case 'O':
			mode = WIRE;
			break;
		case 'p':
			mode = SOLID;
			break;
		case 'P':
			mode = SOLID;
			break;
		case 'q':
			view.lookLeft();
			break;
		case 'e':
			view.lookRight();
			break;
		case 'v':
			view.lookUp();
			break;
		case 'c':
			view.lookDown();
			break;
		case 'a':
			view.moveRight(-DISTANCE);
			break;
		case 'd':
			view.moveRight(DISTANCE);
			break;
		case 'w':
			view.moveForward(DISTANCE);
			break;
		case 's':
			view.moveForward(-DISTANCE);
			break;
		case 'z':
			view.moveUp(DISTANCE);
			break;
		case 'x':
			view.moveUp(-DISTANCE);
			break;
		case '+':
			perimeter += 1;
			break;
		case '-':
			perimeter = perimeter > 0? perimeter-1:0;
			break;
		case 't':
			spotlight.x = spotlight.x + 1;
			break;
		case 'g':
			spotlight.x = spotlight.x - 1;
			break;
		case 'f':
			spotlight.z = spotlight.z - 1;
			break;
		case 'h':
			spotlight.z = spotlight.z + 1;
			break;
	}
	doLighting();
	glutPostRedisplay();
	//printf("Mesh Type = %d\n", mesh_type);
	//printf("Spotlight [%f, %f, %f]\n", spotlight.x, spotlight.y, spotlight.z);
}


void reshape(int w, int h)
{
	view.setViewer();
	glutPostRedisplay();
}

void idle()
{
	//glutPostRedisplay();
}

void setTexCoords()
{
	for(unsigned int i = 0; i < height-1; i+=2) 
	{
		for(unsigned int j = 0; j < width-1; j+=2) 
		{
			vertices[i*width+j].texture[0]=0; vertices[i*width+j].texture[1]=0;
			vertices[i*width+j+1].texture[0]=0; vertices[i*width+j+1].texture[1]=1;
			vertices[(i+1)*width+j+1].texture[0]=1; vertices[(i+1)*width+j+1].texture[1]=1;
			vertices[(i+1)*width+j].texture[0]=1; vertices[(i+1)*width+j].texture[1]=0;
		}
	}
}

void initVBO() {

	glClearColor(0.0, 0.0, 0.1, 0.0);
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);

	GLfloat ambientLightColor [] =	{0.2,0.2,0.2,1.0};
	GLfloat diffuseLight[] =		{ 0.9f, 0.9f, 0.9f, 0.1f };
	GLfloat lightPos[] =			{ 50.f, 60.0f, 50.0f, 1.0f };
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT,ambientLightColor);
    glEnable(GL_LIGHTING);

    glLightfv(GL_LIGHT0,GL_DIFFUSE,diffuseLight);
    glLightfv(GL_LIGHT0,GL_POSITION,lightPos);
    glEnable(GL_LIGHT0);

    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE);

	GLenum eroare = glewInit();
	
	if( eroare ) {
		printf("Error\n");
	}
	if( glewIsSupported("GL_EXT_texture_filter_anisotropic") ) {
		GLfloat fLargest;
		glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &fLargest);
		printf("Largest: %f\n", fLargest);
		fflush(stdout);
	}

	if( glewIsSupported("GL_ARB_multitexture") ) {
		GLint iUnits; 
		glGetIntegerv(GL_MAX_TEXTURE_UNITS, &iUnits);
		printf("Number of textures= %d\n", iUnits);
	}
	if( glewIsSupported("GL_ARB_vertex_buffer_object") ) {
		printf("Supported.\n");
		int maxvertices, maxindices;
		glGetIntegerv(GL_MAX_ELEMENTS_VERTICES, &maxvertices);
		glGetIntegerv(GL_MAX_ELEMENTS_INDICES,  &maxindices);
		printf("Maximum Number of Vertices=%d\nMax Index Values=%d\n", maxvertices, maxindices);
		fflush(stdout);
	} else {
		printf("Not supported.\n");
		fflush(stdout);
		return;
	}

	width = mesh.getWidth();
	height = mesh.getHeight();
	printf("Width %u Height %u\n",width,height);
	vertices = mesh.getVertices();
	setTexCoords();
	indexes = (GLuint*)malloc( width * height * 4 * sizeof(GLuint));

	unsigned int p = 0;
	for(unsigned int i = 0; i < height-1; i++) {
		for(unsigned int j = 0; j < width-1; j++) {
			indexes[p++] = i*width + j;
			indexes[p++] = i*width + j + 1;
			indexes[p++] = (i+1)*width + j + 1;
			indexes[p++] = (i+1)*width + j;
		}
	}
	
	numindexes = p;
	printf("%ld\n", p);

	computeNormals(height, width, vertices);

	glGenBuffers(1, &vertex_id);
	glBindBuffer(GL_ARRAY_BUFFER,  vertex_id);
	glBufferData(GL_ARRAY_BUFFER, width * height * sizeof(point_t), vertices, GL_STATIC_DRAW);
	
	glGenBuffers(1, &index_id);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,  index_id);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, width * height * 4 * sizeof(GLuint), indexes, GL_DYNAMIC_DRAW);
}

void myexit(){
	std::cout<<"Cleanup"<<std::endl;

	//cleanup texturi
	for(int i=0;i<4;i++){
		FreeTexture(textura_iarba[i]);
	}
	glDeleteBuffers(1, &vertex_id);
	glDeleteBuffers(1, &index_id);
}


void displayCB()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	// seteaza modul de desenare al poligoanelor
	if(mode == WIRE)
		glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
	if(mode == SOLID)
		glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);

	view.setViewer();

	unsigned int camera_x = (unsigned int)view.getPosition(0);
	unsigned int camera_y = (unsigned int)view.getPosition(1);
	unsigned int camera_z = (unsigned int)view.getPosition(2);

	unsigned int pminx = camera_x - perimeter;
	unsigned int pmaxx = camera_x + perimeter;
	unsigned int pminz = camera_z - perimeter;
	unsigned int pmaxz = camera_z + perimeter;

	glPushMatrix();

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_INDEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);

	glBindBuffer(GL_ARRAY_BUFFER, vertex_id);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_id);
	glBindTexture(GL_TEXTURE_2D,textura_iarba[1]);

	GLuint* ptr = (GLuint*)glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_WRITE_ONLY);
	if(ptr)
	{
		unsigned int p = 0;
		for(unsigned int i = 0; i < height-1; i++) {
			for(unsigned int j = 0; j < width-1; j++) {
				if(pminx <= i && i+1 <= pmaxx && pminz <= j && j+1 <= pmaxz)
					continue;
				ptr[p++] = i*width + j;
				ptr[p++] = i*width + j + 1;
				ptr[p++] = (i+1)*width + j + 1;
				ptr[p++] = (i+1)*width + j;
			}
		}
		numindexes = p;
		glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER); // unmap it after use
	} else {
		printf("%d\n",glGetError());
	}

	glVertexPointer(3, GL_FLOAT, sizeof(point_t), NULL);
	glNormalPointer(GL_FLOAT, sizeof(point_t), (const GLvoid*)(3*sizeof(float)));
	glTexCoordPointer(2, GL_FLOAT, sizeof(point_t), (const GLvoid*)(6*sizeof(float)));
	glColorPointer(4, GL_FLOAT, sizeof(point_t),(const GLvoid*)(8*sizeof(float)));
	glDrawElements(GL_QUADS,numindexes, GL_UNSIGNED_INT, 0 /*index_offset*/);

	//glDrawElements(GL_QUADS, numindexes, GL_UNSIGNED_INT, (const GLvoid *)indexes);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_INDEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);

	// TODO: functia drawDeformed trebuie completata cu adaugarea de culoare si 
	// normala pentru fiecare vertex calculat din ea
	mesh.drawDeformed(pminx,pmaxx+1,pminz,pmaxz+1,4);
	
	glBegin(GL_QUADS);
		glTexCoord2f(0,0);			glVertex3f(0,0,0);
		glTexCoord2f(0,1);			glVertex3f(0,0,1);
		glTexCoord2f(1,1);			glVertex3f(1,0,1);
		glTexCoord2f(1,0);			glVertex3f(1,0,0);
	glEnd();

	//glutWireCube(2.0);
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);

	glutSwapBuffers();
	glutPostRedisplay();
}

int main(int argc, char** argv)
{
		
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA);
	glutInitWindowSize(800,600);
	glutInitWindowPosition(100,100);
	
	mainWindow = glutCreateWindow("Animatie");
	//init();
	if( !mesh.loadImage("half.tga") )
		return 0;

	initVBO();

	for(int i=0;i<4;i++){
		textura_iarba[i]=LoadTextureBMP("texturi/grass.bmp",i);
		printf("TEX %i %i\n",i,textura_iarba[i]);
	}

	glutDisplayFunc(displayCB);
	glutIdleFunc(idle);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	atexit(myexit);

	glutMainLoop();
	return 0;
}