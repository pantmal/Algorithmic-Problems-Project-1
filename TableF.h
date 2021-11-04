
#ifndef TABLEF_H
#define TABLEF_H

#include <list>
#include <ostream>
#include <iostream>

// #include "VectorElement.h"
// #include "Helpers.h"
// #include "Neighbours.h"

using namespace std;

class TableF
{
    // static int* r_array;
    int buckets; // No. of buckets

    // Pointer to an array containing buckets
    list<int > *table;
    
    int temp;



public:
    TableF();

    void insertItem(double );
    void displayHash();

    ~TableF();
};

#endif /* TABLEF_H */
