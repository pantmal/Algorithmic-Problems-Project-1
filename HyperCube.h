
#ifndef HYPERCUBE_H
#define HYPERCUBE_H

#include <list>
#include <ostream>
#include <iostream>
#include <bits/stdc++.h>

#include "VectorElement.h"
#include "TableF.h"
#include "Neighbours.h"


using namespace std;

class HyperCube
{
    // static int* r_array;
    int buckets; // No. of buckets
    int k_cube;
    int w_cube;

    int N;
    int Ni;

    int M;
    int Mi;

    int probes;
    int probes_counter;

    // Pointer to an array containing buckets
    list<VectorElement * > *table;
    double **array_of_v;
    double *array_of_t;
    TableF **TableOfValuesF;

    //neighboursInfo **neighboursInfoTable;
    int hammingCounter;
    list<int> probesCandidates;
    set<int> visited;

public:
    neighboursInfo **neighboursInfoTable;


    HyperCube(int, int, int, int, int, int);
    void initNeighboursInfo(int);
    int functionF(int, int);
    int indexBuilder(VectorElement *x);
    void insertItem(VectorElement *x);
    void getFirstProbe(VectorElement *x, int );
    void getNextProbe(VectorElement *x, int , int );
    void calculateDistanceAndFindN(VectorElement *x, int , int );
    static bool cmp(VectorElement *L, VectorElement *R)
    {
        return L->distanceCurrQ < R->distanceCurrQ;
    }
    void displayCube();

    ~HyperCube();
};

#endif /* HYPERCUBE_H */
