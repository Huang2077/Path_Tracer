#pragma once
#ifndef Plane_H
#define Plane_H
#include "gvector3.h"
#include "intersectresult.h"
#include "cray.h"
#include "material.h"
#include "cobject.h"
class Plane : public CObject
{
public:
	Plane();
	Plane(const GVector3& _normal, float _d, const Material& _material);
	virtual ~Plane();
	GVector3 getNormal() {return normal;};
	Material getMaterial() { return material; };
	virtual  IntersectResult isIntersected(CRay RAY);
protected:
private:
	//������
	GVector3 normal;
	//��ԭ��ľ���
	float d;
	Material material;
};

#endif // Plane_H

