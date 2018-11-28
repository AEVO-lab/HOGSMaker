//
// Created by Manuel on 18/11/2018.
//

#include "HOGSMaker.h"


HOGSMaker::HOGSMaker()
{
    nextGeneID = 1;
}


void HOGSMaker::InferHOGS(string omaDir, Node* customSpeciesTree)
{
    Node* speciesTree = customSpeciesTree;
    if (!customSpeciesTree)
    {
        string spfile = omaDir + "Output/EstimatedSpeciesTree.nwk";
        string spContent = Util::GetFileContent(spfile);
        speciesTree = NewickLex::ParseNewickString( spContent );
    }

    GeneSpeciesTreeUtil::Instance()->LabelInternalNodesUniquely(speciesTree);

    TreeIterator* it = speciesTree->GetPostOrderIterator();
    while (Node* n = it->next())
    {
        if (!n->IsLeaf())
        {
            Node* s1 = n->GetChild(0);
            Node* s2 = n->GetChild(1);

            Graph g = GetGraphForSpeciesPair(s1->GetLabel(), s2->GetLabel(), 180, omaDir);


            vector< vector<int> > ccs = g.GetConnectedComponents();
            cout<<"ccs="<<ccs.size()<<endl;
            for (int i = 0; i < ccs.size(); i++)
            {
                cout<<ccs[i].size()<<" ";
            }
            cout<<endl;
        }
    }
    speciesTree->CloseIterator(it);
}


Graph HOGSMaker::GetGraphForSpeciesPair(string s1name, string s2name, double minScore, string omaDir)
{
    Graph g = Graph();
    string fname = omaDir + "Cache/AllALL/" + s1name + "/" + s2name + ".gz";

    Timer::Instance()->StartTiming("open");

    cout<<"Opening "<<fname<<endl;

    if (!Util::FileExists(fname))
    {
        fname = omaDir + "Cache/AllALL/" + s2name + "/" + s1name + ".gz";
        cout<<"doesn't exist"<<endl;
        cout<<"Opening "<<fname<<endl;
    }

    if (!Util::FileExists(fname))
    {
        cout<<"AllALL scores don't exist for "<<s1name<<" "<<s2name<<endl;
        return g;
    }

    int line_count = 1;
    igzstream in(fname.c_str());
    string line;
    bool inRefined = false;
    while(getline(in, line))
    {

        if (Util::Contains(line, "RefinedMatches("))
        {
            inRefined = true;
        }
        else if (Util::Contains(line, "):"))
        {
            inRefined = false;
        }
        else if (inRefined)
        {
            //parse line of format
            //[gene1,gene2,swscore,evo dist in PAM units, align range 1, align range 2, variance of evol dist]
            vector<string> pz = Util::Split(line, ",");
            string g1 = pz[0].substr(1);
            string g2 = pz[1];

            double score = Util::ToDouble(pz[2]);

            if (!g.HasVertex(g1))
                g.AddVertex(g1);
            if (!g.HasVertex(g2))
                g.AddVertex(g2);

            if (score >= minScore)
                g.AddWeightedEdge(g1, g2, score);

        }

        line_count++;
    }

    cout<<"Done, g has "<<g.GetNbVertices()<<" vertices and "<<g.GetNbEdges()<<" edges"<<endl;

    auto time = Timer::Instance()->StopTimingMillis("open");
    cout<<"Time="<<time<<endl;

    return g;
}

vector<string> HOGSMaker::GetGenesPerSpecies(Node* sp)
{
    return genesPerSpecies[sp];
}
