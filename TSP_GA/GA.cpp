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
        fitness += adjacencyMatrix->at(chromosome[i])[chromosome[i + 1]];
    }
    fitness += adjacencyMatrix->at(chromosome[chromosomeSize - 1])[chromosome[0]];
    return fitness;
}

void GA::runAlgorithm() {
    int fitness = fitnessFunction(chromosomePool->at(0));
}

int GA::bruteForce() {
    vector<vector<int>> allSolutions = generateAllPermutations(chromosomeSize);
    int minimumFitness = INT_MAX;
    for (int i = 0; i < allSolutions.size(); i++) {
        auto test = allSolutions[i];
        int fitness = fitnessFunction(allSolutions[i]);
        if (fitness < minimumFitness) {
            minimumFitness = fitness;
        }
    }
    return minimumFitness;
}

vector<vector<int>> generateAllPermutations(int n) {
    vector<vector<int>> generatedPerms;
    vector<int> currPerm;
    vector<int> nums;
    
    for (int i = 0; i < n; i++) {
        nums.push_back(i);
    }
    generatePermutations(nums, 0, generatedPerms);

    return generatedPerms;
}

void generatePermutations(vector<int>& nums, int start, vector<vector<int>>& result) {
    if (start == nums.size()) {
        result.push_back(nums);
        return;
    }

    for (int i = start; i < nums.size(); ++i) {
        std::swap(nums[start], nums[i]);
        generatePermutations(nums, start + 1, result);
        std::swap(nums[start], nums[i]);
    }
}