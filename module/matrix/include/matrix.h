#include <stdexcept>
#include <vector>

template <typename T> class Matrix : std::vector<T>
{
public:
	Matrix(size_t cols, size_t rows) : std::vector<T>::vector<T>(rows * cols), rows_(rows), cols_(cols) {}

	T operator()(size_t col, size_t row) const
	{
		const size_t itr = col * rows_ + row;
		if (cols_ * rows_ < itr)
			throw std::runtime_error("Matrix Range Error");
		return (*this)[itr];
	}

	T& operator()(size_t col, size_t row)
	{
		const size_t itr = col * rows_ + row;
		if (rows_ * cols_ < itr)
			throw std::runtime_error("Matrix Range Error");
		return (*this)[itr];
	}

	void operator=(std::initializer_list<T> itr)
	{
		if (this->size() != itr.size())
			throw std::runtime_error("Initializer list size is not equal to matrix size");
		std::copy(itr.begin(), itr.end(), this->begin());
	}

	Matrix<T>& operator=(Matrix<T> mat)
	{
		this->resize(mat.rows_ * mat.cols_);
		std::copy(mat.begin(), mat.end(), this->begin());
		return *this;
	}

	size_t rows() const
	{
		return this->rows_;
	}

	size_t cols() const
	{
		return this->cols_;
	}

private:
	size_t rows_, cols_;
};
