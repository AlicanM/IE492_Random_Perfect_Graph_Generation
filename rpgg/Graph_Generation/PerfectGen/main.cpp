#include <perfectgen.h>
#include <graphconverter.h>
#include <graphstatistics.h>
#include <string>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <filesystem>
#include <chrono>
#include <tuple>

void getArguments(int *size1, int *size2, int *op, int *graphCount) {
    std::cout << "Enter the first graph's size? (Must be >= 5)\t";
    do {
        std::cin >> *size1;
    } while (*size1 < 5);
    std::cout << "Enter the second graph's size? (Must be >= 5)\t";
    do {
        std::cin >> *size2;
    } while (*size2 < 5);
    std::cout << "Enter which combinatorial operation (0-5) you would like to use:" << std::endl
        << "If you enter a number other than (0-5), the algorithm will choose a random operation." << std::endl
        << "0\tClique Identification" << std::endl
        << "1\tSubstitution" << std::endl
        << "2\tComposition" << std::endl
        << "3\tDisjoint Union" << std::endl
        << "4\tJoin" << std::endl
        << "5\tComplement" << std::endl;
    std::cin >> *op;
std::cout << "Enter how many graphs you would like to generate:\t";
    std::cin >> *graphCount;

    if (*graphCount <= 0) {
        *graphCount = 1;
    }
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

void generateOutput(igraph_t *result, double elapsed_time) {
    // Result

    igraph_integer_t result_size = igraph_vcount(result);
    std::cout << "A graph with size " << result_size << " is generated." << std::endl;
    if (result_size < 10) {
        std::cout << "Since result graph's size is less than "<< 10 << ", the graph is not written into a file." << std::endl;
        return;
    }
//  printAdjacencyMatrix(&result);

    std::string g6_result = GraphConverter::igraph_to_graph6(result);

    // Output generation

    std::ofstream outFile;
    std::ifstream readFile, graphCountFile;

    const std::string logDirectoryPath = "./logs";
    const std::string allGraphsLogPath = logDirectoryPath + "/all_log.csv";
    const std::string graphsLogPath = logDirectoryPath + "/perfect" + std::to_string(result_size) + "_log.csv";
    const std::string mckay_path = std::getenv("MCKAY_PATH");
    const std::string outFileName = "perfect" + std::to_string(result_size) + ".g6";
    const std::string filePath = mckay_path + "/" + outFileName;
    const std::string allGraphsFilePath = mckay_path + "/" + "allperfectgraphs.g6";
    const std::string graph_count_path = mckay_path + "/graph_counts/perfect" + std::to_string(result_size) + ".txt";
    const std::string all_graph_count_path = mckay_path + "/graph_counts/allperfectgraphs.txt";

    // Generate log
    std::filesystem::create_directory(logDirectoryPath);

    // Write graph to file
    readFile.open(graph_count_path);
    bool fileExists = readFile.good();
    int num_lines = 1;
    int all_num_lines = 1;

    if (fileExists) {
        std::string line;
        readFile >> num_lines;
        readFile.close();
        num_lines++;

        readFile.open(filePath);
        while (readFile >> line) {
            if (line == g6_result) {
                std::cout << "The graph is already in " + outFileName << std::endl;
                return;
            }
        }
    }
    readFile.close();
    readFile.open(all_graph_count_path);
    if (readFile.good()) {
        std::string line;
        readFile >> all_num_lines;
        all_num_lines++;
    }
    GraphStatistics::writeStatistics(num_lines, result, elapsed_time, &graphsLogPath);
    GraphStatistics::writeStatistics(num_lines, result, elapsed_time, &allGraphsLogPath);

    readFile.close();

    // Write the updated graph count
    outFile.open(graph_count_path);
    outFile << num_lines << "\n";
    outFile.close();
    outFile.open(all_graph_count_path);
    outFile << all_num_lines << "\n";
    outFile.close();

    outFile.open(filePath, std::ios::app); // Append to file
    outFile << g6_result << "\n";
    outFile.close();
    
    outFile.open(allGraphsFilePath, std::ios::app);
    outFile << g6_result << "\n";
    outFile.close();
    // Close the file
    std::cout << "Written the graph in " << outFileName << (fileExists ? "." : ". (New file!)") << std::endl;
}

std::tuple<int,double> runPerfectGen(int it, int size1, int size2, int op, igraph_t *graph1, igraph_t *graph2, igraph_t *result) {
    if (0 > op || op >= PerfectGen::totalOpCount) {
        op = rand() % PerfectGen::totalOpCount;
    }

    std::cout << "iter: " << it << "\tsize1: " << size1 << "\tsize2: " << size2 << "\top: " << op << std::endl;
    std::string g6[2], g6_result;

    // Get random McKay graphs and run PerfectGen
    g6[0] = GraphConverter::getRandomGraph(size1);
    GraphConverter::graph6_to_igraph(g6[0], graph1);

    std::chrono::system_clock::time_point start, end;

    if (op != 5) {
        g6[1] = GraphConverter::getRandomGraph(size2);
        GraphConverter::graph6_to_igraph(g6[1], graph2);
        std::cout << "graph1: " << g6[0] << std::endl;
//      printAdjacencyMatrix(&graph[0]);
        std::cout << "graph2: " << g6[1] << std::endl;
//      printAdjacencyMatrix(&graph[1]);

        start = std::chrono::system_clock::now();
        PerfectGen::perfectGen(result, op, graph1, graph2);
        end = std::chrono::system_clock::now();
    }
    else {
        std::cout << "graph1: " << g6[0] << std::endl;
        start = std::chrono::system_clock::now();
        PerfectGen::perfectGen(result, op, graph1, NULL);
        end = std::chrono::system_clock::now();
    }
    std::chrono::duration<double> elapsed_seconds = end - start;
    std::cout << "Elapsed time: " << elapsed_seconds.count() << "s" << std::endl;
    return std::tuple<int,double>{op, elapsed_seconds.count()};
}
int main(int argc, char *argv[]) {
    srand(time(0));

    igraph_t graph1, graph2, result;
    std::string g6[2], g6_result;
    int size1, size2, op, graphCount;
    
    // Parse command line arguments
    getArguments(&size1, &size2, &op, &graphCount);
    for (int i = 1; i <= graphCount; i++) {
        std::tuple<int,double> op_N_elapsedTime = runPerfectGen(i, size1, size2, op, &graph1, &graph2, &result);
        generateOutput(&result, std::get<double>(op_N_elapsedTime));
        std::cout << std::endl;
        igraph_destroy(&result);
        igraph_destroy(&graph1);
        if (std::get<int>(op_N_elapsedTime) != 5)
            igraph_destroy(&graph2);
    }
    
    // std::cout << "Perfectness Check..." << std::endl;
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