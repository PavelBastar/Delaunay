
#ifndef edgeINCLUDED
#define edgeINCLUDED
#include <vector>
#include <memory>
#include <stdbool.h>

class QuadEdge;
class Vertex;
class VertexEdgeManager;


class Edge
{



  public:


  static Edge* make(std::vector<QuadEdge*>& quadlist);



  /*
   * Splice a given pair of edges.
   * a, b -> the edges to splice;
   *         must be nonnull
   *
   * This operator affects the two edge rings around the origins of a and b,
   * and, independently, the two edge rings around the left faces of a and b.
   * In each case, (i) if the two rings are distinct, Splice will combine
   * them into one; (ii) if the two are the same ring, Splice will break it
   * into two separate pieces.
   * Thus, Splice can be used both to attach the two edges together, and
   * to break them apart. See Guibas and Stolfi (1985) p.96 for more details
   * and illustrations.
   */
  static void splice(Edge* a, Edge* b);


  public:


  Vertex* Org();


  Vertex* Dest();

  void setOrg(Vertex* org);

  void setDest(Vertex* dest);



  Edge* Rot();


  Edge* InvRot();


  Edge* Sym();


  Edge* Onext();


  Edge* Oprev();


  Edge* Dnext();


  Edge* Dprev();


  Edge* Lnext();


  Edge* Lprev();


  Edge* Rnext();


  Edge* Rprev();

  uint32_t getID();
  void setID(uint32_t id);





  Edge();


  ~Edge();



  private:


  uint32_t id;
  unsigned int index;

  Edge* next;

  Vertex* vertex;



  friend QuadEdge;
  friend VertexEdgeManager;

};

inline void Edge::setID(uint32_t id)
{
    this->id = id;
}

inline uint32_t Edge::getID()
{
    return this->id;
}



inline Edge* Edge::Rot()
{
  return index<3 ? this+1 : this-3;
}

inline Edge* Edge::InvRot()
{
  return index>0 ? this-1 : this+3;
}

inline Edge* Edge::Sym()
{
  return index<2 ? this+2 : this-2;
}

inline Edge* Edge::Onext()
{
  return next;
}

inline Edge* Edge::Oprev()
{
  return Rot()->Onext()->Rot();
}

inline Edge* Edge::Dnext()
{
  return Sym()->Onext()->Sym();
}

inline Edge* Edge::Dprev()
{
  return InvRot()->Onext()->InvRot();
}

inline Edge* Edge::Lnext()
{
  return InvRot()->Onext()->Rot();
}

inline Edge* Edge::Lprev()
{
  return Onext()->Sym();
}

inline Edge* Edge::Rnext()
{
  return Rot()->Onext()->InvRot();
}

inline Edge* Edge::Rprev()
{
  return Sym()->Onext();
}

inline Vertex* Edge::Org()
{
  return vertex;
}

inline Vertex* Edge::Dest()
{
  return Sym()->vertex;
}



#endif /* #ifndef edgeINCLUDED */

