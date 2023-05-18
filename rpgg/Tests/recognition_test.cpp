#include <igraph.h>
#include <iostream>
#include <ctime>
#include <oylum.h>
#include <graphconverter.h>

using namespace std;

int main(int argc, char *argv[])
{
    //igraph_integer_t vertex_count[] = {200, 500, 1000, 5000};
    igraph_integer_t vertex_count[] = {20, 40, 45, 50};
    int size_vertex_count = 4;
    igraph_real_t density[] = {0.2};
    int size_density = 1;

    time_t timer;
    cout << "vertex\tdensity\tigraph\toylum" << endl; 
    for(int v = 0; v < size_vertex_count; v++){
        for(int d = 0; d < size_density; d++){
            cout <<  vertex_count[v] << "\t" << density[d] << "\t";
            igraph_t graph;
            //igraph_erdos_renyi_game(&graph, IGRAPH_ERDOS_RENYI_GNP,  vertex_count[v], density[d], IGRAPH_UNDIRECTED, IGRAPH_NO_LOOPS);
            //igraph_bipartite_game (&graph, nullptr, IGRAPH_ERDOS_RENYI_GNP, vertex_count[v]/2, vertex_count[v]/2, density[d], 1, IGRAPH_UNDIRECTED, IGRAPH_ALL);
            GraphConverter graph_conv;
            GraphConverter::graph6_to_igraph(graph_conv.getRandomGraph(9), &graph);

            igraph_bool_t isPerfect;
timer = time(NULL);
            igraph_is_perfect(&graph, &isPerfect);
timer = time(NULL) - timer;
            cout << timer << "\t";

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
