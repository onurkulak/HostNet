/*
21401315
Onur Kulaksýzoðlu
Section 2
HW5
*/



#ifndef GRAPHNODE_H
#define GRAPHNODE_H

#include <string>
using namespace std;

//graph node implementation, holds an array of neighbors
class GraphNode
{
    public:
        GraphNode();
        virtual ~GraphNode();
        int id;
        string name;
        int numberOfConnections;
        GraphNode** connections;
    protected:

    private:
};

#endif // GRAPHNODE_H
