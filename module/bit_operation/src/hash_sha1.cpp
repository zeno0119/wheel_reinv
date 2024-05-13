#include "hash.h"

#include <bit>
#include <cassert>
#include <cstdint>
#include <functional>

#include "common/parse.h"

std::array<unsigned char, 20> hash_sha1(const std::string& path)
{
	const std::array<std::uint32_t, 4> k = {0x5A827999, 0x6ED9EBA1, 0x8F1BBCDC, 0xCA62C1D6};

	const auto fn = std::array<std::function<std::uint32_t(std::uint32_t, std::uint32_t, std::uint32_t)>, 4>{
		[](auto b, auto c, auto d)
		{
			return (b & c) | (~b & d);
		},
		[](auto b, auto c, auto d)
		{
			return b ^ c ^ d;
		},
		[](auto b, auto c, auto d)
		{
			return (b & c) | (b & d) | (c & d);
		},
		[](auto b, auto c, auto d)
		{
			return b ^ c ^ d;
		}
	};

	auto data = load_binary(path);

	const auto size_bit = static_cast<uint64_t>(data.size() * 8);

	// Padding
	data.emplace_back(0x80);
	while (data.size() % 64 != 56)
	{
		data.emplace_back(0);
	}

	for (std::int64_t i = 0; i < 8; ++i)
	{
		const auto mask = std::uint64_t{0xff} << ((7 - i) * 8);
		data.emplace_back((mask & size_bit) >> ((7 - i) * 8));
	}

	assert(data.size() % 64 == 0);

	auto buf = std::array<std::uint32_t, 5>{0x67452301, 0xEFCDAB89, 0x98BADCFE, 0x10325476, 0xC3D2E1F0};

	for (std::uint64_t i = 0; i < data.size(); i += 64)
	{
		auto tmp = buf;
		std::vector<std::uint32_t> w(80, 0);
		for (std::uint64_t itr = 0; itr < 80; itr++)
		{
			if (itr < 16)
			{
				std::uint32_t t = 0;
				for (std::uint64_t j = 0; j < 4; ++j)
				{
					t |= data[i + itr * 4 + 3 - j] << j * 8;
				}
				w[itr] = t;
			}
			else
			{
				w[itr] = std::rotl(w[itr - 3] ^ w[itr - 8] ^ w[itr - 14] ^ w[itr - 16], 1);
			}
		}

		for (std::uint64_t itr = 0; itr < 80; itr++)
		{
			std::uint32_t f = fn[itr / 20](tmp[1], tmp[2], tmp[3]);
			std::uint32_t m = w[itr];

			const auto t = std::rotl(tmp[0], 5) + f + m + k[itr / 20] + tmp[4];
			tmp[4] = tmp[3];
			tmp[3] = tmp[2];
			tmp[2] = std::rotl(tmp[1], 30);
			tmp[1] = tmp[0];
			tmp[0] = t;
		}
		for (std::uint64_t i = 0; i < 5; ++i)
		{
			buf[i] += tmp[i];
		}
	}
	std::array<unsigned char, 20> res;

	for (std::uint64_t i = 0; i < 5; ++i)
	{
		for (std::uint64_t j = 0; j < 4; ++j)
		{
			std::uint64_t mask = std::uint64_t{0xff} << (j * 8);
			res[i * 4 + 3 - j] = (mask & buf[i]) >> (j * 8);
		}
	}

	return res;
}