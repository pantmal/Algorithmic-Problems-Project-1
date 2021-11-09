
#ifndef LSHASH_H
#define LSHASH_H

#include <list>
#include <ostream>
#include <iostream>

#include "VectorElement.h"
#include "Helpers.h"
#include "Neighbours.h"

using namespace std;

class LSHash
{
public:
    static int hashNumber;
    int hashTableNumber;
    // static int* r_array;
    int buckets; // No. of buckets

    // Pointer to an array containing buckets
    list<VectorElement *> *table;
    neighboursInfo **neighboursInfoTable;
    double **array_of_v;
    double *array_of_t;
    int query_rows_field;

    static int k_arg;
    static int w_arg;

    int temp;

public:
    static void set_k_arg(int temp)
    {
        k_arg = temp;
    }
    static int get_k_arg()
    {
        return k_arg;
    }
    void calculateDistanceAndFindN(VectorElement *, int *r_array, int j, int N);
    void displayNeighbours(int);
    void RangeSearch(VectorElement*, int*, int, double);

public:
    LSHash(int V, int v_size);

    void initNeighboursInfo(int query_rows, int N);
    void DeleteNeighborsInfo();

    void insertItem(VectorElement *x, int *r_array);

    //void deleteItem(VectorElement* key);
    static bool cmp(VectorElement *L, VectorElement *R)
    {
        return L->distanceCurrQ < R->distanceCurrQ;
    }

    int AmplifiedHashFunction(VectorElement *x, int *r_array);
    unsigned int getQueryTrickId(VectorElement *x, int *r_array);
    void displayHash();

    ~LSHash();
};

#endif /* LSHASH_H */
