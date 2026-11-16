#include "math/matrix.hpp"

#include <stdexcept>

namespace kairos {

Matrix::Matrix(std::size_t rows, std::size_t cols, double fill)
    : rows_(rows), cols_(cols), data_(rows * cols, fill) {}

Matrix::Matrix(std::initializer_list<std::initializer_list<double>> rows)
    : rows_(rows.size()), cols_(rows.size() == 0 ? 0 : rows.begin()->size()) {
    data_.reserve(rows_ * cols_);
    for (const auto& row : rows) {
        if (row.size() != cols_) {
            throw std::invalid_argument("Matrix: all rows must have the same length");
        }
        data_.insert(data_.end(), row.begin(), row.end());
    }
}

Matrix Matrix::identity(std::size_t n) {
    Matrix m(n, n);
    for (std::size_t i = 0; i < n; ++i) m(i, i) = 1.0;
    return m;
}

Matrix Matrix::operator*(const Matrix& other) const {
    if (cols_ != other.rows_) {
        throw std::invalid_argument("Matrix: shapes do not match for multiplication");
    }
    Matrix result(rows_, other.cols_);
    // i-k-j loop order walks both inputs along rows, which is cache friendly.
    for (std::size_t i = 0; i < rows_; ++i) {
        for (std::size_t k = 0; k < cols_; ++k) {
            const double a = (*this)(i, k);
            for (std::size_t j = 0; j < other.cols_; ++j) {
                result(i, j) += a * other(k, j);
            }
        }
    }
    return result;
}

Matrix Matrix::power(unsigned n) const {
    if (rows_ != cols_) {
        throw std::invalid_argument("Matrix: power needs a square matrix");
    }
    Matrix result = identity(rows_);
    Matrix base = *this;
    while (n > 0) {
        if (n & 1u) result = result * base;
        base = base * base;
        n >>= 1u;
    }
    return result;
}

std::vector<double> operator*(const std::vector<double>& v, const Matrix& m) {
    if (v.size() != m.rows()) {
        throw std::invalid_argument("Matrix: vector length does not match row count");
    }
    std::vector<double> result(m.cols(), 0.0);
    for (std::size_t i = 0; i < m.rows(); ++i) {
        for (std::size_t j = 0; j < m.cols(); ++j) {
            result[j] += v[i] * m(i, j);
        }
    }
    return result;
}

}  // namespace kairos
