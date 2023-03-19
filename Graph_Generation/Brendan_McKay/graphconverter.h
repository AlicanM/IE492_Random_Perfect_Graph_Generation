#ifndef GRAPHCONVERTER_H
#define GRAPHCONVERTER_H

#include <string>
#include <igraph.h>

class GraphConverter {
    public:
        static void graph6_to_igraph(const std::string g6string, igraph_t *graph);
        static std::string igraph_to_graph6(igraph_t *graph);
};

#endif //GRAPHCONVERTER_H