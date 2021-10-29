#include <list>
#include <ostream>
#include <iostream>
#include <chrono> 
#include <random> 
#include <climits>

#include "Hash.h"
#include "VectorElement.h"

using namespace std;

int euclidean_mod(int a, int b){
  int r = a % b;
  return r >= 0 ? r : r + abs(b);
}
 
Hash::Hash(int b, int k_arg, int v_size)
{
    this->BUCKET = b;
    table = new list<VectorElement*>[BUCKET];

    this->array_of_v = new double*[ k_arg ];
    for (unsigned h = 0; h < k_arg; h++){
      this->array_of_v[h] = new double[v_size];
    }

    unsigned seed = chrono::steady_clock::now().time_since_epoch().count(); 
    default_random_engine e (seed); 
    normal_distribution<double> N(0,1); 

    for (unsigned h = 0; h < k_arg; h++){
        double* inner_array = this->array_of_v[h];
        for (unsigned w = 0; w < v_size; w++){
            double n_val = N(e);
            inner_array[w] = n_val;
        }
    }

    int open_k = k_arg - 1;
    uniform_real_distribution<> U(0.0, open_k);
    this->array_of_t = new double[k_arg]; 
    for (int i=0; i<k_arg; i++){
      double t = U(e);
      this->array_of_t[i] = t;
    }

    uniform_int_distribution<> Ur(0, INT_MAX);

    string r_str = "1 ";
    for (int i=0; i<5; i++){
      int r_val = Ur(e);    
      r_str += to_string(r_val) + ' ';
    }
    r_str.pop_back();
    cout << "r_str" << r_str << endl;
    this->r_obj = new VectorElement(5,r_str);

    // for (unsigned h = 0; h < 5; h++){
    //    double* inner_array = this->array_of_v[h];
    //    cout << "In array:" << h << endl;
    //    for (unsigned w = 0; w < 5; w++){
    //        cout << inner_array[w] << endl;
    //    }
    //   }
}
 
void Hash::insertItem(VectorElement* key){

  int *h_array = new int[5];
  for (int i=0; i<5; i++){
    
    double inner_prod = inner_product(key->arrayVectorElement,key->arrayVectorElement+5,this->array_of_v[i],0.0);
    h_array[i] = floor((inner_prod + array_of_t[i]) / 4);
  }

  int pre_mod = 0;
  int M = UINT_MAX - 5;
  for (int i=0; i<5; i++){

    int euc_mod = euclidean_mod(r_obj->arrayVectorElement[i]*h_array[i],M);
    pre_mod += pre_mod + euc_mod;

  }


  int final_mod =  euclidean_mod(pre_mod, M) ;
  int index = euclidean_mod(final_mod, BUCKET) ;

  cout << index << endl;

  delete [] h_array;

    //int index = hashFunction(key);
    table[index].push_back(key);
}
 
void Hash::deleteItem(VectorElement* key)
{
  // get the hash index of key
  int index = hashFunction(key);
 
  // find the key in (index)th list
  list <VectorElement*> :: iterator i;
  for (i = table[index].begin();
           i != table[index].end(); i++) {
    if (*i == key)
      break;
  }
 
  // if key is found in hash table, remove it
  if (i != table[index].end())
    table[index].erase(i);
}
 
// function to display hash table
void Hash::displayHash() {

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

  for (int i = 0; i < BUCKET; i++) {
    cout << i;
    for (auto x : table[i]){
      cout << " --> " << endl; 
      x->displayVectorElementArray();
      }

    }  
    cout << endl;
}


Hash::~Hash(){

    for (unsigned h = 0; h < 5; h++){
        delete[] this->array_of_v[ h ];
    }

    delete[] this->array_of_v;
    delete[] this->array_of_t;

    delete[] table;

    delete r_obj;

}