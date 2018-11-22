#include <iostream>
#include <map>
#include <chrono>

#include "Graph.h"
#include "CCExactSolver.h"

using namespace std;

void DoSomeUnitTests(); //just a forward declaration passing by
void DoSomePerformanceTests();
void PrintPartition(vector<vector<int>> parts);

int main(int argc, char *argv[])
{
    //DoSomeUnitTests();   return 0;
    DoSomePerformanceTests();   return 0;

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
    /*g = new Graph(4);
    g->AddWeightedEdge(0,2,1);
    g->AddWeightedEdge(0,3,-1);
    g->AddWeightedEdge(1,2,-1);
    g->AddWeightedEdge(1,3,1);
    isBipartite = true;
    bipartition_left.push_back(0);
    bipartition_left.push_back(1);*/

    g = Graph::GetRandomBinaryWeightedGraph(5, 5, -1, 1);
    isBipartite = true;
    bipartition_left = Util::GetVectorOfIntegers(0, 4);


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



void DoSomeUnitTests()
{
    vector<int> bip;

    Graph* g = new Graph(8);
    int i = 0;
    g->AddWeightedEdge(i,4,1);
    g->AddWeightedEdge(i,5,-1);
    g->AddWeightedEdge(i,6,-1);
    g->AddWeightedEdge(i,7,-1);
    i++;
    g->AddWeightedEdge(i,4,-1);
    g->AddWeightedEdge(i,5,1);
    g->AddWeightedEdge(i,6,-1);
    g->AddWeightedEdge(i,7,-1);
    i++;
    g->AddWeightedEdge(i,4,-1);
    g->AddWeightedEdge(i,5,-1);
    g->AddWeightedEdge(i,6,1);
    g->AddWeightedEdge(i,7,-1);
    i++;
    g->AddWeightedEdge(i,4,-1);
    g->AddWeightedEdge(i,5,-1);
    g->AddWeightedEdge(i,6,1);
    g->AddWeightedEdge(i,7,-1);

    cout<<"Starting test 1"<<endl;
    CCExactSolver cc(g);
    bip.push_back(0);bip.push_back(1);bip.push_back(2);bip.push_back(3);
    cc.SetBipartition(bip);

    vector<vector<int>> parts;
    parts = cc.FindClusters();
    PrintPartition(parts);
    cout<<"End"<<endl;

    delete g;
    bip.clear();



    /********************************************************
     * TEST 2: complete graph
    ********************************************************/
    string gs = "7|0 4 1|0 5 1|0 6 1|1 4 1|1 5 1|1 6 1|2 4 1|2 5 1|2 6 1|3 4 1|3 5 1|3 6 1";
    g = Graph::FromString(gs, "|");
    cout<<"Starting test 2"<<endl;
    CCExactSolver cc2(g);
    bip.push_back(0);bip.push_back(1);bip.push_back(2);bip.push_back(3);
    cc2.SetBipartition(bip);

    vector<vector<int>> parts2;
    parts2 = cc2.FindClusters();
    PrintPartition(parts2);
    cout<<"End"<<endl;

    delete g;
    bip.clear();


    /**********************************************************************
    * TEST 3: some graph with varied weights
    * We expect
    * CLUSTER1=3 4 5 8
    * CLUSTER2=0 2 6 9
    * CLUSTER3=1 7
    **********************************************************************/
    gs = "0 5 -0.736924|0 6 -0.0826997|0 7 -0.562082|0 8 0.357729|0 9 0.869386|1 5 0.0388327|1 6 -0.930856|1 7 0.0594004|1 8 -0.984604|1 9 -0.866316|2 5 0.373545|2 6 0.860873|2 7 0.0538576|2 8 0.307838|2 9 0.402381|3 5 0.524396|3 6 -0.905071|3 7 -0.343532|3 8 0.512821|3 9 -0.269323|4 5 0.965101|4 6 0.506712|4 7 -0.854628|4 8 0.769414|4 9 -0.127177";
    g = Graph::FromString(gs, "|");
    cout<<"Starting test 3"<<endl;
    CCExactSolver cc3(g);
    bip = Util::GetVectorOfIntegers(0, 4);
    cc3.SetBipartition(bip);

    vector<vector<int>> parts3;
    parts3 = cc3.FindClusters();
    PrintPartition(parts3);
    cout<<"End"<<endl;

    delete g;
    bip.clear();

}


void PrintPartition(vector<vector<int>> parts)
{
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
}


void DoSomePerformanceTests()
{
    int n1 = 7;
    int n2 = 7;
    Graph* g = Graph::GetRandomBinaryWeightedGraph(n1, n2, -1, 1);
    vector<int> bip = Util::GetVectorOfIntegers(0, n1 - 1);

    cout<<"Running test 1"<<endl;
    std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
    CCExactSolver cc(g);
    //cc.FindClusters();
    std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>( t2 - t1 ).count();
    cout<<"Time 1 ="<<duration<<endl;

    cout<<"Running test 2"<<endl;
    std::chrono::high_resolution_clock::time_point t1_bip = std::chrono::high_resolution_clock::now();
    CCExactSolver cc_bip(g);
    cc_bip.SetBipartition(bip);
    cc_bip.FindClusters();
    std::chrono::high_resolution_clock::time_point t2_bip = std::chrono::high_resolution_clock::now();

    auto duration_bip = std::chrono::duration_cast<std::chrono::milliseconds>( t2_bip - t1_bip ).count();
    cout<<"Time 2 ="<<duration_bip<<endl;
}


