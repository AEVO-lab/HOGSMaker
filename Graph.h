//
// Created by Manuel on 2018-11-16.
//

#ifndef PACCLUSTERING_GRAPH_H
#define PACCLUSTERING_GRAPH_H


#include <map>
#include <set>
#include <random>
#include <queue>
#include "TreeSource/div/util.h"

using namespace std;


typedef pair<int, int> Edge;

class Graph
{
public:

    Graph();
    Graph(int nb_vertices);

    ~Graph();


    void AddVertex();

    void AddVertex(string name);

    void AddWeightedEdge(int i, int j, double w);

    void AddWeightedEdge(string v1, string v2, double w);

    double GetEdgeWeight(int i, int j);
    double GetEdgeWeight(string v1, string v2);

    int GetNbVertices();
    int GetNbEdges();

    bool HasVertex(string name);

    bool IsEdge(int i, int j);
    bool IsEdge(string v1, string v2);

    void SetVertexName(int i, string name);
    int GetVertexByName(string name);

    string ToString();

    vector< vector<int> > GetConnectedComponents();

    Graph* GetInducedSubgraph(vector<int> vertices);

    static Graph* GetRandomWeightedGraph(int n, double minw, double maxw);

    static Graph* GetRandomBinaryWeightedGraph(int n1, int n2, double minw, double maxw);

    static Graph* FromString(string s, string edge_separator = "\n");

private:
    int nbVertices;
    map<string, int> vertexNames;

    set<Edge> edges;
    vector< vector<int>* > neighborhoods;
    map<Edge, double> edge_weights;

    bool isDirected;
};


#endif //PACCLUSTERING_GRAPH_H
