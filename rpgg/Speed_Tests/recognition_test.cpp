#include <igraph.h>
#include <iostream>
#include <chrono>
#include <oylum.h>
#include <graphconverter.h>

using namespace std;

int main(int argc, char *argv[])
{
    srand(time(NULL));
    //igraph_integer_t vertex_count[] = {200, 500, 1000, 5000};
    igraph_integer_t vertex_count[] = {20, 40, 45, 50};
    int size_vertex_count = 4;
    igraph_real_t density[] = {0.2};
    int size_density = 1;

    time_t timer;
    cout << "vertex\tdensity\tigraph\tisPerf?\toylum\tisPerf?" << endl; 
    for(int v = 0; v < size_vertex_count; v++){
        for(int d = 0; d < size_density; d++){
            cout <<  vertex_count[v] << "\t" << density[d] << "\t";
            igraph_t graph;
            //igraph_erdos_renyi_game(&graph, IGRAPH_ERDOS_RENYI_GNP,  vertex_count[v], density[d], IGRAPH_UNDIRECTED, IGRAPH_NO_LOOPS);
            //igraph_bipartite_game (&graph, nullptr, IGRAPH_ERDOS_RENYI_GNP, vertex_count[v]/2, vertex_count[v]/2, density[d], 1, IGRAPH_UNDIRECTED, IGRAPH_ALL);
            GraphConverter graph_conv;
            string randomMckayGraph = GraphConverter::getRandomMcKayGraph(9);
            GraphConverter::graph6_to_igraph(randomMckayGraph, &graph);

            igraph_bool_t isPerfect;
            // Timer Begin
            chrono::steady_clock::time_point begin = chrono::steady_clock::now();
            igraph_is_perfect(&graph, &isPerfect);
            // Timer End
            chrono::steady_clock::time_point end = chrono::steady_clock::now();
            cout << chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << "\t" << (isPerfect ? "true" : "false") << "\t";

            PerfectnessChecker pChecker;
            pChecker.ReadGraph(&graph);

            // Timer Begin
            begin = chrono::steady_clock::now();
	        isPerfect = pChecker.CheckPerfectness();
            // Timer End
            end = chrono::steady_clock::now();
            cout << chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << "\t" << (isPerfect ? "true" : "false") << endl;

            igraph_destroy(&graph);
        }
    }
    
    return 0;
}
