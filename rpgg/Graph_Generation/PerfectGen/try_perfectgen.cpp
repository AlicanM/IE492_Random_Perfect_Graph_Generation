#include <perfectgen.h>
#include <graphconverter.h>
#include <string>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include<ctime>
#include <boost/filesystem.hpp>

#ifdef _WIN32
    #include <direct.h> // For Windows
#else 
    #include <sys/stat.h> // For UNIX-like systems
#endif

int main(int argc, char *argv[]) {
    srand(time(0));

    igraph_t graph[2], result;
    std::string g6[2], g6_result;
    igraph_integer_t size[2], op;

    if (argc < 3) {
        std::cerr << "Usage: perfectgen <size of the first graph> <size of the second graph> <combinatorial operation (optional)>" << std::endl
        << "<combinatorial operation> is optional, PerfectGen algorithm will choose a random operation if it is not given.\n\n<combinatorial operation> can be one of the following values:" << std::endl
        << "0\tClique Identification" << std::endl
        << "1\tSubstitution" << std::endl
        << "2\tComposition" << std::endl
        << "3\tDisjoint Union" << std::endl
        << "4\tJoin" << std::endl
        << "5\tComplement" << std::endl;
        return 1;
    }
    
    size[0] = atoi(argv[1]);
    size[1] = atoi(argv[2]);

    if (argc == 3) { // operation not given
        op = rand() % PerfectGen::totalOpCount;
    }
    else { // operation given
        op = atoi(argv[3]);
    }
    std::cout << "size1:\t" << size[0] << "\tsize2:\t" << size[1] << "\top:\t" << op << std::endl;

    // Get random McKay graphs and run PerfectGen
    g6[0] = GraphConverter::getRandomMcKayGraph(size[0]);
    GraphConverter::graph6_to_igraph(g6[0], &graph[0]);

    if (op != 5) {
        g6[1] = GraphConverter::getRandomMcKayGraph(size[1]);
        GraphConverter::graph6_to_igraph(g6[1], &graph[1]);
        std::cout << "graph1:\t" << g6[0] << std::endl << "graph2:\t" << g6[1] << std::endl;
        PerfectGen::perfectGen(&result, op, &graph[0], &graph[1]);
    }
    else {
        PerfectGen::perfectGen(&result, op, &graph[0], NULL);
    }

    // Result

    igraph_integer_t result_size = igraph_vcount(&result);
    std::cout << "A graph with size " << result_size << " is generated." << std::endl;

    g6_result = GraphConverter::igraph_to_graph6(&result);

    // Output generation
    std::ofstream outFile;
    std::ifstream readFile;

    const std::string directoryPath = "./out";
    const std::string outFileName = "perfectgen" + std::to_string(result_size) + ".txt";
    const std::string filePath = directoryPath + "/" + outFileName;
    
    #ifdef _WIN32
        _mkdir(directoryPath.c_str());      // For Windows
    #else 
        mkdir(directoryPath.c_str(), 0777); // For UNIX-like systems
    #endif

    readFile.open(filePath);

    std::string line;
    while (readFile >> line) {
        if (line == g6_result) {
            std::cout << "The graph is already in " + outFileName << std::endl;
            return 0;
        }
    }
    readFile.close();
    outFile.open(filePath, std::ios::app);
    
    outFile << g6_result << "\n"; 
    std::cout << "Written the graph in " + outFileName << std::endl;
    return 0;
}