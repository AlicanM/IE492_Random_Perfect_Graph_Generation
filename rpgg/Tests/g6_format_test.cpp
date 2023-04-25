#include <iostream>
#include <c5_free_gen.h>
#include <ctime>
#include <fstream>
#include <graphconverter.h>

using namespace std;

int main(int argc, char const *argv[])
{
    if(argc < 3){
        cout << "g6_format_test inputfile.g6 outputfile" << endl;
        return 1;
    }

    igraph_t graph;

    ifstream g6_graphs(argv[1]);
    ofstream output_file(argv[2]);

    int perf_count = 0, bipartite_count = 0, chordal_count = 0, connected_count = 0, acyclic_count = 0, total_cliques = 0;

    string g6_str;
    int counter = 0;
    while(getline(g6_graphs, g6_str)){ 
        counter++;
        GraphConverter::graph6_to_igraph(g6_str, &graph);

        bool isPerfect, isBipartite, isChordal, isConnected, isAcyclic;
        igraph_integer_t cliqueNo;

        cout << "Graph " << counter << " perf check started." << endl;
        igraph_is_perfect(&graph, &isPerfect);
        cout << "Graph " << counter << (isPerfect?"is Perfect":"is NOT Perfect") << endl;

        if(isPerfect){ 
            igraph_is_bipartite(&graph, &isBipartite, nullptr);
            igraph_is_chordal(&graph, nullptr, nullptr, &isChordal, nullptr, nullptr);
            igraph_is_connected(&graph, &isConnected, IGRAPH_WEAK);
            igraph_is_acyclic(&graph, &isAcyclic);
            igraph_clique_number(&graph, &cliqueNo);
            output_file << "Graph " << counter << ":\t" 
                        << (isBipartite?"Bipartide":"Not Bipartide") << "\t" 
                        << (isChordal?"Chordal":"Not Chordal") << "\t" 
                        << (isConnected?"Connected":"Not Connected") << "\t" 
                        << (isAcyclic?"Acyclic":"Not Acyclic") << "\t" 
                        << "Max Clique: " << cliqueNo << "\t" 
                        << endl;
            perf_count++;
            total_cliques += cliqueNo;
            bipartite_count += isBipartite?1:0;
            chordal_count += isChordal?1:0;
            connected_count += isConnected?1:0;
            acyclic_count += isAcyclic?1:0;
        }
    }
    output_file << "\nNumber of Perfect Graphs\t\t" << perf_count << endl;
    output_file << "Number of Bipartite Perfect Graphs\t\t" << bipartite_count << endl;
    output_file << "Number of Chordal Perfect Graphs\t\t" << chordal_count << endl;
    output_file << "Number of Connected Perfect Graphs\t\t" << connected_count << endl;
    output_file << "Number of Acyclic Perfect Graphs\t\t" << acyclic_count << endl;
    output_file << "Average Clique Number\t\t" << total_cliques/perf_count << endl;

    g6_graphs.close();
    igraph_destroy(&graph);
    return 0;
}
