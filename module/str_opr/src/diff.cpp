#include "str_opr.h"

#include <algorithm>
#include <sstream>
#include <stack>

auto operator<=>(const diff_result& a, const diff_result& b)
{
	return a.orig_begin <=> b.orig_begin;
}
static diff_result squash_diff(std::stack<std::pair<std::uint64_t, std::string>>& del_st,
							   std::stack<std::pair<std::uint64_t, std::string>>& add_st,
							   const std::uint64_t px, const std::uint64_t py)
{

	std::vector<std::string> del_v;
	std::vector<std::string> add_v;
	std::uint64_t begin_del;
	std::uint64_t end_del;
	std::uint64_t begin_add;
	std::uint64_t end_add;
	if (!del_st.empty())
		end_del = del_st.top().first;
	while (!del_st.empty())
	{
		const auto [line, str] = del_st.top();
		if (del_st.size() == 1)
			begin_del = line;
		del_v.push_back(str);
		del_st.pop();
	}
	if (!add_st.empty())
		end_add = add_st.top().first;
	while (!add_st.empty())
	{
		const auto [line, str] = add_st.top();
		if (add_st.size() == 1)
			begin_add = line;
		add_v.push_back(str);
		add_st.pop();
	}

	if (del_v.empty() && !add_v.empty())
		return {px, px, std::vector<std::string>(), begin_add, end_add, add_v, diff_state::added};
	else if (!del_v.empty() && add_v.empty())
		return {begin_del, end_del, del_v, py, py, std::vector<std::string>(), diff_state::deleted};
	else
		return {begin_add, end_add, del_v, begin_del, end_del, add_v, diff_state::changed};
}

std::vector<diff_result> diff(const std::string& orig_str, const std::string& new_str)
{
	std::vector<diff_result> res;
	std::vector<std::string> orig_v, new_v;
	{
		std::stringstream ss(orig_str);
		std::string s;
		while (std::getline(ss, s))
			orig_v.emplace_back(s);
	}
	{
		std::stringstream ss(new_str);
		std::string s;
		while (std::getline(ss, s))
			new_v.emplace_back(s);
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
	for (std::uint64_t i = 1; i < n; ++i)
	{
		for (std::uint64_t j = 1; j < m; ++j)
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
	std::stack<std::pair<std::uint64_t, std::string>> del_st;
	std::stack<std::pair<std::uint64_t, std::string>> add_st;
	std::pair<std::uint64_t, std::uint64_t> pos{n - 1, m - 1};
	while (pos.first != 0 || pos.second != 0)
	{
		const auto [px, py] = pos;
		const auto [nx, ny] = cost_matrix[py * n + px].second;
		const bool isChanged = cost_matrix[py * n + px].first != cost_matrix[ny * n + nx].first;
		if (py > ny && isChanged)
			add_st.emplace(py, new_v[py - 1]);
		if (px > nx && isChanged)
			del_st.emplace(px, orig_v[px - 1]);
		if (!isChanged)
			res.emplace_back(squash_diff(del_st, add_st, px, py));
		pos = cost_matrix[py * n + px].second;
	}
	if (!del_st.empty() || !add_st.empty())
		res.emplace_back(squash_diff(del_st, add_st, 0, 0));
	std::sort(res.begin(), res.end());
	return res;
}
