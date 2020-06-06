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
//ͨ�������볡�����ཻ���������ս��
bool PointLight::isLighting(Union &scence, IntersectResult &rayResult)
{
	//����shadowRay������ֵ
	const float k = 1e-4;
	GVector3 delta = this->position - rayResult.position;
	float distance = delta.getLength();
	//����������෴�����shadowRay
	CRay shadowRay = CRay(rayResult.position, delta.normalize());
	GVector3 shadowDir = delta.normalize();
	//����shadowRay�Ƿ��볡���ཻ
	IntersectResult lightResult = scence.isIntersected(shadowRay);
	//���shadowRay�볡���е������ཻ
	if (lightResult.isHit && (lightResult.distance <= distance))
		return false;
	else
		return true;
}
