#include "deluanay.hh"
#include <tuple>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <unordered_set>




void Delaunay::initializeVertices(double low_bound, double up_bound, int N)
{
    std::ofstream myfile;
    myfile.open("./data/coordinates.txt");
    std::vector<double> x_coords = getRandomVector(low_bound, up_bound, N);
    std::sort(x_coords.begin(), x_coords.end());
    std::vector<double> y_coords = getRandomVector(low_bound, up_bound, N);
    double x, y;
    for(int i{0}; i < N; i++)
    {
        x = x_coords[i];
        y = y_coords[i];
        myfile << x << ' ' << y << std::endl;
        subdivision->createVertex(x, y);
    }
    myfile.close();
}


Delaunay::Delaunay(double low_bound, double up_bound, int N)
{
    subdivision = new VertexEdgeManager();
    file_index = 0;
    initializeVertices(low_bound, up_bound, N);
}



VertexHalves Delaunay::SplitVertices(const VertexList& points)
{
    VertexList Left(points.begin(), points.begin() + points.size()/2);
    VertexList Right(points.begin() + points.size()/2, points.end());

    return VertexHalves(Left, Right);
}

Edge* Delaunay::connectTwoVertices(int first, int second, const VertexList& points)
{
    Edge* e = subdivision->createEdge(points[first], points[second]);

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

        Edge* c = subdivision->connect(b, a);
        return EdgeHalves( { a }, { b->Sym() });

    }
    else if (CCW(points[0], points[2], points[1]))
    {

        Edge* c = subdivision->connect(b, a);
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
    Edge* base = subdivision->connect(right_inner->Sym(), left_inner);
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
            subdivision->removeEdge(lcand);
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
			subdivision->removeEdge(rcand);
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

            basel = subdivision->connect(rcand, basel->Sym());

    }
    else

    {

        basel = subdivision->connect(basel->Sym(), lcand->Sym());

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

    gatherTriangles();

    return EdgeHalves({left_outer}, {right_outer});
}

void Delaunay::getTriangulation()
{

    EdgeHalves edge_tuple = triangulate(subdivision->getVertices());
    getVoronoiDiagram();
    gatherTriangles();

}

 std::string Delaunay::generateFileName()
{
    file_index++;
    std::string filename_left = "./data/triangle_files/gathered_triangles_";
    std::string filename_right = ".txt";
    std::string file_number = std::to_string(file_index);
    std::string filename = filename_left + file_number + filename_right;
    return filename;
}

void Delaunay::gatherTriangles()
{

 std::ofstream triangle_file;
 triangle_file.open(generateFileName());
 std::unordered_set<Edge*> processedEdges;



 for (uint32_t i = 0; i < subdivision->getMaxEdges()*2; i++)
 {
    Edge* edge = subdivision->getEdge(i/2);
    if (!edge) continue;
    if (i % 2) edge = edge->Sym();

    Vertex* p1 = edge->Org();
    Vertex* p2 = edge->Lnext()->Org();
    Vertex* p3 = edge->Lprev()->Org();

    if (!CCW(p1, p2, p3))
        continue;
    if (processedEdges.count(edge) != 0)
        continue;

    triangle_file << p1->getID() << ' ' << p2->getID() << ' ' << p3->getID() << std::endl;

    processedEdges.insert(edge);
    processedEdges.insert(edge->Lnext());
    processedEdges.insert(edge->Lprev());
 }
 triangle_file.close();

}

void Delaunay::getVoronoiDiagram()
{
    for (uint32_t i = 0; i < subdivision->getNumEdges(); i++)
    {
        Edge* e = subdivision->getEdge(i);

        if (CCW(e->Org(), e->Dest(), e->Onext()->Dest()) &&
            CCW(e->Org(), e->Oprev()->Dest(), e->Dest()))
        {
           e->Rot()->setOrg(subdivision->circumCenter(e->Org(), e->Dest(), e->Onext()->Dest()));
           e->InvRot()->setOrg(subdivision->circumCenter(e->Org(), e->Oprev()->Dest(), e->Dest()));
        }

    }

    std::ofstream voronoi_file;
    voronoi_file.open("./data/dual_vertices.txt");

    const VertexList& voronoi_vertices = subdivision->getDualVertices();

    std::unordered_set<Vertex*> processed_vertices;

    for (Vertex* dual_vertex : voronoi_vertices)
    {
        if (processed_vertices.count(dual_vertex) != 0)
        {
            continue;
        }
        voronoi_file << dual_vertex->getX() << ' ' << dual_vertex->getY() << std::endl;
        processed_vertices.insert(dual_vertex);
    }

    voronoi_file.close();
}



Delaunay::~Delaunay()
{
    delete subdivision;
}






