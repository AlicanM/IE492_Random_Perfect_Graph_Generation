#include "graphconverter.h"
#include <stdlib.h>
#include <algorithm>
#include <bitset>
#include <fstream>


// Initialize McKay's Graphs Counts
GraphConverter::GraphConverter(){
    mckayGraphCount[5]  = 33;
    mckayGraphCount[6]  = 148;
    mckayGraphCount[7]  = 906;
    mckayGraphCount[8]  = 8887;
    mckayGraphCount[9]  = 136756;
    mckayGraphCount[10] = 3269264;
    mckayGraphCount[11] = 115811998;
}

// Parses graph6 (.g6) format, creates an igraph and assigns it to the given igraph pointer
// Details of graph6 format can be found at: https://users.cecs.anu.edu.au/~bdm/data/formats.txt
void GraphConverter::graph6_to_igraph(const std::string g6string, igraph_t *graph) {
    if (g6string.length() != 0) {

        int N = 0;
        int n_level = 0;

        // step 1: read N value
        if (g6string[0] == 126) {
            n_level = (g6string[1] == 126 ? 2 : 1);

            std::string N_bits;
            for (int i = 0; i < 3 * n_level; i++) {
                N_bits.append(std::bitset<6>(g6string[n_level + i] - 63).to_string());
            }
            N = stoi(N_bits, 0, 2);
        }
        else {
            N = g6string[0] - 63;
        }

        // step 2: read adjacency matrix's upper triangle
        std::string adjMatStr;

        for (int i = (n_level == 0 ? 1 : (n_level == 1 ? 4 : 8)); i < g6string.length(); i++) {
            adjMatStr.append(std::bitset<6>(g6string[i] - 63).to_string());
        }

        igraph_matrix_t adjMatrix;
        igraph_matrix_init(&adjMatrix, N, N);

        int counter = 0;
        for (int col = 1; col < N; col++) {
            for (int row = 0; row < col; row++) {
                igraph_matrix_set(&adjMatrix, row, col, (adjMatStr[counter] - '0'));
                counter++;
            }
        }

        igraph_adjacency(graph, &adjMatrix, IGRAPH_ADJ_UPPER, IGRAPH_NO_LOOPS);
        igraph_matrix_destroy(&adjMatrix);
    }

}

// Creates and returns the graph6 (.g6) format of the given igraph
// Details of the graph6 format can be found at: https://users.cecs.anu.edu.au/~bdm/data/formats.txt
std::string GraphConverter::igraph_to_graph6(igraph_t *graph) {
    // get adjacency list of the graph
    igraph_adjlist_t adjList;
    igraph_adjlist_init(graph, &adjList, IGRAPH_ALL, IGRAPH_NO_LOOPS, IGRAPH_NO_MULTIPLE);
    
    igraph_integer_t size = igraph_adjlist_size(&adjList);

    // the size of the adjacency list string in .g6 = the smallest multiple of 6 greater than or equal to N * (N-1) / 2
    int adjListLen = (size * (size-1) / 2);
    if (adjListLen % 6) {
        adjListLen += 6 - (adjListLen % 6);
    }

    // fill the adjacency list string with 0's first
    std::string adjListStr(adjListLen, '0');

    // put 1's in adjacency list string
    for (igraph_integer_t i = 0; i < size; i++) {
        igraph_vector_int_t *v = igraph_adjlist_get(&adjList, i);

        igraph_integer_t sizeVec = igraph_vector_int_size(v);

        for (igraph_integer_t j = 0; j < sizeVec; j++) {
            igraph_integer_t edge = igraph_vector_int_get(v, j);
            if (edge > i) {
                int index = (edge * (edge - 1) / 2) + i;
                adjListStr[index] = '1';
            }
        }
    }
    igraph_adjlist_destroy(&adjList); // free memory allocated to adjaceny list object

    // graph6 (.g6) format string
    std::string graph6;
    // step 1: convert size to ASCII
    if (size < 63) {
        graph6.push_back(size + 63);
    }
    else {
        std::string N, N_bits;
        graph6.push_back(126);
        if (size < 258048){   
            N_bits = std::bitset<18>(size).to_string();
        
            for (int i = 0; i < 18; i+=6) {
                graph6.push_back((char) (std::bitset<6>(N_bits.substr(0 + i, 6)).to_ulong() + 63));
            }
        }
        else if ((long) size < 68719476736) {
            graph6.push_back(126);
            N_bits = std::bitset<36>(size).to_string();
        
            for (int i = 0; i < 36; i+=6) {
                graph6.push_back((char) (std::bitset<6>(N_bits.substr(0 + i, 6)).to_ulong() + 63));
            }
        }
    }

    // step 2: convert adjListStr to ASCII
    for (int i = 0; i < adjListLen; i+=6) {
        graph6.push_back((char) (std::bitset<6>(adjListStr.substr(0 + i, 6)).to_ulong() + 63));
    }
    return graph6;
}

// gets a random mckay graph in graph6 (.g6) format.
std::string GraphConverter::getRandomMcKayGraph(int numOfVertices) {

    std::ifstream input("../Generated_Perfect_Graphs/perfect" + std::to_string(numOfVertices) + ".g6");
    std::string graph;

    int Nlength, adjListLength, stringLength, random;
    if (0 <= numOfVertices && numOfVertices < 63)
        Nlength = 1;
    else if (62 < numOfVertices && numOfVertices < 258048)
        Nlength = 4;
    else if (258047 < numOfVertices && (long) numOfVertices < 68719476736)
        Nlength = 8;

    adjListLength = (numOfVertices * (numOfVertices-1) / 2);
    adjListLength = ((adjListLength % 6) ? (adjListLength / 6 + 1) : (adjListLength / 6));
    
    stringLength = Nlength + adjListLength;
    random = rand() % mckayGraphCount[numOfVertices];

    input.seekg((stringLength + 1)*random, input.beg);
    input >> graph;
    input.close();
    return graph;
}