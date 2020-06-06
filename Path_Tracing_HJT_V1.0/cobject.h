#pragma once
#ifndef COBJECT
#define COBJECT
#include "cray.h"
#include "intersectresult.h"
class CObject
{
public:
	virtual  IntersectResult isIntersected(CRay RAY) = 0;

};

#endif // !COBJECT

