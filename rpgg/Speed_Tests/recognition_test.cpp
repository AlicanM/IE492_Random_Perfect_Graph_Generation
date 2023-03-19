#include <igraph.h>
#include <iostream>
#include <ctime>
#include <oylum.h>

using namespace std;

int main(int argc, char *argv[])
{
    igraph_integer_t vcount = 50;
    igraph_real_t density = 0.3;

time_t timer;

    //for(){
        igraph_t graph;
        igraph_erdos_renyi_game(&graph, IGRAPH_ERDOS_RENYI_GNP,  vcount, density, IGRAPH_UNDIRECTED, IGRAPH_NO_LOOPS);
        //igraph_bipartite_game (&graph, nullptr, IGRAPH_ERDOS_RENYI_GNP, 30000, 70000, 0.3, 1, IGRAPH_UNDIRECTED, IGRAPH_ALL);
        
        igraph_bool_t isPerfect;
timer = time(NULL);
        igraph_is_perfect(&graph, &isPerfect);
timer = time(NULL) - timer;
        cout << timer << endl;

        PerfectnessChecker pChecker;
        pChecker.ReadGraph(&graph);

timer = time(NULL);
	    isPerfect = pChecker.CheckPerfectness();
timer = time(NULL) - timer;
        cout << timer << endl;

        igraph_destroy(&graph);
    //}
    
    return 0;
}
