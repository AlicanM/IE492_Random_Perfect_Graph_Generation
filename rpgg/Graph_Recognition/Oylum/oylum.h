#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <igraph.h>

using namespace std;

class PerfectnessChecker
{
public:

    PerfectnessChecker() {}
    
    bool CheckPerfectness();
    void ReadGraph(string inputFile);
    void ReadGraph(igraph_t *graph);

private:
    
    bool _complementChecked = false;
    int _numNodes;
    vector<vector<int> > _adjList;
    vector<vector<int> > _incMatrix;

    int FindAdmissableNeighbour(const vector<bool>& labels, vector<int>& nextNeighbourIndices, int currentNode);
    void GoUp(vector<bool>& labels, vector<int>& list, vector<int>& nextNeighbourIndices);
    bool SearchOddHoleStartingWithGivenNode2(int startNode);
    bool DetectOddHole3();
    void ConvertToComplement();
    
};

