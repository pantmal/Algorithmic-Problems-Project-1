
#ifndef HASH_H
#define HASH_H

#include <list>
#include <ostream>
#include <iostream>

#include "VectorElement.h"


using namespace std;

class Hash
{
    int BUCKET;    // No. of buckets
 
    // Pointer to an array containing buckets
    list<VectorElement*> *table;

    double** array_of_v;
    double* array_of_t;
    VectorElement * r_obj;
public:


    Hash(int V, int k_arg, int v_size);  // Constructor
 
    // inserts a key into hash table
    void insertItem(VectorElement* x);
 
    // deletes a key from hash table
    void deleteItem(VectorElement* key);
 
    // hash function to map values to key
    int hashFunction(VectorElement* x) {

        int size = x->size;
        int sum = 0;
        for (int i = 0; i < size; i++){
            sum += x->arrayVectorElement[i];
        }

        return (sum % BUCKET);
    }
 
    void displayHash();

    ~Hash();
};

#endif /* HASH_H */
