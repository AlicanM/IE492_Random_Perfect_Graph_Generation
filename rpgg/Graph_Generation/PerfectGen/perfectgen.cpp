#include <perfectgen.h>

void PerfectGen::cliqueIdentification(igraph_t *result, igraph_t *left, igraph_t *right) {

}

void PerfectGen::substitution(igraph_t *result, igraph_t *left, igraph_t *right) {
    
}

void PerfectGen::composition(igraph_t *result, igraph_t *left, igraph_t *right) {
    
}

void PerfectGen::disjointUnion(igraph_t *result, igraph_t *left, igraph_t *right) {
    igraph_disjoint_union(result, left, right);
}

void PerfectGen::join(igraph_t *result, igraph_t *left, igraph_t *right) {
    
}

void PerfectGen::complement(igraph_t *result, igraph_t *left, igraph_t *right) {
    
}

// PerfectGen algorithm explained in:
// Oylum Şeker, Tınaz Ekim, and Zeki Caner Taşkın. “An Exact Cutting Plane Algorithm to Solve the Selective Graph Coloring Problem in Perfect Graphs”. In: European Journal of Operational Research 291(1) (2021), 67–83. doi: https://doi.org/10.1016/j.ejor.2020.09.017
void PerfectGen::perfectGen(igraph_t *result, igraph_t *g1, igraph_t *g2) {

    // get random number for a random operation
    int random = rand() % 6;

    if (random == 0) {      // Clique Identification
        PerfectGen::cliqueIdentification(result, g1, g2);
    }
    else if (random == 1) { // Substitution
        PerfectGen::substitution(result, g1, g2);
    }
    else if (random == 2) { // Composition
        PerfectGen::composition(result, g1, g2);
    }
    else if (random == 3) { // Disjoint Union
        PerfectGen::disjointUnion(result, g1, g2);
    }
    else if (random == 4) { // Join
        PerfectGen::join(result, g1, g2);
    }
    else if (random == 5) { // Complement
        PerfectGen::complement(result, g1, g2);
    }
}