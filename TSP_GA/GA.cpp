#pragma once

#include "GA.h"
#include <string>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <random>

using std::make_unique;
using std::vector;
using std::string;
using std::cout;
using std::endl;
using std::map;

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
    // Ok, I copied the shuffling routine for stackoverflow.
    // SRC: https://stackoverflow.com/questions/6926433/how-to-shuffle-a-stdvector
    auto randomNumGen = std::default_random_engine();
    for (int i = 0; i < poolSize; i++) {
        vector<int> chromosome;

        for (int j = 0; j < chromosomeSize; j++) {
            chromosome.push_back(j);
        }

        std::shuffle(std::begin(chromosome), std::end(chromosome), randomNumGen);
        chromosomePool->push_back(chromosome);
    }
}

void GA::initializeAdjacencyMatrix(string adjacencyMatrixPath) {
    std::ifstream file(adjacencyMatrixPath);
    if (!file.is_open()) {
        std::cerr << "Couldn't open file" << std::endl;
        return;
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

float GA::fitnessFunction(vector<int> const& chromosome) {
    float fitness = 0;
    for (int i = 0; i < chromosomeSize - 1; i++) {
        fitness += adjacencyMatrix->at(chromosome[i])[chromosome[i + 1]];
    }
    fitness += adjacencyMatrix->at(chromosome[chromosomeSize - 1])[chromosome[0]];
    return fitness;
}

map<float, vector<vector<int>>> GA::calcRouletteFitness(vector<vector<int>> const& chromosomePool) {
    map<float, vector<vector<int>>> fitnessToChromosomeMap;
    float sumOfAllFitnesses = 0;
    for (int i = 0; i < chromosomePool.size(); i++) {
        vector<int> chromosome(chromosomePool[i]);
        float fitness = fitnessFunction(chromosome);
        fitnessToChromosomeMap[fitness].push_back(chromosome);

        // fitness was not found as a key in 'fitnessToChromosomeMap'
        if (fitnessToChromosomeMap.find(fitness) != fitnessToChromosomeMap.end()) {
            sumOfAllFitnesses += fitness;
        }
    }
    map<float, vector<vector<int>>> rouletteFitToChromosomeMap;
    for (auto iter = fitnessToChromosomeMap.begin(); iter != fitnessToChromosomeMap.end(); iter++) {
        float fitness = iter->first;
        vector<vector<int>> chromosomes = iter->second;
        float rouletteFitness = fitness / sumOfAllFitnesses;

        rouletteFitToChromosomeMap[rouletteFitness] = chromosomes;
    }

    return rouletteFitToChromosomeMap;
}

vector<vector<int>> GA::selection(vector<vector<int>> const& chromosomePool) {
    map<float, vector<vector<int>>> fitnessToChromosomeMap = calcRouletteFitness(chromosomePool);
    vector<vector<int>> selectedChromosomes;

    while(selectedChromosomes.size() < chromosomeSize) {
        float chance = static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX);
        float sumFitness = 0;
        for (auto iter = fitnessToChromosomeMap.begin(); iter != fitnessToChromosomeMap.end(); iter++) {
            float fitness = iter->first;
            vector<vector<int>> chromosomes = iter->second;

            sumFitness += fitness;
            if (chance < sumFitness) {
                std::random_device rd;
                std::mt19937 gen(rd());
                std::uniform_int_distribution<> dis(0, chromosomes.size() - 1);
                vector<int> selectedChromosome = chromosomes[dis(gen)];
                selectedChromosomes.push_back(selectedChromosome);
                break;
            }
        }
    }

    return selectedChromosomes;
}

void GA::singlePointCrossover(vector<int> const& firstChromo, vector<int> const& secondChromo) {
    for (int i = 0; i < firstChromo.size(); i++) {
        cout << firstChromo[i] << " " << endl;
    }
    cout << "\n" << endl;
    for (int i = 0; i < secondChromo.size(); i++) {
        cout << secondChromo[i] << " " << endl;
    }
    cout << "\n" << endl;
}

void GA::crossover(vector<vector<int>> const& chromosomePool) {
    int halfwayIndex = static_cast<int>(chromosomeSize / 2);
    for (int i = 0; i < halfwayIndex; i++) {
        singlePointCrossover(chromosomePool[i], chromosomePool[i+halfwayIndex]);
    }
    auto test = chromosomePool[0];
}

void GA::runAlgorithm(int iterations) {
    float fitness = fitnessFunction(chromosomePool->at(0));
    vector<vector<int>> selectedChromosomes = selection(*chromosomePool);
    crossover(selectedChromosomes);
}

float GA::bruteForce() {
    vector<vector<int>> allSolutions = generateAllPermutations(chromosomeSize);
    float minimumFitness = INT_MAX;
    for (int i = 0; i < allSolutions.size(); i++) {
        auto test = allSolutions[i];
        float fitness = fitnessFunction(allSolutions[i]);
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