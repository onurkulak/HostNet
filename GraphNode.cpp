/*
21401315
Onur Kulaks�zo�lu
Section 2
HW5
*/



#include "GraphNode.h"

GraphNode::GraphNode()
{
    //ctor
}

GraphNode::~GraphNode()
{
    delete[] connections;
}
