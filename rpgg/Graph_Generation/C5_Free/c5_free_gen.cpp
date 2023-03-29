#include <iostream>
#include <igraph.h>
#include <c5_free_gen.h>
#include <chrono>

using namespace std;

int generate_c5_free_graph(igraph_t *graph, igraph_integer_t vertex_count, igraph_real_t density)
{
    igraph_rng_t *rng = igraph_rng_default();
    igraph_rng_seed(rng, chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch()).count());

    igraph_empty(graph, vertex_count, IGRAPH_UNDIRECTED);    

    int vertex_ids[5];
    igraph_integer_t edge_id;
    igraph_integer_t final_edge_count = density * vertex_count*(vertex_count-1)/2;

    while(igraph_ecount(graph) < final_edge_count){
        // Select two random vertices
        vertex_ids[0] = igraph_rng_get_integer(rng, 0, vertex_count-1);
        vertex_ids[1] = igraph_rng_get_integer(rng, 0, vertex_count-1);

        // If vetrices are the same skip
        if(vertex_ids[0] == vertex_ids[1]){ continue; }
        // If edge already exists skip
        igraph_get_eid(graph, &edge_id, vertex_ids[0], vertex_ids[1], IGRAPH_UNDIRECTED, false);
        if(edge_id >= 0){ continue; }
        
        // Add an edge between
        igraph_add_edge(graph, vertex_ids[0], vertex_ids[1]);

//cout << igraph_ecount(graph) << " " << vertex_ids[0] << " " <<vertex_ids[1] << endl;

        // Gets all the neighbors that can be reach in at least two steps
        igraph_vector_int_list_t neighbors_list;
        igraph_vector_int_list_init(&neighbors_list, 0);
        igraph_vs_t vertex_set;
        igraph_vs_vector_small(&vertex_set, vertex_ids[0], vertex_ids[1], -1);                                  /////
        igraph_neighborhood(graph, &neighbors_list,  vertex_set, 2, IGRAPH_ALL, 0);
        
        // Find common neighbors of distance two
        igraph_vector_int_t common_neighbours;
        igraph_vector_int_init(&common_neighbours, 0);
        igraph_vector_int_t neighbours1 = igraph_vector_int_list_pop_back(&neighbors_list);
        igraph_vector_int_t neighbours2 = igraph_vector_int_list_pop_back(&neighbors_list);
        for(int i1 = 0; i1 < igraph_vector_int_size(&neighbours1); i1++){
            igraph_integer_t candidate_vector = igraph_vector_int_get(&neighbours1, i1);
            if(candidate_vector == vertex_ids[0] || candidate_vector == vertex_ids[1]){ continue;}
            for(int i2 = 0; i2 < igraph_vector_int_size(&neighbours2); i2++){
                if(candidate_vector == igraph_vector_int_get(&neighbours2, i2)){
                    igraph_vector_int_push_back(&common_neighbours, candidate_vector);
                    break;
                }
            }
        }

        igraph_integer_t vertex3_id, vertex4_id, vertex5_id;
        bool haveC5 = false;
        for(int i = 0; i < igraph_vector_int_size(&common_neighbours) && !haveC5; i++){
            for(int j = i+1; j < igraph_vector_int_size(&common_neighbours) && !haveC5; j++){
                for(int k = j+1; k < igraph_vector_int_size(&common_neighbours) && !haveC5; k++){
                    // For all three element subsets of neighbours 
                    vertex_ids[2] = igraph_vector_int_get(&common_neighbours, i);
                    vertex_ids[3] = igraph_vector_int_get(&common_neighbours, j);
                    vertex_ids[4] = igraph_vector_int_get(&common_neighbours, k);

                    // Looks at all 10 possible edges 
                    // If all 5 vertices has degree 2, then there is a C5
                    int v_counts[] = {0,0,0,0,0};                   
                    for(int i = 0; i<5; i++){
                        for(int j = i+1; j<5; j++){
                            igraph_get_eid(graph, &edge_id, vertex_ids[i], vertex_ids[j], IGRAPH_UNDIRECTED, false);
                            if(edge_id >= 0){ v_counts[i]++; v_counts[j]++; }
                        }
                    }
                    
                    if(v_counts[0] == 2 && v_counts[1] == 2 && v_counts[2] == 2 && v_counts[3] == 2 && v_counts[4] == 2 ){  
                        // New edge created a C5!!
                        // Remove an edge that is a part of it.
                        haveC5 = true;
                    }
                }            
            }            
        }

        // If new edge created a C5 remove the new edge 
        // Also sometimes remove one additional edge of found C5 to prevent getting stuck in edge generation
        if(haveC5){
            igraph_es_t edge_selector;

            int remove_additional = igraph_rng_get_integer(rng, 0, 9); 
            if(remove_additional == 0){
                int ind1, ind2;
                do{
                    ind1 = igraph_rng_get_integer(rng, 0, 4);    
                    do{
                        ind2 = igraph_rng_get_integer(rng, 0, 4);
                    } while(ind1 == ind2 || (ind1 == 0 && ind2 == 1) || (ind1 == 1 && ind2 == 0));
                    igraph_get_eid(graph, &edge_id, vertex_ids[ind1], vertex_ids[ind2], IGRAPH_UNDIRECTED, false);
                } while(edge_id == -1);

                igraph_es_pairs_small(&edge_selector, IGRAPH_DIRECTED,
                                      vertex_ids[0],
                                      vertex_ids[1], 
                                      vertex_ids[ind1],
                                      vertex_ids[ind2],
                                      -1 );
            }
            else{
                igraph_es_pairs_small(&edge_selector, IGRAPH_DIRECTED,
                                      vertex_ids[0],
                                      vertex_ids[1], 
                                      -1 );
            }

            igraph_delete_edges(graph, edge_selector);
        }
    }

    return 0;
}
