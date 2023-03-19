#include <igraph.h>
#include <iostream>

using namespace std;

igraph_integer_t vertex_count = 10;
int main(int argc, char const *argv[])
{
    igraph_t graph;
    igraph_empty(&graph, vertex_count, IGRAPH_UNDIRECTED) ;
    igraph_add_edge(&graph,0,1);
    cout << igraph_write_graph_edgelist(&graph, stdout) << endl;
    return 0;
}
