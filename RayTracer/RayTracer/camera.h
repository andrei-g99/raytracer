#ifndef CAMERA_H
#define CAMERA_H

using Math::vec3;

class camera
{
private:
	vec3 m_center;
	vec3 m_orientation;
	vec3 m_near; // always {0,0,1}
	double m_width;
	double m_height;

	friend class world;
public:
	camera(const vec3& center, const vec3& orientation, const vec3& near, const double width, const double height)
	{
		m_center = center;
		m_orientation = orientation;
		m_near = near;
		m_width = width;
		m_height = height;
	}

	vec3 orientation() const
	{
		return m_orientation;
	}

	vec3 center() const
	{
		return m_center;
	}

	vec3 near() const
	{
		return m_near;
	}

	double width() const
	{
		return m_width;
	}

	double height() const
	{
		return m_height;
	}

	void rotate_x(const double a_x)
	{
		m_orientation.set_x(m_orientation[0] + a_x);
	}
	void rotate_y(const double a_y)
	{
		m_orientation.set_y(m_orientation[1] + a_y);
	}
	void rotate_z(const double a_z)
	{
		m_orientation.set_z(m_orientation[2] + a_z);
	}


	void move_x(const double x)
	{
		m_center.set_x(m_center[0] + x);
	}
	void move_y(const double y)
	{
		m_center.set_y(m_center[1] + y);
	}
	void move_z(const double z)
	{
		m_center.set_z(m_center[2] + z);
	}

};







#endif