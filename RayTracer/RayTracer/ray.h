#ifndef RAY_H
#define RAY_H


using Math::vec3;

class ray
{
private:
	vec3 m_direction;
	vec3 m_center;
public:

	ray(const vec3& first, const vec3& second)
	{
		m_center = first;
		m_direction = Math::normalize(second - first);
	}

	vec3 at(const double t) const
	{
		const vec3 output = m_center + t * m_direction;
		return output;
	}

	vec3 dir(){
		return m_direction;
}
	vec3 center() {
		return m_center;
	}

};

#endif