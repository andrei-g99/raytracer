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

int main()
{
	//RENDER

	Math::mat3 mat = { {2,4,5},{1,2,3},{0,-1,1} };

	auto t0 = utility::TimerStart();///
	Math::inverse(mat);
	utility::TimerRead(t0);

}