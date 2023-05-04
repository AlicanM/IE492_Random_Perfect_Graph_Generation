#include <iostream>
#include <vector>
#include <queue>
#include <igraph.h>
#include <unistd.h>
#include <fstream>
#include <chrono>
#include <fstream>
#include <random>
#include <algorithm>
#include <cmath>
#include <igraph/igraph.h>
#include <igraph/igraph_vector.h>

using namespace std;


bool clean_cycle_check(igraph_t* graph, igraph_integer_t v1, igraph_integer_t v2) {
    igraph_vector_t vlist;
    igraph_vector_init(&vlist, igraph_vcount(graph));
    igraph_vector_fill(&vlist, 0);

    igraph_integer_t* vs = IGRAPH_VITOF(graph, 0);
    igraph_integer_t vcount = igraph_vcount(graph);
    for (igraph_integer_t i = 0; i < vcount; i++) {
        if (vs[i] != v1 && vs[i] != v2) {
            VECTOR(vlist)[vs[i]] = 1;
        }
    }

    igraph_vector_t p13, p23;
    igraph_vector_init(&p13, 0);
    igraph_vector_init(&p23, 0);

    for (igraph_integer_t i = 0; i < vcount; i++) {
        if (VECTOR(vlist)[vs[i]] == 1) {
            igraph_vector_t path;
            igraph_vector_init(&path, 0);

            igraph_get_shortest_paths(graph, &path, v1, vs[i], IGRAPH_ALL);
            if (igraph_vector_size(&path) > 0) {
                igraph_vector_push_back(&p13, VECTOR(path)[0]);
            }
            igraph_vector_destroy(&path);

            igraph_get_shortest_paths(graph, &path, v2, vs[i], IGRAPH_ALL);
            if (igraph_vector_size(&path) > 0) {
                igraph_vector_push_back(&p23, VECTOR(path)[0]);
            }
            igraph_vector_destroy(&path);
        }
    }

    igraph_vector_t p13p23;
    igraph_vector_init(&p13p23, 0);
    igraph_vector_append(&p13p23, &p13);
    igraph_vector_append(&p13p23, &p23);
    igraph_vector_sort_unique(&p13p23);

    bool result = false;
    if (igraph_vector_size(&p13p23) >= 5 && igraph_vector_size(&p13p23) % 2 == 1) {
        igraph_t indg;
        igraph_empty(&indg, 0, IGRAPH_UNDIRECTED);
        igraph_vector_t subgraph;
        igraph_vector_init(&subgraph, 0);
        igraph_vector_append(&subgraph, &p13p23);
        igraph_induced_subgraph(graph, &indg, &subgraph, IGRAPH_SUBGRAPH_AUTO);

        igraph_vector_t degrees;
        igraph_vector_init(&degrees, 0);
        igraph_degree(&indg, &degrees, igraph_vss_all(), IGRAPH_ALL, 0);
        result = (std::all_of(VECTOR(degrees), VECTOR(degrees) + igraph_vector_size(&degrees), [](int d) { return d == 2; }));

        igraph_vector_destroy(&degrees);
        igraph_vector_destroy(&subgraph);
        igraph_destroy(&indg);
    }

    igraph_vector_destroy(&vlist);
    igraph_vector_destroy(&p13);
    igraph_vector_destroy(&p23);
    igraph_vector_destroy(&p13p23);

    return result;
}

igraph_t* clean_cycle_generation_er(igraph_integer_t n, double p) {
    igraph_t* graph = new igraph_t();
    igraph_empty(graph, n, IGRAPH_UNDIRECTED);

    igraph_integer_t e = round((n * (n - 1)) / 2 * p);
    while (igraph_ecount(graph) < e) {
        igraph_integer_t number_of_edge = round(std::poisson_distribution<double>(0.25)(std::mt19937(std::chrono::system_clock::now().time_since_epoch().count()))) + 1;

        std::vector<igraph_integer_t> vertices;
        for (igraph_integer_t i = 0; i < n; i++) {
            vertices.push_back(i);
        }

        std::vector<std::pair<igraph_integer_t, igraph_integer_t>> edges;
        for (igraph_integer_t i = 0; i < number_of_edge; i++) {
            std::random_shuffle(vertices.begin(), vertices.end());
            igraph_integer_t v1 = vertices[0];
            igraph_integer_t v2 = vertices[1];
            edges.push_back(std::make_pair(v1, v2));
        }

        bool hasCleanCycle = false;
        for (igraph_integer_t i = 0; i < number_of_edge; i++) {
            igraph_t subgraph;
            igraph_empty(&subgraph, 0, IGRAPH_UNDIRECTED);

            igraph_add_edge(graph, edges[i].first, edges[i].second);
            igraph_simplify(graph, 1, 0, 0);

            std::vector<bool> edge_results;
            for (igraph_integer_t j = 0; j < number_of_edge; j++) {
                edge_results.push_back(clean_cycle_check(graph, edges[j].first, edges[j].second));
            }

            if (std::any_of(edge_results.begin(), edge_results.end(), [](bool res) { return res; })) {
                igraph_delete_edges(graph, igraph_ess_pairs(edges[i].first, edges[i].second));
            } else {
                hasCleanCycle = true;
                break;
            }

            igraph_destroy(&subgraph);
        }

        if (!hasCleanCycle) {
            igraph_empty(graph, n, IGRAPH_UNDIRECTED);
        }
    }

    return graph;
}

void writeGraphsToFile(const char* filename, igraph_integer_t numGraphs, igraph_integer_t n, double p) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return;
    }

    for (igraph_integer_t i = 0; i < numGraphs; i++) {
        igraph_t* g = clean_cycle_generation_er(n, p);
        igraph_matrix_t matrix;
        igraph_matrix_init(&matrix, n, n);
        igraph_get_adjacency(g, &matrix, IGRAPH_GET_ADJACENCY_UPPER, IGRAPH_DIRECTED);

        for (igraph_integer_t j = 0; j < n; j++) {
            for (igraph_integer_t k = 0; k < n; k++) {
                file << MATRIX(matrix, j, k) << " ";
            }
            file << "\n";
        }

        igraph_matrix_destroy(&matrix);
        igraph_destroy(g);
        delete g;
    }

    file.close();
}

writeGraphsToFile("C:\\Users\\kpehl\\Desktop\\generated_graphs.txt", 10, 20, 0.4);
