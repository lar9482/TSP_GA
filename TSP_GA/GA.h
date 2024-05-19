#pragma once
#include <memory>
#include <vector>
#include <string>
#include <map>

class GA
{
private:
	std::unique_ptr< std::vector<std::vector<int>> > chromosomePool;
	std::unique_ptr< std::vector<std::vector<int>> > adjacencyMatrix;
	int poolSize;
	int chromosomeSize;

	void initializeChromosomePool();
	void initializeAdjacencyMatrix(std::string adjacencyMatrixPath);
	float fitnessFunction(std::vector<int> const& chromosome);
	std::map<float, std::vector<std::vector<int>>> calcRouletteFitness();

	std::vector<std::vector<int>> selection();

public:
	GA(int poolSize, std::string adjacencyMatrixPath);
	~GA();
	void runAlgorithm(int iterations);
	float bruteForce();
};

std::vector<std::vector<int>> generateAllPermutations(int n);
void generatePermutations(std::vector<int>& nums, int start, std::vector<std::vector<int>>& result);