#include "oylum.h"

bool PerfectnessChecker::CheckPerfectness()
{
    //first check the original graph
    bool oddHoleInOriginal = DetectOddHole3();
    if (!oddHoleInOriginal)
        {//if no odd hole in original graph, then check its complement
            ConvertToComplement();
            bool oddHoleInComplement = DetectOddHole3();
            _complementChecked = true;
            if (oddHoleInComplement) {
                // This graph is not perfect because there is an odd hole in its complement!
                return false;
            }
            else {
                // This graph is perfect!!
                return true;
            }
        }
    else {
        // This graph is not perfect because there is an odd hole in it!
        return false;
    }
}
void PerfectnessChecker::ReadGraph(string inputFile)
{
    ifstream file(inputFile);
    int numNodes;
    file >> numNodes;
    _numNodes = numNodes;
    _incMatrix.resize(_numNodes);
    _adjList.resize(_numNodes);
    int numNeighbours, a;
    for (int i = 0; i < _numNodes; ++i){
        _incMatrix.at(i).resize(_numNodes);
        file >> numNeighbours;
        for (int j = 0; j < numNeighbours; ++j) {
            file >> a;
            _adjList.at(i).push_back(a);
            _incMatrix.at(i).at(a) = 1;
        }
    }
}
void PerfectnessChecker::ReadGraph(igraph_t *graph){
    _numNodes = igraph_vcount(graph);

    _incMatrix.resize(_numNodes);
    _adjList.resize(_numNodes);
    
    igraph_vector_int_t neighbors;
    igraph_vector_int_init(&neighbors, 0);
    
    for (igraph_integer_t node_id = 0; node_id < _numNodes; node_id++) {
        igraph_neighbors(graph, &neighbors, node_id, IGRAPH_ALL);

        _incMatrix.at(node_id).resize(_numNodes);

        for (igraph_integer_t j = 0; j < igraph_vector_int_size(&neighbors); j++) {
            igraph_integer_t neighbor_id = VECTOR(neighbors)[j];

            _adjList.at(node_id).push_back(neighbor_id);
            _incMatrix.at(node_id).at(neighbor_id) = 1;

        }
        igraph_vector_int_clear(&neighbors);
    }
    igraph_vector_int_destroy(&neighbors);
}

int PerfectnessChecker::FindAdmissableNeighbour(const vector<bool>& labels, vector<int>& nextNeighbourIndices, int currentNode)
{//returns the ID of the neighbour if any, -1 otherwise
    for (int i = nextNeighbourIndices.at(currentNode); i < _adjList.at(currentNode).size(); ++i) {
        if (!labels.at(_adjList.at(currentNode).at(i))) { //if that neigbour is not marked
            if (_adjList.at(currentNode).size() <= i)
                nextNeighbourIndices.at(currentNode) = -1;
            else
                nextNeighbourIndices.at(currentNode) = i + 1;
            return _adjList.at(currentNode).at(i);
        }
    }
    return -1;
}
void PerfectnessChecker::GoUp(vector<bool>& labels, vector<int>& list, vector<int>& nextNeighbourIndices)
{
    int lastNode = list.back();
    labels.at(lastNode) = false;
    nextNeighbourIndices.at(lastNode) = 0;
    list.pop_back();
}
bool PerfectnessChecker::SearchOddHoleStartingWithGivenNode2(int startNode)
{ //Starting from a given node, check for odd holes
    vector<int> nextNeighbourIndices;
    vector<bool> labels;
    vector<int> list;
    nextNeighbourIndices.resize(_numNodes, 0);
    labels.resize(_numNodes, false); //all nodes unmarked initially
    list.push_back(startNode);
    labels.at(startNode) = true;
    while (list.size() > 0.5) {
        int currentNode = list.back();
        int neighbour = FindAdmissableNeighbour(labels, nextNeighbourIndices, currentNode);
        if (neighbour < -0.5) {//if an admissable neighbour does not exist, then go up one step
                GoUp(labels, list, nextNeighbourIndices);
        }
        else {
            list.push_back(neighbour);
            labels.at(neighbour) = true;
            int i = list.size() - 3;
            bool chord = false;
            while (i > 0 && !chord) { // while the size of the list is at least 4 and there is no chord
                if (_incMatrix.at(list.at(i)).at(list.back()) > 0.5) { //if ith and last node in the list are connected to each other
                    if ((list.size() - 1 - i) > 3.5 && (list.size() - 1 - i) % 2 < 0.1) { //then this is already an odd hole
                        // ODD HOLE found! 
                        return true;
                    }
                    else {
                        chord = true;
                    }
                }
                --i;
            }
            if (list.size() > 2) {
                if (!chord)    {
                    if (_incMatrix.at(list.at(0)).at(list.back()) > 0.5) {
                        if (list.size() >= 5 && list.size() % 2 > 0.5) {
                            // ODD HOLE found!
                            return true;
                        }
                        else {
                            GoUp(labels, list, nextNeighbourIndices);
                        }
                    }
                }
                else {
                    GoUp(labels, list, nextNeighbourIndices);
                }
            }
        }
    }
    return false;
}
bool PerfectnessChecker::DetectOddHole3()
{
    for (int i = 0; i < _numNodes - 4; ++i) {
        //since we try to find odd cycles of length>=5,
        //we can skip the last four because even if the last four is included in an odd cycle,
        //we would need one node from the rest to complete such a hole, which would already be covered
        if (SearchOddHoleStartingWithGivenNode2(i)) {
            // THERE IS ODD HOLE IN THIS GRAPH!
            return true;
        }
    }
    // NO ODD HOLE IN THIS GRAPH!
    return false;
}
void PerfectnessChecker::ConvertToComplement()
{//converts the adj list and inc matrix of the original graph to those of its complement
    for (int i = 0; i < _numNodes; ++i) {
        _adjList.at(i).clear();
        for (int j = 0; j < _numNodes; ++j) {
            if (_incMatrix.at(i).at(j) > 0.5)
                _incMatrix.at(i).at(j) = 0;
            else {
                if (i != j) {
                    _incMatrix.at(i).at(j) = 1;
                    _adjList.at(i).push_back(j);
                }
            }
        }
    }
}
