#include <igraph.h>
#include <iostream>

using namespace std;


#include <igraph/igraph.h>

void print_neighbors(igraph_t *graph) {
    igraph_vector_int_t neighbors;
    igraph_vector_int_init(&neighbors, 0);
    
    igraph_integer_t node_count = igraph_vcount(graph);

    for (igraph_integer_t i = 0; i < node_count; i++) {
        igraph_integer_t node_id = i;
        igraph_neighbors(graph, &neighbors, node_id, IGRAPH_ALL);
        printf("Node %ld's neighbors: ", node_id);
        for (igraph_integer_t j = 0; j < igraph_vector_int_size(&neighbors); j++) {
            igraph_integer_t neighbor_id = VECTOR(neighbors)[j];
            printf("%ld ", neighbor_id);
        }
        printf("\n");
        igraph_vector_int_clear(&neighbors);
    }
    
    igraph_vector_int_destroy(&neighbors);
}


int main(void) {

    igraph_t g;
    igraph_vector_int_t v;

    igraph_vector_int_init(&v, 0);
    igraph_small(&g, 4, IGRAPH_ADJ_UNDIRECTED, 0,1, 1,2, 2,3, -1);

    print_neighbors(&g);

    igraph_vector_int_destroy(&v);
    igraph_destroy(&g);
    return 0;
}