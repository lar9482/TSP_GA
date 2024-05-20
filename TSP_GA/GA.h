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
	float mutationRate;

	void initializeChromosomePool();
	void initializeAdjacencyMatrix(string adjacencyMatrixPath);
	float fitnessFunction(std::vector<int> const& chromosome);
	map<float, vector<vector<int>>> calcRouletteFitness(vector<vector<int>> const& chromosomePool);
	vector<int> orderCrossover(vector<int>& firstChromo, vector<int>& secondChromo);
	vector<vector<int>> getEliteChromosomes(map<float, vector<vector<int>>> const& fitnessToChromosomeMap);

	vector<vector<int>> selection(map<float, vector<vector<int>>> const& fitnessToChromosomeMap);
	void crossover(vector<vector<int>>& chromosomePool, map<float, vector<vector<int>>> const& fitnessToChromosomeMap);
	void mutation(vector<int>& chromosome);
	
	float getBestFitnessFromPool(vector<vector<int>> const& chromosomePool);
public:
	GA(int poolSize, float mutationRate, std::string adjacencyMatrixPath);
	~GA();
	void runAlgorithm(int iterations);
	float bruteForce();
};

vector<vector<int>> generateAllPermutations(int n);
void generatePermutations(vector<int>& nums, int start, vector<vector<int>>& result);
int randomInteger(int min, int max);
float randomFloat(float min, float max);
bool numIsInIntVector(int num, vector<int> const& vec);