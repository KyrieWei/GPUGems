#include "Maths.h"


template<typename T>
T RandT(T _min, T _max)
{
	T temp;
	if (_min > _max)
	{
		temp = _max;
		_max = _min;
		_min = temp;
	}

	return rand() / (T)RAND_MAX * (_max - _min) + _min;
}

std::vector<float> random_dir()
{
	float u = RandT<float>(0, RAND_MAX);
	float cose = std::cos(u);
	float sine = std::sin(u);
	std::vector<float> res = { cose, sine };
	return res;
}

std::vector<float> random_dir(float theta_min, float theta_max)
{
	float u = RandT<float>(theta_min, theta_max);
	float cose = std::cos(u);
	float sine = std::sin(u);
	std::vector<float> res = { cose, sine };
	return res;
}