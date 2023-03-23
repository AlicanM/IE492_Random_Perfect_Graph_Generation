#ifndef PERFECTGEN_H
#define PERFECTGEN_H

#include <igraph.h>

class PerfectGen {

    public:
        static void cliqueIdentification(igraph_t *result, igraph_t *left, igraph_t *right);
        static void substitution(igraph_t *result, igraph_t *left, igraph_t *right);
        static void composition(igraph_t *result, igraph_t *left, igraph_t *right);
        static void disjointUnion(igraph_t *result, igraph_t *left, igraph_t *right);
        static void join(igraph_t *result, igraph_t *left, igraph_t *right);
        static void complement(igraph_t *result, igraph_t *left, igraph_t *right);
        static void perfectGen(igraph_t *g1, igraph_t *g2, igraph_t *result);
};

#endif //PERFECTGEN_H