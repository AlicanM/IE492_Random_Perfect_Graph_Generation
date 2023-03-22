#include <iostream>
#include <c5_free_gen.h>

using namespace std;

int main(int argc, char const *argv[])
{
    igraph_t graph;

    generate_c5_free_graph(&graph, 100, 0.5);

    igraph_write_graph_edgelist(&graph, stdout);
    igraph_bool_t isPerfect;
    igraph_is_perfect(&graph, &isPerfect);
    if(isPerfect){
        cout << "PERFECT!!" << endl;
    }
    else{
        cout << "Not Perfect" << endl;
    }
    return 0;
}
