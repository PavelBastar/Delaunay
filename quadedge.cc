#include "edge.hh"
#include "quadedge.hh"



  QuadEdge::QuadEdge()
  {
    edges[0].index = 0;
    edges[1].index = 1;
    edges[2].index = 2;
    edges[3].index = 3;

    edges[0].next = edges+0;
    edges[1].next = edges+3;
    edges[2].next = edges+2;
    edges[3].next = edges+1;
  }


Edge* Edge::make(std::vector<QuadEdge*>& quadlist)
{
   quadlist.push_back(new QuadEdge());
   return quadlist.back()->edges; // ->edges
}
