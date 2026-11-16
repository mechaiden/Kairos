#pragma once

#include <cstddef>
#include <initializer_list>
#include <vector>

namespace kairos {

// Small dense matrix of doubles, stored row by row in one contiguous vector.
class Matrix {
public:
    Matrix() = default;
    Matrix(std::size_t rows, std::size_t cols, double fill = 0.0);
    Matrix(std::initializer_list<std::initializer_list<double>> rows);

    static Matrix identity(std::size_t n);

    std::size_t rows() const { return rows_; }
    std::size_t cols() const { return cols_; }

    double& operator()(std::size_t i, std::size_t j) { return data_[i * cols_ + j]; }
    double operator()(std::size_t i, std::size_t j) const { return data_[i * cols_ + j]; }

    // Matrix product. Throws std::invalid_argument if the shapes do not match.
    Matrix operator*(const Matrix& other) const;

    // n-th power of a square matrix by repeated squaring: O(log n) products.
    Matrix power(unsigned n) const;

private:
    std::size_t rows_ = 0;
    std::size_t cols_ = 0;
    std::vector<double> data_;
};

// Row vector times matrix: result[j] = sum_i v[i] * m(i, j).
// This is one step of a Markov chain applied to a distribution v.
std::vector<double> operator*(const std::vector<double>& v, const Matrix& m);

}  // namespace kairos
