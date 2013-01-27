#include <stdlib.h>
#include "glew\glew.h"
#include "glut.h"
#include <iostream>

#define TEXTURA_FILTRARE_NEAREST 0
#define TEXTURA_FILTRARE_BILINEAR 1
#define TEXTURA_FILTRARE_TRILINEAR 2
#define TEXTURA_FILTRARE_TRILINEAR_ANISOTROPIC 3

GLuint LoadTextureBMP( const char *filename, int optiune_filtrare);
void FreeTexture( GLuint texture );
void CreeazaTexturaEcran(GLuint *texture_id, int width, int height);