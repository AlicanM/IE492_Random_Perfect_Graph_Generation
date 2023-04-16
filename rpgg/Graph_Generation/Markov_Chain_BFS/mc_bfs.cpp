#include <iostream>
#include <vector>
#include <queue>
#include <igraph.h>
#include <c5_free_gen.h>
#include <graphconverter.h>
#include <unistd.h>
#include <fstream>
#include <oylum.h>
#include <chrono>

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

// Follow bfs tree for a random vertex
// If same level vertices (coisins) shares an edge, remove and adds an edge to alter the structure
// if mode is false, seperated parent connected to the cousin
// if mode is true, one of the cousins connected to the staring vertex
void possible_oddcycle_modifications(igraph_t *graph, bool mode){
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
                igraph_es_destroy(&edge_selector);

                igraph_vector_int_t edges;
                igraph_vector_int_init(&edges,0);

                igraph_get_shortest_path(graph, nullptr, &edges, start_vertex, v1_id.first, IGRAPH_ALL);
                igraph_get_eid(graph, &edge_id, v1_id.second, v2_id.first, IGRAPH_UNDIRECTED, false);
                if((igraph_vector_int_size(&edges)-level)%2 == 0 || edge_id >= 0){
                    // If there is no odd cycle potential add back the edge
                    igraph_add_edge(graph, v1_id.first, v1_id.second);
                }
                else{
                    // If possible odd cycle, connect the parent to the cousin instead
                    if(mode){
                        igraph_get_eid(graph, &edge_id, v1_id.first, start_vertex, IGRAPH_UNDIRECTED, false);
                        if(edge_id == -1 && level%2 == 0 ){
                            igraph_add_edge(graph, v1_id.first, start_vertex);
                            break;
                        }
                        igraph_get_eid(graph, &edge_id, v2_id.first, start_vertex, IGRAPH_UNDIRECTED, false);
                        if(edge_id == -1 &&  level%2 != 0 ){
                            igraph_add_edge(graph, v2_id.first, start_vertex);
                            break;
                        }
                    }
                    igraph_add_edge(graph, v1_id.second, v2_id.first);
                    break;
                }
                igraph_vector_int_destroy(&edges);
            }
        }
    }
}

int main(int argc, char const *argv[])
{
    auto start_time = std::chrono::high_resolution_clock::now();

    if (argc < 5){
        cout << "./mc_bfs vertices density repetition mode" << endl;
        cout << "\tvertices as integer" << endl;
        cout << "\tdensity as float" << endl;
        cout << "\trepetition as integer" << endl;
        cout << "\tmode as {0,1,2} where 0 is start vertex connection, 1 is cousin connection, 2 as hybrid" << endl;
        return 1;
    }

    int vertices = atoi(argv[1]), repetition = atoi(argv[3]), mode = atoi(argv[4]) ;
    float density = atof(argv[2]);

    //Filename handler for running in parallel instance
    char res_file_name[1024], graph_file_name[1024];
    char path[70] = "/ie492_rpgg/rpgg/Graph_Generation/Markov_Chain_BFS/outputs/bfs_output";
    int version = 1;
    sprintf(res_file_name, "%s%d%s%d%s%.1f%s%d%s%d%s", path, version, "_", vertices, "_", density, "_", repetition, "_", mode, ".txt");
    sprintf(graph_file_name, "%s%d%s%d%s%.1f%s%d%s%d%s", path, version, "_", vertices, "_", density, "_", repetition, "_", mode, ".g6");

    while(access(res_file_name, F_OK ) != -1 ){
        version++;
        sprintf(res_file_name, "%s%d%s%d%s%.1f%s%d%s%d%s", path, version, "_", vertices, "_", density, "_", repetition, "_", mode, ".txt");
        sprintf(graph_file_name, "%s%d%s%d%s%.1f%s%d%s%d%s", path, version, "_", vertices, "_", density, "_", repetition, "_", mode, ".g6");
    }

    ofstream out_results(res_file_name);
    ofstream out_graphs(graph_file_name);

    igraph_t graph;
    time_t total_timer = 0;    

    out_results << "GraphNo\t\t#ofSkips\t#ofModifications\tGraphGenTime\tTimeAfterLastSkip\tTotalTime" << endl;
    // Generate repetition number of times perfect graph
    for(int i = 1; i <= repetition; i++){
        int skip_counter = 0, not_perf_counter = 0;
        time_t graph_timer = time(NULL), mc_timer = time(NULL);    

        cout << "C5 Free Generation of Graph " << i << "started." << endl;
        generate_c5_free_graph(&graph, vertices, density);

        cout << "C5 Free Generation of Graph " << i << " ended, starting perfectness check." << endl;
        bool is_perfect;
        igraph_is_perfect(&graph, &is_perfect);
        cout << "Perfectness check of Graph " << i << " ended." << endl;

        while(!is_perfect){

            // If a graph cant be perfected in v^2 tries then generate new graph
            not_perf_counter++;
            if(not_perf_counter >= vertices*vertices){
                cout << "Regeneration C5 Free for Graph " << i << " started. not_perf_counter = " << not_perf_counter << endl;
                skip_counter++;
                not_perf_counter = 0;
                mc_timer = time(NULL);
                generate_c5_free_graph(&graph, vertices, density);
                cout << "Regeneration C5 Free for Graph " << i << " ended." << endl;
            }
                
            cout << "Modification of Graph " << i << " started." << endl;
            // Call markov chain seperation with given mode
            if(mode == 0) possible_oddcycle_modifications(&graph, true);
            else if (mode == 1) possible_oddcycle_modifications(&graph, false);
            else if (mode == 2) possible_oddcycle_modifications(&graph, i%2);
            cout << "Modification of Graph " << i << " ended, starting perfectness check." << endl;

            //PerfectnessChecker pChecker;
            //pChecker.ReadGraph(&graph);
            //is_perfect = pChecker.CheckPerfectness();
            igraph_is_perfect(&graph, &is_perfect);
            cout << "Perfectness check of Graph " << i << " ended." << endl;
        }
        cout << "Graph " << i << " generated." << endl;
        graph_timer = time(NULL) - graph_timer;
        mc_timer = time(NULL) - mc_timer;
        total_timer += graph_timer;
        out_graphs << GraphConverter::igraph_to_graph6(&graph) << endl;
        out_results << "\t" << i << "\t\t\t" << skip_counter << "\t\t\t\t" << not_perf_counter << "\t\t\t\t" << graph_timer << " sec\t\t\t\t" << mc_timer << " sec\t\t\t" << total_timer << " sec" << endl;
    }
    auto end_time = std::chrono::high_resolution_clock::now();
    auto elapsed_time = std::chrono::duration_cast<std::chrono::seconds>(end_time - start_time);
    std::cout << "Elapsed time: " << elapsed_time.count() << " seconds" << std::endl;
    igraph_destroy(&graph);
    out_results.close();
    out_graphs.close();
    return 0;
}

