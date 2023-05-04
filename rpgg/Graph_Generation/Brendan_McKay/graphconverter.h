#ifndef GRAPHCONVERTER_H
#define GRAPHCONVERTER_H

#include <string>
#include <igraph.h>
#include <map>

class GraphConverter {
        
    public:
        static std::map<int, int> mckayGraphCount;
        static void graph6_to_igraph(const std::string g6string, igraph_t *graph);
        static std::string igraph_to_graph6(igraph_t *graph);
        static std::string getRandomGraph(int numOfVertices);
        static int getGraph6StringLength(int numOfVertices);
};


#endif //GRAPHCONVERTER_H