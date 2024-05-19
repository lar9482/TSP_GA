#pragma once

#include "GA.h"
#include <string>
#include <fstream>
#include <iostream>

using std::make_unique;
using std::vector;
using std::string;
using std::cout;
using std::endl;

GA::GA(int poolSize, string adjacencyMatrixPath):
    poolSize(poolSize), 
    chromosomeSize(-1), 
    chromosomePool(make_unique<vector<vector<int>>>()),
    adjacencyMatrix(make_unique<vector<vector<int>>>())
{
    initializeAdjacencyMatrix(adjacencyMatrixPath);
    initializeChromosomePool();
}

GA::~GA() {

}

void GA::initializeChromosomePool() {
    for (int i = 0; i < poolSize; i++) {
        vector<int> chromosome;

        for (int j = 0; j < chromosomeSize; j++) {
            chromosome.push_back(j);
        }
        chromosomePool->push_back(chromosome);
    }
}

void GA::initializeAdjacencyMatrix(string adjacencyMatrixPath) {
    std::ifstream file(adjacencyMatrixPath);
    if (!file.is_open()) {
        std::cout << "Couldn't open file" << std::endl;
    }

    string fileLine;
    while (std::getline(file, fileLine)) {
        vector<int> matrixRow;

        string currNum = "";
        for (int i = 0; i < fileLine.size(); i++) {
            char currChar = fileLine[i];
            if (currChar == ',') {
                matrixRow.push_back(std::stoi(currNum));
                currNum = "";
            }
            else {
                currNum.push_back(currChar);
            }
        }
        matrixRow.push_back(std::stoi(currNum));
        adjacencyMatrix->push_back(matrixRow);
        
        if (chromosomeSize == -1) {
            chromosomeSize = matrixRow.size();
        }
    }
}

int GA::fitnessFunction(vector<int> const& chromosome) {
    int fitness = 0;
    for (int i = 0; i < chromosomeSize - 1; i++) {
        fitness += adjacencyMatrix->at(i)[i+1];
    }
    fitness += adjacencyMatrix->at(chromosomeSize - 1)[0];
    return fitness;
}

void GA::runAlgorithm() {
    int fitness = fitnessFunction(chromosomePool->at(0));
}