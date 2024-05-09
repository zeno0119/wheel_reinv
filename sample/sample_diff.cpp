#include "str_opr.h"
#include <fstream>
#include <iostream>
#include <string>

int main(int argc, char** argv)
{
	if (argc != 3)
		return -1;
	std::ifstream ifs(argv[1]);
	if (ifs.fail())
		return -1;
	auto orig_str = std::string(std::istreambuf_iterator<char>(ifs), std::istreambuf_iterator<char>());

	std::ifstream ifs2(argv[2]);
	if (ifs2.fail())
		return -1;

	auto new_str = std::string(std::istreambuf_iterator<char>(ifs2), std::istreambuf_iterator<char>());

	const auto s = diff(orig_str, new_str);
	std::cout << s.size() << "\n";
	for (const auto& el : s)
	{
		std::cout << el.orig_begin;
		if (el.orig_begin != el.orig_end)
			std::cout << "," << el.orig_end;
		if (el.state == diff_state::added)
			std::cout << "a";
		else if (el.state == diff_state::changed)
			std::cout << "c";
		else if (el.state == diff_state::deleted)
			std::cout << "d";
		std::cout << el.new_begin;
		if (el.new_begin != el.new_end)
			std::cout << ", " << el.new_end;
		std::cout << "\n";
		if (el.state == diff_state::changed || el.state == diff_state::deleted)
			std::cout << "\e[31m";
		for (const auto& str : el.orig_str)
			std::cout << "< " << str << "\n";
		if (el.state == diff_state::changed)
			std::cout << "\e[0m---\n";
		if (el.state == diff_state::changed || el.state == diff_state::added)
			std::cout << "\e[32m";
		for (const auto& str : el.new_str)
			std::cout << "> " << str << "\n";
		std::cout << "\e[0m";
	}
	return 0;
}
