#include "gray_code.h"
#include <bit>
#include <cmath>

inline unsigned int EncodeGray(const unsigned int input, const unsigned int n_digit)
{
	return input ^ (input >> 1);
}

inline unsigned int DecodeGray(const unsigned int input, const unsigned int n_digit)
{
	if (input <= 1)
		return input;
	unsigned int top = std::min(static_cast<unsigned int>(32 - std::countl_zero(input)), n_digit);
	unsigned int res = 0;
	for (unsigned int mask = 1 << (top - 1), prev = 0; mask > 0; mask >>= 1)
	{
		prev = (prev >> 1) ^ (mask & input);
		res |= prev;
	}
	return res;
}