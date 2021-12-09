#include "deluanay.hh"
#include "utils.hh"
#include "vertex.hh"
#include "edge.hh"
#include "quadedge.hh"
#include <tuple>
#include <vector>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <functional>
#include <memory>
#include <stdbool.h>




void Delaunay::initializeVertices(double low_bound, double up_bound, int N)
{
    std::ofstream myfile;
    myfile.open("coordinates.txt");
    std::vector<double> x_coords = getRandomVector(low_bound, up_bound, N);
    std::sort(x_coords.begin(), x_coords.end());
    std::vector<double> y_coords = getRandomVector(low_bound, up_bound, N);
    double x, y;
    for(int i{0}; i < N; i++)
    {
        x = x_coords[i];
        y = y_coords[i];
        myfile << x << ' ' << y << std::endl;
        Vertex* v = nullptr;
        v = Vertex::make(x,y);
        vertices.push_back(v);
    }
    myfile.close();
}


Delaunay::Delaunay(double low_bound, double up_bound, int N)
{
    edges = QuadEdgeList();
    initializeVertices(low_bound, up_bound, N);
}

void Delaunay::kill(Edge* e)
{
    Edge::splice(e, e->Oprev());
    Edge::splice(e->Sym(), e->Sym()->Oprev());
    QuadEdge* free_edge = (QuadEdge*) (e - (e->getIndex()));
    edges.erase(std::remove(edges.begin(), edges.end(), free_edge));
    delete free_edge;
}

VertexHalves Delaunay::SplitVertices(const VertexList& points)
{
    VertexList Left(points.begin(), points.begin() + points.size()/2);
    VertexList Right(points.begin() + points.size()/2, points.end());

    return VertexHalves(Left, Right);
}

Edge* Delaunay::connectTwoVertices(int first, int second, const VertexList& points)
{
    Edge* e = Edge::make(edges);

    e->setOrg(points[first]);

    e->setDest(points[second]);

    return e;
}

Edge* Delaunay::connect(Edge* a, Edge* b)
{
   Edge* e = Edge::make(edges);
   e->setOrg(a->Dest());

   e->setDest(b->Org());

   Edge::splice(e, a->Lnext());

   Edge::splice(e->Sym(), b);

   return e;

}

EdgeHalves Delaunay::makeLine(const VertexList& points)
{
    Edge* e = connectTwoVertices(0, 1, points);

    return EdgeHalves({ e }, { e->Sym() });
}


EdgeHalves Delaunay::makeTriangle(const VertexList& points)
{
    Edge* a = connectTwoVertices(0, 1, points);
    Edge* b = connectTwoVertices(1, 2, points);

    Edge::splice(a->Sym(), b);

    if (CCW(points[0], points[1], points[2]))
    {

        Edge* c = connect(b, a);
        return EdgeHalves( { a }, { b->Sym() });

    }
    else if (CCW(points[0], points[2], points[1]))
    {

        Edge* c = connect(b, a);
        return EdgeHalves({ c->Sym()}, { c });

    }
    else
    {
        return EdgeHalves({ a }, { b->Sym() });
    }
}


Edge* Delaunay::lowestTangentLoop(Edge*& left_inner, Edge*& right_inner)
{

    while (1)
    {

        if (LeftOf(right_inner->Org(), left_inner))
        {

            left_inner = left_inner->Lnext();

        }
        else if(RightOf(left_inner->Org(), right_inner))
        {

            right_inner = right_inner->Rprev();

        }
        else
        {

            break;

        }

    }
    Edge* base = connect(right_inner->Sym(), left_inner);
    return base;

}


Edge* Delaunay::lCand(Edge* base)
{

    Edge* lcand = base->Sym()->Onext();

    if(Valid(lcand, base))
    {

        while(inCircle(base->Dest(), base->Org(), lcand->Dest(), lcand->Onext()->Dest()))
        {

            Edge* t = lcand->Onext();
            kill(lcand);
            lcand = t;
        }

    }

    return lcand;

}

Edge* Delaunay::rCand(Edge* base)
{
	Edge* rcand = base->Oprev();

	if (Valid(rcand, base))
	{
		while (inCircle(base->Dest(), base->Org(), rcand->Dest(), rcand->Oprev()->Dest()))
		{
			Edge* t = rcand->Oprev();
			kill(rcand);
			rcand = t;
		}
	}

	return rcand;

}

void Delaunay::mergeLoop(Edge*& basel)
{
    while (1)
    {

        Edge* lcand = lCand(basel);
        Edge* rcand = rCand(basel);

    if (!Valid(lcand, basel) && !Valid(rcand, basel))
    {

                break;

    }

    else if (!Valid(lcand, basel) || (Valid(rcand, basel)
            && inCircle(lcand->Dest(), lcand->Org(), rcand->Org(), rcand->Dest())))
    {

            basel = connect(rcand, basel->Sym());

    }
    else

    {

        basel = connect(basel->Sym(), lcand->Sym());

    }


    }
}
EdgeHalves Delaunay::triangulate(const VertexList& points)
{

    if (points.size() == 2)
    {

        return makeLine(points);

    }
    if (points.size() == 3)
    {

        return makeTriangle(points);

    }

    VertexHalves split_points = SplitVertices(points);

    EdgeHalves left = triangulate(std::get<0>(split_points));
    EdgeHalves right = triangulate(std::get<1>(split_points));

    Edge* right_inner = std::get<0>(right)[0];
    Edge* left_inner = std::get<1>(left)[0];

    Edge* left_outer = std::get<0>(left)[0];
    Edge* right_outer = std::get<1>(right)[0];

    Edge* base = lowestTangentLoop(left_inner, right_inner);

    if (sameVertex(left_inner->Org(), left_outer->Org()))
    {

        left_outer = base->Sym();

    }
    if(sameVertex(right_inner->Org(), right_outer->Org()))
    {

        right_outer = base;

    }

    mergeLoop(base);

    return EdgeHalves({left_outer}, {right_outer});
}

QuadEdgeList Delaunay::getTriangulation()
{

    EdgeHalves edge_tuple = triangulate(vertices);
    return edges;

}

