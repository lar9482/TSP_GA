#include "GA.h"

GA::GA(int poolSize):
poolSize(poolSize), pool(std::make_unique<std::vector<std::vector<int>>>()){
    pool->push_back(std::vector<int>{1, 2, 3});
    pool->push_back(std::vector<int>{4, 5, 6});
    pool->push_back(std::vector<int>{7, 8, 9});
}

GA::~GA() {

}