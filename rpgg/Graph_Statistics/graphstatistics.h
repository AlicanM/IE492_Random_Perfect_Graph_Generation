#ifndef GRAPHSTATISTICS_H
#define GRAPHSTATISTICS_H

#include <igraph.h>
#include <fstream>
#include <chrono>

class GraphStatistics {

    public:
        static void writeStatistics(const int id, const igraph_t *graph, const double elapsedTime, const std::string *filePath);
        static void initializeLog(const std::string *filePath);
       
};


#endif //GRAPHSTATISTICS_H