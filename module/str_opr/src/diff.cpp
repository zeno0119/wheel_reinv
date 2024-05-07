#include "str_opr.h"

#include <sstream>
#include <stack>

#include <iostream>

std::vector<std::tuple<std::uint64_t, bool, std::string>> diff(const std::string& orig_str, const std::string& new_str)
{
	std::vector<std::tuple<std::uint64_t, bool, std::string>> res;
	std::vector<std::string> orig_v, new_v;
	{
		std::stringstream ss(orig_str);
		std::string s;
		while (std::getline(ss, s))
		{
			orig_v.emplace_back(s);
		}
	}
	{
		std::stringstream ss(new_str);
		std::string s;
		while (std::getline(ss, s))
		{
			new_v.emplace_back(s);
		}
	}

	const auto n = orig_v.size() + 1;
	const auto m = new_v.size() + 1;

	std::vector<std::pair<std::uint64_t, std::pair<std::uint64_t, std::uint64_t>>> cost_matrix(n * m, {0, {0, 0}});
	for (auto i = std::uint64_t{1}; i < n; ++i)
	{
		cost_matrix[i] = {i, {i - 1, 0}};
	}

	for (auto j = std::uint64_t{1}; j < m; ++j)
	{
		cost_matrix[j * n] = {j, {0, j - 1}};
	}

	// orig -> newへの編集コストを求める
	for (auto i = std::uint64_t{1}; i < n; ++i)
	{
		for (auto j = std::uint64_t{1}; j < m; ++j)
		{
			const auto cost_del = cost_matrix[j * n + i - 1].first + 1;
			const auto cost_add = cost_matrix[(j - 1) * n + i].first + 1;
			auto cost_per = cost_matrix[(j - 1) * n + i - 1].first;
			if (orig_v[i - 1].compare(new_v[j - 1]) != 0)
				cost_per += 1;
			if (cost_per <= cost_del && cost_per <= cost_add)
				cost_matrix[j * n + i] = {cost_per, {i - 1, j - 1}};
			else if (cost_del <= cost_per && cost_del <= cost_add)
				cost_matrix[j * n + i] = {cost_del, {i - 1, j}};
			else
				cost_matrix[j * n + i] = {cost_add, {i, j - 1}};
		}
	}
	// backtrack
	std::stack<std::tuple<std::uint64_t, bool, std::string>> st;
	std::pair<std::uint64_t, std::uint64_t> pos{n - 1, m - 1};
	while (pos.first != 0 || pos.second != 0)
	{
		const auto [px, py] = pos;
		const auto [nx, ny] = cost_matrix[py * n + px].second;
		const bool isChanged = cost_matrix[py * n + px].first != cost_matrix[ny * n + nx].first;
		if (py > ny && isChanged)
			st.push(std::make_tuple(py, false, new_v[py - 1]));
		if (px > nx && isChanged)
			st.push(std::make_tuple(px, true, orig_v[px - 1]));

		pos = cost_matrix[py * n + px].second;
	}
	while (!st.empty())
	{
		res.emplace_back(st.top());
		st.pop();
	}
	return res;
}
