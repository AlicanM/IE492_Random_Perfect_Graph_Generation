#include <perfectgen.h>
#include <iostream>
#include <vector>
#include <algorithm>

int const PerfectGen::totalOpCount = 6;

// Let G1, G2 be disjoint graphs, and Ki be a nonempty clique in Gi satisfying |K1| = |K2|. Define a one-to-one correspondence between vertices of K1 and K2; i.e., choose a bijective map f: K1->K2. A graph obtained by unifying each vertex v in K1 with vertex f(v) in K2 is said to arise from G1 and G2 by clique identification. A graph G obtained from two perfect graphs via clique identification is perfect.
// We randomly select one vertex from G and one vertex from G', and extend each one to a maximal clique, say K1 and K2 . Without loss of generality, say |K1| ≤ |K2|. We randomly choose |K1| vertices from K2 and identify them with those in K1. The bijection f to identify those vertices is randomly determined.
void PerfectGen::cliqueIdentification(igraph_t *result, igraph_t *left, igraph_t *right) {

    igraph_t *graphs[2] = {left, right};
    igraph_integer_t size[2], vertex[2];
    igraph_vector_int_t randomMaximalClique[2];

    // Part 1: Finding random cliques K1 and K2 in the graphs
    for (int i = 0; i < 2; i++) {
        // Get sizes of graphs
        size[i] = igraph_vcount(graphs[i]);

        // Choose random vertices v1 in left graph and v2 in right graph
        vertex[i] = rand() % size[i];
//      std::cout << "Vertex "  << i+1 << ": " << vertex[i] << std::endl;

        // Get maximal cliques of the vertices
        igraph_vector_int_list_t allMaximalCliques;
        igraph_integer_t maximalCliqueCount;
        igraph_vector_int_list_init(&allMaximalCliques, 0);

        // Get all maximal cliques of graphs
        igraph_maximal_cliques(graphs[i], &allMaximalCliques, 0, 0);
        
        maximalCliqueCount = igraph_vector_int_list_size(&allMaximalCliques);
        
        // Detect maximal cliques that contain v1 and v2
        igraph_vector_int_t cliquesWithTheVertex;
        igraph_vector_int_init(&cliquesWithTheVertex, 0);
        for (int j = 0; j < maximalCliqueCount; j++) {
            if (igraph_vector_int_contains(igraph_vector_int_list_get_ptr(&allMaximalCliques, j), vertex[i])) {
                igraph_vector_int_push_back(&cliquesWithTheVertex, j);
            }
        }

        // Get random cliques
        igraph_integer_t randomMaximalCliqueIndex = VECTOR(cliquesWithTheVertex)[rand() % igraph_vector_int_size(&cliquesWithTheVertex)];
        igraph_vector_int_init_copy(&randomMaximalClique[i], igraph_vector_int_list_get_ptr(&allMaximalCliques, randomMaximalCliqueIndex));

        igraph_vector_int_list_destroy(&allMaximalCliques);
        igraph_vector_int_destroy(&cliquesWithTheVertex);
/* 
        std::cout << "Random clique " << i+1 <<  " with " << vertex[i] << ":" << std::endl;
        for (int j = 0; j < igraph_vector_int_size(&randomMaximalClique[i]); j++) {
            std::cout << VECTOR(randomMaximalClique[i])[j] << " ";
        }
        std::cout << std::endl;
*/
    }

    // Part 2: Generating result graph
    igraph_integer_t cliqueSizes[2] = {igraph_vector_int_size(&randomMaximalClique[0]), igraph_vector_int_size(&randomMaximalClique[1])};
    igraph_integer_t smallClique = (cliqueSizes[0] <= cliqueSizes[1] ? 0 : 1), largeClique = (cliqueSizes[0] <= cliqueSizes[1] ? 1 : 0);

    std::vector<int> indices;
    for (int i = 0; i < cliqueSizes[largeClique]; i++) {
        indices.push_back(i);
    }
    std::random_shuffle(indices.begin(), indices.end());

 
    std::cout << "Large clique - " << (largeClique == 0 ? "left graph " : "right graph") << "(size "<< cliqueSizes[largeClique] << ")" << std::endl;
    for (std::vector<int>::const_iterator i = indices.cbegin(); i != indices.cend(); i++) {
        std::cout << 1 + VECTOR(randomMaximalClique[largeClique])[*i] << " ";
    }
    std::cout << std::endl;
    std::cout << "Small clique - " << (smallClique == 0 ? "left graph " : "right graph")  << "(size "<< cliqueSizes[smallClique] << ")" << std::endl;
    for (int i = 0; i < cliqueSizes[smallClique]; i++) {
        std::cout << 1 + VECTOR(randomMaximalClique[smallClique])[i] << " ";
    }
    std::cout << std::endl;


    igraph_disjoint_union(result, left, right);
    igraph_vector_int_add_constant(&randomMaximalClique[1], size[0]);

    igraph_vector_int_t edges;
    igraph_vector_int_init(&edges, 0);
    for (igraph_integer_t i = 0; i < cliqueSizes[smallClique]; i++) {
        igraph_integer_t smallCliqueVertex = igraph_vector_int_get(&randomMaximalClique[smallClique], i);
        igraph_integer_t largeCliqueVertex = igraph_vector_int_get(&randomMaximalClique[largeClique], indices[i]);

        // Get neighbors of the vertex in the small clique
        igraph_vector_int_t neighbors;
        igraph_vector_int_init(&neighbors, 0);

        igraph_neighbors(result, &neighbors, smallCliqueVertex, IGRAPH_ALL);
        igraph_integer_t neighborCount = igraph_vector_int_size(&neighbors);

        
  
//      std::cout << "Vertex " << 1+smallCliqueVertex << " - Neigbor count: " << neighborCount << std::endl;
        for (igraph_integer_t j = 0; j < neighborCount; j++)  {
            igraph_integer_t neighbor = VECTOR(neighbors)[j];
//          std::cout << "neighbor: " << 1+neighbor << std::endl;
            igraph_vector_int_push_back(&edges, largeCliqueVertex);
            igraph_vector_int_push_back(&edges, neighbor);
        }

        igraph_vector_int_destroy(&neighbors); // Free memory
    }
/* 
    std::cout << "Edges to be added:" << std::endl;
    for (int i = 0 ; i < igraph_vector_int_size(&edges); i+=2) {
        std::cout << VECTOR(edges)[i] << "-" << VECTOR(edges)[i+1] << " ";
    }
    std::cout << std::endl;
*/
    // Add edges
    igraph_add_edges(result, &edges, 0);

    // Remove vertices that are from the small clique
    igraph_delete_vertices(result, igraph_vss_vector(&randomMaximalClique[smallClique]));

    igraph_vector_int_destroy(&edges); // Free memory
}

// Let G1, G2 be disjoint graphs, v be a vertex of G1, and N the set of all neighbors of v in G1.
// Removing v from G1 and linking each vertex in G2 to those in N results in a graph that arises from G1 and G2 by substitution.
// If G1 and G2 are perfect, a graph G derived via substitution of the two is perfect too.
void PerfectGen::substitution(igraph_t *result, const igraph_t *left, const igraph_t *right) {
    // Choose a random vertex v in left graph
    igraph_integer_t v = rand() % igraph_vcount(left);

    // Get the neighbors of vertex v in left graph
    igraph_vector_int_t neighbors;
    igraph_vector_int_init(&neighbors, 0);
    igraph_neighbors(left, &neighbors, v, IGRAPH_ALL);

    // store the disjoint union of two graphs
    igraph_disjoint_union(result, left, right);

    // Remove vertex v from the result graph
    igraph_delete_vertices(result, igraph_vss_1(v));

    // Link each vertex in right graph to the neighbors of v in left graph
    igraph_integer_t neighborCount = igraph_vector_int_size(&neighbors);
    igraph_integer_t right_vcount = igraph_vcount(right);
    igraph_integer_t left_vcount = igraph_vcount(left) - 1;
    igraph_vector_int_t edges;
    igraph_vector_int_init(&edges, neighborCount * right_vcount * 2);

    igraph_integer_t k = 0;
    for (igraph_integer_t i = 0; i < right_vcount; i++) {
        for (igraph_integer_t j = 0; j < neighborCount; j++) {
            igraph_integer_t neighbor = VECTOR(neighbors)[j];
            igraph_vector_int_push_back(&edges, (neighbor > v ? neighbor - 1 : neighbor));
            igraph_vector_int_push_back(&edges, left_vcount + i);
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
    igraph_integer_t v1 = rand() % igraph_vcount(left);
    igraph_integer_t v2 = rand() % igraph_vcount(right);

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
            igraph_vector_int_push_back(&edges, (neighbor1 > v1 ? neighbor1 - 1 : neighbor1));
            igraph_vector_int_push_back(&edges, left_vcount + (neighbor2 > v2 ? neighbor2 - 1 : neighbor2));
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

void PerfectGen::complement(igraph_t *result, igraph_t *graph) {
    igraph_complementer(result, graph, false);
}

// PerfectGen algorithm explained in:
// Oylum Şeker, Tınaz Ekim, and Zeki Caner Taşkın. “An Exact Cutting Plane Algorithm to Solve the Selective Graph Coloring Problem in Perfect Graphs”. In: European Journal of Operational Research 291(1) (2021), 67–83. doi: https://doi.org/10.1016/j.ejor.2020.09.017
void PerfectGen::perfectGen(igraph_t *result, igraph_integer_t op, igraph_t *g1, igraph_t *g2) {

    if (op == 0) {      // Clique Identification
        PerfectGen::cliqueIdentification(result, g1, g2);
    }
    else if (op == 1) { // Substitution
        PerfectGen::substitution(result, g1, g2);
    }
    else if (op == 2) { // Composition
        PerfectGen::composition(result, g1, g2);
    }
    else if (op == 3) { // Disjoint Union
        PerfectGen::disjointUnion(result, g1, g2);
    }
    else if (op == 4) { // Join
        PerfectGen::join(result, g1, g2);
    }
    else if (op == 5) { // Complement
        PerfectGen::complement(result, g1);
    }
}