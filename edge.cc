
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "edge.hh"
#include "face.hh"
#include "vertex.hh"
#include <vector>
#include <memory>




void Edge::splice(Edge* a, Edge* b)
{
  assert(a!=0);
  assert(b!=0);

  // see Guibas and Stolfi

  Edge* alpha = a->Onext()->Rot();
  Edge* beta  = b->Onext()->Rot();

  Edge* t1 = b->Onext();
  Edge* t2 = a->Onext();
  Edge* t3 = beta->Onext();
  Edge* t4 = alpha->Onext();

  a->next     = t1;
  b->next     = t2;
  alpha->next = t3;
  beta->next  = t4;
}






void Edge::setOrg(Vertex* org)
{
  // add this edge to the (vertex) orbit of _org_

  vertex = org;

  org->addEdge(this);
}

void Edge::setDest(Vertex* dest)
{
  // add this edge to the (vertex) orbit of _dest_

  Sym()->vertex = dest;

  dest->addEdge(Sym());
}





Edge::Edge()
{
  // _index_ is initialized by QuadEdge
  // _next_ is initialized by QuadEdge
  // _id_ is initialized by QuadEdge


  vertex = 0;
}

Edge::~Edge()
{
}





