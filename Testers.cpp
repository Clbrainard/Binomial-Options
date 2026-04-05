#include "Pricers.h"
#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

double getModelPrice(int type, double S, double v, double T, int N, double K, double r) {
    switch (type) {
        case 1: return priceEuropeanCall(S, v, T, N, K, r);
        case 2: return priceEuropeanPut(S, v, T, N, K, r);
        case 3: return priceAmericanCall(S, v, T, N, K, r);
        case 4: return priceAmericanPut(S, v, T, N, K, r);
        default: return 0.0;
    }
}

double computeAverageErrorAgainstActual(const std::string& datasetPath, int type, int N) {
    std::ifstream input(datasetPath);
    if (!input.is_open()) {
        std::cerr << "Failed to open dataset: " << datasetPath << "\n";
        return 0.0;
    }

    std::string line;

    double totalError = 0.0;
    int count = 0;

    while (std::getline(input, line)) {
        if (line.empty()) continue;
        if (line[0] == '#' || line[0] == 'S') continue;

        std::stringstream ss(line);
        std::string cell;
        auto next = [&]() { std::getline(ss, cell, ','); return std::stod(cell); };

        double S      = next();
        double r      = next();
        double K      = next();
        next();            // Days_to_exp (unused)
        double T      = next();
        next();            // Moneyness (unused)
        double v      = next();
        double actual = next();

        double model = getModelPrice(type, S, v, T, N, K, r);
        totalError += std::abs((model - actual) / actual) * 100.0;
        ++count;
    }

    return count > 0 ? totalError / count : 0.0;
}

int main() {


    // Change these values to run different validations.
    /*
    1=European Call
    2=European Put
    3=American Call
    4=American Put
    */ 
    int pricingType = 1;
    std::string datasetPath = "validation/EuropeanCalls.csv";

    //Run validation here
    int numSteps;
    std::cout << "Input number of time steps:" << "\n";
    std::cin >> numSteps;

    double avgError = computeAverageErrorAgainstActual(datasetPath, pricingType, numSteps);
    std::cout << "European Call Average percent error (%): " << avgError << "\n";

    return 0;
}
