#include <igraph.h>
#include <fstream>
#include <graphconverter.h>
#include <cstdlib>
#include <graphstatistics.h>


void GraphStatistics::writeStatistics(const int id, const igraph_t *graph, const double elapsedTime, const std::string *filePath) {

    GraphStatistics::initializeLog(filePath);
    std::ofstream outputStream(*filePath, std::ios_base::app);
    igraph_bool_t isBipartite, isChordal, isConnected, isAcyclic;
    igraph_integer_t cliqueNo, size;

    size = igraph_vcount(graph);
    igraph_is_bipartite(graph, &isBipartite, nullptr);
    igraph_is_chordal(graph, nullptr, nullptr, &isChordal, nullptr, nullptr);
    igraph_is_connected(graph, &isConnected, IGRAPH_WEAK);
    igraph_is_acyclic(graph, &isAcyclic);
    igraph_clique_number(graph, &cliqueNo);
    igraph_real_t density;
    igraph_density(graph, &density, false);
    std::string seperator = ",";
    std::string newline = "\n";

    outputStream << id << seperator << elapsedTime << seperator << size << seperator << density << seperator << isBipartite << seperator << isChordal << seperator << isConnected << seperator << isAcyclic << seperator << cliqueNo << newline;
    outputStream.close();
}

void GraphStatistics::initializeLog(const std::string *filePath) {
    std::string headers = "\"Graph_ID\",\"Elapsed_Seconds\",\"Size\",\"Density\",\"Is_Bipartite\",\"Is_Chordal\",\"Is_Connected\",\"Is_Acyclic\",\"Clique_No\"\n";
    std::ifstream inputStream(*filePath);
    
    if (!inputStream.good()) {
        std::ofstream outputStream(*filePath);
        outputStream << headers;
        outputStream.close();
    }
    inputStream.close();
}