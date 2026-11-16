#include "math/root_finding.hpp"

#include <gtest/gtest.h>

#include <cmath>
#include <numbers>

using namespace kairos;

namespace {
double square_minus_two(double x) { return x * x - 2.0; }
double two_x(double x) { return 2.0 * x; }
}  // namespace

TEST(Bisection, FindsSqrtTwo) {
    const RootResult r = bisection(square_minus_two, 0.0, 2.0);
    EXPECT_TRUE(r.converged);
    EXPECT_NEAR(r.root, std::numbers::sqrt2, 1e-9);
}

TEST(Bisection, RejectsIntervalWithoutSignChange) {
    const RootResult r = bisection(square_minus_two, 2.0, 3.0);
    EXPECT_FALSE(r.converged);
    EXPECT_TRUE(std::isnan(r.root));
}

TEST(Bisection, ReturnsEndpointThatIsARoot) {
    const RootResult r = bisection([](double x) { return x - 1.0; }, 1.0, 5.0);
    EXPECT_TRUE(r.converged);
    EXPECT_DOUBLE_EQ(r.root, 1.0);
}

TEST(Newton, ConvergesQuadratically) {
    const RootResult r = newton(square_minus_two, two_x, 1.0);
    EXPECT_TRUE(r.converged);
    EXPECT_NEAR(r.root, std::numbers::sqrt2, 1e-10);
    EXPECT_LE(r.iterations, 8);
}

TEST(Newton, ReportsFailureOnZeroDerivative) {
    const RootResult r = newton(square_minus_two, two_x, 0.0);
    EXPECT_FALSE(r.converged);
}

TEST(NewtonBisection, FindsRootFasterThanBisection) {
    const RootResult safe = newton_bisection(square_minus_two, two_x, 0.0, 2.0);
    const RootResult slow = bisection(square_minus_two, 0.0, 2.0);
    EXPECT_TRUE(safe.converged);
    EXPECT_NEAR(safe.root, std::numbers::sqrt2, 1e-9);
    EXPECT_LT(safe.iterations, slow.iterations);
}

TEST(NewtonBisection, SurvivesWherePlainNewtonDiverges) {
    // atan has a root at 0, but plain Newton overshoots further on every
    // step when it starts beyond about 1.39.
    auto f = [](double x) { return std::atan(x); };
    auto df = [](double x) { return 1.0 / (1.0 + x * x); };

    EXPECT_FALSE(newton(f, df, 3.0).converged);

    const RootResult r = newton_bisection(f, df, -2.0, 30.0);
    EXPECT_TRUE(r.converged);
    EXPECT_NEAR(r.root, 0.0, 1e-9);
}

TEST(NewtonBisection, RejectsIntervalWithoutSignChange) {
    EXPECT_FALSE(newton_bisection(square_minus_two, two_x, 2.0, 3.0).converged);
}
