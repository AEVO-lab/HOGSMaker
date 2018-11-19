//
// Created by Manuel on 2018-11-16.
//

#ifndef PACCLUSTERING_GRAPH_H
#define PACCLUSTERING_GRAPH_H


#include <map>
#include <set>
#include <random>
#include "TreeSource/div/util.h"

using namespace std;


typedef pair<int, int> Edge;

class Graph
{
public:
    Graph(int nb_vertices);



    void AddWeightedEdge(int i, int j, double w);

    void AddWeightedEdge(string v1, string v2, double w);

    double GetEdgeWeight(int i, int j);

    int GetNbVertices();


    string ToString();

    static Graph* GetRandomWeightedGraph(int n, double minw, double maxw);
    static Graph* FromString(string s);

private:
    int nbVertices;
    map<string, int> vertexNames;

    set<Edge> edges;
    map<Edge, double> edge_weights;
};


#endif //PACCLUSTERING_GRAPH_H
