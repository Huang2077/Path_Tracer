#pragma once
#ifndef POINTLIGHT_H
#define POINTLIGHT_H
#include "gvector3.h"
#include "union.h"

class PointLight
{
public:
	PointLight();
	PointLight(GVector3 _color, GVector3 _position);
	virtual ~PointLight();
	bool isLighting(Union &scence, IntersectResult &result);
	GVector3 color;
	GVector3 position;
};

#endif // POINTLIGHT_H

