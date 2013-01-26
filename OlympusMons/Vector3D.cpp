#include "Vector3D.h"
#include "Math.h"

Vector3D::Vector3D(void) {
	x = 0;
	y = 0;
	z = 0;
}

Vector3D::Vector3D(GLdouble _x, GLdouble _y, GLdouble _z) {
	x = _x;
	y = _y;
	z = _z;
}

Vector3D::Vector3D(const Vector3D& V2) {
	x=V2.x;
	y=V2.y;
	z=V2.z;
}

Vector3D& Vector3D::operator= (const Vector3D& V2) {
	x=V2.x;
	y=V2.y;
	z=V2.z;
	return *this;
}

Vector3D Vector3D::operator+ (const Vector3D& V2) const {
	return Vector3D(x + V2.x, y + V2.y, z + V2.z);
}

Vector3D Vector3D::operator- (const Vector3D& V2) const {
	return Vector3D(x - V2.x, y - V2.y, z - V2.z);
}

Vector3D Vector3D::operator* (const Vector3D& V2) const {
	return Vector3D(x * V2.x, y * V2.y, z * V2.z);
}

Vector3D Vector3D::operator/ (const Vector3D& V2) const {
	return Vector3D(x * V2.x, y * V2.y, z * V2.z);
}

Vector3D Vector3D::operator+ (const GLdouble s)	const {
	return Vector3D(x + s, y + s, z + s);
}

Vector3D Vector3D::operator- (const GLdouble s)	const {
	return Vector3D(x - s, y - s, z - s);
}

Vector3D Vector3D::operator* (const GLdouble s)	const {
	return Vector3D(x * s, y * s, z * s);
}

Vector3D Vector3D::operator/ (const GLdouble s)	const {
	return Vector3D(x / s, y / s, z / s);
}

Vector3D Vector3D::operator- () const {
	return Vector3D(-x, -y, -z);
}

Vector3D Vector3D::CrossProduct( const Vector3D &V2 ) const {
return Vector3D(
	y * V2.z  -  z * V2.y,
	z * V2.x  -  x * V2.z,
	x * V2.y  -  y * V2.x 	);
}

Vector3D Vector3D::Normalize()
{
	GLfloat fMag = ( x*x + y*y + z*z );
	if (fMag == 0) {return Vector3D(0,0,0);}

	GLfloat fMult = 1.0f/sqrtf(fMag);            
	x *= fMult;
	y *= fMult;
	z *= fMult;
	return Vector3D(x,y,z);
}

Vector3D::~Vector3D(void)
{
}
