#include <array>
#include <string>

std::array<unsigned char, 16> hash_md5(const std::string& path);
std::array<unsigned char, 20> hash_sha1(const std::string& path);
