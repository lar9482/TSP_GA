#include "GA.h"

GA::GA(int poolSize, int chromosomeSize):
poolSize(poolSize), 
chromosomeSize(chromosomeSize), 
chromosomePool(std::make_unique<std::vector<std::vector<int>>>()) {
    for (int i = 0; i < poolSize; i++) {
        std::vector<int> chromosome;

        for (int j = 0; j < chromosomeSize; j++) {
            chromosome.push_back(j);
        }
        chromosomePool->push_back(chromosome);
    }
}

GA::~GA() {

}