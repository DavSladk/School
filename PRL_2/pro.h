#ifndef PRO_H
#define PRO_H

#include <mpi.h>
#include <iostream>
#include <string>
#include <vector>

#define DEBUG_IO(f) if(DEBUG_ON) {f;}
#define DEBUG_ON false

struct edge
{
    char startState;
    char endState;
};

struct adjElement
{
    int edge;
    int reverse;
    adjElement* next; 
};

#endif // PRO_H