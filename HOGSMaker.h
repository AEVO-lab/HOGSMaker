//
// Created by Manuel on 18/11/2018.
//

#ifndef HOGSMAKER_HOGSMAKER_H
#define HOGSMAKER_HOGSMAKER_H

#include <vector>
#include <map>
#include "TreeSource/trees/node.h"
#include "TreeSource/trees/treeiterator.h"
#include "TreeSource/trees/genespeciestreeutil.h"
#include "Graph.h"
#include "TreeSource/trees/newicklex.h"
#include "TreeSource/div/Timer.h"

#include "zlib/gzstream.h"

using namespace std;

class HOGSMaker
{
public:

    HOGSMaker();

    void InferHOGS(string omaDir, Node* customSpeciesTree = NULL);

    vector<string> GetGenesPerSpecies(Node* sp);

    Graph GetGraphForSpeciesPair(string s1name, string s2name, double minScore, string omaDir);

private:

    map<Node*, vector<string> > genesPerSpecies;

    int nextGeneID;

};


#endif //HOGSMAKER_HOGSMAKER_H
