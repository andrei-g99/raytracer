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
		auto shortest_distance = distance_point_to_line(r, s->center());
		auto temp = norm(s->center());
		auto l = sqrt(temp * temp - shortest_distance*shortest_distance);
		
		if(shortest_distance <=s->radius())
		{
			//it hit
			auto half_cord = sqrt(s->radius() * s->radius() - shortest_distance * shortest_distance);
			auto t = l - half_cord;
			std::pair<std::shared_ptr<sphere>, double> info = {s,t};
			hit_spheres.push_back(info);

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

sf::Image render(object_list& list, const camera& cam,unsigned int width,unsigned int height,sf::Color bck)
{
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
			//construct ray
			auto r = ray(cam.center(),left_corner + vec3{i*pixel_l,-((int)j)*pixel_l,0});
			auto ptr = hit(r, list);
			if (ptr.first != nullptr)
			{
				output.setPixel(i, height-j-1, ptr.first->color());
			}
			r.~ray();
		}

	return output;
}

int main()
{
	const int width = 600;
	const int height = 400;
	double w = 1;
	double h = (double)height / (double)width;
	//const sf::Color background_color = sf::Color(135,206,235); //sky blue
	const sf::Color background_color = sf::Color(118, 146, 151);
	vec3 camera_center = { 0,0,0 };
	vec3 camera_angle = { 0,0,0 };
	camera cam = camera(camera_center, camera_angle, {0,0,1}, w, h);
	object_list list = object_list();
	list.push_new_sphere(vec3{ 0,0,-7 }, 1);

	sf::Image img = render(list,cam,width,height,background_color);

	std::string path = "C:\\Users\\bitma\\Desktop\\renders\\raytracer.png";
	img.saveToFile(path);

}