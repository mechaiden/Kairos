#include "math/matrix.hpp"

#include <gtest/gtest.h>

#include <stdexcept>
#include <vector>

using namespace kairos;

TEST(Matrix, ConstructionAndAccess) {
    Matrix m(2, 3, 1.5);
    EXPECT_EQ(m.rows(), 2u);
    EXPECT_EQ(m.cols(), 3u);
    EXPECT_DOUBLE_EQ(m(1, 2), 1.5);
    m(0, 1) = 7.0;
    EXPECT_DOUBLE_EQ(m(0, 1), 7.0);
}

TEST(Matrix, InitializerListRejectsRaggedRows) {
    EXPECT_THROW((Matrix{{1.0, 2.0}, {3.0}}), std::invalid_argument);
}

TEST(Matrix, Multiplication) {
    const Matrix a{{1.0, 2.0}, {3.0, 4.0}};
    const Matrix b{{5.0, 6.0}, {7.0, 8.0}};
    const Matrix c = a * b;
    EXPECT_DOUBLE_EQ(c(0, 0), 19.0);
    EXPECT_DOUBLE_EQ(c(0, 1), 22.0);
    EXPECT_DOUBLE_EQ(c(1, 0), 43.0);
    EXPECT_DOUBLE_EQ(c(1, 1), 50.0);
}

TEST(Matrix, MultiplicationRejectsShapeMismatch) {
    EXPECT_THROW(Matrix(2, 3) * Matrix(2, 3), std::invalid_argument);
}

TEST(Matrix, IdentityIsNeutral) {
    const Matrix a{{1.0, 2.0}, {3.0, 4.0}};
    const Matrix c = a * Matrix::identity(2);
    for (std::size_t i = 0; i < 2; ++i)
        for (std::size_t j = 0; j < 2; ++j) EXPECT_DOUBLE_EQ(c(i, j), a(i, j));
}

TEST(Matrix, PowerMatchesRepeatedMultiplication) {
    const Matrix p{{0.9, 0.1}, {0.3, 0.7}};
    Matrix expected = Matrix::identity(2);
    for (int i = 0; i < 5; ++i) expected = expected * p;
    const Matrix actual = p.power(5);
    for (std::size_t i = 0; i < 2; ++i)
        for (std::size_t j = 0; j < 2; ++j) EXPECT_NEAR(actual(i, j), expected(i, j), 1e-14);

    const Matrix zeroth = p.power(0);
    EXPECT_DOUBLE_EQ(zeroth(0, 0), 1.0);
    EXPECT_DOUBLE_EQ(zeroth(0, 1), 0.0);
}

TEST(Matrix, PowerRejectsNonSquare) {
    EXPECT_THROW(Matrix(2, 3).power(2), std::invalid_argument);
}

TEST(Matrix, RowVectorTimesMatrix) {
    const Matrix p{{0.9, 0.1}, {0.3, 0.7}};
    const std::vector<double> v = std::vector<double>{1.0, 0.0} * p;
    EXPECT_DOUBLE_EQ(v[0], 0.9);
    EXPECT_DOUBLE_EQ(v[1], 0.1);
    EXPECT_THROW((std::vector<double>{1.0} * p), std::invalid_argument);
}
