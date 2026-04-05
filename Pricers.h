#pragma once

#include <vector>

std::vector<std::vector<double>> generateTree(
    double initial_price, double v, double dt, int N, double u, double d
);

double priceEuropeanCall(
    double initial_price, double v, double T, int N, double K, double r
);

double priceEuropeanPut(
    double initial_price, double v, double T, int N, double K, double r
);

double priceAmericanPut(
    double initial_price, double v, double T, int N, double K, double r
);

double priceAmericanCall(
    double initial_price, double v, double T, int N, double K, double r
);
