
#ifndef HYPERCUBE_H
#define HYPERCUBE_H

#include <list>
#include <ostream>
#include <iostream>

#include "VectorElement.h"
#include "TableF.h"
// #include "Neighbours.h"

using namespace std;

class HyperCube
{
    // static int* r_array;
    int buckets; // No. of buckets

    // Pointer to an array containing buckets
    list<VectorElement * > *table;
    double **array_of_v;
    double *array_of_t;
    TableF **TableOfValuesF;

public:
    HyperCube(int, int);
    int functionF();
    int indexBuilder();
    void insertItem(VectorElement *x );
    void displayHash();

    ~HyperCube();
};

#endif /* HYPERCUBE_H */
