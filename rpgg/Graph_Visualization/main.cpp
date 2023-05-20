#include <graphconverter.h>
#include <fstream>
#include <set>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <algorithm>

int main() {
    std::string graph;
    igraph_t g;
    std::string mcKayPath = getenv("MCKAY_PATH");
    std::ifstream input;

    for (int i = 10; i <= 100; i+=5) {
        srand(time(NULL));
        std::string inputFilePath = mcKayPath + "/perfect" + std::to_string(i) + ".g6";
        std::string inputCountFilePath = mcKayPath + "/graph_counts/perfect" + std::to_string(i) + ".txt";
        input.open(inputCountFilePath);

        int graphCount = 0;
        input >> graphCount;
        input.close();

        std::set<int> ids_set;

        while(ids_set.size() < 30) {
            int id = rand() % graphCount;
            ids_set.insert(id);
        }

        // Vector declared
        std::vector<int> ids_vec;
 
        // Using vector::assign
        ids_vec.assign(ids_set.begin(), ids_set.end());
        std::sort(ids_vec.begin(), ids_vec.end());

        int counter = 0;
        input.open(inputFilePath);
        int j = 0;
        while(counter < 30) {
            std::string outFileName = "/ie492_rpgg/rpgg/Graph_Visualization/plots/dot/plot" + std::to_string(i) + "-" + std::to_string(counter) + ".dot";
            FILE *outfile = fopen(outFileName.c_str(), "w");

            int id = ids_vec[counter];
            std::string g6;
            input >> g6;
            if (id == j) {
                counter++;
                GraphConverter::graph6_to_igraph(g6, &g);
                igraph_write_graph_dot(&g, outfile);
                fclose(outfile);
                igraph_destroy(&g);
            }
            j++; 
        }
        input.close();
    }
    return 0;
}