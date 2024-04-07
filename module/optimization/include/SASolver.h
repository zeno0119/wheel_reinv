#include <functional>
#include <iostream>
#include <random>
#include <stdexcept>

template <typename E, typename M, typename I> class SASolver
{
public:
	SASolver(const M& model) : model_(model)
	{
		std::random_device seedGen;
		engine_ = std::mt19937(seedGen());
	}
	void setEval(std::function<E(const M&, const I&)> fn)
	{
		fnEval_ = fn;
	}

	void setGetNeighbor(std::function<I(const I&)> fn)
	{
		fnGetNeighbor_ = fn;
	}

	void setCalcPtrans(std::function<double(const E&, const E&, double)> fn)
	{
		fnCalcPtrans_ = fn;
	}

	void setTransition(std::function<double(double)> fn)
	{
		fnCalcTemp_ = fn;
	}

	I solve(double T_ini, const I& I_ini, double T_end)
	{
		if (fnEval_ == nullptr || fnGetNeighbor_ == nullptr || fnCalcPtrans_ == nullptr || fnCalcTemp_ == nullptr)
			throw std::runtime_error("Some of User defined functions are nullptr");
		T_ = T_ini;
		I i = I_ini;
		auto e = fnEval_(model_, I_ini);
		while (T_ > T_end)
		{
			const auto iPrime = fnGetNeighbor_(i);
			const auto ePrime = fnEval_(model_, iPrime);
			if (dist_(engine_) < fnCalcPtrans_(e, ePrime, T_))
			{
				i = iPrime;
				e = ePrime;
			}
			T_ = fnCalcTemp_(T_);
		}
		return i;
	}

private:
	std::function<E(const M&, const I&)> fnEval_;
	std::function<I(const I&)> fnGetNeighbor_;
	std::function<double(const E&, const E&, double)> fnCalcPtrans_;
	std::function<double(double)> fnCalcTemp_;

	double T_;
	M model_;

	std::mt19937 engine_;
	std::uniform_real_distribution<float> dist_;
};