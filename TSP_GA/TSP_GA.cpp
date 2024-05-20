// TSP_GA.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>
#include "GA.h"
#include <fstream>

using std::string;

int main() {
    int poolSize = 10;
    int iterations = 30000;
    float mutationRate = 0.05;
    string adjacencyMatrixPath = "DistanceMatrix_1000.csv";

    GA geneticAlgo(poolSize, mutationRate, adjacencyMatrixPath);
    geneticAlgo.runAlgorithm(iterations);
}
