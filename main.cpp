#include <iostream>
#include <map>

#include "Graph.h"
#include "CCExactSolver.h"

using namespace std;

int main(int argc, char *argv[])
{
    int verbose = 0;
    map<string, string> args;

    string prevArg = "";
    for (int i = 0; i < argc; i++)
    {
        if (string(argv[i]) == "-v")
        {
            verbose = 1;
            prevArg = "";
        }
        else if (string(argv[i]) == "-v2")
        {
            verbose = 2;
            prevArg = "";
        }
        else
        {
            if (prevArg != "" && prevArg[0] == '-')
            {
                args[Util::ReplaceAll(prevArg, "-", "")] = string(argv[i]);
            }

            prevArg = string(argv[i]);
        }
    }

    Graph* g = NULL;
    bool isBipartite = false;
    vector<int> bipartition_left;

    if (args.find("g") != args.end())
    {
        string str = Util::GetFileContent(args["g"]);
        g = Graph::FromString(str);
    }

    //>PACC -r 22  creates a random graph with 22 vertices
    if (args.find("r") != args.end())
    {
        int n = Util::ToInt(args["r"]);
        g = Graph::GetRandomWeightedGraph(n, -1, 1);
    }



    /*Graph* g = new Graph(3);

    g->AddWeightedEdge(0,1,1);
    g->AddWeightedEdge(0,2,1);
    g->AddWeightedEdge(1,2,-1);*/

    //GRAPH FROM A FIXD STRING
    //string s = "5\n0 1 -0.736924\n0 2 -0.0826997\n0 3 -0.562082\n0 4 0.357729\n1 2 0.869386\n1 3 0.0388327\n1 4 -0.930856\n2 3 0.0594004\n2 4 -0.984604\n3 4 -0.866316";
    //Graph* g = Graph::FromString(s);

    //RANDOM GRAPH
    //g = Graph::GetRandomWeightedGraph(10, -1, 1);


    //TEST BIPARTITE GRAPH
    g = new Graph(4);
    g->AddWeightedEdge(0,2,1);
    g->AddWeightedEdge(0,3,-1);
    g->AddWeightedEdge(1,2,-1);
    g->AddWeightedEdge(1,3,1);
    isBipartite = true;
    bipartition_left.push_back(0);
    bipartition_left.push_back(1);


    cout<<g->ToString()<<endl;

    cout<<"Starting"<<endl;
    CCExactSolver cc(g);

    if (isBipartite)
    {
        cc.SetBipartition(bipartition_left);
    }

    vector<vector<int>> parts;
    parts = cc.FindClusters();
    cout<<"End"<<endl;


    for (int p = 0; p < parts.size(); p++)
    {
        vector<int> part = parts[p];

        cout<<"CLUSTER"<<p + 1<<"=";
        for (int i = 0; i < part.size(); i++)
        {
            cout<<part[i]<<" ";
        }
        cout<<endl;
    }

    delete g;

    return 0;
}