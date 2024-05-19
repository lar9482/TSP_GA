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

public:
	GA(int poolSize, int chromosomeSize, std::string adjacencyMatrixPath);
	~GA();
};

