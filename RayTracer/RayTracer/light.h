#ifndef LIGHT_SOURCE_H
#define LIGHT_SOURCE_H

#include "mmath.h"
using Math::vec3;

class light
{
private:
	double m_intensity;
	bool m_is_on;
	vec3 m_center;

public:

	light()
	{
		m_intensity = 1;
		m_center = { 0,0,0 };
		m_is_on = true;
	}

	light(const vec3& center, const double intensity)
	{

		if ((intensity <= 1) && (intensity >= 0))
		{
			m_intensity = intensity;
		}
		else if (intensity > 1)
		{
			m_intensity = 1;
		}
		else if (intensity < 0)
		{
			m_intensity = 0;
		}
		else
		{
			m_intensity = 1;
		}

		m_center = center;
		m_is_on = true;
	}

	void toggle()
	{
		m_is_on = !m_is_on;
	}

	vec3 center() const
	{
		return m_center;
	}

	double intensity() const
	{
		return m_intensity;
	}

};




#endif