#include "hash.h"

#include <array>
#include <bit>
#include <cassert>
#include <cstdint>
#include <functional>
#include <string>

#include "common/parse.h"

std::array<unsigned char, 16> hash_md5(const std::string& path)
{
	const std::array<uint32_t, 64> s = {7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22,
										5, 9,  14, 20, 5, 9,  14, 20, 5, 9,	 14, 20, 5, 9,	14, 20,
										4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23,
										6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21};
	const std::array<uint32_t, 64> k = {
		0xD76AA478, 0xE8C7B756, 0x242070DB, 0xC1BDCEEE, 0xF57C0FAF, 0x4787C62A, 0xA8304613, 0xFD469501,
		0x698098D8, 0x8B44F7AF, 0xFFFF5BB1, 0x895CD7BE, 0x6B901122, 0xFD987193, 0xA679438E, 0x49B40821,
		0xF61E2562, 0xC040B340, 0x265E5A51, 0xE9B6C7AA, 0xD62F105D, 0x02441453, 0xD8A1E681, 0xE7D3FBC8,
		0x21E1CDE6, 0xC33707D6, 0xF4D50D87, 0x455A14ED, 0xA9E3E905, 0xFCEFA3F8, 0x676F02D9, 0x8D2A4C8A,
		0xFFFA3942, 0x8771F681, 0x6D9D6122, 0xFDE5380C, 0xA4BEEA44, 0x4BDECFA9, 0xF6BB4B60, 0xBEBFBC70,
		0x289B7EC6, 0xEAA127FA, 0xD4EF3085, 0x04881D05, 0xD9D4D039, 0xE6DB99E5, 0x1FA27CF8, 0xC4AC5665,
		0xF4292244, 0x432AFF97, 0xAB9423A7, 0xFC93A039, 0x655B59C3, 0x8F0CCC92, 0xFFEFF47D, 0x85845DD1,
		0x6FA87E4F, 0xFE2CE6E0, 0xA3014314, 0x4E0811A1, 0xF7537E82, 0xBD3AF235, 0x2AD7D2BB, 0xEB86D391
	};

	std::array<uint32_t, 4> buf = {0x67452301, 0xEFCDAB89, 0x98BADCFE, 0x10325476};
	auto fn = std::array<std::function<uint32_t(uint32_t, uint32_t, uint32_t)>, 4>{
		[](auto b, auto c, auto d)
		{
			return (b & c) | (~b & d);
		},
		[](auto b, auto c, auto d)
		{
			return (b & d) | (c & ~d);
		},
		[](auto b, auto c, auto d)
		{
			return ((b ^ c) ^ d);
		},
		[](auto b, auto c, auto d)
		{
			return c ^ (b | ~d);
		}
	};

	auto data = load_binary(path);

	const auto size_bit = static_cast<std::uint64_t>(data.size() << 3);

	// Padding

	data.emplace_back(0x80);
	while (data.size() % 64 != 56)
	{
		data.emplace_back(0);
	}

	for (std::int64_t i = 0; i < 8; ++i)
	{
		const auto mask = std::uint64_t{0xff} << (i * 8);
		data.emplace_back((mask & size_bit) >> (i * 8));
	}

	assert(data.size() % 64 == 0);

	for (std::uint64_t i = 0; i < data.size(); i += 64)
	{
		auto tmp = buf;
		// md5の1ラウンドを実行
		for (int itr = 0; itr < 64; ++itr)
		{
			int g;
			if (itr < 16)
				g = itr;
			else if (itr < 32)
				g = (5 * itr + 1) % 16;
			else if (itr < 48)
				g = (3 * itr + 5) % 16;
			else
				g = (7 * itr) % 16;
			auto chunk = std::uint32_t{0};
			for (int j = 0; j < 4; ++j)
				chunk |= data[i + g * 4 + j] << j * 8;

			const auto f = fn[itr / 16](tmp[1], tmp[2], tmp[3]);
			tmp[0] += f;
			tmp[0] += chunk;
			tmp[0] += k[itr];
			tmp[0] = std::rotl(tmp[0], s[itr]);
			tmp[0] += tmp[1];
			std::rotate(tmp.begin(), tmp.end() - 1, tmp.end());
		}
		for (int i = 0; i < 4; ++i)
		{
			buf[i] += tmp[i];
		}
	}

	std::array<unsigned char, 16> res;
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			std::uint64_t mask = std::uint64_t{0xff} << (j * 8);
			res[i * 4 + j] = (mask & buf[i]) >> (j * 8);
		}
	}

	return res;
}