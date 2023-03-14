#include <igraph.h>
#include <iostream>

using namespace std;

int main(void) {

    igraph_t g;
    igraph_vector_int_t v;

    igraph_vector_int_init(&v, 0);
    igraph_small(&g, 4, IGRAPH_ADJ_UNDIRECTED, 0,1, 1,2, 2,3, -1);

    igraph_neighbors(&g, &v, 2, IGRAPH_ALL);
    igraph_vector_int_sort(&v);
    cout << igraph_vector_int_size(&v);
    igraph_vector_int_print(&v);

    igraph_vector_int_destroy(&v);
    igraph_destroy(&g);
    return 0;
}