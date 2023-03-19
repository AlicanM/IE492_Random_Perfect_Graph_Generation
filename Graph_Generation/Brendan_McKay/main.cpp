#include "graphconverter.h"
#include <vector>
#include <iostream>

// Example use case of GraphConverter class
int main() {
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

    return 0;
}