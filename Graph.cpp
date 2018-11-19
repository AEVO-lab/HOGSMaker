//
// Created by Manuel on 2018-11-16.
//

#include "Graph.h"


Graph::Graph(int nb_vertices)
{
    nbVertices = nb_vertices;
}


int Graph::GetNbVertices()
{
    return nbVertices;
}

void Graph::AddWeightedEdge(int i, int j, double w)
{
    //TODO: error checking
    Edge e = make_pair(i, j);
    edges.insert(e);
    edge_weights[e] = w;
}

void Graph::AddWeightedEdge(string v1, string v2, double w)
{
    //TODO: error checking
    AddWeightedEdge(vertexNames[v1], vertexNames[v2], w);
}


double Graph::GetEdgeWeight(int i, int j)
{
    //TODO: error checking
    Edge e = make_pair(i, j);
    return edge_weights[e];
}



Graph* Graph::GetRandomWeightedGraph(int n, double minw, double maxw)
{
    Graph* g = new Graph(n);

    std::uniform_real_distribution<double> r(minw, maxw);
    std::default_random_engine re;

    for (int i = 0; i < n; i++)
    {
        for (int j = i + 1; j < n; j++)
        {
            double w = r(re);
            g->AddWeightedEdge(i, j, w);
        }
    }

    return g;
}

string Graph::ToString()
{
    string s = Util::ToString(this->GetNbVertices()) + "\n";
    for (int i = 0; i < this->GetNbVertices(); i++)
    {
        for (int j = i + 1; j < this->GetNbVertices(); j++)
        {
            s += Util::ToString(i) + " " + Util::ToString(j) + " " + Util::ToString(this->GetEdgeWeight(i, j)) + "\n";
        }
    }
    return s;
}

Graph* Graph::FromString(string s)
{
    //TODO; error checking
    vector<string> lines = Util::Split(s, "\n", false);

    Graph* g = new Graph(Util::ToInt(lines[0]));

    for (int l = 1; l < lines.size(); l++)
    {
        string line = lines[l];

        vector<string> pz = Util::Split(line, " ");
        g->AddWeightedEdge(Util::ToInt(pz[0]), Util::ToInt(pz[1]), Util::ToDouble(pz[2]));
    }

    return g;
}