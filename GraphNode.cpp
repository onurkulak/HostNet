/*
21401315
Onur Kulaksızoğlu
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
