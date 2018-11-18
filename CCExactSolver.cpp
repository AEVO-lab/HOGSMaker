//
// Created by Manuel on 2018-11-16.
//

#include "CCExactSolver.h"


vector< vector<int> > CCExactSolver::FindClusters(Graph *g)
{
    bitset<MAX_VERTICES> bitsall;
    for (int i = 0; i < g->GetNbVertices(); i++)
    {
        bitsall[i] = 1;
    }

    FindClusters(g, bitsall);

    //construct the partition
    int addedSoFar = 0;
    vector< vector<int> > parts;

    bitset<MAX_VERTICES> curbits = bitsall;
    while (addedSoFar < g->GetNbVertices())
    {
        pair<double, bitset<MAX_VERTICES>> p = costsTable[curbits];
        vector<int> v = BitsetToVector(g, p.second);    //p.second are the outsiders
        addedSoFar += v.size();
        parts.push_back(v);

        //proceed with the guys not in p.second
        bitset<MAX_VERTICES> nextbits = bitset<MAX_VERTICES>();
        for (int i = 0; i < g->GetNbVertices(); i++)
        {
            if (curbits.test(i) && !p.second.test(i))
                nextbits[i] = 1;
        }
        curbits = nextbits;
    }

    return parts;
}

vector<int> CCExactSolver::BitsetToVector(Graph* g, bitset<MAX_VERTICES> bits)
{
    vector<int> v;
    for (int i = 0; i < bits.size(); i++)
    {
        if (bits.test(i))
            v.push_back(i);
    }
    return v;
}


void CCExactSolver::FindClusters(Graph *g, bitset<MAX_VERTICES> bits)
{
    if (bits.count() == 0 || costsTable.find(bits) != costsTable.end())
    {
        return;
    }
    else if (bits.count() == 1 && costsTable.find(bits) == costsTable.end())
    {
        costsTable[bits] = make_pair(0, bitset<MAX_VERTICES>());
    }
    costsTable[bits] = make_pair(9999999,bitset<MAX_VERTICES>());


    //This implements an exponential DP algorithm where
    //d(X) = min_{Y \subset X} d(Y) + cost(X \ Y)

    int n = bits.count();

    unsigned long counter = 0;

    //counter kind of enumerates every subset
    while (counter <= pow(2, n + 1) - 1)
    {
        //translate counter as corresponding subset of bits
        bitset<MAX_VERTICES> counterbits = bitset<MAX_VERTICES>(counter);
        bitset<MAX_VERTICES> Y_bits;
        bitset<MAX_VERTICES> outsiderbits;  //outsider = X \ Y
        int cptbit = 0;

        //just partition the bits into Y and outsider.  The counter variable determines what Y is made of.
        for (int i = 0; i < g->GetNbVertices(); i++)
        {
            Y_bits[i] = 0;
            if (bits.test(i))
            {
                if (counterbits.test(cptbit))
                {
                    Y_bits[i] = 1;
                }
                else
                {
                    outsiderbits[i] = 1;
                }
                cptbit++;
            }
        }

        double costSubset = 0;

        if (Y_bits.count() > 0)
        {
            FindClusters(g, Y_bits); //table[sub] + cost(v - sub)
            pair<double, bitset<MAX_VERTICES>> p = costsTable[Y_bits];
            costSubset = p.first;
        }


        double costOutsider = 0;

        //compute the cost for putting the guys outside of subsetbits as a single cluster
        if (outsiderbits.count() == 0)
        {
            costOutsider = 99999999;
        }
        else {
            for (int v = 0; v < bits.size(); v++) {
                if (bits.test(v))
                {
                    for (int v2 = v + 1; v2 < bits.size(); v2++) {
                        if (outsiderbits.test(v) && outsiderbits.test(v2))  //v and v2 are both outside in X \ Y = pay if non-edge
                        {
                            double w = g->GetEdgeWeight(v, v2);
                            if (w < 0)
                                costOutsider += (-w);
                        } else if (outsiderbits.test(v))     //v in outsider, v2 is not, ie v in X \Y and v2 in Y = pay if edge
                        {
                            double w = g->GetEdgeWeight(v, v2);
                            if (w > 0)
                                costOutsider += w;
                        }
                    }
                }
            }
        }


        double cost = costSubset + costOutsider;


        pair<double, bitset<MAX_VERTICES>> p_old = costsTable[bits];


        //DEBUG
        /*cout<<"bits=";
        PrintBitSet(bits);
        cout<<"   Y=";
        PrintBitSet(Y_bits);
        cout<<"  out=";
        PrintBitSet(outsiderbits);
        cout<<"   costY = "<<costSubset<<"   coutOut = "<<costOutsider<<endl;*/

        if (cost < p_old.first)
        {
            pair<double, bitset<MAX_VERTICES>> p_new = make_pair(cost, outsiderbits);
            costsTable[bits] = p_new;
        }

        counter++;
    }

    //DEBUG
    if (bits.count() == g->GetNbVertices()) {
        PrintBitSet(bits);
        cout << "  cost=" << costsTable[bits].first << "  outbits = ";
        bitset<MAX_VERTICES> outbits = costsTable[bits].second;
        PrintBitSet(outbits);
        cout << endl;
    }


}



void CCExactSolver::PrintBitSet(bitset<MAX_VERTICES> bits)
{
    for (int i = 0; i < bits.size(); i++)
    {
        cout<<(bits.test(i) ? "1" : "0")<<" ";
    }
}


