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
	Material(float _metallic,float _specular,float _roughness,GVector3 _basecolor);//�������ԡ�������F0,�ֲڶȣ���ɫ
	~Material();
	GVector3 BRDF(GVector3 V, GVector3 L, GVector3 N);
	float D_GGX_TR(GVector3 N, GVector3 H);//���߷ֲ�����
	float GeometrySchlickGGX(float NdotV);
	float GeometrySmith(GVector3 N, GVector3 V, GVector3 L);//�������ֺ���
	GVector3 fresnelSchlick(float cosTheta, GVector3 F0);//������������


private:
	float metallic;//������
	float specular;//F0
	float roughness;//�ֲڶ�
	GVector3 basecolor;//��ɫ
};

#endif // !1

