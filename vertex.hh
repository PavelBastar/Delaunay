
#ifndef vertexINCLUDED
#define vertexINCLUDED


#include "edge.hh"
#include <stdbool.h>



class Vertex
{

  public:

   int getID();


  void setID( int id);


  Edge *getEdge();


  void addEdge(Edge* edge);

  void removeEdge(Edge* edge);


    Vertex(double x, double y);

    static Vertex* make(double x, double y);

    ~Vertex();

    double getX();
    double getY();
    bool traversed;

private:

   double coordinates[2];
   int id;
   static int index;
   Edge* edge;


};



inline double Vertex::getX()
{
    return coordinates[0];
}

inline double Vertex::getY()
{
    return coordinates[1];
}

inline int Vertex::getID()
{
  return id;
}

inline Edge *Vertex::getEdge()
{
  return edge;
}


class VertexEdgeIterator
{



  public:


  VertexEdgeIterator(Vertex *vertex)
  {
    // pick an arbitrary edge in the vertex orbit

    start = vertex->getEdge();
    edge  = start;
  }


  ~VertexEdgeIterator()
  {
  }


  Edge *next()
  {
    // check for degeneracy or exhausted iteration

    Edge *current = edge;

    if (current==0)
	return 0;

    // get the next edge in the counterclockwise orbit of the vertex, but
    // return the current edge
    // reset to null if we've come back to the start

    Edge *next = current->Onext();

    edge = next!=start ? next : 0;

    return current;
  }


  private:


  Edge *start;


  Edge *edge;

};

#endif /* #ifndef vertexINCLUDED */

