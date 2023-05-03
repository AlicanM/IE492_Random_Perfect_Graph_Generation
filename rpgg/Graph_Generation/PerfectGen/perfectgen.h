#ifndef PERFECTGEN_H
#define PERFECTGEN_H

#include <igraph.h>

class PerfectGen {

    public:
        static int const totalOpCount;
        static void cliqueIdentification(igraph_t *result, igraph_t *left, igraph_t *right);
        static void substitution(igraph_t *result, const igraph_t *left, const igraph_t *right);
        static void composition(igraph_t *result, const igraph_t *left, const igraph_t *right);
        static void disjointUnion(igraph_t *result, igraph_t *left, igraph_t *right);
        static void join(igraph_t *result, igraph_t *left, igraph_t *right);
        static void complement(igraph_t *result, igraph_t *graph);
        static void perfectGen(igraph_t *result, igraph_integer_t op, igraph_t *g1, igraph_t *g2);
};

#endif //PERFECTGEN_H