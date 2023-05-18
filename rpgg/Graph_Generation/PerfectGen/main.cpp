#include <perfectgen.h>
#include <graphconverter.h>
#include <string>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <filesystem>
#include <chrono>

bool parseArguments(int argc, char *argv[], igraph_integer_t size[], igraph_integer_t *op) {
    if (argc < 3) {
        std::cerr << "Usage: perfectgen <size of the first graph> <size of the second graph> <combinatorial operation (optional)>" << std::endl
        << "<combinatorial operation> is optional, PerfectGen algorithm will choose a random operation if it is not given.\n\n<combinatorial operation> can be one of the following values:" << std::endl
        << "0\tClique Identification" << std::endl
        << "1\tSubstitution" << std::endl
        << "2\tComposition" << std::endl
        << "3\tDisjoint Union" << std::endl
        << "4\tJoin" << std::endl
        << "5\tComplement" << std::endl;
        return false;
    }
    size[0] = atoi(argv[1]);
    size[1] = atoi(argv[2]);

    if (argc == 3) { // operation not given
        *op = rand() % PerfectGen::totalOpCount;
    }
    else { // operation given
        *op = atoi(argv[3]);
    }
    return true;

}

void printAdjacencyMatrix(igraph_t *graph) {
    igraph_integer_t size = igraph_vcount(graph);

    igraph_matrix_t adjacency_matrix;
    igraph_matrix_init(&adjacency_matrix, size, size);

    igraph_get_adjacency(graph, &adjacency_matrix, IGRAPH_GET_ADJACENCY_BOTH, NULL, IGRAPH_NO_LOOPS);

    for (long int i = 0; i < size; i++) {
        for (long int j = 0; j < size; j++) {
            std::cout << MATRIX(adjacency_matrix, i, j) << " ";
        }
        std::cout << std::endl;
    }

    igraph_matrix_destroy(&adjacency_matrix);
}

void generateOutput(int result_size, std::string *g6_result) {
    std::ofstream outFile;
    std::ifstream readFile, graphCountFile;

    const std::string logDirectoryPath = "./logs";
    std::string mckay_path = std::getenv("MCKAY_PATH");
    const std::string outFileName = "perfect" + std::to_string(result_size) + ".g6";
    const std::string filePath = mckay_path + "/" + outFileName;
    const std::string graph_count_path = mckay_path + "/graph_counts/perfect" + std::to_string(result_size) + ".txt";

    // Generate log
    std::filesystem::create_directory(logDirectoryPath);
    //TODO: generate log


    // Write graph to file
    readFile.open(graph_count_path);
    bool fileExists = readFile.good();
    int num_lines = 1;

    if (fileExists) {
        std::string line;
        readFile >> num_lines;
        readFile.close();
        num_lines++;

        readFile.open(filePath);
        while (readFile >> line) {
            if (line == *g6_result) {
                std::cout << "The graph is already in " + outFileName << std::endl;
                return;
            }
        }
    }
    readFile.close();

    // Write the updated graph count
    outFile.open(graph_count_path);
    outFile << num_lines << "\n";
    outFile.close();

    outFile.open(filePath, std::ios::app); // Append to file
    outFile << *g6_result << "\n";

    // Close the file
    outFile.close();
    std::cout << "Written the graph in " << outFileName << (fileExists ? "." : ". (New file!)") << std::endl;
}

int main(int argc, char *argv[]) {
    srand(time(0));

    igraph_t graph[2], result;
    std::string g6[2], g6_result;
    igraph_integer_t size[2], op;
    
    // Parse command line arguments
    if(!parseArguments(argc, argv, size, &op))
        return -1;

    std::cout << "size1:\t" << size[0] << "\tsize2:\t" << size[1] << "\top:\t" << op << std::endl;

    // Get random McKay graphs and run PerfectGen
    g6[0] = GraphConverter::getRandomGraph(size[0]);
    GraphConverter::graph6_to_igraph(g6[0], &graph[0]);

    std::chrono::system_clock::time_point start, end;

    if (op != 5) {
        g6[1] = GraphConverter::getRandomGraph(size[1]);
        GraphConverter::graph6_to_igraph(g6[1], &graph[1]);
        std::cout << "graph1:\t" << g6[0] << std::endl;
//      printAdjacencyMatrix(&graph[0]);
        std::cout << "graph2:\t" << g6[1] << std::endl;
//      printAdjacencyMatrix(&graph[1]);

        start = std::chrono::system_clock::now();
        PerfectGen::perfectGen(&result, op, &graph[0], &graph[1]);
        end = std::chrono::system_clock::now();
    }
    else {
        std::cout << "graph1:\t" << g6[0] << std::endl;
        start = std::chrono::system_clock::now();
        PerfectGen::perfectGen(&result, op, &graph[0], NULL);
        end = std::chrono::system_clock::now();
    }
    
    // Result
    std::chrono::duration<double> elapsed_seconds = end - start;
    std::cout << "Elapsed time: " << elapsed_seconds.count() << "s" << std::endl;

    igraph_integer_t result_size = igraph_vcount(&result);
    std::cout << "A graph with size " << result_size << " is generated." << std::endl;
    if (result_size < 10) {
        std::cout << "Since result graph's size is less than "<< 10 << ", the graph is not written into a file." << std::endl;
        return 0;
    }
//  printAdjacencyMatrix(&result);

    g6_result = GraphConverter::igraph_to_graph6(&result);

    // Output generation
    generateOutput(result_size, &g6_result);

    std::cout << "Perfectness Check..." << std::endl;
    // PERFECTNESS CHECK
    /*
    igraph_bool_t isPerfect;
    start = std::chrono::system_clock::now();
    igraph_is_perfect(&result, &isPerfect);
    end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = end - start;
    
    std::cout << (isPerfect ? "Perfect" : "NOT perfect !!!") << std::endl;
    std::cout << "Elapsed time: " << elapsed_seconds.count() << "s" << std::endl;
    */
    
    return 0;
}