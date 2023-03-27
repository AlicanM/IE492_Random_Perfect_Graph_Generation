#include <iostream>
#include <c5_free_gen.h>
#include <ctime>

using namespace std;

#define VERTICES  30
#define DENSITY 0.3
#define REPETITION  6000

int main(int argc, char const *argv[])
{
    igraph_t graph;
    time_t timer = 0;
    int perf_count = 0, not_perf_count = 0, bipartite_count = 0, chordal_count = 0, connected_count = 0, acyclic_count = 0, total_cliques = 0;

    for(int i = 0; i < REPETITION; i++){
        if(i%100 == 0) { cout << i << endl; }
timer -= time(NULL);
        generate_c5_free_graph(&graph, VERTICES, DENSITY);
timer += time(NULL);
        bool isPerfect, isBipartite, isChordal, isConnected, isAcyclic;
        igraph_integer_t cliqueNo;
        igraph_is_perfect(&graph, &isPerfect);
        if(isPerfect){ 
            perf_count++; 
            igraph_is_bipartite(&graph, &isBipartite, nullptr);
            igraph_is_chordal(&graph, nullptr, nullptr, &isChordal, nullptr, nullptr);
            igraph_is_connected(&graph, &isConnected, IGRAPH_WEAK);
            igraph_is_acyclic(&graph, &isAcyclic);
            igraph_clique_number(&graph, &cliqueNo);
            total_cliques += cliqueNo;
            bipartite_count += isBipartite?1:0;
            chordal_count += isChordal?1:0;
            connected_count += isConnected?1:0;
            acyclic_count += isAcyclic?1:0;
        }
        else { 
            not_perf_count++; 
        }
    }

    cout << "Number of Vertices\t\t" << VERTICES << endl;
    cout << "Density\t\t" << DENSITY << endl;
    cout << "Repetition\t\t" << REPETITION << endl;
    cout << "Total Generation Time\t\t" << timer << " seconds" << endl;
    cout << "Average Graph Generation Time\t\t" << timer/REPETITION << " seconds" << endl;
    cout << "Number of Not Perfect Graphs\t\t" << not_perf_count << endl;
    cout << "Number of Perfect Graphs\t\t" << perf_count << endl;
    if(perf_count > 0){
        cout << "Number of Bipartite Perfect Graphs\t\t" << bipartite_count << endl;
        cout << "Number of Chordal Perfect Graphs\t\t" << chordal_count << endl;
        cout << "Number of Connected Perfect Graphs\t\t" << connected_count << endl;
        cout << "Number of Acyclic Perfect Graphs\t\t" << acyclic_count << endl;
        cout << "Average Clique Number\t\t" << total_cliques/perf_count << endl;
    }

    return 0;
}
