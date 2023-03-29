#include <iostream>
#include <c5_free_gen.h>
#include <oylum.h>

using namespace std;


bool haveC5(igraph_t *graph){

    int vids[] = {0,1,2,3,4};
    igraph_integer_t edge_id;
    igraph_integer_t vcount = igraph_vcount(graph);
    for(; vids[0]<vcount; vids[0]++){
        for(; vids[1]<vcount; vids[1]++){
            for(; vids[2]<vcount; vids[2]++){
                for(; vids[3]<vcount; vids[3]++){
                    for(; vids[4]<vcount; vids[4]++){ 
    int v_counts[] = {0,0,0,0,0};                   
    for(int i = 0; i<5; i++){
        for(int j = i+1; j<5; j++){
            igraph_get_eid(graph, &edge_id, vids[i], vids[j], IGRAPH_UNDIRECTED, false);
            if(edge_id >= 0){ v_counts[i]++; v_counts[j]++; }
        }
    }

    if(v_counts[0] == 2 && v_counts[1] == 2 && v_counts[2] == 2 && v_counts[3] == 2 && v_counts[4] == 2 ){  
        cout << "C5 found! " << vids[0] << " " << vids[1] << " " << vids[2] << " " << vids[3] << " " << vids[4] << endl;
        return true;
    }
                    }
                }
            }
        }
    }
    return false;
}

int main(int argc, char const *argv[])
{
    igraph_t graph;

    int vc[] = {100};
    float dn[] = {0.2};
    int repeat = 1;
    cout << "Start" << endl;
    for(int v : vc){
cout << v << endl;
        for( float d : dn){
cout << d << endl;
            for(int i = 0; i<repeat; i++){
                generate_c5_free_graph(&graph, v, d);
                igraph_write_graph_edgelist(&graph, stdout);
                PerfectnessChecker pChecker;
                pChecker.ReadGraph(&graph);
                cout << pChecker.CheckPerfectness() << endl;
                if(haveC5(&graph)){
                    cout << "C5 FOUND!!" << endl;
                }
            }
        }
    }
    cout << "Done" << endl;
    return 0;
}
