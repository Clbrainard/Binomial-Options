#include <vector> 
#include <cmath>
#include <iostream>
#include "Pricers.h"

//#############################################################
//                 PATH SIM IMPLEMENTATION
//#############################################################

std::vector<std::vector<double>> generateTree(
    double initial_price, double v, double dt, int N, double u, double d
) {

    std::vector<std::vector<double>> tree(N);

    tree[0].push_back(initial_price);

    for (int n = 1; n<N; n++) {
        tree[n].push_back(tree[n-1][0] * d);
        int prevSize = tree[n-1].size();
        for (int k=0; k<prevSize; k++) {
            tree[n].push_back(tree[n-1][k] * u);
        }
    }

    return tree;
}

//combinatorial equation, uses lgamma to avoid factorial overflow
double comb(int n, int k) {
    return std::exp(std::lgamma(n + 1) - std::lgamma(k + 1) - std::lgamma(n - k + 1));
}

//#############################################################
//                  EUROPEAN IMPLEMENTATION
//#############################################################

double priceEuropeanCall(
    double initial_price, double v, double T, int N, double K, double r
) {
    double dt = T/N;
    double u = std::exp(v * std::sqrt(dt));
    double d = 1/u;
    double p = (std::exp(r*dt)-d) / (u-d);
    double q = 1-p;
    int termSize = N-1;

    std::vector<std::vector<double>>  tree = generateTree(initial_price, v, dt, N, u, d);
    std::vector<double> terminalNodes = tree[N-1];

    double NTV = 0.0;

    for (int n = 0; n <= termSize; n++) {
        NTV += fmax(terminalNodes[n]-K,0) * comb(termSize,n) * std::pow(p,n) * std::pow(q,termSize-n);
    }

    return NTV * std::exp(-r*T);
}


double priceEuropeanPut(
    double initial_price, double v, double T, int N, double K, double r
) {
    double dt = T/N;
    double u = std::exp(v * std::sqrt(dt));
    double d = 1/u;
    double p = (std::exp(r*dt)-d) / (u-d);
    double q = 1-p;
    int termSize = N-1;

    std::vector<std::vector<double>>  tree = generateTree(initial_price, v, dt, N, u, d);
    std::vector<double> terminalNodes = tree[N-1];

    double NTV = 0.0;

    for (int n = 0; n <= termSize; n++) {
        NTV += fmax(K-terminalNodes[n],0) * comb(termSize,n) * std::pow(p,n) * std::pow(q,termSize-n);
    }

    return NTV * std::exp(-r*T);
}

//#############################################################
//                  EUROPEAN IMPLEMENTATION
//#############################################################


double priceAmericanCall(
    double initial_price, double v, double T, int N, double K, double r
) { 
    double dt = T/N;
    double u = std::exp(v * std::sqrt(dt));
    double d = 1/u;
    double p = (std::exp(r*dt)-d) / (u-d);
    double q = 1-p;

    std::vector<std::vector<double>>  tree = generateTree(initial_price, v, dt, N, u, d);


    for (int j=0; j<tree[N-1].size(); j++) {
            tree[N-1][j] = fmax(tree[N-1][j]-K,0);
    }


    for (int i = N-2; i>=0; i--) {
        for (int j=0; j<tree[i].size(); j++) {
            double IV = fmax(tree[i][j]-K,0);

            double EV = 0;
            EV += q*tree[i+1][j];
            EV += p*tree[i+1][j+1];
            EV *= std::exp(-r*dt);

            if (IV > EV) {
                tree[i][j] = IV;
            } else {
                tree[i][j] = EV;
            }
        }
    }


    return tree[0][0];
    
}

double priceAmericanPut(
    double initial_price, double v, double T, int N, double K, double r
) { 
    double dt = T/N;
    double u = std::exp(v * std::sqrt(dt));
    double d = 1/u;
    double p = (std::exp(r*dt)-d) / (u-d);
    double q = 1-p;

    std::vector<std::vector<double>>  tree = generateTree(initial_price, v, dt, N, u, d);


    for (int j=0; j<tree[N-1].size(); j++) {
            tree[N-1][j] = fmax(K-tree[N-1][j],0);
    }


    for (int i = N-2; i>=0; i--) {
        for (int j=0; j<tree[i].size(); j++) {
            double IV = fmax(K-tree[i][j],0);

            double EV = 0;
            EV += q*tree[i+1][j];
            EV += p*tree[i+1][j+1];
            EV *= std::exp(-r*dt);

            if (IV > EV) {
                tree[i][j] = IV;
            } else {
                tree[i][j] = EV;
            }
        }
    }


    return tree[0][0];
}