#pragma once
#include <memory>
#include <vector>

class GA
{
private:
	std::unique_ptr< std::vector<std::vector<int>> > chromosomePool;
	
	int poolSize;
	int chromosomeSize;
public:
	GA(int poolSize, int chromosomeSize);
	~GA();
};

