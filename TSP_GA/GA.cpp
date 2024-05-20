#pragma once

#include "GA.h"
#include <string>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <random>
#include <cstdlib>

using std::make_unique;
using std::vector;
using std::string;
using std::cout;
using std::endl;
using std::map;

GA::GA(int poolSize, float mutationRate, string adjacencyMatrixPath):
    poolSize(poolSize), 
    chromosomeSize(-1), 
    mutationRate(mutationRate),
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

vector<vector<int>> GA::selection(map<float, vector<vector<int>>> const&  fitnessToChromosomeMap) {
    vector<vector<int>> selectedChromosomes;

    while(selectedChromosomes.size() < chromosomeSize) {
        float chance = randomFloat(0, 1);
        float sumFitness = 0;
        for (auto iter = fitnessToChromosomeMap.begin(); iter != fitnessToChromosomeMap.end(); iter++) {
            float fitness = iter->first;
            vector<vector<int>> chromosomes = iter->second;

            sumFitness += fitness;
            if (chance < sumFitness) {
                int index = randomInteger(0, chromosomes.size() - 1);
                vector<int> selectedChromosome = chromosomes[index];
                selectedChromosomes.push_back(selectedChromosome);
                break;
            }
        }
    }

    return selectedChromosomes;
}

//THIS IS WRONG RIGHT NOW:
//DO THIS INSTEAD: https://mat.uab.cat/~alseda/MasterOpt/GeneticOperations.pdf
void GA::orderCrossover(vector<int>& firstChromo, vector<int>& secondChromo) {
    int firstSplicePoint = static_cast<int>(rand() % (chromosomeSize-2));
    int secondSplicePoint = static_cast<int>(rand() % (chromosomeSize-firstSplicePoint) + firstSplicePoint);
    cout << "" << endl;
    // for (int i = 0; i < chromosomeSize; i++) {
    //     if (splicePoint < i) {
    //         std::swap(firstChromo[i], secondChromo[i]);
    //     }
    // }
}

void GA::crossover(vector<vector<int>>& chromosomePool) {
    int halfwayIndex = static_cast<int>(chromosomeSize / 2);
    for (int i = 0; i < halfwayIndex; i++) {
        orderCrossover(chromosomePool[i], chromosomePool[i+halfwayIndex]);
    }
}

void GA::mutation(vector<int>& chromosome) {
    int firstPoint = static_cast<int>(rand() % (chromosome.size()));
    int secondPoint = static_cast<int>(rand() % (chromosome.size()));
    while (secondPoint == firstPoint) {
        secondPoint = static_cast<int>(rand() % (chromosome.size()));
    }

    std::swap(chromosome[firstPoint], chromosome[secondPoint]);
}

void GA::runAlgorithm(int iterations) {
    map<float, vector<vector<int>>> fitnessToChromosomeMap = calcRouletteFitness(*chromosomePool);

    vector<vector<int>> selectedChromosomes = selection(fitnessToChromosomeMap);
    crossover(selectedChromosomes);
    float chance = static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX);

    for (int i = 0; i < chromosomeSize; i++) {
        if (chance < mutationRate) {
            mutation(selectedChromosomes[i]);
        }
    }
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

int randomInteger(int min, int max) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(min, max);

    return dis(gen);
}

float randomFloat(float min, float max) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(min, max);

    return dis(gen);
}