//
// Created by Manuel on 2018-11-16.
//

#ifndef PACCLUSTERING_CCEXACTSOLVER_H
#define PACCLUSTERING_CCEXACTSOLVER_H

#define MAX_VERTICES 32

#include <iostream>
#include "Graph.h"
#include <cmath>
#include <vector>
#include <bitset>
#include <unordered_map>



class CCExactSolver {
public:

    vector< vector<int> > FindClusters(Graph *g);
    void FindClusters(Graph* g, bitset<MAX_VERTICES> subset);

    //TODO: put in some util class
    void PrintBitSet(bitset<MAX_VERTICES> bits);



private:

    vector<int> BitsetToVector(Graph* g, bitset<MAX_VERTICES> bits);

    //key = integer corresponding to subset
    //value = pair.first is the cost, pair.second is the subset that led to optimum
    unordered_map<bitset<MAX_VERTICES>, pair<double, bitset<MAX_VERTICES>> > costsTable;

};


#endif //PACCLUSTERING_CCEXACTSOLVER_H
