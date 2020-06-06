#pragma once
#ifndef ONB
#define ONB	
#define _USE_MATH_DEFINES
#include "gvector3.h"
#include <cstdlib>
#include <math.h>

//sampling function
inline double random_double() {
	return rand() / (RAND_MAX + 1.0);
}

inline GVector3 random_cosine_direction() {
	float r1 = random_double();
	float r2 = random_double();
	float z = sqrt(1 - r2);
	float phi = 2 * M_PI*r1;
	float x = cos(phi)*sqrt(r2);
	float y = sin(phi)*sqrt(r2);
	return GVector3(x, y, z);
}
class onb
{
public:
	onb() {}
	inline GVector3 operator[](int i) const { return axis[i]; }
	GVector3 u() const { return axis[0]; }
	GVector3 v() const { return axis[1]; }
	GVector3 w() const { return axis[2]; }
	GVector3 local(float a, float b, float c) const { return u()* a + v()*b + w() * c; }
	GVector3 local(const GVector3& a) const { return  u()*a.x  +   v()*a.y +  w() * a.z; }
	void build_from_w( GVector3& n);
	GVector3 axis[3];
};


void onb::build_from_w( GVector3& n) {
	axis[2] = n.normalize();
	GVector3 a;
	if (fabs(w().x) > 0.9)
		a = GVector3(0, 1, 0);
	else
		a = GVector3(1, 0, 0);
	axis[1] =  w().crossMul(a).normalize();
	axis[0] = w().crossMul(v());
}

#endif
