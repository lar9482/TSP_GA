// TSP_GA.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>
#include "GA.h"
#include <fstream>

using std::string;

int main() {
    int poolSize = 20;
    int iterations = 10000;
    float mutationRate = 0.7;
    string adjacencyMatrixPath = "DistanceMatrix_5.csv";

    GA geneticAlgo(poolSize, mutationRate, adjacencyMatrixPath);
    geneticAlgo.runAlgorithm(iterations);
    // float bestScore = geneticAlgo.bruteForce();
    // std::cout << bestScore << std::endl;
}
