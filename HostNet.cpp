/*
21401315
Onur Kulaksýzoðlu
Section 2
HW5
*/




#include "HostNet.h"
#include <fstream>
#include <iostream>
#include <cmath>


//dtor
HostNet::~HostNet()
{
    for(int i=0; i<graphSize; i++)
        delete graph[i];
    delete[] graph;
}

//ctor, connects all the nodes, holds all the pointers of the nodes in "graph" array
HostNet::HostNet(const string hName)
{
    ifstream file;
    file.open(hName);
    file>>graphSize;
    graph = new GraphNode*[graphSize];
    int** ids = new int*[graphSize];
    for(int i=0; i<graphSize; i++)
    {
        graph[i]=new GraphNode();
        file>>graph[i]->id;
        file>>graph[i]->name;
        int numberOfConnections;
        file>>numberOfConnections;
        graph[i]->numberOfConnections=numberOfConnections;
        graph[i]->connections=new GraphNode*[numberOfConnections];
        ids[i]=new int[numberOfConnections];
        for(int j=0; j<numberOfConnections; j++)
            file>>ids[i][j];
    }

    for(int i=0; i<graphSize; i++)
        for(int j=0; j<graph[i]->numberOfConnections; j++)
            graph[i]->connections[j]=graph[findById(ids[i][j])];
    for(int i=0; i<graphSize; i++)
        delete[] ids[i];
    delete[] ids;
}

HostNet::HostNet(const HostNet& hNet)
{

}



void HostNet::listCommunicationsLowerThan(const string aname, const int cost)
{
    int order=findByName(aname);
    if(order==-1)
    {//if element is not in there return failure
        cout<<"\""<<aname<<"\" "<<"does not exist in the host network."<<endl;
    }
    else
    {
        GraphNode** visited=new GraphNode*[graphSize];
        int visitedCount=0;

        //visit lower than method returns all the visited nodes in visited array within the given cost
        visitLowerThanCost(graph[order], cost, visited, visitedCount);

        cout<<"From \""<<aname<<"\" "<<visitedCount-1<< " hosts are directly reachable with cost lower than "<<cost<<":"<<endl;
        for(int i=1; i<visitedCount; i++)
            cout<<visited[i]->name<<", ";
        cout<<endl;

        delete[] visited;
    }
}

//from the graph array, returns the order of the element with given id
int HostNet::findById(int id)
{
    for(int i=0; i<graphSize; i++)
        if(graph[i]->id==id)
            return i;
    return -1;
}

//from the graph array, returns the order of the element with given name
int HostNet::findByName(string name)
{
    for(int i=0; i<graphSize; i++)
    {
        if(!graph[i]->name.compare(name))
            return i;
    }
    return -1;
}

//returns true if given pointer is in the given pointer array
bool HostNet::doesInclude(GraphNode** visitedIds, int size, GraphNode* value)
{
    for(int i=0; i<size; i++)
        if(visitedIds[i]==value)
            return true;
    return false;
}

//from the given landlord node, visits all the elements with a length smaller than cost
//adds visited items to the visited nodes array
//returns number of visited nodes in visited current
//works recursively
void HostNet::visitLowerThanCost(GraphNode* landlord, int cost, GraphNode** visitedNodes, int& visitedCurrent)
{
    if(cost>0&&!doesInclude(visitedNodes, visitedCurrent, landlord))
    {
        visitedNodes[visitedCurrent]=landlord;
        visitedCurrent++;
        if(cost!=1)
            for(int i=0; i<landlord->numberOfConnections; i++)
                visitLowerThanCost(landlord->connections[i], cost-1, visitedNodes, visitedCurrent);
    }
}



void HostNet::listCommunicationsOnHubHost(const string hname)
{
    int order = findByName(hname);
    if(order==-1)
    {
        cout<<"\""<<hname<<"\" "<<"does not exist in the host network."<<endl;
    }
    else
    {
        //if one of the neighbors of the node "does not include" another neighbor
        //prints these two neighbors
        cout<<"If "<<hname<<" is considered as hub these routes are possible:"<<endl;
        GraphNode* hub = graph[order];
        for(int i=0; i<hub->numberOfConnections; i++)
            for(int j=i+1; j<hub->numberOfConnections; j++)
                if(!doesInclude(hub->connections[i]->connections, hub->connections[i]->numberOfConnections, hub->connections[j]))
                    cout<<"<"<<hub->connections[i]->name<<", "<<hub->connections[j]->name<<">"<<endl;
    }
}

void HostNet::findConnectedComponents()
{
    //cost is just a high number, so we can assume algorithm iterates all elements of the component
    int cost = 1000;
    GraphNode** visited = new GraphNode* [graphSize];
    int preVisited=0;
    int curVisited=0;
    int * componentSizes = new int[graphSize];
    int componentCount=0;
    for(int i=0; i<graphSize; i++)
    {
        //for every element in the graph
        //calls visit lower than method with same parameters
        //if number of visited nodes is changed that means we found a new component
        visitLowerThanCost(graph[i], cost, visited, curVisited);
        if(curVisited!=preVisited)
        {
            componentSizes[componentCount]=curVisited-preVisited;
            preVisited=curVisited;
            componentCount++;
        }
    }
    cout<<"There are "<<componentCount<<" connected components:"<<endl;
    for(int i=0; i<componentCount; i++)
        cout<<"For component "<<i+1<<": "<<componentSizes[i]<<" nodes"<<endl;
    delete[] visited;
    delete[] componentSizes;
}

void HostNet::displayMostCentralHosts()
{
    int cost = graphSize;
    GraphNode** visited = new GraphNode* [graphSize];
    int preVisited=0;
    int curVisited=0;
    int * componentSizes = new int[graphSize];
    int componentCount=0;
    for(int i=0; i<graphSize; i++)
    {
        visitLowerThanCost(graph[i], cost, visited, curVisited);
        if(curVisited!=preVisited)
        {
            //first finds components separately, then calls the method on this component only
            componentSizes[componentCount]=curVisited-preVisited;
            string mostCentral=findCentralized(visited, preVisited, curVisited);

            cout<<"Most central node in component "<<componentCount+1<<": "<<mostCentral<<endl;

            preVisited=curVisited;
            componentCount++;
        }
    }
    delete[] componentSizes;
    delete[] visited;
}



string HostNet::findCentralized(GraphNode** nodes, int start, int end)
{
    int size=end-start;
    //center values will hold the betweenness value for each node
    double* centerValues = new double[size]();

    //from every node to every other node in given array
    for(int i=start; i<end; i++)
        for(int j=i+1; j<end; j++)
        {

            //first finds the minimum distance between to nodes calling lower cost method iteratively
            int minDistance;
            for(int k=2; k<size; k++)
            {
                GraphNode** visited = new GraphNode*[size];
                int visitedCurrent=0;
                visitLowerThanCost(nodes[i], k, visited, visitedCurrent);
                if(doesInclude(visited, visitedCurrent, nodes[j]))
                {
                    minDistance=k;
                    delete[] visited;
                    break;
                }
                delete[] visited;
            }

            //considers all the possible paths with given distance, if successful increments the corresponding
            //number in occurrences array, also increments total successful paths for division
            int pointer=0;
            int totalSuccessfulPaths=0;
            int * occurrences=new int[size]();
            GraphNode** thePath=new GraphNode*[minDistance];
            hittingHeadsLikeHydra(nodes[i], thePath, pointer, minDistance, totalSuccessfulPaths, nodes[j], occurrences, nodes+start, size);
            for(int k=0; k<size; k++)
            {
                //adds to the corresponding center values position
                centerValues[k]+=occurrences[k]/(double)totalSuccessfulPaths;
            }
            delete[] occurrences;
            delete[] thePath;
        }
    double max=0;
    int index=0;
    for(int i=0; i<size; i++)
        if(centerValues[i]>max)
        {
            max=centerValues[i];
            index=i;
        }
    //returns the node with highest value in corresponding center value array
    delete[] centerValues;
    return nodes[start+index]->name;
}


//considers all the paths with given distance, if last added node is the key we are searching
//returns success, and increment occurrences, else keeps iterating with new nodes
//pointer holds the position of last added element to the path
bool HostNet::hittingHeadsLikeHydra(GraphNode* cur, GraphNode** thePath,int & pointer, int distance, int & success, GraphNode* key, int * occurrences, GraphNode** nodes, int size)
{
    thePath[pointer]=cur;
    pointer++;
    if(pointer==distance)
    {
        if(cur==key)
        {
            success++;
            incrementOccurrences(occurrences, thePath, distance, nodes, size);
            pointer--;
            return true;
        }
        pointer--;
        return false;
    }
    else
    {
        for(int i=0; i<cur->numberOfConnections; i++)
            if(hittingHeadsLikeHydra(cur->connections[i], thePath, pointer, distance, success, key, occurrences, nodes, size))
                break;
        pointer--;
        return false;
    }

}

//increments the occurrences[index] where index  is the indexes of path elements in the allNodes array
void HostNet::incrementOccurrences(int * occurrences, GraphNode** thePath, int distance, GraphNode** allNodes, int size)
{
    for(int i=0; i<distance; i++)
        for(int j=0; j<size; j++)
            if(thePath[i]==allNodes[j])
            {
                occurrences[j]++;
                break;
            }
}


void HostNet::displayAverageClusteringCoefficients()
{
    int cost = graphSize;
    GraphNode** visited = new GraphNode* [graphSize];
    int preVisited=0;
    int curVisited=0;
    int * componentSizes = new int[graphSize];
    int componentCount=0;
    for(int i=0; i<graphSize; i++)
    {
        visitLowerThanCost(graph[i], cost, visited, curVisited);
        if(curVisited!=preVisited)
        {
            //first divides components, then calculates clustering separately
            componentSizes[componentCount]=curVisited-preVisited;
            double clustering=findClusteringForComponent(visited, preVisited, curVisited);

            cout<<"Average clustering coefficient for component "<<componentCount+1<<": "<<clustering<<endl;

            preVisited=curVisited;
            componentCount++;
        }
    }
    delete[] componentSizes;
    delete[] visited;
}

//find clustering for individual nodes and takes average
double HostNet::findClusteringForComponent(GraphNode** nodes, int start, int end)
{
    double result=0;
    for(int i=start; i<end; i++)
        result+=findClusteringOfVertex(nodes[i]);
    return result/(end-start);
}

//clustering of a vertex is equal to the equation in else part,
//it calls a similar method to the find hubs method
double HostNet::findClusteringOfVertex(GraphNode* vertex)
{
    if(vertex->numberOfConnections<2)
        return 0;
    else
    {
        return 1-2.0*getHubCount(vertex)/(vertex->numberOfConnections*(vertex->numberOfConnections-1));
    }
}


//returns the number of hub occurrences of this vertex
int HostNet::getHubCount(GraphNode* hub)
{
    int hubCount=0;
    for(int i=0; i<hub->numberOfConnections; i++)
        for(int j=i+1; j<hub->numberOfConnections; j++)
            if(!doesInclude(hub->connections[i]->connections, hub->connections[i]->numberOfConnections, hub->connections[j]))
                hubCount++;
    return hubCount;
}


