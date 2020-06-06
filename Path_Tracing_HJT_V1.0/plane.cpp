#include "plane.h"

Plane::Plane()
{
	//ctor
}

Plane::~Plane()
{
	//dtor
}

Plane::Plane(const GVector3& _normal, float _d, const Material& _material)
{
	normal = _normal;
	d = _d;
	material = _material;
}
IntersectResult Plane::isIntersected(CRay ray)
{
	IntersectResult result = IntersectResult::noHit();
	
	float a = ray.getDirection().dotMul(this->normal);
	if (a <0)
	{
		float b = this->normal.dotMul(ray.getOrigin() - normal * d);
		if ((-b / a) > 0)
		{
			result.isHit = 1;
			result.distance = -b / a;
			result.position = ray.getPoint(result.distance);
			result.normal = this->normal;
			result.material = material;
		}
		else result.isHit = 0;
	}
	return result;
}
