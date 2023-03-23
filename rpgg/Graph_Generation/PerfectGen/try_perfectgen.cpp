#include <perfectgen.h>
#include <graphconverter.h>
#include <string>

int  main() {
    igraph_t g[2], result;
    std::string mckay[2];

    for (int i = 0; i < 2; i++) {
        mckay[i] = GraphConverter::getRandomMcKayGraph(9);
        GraphConverter::graph6_to_igraph(mckay[i], &g[i]);
    }

    PerfectGen::perfectGen(&result, &g[0], &g[1]);
    
}