#include <random>
#include <iostream>
#include <cmath>

int  main() {
  const double S0 = 100.0, K = 100.0, r = 0.05, sigma = 0.2, T = 1.0;
  const int steps = 252;

  const unsigned seed = 42;
  std::mt19937_64 rng(seed);
  std::normal_distribution<double> normal(0.0, 1.0);


  const double dt = T / steps;
  const double drift = (r - 0.5 * sigma * sigma) * dt;
  const double vol = sigma * std::sqrt(dt);

  double S = S0;
  for (int i = 0; i < steps; ++i) {
    const double Z = normal(rng);
    S = S * std::exp(drift + vol * Z);
    std::cout << "Day " << i + 1 << ": " << S << "\n";

  }

  std::cout << "Final Price: " << S << "\n";
  return 0;

}

