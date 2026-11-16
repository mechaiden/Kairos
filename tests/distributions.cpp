#include "math/distributions.hpp"

#include <gtest/gtest.h>

#include <cmath>
#include <limits>

using namespace kairos;

TEST(NormPdf, KnownValues) {
    EXPECT_NEAR(norm_pdf(0.0), 0.3989422804014327, 1e-15);
    EXPECT_NEAR(norm_pdf(1.0), 0.24197072451914337, 1e-15);
    EXPECT_DOUBLE_EQ(norm_pdf(-1.3), norm_pdf(1.3));
}

TEST(NormCdf, KnownValues) {
    EXPECT_DOUBLE_EQ(norm_cdf(0.0), 0.5);
    EXPECT_NEAR(norm_cdf(1.0), 0.8413447460685429, 1e-15);
    EXPECT_NEAR(norm_cdf(-1.96), 0.024997895148220435, 1e-15);
}

TEST(NormCdf, SymmetryAndLimits) {
    for (double x : {0.1, 0.7, 1.5, 3.0, 6.0}) {
        EXPECT_NEAR(norm_cdf(x) + norm_cdf(-x), 1.0, 1e-15);
    }
    EXPECT_DOUBLE_EQ(norm_cdf(40.0), 1.0);
    EXPECT_DOUBLE_EQ(norm_cdf(-40.0), 0.0);
}

TEST(NormCdf, LowerTailKeepsRelativePrecision) {
    // N(-10) is about 7.62e-24. The erf form would return exactly 0 here.
    const double expected = 7.619853024160527e-24;
    EXPECT_NEAR(norm_cdf(-10.0) / expected, 1.0, 1e-12);
}

TEST(NormInvCdf, KnownQuantiles) {
    EXPECT_NEAR(norm_inv_cdf(0.5), 0.0, 1e-15);
    EXPECT_NEAR(norm_inv_cdf(0.975), 1.959963984540054, 1e-12);
    EXPECT_NEAR(norm_inv_cdf(0.01), -2.3263478740408408, 1e-12);
}

TEST(NormInvCdf, RoundTrip) {
    for (double p : {1e-10, 1e-4, 0.02, 0.3, 0.5, 0.8, 0.99, 1.0 - 1e-9}) {
        EXPECT_NEAR(norm_cdf(norm_inv_cdf(p)) / p, 1.0, 1e-10) << "p = " << p;
    }
}

TEST(NormInvCdf, EdgeCases) {
    EXPECT_EQ(norm_inv_cdf(0.0), -std::numeric_limits<double>::infinity());
    EXPECT_EQ(norm_inv_cdf(1.0), std::numeric_limits<double>::infinity());
}
