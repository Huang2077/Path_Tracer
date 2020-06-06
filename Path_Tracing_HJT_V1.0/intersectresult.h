#pragma once
#ifndef INTERSECTRESULT_H_INCLUDED
#define INTERSECTRESULT_H_INCLUDED
#include "gvector3.h"
#include "material.h"
struct IntersectResult {
	float distance;
	bool isHit;
	GVector3 position;
	GVector3 normal;
	Material material;
	static inline IntersectResult noHit() { return IntersectResult(); }
};

#endif // INTERSECTRESULT_H_INCLUDED

