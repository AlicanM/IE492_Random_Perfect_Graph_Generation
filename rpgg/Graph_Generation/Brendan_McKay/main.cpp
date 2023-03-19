#include "graphconverter.h"
#include <vector>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <map>

std::map<int, int> mckayGraphCount;

void initializeMcKatGraphCount() {
    mckayGraphCount[5]  = 33;
    mckayGraphCount[6]  = 148;
    mckayGraphCount[7]  = 906;
    mckayGraphCount[8]  = 8887;
    mckayGraphCount[9]  = 136756;
    mckayGraphCount[10] = 3269264;
    mckayGraphCount[11] = 115811998;
}

// gets a random mckay graph in graph6 (.g6) format.
std::string getRandomMcKayGraph(int numOfVertices) {

    std::ifstream input("../Generated_Perfect_Graphs/perfect" + std::to_string(numOfVertices) + ".g6");
    std::string graph;

    int Nlength, adjListLength, stringLength, random;
    if (0 <= numOfVertices && numOfVertices < 63)
        Nlength = 1;
    else if (62 < numOfVertices && numOfVertices < 258048)
        Nlength = 4;
    else if (258047 < numOfVertices && (long) numOfVertices < 68719476736)
        Nlength = 8;

    adjListLength = (numOfVertices * (numOfVertices-1) / 2);
    adjListLength = ((adjListLength % 6) ? (adjListLength / 6 + 1) : (adjListLength / 6));
    
    stringLength = Nlength + adjListLength;
    random = rand() % mckayGraphCount[numOfVertices];

    input.seekg((stringLength + 1)*random, input.beg);
    input >> graph;
    input.close();
    return graph;
}

// Example use case of GraphConverter class
int main() {
    initializeMcKatGraphCount();
    srand(time(NULL));
    std::vector<std::string> str;
    str.push_back("D?_"); // percect graph
    str.push_back("D?o"); // percect graph
    str.push_back("DQo"); // percect graph
    str.push_back("G?BDAw"); // chordal graph with 8 vertices
    str.push_back("FhCKG"); // ring graph with 7 vertices
    std::vector<igraph_t> graphs;
    
    for (int i = 0; i < str.size(); i++) {
        igraph_t g;
        GraphConverter::graph6_to_igraph(str[i], &g);
        graphs.push_back(g);

    }
    igraph_t ringGraph;
    igraph_ring(&ringGraph, 8, false, false, true); // ring graph with 8 vertices (perfect)
    graphs.push_back(ringGraph);

    igraph_t ringGraph2;
    igraph_ring(&ringGraph2, 9, false, false, true); // ring graph with 9 vertices (not perfect)
    graphs.push_back(ringGraph2);
    
    FILE *out;
    out = fopen("out.txt", "w");
    for (int i = 0; i < graphs.size(); i++)
    {
        igraph_bool_t isPerfect;
        igraph_is_perfect(&graphs[i], &isPerfect);
        fprintf(out, "graph %d\n.g6 representation: %s\n%s\n", i+1, GraphConverter::igraph_to_graph6(&graphs[i]).c_str(), (isPerfect ? "perfect" : "not perfect"));

        igraph_write_graph_edgelist(&graphs[i], out);
        fprintf(out, "\n");
    }

    for (int i = 0; i < 5; i++) {
        std::cout << getRandomMcKayGraph(7) << std::endl;
    }
    return 0;
}