# Binomial Tree Option Pricer (CRR)

A C++ implementation of the Cox-Ross-Rubinstein binomial tree model for pricing European and American options. This repo is part of a larger portfolio of option pricing methods; companion implementations include a LSM Monte Carlo pricer.

## Supported Instruments

| Type | Style | Method |
|------|-------|--------|
| Call | European | Combinatorial closed-form over terminal nodes |
| Put  | European | Combinatorial closed-form over terminal nodes |
| Call | American | Backward induction with early exercise check |
| Put  | American | Backward induction with early exercise check |

## Methodology

Up/down factors follow the CRR parameterization: $u = e^{\sigma\sqrt{\Delta t}}$, $d = 1/u$, with risk-neutral probability $p = \frac{e^{r\Delta t} - d}{u - d}$. American options are priced by backward induction — at each node the option value is $\max(\text{intrinsic value},\ e^{-r\Delta t}(p \cdot V_u + (1-p) \cdot V_d))$. European option prices are computed directly from the terminal distribution using the binomial formula, avoiding the full backward pass.

> Cox, J. C., Ross, S. A., & Rubinstein, M. (1979). Option pricing: A simplified approach. *Journal of Financial Economics*, 7(3), 229–263.

## Validation

Benchmark prices are generated from QuantLib's Crank-Nicolson PDE solver at 3200×3200 grid resolution. The validation suite covers all four option types across a range of strikes, maturities, volatilities, and moneyness levels.

Accuracy is reported as mean absolute percentage error (MAPE) at varying step counts $N$.

### Results

| Option Type | N = 50 | N = 100 | N = 250 | N = 1000 |
|-------------|--------|---------|---------|----------|
| European Call | `1.147` | `0.594` | `0.255` | `0.056` |
| European Put  | `0.987` | `0.397` | `0.144` | `0.037` |
| American Call | `1.114` | `0.577` | `0.248` | `0.054` |
| American Put  | `0.866` | `0.379` | `0.139` | `0.034` |

> *Insert MAPE vs. N convergence plot here*

## Key Findings

MAPE decreases steadily as $N$ increases across all four instrument types. At $N = 1000$ all instruments price within 0.06% MAPE of the QuantLib Crank-Nicolson reference.

American call and European call MAPE are nearly identical at every step count (e.g., 0.054% vs. 0.056% at $N = 1000$), consistent with the result that an American call on a non-dividend-paying asset carries no early exercise premium. American puts converge at a similar rate to European puts despite the added early exercise boundary.

## Usage

```cpp
#include "Pricers.h"

// Parameters
double S = 100.0;   // spot price
double K = 100.0;   // strike
double T = 1.0;     // time to expiry (years)
double r = 0.05;    // continuous risk-free rate
double v = 0.20;    // annualised volatility
int    N = 500;     // number of time steps

double american_put = priceAmericanPut(S, v, T, N, K, r);
double american_call = priceAmericanCall(S, v, T, N, K, r);
double european_put = priceEuropeanPut(S, v, T, N, K, r);
double european_call = priceEuropeanCall(S, v, T, N, K, r);
```

### Validation runner

```cpp
// In Testers.cpp — set pricingType and datasetPath, then:
// 1 = European Call, 2 = European Put, 3 = American Call, 4 = American Put
./pricer   // enter N at the prompt; outputs mean absolute percentage error
```

## References

1. Cox, J. C., Ross, S. A., & Rubinstein, M. (1979). Option pricing: A simplified approach. *Journal of Financial Economics*, 7(3), 229–263.
2. Shreve, S. E. (2004). *Stochastic Calculus for Finance I: The Binomial Asset Pricing Model*. Springer.
3. QuantLib Development Team. (2024). *QuantLib: A free/open-source library for quantitative finance*. https://www.quantlib.org
