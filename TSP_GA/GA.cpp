#pragma once

#include "GA.h"
#include <string>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <random>
#include <cstdlib>
#include <limits>

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

//BUG!!!:
//This fitness routine will favor fitness values that are higher, which is NOT what I want.
map<float, vector<vector<int>>> GA::calcRouletteFitness(vector<vector<int>> const& chromosomePool) {
    map<float, vector<vector<int>>> fitnessToChromosomeMap;
    float sumOfAllFitnesses = 0;

    for (int i = 0; i < chromosomePool.size(); i++) {
        vector<int> chromosome(chromosomePool[i]);
        float fitness = fitnessFunction(chromosome);
        float invertedFitness = 1/fitness;
        fitnessToChromosomeMap[invertedFitness].push_back(chromosome);

        // fitness was not found as a key in 'fitnessToChromosomeMap'
        if (fitnessToChromosomeMap.find(invertedFitness) != fitnessToChromosomeMap.end()) {
            sumOfAllFitnesses += invertedFitness;
        }
    }

    map<float, vector<vector<int>>> rouletteFitToChromosomeMap;
    for (auto iter = fitnessToChromosomeMap.begin(); iter != fitnessToChromosomeMap.end(); iter++) {
        float fitness = iter->first;
        
        vector<vector<int>> chromosomes = iter->second;
        float rouletteFitness = (fitness / sumOfAllFitnesses);

        rouletteFitToChromosomeMap[rouletteFitness] = chromosomes;
    }

    return rouletteFitToChromosomeMap;
}

vector<vector<int>> GA::selection(map<float, vector<vector<int>>> const&  fitnessToChromosomeMap) {
    vector<vector<int>> selectedChromosomes;

    while(selectedChromosomes.size() < poolSize) {
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
vector<int> GA::orderCrossover(vector<int>& firstChromo, vector<int>& secondChromo) {
    int firstSplicePoint = randomInteger(0, chromosomeSize - 2);
    int secondSplicePoint = randomInteger(firstSplicePoint, chromosomeSize - 1);
    
    vector<int> firstChromoSubstring;
    vector<int> secondChromoSubstring;
    for (int i = firstSplicePoint; i <= secondSplicePoint; i++) {
        firstChromoSubstring.push_back(
            firstChromo[i]
        );
    }
    for (int i = 0; i < chromosomeSize; i++) {
        if (!numIsInIntVector(secondChromo[i], firstChromoSubstring)) {
            secondChromoSubstring.push_back(
                secondChromo[i]
            );
        }
    }

    vector<int> newChromosome;
    int firstChromoSubstringIndex = 0;
    int secondChromoSubstringIndex = 0;
    for (int i = 0; i < chromosomeSize; i++) {
        if (i >= firstSplicePoint && i <= secondSplicePoint) {
            newChromosome.push_back(
                firstChromoSubstring[firstChromoSubstringIndex]
            );
            firstChromoSubstringIndex++;
        }
        else {
            newChromosome.push_back(
                secondChromoSubstring[secondChromoSubstringIndex]
            );
            secondChromoSubstringIndex++;
        }
    }
    
    return newChromosome;
}

vector<vector<int>> GA::getEliteChromosomes(map<float, vector<vector<int>>> const& fitnessToChromosomeMap) {
    auto bestFitnessIter = fitnessToChromosomeMap.rbegin();
    vector<vector<int>> fittestChromosomes = bestFitnessIter->second;
    vector<vector<int>> eliteChromosomesToReturn;

    if (fittestChromosomes.size() >= 2) {
        eliteChromosomesToReturn.push_back(fittestChromosomes[0]);
        eliteChromosomesToReturn.push_back(fittestChromosomes[1]);
        return eliteChromosomesToReturn;
    }
    else {
        bestFitnessIter++;
        vector<vector<int>> secondFittestChromosomes = bestFitnessIter->second;
        eliteChromosomesToReturn.push_back(fittestChromosomes[0]);
        eliteChromosomesToReturn.push_back(secondFittestChromosomes[0]);
        
        return eliteChromosomesToReturn;
    }
}

void GA::crossover(vector<vector<int>>& chromosomePool, map<float, vector<vector<int>>> const& fitnessToChromosomeMap) {
    int halfwayIndex = static_cast<int>(poolSize / 2);
    for (int i = 0; i < halfwayIndex; i++) {
        if (i == 0) {
            vector<vector<int>> eliteChromosomes = getEliteChromosomes(fitnessToChromosomeMap);
            vector<int> firstChild = eliteChromosomes[0];
            vector<int> secondChild = eliteChromosomes[1];
            chromosomePool[i] = firstChild;
            chromosomePool[i + halfwayIndex] = secondChild;
        }
        else {
            vector<int> firstChild = orderCrossover(chromosomePool[i], chromosomePool[i + halfwayIndex]);
            vector<int> secondChild = orderCrossover(chromosomePool[i], chromosomePool[i + halfwayIndex]);
            chromosomePool[i] = firstChild;
            chromosomePool[i + halfwayIndex] = secondChild;
        }
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

float GA::getBestFitnessFromPool(vector<vector<int>> const& chromosomePool) {
    float minimumFitness = std::numeric_limits<float>::max();
    for (int i = 0; i < chromosomePool.size(); i++) {
        float fitness = fitnessFunction(chromosomePool[i]);
        if (fitness < minimumFitness) {
            minimumFitness = fitness;
        }
    }
    return minimumFitness;
}

void GA::printBestChromosome(vector<vector<int>> const& chromosomePool) {
    float minimumFitness = std::numeric_limits<float>::max();
    vector<int> bestChromosome;
    for (int i = 0; i < chromosomePool.size(); i++) {
        float fitness = fitnessFunction(chromosomePool[i]);
        if (fitness < minimumFitness) {
            minimumFitness = fitness;
            bestChromosome = chromosomePool[i];
        }
    }

    for (int i = 0; i < bestChromosome.size(); i++) {
        cout << bestChromosome[i] << "->";
    }
    cout << "\n" << endl;
}

void GA::runAlgorithm(int iterations) {
    for (int iteration = 0; iteration < iterations; iteration++) {
        map<float, vector<vector<int>>> fitnessToChromosomeMap = calcRouletteFitness(*chromosomePool);

        vector<vector<int>> newChromosomePool = selection(fitnessToChromosomeMap);
        crossover(newChromosomePool, fitnessToChromosomeMap);

        for (int i = 0; i < poolSize; i++) {
            float chance = static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX);
            if (chance < mutationRate) {
                mutation(newChromosomePool[i]);
            }
        }

        for (int i = 0; i < poolSize; i++) {
            chromosomePool->at(i) = newChromosomePool[i];
        }
        
        cout << getBestFitnessFromPool(*chromosomePool) << endl;
        if (getBestFitnessFromPool(*chromosomePool) <= 270) {
            cout << getBestFitnessFromPool(*chromosomePool) << endl;
        }
    }
    
    printBestChromosome(*chromosomePool);
}

float GA::bruteForce() {
    vector<vector<int>> allSolutions = generateAllPermutations(chromosomeSize);
    printBestChromosome(allSolutions);
    return getBestFitnessFromPool(allSolutions);
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

    return static_cast<float>(dis(gen));
}

bool numIsInIntVector(int num, vector<int> const& vec) {
    auto iter = std::find(vec.begin(), vec.end(), num);
    return (iter != vec.end());
}