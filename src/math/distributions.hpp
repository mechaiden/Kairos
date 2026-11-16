#pragma once

namespace kairos {

// Standard normal probability density: phi(x) = exp(-x^2 / 2) / sqrt(2*pi).
double norm_pdf(double x);

// Standard normal cumulative distribution: N(x) = P(Z <= x).
// Uses erfc so the lower tail keeps full relative precision.
double norm_cdf(double x);

// Inverse of norm_cdf: returns x such that N(x) = p, for p in (0, 1).
// Returns -inf for p <= 0 and +inf for p >= 1.
double norm_inv_cdf(double p);

}  // namespace kairos
