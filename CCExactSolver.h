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
#include "TreeSource/div/Timer.h"

//for internal use only
class CCExactSolver_Split
{
public:
    bitset<MAX_VERTICES> Y_bits;
    bitset<MAX_VERTICES> outsiderbits;
    bool finished;
};


class CCExactSolver_Splitter
{
public:

    /**
     * Partitions the true bits of bits according to counter.  Each bit of counter represents a true position of bits.
     * @param counter A counter between 0 and 2^{n+1}-1, where n is the number of 1 bits in bits
     * @param bits A bit vector, each 1 representing membership
     * @return A first bit vector where the i-th bit true bit of bits is 1 iff the i-th bit of counter is 1, the other is the complement w.r.t. bits
     */
    static pair< bitset<MAX_VERTICES>, bitset<MAX_VERTICES> > CounterToBitsets(unsigned long counter, bitset<MAX_VERTICES> &bits);
    virtual CCExactSolver_Split GetNextValidSplit() = 0;
};


class CCExactSolver_SplitterGeneric : public CCExactSolver_Splitter
{
public:

    CCExactSolver_SplitterGeneric(Graph* g, bitset<MAX_VERTICES> &bits)
    {
        graph = g;
        counter = 0;
        this->bits = bits;
    }

    CCExactSolver_Split GetNextValidSplit();
    void Reset(bitset<MAX_VERTICES> &bits);

private:
    Graph* graph;
    unsigned long counter;
    bitset<MAX_VERTICES> bits;

};

//for internal use only
class CCExactSolver_SplitterBipartite : public CCExactSolver_Splitter
{
public:

    CCExactSolver_SplitterBipartite(Graph* g, bitset<MAX_VERTICES> &bits, bitset<MAX_VERTICES> &vertices_left, bitset<MAX_VERTICES> &vertices_right);

    CCExactSolver_Split GetNextValidSplit();
    void Reset(bitset<MAX_VERTICES> &bits, bitset<MAX_VERTICES> &vertices_left, bitset<MAX_VERTICES> &vertices_right);

private:
    Graph* graph;
    bitset<MAX_VERTICES> bits;
    unsigned long counter_left;
    unsigned long counter_right;
    bitset<MAX_VERTICES> bits_left;
    bitset<MAX_VERTICES> bits_right;

    void ApplyNewCounterLeft();

    //this guy maintains the current outsider on the left side, then iterates
    //the guys on the right side.  Some guys on the right MUST be with outsider,
    //and some CANNOT.  We remember those who must or could (the maybe's) be tehre until we move on to the next outsider.
    bitset<MAX_VERTICES> cur_outsider_left;
    bitset<MAX_VERTICES> cur_insider_left;
    bitset<MAX_VERTICES> cur_mustbe_right;
    bitset<MAX_VERTICES> cur_maybe_right;
};



class CCExactSolver {
public:

    int verbose;

    CCExactSolver(Graph *g)
    {
        this->graph = g;
        isBipartite = false;
        verbose = 0;
        nbCalls = 0;
        useSplitterPool = true;
    }

    ~CCExactSolver()
    {
        for (int i = 0; i < splitterPool.size(); i++)
        {
            delete splitterPool[i];
        }
    }

    vector< vector<int> > FindClusters();
    void FindClusters(bitset<MAX_VERTICES> subset);

    //TODO: put in some util class
    void PrintBitSet(bitset<MAX_VERTICES> bits);

    void SetBipartition(vector<int> left_vertices);

private:

    vector<int> BitsetToVector(bitset<MAX_VERTICES> bits);

    CCExactSolver_Split GetNextValidSplit(unsigned long counter, bitset<MAX_VERTICES> &bits);

    double GetOutsiderCost(bitset<MAX_VERTICES> &bits, bitset<MAX_VERTICES> &outsiderbits);

    //key = integer corresponding to subset
    //value = pair.first is the cost, pair.second is the subset that led to optimum
    unordered_map<bitset<MAX_VERTICES>, pair<double, bitset<MAX_VERTICES>> > costsTable;

    Graph* graph;

    bool isBipartite;
    bitset<MAX_VERTICES> bits_left;
    bitset<MAX_VERTICES> bits_right;

    int nbCalls;

    vector< CCExactSolver_Splitter* > splitterPool;
    vector< bool > splitterPoolInUse;
    void InitSplitterPool();
    CCExactSolver_Splitter* GetSplitterFromPool(int id_wanted);
    void ReleaseSplitter(int id);
    bool useSplitterPool;
};


#endif //PACCLUSTERING_CCEXACTSOLVER_H
