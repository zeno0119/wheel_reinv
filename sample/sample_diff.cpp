#include "str_opr.h"
#include <iostream>
#include <string>

int main()
{
	const std::string orig = R"(this is test
    and i am sleepy)";
	const std::string new_str = R"(this is test
    and i am hungry)";

	const auto s = diff(orig, new_str);
	for (const auto& [line, del, str] : s)
	{
		std::cout << "L: " << line;
		if (del)
			std::cout << "-";
		else
			std::cout << "+";
		std::cout << str << "\n";
	}
}
