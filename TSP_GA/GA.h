#pragma once
#include <memory>
#include <vector>
#include <string>

class GA
{
private:
	std::unique_ptr< std::vector<std::vector<int>> > chromosomePool;
	std::unique_ptr< std::vector<std::vector<int>> > adjacencyMatrix;
	int poolSize;
	int chromosomeSize;
	void initializeChromosomePool();
	void initializeAdjacencyMatrix(std::string adjacencyMatrixPath);
	int fitnessFunction(std::vector<int> const& chromosome);

public:
	GA(int poolSize, std::string adjacencyMatrixPath);
	~GA();
	void runAlgorithm();
	int bruteForce();
};

std::vector<std::vector<int>> generateAllPermutations(int n);
void generatePermutations(std::vector<int>& nums, int start, std::vector<std::vector<int>>& result);