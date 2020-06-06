#include "pointlight.h"

PointLight::PointLight()
{
	//ctor
}

PointLight::~PointLight()
{
	//dtor
}
PointLight::PointLight(GVector3 _color, GVector3 _position)
{
	color = _color;
	position = _position;
}
//通过光线与场景的相交结果计算光照结果
bool PointLight::isLighting(Union &scence, IntersectResult &rayResult)
{
	//生产shadowRay的修正值
	const float k = 1e-4;
	GVector3 delta = this->position - rayResult.position;
	float distance = delta.getLength();
	//生成与光照相反方向的shadowRay
	CRay shadowRay = CRay(rayResult.position, delta.normalize());
	GVector3 shadowDir = delta.normalize();
	//计算shadowRay是否与场景相交
	IntersectResult lightResult = scence.isIntersected(shadowRay);
	//如果shadowRay与场景中的物体相交
	if (lightResult.isHit && (lightResult.distance <= distance))
		return false;
	else
		return true;
}
