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

#define DISTANCE	1

#define WIRE	0
#define SOLID	1

// modul initial in care sunt desenate poligoanele
int mode = SOLID;

Viewer view(0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 1.0, 0.0);
int mainWindow;
Mesh mesh;
int mesh_type = 1;
Vector3D spotlight(0.0, 15.0, 0.0);

void init(void)
{

	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);
	glShadeModel(GL_SMOOTH);

		// Light values and coord inates
	GLfloat ambientLight[] = { 0.1f, 0.1f, 0.1f, 0.1f };
	GLfloat diffuseLight[] = { 0.2f, 0.2f, 0.2f, 0.1f };
	GLfloat specular[] = { 1.0f, 1.0f, 1.0f, 1.0f};
	GLfloat lightPos[] = { -50.f, 25.0f, -50.0f, 1.0f };

	// Enable lighting
	glEnable(GL_LIGHTING);
	// Setup and enable light 0
	glLightfv(GL_LIGHT0,GL_AMBIENT,ambientLight);
	glLightfv(GL_LIGHT0,GL_DIFFUSE,diffuseLight);
	glLightfv(GL_LIGHT0,GL_SPECULAR,specular);
	glLightfv(GL_LIGHT0,GL_POSITION,lightPos);
	glEnable(GL_LIGHT0);

	GLfloat light1_ambient[] = { 0.2, 0.2, 0.2, 1.0 };
	GLfloat light1_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat light1_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat light1_position[] = { spotlight.x, spotlight.y, spotlight.z, 1.0 };
	GLfloat spot_direction[] = { 0.0, -1.0, 0.0 };
	glLightfv(GL_LIGHT1, GL_AMBIENT, light1_ambient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light1_diffuse);
	glLightfv(GL_LIGHT1, GL_SPECULAR, light1_specular);
	glLightfv(GL_LIGHT1, GL_POSITION, light1_position);
	glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 45.0);
	glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, spot_direction);
	//glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 2.0);

	glEnable(GL_LIGHT1);

	// Enable color tracking
	glEnable(GL_COLOR_MATERIAL);
	// Set Material properties to follow glColor values
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

	mesh_type = UNDEFORMED_X1;
}


void doLighting() {
	GLfloat light1_position[] = { spotlight.x, spotlight.y, spotlight.z, 1.0 };
	glLightfv(GL_LIGHT1, GL_POSITION, light1_position);
}


// functia pentru desenarea scenei 3D
void drawScene()
{
	// seteaza modul de desenare al poligoanelor
	if(mode == WIRE)
		glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
	if(mode == SOLID)
		glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
	
	switch(mesh_type) {
	case UNDEFORMED_X1:
		glCallList(UNDEFORMED_X1);
		break;
	case UNDEFORMED_X2:
		glCallList(UNDEFORMED_X2);
		break;
	case UNDEFORMED_X4:
		glCallList(UNDEFORMED_X4);
		break;
	case DEFORMED_X1:
		glCallList(DEFORMED_X1);
		break;
	case DEFORMED_X2:
		glCallList(DEFORMED_X2);
		break;
	case DEFORMED_X4:
		glCallList(DEFORMED_X4);
		break;
	}
}

// functia de display
void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	drawScene();

	glutSwapBuffers();
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
			mesh_type = ( (mesh_type-1) + 1) % NO_MESH + 1;
			break;
		case '-':
			mesh_type = ( (mesh_type-1) - 1) % NO_MESH + 1;
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


GLfloat* vertices;
unsigned int width, height;
unsigned int numindexes;
GLuint*  indexes;

GLuint vertex_id, index_id;

void initVBO() {

	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);

	GLenum eroare = glewInit();
	
	if( eroare ) {
		printf("Error\n");
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
	vertices = mesh.getVertices();
	indexes = (GLuint*)malloc(width*height*4*sizeof(GLuint));

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

	glGenBuffers(1, &vertex_id);
	glBindBuffer(GL_ARRAY_BUFFER,  vertex_id);
	glBufferData(GL_ARRAY_BUFFER, width * height * 3 * sizeof(float), vertices, GL_STATIC_DRAW);
	
	glGenBuffers(1, &index_id);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,  index_id);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, width * height * 4 * sizeof(GLuint), indexes, GL_STATIC_DRAW);
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
	
	glPushMatrix();

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_INDEX_ARRAY);

	glBindBuffer(GL_ARRAY_BUFFER, vertex_id);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_id);

	glEnableVertexAttribArray(0);    //We like submitting vertices on stream 0 for no special reason
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);   //The starting point of the VBO, for the vertices
//	glVertexPointer(3, GL_FLOAT, 0, NULL);
	
	glDrawElements(GL_QUADS, numindexes, GL_UNSIGNED_INT, 0 /*index_offset*/);
//	glDrawElements(GL_QUADS, numindexes, GL_UNSIGNED_INT, indexes);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glDisableClientState(GL_INDEX_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);

	glutWireCube(2.0);
	glPopMatrix();

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

	glutDisplayFunc(displayCB);
	glutIdleFunc(idle);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);

	glutMainLoop();
	return 0;
}