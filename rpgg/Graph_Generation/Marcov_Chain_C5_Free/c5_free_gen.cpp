#include <igraph.h>
#include <iostream>

using namespace std;

igraph_integer_t vertex_count = 30;
igraph_real_t density = 0.3;

int main(int argc, char const *argv[])
{
    igraph_integer_t final_edge_count = density * vertex_count*(vertex_count-1)/2;

    igraph_rng_t *rng = igraph_rng_default();
    igraph_rng_seed(rng, 1);                            //SET SEED, REMOVE LATER

    igraph_t graph;
    //igraph_empty(&graph, vertex_count, IGRAPH_UNDIRECTED);    
    igraph_regular_tree(&graph, 3, 3, IGRAPH_TREE_UNDIRECTED);  /////////////////////////////////////////
    
    igraph_integer_t vertex1_id, vertex2_id, edge_id;
    
    while(igraph_ecount(&graph) < final_edge_count){
        // Select two random vertices
        vertex1_id = 3;//igraph_rng_get_integer(rng, 0, vertex_count-1);    /////////////////////////////////////////
        vertex2_id = 6;//igraph_rng_get_integer(rng, 0, vertex_count-1);    /////////////////////////////////////////
        // If vetrices are the same skip
        if(vertex1_id == vertex2_id){ continue; }
        // If edge already exists skip
        igraph_get_eid(&graph, &edge_id, vertex1_id, vertex2_id, IGRAPH_UNDIRECTED, false);
        if(edge_id >= 0){ continue; }
        
        // Add an edge between
        igraph_add_edge(&graph, vertex1_id, vertex2_id);

        // Gets all the neighbors that can be reach in at least two steps
        igraph_vector_int_list_t neighbors_list;
        igraph_vector_int_list_init(&neighbors_list, 0);
        igraph_vs_t vertex_set;
        igraph_vs_vector_small(&vertex_set, vertex1_id, vertex2_id, -1);
        igraph_neighborhood(&graph, &neighbors_list,  vertex_set, 2, IGRAPH_ALL, 0);
        
        // Find common neighbors of distance two
        igraph_vector_int_t common_neighbours;
        igraph_vector_int_init(&common_neighbours, 0);
        igraph_vector_int_t neighbours1 = igraph_vector_int_list_pop_back(&neighbors_list);
        igraph_vector_int_t neighbours2 = igraph_vector_int_list_pop_back(&neighbors_list);
        for(int i1 = 0; i1 < igraph_vector_int_size(&neighbours1); i1++){
            igraph_integer_t candidate_vector = igraph_vector_int_get(&neighbours1, i1);
            if(candidate_vector == vertex1_id || candidate_vector == vertex2_id){ continue;}
            for(int i2 = 0; i2 < igraph_vector_int_size(&neighbours2); i2++){
                if(candidate_vector == igraph_vector_int_get(&neighbours2, i2)){
                    igraph_vector_int_push_back(&common_neighbours, candidate_vector);
                    break;
                }
            }
        }

        igraph_vector_int_print(&common_neighbours);    /////////////////////////////////////////
        igraph_integer_t vertex3_id, vertex4_id, vertex5_id;
        for(int i = 0; i < igraph_vector_int_size(&common_neighbours); i++){
            for(int j = i+1; j < igraph_vector_int_size(&common_neighbours); j++){
                for(int k = j+1; k < igraph_vector_int_size(&common_neighbours); k++){
                    // For all three element subsets of neighbours 
                    vertex3_id = igraph_vector_int_get(&common_neighbours, i);
                    vertex4_id = igraph_vector_int_get(&common_neighbours, j);
                    vertex5_id = igraph_vector_int_get(&common_neighbours, k);

                    cout << "\t" << vertex1_id << "\t" << vertex2_id << "\t" << vertex3_id << "\t" << vertex4_id << "\t" << vertex5_id << endl;

                    // Looks at all 10 possible edges 
                    // If all 5 vertices has degree 2, then there is a C5
                    igraph_integer_t v1_count = 0, v2_count  = 0, v3_count = 0, v4_count = 0, v5_count = 0;

                    igraph_get_eid(&graph, &edge_id, vertex1_id, vertex2_id, IGRAPH_UNDIRECTED, false);
                    if(edge_id >= 0){ v1_count++; v2_count++; }                    
                    igraph_get_eid(&graph, &edge_id, vertex1_id, vertex3_id, IGRAPH_UNDIRECTED, false);
                    if(edge_id >= 0){ v1_count++; v3_count++; }
                    igraph_get_eid(&graph, &edge_id, vertex1_id, vertex4_id, IGRAPH_UNDIRECTED, false);
                    if(edge_id >= 0){ v1_count++; v4_count++; }
                    igraph_get_eid(&graph, &edge_id, vertex1_id, vertex5_id, IGRAPH_UNDIRECTED, false);
                    if(edge_id >= 0){ v1_count++; v5_count++; }
                    if(v1_count != 2){  continue; }

                    igraph_get_eid(&graph, &edge_id, vertex2_id, vertex3_id, IGRAPH_UNDIRECTED, false);
                    if(edge_id >= 0){ v2_count++; v3_count++; }
                    igraph_get_eid(&graph, &edge_id, vertex2_id, vertex4_id, IGRAPH_UNDIRECTED, false);
                    if(edge_id >= 0){ v2_count++; v4_count++; }
                    igraph_get_eid(&graph, &edge_id, vertex2_id, vertex5_id, IGRAPH_UNDIRECTED, false);
                    if(edge_id >= 0){ v2_count++; v5_count++; }
                    if(v2_count != 2){  continue; }

                    igraph_get_eid(&graph, &edge_id, vertex3_id, vertex4_id, IGRAPH_UNDIRECTED, false);
                    if(edge_id >= 0){ v3_count++; v4_count++; }
                    igraph_get_eid(&graph, &edge_id, vertex3_id, vertex5_id, IGRAPH_UNDIRECTED, false);
                    if(edge_id >= 0){ v3_count++; v5_count++; }
                    if(v3_count != 2){  continue; }

                    igraph_get_eid(&graph, &edge_id, vertex4_id, vertex5_id, IGRAPH_UNDIRECTED, false);
                    if(edge_id >= 0){ v4_count++; v5_count++; }
                    if(v4_count != 2){  continue; }
                    if(v5_count != 2){  continue; } //redundant

                    // New edge created a C5!!
                    // Remove an edge that is a part of it.


                }            
            }            
        }
        // IF odd jump here
    }
        cout << igraph_ecount(&graph) << endl;
    return 0;
}
