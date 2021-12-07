
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>



#include "edge.hh"
#include "vertex.hh"
#include <stdbool.h>


Vertex::Vertex(double x, double y)
{
    coordinates[0] = x;
    coordinates[1] = y;
    id = index++;
    traversed = false;
}
Vertex* Vertex::make(double x, double y)
{
    return new Vertex(x, y);
}


void Vertex::addEdge(Edge *edge)
{
  assert(edge!=0);

  this->edge = edge;
}

void Vertex::removeEdge(Edge *edge)
{
  assert(edge!=0);

  Edge *next = edge->Onext();

  this->edge = next!=edge ? next : 0;
}






