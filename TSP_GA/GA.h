#pragma once
#include <memory>
#include <vector>

class GA
{
private:
	std::unique_ptr< std::vector<std::vector<int>> > pool;
	int poolSize;
public:
	GA(int poolSize);
	~GA();
};

