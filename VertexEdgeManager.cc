#include "VertexEdgeManager.hh"
#include <cstdlib>
#include <iostream>
#include "utils.hh"






VertexEdgeManager::VertexEdgeManager()
{
    vertices = VertexList();
    quad_edges = QuadEdgeList();
    vertex_flags = std::vector<bool>();
    edge_flags = std::vector<bool>();
    dual_vertices = VertexList();
}

uint32_t VertexEdgeManager::getMaxVertices() const
{
    return vertices_ID_manager.peekNextID();
}

uint32_t VertexEdgeManager::getNumVertices() const
{
    return vertices_ID_manager.numberUsedIDs();
}


Vertex* VertexEdgeManager::getVertex(uint32_t id)
{
    return vertex_flags[id] ? vertices[id] : NULL;
}




Vertex* VertexEdgeManager::createVertex(double x, double y)
{
    uint32_t id = vertices_ID_manager.getID();

     if (id >= vertices.size())
    {

        vertices.push_back(new Vertex());
        vertex_flags.push_back(false);
        vertices[id]->setID(id);
    }
    vertex_flags[id] = true;
    Vertex* v = vertices[id];
    v->setCoordinates(x, y);
    v->setID(id);
    return v;
}



uint32_t VertexEdgeManager::getMaxEdges() const
{
    return edges_ID_manager.peekNextID();
}

uint32_t VertexEdgeManager::getNumEdges() const
{
    return edges_ID_manager.numberUsedIDs();
}

Edge* VertexEdgeManager::getEdge(uint32_t id)
{
    return edge_flags[id] ? quad_edges[id]->edges : NULL;
}





Edge* VertexEdgeManager::createEdge(Vertex* a, Vertex* b)
{
    uint32_t id = edges_ID_manager.getID();
    if (id >= quad_edges.size())
    {
        std::cout << quad_edges.size() << std::endl;
        quad_edges.push_back(new QuadEdge());
        quad_edges[id]->edges[0].setID(id);
        quad_edges[id]->edges[1].setID(id);
        quad_edges[id]->edges[2].setID(id);
        quad_edges[id]->edges[3].setID(id);
        edge_flags.push_back(false);
    }


    edge_flags[id] = true;
    Edge* e = quad_edges[id]->edges;
    e->setOrg(a);
    e->setDest(b);
    quad_edges[id]->edges[1].vertex = NULL;
    quad_edges[id]->edges[3].vertex = NULL;
    return e;
}


void VertexEdgeManager::removeEdge(Edge* edge)
{

    if(!edge)
    {
        return;
    }

    if (edge->Org())
    {
        edge->Org()->removeEdge(edge);
    }

    if (edge->Dest())
    {
        edge->Dest()->removeEdge(edge->Sym());
    }
    Edge::splice(edge, edge->Oprev());
    Edge::splice(edge->Sym(), edge->Sym()->Oprev());

    edges_ID_manager.freeID(edge->getID());
    edge_flags[edge->getID()] = false;

}

Edge* VertexEdgeManager::connect(Edge* a, Edge* b)
{
    Edge* edge = createEdge(a->Dest(), b->Org());
    if(!edge)
    {
        return NULL;
    }

    Edge::splice(edge, a->Lnext());
    Edge::splice(edge->Sym(), b);

    return edge;
}

const VertexList& VertexEdgeManager::getVertices() const
{
    return vertices;
}


Vertex* VertexEdgeManager::circumCenter(Vertex* A, Vertex* B, Vertex* C)
{
    double x_A, x_B, x_C, y_A, y_B, y_C, denom, x_O, y_O;

    x_A = A->getX();  x_B = B->getX();   x_C = C->getX();

    y_A = A->getY();  y_B = B->getY();   y_C = C->getY();

    double a = 2*(x_B - x_A);
    double b = 2*(y_B - y_A);
    double c = 2*(x_C - x_A);
    double d = 2*(y_C - y_A);

    double b_1 = x_B*x_B - x_A*x_A + y_B*y_B - y_A*y_A;
    double b_2 = x_C*x_C - x_A*x_A + y_C*y_C - y_A*y_A;

    denom = determinant(a, b, c, d);

    x_O = determinant(b_1, b, b_2, d) / denom;

    y_O = determinant(a, b_1, c, b_2) / denom;

    Vertex* new_dual_vertex = new Vertex(x_O, y_O);
    dual_vertices.push_back(new_dual_vertex);

    return new_dual_vertex;
}


const VertexList& VertexEdgeManager::getDualVertices() const
{
    return dual_vertices;
}


VertexEdgeManager::~VertexEdgeManager()
{
    for (Vertex* vertex : vertices)
    {
        delete vertex;
    }

    for (QuadEdge* edge : quad_edges)
    {
        delete edge;
    }

}
