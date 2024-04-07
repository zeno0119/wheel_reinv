#include <iostream>

#include "matrix.h"

int main(int argc, char** argv)
{
	Matrix<float> mat(3, 3);
	mat(0, 0) = 1;
	std::cout << mat(0, 0) << std::endl;

	Matrix<float> mat2(2, 2);
	mat2 = {11, 2, 3, 4};
	mat = mat2;
	std::cout << mat(0, 1) << std::endl;
}