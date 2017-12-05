/*
21401315
Onur Kulaksýzoðlu
Section 2
HW5
*/




#ifndef HOSTNET_H
#define HOSTNET_H
#include "GraphNode.h"
#include <string>
using namespace std;
class HostNet
{
public:
    HostNet(const string hName);
    HostNet(const HostNet& hNet); // copy contructor
    ~HostNet(); // destructor
    void listCommunicationsLowerThan(const string aname, const int cost);
    void listCommunicationsOnHubHost(const string hname);
    void findConnectedComponents();
    void displayMostCentralHosts();
    void displayAverageClusteringCoefficients();
protected:

private:
    GraphNode** graph;
    int graphSize;
    int findById(int id);
    int findByName(string name);
    void visitLowerThanCost(GraphNode* landlord, int cost, GraphNode** visitedNodes, int& visitedCurrent);
    bool doesInclude(GraphNode** visitedIds, int size, GraphNode* value);
    string findCentralized(GraphNode** nodes, int start, int end);
    bool hittingHeadsLikeHydra(GraphNode* cur, GraphNode** thePath,int & pointer, int distance, int & success, GraphNode* key, int * occurrences, GraphNode** nodes, int size);
    void incrementOccurrences(int * occurrences, GraphNode** thePath, int distance, GraphNode** allNodes, int size);
    double findClusteringOfVertex(GraphNode* vertex);
    double findClusteringForComponent(GraphNode** nodes, int start, int end);
    int getHubCount(GraphNode* hub);

};

#endif // HOSTNET_H
