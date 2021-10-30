
#ifndef HASH_H
#define HASH_H

#include <list>
#include <ostream>
#include <iostream>

#include "VectorElement.h"
#include "Helpers.h"

using namespace std;

class Hash{
    // static int* r_array;
    int buckets;    // No. of buckets
 
    // Pointer to an array containing buckets
    list<VectorElement*> *table;

    double** array_of_v;
    double* array_of_t;
    // VectorElement * r_obj;  //vector r

    static int k_arg;
    static int w_arg;

    int temp;

public:
    static void set_k_arg(int temp){
        k_arg=temp;
    }
    static int get_k_arg(){
        return k_arg;
    }
public:


    Hash(int V, int v_size);
 
    void insertItem(VectorElement* x, int* r_array);
 
    //void deleteItem(VectorElement* key);
 
    int AmplifiedHashFunction(VectorElement* x, int* r_array);
 
    void displayHash();

    ~Hash();
};

#endif /* HASH_H */
