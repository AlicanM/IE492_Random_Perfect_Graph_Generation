#ifndef GRAPHCONVERTER_H
#define GRAPHCONVERTER_H

#include <string>
#include <igraph.h>
#include <map>


class GraphConverter {
        std::map<int, int> mckayGraphCount;
    public:
        GraphConverter();
        static void graph6_to_igraph(const std::string g6string, igraph_t *graph);
        static std::string igraph_to_graph6(igraph_t *graph);
        std::string getRandomMcKayGraph(int numOfVertices);
};

#endif //GRAPHCONVERTER_H