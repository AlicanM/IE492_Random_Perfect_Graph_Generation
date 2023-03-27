#include <iostream>
#include <vector>
#include <queue>
#include <igraph.h>
#include <c5_free_gen.h>

using namespace std;

void get_bfs_tree_levels(igraph_t *graph, int start_vertex, std::vector<std::vector<pair<int,int>>>& levels) {
    int num_vertices = igraph_vcount(graph);

    // Initialize visited vector to false
    std::vector<bool> visited(num_vertices, false);

    // Initialize queue and mark the start vertex as visited
    std::queue<int> queue, next_queue;
    queue.push(start_vertex);
    visited[start_vertex] = true;

    // Initialize the first level with the starting vertex
    levels.push_back(std::vector<pair<int,int>>{make_pair(start_vertex,0)});

    int current_level = 1;

    while (!queue.empty()) {
        // Dequeue a vertex from the queue and print it
        int current_vertex = queue.front();
        queue.pop();

        // Enqueue all adjacent vertices that have not been visited
        igraph_vector_int_t neighbors;
        igraph_vector_int_init(&neighbors, 0);
        igraph_neighbors(graph, &neighbors, current_vertex, IGRAPH_ALL);
        for (int i = 0; i < igraph_vector_int_size(&neighbors); ++i) {
            int neighbor_vertex = VECTOR(neighbors)[i];
            if (!visited[neighbor_vertex]) {
                next_queue.push(neighbor_vertex);
                visited[neighbor_vertex] = true;

                // Add the vertex to the next level
                if (levels.size() == current_level) {
                    levels.push_back(std::vector<pair<int,int>>{});
                }
                levels[current_level].push_back(make_pair(neighbor_vertex, current_vertex));
            }
        }
        igraph_vector_int_destroy(&neighbors);

        // Check if we've finished the current level
        if (queue.size() == 0) {
            current_level++;
            queue.swap(next_queue);
        }
    }
}


void cousin_seperation(igraph_t *graph){
    // Get random starting vertex for bfs 
    igraph_rng_t *rng = igraph_rng_default();
    igraph_rng_seed(rng, time(NULL));
    int start_vertex = igraph_rng_get_integer(rng, 0, igraph_vcount(graph)-1);

    // Get bfs levels
    std::vector<std::vector<pair<int,int>>> levels;
    get_bfs_tree_levels(graph, start_vertex, levels);

    // For each level compare each vertex with same level ones
    igraph_integer_t edge_id;
    for(int level = 2; level < levels.size(); level++){
        for(auto v1_id : levels[level]){
            for(auto v2_id : levels[level]){
                //If they share an edge
                igraph_get_eid(graph, &edge_id, v1_id.first, v2_id.first, IGRAPH_UNDIRECTED, false);
                if(edge_id == -1){ continue; }

                // Remove the edge between one and its parent
                igraph_es_t edge_selector;
                igraph_es_pairs_small(&edge_selector, IGRAPH_DIRECTED, v1_id.first, v1_id.second, -1 );
                igraph_delete_edges(graph, edge_selector);

                igraph_vector_int_t edges;
                igraph_vector_int_init(&edges,0);

                igraph_get_shortest_path(graph, nullptr, &edges, start_vertex, v1_id.first, IGRAPH_ALL);
                igraph_get_eid(graph, &edge_id, v1_id.second, v2_id.first, IGRAPH_UNDIRECTED, false);
                if((igraph_vector_int_size(&edges)-level)%2 == 0 || edge_id >= 0){
                    // If there is no odd cycle potential add back the edge
                    igraph_add_edge(graph, v1_id.first, v1_id.second);
                }
                else{
                    // If not connect the parent to the cousin instead
                    igraph_add_edge(graph, v1_id.second, v2_id.first);
                    break;
                }
            }
        }
    }
}

int main(int argc, char const *argv[])
{
    igraph_t graph;
    generate_c5_free_graph(&graph, 35, 0.2);
    
    bool is_perfect;
    igraph_is_perfect(&graph, &is_perfect);
    while(!is_perfect){
        cout << "Seperate!" << endl;
        cousin_seperation(&graph);
        igraph_is_perfect(&graph, &is_perfect);
    }
    igraph_write_graph_edgelist(&graph, stdout);
    cout << "Perfect!" << endl;
    return 0;
}
