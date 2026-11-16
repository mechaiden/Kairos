#include "math/distributions.hpp"

#include <cmath>
#include <limits>
#include <numbers>

namespace kairos {

double norm_pdf(double x) {
    return std::numbers::inv_sqrtpi / std::numbers::sqrt2 * std::exp(-0.5 * x * x);
}

double norm_cdf(double x) {
    // N(x) = 0.5 * erfc(-x / sqrt(2)). The textbook form 0.5 * (1 + erf(x / sqrt(2)))
    // loses all precision for very negative x, because erf is then close to -1.
    return 0.5 * std::erfc(-x / std::numbers::sqrt2);
}

double norm_inv_cdf(double p) {
    if (p <= 0.0) return -std::numeric_limits<double>::infinity();
    if (p >= 1.0) return std::numeric_limits<double>::infinity();

    // Acklam's rational approximation (relative error about 1e-9),
    // followed by one Halley step that brings it to machine precision.
    static constexpr double a[] = {-3.969683028665376e+01, 2.209460984245205e+02,
                                   -2.759285104469687e+02, 1.383577518672690e+02,
                                   -3.066479806614716e+01, 2.506628277459239e+00};
    static constexpr double b[] = {-5.447609879822406e+01, 1.615858368580409e+02,
                                   -1.556989798598866e+02, 6.680131188771972e+01,
                                   -1.328068155288572e+01};
    static constexpr double c[] = {-7.784894002430293e-03, -3.223964580411365e-01,
                                   -2.400758277161838e+00, -2.549732539343734e+00,
                                   4.374664141464968e+00,  2.938163982698783e+00};
    static constexpr double d[] = {7.784695709041462e-03, 3.224671290700398e-01,
                                   2.445134137142996e+00, 3.754408661907416e+00};
    constexpr double p_low = 0.02425;

    auto tail = [](double q) {
        return (((((c[0] * q + c[1]) * q + c[2]) * q + c[3]) * q + c[4]) * q + c[5]) /
               ((((d[0] * q + d[1]) * q + d[2]) * q + d[3]) * q + 1.0);
    };

    double x;
    if (p < p_low) {
        x = tail(std::sqrt(-2.0 * std::log(p)));
    } else if (p > 1.0 - p_low) {
        x = -tail(std::sqrt(-2.0 * std::log1p(-p)));
    } else {
        const double q = p - 0.5;
        const double r = q * q;
        x = (((((a[0] * r + a[1]) * r + a[2]) * r + a[3]) * r + a[4]) * r + a[5]) * q /
            (((((b[0] * r + b[1]) * r + b[2]) * r + b[3]) * r + b[4]) * r + 1.0);
    }

    const double error = norm_cdf(x) - p;
    const double u = error / norm_pdf(x);
    return x - u / (1.0 + 0.5 * x * u);
}

}  // namespace kairos
