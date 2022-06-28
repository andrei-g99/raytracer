#ifndef RUTILITY_H
#define RUTILITY_H

#include <chrono>


using Math::vec3;
namespace utility
{


	std::pair<double, double> ScreenToProjSpace(double x, double y, unsigned int width, unsigned int height, double w, double h)
	{
		return { (x / (double)width) - (w / 2),(y / (double)height) - (h / 2) };
	}


	//DEBUG


	std::pair<unsigned int, unsigned int> CoordsToSFML(const std::pair<unsigned int, unsigned int>& in, unsigned int height, unsigned int width)
	{
		//2D natural coordinates to SFML coordinates
		return { width - in.first,in.second };
	}

	vec3 vecCoordsToSFML(vec3 in, unsigned int height)
	{//2D natural coordinates to SFML coordinates
		return { in[0],(double)(height)-in[1],in[2] };
	}

	inline void print_vector(vec3 in)
	{
		std::cout << "________VECTOR________\n";
		std::cout << "x = " << in[0] << std::endl;
		std::cout << "y = " << in[1] << std::endl;
		std::cout << "z = " << in[2] << std::endl;
		std::cout << "______________________\n";
	}

	inline void print_matrix(const Math::mat3& in)
	{
		std::cout << "________MATRIX________\n";
		std::cout << in[0][0] << "|" << in[1][0] << "|" << in[2][0] << "|" << std::endl;
		std::cout << in[0][1] << "|" << in[1][1] << "|" << in[2][1] << "|" << std::endl;
		std::cout << in[0][2] << "|" << in[1][2] << "|" << in[2][2] << "|" << std::endl;
		std::cout << "______________________\n";
	}

	//timing

	std::chrono::time_point<std::chrono::steady_clock> TimerStart() {
		auto const t0 = std::chrono::high_resolution_clock::now();
		return t0;
	}

	void TimerRead(const std::chrono::time_point<std::chrono::steady_clock>& t0) {
		auto const t1 = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double> duration = t1 - t0;
		std::cout << "implementation duration: " << duration.count() << "s\n";
	}

}

#endif
