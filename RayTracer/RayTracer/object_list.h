#ifndef OBJECT_LIST_H
#define OBJECT_LIST_H
/*
#include "mmath.h"
#include "SFML/Graphics.hpp"
#include <vector>
#include "sphere.h"
#include <iterator>
#include <memory>
*/
#include "light.h"
class object_list
{
private:
	std::vector<std::shared_ptr<sphere>> m_list;
	std::vector<std::shared_ptr<light>> m_lights;
public:

	object_list() = default;

	object_list(const std::vector<sphere>& vin,const std::vector<light> lin)
	{
		for(const auto& i : vin)
		{
			m_list.push_back(std::make_shared<sphere>(i));
		}

		for (const auto& i : lin)
		{
			m_lights.push_back(std::make_shared<light>(i));
		}
		
	}

	void push_new_sphere(const vec3& center,const double radius,const sf::Color color = sf::Color::Red)
	{
		m_list.push_back(std::make_shared<sphere>(sphere(center, radius, color)));
	}

	void push_light(const light& in)
	{
		m_lights.push_back(std::make_shared<light>(in));
	}


	std::shared_ptr<sphere> operator[](const unsigned int index) const
	{
		if (index > m_list.size())
		{
			std::cout << "\nIndex out of bound in object_list access []\n";
			exit(-1);
		}else
		{
			return m_list[index];
		}
	}

	std::vector<std::shared_ptr<sphere>> list()
	{
		return m_list;
	}

	std::vector<std::shared_ptr<light>> lights()
	{
		return m_lights;
	}
};


#endif