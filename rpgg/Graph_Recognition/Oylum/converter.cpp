#include <iostream>
#include <ctime>
#include <fstream>
#include <igraph.h>
#include <sstream> 
#include <cctype>
#include <algorithm>
#include <graphconverter.h>

using namespace std;

int main(int argc, char const *argv[])
{
    if(argc < 3){
        cout << "recognition_compare inputfile outputfile.g6" << endl;
        return 1;
    }

    ifstream oylum_graphs(argv[1]);
    ofstream output_file(argv[2], fstream::app);

    string v_count_str;
    getline(oylum_graphs, v_count_str);

    igraph_t graph;
    igraph_empty(&graph, stoi(v_count_str), IGRAPH_UNDIRECTED);
        
    string adj_str, adj_v_id;
    int v_id = 0;
    while(getline(oylum_graphs, adj_str)){ 
        stringstream ss(adj_str); 
        while(getline(ss, adj_v_id, ' ')){ 
            if(adj_v_id.empty() || std::all_of(adj_v_id.begin(), adj_v_id.end(), [](char c){return std::isspace(c);})){continue;}
            igraph_add_edge(&graph, v_id, stoi(adj_v_id));
        }
        v_id++;
    }
    output_file << GraphConverter::igraph_to_graph6(&graph) << endl;

    oylum_graphs.close();
    output_file.close();
    igraph_destroy(&graph);

    return 0;
}
