#include "gtest/gtest.h"
#include <stdexcept>

#include "matrix.h"

TEST(test_matrix, initialize)
{
	Matrix<int> mat1(3, 3);
	EXPECT_EQ(0, mat1(0, 0));

	mat1 = {1, 2, 3, 4, 5, 6, 7, 8, 9};
	std::vector<int> v = {1, 2, 3, 4, 5, 6, 7, 8, 9};
	for (size_t i = 0; i < 3; ++i)
	{
		for (size_t j = 0; j < 3; ++j)
		{
			EXPECT_EQ(v[i * 3 + j], mat1(i, j));
		}
	}
}

TEST(test_matrix, range_error)
{
	Matrix<float> mat2(3, 3);
	EXPECT_THROW(mat2(3, 4), std::runtime_error);
}