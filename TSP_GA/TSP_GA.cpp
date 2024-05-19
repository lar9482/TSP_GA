// TSP_GA.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>
#include "GA.h"
#include <fstream>

using std::string;

int main() {
    int poolSize = 10;
    int iterations = 100;
    string adjacencyMatrixPath = "DistanceMatrix_5.csv";

    GA geneticAlgo(poolSize, adjacencyMatrixPath);
    // geneticAlgo.bruteForce();
    geneticAlgo.runAlgorithm(iterations);
}
