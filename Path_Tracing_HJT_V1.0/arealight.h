#pragma once
#ifndef AREALIGHT
#define AREALIGHT
#include "gvector3.h"
#include "intersectresult.h"
#include "cray.h"
#include "material.h"
#include "cobject.h"
#include "union.h"
#include "plane.h"

class Arealight 
{
public:
	float x0, x1, z0, z1, k;
	GVector3 color;
	Arealight();
	Arealight(float _x0, float _x1, float _z0, float _z1, float _k, GVector3 col)
		: x0(_x0), x1(_x1), z0(_z0), z1(_z1), k(_k), color(col) {};
	virtual ~Arealight();
	bool isLighting(Union &scence,  CRay ray, IntersectResult &rec);
};

#endif 
