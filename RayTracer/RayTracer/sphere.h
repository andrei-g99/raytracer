#ifndef SPHERE_H
#define SPHERE_H


using Math::vec3;

enum material { normal = 0, reflective, metallic };

class sphere
{
private:
	vec3 m_center;
	double m_radius;
	sf::Color m_color;
	material m_material;

public:

	sphere()
	{
		m_center = { 0,0,0 };
		m_radius = 1;
		m_color = sf::Color::Red;
		m_material = normal;
	}

	sphere(const vec3& center,const double radius,const sf::Color color = sf::Color::Red,material mat = normal)
	{
		m_center = center;
		m_radius = radius;
		m_color = color;
		m_material = mat;
	}

	material material() const
	{
		return m_material;
	}

	void move(const vec3& in)
	{
		m_center = m_center + in;
	}

	void set_center(const vec3& center)
	{
		m_center = center;
	}

	vec3 center() const
	{
		return m_center;
	}

	sf::Color color() const
	{
		return m_color;
	}

	double radius()
	{
		return m_radius;
	}


};

#endif
