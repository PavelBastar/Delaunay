#ifndef UTILS_HH_INCLUDED
#define UTILS_HH_INCLUDED
#include "vertex.hh"
#include "edge.hh"
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

bool CCW(Vertex* A, Vertex* B, Vertex* C);

bool RightOf(Vertex* X, Edge* e);


bool LeftOf(Vertex* X, Edge* e);



bool Valid(Edge* e, Edge* basel);


bool inCircle(Vertex* A, Vertex* B, Vertex* C, Vertex* D);


bool sameVertex(Vertex* a, Vertex* b);

template<typename Generator>
void getrands(std::vector<double>& x, Generator& gen, int num);

std::vector<double> getRandomVector(double low_bound, double up_bound, int N);


#endif // UTILS_HH_INCLUDED
