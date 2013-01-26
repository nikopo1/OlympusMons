#pragma once
#ifndef _VECTOR_3D
#define _VECTOR_3D
#include "glut.h"

class Vector3D
{
public:
	GLdouble x;
	GLdouble y;
	GLdouble z;

	Vector3D();
	Vector3D(const Vector3D& V2);
	Vector3D(GLdouble x, GLdouble y, GLdouble z);

	Vector3D operator+ (const Vector3D& V2) const;
	Vector3D operator- (const Vector3D& V2) const;
	Vector3D operator* (const Vector3D& V2) const;
	Vector3D operator/ (const Vector3D& V2) const;

	Vector3D& operator= (const Vector3D& V2);

	Vector3D operator+ (const GLdouble s)	const;
	Vector3D operator- (const GLdouble s)	const;
	Vector3D operator* (const GLdouble s)	const;
	Vector3D operator/ (const GLdouble s)	const;

	Vector3D operator- ( ) const;

	Vector3D CrossProduct( const Vector3D &V2 ) const;
	Vector3D Normalize();

	~Vector3D(void);
};

#endif
