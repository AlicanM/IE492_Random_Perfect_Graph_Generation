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

    // store the disjoint union of two graphs
    igraph_disjoint_union(result, left, right);

    // Remove vertex v from the result graph
    igraph_delete_vertices(result, igraph_vss_1(v));

    // Link each vertex in right graph to the neighbors of v in left graph
    igraph_integer_t neigborCount = igraph_vector_int_size(&neighbors);
    igraph_integer_t right_vcount = igraph_vcount(right);
    igraph_integer_t left_vcount = igraph_vcount(left) - 1;
    igraph_vector_int_t edges;
    igraph_vector_int_init(&edges, neigborCount * right_vcount * 2);

    igraph_integer_t k = 0;
    for (igraph_integer_t i = 0; i < right_vcount; i++) {
        for (igraph_integer_t j = 0; j < neigborCount; j++) {
            igraph_integer_t neighbor = VECTOR(neighbors)[j];
            VECTOR(edges)[k++] = (neighbor > v ? neighbor - 1 : neighbor);
            VECTOR(edges)[k++] = left_vcount + i;
        }
    }

    // Add all the edges
    igraph_add_edges(result, &edges, 0);

    // Free memory
    igraph_vector_int_destroy(&edges);
    igraph_vector_int_destroy(&neighbors);
}

// Let G1, G2 be disjoint graphs each with at least three vertices, vi be a vertex of Gi, and Ni the set of all neighbors of vi.
// The composition of G1 and G2 is obtained from removing v1 from G1 and v2 from G2, and by connecting all vertices in N1 to those in N2.
void PerfectGen::composition(igraph_t *result, const igraph_t *left, const igraph_t *right) {
    // Get sizes of graphs
    igraph_integer_t left_vcount = igraph_vcount(left);
    igraph_integer_t right_vcount = igraph_vcount(right);

    // Choose random vertices v1 in left graph and v2 in right graph
    igraph_integer_t v1 = igraph_rng_get_integer(igraph_rng_default(), 0, igraph_vcount(left) - 1);
    igraph_integer_t v2 = igraph_rng_get_integer(igraph_rng_default(), 0, igraph_vcount(right) - 1);

    // store the disjoint union of two graphs
    igraph_disjoint_union(result, left, right);

    // Get the neighbors of v1 in left graph and v2 in right graph
    igraph_vector_int_t neighbors1, neighbors2;
    igraph_vector_int_init(&neighbors1, 0);
    igraph_neighbors(left, &neighbors1, v1, IGRAPH_ALL);
    igraph_vector_int_init(&neighbors2, 0);
    igraph_neighbors(right, &neighbors2, v2, IGRAPH_ALL);

    // Put v1 and v2 in a list
    igraph_vector_int_t verticesToBeDeleted;
    igraph_vector_int_init(&verticesToBeDeleted, 2);
    VECTOR(verticesToBeDeleted)[0] = v1;
    VECTOR(verticesToBeDeleted)[0] = left_vcount + v2;

    // Remove vertices v1 and v2 from the result graph
    igraph_delete_vertices(result, igraph_vss_vector(&verticesToBeDeleted));
    left_vcount--;
    right_vcount--;

    // Free memory
    igraph_vector_int_destroy(&verticesToBeDeleted);

    igraph_vector_int_t edges;
    igraph_integer_t neighbor1Count = igraph_vector_int_size(&neighbors1);
    igraph_integer_t neighbor2Count = igraph_vector_int_size(&neighbors2);
    igraph_vector_int_init(&edges, neighbor1Count * neighbor2Count * 2);

    // Link vertices in N1 to vertices in N2
    igraph_integer_t k = 0;
    for (igraph_integer_t i = 0; i < neighbor1Count; i++) {
        for (igraph_integer_t j = 0; j < neighbor2Count; j++) {
            igraph_integer_t neighbor1 = VECTOR(neighbors1)[i];
            igraph_integer_t neighbor2 = VECTOR(neighbors2)[j];
            VECTOR(edges)[k++] = (neighbor1 > v1 ? neighbor1 - 1 : neighbor1);
            VECTOR(edges)[k++] = left_vcount + (neighbor2 > v2 ? neighbor2 - 1 : neighbor2);
        }
    }

    // Add all the edges
    igraph_add_edges(result, &edges, 0);

    // Free memory
    igraph_vector_int_destroy(&edges);
    igraph_vector_int_destroy(&neighbors1);
    igraph_vector_int_destroy(&neighbors2);
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