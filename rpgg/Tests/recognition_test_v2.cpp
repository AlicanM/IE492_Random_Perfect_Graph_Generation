#include <igraph.h>
#include <iostream>
#include <fstream>
#include <ctime>
#include <sstream>

using namespace std;

int main(int argc, char *argv[])
{
    igraph_integer_t vertex_count[] = {200, 500, 1000, 5000};
    int size_vertex_count = 4;
    igraph_real_t density[] = {0.2, 0.4, 0.6};
    int size_density = 3;

    time_t timer;
    cout << "vertex\tdensity\tigraph\toylum" << endl; 
    for(int v = 0; v < size_vertex_count; v++){
        for(int d = 0; d < size_density; d++){
            cout <<  vertex_count[v] << "\t" << density[d] << "\t";

            // Read the graph from the input file
            igraph_t graph;
            ifstream input_file(argv[1]);
            if (!input_file.is_open()) {
                cerr << "Error: could not open input file " << argv[1] << endl;
                return 1;
            }
            stringstream graph_stringstream;
            graph_stringstream << input_file.rdbuf();
            input_file.close();
            igraph_read_graph_graph6(&graph, graph_stringstream.str().c_str());

            // Check if the graph is perfect using igraph
            igraph_bool_t isPerfect;
            timer = time(NULL);
            igraph_is_perfect(&graph, &isPerfect);
            timer = time(NULL) - timer;
            cout << timer << "\t";

            // Check if the graph is perfect using Oylum's algorithm
            PerfectnessChecker pChecker;
            pChecker.ReadGraph(&graph);
            timer = time(NULL);
            isPerfect = pChecker.CheckPerfectness();
            timer = time(NULL) - timer;
            cout << timer << endl;

            igraph_destroy(&graph);
        }
    }
    
    return 0;
}
