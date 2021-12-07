#include "utils.hh"
#include <iostream>
#include <stdbool.h>
#include <cassert>
#include <vector>
#include <algorithm>
#include <functional>
#include <iterator>
#include <random>
#include <fstream>
#include <cstdio>
#include "vertex.hh"
#include "edge.hh"

bool CCW(Vertex* A, Vertex* B, Vertex* C)
{
    double x_A, x_B, x_C, y_A, y_B, y_C, result;
    x_A = A->getX();

    x_B = B->getX();

    x_C = C->getX();

    y_A = A->getY();

    y_B = B->getY();

    y_C = C->getY();

    result = x_B*y_C - y_B*x_C -x_A*y_C + y_A*x_C + x_A*y_B - x_B*y_A;

    return result > 0.0;
}

bool RightOf(Vertex* X, Edge* e)
{
    return CCW(X, e->Dest(), e->Org());
}

bool LeftOf(Vertex* X, Edge* e)
{
    return CCW(X, e->Org(), e->Dest());
}

bool Valid(Edge* e, Edge* basel)
{
    return CCW(e->Dest(), basel->Dest(), basel->Org());
}


bool inCircle(Vertex* A, Vertex* B, Vertex* C, Vertex* D)
{
    double ax, ay, bx, by, cx, cy, dx, dy;
    ax = A->getX(); ay = A->getY();
    bx = B->getX(); by = B->getY();
    cx = C->getX(); cy = C->getY();
    dx = D->getX(); dy = D->getY();

    double adx, ady, bdx, bdy, cdx, cdy;

    adx = ax - dx; ady = ay - dy;
    bdx = bx - dx; bdy = by - dy;
    cdx = cx - dx; cdy = cy - dy;

    double abdet, bcdet, cadet, alift, blift, clift;

    abdet = adx * bdy - bdx * ady;
    bcdet = bdx * cdy - cdx * bdy;
    cadet = cdx * ady - adx * cdy;
    alift = adx * adx + ady * ady;
    blift = bdx * bdx + bdy * bdy;
    clift = cdx * cdx + cdy * cdy;

    //cout << alift * bcdet + blift * cadet + clift * abdet << endl;
    return alift * bcdet + blift * cadet + clift * abdet > 0.0;

}

bool sameVertex(Vertex* a, Vertex* b)
{
    return (a->getX() == b->getX()) && (a->getY() == b->getY());
}


template<typename Generator>
void getrands(std::vector<double>& x, Generator& gen, int num)
{
  generate_n(std::back_inserter(x), num, std::ref(gen));
}

std::vector<double> getRandomVector(double low_bound, double up_bound, int N)
{
   std::uniform_real_distribution<double> unif(low_bound,up_bound);
   std::mt19937 re(std::random_device{}());
   auto generator = std::bind(unif, std::ref(re));
   std::vector<double> vs;
   getrands(vs, generator, N);
   //sort(vs.begin(), vs.end());
   return vs;
}


