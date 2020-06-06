#include "arealight.h"

Arealight::Arealight()
{
	//ctor
}

Arealight::~Arealight()
{
	//dtor
}


bool Arealight::isLighting(Union &scence,  CRay ray, IntersectResult &rec)
{
	float t = (k - ray.getOrigin().y) / ray.getDirection().y;
	if (t < 0.0 || t > INFINITY)
		return false;
	float x = ray.getOrigin().x + t*ray.getDirection().x;
	float z = ray.getOrigin().z + t*ray.getDirection().z;
	if (x < x0 || x > x1 || z < z0 || z > z1)
		return false;

	rec.distance = t;
	return true;
	
}