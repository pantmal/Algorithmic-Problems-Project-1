#include <list>
#include <ostream>
#include <iostream>
#include <chrono> 
#include <random> 
#include <climits>

#include "Hash.h"
#include "VectorElement.h"
#include "Helpers.h"


using namespace std;

int Hash::k_arg=5;
int Hash::w_arg=4;

// int euclidean_mod(int a, unsigned int b){
//   int r = a % b;
//   return r >= 0 ? r : r + abs(b);
// }

unsigned int euclidean_mod(int a, unsigned int b){ //not the typical implementation of euclidian mod cause it may trigger overflows
  if( a<0) a=-a;
  unsigned int r= a % b;
  // return r >= 0 ? r : r + abs(b);
  return a%b;
}
 
Hash::Hash(int b, int v_size){

    this->buckets = b;
    table = new list<VectorElement*>[buckets];

    this->array_of_v = new double*[k_arg];
    for (unsigned k = 0; k < k_arg; k++){
      this->array_of_v[k] = new double[v_size];
    }

    unsigned seed = chrono::steady_clock::now().time_since_epoch().count(); 
    default_random_engine e (seed); 
    normal_distribution<double> N(0,1); 

    for (unsigned k = 0; k < k_arg; k++){
        double* inner_array = this->array_of_v[k];
        for (unsigned column = 0; column < v_size; column++){
            double n_val = N(e);
            inner_array[column] = n_val;
        }
    }

    int open_w = w_arg - 1;
    uniform_real_distribution<> U(0.0, open_w);
    this->array_of_t = new double[k_arg]; 
    for (int k=0; k<k_arg; k++){
      double t = U(e);
      this->array_of_t[k] = t;
    }

}

int Hash::AmplifiedHashFunction(VectorElement* key, int* r_array){
  //k_arg=5;
  //array_of_v=>[-3,3]
  //array_of_t=>[0,3] epireazetai apo to w
  //w=4
  //r_array=>[0,intmax]
  //h_array=> megales
  //pre-mod=>0
  //final-mod=>
  int *h_array = new int[k_arg];//k_arg=5
  for (int k=0; k<k_arg; k++){
    
    int* array_start = key->arrayVectorElement;
    double inner_prod = inner_product(array_start,array_start+(key->size),this->array_of_v[k],0.0); //~=[-11,11]
    h_array[k] = floor((inner_prod + array_of_t[k]) / w_arg);  //~=2-3
    //cout << h_array[k]<<endl;
  }

  int pre_mod = 0;
  unsigned int M = UINT_MAX - 5;
  for (int i=0; i<k_arg; i++){

    unsigned int euc_mod = euclidean_mod(r_array[i]*h_array[i],M);
    pre_mod += euc_mod;
  }

  //cout<<pre_mod<<endl;
  unsigned int final_mod =  euclidean_mod(pre_mod, M) ;
  //cout<<final_mod<<endl;
  unsigned int index = euclidean_mod(final_mod, buckets);

  delete [] h_array;

  return index;
}
 
void Hash::insertItem(VectorElement* key, int* r_array){

    int index = AmplifiedHashFunction(key, r_array);
    table[index].push_back(key);
}
 
//UNCOMMENT IN CASE WE WANT TO DELETE STUFF
// void Hash::deleteItem(VectorElement* key){
//   // get the hash index of key
//   int index = hashFunction(key);
 
//   // find the key in (index)th list
//   list <VectorElement*> :: iterator i;
//   for (i = table[index].begin();
//            i != table[index].end(); i++) {
//     if (*i == key)
//       break;
//   }
 
//   // if key is found in hash table, remove it
//   if (i != table[index].end())
//     table[index].erase(i);
// }
 
// function to display hash table
void Hash::displayHash() {


  // UNCOMMENT TO DISPLAY V AND T ARRAYS
  // for (unsigned h = 0; h < 5; h++){
  //      double* inner_array = array_of_v[h];
  //      cout << "In array:" << h << endl;
  //      for (unsigned w = 0; w < 5; w++){
  //          cout << inner_array[w] << endl;
  //      }
  // }

  // for (unsigned h = 0; h < 5; h++){
       
  //      cout << "In t array:" << this->array_of_t[h] << endl;
      
  // }

  //myLogFile << "BUCKETS: " << buckets << endl;

  for (int i = 0; i < buckets; i++) {
    myLogFile << "INDEX: " << i << endl;
    for (auto x : table[i]){
      myLogFile << " --> " << endl; 
      x->displayVectorElementArray();
      }

    }  
    myLogFile << endl;
}


Hash::~Hash(){

    for (unsigned k = 0; k < k_arg; k++){
        delete[] this->array_of_v[k];
    }

    delete[] this->array_of_v;
    delete[] this->array_of_t;

    delete[] table;

}