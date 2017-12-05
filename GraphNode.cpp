/*
21401315
Onur Kulaksýzoðlu
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
