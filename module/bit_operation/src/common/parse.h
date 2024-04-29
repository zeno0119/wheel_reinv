#include <fstream>
#include <string>
#include <vector>

std::vector<unsigned char> load_binary(const std::string& path)
{
	std::ifstream ifs(path, std::ios::binary);

	ifs.seekg(0, ifs.end);
	const auto size = ifs.tellg();
	ifs.seekg(0);

	std::vector<char> buf(size);
	std::vector<unsigned char> res(size);

	ifs.read(buf.data(), size);
	for (auto i = 0; i < size; ++i)
	{
		res[i] = static_cast<unsigned char>(buf[i]);
	}

	return res;
}