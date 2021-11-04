
#include <list>
#include <ostream>
#include <iostream>
#include <chrono>
#include <random>
#include <cmath>
#include <string>
#include <bitset>

#include "VectorElement.h"
#include "TableF.h"
#include "HyperCube.h"

HyperCube::HyperCube(int k_arg, int v_size){

  //change this
  int w_arg = 4;

  dims = pow(2,k_arg) 
  this->buckets = dims;
  table = new list<VectorElement* >[buckets];

  this->array_of_v = new double *[k_arg];
  for (unsigned k = 0; k < k_arg; k++)
  {
    this->array_of_v[k] = new double[v_size];
  }

  unsigned seed = chrono::steady_clock::now().time_since_epoch().count();
  default_random_engine e(seed);
  normal_distribution<double> N(0, 1);

  for (unsigned k = 0; k < k_arg; k++)
  {
    double *inner_array = this->array_of_v[k];
    for (unsigned column = 0; column < v_size; column++)
    {
      double n_val = N(e);
      inner_array[column] = n_val;
    }
  }

  int open_w = w_arg - 1;
  uniform_real_distribution<> U(0.0, open_w);
  this->array_of_t = new double[k_arg];
  for (int k = 0; k < k_arg; k++)
  {
    double t = U(e);
    this->array_of_t[k] = t;
  }


  TableOfValuesF = new TableF *[k_arg];
  for (int i = 0; i < k_arg; i++){
    TableOfValuesF[i] = new TableF(NUMBER_OF_BUCKETS, how_many_columns);
   }

    
}


int HyperCube::functionF(int h, int i){

    unsigned seed = chrono::steady_clock::now().time_since_epoch().count();
    default_random_engine e(seed);
    uniform_int_distribution<> U(0,1);

    int checkpos = TableOfValuesF[i]->checkItem(h);

    if (checkpos == -1){
        int bit = U(e);
        TableOfValuesF[i]->insertItem(h,bit);
        return bit;
    }else{
        return checkpos;
    }


}

int HyperCube::indexBuilder(VectorElement *key){

    string index_s = "";
    int *h_array = new int[k_arg];
    for (int i = 0; i < k_arg; i++){
        double *array_start = key->arrayVectorElement;
        double inner_prod = inner_product(array_start, array_start + (key->size), this->array_of_v[k], 0.0); //~=[-11,11]
        h_array[k] = floor((inner_prod + array_of_t[k]) / w_arg); 
    }

    for (int i = 0; i < k_arg; i++){
        int bit = functionF(h[i], i);
        string bit_s = to_string(bit);    
        string += bit_s;
    }

    cout << "int " << bitset<k_arg>(index_s).to_ulong();

    long index = bitset<k_arg>(index_s).to_ulong();

    return (int)index;


}


void HyperCube::insertItem(VectorElement *key){

    int index = indexBuilder(key)
    table[index].push_back(item);

}

void HyperCube::displayHash(){
    for (int i = 0; i < buckets; i++){
    cout << "INDEX: " << i << endl;
    for (auto x : table[i])
    {
      cout << " --> " << x << endl;
    }
  }

}

~HyperCube::HyperCube(){

    //change this 
    int k_arg = 3;

    for (unsigned k = 0; k < k_arg; k++)
    {
        delete[] this->array_of_v[k];
    }

    delete[] this->array_of_v;
    delete[] this->array_of_t;

    for (int i = 0; i < k_arg; i++){
        delete TableOfValuesF[i];
    }
    delete[] TableOfValuesF;

    delete[] table;
}

//f

//indexBuilder

//insert

//des