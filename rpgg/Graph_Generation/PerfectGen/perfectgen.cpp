#include <perfectgen.h>

void PerfectGen::cliqueIdentification(igraph_t *result, igraph_t *left, igraph_t *right) {

}

// Let G1, G2 be disjoint graphs, v be a vertex of G1, and N the set of all neighbors of v in G1.
// Removing v from G1 and linking each vertex in G2 to those in N results in a graph that arises from G1 and G2 by substitution.
// If G1 and G2 are perfect, a graph G derived via substitution of the two is perfect too.
void PerfectGen::substitution(igraph_t *result, const igraph_t *left, const igraph_t *right) {
    // Choose a random vertex v in left graph
    igraph_integer_t v = igraph_rng_get_integer(igraph_rng_default(), 0, igraph_vcount(left) - 1);

    // Get the neighbors of vertex v in left graph
    igraph_vector_int_t neighbors;
    igraph_vector_int_init(&neighbors, 0);
    igraph_neighbors(left, &neighbors, v, IGRAPH_ALL);

    // Copy G1 to the result graph
    igraph_copy(result, left);

    // Remove vertex v from the result graph
    igraph_delete_vertices(result, igraph_vss_1(v));

    // Link each vertex in G2 to the neighbors of v in G1
    igraph_integer_t n = igraph_vector_int_size(&neighbors);
    igraph_integer_t m = igraph_vcount(right);
    igraph_vector_int_t edges;
    igraph_vector_int_init(&edges, n * m * 2);

    igraph_integer_t k = 0;
    for (igraph_integer_t i = 0; i < m; i++) {
        for (igraph_integer_t j = 0; j < n; j++) {
            igraph_integer_t neighbor = VECTOR(neighbors)[j];
            VECTOR(edges)[k++] = neighbor;
            VECTOR(edges)[k++] = igraph_vcount(result) + i;
        }
    }

    // Add all the edges
    igraph_add_edges(result, &edges, 0);

    // Free memory
    igraph_vector_int_destroy(&edges);
    igraph_vector_int_destroy(&neighbors);
}

void PerfectGen::composition(igraph_t *result, igraph_t *left, igraph_t *right) {
    
}

void PerfectGen::disjointUnion(igraph_t *result, igraph_t *left, igraph_t *right) {
    igraph_disjoint_union(result, left, right);
}

void PerfectGen::join(igraph_t *result, igraph_t *left, igraph_t *right) {
    igraph_integer_t left_vcount = igraph_vcount(left);
    igraph_integer_t right_vcount = igraph_vcount(right);

    // store the disjoint union of two graphs
    igraph_disjoint_union(result, left, right);

    // iterate over all vertices in the first graph
    for (igraph_integer_t i = 0; i < left_vcount; i++) {

        // iterate over all vertices in the second graph
        for (igraph_integer_t j = 0; j < right_vcount; j++) {

            // create a new edge between the i-th vertex in the first graph and the j-th vertex in the second graph
            igraph_add_edge(result, i, j + left_vcount);
        }
    }

    // the result graph now contains all vertices from both graphs connected to each other
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