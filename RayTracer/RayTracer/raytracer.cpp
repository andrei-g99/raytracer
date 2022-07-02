#include <string>
#include <vector>
#include <memory>
#include <iterator>
#include <fstream>
#include <iostream>
#include "mmath.h"
#include "rutility.h"
#include "camera.h"
#include "SFML/Graphics.hpp"
#include "SFML/Window.hpp"
#include "SFML/System.hpp"
#include "ray.h"
#include "sphere.h"
#include "object_list.h"

using Math::vec3;
using Math::mat3;
using Math::norm;
using sf::Color;
using Math::are_doubles_equal;
using Math::dot;
using Math::normalize;

double distance_point_to_line(ray& r,const vec3& p)
{
	auto a = r.dir();
	auto b = r.center();
	return norm((p-b)-(dot((p - b),a)*a));
}

std::pair<std::shared_ptr<sphere>, double>  hit(ray& r,object_list& list)
{
	std::vector<std::pair<std::shared_ptr<sphere>,double>> hit_spheres;

	for(const auto& s : list.list())
	{
		if (s->center()[2] > 1)
		{

			auto shortest_distance = distance_point_to_line(r, s->center());
			auto temp = norm(s->center());
			auto l = sqrt(temp * temp - shortest_distance * shortest_distance);

			if (shortest_distance <= s->radius())
			{

				//it hit
				auto half_cord = sqrt(s->radius() * s->radius() - shortest_distance * shortest_distance);
				auto t = l - half_cord;
				std::pair<std::shared_ptr<sphere>, double> info = { s,t };
				hit_spheres.push_back(info);

			}
		}

	}

	if(!hit_spheres.empty())
	{
		std::vector<double> tees;
		for(const auto& i : hit_spheres)
		{
			tees.push_back(i.second);
		}
		std::sort(hit_spheres.begin(), hit_spheres.end(), [](const auto& lhs, const auto& rhs) {
			return lhs.second < rhs.second; });
		return { hit_spheres.front().first, hit_spheres.front().second };
	}

	return { nullptr ,-1};
}

bool any_hit(ray& r,object_list& list,const std::shared_ptr<sphere>& exception = nullptr)
{
	//detects only hits in front of the ray (t > 0)
	std::vector<std::pair<std::shared_ptr<sphere>, double>> hit_spheres;
	auto spheres = list.list();

	for (const auto& s : spheres)
	{
		if(s != exception)
		{
				auto shortest_distance = distance_point_to_line(r, s->center());
				if (shortest_distance <= s->radius())
				{
					if(Math::dot(r.dir(),s->center()-r.center()) > 0)
					return true;

				}
		}

	}

	return false;

}

Color shader(const sf::Color& initial_color, const vec3& p, const std::shared_ptr<sphere>& sf,object_list& scene, const camera& cam)
{
	vec3 normal = normalize(p - sf->center());
	double total_light = 0.2;

	for(const auto& i : scene.lights())
	{
		vec3 first_to_source = normalize(i->center() - p);
		ray occlusion_ray = ray(p,i->center());
		if(!any_hit(occlusion_ray,scene,sf))
		{
			double product = dot(normal, first_to_source);
			double intensity = i->intensity();
			if (product > 0)
				total_light += intensity * product;
		}

	}

			//auto subtract = (sf::Uint8)((1 / intensity) * (255 * (1 - product)));
			sf::Uint8 R = (sf::Uint8)(total_light * (double)initial_color.r);
			if (total_light * (double)initial_color.r > 255) R = 255;

			sf::Uint8 G = (sf::Uint8)(total_light * (double)initial_color.g);
			if (total_light * (double)initial_color.g > 255) G = 255;

			sf::Uint8 B = (sf::Uint8)(total_light * (double)initial_color.b);
			if (total_light * (double)initial_color.b > 255) B = 255;

			sf::Color newc = { R,G,B };
			return newc;



}


 Color anti_aliasing(double pixel_l, object_list& list, const camera& cam,const vec3& pixmid,const unsigned int N,const sf::Color& bck,bool shader_on = true){
	 std::vector<Color> colors;
	 double subpixel = pixel_l / N;
	 vec3 left_corner = pixmid + vec3{ 0,pixel_l / 2 - subpixel/2,0 } + vec3{ subpixel/2 - pixel_l / 2 ,0,0 };

	for(int i = 0;i<N;i++)
		for(int j = 0;j<N;j++)
		{
			//construct ray
			vec3 deflected = left_corner + vec3{ i * subpixel,-((int)j) * subpixel,0 };
			auto r = ray(cam.center(), deflected);
			auto ptr = hit(r, list);
			if (ptr.first != nullptr)
			{

					auto shaded_color = ptr.first->color();
					if (shader_on)
						shaded_color = shader(ptr.first->color(), ptr.second * r.dir() + r.center(), ptr.first, list, cam);

					colors.push_back(shaded_color);
			

			}else
			{
				colors.push_back(bck);
			}
		}
	unsigned int Rsum = 0;
	unsigned int Gsum = 0;
	unsigned int Bsum = 0;
	for(const auto& i : colors)
	{
		Rsum += (unsigned int)i.r;
		Gsum += (unsigned int)i.g;
		Bsum += (unsigned int)i.b;
	}

	return {(sf::Uint8)(Rsum/colors.size()),(sf::Uint8)(Gsum / colors.size()),(sf::Uint8)(Bsum / colors.size()) };

 }
 
sf::Image render(object_list& list, const camera& cam,unsigned int width,unsigned int height,const sf::Color& bck, const unsigned int N,bool shader_on = true,bool anti_aliasing_on = true)
{
	auto t0 = utility::TimerStart();///

	double w = 1;
	double h = (double)height / (double)width;
	double pixel_l = w / (double)width;
	//inverse rotate camera orientation : rotate all objects in scene around camera (-THETA)
	auto tx = -cam.orientation()[0];
	auto ty = -cam.orientation()[1];
	auto tz = -cam.orientation()[2];

	mat3 rot_xm{ {1,0,0} ,{0,cos(tx),sin(tx)} ,{0,-sin(tx),cos(tx)} };
	mat3 rot_ym{ {cos(ty),0,-sin(ty)} ,{0,1,0} ,{sin(ty),0,cos(ty)} };
	mat3 rot_zm{ {cos(tz),sin(tz),0} ,{-sin(tz),cos(tz),0} ,{0,0,1} };
	mat3 total = rot_zm * rot_ym * rot_xm;

	for (auto& i : list.list())
	{
		i->set_center(total * i->center());
	}

	//create image and start looping through center of pixels on real screen
	sf::Image output;
	output.create(width, height, bck);

	vec3 left_corner = cam.center() + cam.near() + vec3{0,cam.height()/2 - pixel_l / 2,0} - vec3{ pixel_l+cam.width()/2,0,0};
	//generate ray, check hit, set pixel color
	for(unsigned int i = 0;i<width;i++)
		for(unsigned int j = 0;j<height;j++)
		{
			if(!anti_aliasing_on)
			{
				//construct ray
				auto r = ray(cam.center(), left_corner + vec3{ i * pixel_l,-((int)j) * pixel_l,0 });
				auto ptr = hit(r, list);
				if (ptr.first != nullptr)
				{
					
						auto shaded_color = ptr.first->color();
						if (shader_on)
							shaded_color = shader(ptr.first->color(), ptr.second * r.dir() + r.center(), ptr.first, list, cam);

						output.setPixel(i, j, shaded_color);
					

				}
				r.~ray();
			}else
			{
				//WITH ANTI-ALIASING
				auto pixel_middle = left_corner + vec3{ i * pixel_l,-((int)j) * pixel_l,0 };
				output.setPixel(i, j, anti_aliasing(pixel_l, list, cam, pixel_middle, N, bck, true));

			}

		}

	return output;
}

int main()
{
	
	const int width = 800;
	const int height = 600;
	const unsigned int N = 4;
	double w = 1;
	double h = (double)height / (double)width;
	const sf::Color background_color = sf::Color(135,206,235); //sky blue

	vec3 camera_center = { -3,0,0 };
	vec3 camera_angle = { 0,0.3,0 };
	camera cam = camera(camera_center, camera_angle, {0,0,1}, w, h);
	object_list list = object_list();
	list.push_new_sphere(vec3{ 0,0,5 }, 1);
	list.push_new_sphere(vec3{ -2,0,5 }, 0.25,sf::Color::Green);
	light source = light({-30,-10,5},2);
	list.push_light(source);
	

	//RENDER
	auto t0 = utility::TimerStart();///

	sf::Image img = render(list,cam,width,height,background_color,N,true,true);

	utility::TimerRead(t0);///
	std::string path = "C:\\Users\\bitma\\Desktop\\renders\\raytracer\\two_sources.png";
	img.saveToFile(path);
	

}