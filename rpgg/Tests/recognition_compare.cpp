#include <iostream>
#include <c5_free_gen.h>
#include <ctime>
#include <fstream>
#include <graphconverter.h>
#include <oylum.h>

using namespace std;

int main(int argc, char const *argv[])
{
    if(argc < 3){
        cout << "recognition_compare inputfile.g6 outputfile" << endl;
        return 1;
    }

    igraph_t graph;

    ifstream g6_graphs(argv[1]);
    ofstream output_file(argv[2]);
    time_t timer, igraph_tot_time = 0, oylum_tot_time = 0;

    string g6_str;
    int counter = 0;

    output_file << "Graph No \t IGraph \t Oylum" << endl;
    while(getline(g6_graphs, g6_str)){ 
        counter++;
        GraphConverter::graph6_to_igraph(g6_str, &graph);
        output_file << "Graph " << counter << "\t";

        bool isPerfect;
        cout << "Graph " << counter << "igraph perf check started." << endl;
timer = time(NULL);
        igraph_is_perfect(&graph, &isPerfect);
timer = time(NULL) - timer;
        output_file << timer << "\t";
        igraph_tot_time += timer;

        cout << "Graph " << counter << "oylum perf check started." << endl;
        PerfectnessChecker pChecker;
        pChecker.ReadGraph(&graph);
timer = time(NULL);
	    isPerfect = pChecker.CheckPerfectness();
timer = time(NULL) - timer;
        output_file << timer << endl;
        oylum_tot_time += timer;
    }

    output_file << endl;
    output_file << "Igraph average: " << igraph_tot_time/counter << endl;
    output_file << "Oylum average: " << oylum_tot_time/counter << endl;

    g6_graphs.close();
    output_file.close();
    igraph_destroy(&graph);
    return 0;
}
