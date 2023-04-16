#include <igraph.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <chrono>
#include <iomanip>
#include <oylum.h>
#include <graphconverter.h>

using namespace std;

int main(int argc, char *argv[])
{
    vector<string> graph6_strings;
    ifstream infile("mc_bfs_output_100_0.4.g6");
    string line;
    while (getline(infile, line)) {
        graph6_strings.push_back(line);
    }

    cout << "Graph\t\t\t\t\tigraph\toylum" << endl;
    for (const auto& graph6_string : graph6_strings) {
        cout << graph6_string << "\t";
        igraph_t graph;
        GraphConverter::graph6_to_igraph(graph6_string, &graph);

        igraph_bool_t isPerfect;
        auto start = chrono::steady_clock::now();
        igraph_is_perfect(&graph, &isPerfect);
        auto end = chrono::steady_clock::now();
        auto elapsed = chrono::duration_cast<chrono::microseconds>(end - start);
        cout << fixed << setprecision(2) << elapsed.count() / 1000.0 << "ms\t";

        PerfectnessChecker pChecker;
        pChecker.ReadGraph(&graph);
        start = chrono::steady_clock::now();
        isPerfect = pChecker.CheckPerfectness();
        end = chrono::steady_clock::now();
        elapsed = chrono::duration_cast<chrono::microseconds>(end - start);
        cout << fixed << setprecision(2) << elapsed.count() / 1000.0 << "ms" << endl;

        igraph_destroy(&graph);
    }

    return 0;
}
