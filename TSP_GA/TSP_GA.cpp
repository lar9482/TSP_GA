// TSP_GA.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>
#include "GA.h"
#include <fstream>

using std::string;

int main() {
    int poolSize = 100;
    int iterations = 1000;
    float mutationRate = 0.05;
    string adjacencyMatrixPath = "DistanceMatrix_100.csv";

    GA geneticAlgo(poolSize, mutationRate, adjacencyMatrixPath);
    geneticAlgo.runAlgorithm(iterations);
    // float bestScore = geneticAlgo.bruteForce();
    // std::cout << bestScore << std::endl;
}
