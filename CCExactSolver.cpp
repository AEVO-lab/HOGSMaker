//
// Created by Manuel on 2018-11-16.
//

#include "CCExactSolver.h"


vector< vector<int> > CCExactSolver::FindClusters()
{
    InitSplitterPool();
    bitset<MAX_VERTICES> bitsall;
    for (int i = 0; i < graph->GetNbVertices(); i++)
    {
        bitsall[i] = 1;
    }

    FindClusters( bitsall);

    //construct the partition
    int addedSoFar = 0;
    vector< vector<int> > parts;

    bitset<MAX_VERTICES> curbits = bitsall;
    while (addedSoFar < graph->GetNbVertices())
    {
        pair<double, bitset<MAX_VERTICES>> p = costsTable[curbits];
        vector<int> v = BitsetToVector(p.second);    //p.second are the outsiders
        addedSoFar += v.size();
        parts.push_back(v);

        //proceed with the guys not in p.second
        bitset<MAX_VERTICES> nextbits = bitset<MAX_VERTICES>();
        for (int i = 0; i < graph->GetNbVertices(); i++)
        {
            if (curbits.test(i) && !p.second.test(i))
                nextbits[i] = 1;
        }
        curbits = nextbits;
    }

    //if (verbose >= 1)
    {
        cout<<"NB CALLS = "<<nbCalls<<endl;
    }


    return parts;
}

vector<int> CCExactSolver::BitsetToVector(bitset<MAX_VERTICES> bits)
{
    vector<int> v;
    for (int i = 0; i < bits.size(); i++)
    {
        if (bits.test(i))
            v.push_back(i);
    }
    return v;
}


void CCExactSolver::InitSplitterPool()
{
    for (int i = 0; i <= graph->GetNbVertices() + 1; i++)
    {
        CCExactSolver_Splitter* splitter;
        bitset<MAX_VERTICES> dummy;
        if (isBipartite)
        {
            splitter = new CCExactSolver_SplitterBipartite(graph, dummy, dummy, dummy);
        }
        else
        {
            splitter = new CCExactSolver_SplitterGeneric(graph, dummy);
        }

        splitterPool.push_back(splitter);
        splitterPoolInUse.push_back(false);
    }
}

CCExactSolver_Splitter* CCExactSolver::GetSplitterFromPool(int id_wanted)
{
    if (splitterPoolInUse[id_wanted])
    {
        cout<<"ERROR!  Splitter already in use.  Program is likely to crash soon."<<endl;
        return NULL;
    }

    splitterPoolInUse[id_wanted] = true;
    return splitterPool[id_wanted];
}

void CCExactSolver::ReleaseSplitter(int id)
{
    splitterPoolInUse[id] = false;
}


void CCExactSolver::FindClusters(bitset<MAX_VERTICES> bits)
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

    if (verbose >= 2)
    {
        cout << "START COMPUTING ";
        PrintBitSet(bits);
        cout << endl;
    }

    nbCalls++;

    //This implements an exponential DP algorithm where
    //d(X) = min_{Y \subset X} d(Y) + cost(X \ Y)
    int nbits = bits.count();
    CCExactSolver_Splitter* splitter;

    if (this->isBipartite)
    {
        if (useSplitterPool) {
            splitter = GetSplitterFromPool(nbits);
            ((CCExactSolver_SplitterBipartite *) splitter)->Reset(bits, bits_left, bits_right);
        }
        else
            splitter = new CCExactSolver_SplitterBipartite(graph, bits, bits_left, bits_right);
    }
    else
    {
        if (useSplitterPool) {
            splitter = GetSplitterFromPool(nbits);
            ((CCExactSolver_SplitterGeneric *) splitter)->Reset(bits);
        }
        else
            splitter = new CCExactSolver_SplitterGeneric(graph, bits);
    }


    CCExactSolver_Split split = splitter->GetNextValidSplit();

    while (!split.finished)
    {
        double costSubset = 0;

        bitset<MAX_VERTICES> Y_bits = split.Y_bits;
        bitset<MAX_VERTICES> outsiderbits = split.outsiderbits;

        if (outsiderbits.count() > 0)   //otherwise do nothing
        {
            if (Y_bits.count() > 0) {
                FindClusters(Y_bits); //table[sub] + cost(v - sub)
                pair<double, bitset<MAX_VERTICES>> p = costsTable[Y_bits];
                costSubset = p.first;
            }


            double costOutsider = 0;
            //compute the cost for putting the guys outside of subsetbits as a single cluster
            if (outsiderbits.count() == 0) {
                costOutsider = 99999999;
            } else {
                costOutsider = GetOutsiderCost(bits, outsiderbits);
            }


            double cost = costSubset + costOutsider;


            pair<double, bitset<MAX_VERTICES>> p_old = costsTable[bits];


            //DEBUG
            if (verbose >= 2) {
                cout << "bits=";
                PrintBitSet(bits);
                cout << "   Y=";
                PrintBitSet(Y_bits);
                cout << "  out=";
                PrintBitSet(outsiderbits);
                cout << "   costY = " << costSubset << "   costOut = " << costOutsider << endl;
            }

            if (cost < p_old.first) {
                pair<double, bitset<MAX_VERTICES>> p_new = make_pair(cost, outsiderbits);
                costsTable[bits] = p_new;
            }
        }

        split = splitter->GetNextValidSplit();
    }

    //DEBUG
    if (verbose >= 1) {
        if (bits.count() == graph->GetNbVertices()) {
            cout << "DONE" << endl;
            PrintBitSet(bits);
            cout << "  cost=" << costsTable[bits].first << "  outbits = ";
            bitset<MAX_VERTICES> outbits = costsTable[bits].second;
            PrintBitSet(outbits);
            cout << endl;
        }
    }


    //DEBUG
    if (verbose >= 2) {
        cout << "END COMPUTING ";
        PrintBitSet(bits);
        cout << endl;
    }

    if (useSplitterPool)
        ReleaseSplitter(nbits);
    else
        delete splitter;

}



void CCExactSolver::PrintBitSet(bitset<MAX_VERTICES> bits)
{
    for (int i = 0; i < bits.size(); i++)
    {
        cout<<(bits.test(i) ? "1" : "0")<<" ";
    }
}



CCExactSolver_Split CCExactSolver_SplitterGeneric::GetNextValidSplit()
{
    CCExactSolver_Split split;
    split.finished = false;

    int n = bits.count();
    if (counter > pow(2, n) - 1)
    {
        split.finished = true;
        return split;
    }

    //just partition the bits into Y and outsider.  The counter variable determines what Y is made of.
    pair<bitset<MAX_VERTICES>, bitset<MAX_VERTICES>> pair_of_bits;
    pair_of_bits = CCExactSolver_Splitter::CounterToBitsets(counter, bits);

    split.Y_bits = pair_of_bits.first;
    split.outsiderbits = pair_of_bits.second;

    counter++;

    return split;
}



double CCExactSolver::GetOutsiderCost(bitset<MAX_VERTICES> &bits, bitset<MAX_VERTICES> &outsiderbits)
{
    double costOutsider = 0;
    for (int v = 0; v < bits.size(); v++)
    {
        if (bits.test(v))
        {
            for (int v2 = v + 1; v2 < bits.size(); v2++) {
                if (outsiderbits.test(v) && outsiderbits.test(v2))  //v and v2 are both outside in X \ Y = pay if non-edge
                {
                    double w = graph->GetEdgeWeight(v, v2);
                    if (w < 0)
                        costOutsider += (-w);
                } else if (outsiderbits.test(v))     //v in outsider, v2 is not, ie v in X \Y and v2 in Y = pay if edge
                {
                    double w = graph->GetEdgeWeight(v, v2);
                    if (w > 0)
                        costOutsider += w;
                }
            }
        }
    }
    return costOutsider;
}



void CCExactSolver::SetBipartition(vector<int> left_vertices)
{
    this->isBipartite = true;

    bits_left = bitset<MAX_VERTICES>();
    bits_right = bitset<MAX_VERTICES>();

    for (int i = 0; i < left_vertices.size(); i++)
    {
        if (i < bits_left.size() && i < graph->GetNbVertices())
        {
            bits_left[i] = 1;
        }
    }

    for (int i = 0; i < graph->GetNbVertices(); i++)
    {
        if (!bits_left.test(i))
        {
            bits_right[i] = 1;
        }
    }


}



pair<bitset<MAX_VERTICES>, bitset<MAX_VERTICES>> CCExactSolver_Splitter::CounterToBitsets(unsigned long counter, bitset<MAX_VERTICES> &bits)
{
    pair<bitset<MAX_VERTICES>, bitset<MAX_VERTICES>> pair_of_bits;

    //translate counter as corresponding subset of bits
    bitset<MAX_VERTICES> counterbits = bitset<MAX_VERTICES>(counter);
    bitset<MAX_VERTICES> yes_bits;
    bitset<MAX_VERTICES> no_bits;  //outsider = X \ Y, determined by counter
    int cptbit = 0;

    //just partition the bits into Y and outsider.  The counter variable determines what Y is made of.
    for (int i = 0; i < bits.size(); i++)
    {
        if (bits.test(i))
        {
            if (counterbits.test(cptbit))
            {
                yes_bits[i] = 1;
            }
            else
            {
                no_bits[i] = 1;
            }
            cptbit++;
        }
    }

    return make_pair(yes_bits, no_bits);
}



CCExactSolver_SplitterBipartite::CCExactSolver_SplitterBipartite(Graph* g, bitset<MAX_VERTICES> &bits, bitset<MAX_VERTICES> &vertices_left, bitset<MAX_VERTICES> &vertices_right)
{
    graph = g;

    Reset(bits, vertices_left, vertices_right);
}


CCExactSolver_Split CCExactSolver_SplitterBipartite::GetNextValidSplit()
{
    CCExactSolver_Split split;
    split.finished = false;
    int n = bits_left.count();
    if (counter_left > pow(2, n) - 1)
    {
        split.finished = true;
        return split;
    }

    pair<bitset<MAX_VERTICES>, bitset<MAX_VERTICES>> pair_of_bits_right;
    pair_of_bits_right = CCExactSolver_Splitter::CounterToBitsets(counter_right, cur_maybe_right);
    //union of must be + selected
    bitset<MAX_VERTICES> insiderbits = pair_of_bits_right.first;
    bitset<MAX_VERTICES> outsiderbits = pair_of_bits_right.second;
    for (int i = 0; i < this->bits.size(); i++)
    {
        if (bits.test(i) && (cur_outsider_left.test(i) || cur_mustbe_right.test(i)))
        {
            outsiderbits[i] = 1;
        }
        if (bits.test(i) && !outsiderbits.test(i))
        {
            insiderbits[i] = 1;
        }
    }
    split.Y_bits = insiderbits;
    split.outsiderbits = outsiderbits;

    //We've gone through all possible guys on the right -> increase left counter and restart
    counter_right++;
    int m = cur_maybe_right.count();
    if (counter_right > pow(2, m) - 1)
    {
        counter_left++;
        ApplyNewCounterLeft();
    }


    return split;
}

void CCExactSolver_SplitterBipartite::ApplyNewCounterLeft()
{
    pair<bitset<MAX_VERTICES>, bitset<MAX_VERTICES>> pair_of_bits_left;
    pair_of_bits_left = CCExactSolver_Splitter::CounterToBitsets(counter_left, bits_left);
    cur_insider_left = pair_of_bits_left.first;
    cur_outsider_left = pair_of_bits_left.second;

    //SETUP MAYBES AND MUST BE
    cur_maybe_right = bits_right;
    cur_mustbe_right = bitset<MAX_VERTICES>();
    for (int r = 0; r < bits_right.size(); r++)
    {
        if (bits.test(r))
        {
            //sum of weights into left bits
            //if we save more cost by NOT adding r with the left outsiders, then we won't even try
            double w_pos_left_outsider = 0, w_neg_left_outsider = 0;
            double w_pos_left_insider = 0, w_neg_left_insider = 0;
            for (int l = 0; l < bits_left.size(); l++)
            {
                if (cur_outsider_left.test(l))
                {
                    double w = graph->GetEdgeWeight(l, r);
                    if (w < 0)
                        w_neg_left_outsider += (-w);
                    else
                        w_pos_left_outsider += w;
                }
                else if (cur_insider_left.test(l))
                {
                    double w = graph->GetEdgeWeight(l, r);
                    if (w < 0)
                        w_neg_left_insider += (-w);
                    else
                        w_pos_left_insider += w;
                }
            }

            //if adding r in the outsider clique costs more than isolating it, don't consider it in outsider
            if (w_neg_left_outsider > w_pos_left_outsider)
            {
                cur_maybe_right[r] = 0;
            }
            //if what we pay to have r not in outsider is greater than what we pay by having it in, then put it in
            else if (w_pos_left_outsider > w_pos_left_insider + w_neg_left_outsider)
            {
                cur_mustbe_right[r] = 1;
                cur_maybe_right[r] = 0;
            }
        }
    }






    counter_right = 0;
}


void CCExactSolver_SplitterGeneric::Reset(bitset<MAX_VERTICES> &bits)
{
    this->bits = bits;
    this->counter = 0;
}

void CCExactSolver_SplitterBipartite::Reset(bitset<MAX_VERTICES> &bits, bitset<MAX_VERTICES> &vertices_left, bitset<MAX_VERTICES> &vertices_right)
{
    bits_left = vertices_left;
    bits_right = vertices_right;
    this->bits = bits;
    counter_left = 0;
    counter_right = 0;
    for (int i = 0; i < bits_left.size(); i++)
    {
        if (!bits.test(i))
        {
            bits_left[i] = 0;
            bits_right[i] = 0;
        }
    }

    ApplyNewCounterLeft();
}