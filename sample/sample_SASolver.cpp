#include <algorithm>
#include <cmath>
#include <iostream>
#include <random>
#include <vector>

#include "SASolver.h"

int main()
{
	std::vector<std::pair<double, double>> model;

	std::vector<size_t> I_ini = {0, 1, 2, 3};

	std::random_device seedGen;
	std::mt19937 engine(seedGen());

	SASolver<double, std::vector<std::pair<int, int>>, std::vector<size_t>> solver(model);

	solver.setEval(
		[](const std::vector<std::pair<int, int>> model, std::vector<size_t> i)
		{
			double dist = 0;
			for (size_t itr = 0; itr < model.size(); itr++)
			{
				const auto [px, py] = model[i[itr]];
				const auto [cx, cy] = model[i[(itr + 1) % model.size()]];
				dist += std::sqrt(std::pow(cx - px, 2) + std::pow(cy - py, 2));
			}
			return dist;
		});

	solver.setGetNeighbor(
		[&](const std::vector<size_t>& I)
		{
			std::vector<size_t> l;
			for (const auto& el : I)
				l.push_back(el);
			std::uniform_int_distribution<int> dist(1, l.size() - 1);
			const size_t start = dist(engine);
			std::erase(l, start);
			if (l.size() < 1)
				throw std::runtime_error("List size is too small");
			std::uniform_int_distribution<int> dist2(1, l.size() - 1);
			const size_t end = l[dist2(engine)];
			std::vector<size_t> res(I.begin(), I.end());
			std::iter_swap(res.begin() + start, res.begin() + end);
			return res;
		});

	solver.setCalcPtrans(
		[](const double& e, const double& ePrime, double T)
		{
			if (ePrime < e)
				return 1.0;
			else
				return std::exp((e - ePrime) / T);
		});

	solver.setTransition(
		[](double T)
		{
			return T * 0.99;
		});

	const auto res = solver.solve(100, I_ini, 1e-6);
	for (const auto& el : res)
	{
		std::cout << el << std::endl;
	}
	return 0;
}
