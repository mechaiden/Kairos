#pragma once

#include <cmath>
#include <limits>

namespace kairos {

struct RootResult {
    double root = std::numeric_limits<double>::quiet_NaN();
    int iterations = 0;
    bool converged = false;
};

// Bisection on [lo, hi]. Requires f(lo) and f(hi) to have opposite signs.
// Slow (one bit of accuracy per step) but cannot fail on a valid bracket.
template <typename F>
RootResult bisection(F&& f, double lo, double hi, double tol = 1e-10, int max_iter = 200) {
    RootResult result;
    double f_lo = f(lo);
    double f_hi = f(hi);
    if (f_lo == 0.0) return {lo, 0, true};
    if (f_hi == 0.0) return {hi, 0, true};
    if ((f_lo < 0.0) == (f_hi < 0.0)) return result;  // no sign change: not a bracket

    for (int i = 1; i <= max_iter; ++i) {
        const double mid = 0.5 * (lo + hi);
        const double f_mid = f(mid);
        result.root = mid;
        result.iterations = i;
        if (f_mid == 0.0 || 0.5 * (hi - lo) < tol) {
            result.converged = true;
            return result;
        }
        if ((f_mid < 0.0) == (f_lo < 0.0)) {
            lo = mid;
            f_lo = f_mid;
        } else {
            hi = mid;
        }
    }
    return result;
}

// Plain Newton-Raphson from a starting guess. Fast (quadratic convergence)
// near the root, but can diverge when the derivative is small.
template <typename F, typename DF>
RootResult newton(F&& f, DF&& df, double x0, double tol = 1e-10, int max_iter = 50) {
    RootResult result;
    double x = x0;
    for (int i = 1; i <= max_iter; ++i) {
        const double fx = f(x);
        const double dfx = df(x);
        result.root = x;
        result.iterations = i;
        if (std::abs(fx) < tol) {
            result.converged = true;
            return result;
        }
        if (dfx == 0.0 || !std::isfinite(dfx)) return result;
        const double next = x - fx / dfx;
        if (!std::isfinite(next)) return result;
        if (std::abs(next - x) < tol) {
            result.root = next;
            result.converged = true;
            return result;
        }
        x = next;
    }
    return result;
}

// Newton-Raphson kept inside a bracket [lo, hi]. Whenever a Newton step would
// leave the bracket (or the derivative is unusable) it takes a bisection step
// instead, so it has Newton's speed with bisection's guarantee.
template <typename F, typename DF>
RootResult newton_bisection(F&& f, DF&& df, double lo, double hi, double tol = 1e-10,
                            int max_iter = 100) {
    RootResult result;
    const double f_lo = f(lo);
    const double f_hi = f(hi);
    if (f_lo == 0.0) return {lo, 0, true};
    if (f_hi == 0.0) return {hi, 0, true};
    if ((f_lo < 0.0) == (f_hi < 0.0)) return result;

    const bool lo_is_negative = f_lo < 0.0;
    double x = 0.5 * (lo + hi);
    for (int i = 1; i <= max_iter; ++i) {
        const double fx = f(x);
        result.root = x;
        result.iterations = i;
        if (std::abs(fx) < tol) {
            result.converged = true;
            return result;
        }
        // Shrink the bracket so it still contains the sign change.
        if ((fx < 0.0) == lo_is_negative) {
            lo = x;
        } else {
            hi = x;
        }
        const double dfx = df(x);
        double next = x - fx / dfx;
        if (dfx == 0.0 || !std::isfinite(next) || next <= lo || next >= hi) {
            next = 0.5 * (lo + hi);
        }
        if (std::abs(next - x) < tol || 0.5 * (hi - lo) < tol) {
            result.root = next;
            result.converged = true;
            return result;
        }
        x = next;
    }
    return result;
}

}  // namespace kairos
