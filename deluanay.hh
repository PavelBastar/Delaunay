#ifndef DALUANAY_HH_INCLUDED
#define DALUANAY_HH_INCLUDED

#include "vertex.hh"
#include "utils.hh"
#include "edge.hh"
#include "quadedge.hh"
#include "VertexEdgeManager.hh"
#include <tuple>
#include <vector>
#include <stdlib.h>
#include <iostream>
#include <string>

typedef std::vector<Edge*>                     EdgeList;
typedef std::tuple<EdgeList, EdgeList>         EdgeHalves;
typedef std::tuple<VertexList, VertexList>     VertexHalves;


class Delaunay
{
private:
    VertexEdgeManager*      subdivision;

    void initializeVertices(double low_bound, double up_bound, int N);

    VertexHalves              SplitVertices(const VertexList& points);

    Edge*                     connectTwoVertices(int first, int second, const VertexList& points);



    EdgeHalves                makeLine(const VertexList& points);

    EdgeHalves                makeTriangle(const VertexList& points);

    Edge*                     lowestTangentLoop(Edge*& left_inner, Edge*& right_inner);

    Edge*                     lCand(Edge* base);

    Edge*                     rCand(Edge* base);

    void                      mergeLoop(Edge*& base);

    EdgeHalves                triangulate(const VertexList& points);
    int                       file_index;
    void                      getVoronoiDiagram();

public:

    Delaunay(double low_bound, double up_bound, int N);

    void            getTriangulation();
    ~Delaunay();
    void                     gatherTriangles();
    std::string       generateFileName();


};


#endif // DALUANAY_HH_INCLUDED
