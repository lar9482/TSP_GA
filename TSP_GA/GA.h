#pragma once
#include <memory>
#include <vector>
#include <string>
#include <map>

using std::unique_ptr;
using std::vector;
using std::string;
using std::map;

class GA
{
private:
	unique_ptr<vector<vector<int>> > chromosomePool;
	unique_ptr<vector<vector<int>> > adjacencyMatrix;
	int poolSize;
	int chromosomeSize;

	void initializeChromosomePool();
	void initializeAdjacencyMatrix(string adjacencyMatrixPath);
	float fitnessFunction(std::vector<int> const& chromosome);
	map<float, vector<vector<int>>> calcRouletteFitness(vector<vector<int>> const& chromosomePool);
	void singlePointCrossover(vector<int> const& firstChromo, vector<int> const& secondChromo);

	vector<vector<int>> selection(vector<vector<int>> const& chromosomePool);
	void crossover(vector<vector<int>> const& chromosomePool);

public:
	GA(int poolSize, std::string adjacencyMatrixPath);
	~GA();
	void runAlgorithm(int iterations);
	float bruteForce();
};

vector<vector<int>> generateAllPermutations(int n);
void generatePermutations(vector<int>& nums, int start, vector<vector<int>>& result);