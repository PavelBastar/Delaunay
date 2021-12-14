#ifndef VERTEXEDGEMANAGER_HH_INCLUDED
#define VERTEXEDGEMANAGER_HH_INCLUDED
#include "vertex.hh"
#include "edge.hh"
#include "quadedge.hh"
#include "IDManager.h"
#include <vector>
typedef std::vector<Vertex*> VertexList;
typedef std::vector<QuadEdge*> QuadEdgeList;

class VertexEdgeManager
{
private:
    VertexList vertices;
    QuadEdgeList quad_edges;
    VertexList dual_vertices;

    IDManager              vertices_ID_manager;
    IDManager              edges_ID_manager;

    std::vector<bool>      vertex_flags;
    std::vector<bool>      edge_flags;


public:
    VertexEdgeManager();
    ~VertexEdgeManager();

    uint32_t getMaxVertices() const;

    uint32_t getNumVertices() const;

    Vertex* getVertex(uint32_t id);

    Vertex* createVertex(double x, double y);

    uint32_t getMaxEdges() const;

    uint32_t getNumEdges() const;

    Edge* getEdge(uint32_t id);

    Edge* createEdge(Vertex* a, Vertex* b);

    void removeEdge(Edge* edge);

    Edge* connect(Edge* a, Edge* b);

    const VertexList& getVertices() const;

    Vertex* circumCenter(Vertex* a, Vertex* b, Vertex* c);

    const VertexList& getDualVertices() const;

};



#endif // VERTEXEDGEMANAGER_HH_INCLUDED
