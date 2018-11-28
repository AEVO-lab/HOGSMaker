//
// Created by Manuel on 2018-11-16.
//

#include "Graph.h"


Graph::Graph()
{
    isDirected = false;
    nbVertices = 0;
}

Graph::Graph(int nb_vertices) {
    nbVertices = 0;
    for (int i = 0; i < nb_vertices; i++) {
        AddVertex();
    }

    isDirected = false;
}

Graph::~Graph()
{
    for (int i = 0; i < this->GetNbVertices(); i++) {
        delete neighborhoods[i];
    }
}

int Graph::GetNbVertices() {
    return nbVertices;
}

int Graph::GetNbEdges(){
    return edges.size();
}


void Graph::AddVertex()
{
    neighborhoods.push_back(new vector<int>());
    nbVertices++;
}

void Graph::AddVertex(string name)
{
    AddVertex();
    SetVertexName(GetNbVertices() - 1, name);
}


void Graph::AddWeightedEdge(int i, int j, double w) {
    //TODO: error checking
    Edge e = make_pair(i, j);
    edges.insert(e);
    edge_weights[e] = w;
    neighborhoods[i]->push_back(j);

    if (!isDirected)
    {
        Edge e2 = make_pair(j, i);
        edges.insert(e2);
        edge_weights[e2] = w;
        neighborhoods[j]->push_back(i);
    }
}

void Graph::AddWeightedEdge(string v1, string v2, double w) {
    //TODO: error checking
    AddWeightedEdge(vertexNames[v1], vertexNames[v2], w);
}


double Graph::GetEdgeWeight(int i, int j) {
    //TODO: error checking
    Edge e = make_pair(i, j);
    return edge_weights[e];
}

double Graph::GetEdgeWeight(string s1, string s2) {
    return GetEdgeWeight(vertexNames[s1], vertexNames[s2]);
}


Graph *Graph::GetRandomWeightedGraph(int n, double minw, double maxw) {
    Graph *g = new Graph(n);

    std::uniform_real_distribution<double> r(minw, maxw);
    std::default_random_engine re;

    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            double w = r(re);
            g->AddWeightedEdge(i, j, w);
        }
    }

    return g;
}


Graph *Graph::GetRandomBinaryWeightedGraph(int n1, int n2, double minw, double maxw) {
    Graph *g = new Graph(n1 + n2);

    std::uniform_real_distribution<double> r(minw, maxw);
    std::default_random_engine re;

    for (int i = 0; i < n1; i++) {
        for (int j = 0; j < n2; j++) {
            double w = r(re);
            g->AddWeightedEdge(i, n1 + j, w);
        }
    }

    return g;
}


string Graph::ToString() {
    string s = Util::ToString(this->GetNbVertices()) + "\n";
    for (int i = 0; i < this->GetNbVertices(); i++) {
        for (int j = i + 1; j < this->GetNbVertices(); j++) {
            double w = this->GetEdgeWeight(i, j);
            if (w != 0) {
                s += Util::ToString(i) + " " + Util::ToString(j) + " " + Util::ToString(w) + "\n";
            }
        }
    }
    return s;
}

Graph *Graph::FromString(string s, string edge_separator) {
    //TODO; error checking
    vector<string> lines = Util::Split(s, edge_separator, false);

    Graph *g = new Graph(Util::ToInt(lines[0]));

    for (int l = 1; l < lines.size(); l++) {
        string line = lines[l];

        vector<string> pz = Util::Split(line, " ");
        g->AddWeightedEdge(Util::ToInt(pz[0]), Util::ToInt(pz[1]), Util::ToDouble(pz[2]));
    }

    return g;
}


vector<vector<int> > Graph::GetConnectedComponents()
{
    vector<vector<int> > components;
    //a classic
    set<int> visited;
    queue<int> to_visit;

    int next_to_visit = 0;

    while (visited.size() < GetNbVertices())
    {
        while (next_to_visit < GetNbVertices() && visited.find(next_to_visit) != visited.end())
            next_to_visit++;


        to_visit.push(next_to_visit);
        vector<int> cur_component;
        while (to_visit.size() > 0)
        {
            int nextguy = to_visit.front();
            to_visit.pop();
            cur_component.push_back(nextguy);

            for (int i = 0; i < neighborhoods[nextguy]->size(); i++)
            {
                int j = neighborhoods[nextguy]->at(i);
                if (visited.find(j) == visited.end())
                {
                    visited.insert(j);
                    to_visit.push(j);
                    cur_component.push_back(j);
                }
            }
        }
        components.push_back(cur_component);
    }

    return components;

}

bool Graph::IsEdge(int i, int j)
{
    Edge e = make_pair(i, j);
    return (edges.find(e) != edges.end());
}

bool Graph::IsEdge(string v1, string v2)
{
    return IsEdge(vertexNames[v1], vertexNames[v2]);
}

void Graph::SetVertexName(int i, string name)
{
    vertexNames[name] = i;
}

int Graph::GetVertexByName(string name)
{
    return vertexNames[name];
}


bool Graph::HasVertex(string name)
{
    return (vertexNames.find(name) != vertexNames.end());
}

Graph* Graph::GetInducedSubgraph(vector<int> vertices)
{
    Graph* g = new Graph(vertices.size());

    map<int, int> corresp;

    for (int i = 0; i < vertices.size(); i++)
    {
        corresp[vertices[i]] = i;
    }

    for (int i = 0; i < vertices.size(); i++)
    {
        int xi = corresp[vertices[i]];
        for (int j = i + 1; j < vertices.size(); j++)
        {
            int xj = corresp[vertices[j]];
            if (IsEdge(i, j))
            {
                g->AddWeightedEdge(xi, xj, g->GetEdgeWeight(i, j));
            }
        }
    }

    return g;


}