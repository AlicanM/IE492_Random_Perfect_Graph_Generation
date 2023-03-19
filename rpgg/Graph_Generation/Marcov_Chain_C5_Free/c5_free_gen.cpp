#include <igraph.h>
#include <iostream>

using namespace std;

igraph_integer_t vertex_count = 20;
igraph_real_t density = 0.3;

int main(int argc, char const *argv[])
{
    igraph_integer_t final_edge_count = density * vertex_count*(vertex_count-1)/2;

    igraph_rng_t *rng = igraph_rng_default();
    igraph_rng_seed(rng, 1);                            //SET SEED, REMOVE LATER

    igraph_t graph;
    igraph_empty(&graph, vertex_count, IGRAPH_UNDIRECTED);
    igraph_integer_t vertex1_id, vertex2_id, edge_id;
    
    while(igraph_ecount(&graph) < final_edge_count){
        // Select two random vertices
        vertex1_id = igraph_rng_get_integer(rng, 0, vertex_count-1);
        vertex2_id = igraph_rng_get_integer(rng, 0, vertex_count-1);
        // If vetrices are the same skip
        if(vertex1_id == vertex2_id){ continue; }
        // If edge already exists skip
        igraph_get_eid(&graph, &edge_id, vertex1_id, vertex2_id, IGRAPH_UNDIRECTED, false);
        if(edge_id >= 0){ continue; }
        
        igraph_add_edge(&graph, vertex1_id, vertex2_id);
    }
        cout << igraph_ecount(&graph) << endl;
    return 0;
}
