
#include "gvector3.h"
#include "intersectresult.h"
#include "cray.h"
#include "csphere.h"
#include "plane.h"
#include "perspectiveCamera.h"
#include "material.h"
#include "union.h"
#include "pointlight.h"
#include "arealight.h"
#include "onb.h"
#include <cmath>
#include <fstream>
#include <iostream>


#define WINDOW_WIDTH  200
#define WINDOW_HEIGHT 200
#define SAMPLE_NUMBER 1 //抗锯齿采样数
#define MC_SAMPLE 128  //MC采样数
#define BOUNCE_TIME 2 //反弹次数
#define Threshold 0.0 //贡献阈值
#define Russian 0.01 //俄罗斯轮盘

Union scence;
Arealight arealight;
using namespace std;

GVector3 Integrator(CRay ray, int bt, GVector3 contribution);



int main(void)
{
	//init
	//camera
	perspectiveCamera camera(GVector3(0, 6, 25), GVector3(0, 0, -1), GVector3(0, 1, 0), 45);

	//scene
	Material m_sphere1(0.01, 0.04, 0.5, GVector3(1.0, 1.0, 1.0));
	CSphere* sphere1 = new CSphere(GVector3(-2, 3, 5), 3.0, m_sphere1);
	CSphere* sphere2 = new CSphere(GVector3(3, 2, 8), 2.0, m_sphere1);

	Material m_plane_green(0.0, 0.06, 0.9, GVector3(0, 1.0, 0));
	Material m_plane_red(0.0, 0.06, 0.9, GVector3(1.0, 0, 0));
	Material m_plane_white(0.0, 0.06, 0.9, GVector3(1.0, 1.0 , 1.0));

	Plane* plane1 = new Plane(GVector3(0, 1, 0), 0, m_plane_white);//ground
	Plane* plane2 = new Plane(GVector3(1, 0, 0), -6, m_plane_green);//left
	Plane* plane3 = new Plane(GVector3(0, 0, 1), 0, m_plane_white);//back
	Plane* plane4 = new Plane(GVector3(-1, 0, 0), -6, m_plane_red);//right
	Plane* plane5 = new Plane(GVector3(0, -1, 0), -12, m_plane_white);//up
	

	scence.push(sphere1);
	scence.push(sphere2);
	scence.push(plane1);
	scence.push(plane2);
	scence.push(plane3);
	scence.push(plane4);
	scence.push(plane5);

	//lights
	Arealight area(-2,2,5.5,9.5,11.99,GVector3(15.0, 15.0, 15.0));
	arealight = area;

	ofstream OutImage;
	OutImage.open("Image.ppm", ios::out);

	OutImage << "P3\n" << WINDOW_WIDTH << " " << WINDOW_HEIGHT << "\n255\n";

	float dx = 1.0f / WINDOW_WIDTH;
	float dy = 1.0f / WINDOW_HEIGHT;

	//start!
	for (long y = 0; y < WINDOW_HEIGHT; ++y)
	{
		
		for (long x = 0; x < WINDOW_WIDTH; ++x)
		{
			
			GVector3 Color(0.0, 0.0, 0.0);
			//sampling
			for (int s = 0; s < SAMPLE_NUMBER; s++) 
			{
				//culculate each ray color
				GVector3 color(0.0, 0.0, 0.0);
				//creat ray
				float sy = 1 - dy * (y + random_double());
				float sx = dx * (x + random_double());
				CRay ray(camera.generateRay(sx, sy));
				color = Integrator(ray, 0, GVector3(1.0,1.0,1.0));
				//gama
				color = color / (color + 1);
				color = GVector3(pow(color.x, 1.0 / 2.2), pow(color.y, 1.0 / 2.2), pow(color.z, 1.0 / 2.2)) * 255;
				Color = Color + color;
			}
			Color = Color / float(SAMPLE_NUMBER);
			OutImage << Color.x << " " << Color.y << " " << Color.z << "\n";
		} 
	}
	OutImage.close();
	return 0;
	
}

GVector3 Integrator(CRay ray,int bt,GVector3 contribution)
{
	GVector3 kill(0.0, 0.0, 0.0);
	IntersectResult scene_result = scence.isIntersected(ray);
	IntersectResult lightresult;
	
	//击中光源直接停止
	if (arealight.isLighting(scence, ray, lightresult))
	{
		if ((!scene_result.isHit) || (scene_result.isHit && scene_result.distance > lightresult.distance))
			return  arealight.color;
	}
	//没击中场景也没击中光直接停止
	if (!scene_result.isHit)
		return kill;
	//没击中光源，击中场景递归
	if(bt < BOUNCE_TIME )
	{
		
		if (contribution.x > Threshold && contribution.y > Threshold && contribution.z > Threshold)
		{
			bt++;//递归次数+1
			//随机采样
			GVector3 integration(0.0, 0.0, 0.0);
			for (int MC_S = 0; MC_S < MC_SAMPLE; MC_S++)
			{
				//distibution function
				onb uvw;
				uvw.build_from_w(scene_result.normal);
				GVector3 out_direction = uvw.local(random_cosine_direction());
				out_direction = out_direction.normalize();
				//creat one of new on ray
				CRay oray(scene_result.position, out_direction);
				//BRDF
				GVector3 _in_direction(-ray.getDirection().x, -ray.getDirection().y, -ray.getDirection().z);
				_in_direction = _in_direction.normalize();
				//总贡献值
				//contribution = contribution * scene_result.material.BRDF(_in_direction, out_direction, scene_result.normal);
				//渲染方程
				integration = integration + scene_result.material.BRDF(_in_direction, out_direction, scene_result.normal) * Integrator(oray, bt, contribution);
			}

			integration = integration * (2 * M_PI) / MC_SAMPLE;
			return integration;
		}
		else //Russian Roulette
		{
			if (random_double() > Russian) //贡献低的存活
			{
				bt++;//递归次数+1
					 //随机采样
				GVector3 integration(0.0, 0.0, 0.0);
				for (int MC_S = 0; MC_S < MC_SAMPLE; MC_S++)
				{
					//distibution function
					onb uvw;
					uvw.build_from_w(scene_result.normal);
					GVector3 out_direction = uvw.local(random_cosine_direction());
					out_direction = out_direction.normalize();
					//creat one of new on ray
					CRay oray(scene_result.position, out_direction);
					//BRDF
					GVector3 _in_direction(-ray.getDirection().x, -ray.getDirection().y, -ray.getDirection().z);
					_in_direction = _in_direction.normalize();
					//总贡献值
					contribution = contribution * scene_result.material.BRDF(_in_direction, out_direction, scene_result.normal);
					//渲染方程
					integration = integration + scene_result.material.BRDF(_in_direction, out_direction, scene_result.normal) * Integrator(oray, bt, contribution);
				}

				integration = integration * (2 * M_PI) / MC_SAMPLE;
				return (integration / (1 - Russian));
			}
			else
				return kill;	
		}
	}
	else return kill;
}