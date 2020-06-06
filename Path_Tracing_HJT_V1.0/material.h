#pragma once
#ifndef MATERIAL_H
#define MATERIAL_H
#include "gvector3.h"
#include <cmath>
using namespace std;
#define PI 3.14159
class Material
{
public:
	Material();
	Material(float _metallic,float _specular,float _roughness,GVector3 _basecolor);//金属属性、菲涅尔F0,粗糙度，本色
	~Material();
	GVector3 BRDF(GVector3 V, GVector3 L, GVector3 N);
	float D_GGX_TR(GVector3 N, GVector3 H);//法线分布函数
	float GeometrySchlickGGX(float NdotV);
	float GeometrySmith(GVector3 N, GVector3 V, GVector3 L);//几何遮罩函数
	GVector3 fresnelSchlick(float cosTheta, GVector3 F0);//菲涅尔反射率


private:
	float metallic;//金属率
	float specular;//F0
	float roughness;//粗糙度
	GVector3 basecolor;//本色
};

#endif // !1

