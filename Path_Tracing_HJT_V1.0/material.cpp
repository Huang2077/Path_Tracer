#include "material.h"

Material::Material()
{

}
Material::Material(float _metallic, float _specular, float _roughness, GVector3 _basecolor)
{
	metallic = _metallic;
	specular = _specular;
	roughness = _roughness;
	basecolor = _basecolor;

}
Material::~Material()
{

}
float Material::D_GGX_TR(GVector3 N, GVector3 H)//微面元法线分布函数NDF-使用GGX分布
{
	float a = roughness * roughness;
	float a2 = a * a;
	float NdotH = MAX(N.dotMul(H), 0.0);
	float NdotH2 = NdotH * NdotH;

	float nom = a2;
	float denom = (NdotH2 * (a2 - 1.0) + 1.0);
	denom = PI * denom * denom ;

	return (nom / denom);
}
float Material::GeometrySchlickGGX(float NdotV)//单向阴影遮挡函数，使用Schlick近似模型
{
	float r = (roughness + 1.0);
	float K = (r*r) / 8.0;

	float nom = NdotV;
	float denom = NdotV * (1.0 - K) + K;

	return nom / denom;
}
float Material::GeometrySmith(GVector3 N, GVector3 V, GVector3 L)//双向阴影遮挡函数
{
	float NdotV = MAX(N.dotMul(V), 0.0);
	float NdotL = MAX(N.dotMul(L), 0.0);
	float ggx1 = GeometrySchlickGGX(NdotV);
	float ggx2 = GeometrySchlickGGX(NdotL);

	return ggx1 * ggx2;
}
GVector3 Material::fresnelSchlick(float cosTheta, GVector3 F0)//菲涅尔公式
{
	return F0 + (GVector3(1.0, 1.0, 1.0) - F0) * pow(1.0 - cosTheta, 5.0);
}
GVector3 Material::BRDF(GVector3 V, GVector3 L, GVector3 N)
{
	//
	GVector3 F0 = GVector3(specular, specular, specular);
	GVector3 H = (V + L).normalize();
	float HdotV = MAX(H.dotMul(V), 0.0);
	float NdotV = MAX(N.dotMul(V), 0.0);
	float NdotL = MAX(N.dotMul(L), 0.0)
	//菲涅尔反射率
	F0.x = (1.0 - metallic) *F0.x + metallic * basecolor.x;
	F0.y = (1.0 - metallic) *F0.y + metallic * basecolor.y;
	F0.z = (1.0 - metallic) *F0.z + metallic * basecolor.z;
	GVector3 F = fresnelSchlick(HdotV,F0);
	//法线分布函数
	float NDF = D_GGX_TR(N, H);
	//几何遮罩函数
	float G = GeometrySmith(N, V, L);
	//BRDF函数
	//镜面反射部分
	GVector3 nominator = F * NDF * G ;
	float denominator = 4.0 * NdotV * NdotL + 0.001;
	GVector3 specular = nominator / denominator;
	//根据菲涅尔反射率计算漫反射与镜面反射的比例
	GVector3 kS = F;
	GVector3 kD = GVector3(1.0,1.0,1.0) - kS;
	//根据金属属性插值计算漫反射贡献值
	kD =kD *( 1.0 - metallic);
	GVector3 BRDF = ( basecolor * kD  / PI + specular) * NdotL;
	return BRDF;
}