#include <list>
#include <ostream>
#include <iostream>
#include <chrono>
#include <random>
#include <climits>
#include <algorithm>
#include <list>
#include <vector>
#include "Hash.h"
#include "VectorElement.h"
#include "Helpers.h"
#include "Neighbours.h"

using namespace std;

int Hash::k_arg = 5;
int Hash::w_arg = 4;

int euclidean_mod(int a, unsigned int b)
{
  int r = a % b;
  return r >= 0 ? r : r + abs(b);
}

// unsigned int euclidean_mod(int a, unsigned int b){ //not the typical implementation of euclidian mod cause it may trigger overflows
//   if( a<0) a=-a;
//   unsigned int r= a % b;
//   // return r >= 0 ? r : r + abs(b);
//   return a%b;
// }

Hash::Hash(int b, int v_size)
{

  this->buckets = b;
  table = new list<VectorElement *>[buckets];

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
}

void Hash::initNeighboursInfo(int query_rows, int N)
{ //N =no of neighbours

  for (int i = 0; i < query_rows; i++)
  {
    this->neighboursInfoTable = new neighboursInfo *[query_rows];
  }

  for (int i = 0; i < query_rows; i++)
  {
    for (int j = 0; j < N; j++)
    {
      this->neighboursInfoTable[j] = new neighboursInfo(N);
    }
  }
}

int Hash::AmplifiedHashFunction(VectorElement *key, int *r_array)
{
  //k_arg=5;
  //array_of_v=>[-3,3]
  //array_of_t=>[0,3] epireazetai apo to w
  //w=4
  //r_array=>[0,intmax]
  //h_array=> megales
  //pre-mod=>0
  //final-mod=>
  int *h_array = new int[k_arg]; //k_arg=5
  for (int k = 0; k < k_arg; k++)
  {

    double *array_start = key->arrayVectorElement;
    double inner_prod = inner_product(array_start, array_start + (key->size), this->array_of_v[k], 0.0); //~=[-11,11]
    h_array[k] = floor((inner_prod + array_of_t[k]) / w_arg);                                            //~=2-3
    //cout << h_array[k]<<endl;
  }

  int pre_mod = 0;
  unsigned int M = UINT_MAX - 5;
  for (int i = 0; i < k_arg; i++)
  {

    unsigned int euc_mod = euclidean_mod(r_array[i] * h_array[i], M);
    pre_mod += euc_mod;
  }

  //cout<<pre_mod<<endl;
  unsigned int final_mod = euclidean_mod(pre_mod, M);
  //cout<<final_mod<<endl;
  unsigned int index = euclidean_mod(final_mod, buckets);

  delete[] h_array;

  return index;
}

void Hash::calculateDistanceAndFindN(VectorElement *q, int *r_array, int j, int N) //j=no of query
{
  int index = AmplifiedHashFunction(q, r_array);
  list<VectorElement *>::iterator hitr1;
  list<VectorElement *>::iterator hitr2;
  // list<VectorElement *>::iterator hitr2;
  // list<VectorElement *>::iterator hitr3;
  //sort the lists so that we can get the closest neighbours after
  for (hitr1 = table[index].begin(); hitr1 != table[index].end(); ++hitr1)
  {
    VectorElement *vobj = *hitr1;
    // vobj->setDistanceRandom();
    vobj->getL2Distance(q);
    cout << "init dist:" << vobj->distanceCurrQ << endl;
  }
  table[index].sort(cmp);

  int Ni = 0;
  //int arr[5];
  for (hitr2 = table[index].begin(); hitr2 != table[index].end(); ++hitr2)
  {

    VectorElement *vobj = *hitr2;

    neighboursInfoTable[j]->arrayDistance[Ni] = vobj->distanceCurrQ;
    neighboursInfoTable[j]->arrayId[Ni] = vobj->id;
    Ni++;
    if (Ni == N)
    {
      break;
    }
  }

  // for (int i = 0; i < 5; i++)
  // {
  //   cout << "temp array dist:" << arr[i] << endl;
  // }

  // int counter = 1;
  // for (hitr2 = table[index].begin(); hitr2 != table[index].end(); ++hitr2)
  // {
  // cout << "WHY DOES THIS DESTROY EVERTYHING" << endl;
  // VectorElement *vobj2 = *hitr2;
  // vobj2->setDistanceRandom();
  // nInfo[j].id.push_back(vobj2->id);
  // nInfo[j].id.push_back(vobj2->distanceCurrQ);
  // nInfo[j].distance.push_back(vobj2->distanceCurrQ);
  // cout << "vobj2->id: " << vobj2->distanceCurrQ << endl;
  // }
  // for (hitr1 = table[index].begin(); hitr1 != table[index].end(); hitr1++)
  // {
  //   VectorElement *vobj2 = *hitr2;
  //   vobj2->setDistanceRandom();
  //   nInfo[j].id.push_back(vobj2->id);
  //   nInfo[j].distance.push_back(vobj2->distanceCurrQ);
  // }
  // hitr2 = table[index].begin();
  // hitr3 = table[index].end();
  // hitr1 = table[index].begin();
  // hitr2 = table[index].end();
  // sort(hitr1, hitr2, cmp);
}

void Hash::insertItem(VectorElement *key, int *r_array)
{

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
void Hash::displayHash()
{

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

  for (int i = 0; i < buckets; i++)
  {
    myLogFile << "INDEX: " << i << endl;
    for (auto x : table[i])
    {
      cout << " --> " << endl;
      x->displayVectorElementArray();
      x->displayDistanceCurrQ();
    }
  }
  myLogFile << endl;
}

void Hash::displayNeighbours(int query_rows, int N)
{
  coutLineWithMessage();
  for (int i = 0; i < query_rows; i++)
  {
    for (int j = 0; j < N; j++)
    {
      cout << "id: " << neighboursInfoTable[i]->arrayId[j] << endl;
      cout << "distance: " << neighboursInfoTable[i]->arrayDistance[j] << endl;
    }
    coutLineWithMessage("query end");
  }
  coutLineWithMessage();
}

Hash::~Hash()
{

  for (unsigned k = 0; k < k_arg; k++)
  {
    delete[] this->array_of_v[k];
  }

  delete[] this->array_of_v;
  delete[] this->array_of_t;

  delete[] table;
}