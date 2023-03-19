#include <igraph.h>
#include <string>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <fstream>
// argv[1]: Relative path of McKay's program (showg)
// argv[2]: Relative path of the graph6 (.g6) file to be read.
int main(int argc, char **argv) {

    if (argc < 3)  {
        std::cerr << "Usage: graph6_to_igraph  <relative/path/to/showg>  <relative/path/to/.g6>" << std::endl;
        return -1;
    }

    // command line argument
    // Generated_Perfect_graphs/perfect5.g6
    std::string mcKayPath = argv[1];
    std::string graph6Path = argv[2];
    std::string programCall = "./" + mcKayPath + " -e " + graph6Path + " out.txt";
    
    // run showG
    system(programCall.c_str());

    // list of graphs
    std::vector<igraph_t*> perfGraphs;


    // read output
    std::ifstream input("out.txt");

    std::string word;
    int id;
    while(input >> word >> word >> word >> word) {
        int order, edgeCount;
        input >> order >> edgeCount;

        // empty graph
        igraph_t newGraph;
        perfGraphs.push_back(&newGraph);
        
        igraph_adjlist_t adjList;

        // initialize an empty adjacency list
        igraph_adjlist_init_empty(&adjList, order);

        //TODO: initialize adjacency list
        for (int i = 0; i < edgeCount; i++) {
            int vertices[2];
            input >> vertices[0] >> vertices[1];
            igraph_vector_int_t *v = igraph_adjlist_get(&adjList, vertices[0]);
            igraph_vector_int_push_back(v, vertices[1]);         
        }
        
        // create graph from adjacency list
        igraph_adjlist(perfGraphs.front(), &adjList, IGRAPH_ALL, 0);
        // igraph_adjlist_destroy(&adjList);
    }
    remove("out.txt");
    
    // loop all the graphs generated
    for (int i = 0;  i < perfGraphs.size() ; i++){
        igraph_bool_t isPerfect;
        igraph_is_perfect(perfGraphs[i], &isPerfect);
        std::cout << "Graph " << (i+1) << ": " << (isPerfect ? "perfect" : "not perfect") << std::endl;
    }

}