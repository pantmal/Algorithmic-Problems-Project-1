#include <list>
#include <ostream>
#include <iostream>
#include <chrono>
#include <random>
#include <climits>
#include <algorithm>
#include <list>
#include <vector>
#include <set>
#include "LSHash.h"
#include "VectorElement.h"
#include "Helpers.h"
#include "Neighbours.h"

using namespace std;

//int LSHash::k_arg = 5;
//int LSHash::w_arg = 100;

int euclidean_mod(int a, unsigned int b)
{
  int r = a % b;
  return r >= 0 ? r : r + b; //abs(b) doesn't work here
}


int LSHash::hashNumber = 0;
LSHash::LSHash(int b, int v_size, int k_arg, int w_arg)
{

  this->k_arg = k_arg;
  this->w_arg = w_arg;

  hashTableNumber = hashNumber;
  hashNumber++;
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

  cluster_mode = false;
  current_cluster = 0;
  assigned_total = 0;

}

void LSHash::initNeighboursInfo(int query_rows, int N)
{ //N =no of neighbours

  query_rows_field = query_rows;
  this->neighboursInfoTable = new neighboursInfo*[query_rows];
 
  
  for (int i = 0; i < query_rows; i++)
  {

    this->neighboursInfoTable[i] = new neighboursInfo(N);    
  }
}

int LSHash::AmplifiedHashFunction(VectorElement *key, int *r_array)
{
  
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
  key->QueryTrickid[hashTableNumber] = final_mod;
  return index;
}


void LSHash::calculateDistanceAndFindN(VectorElement *q, int *r_array, int j, int N) //j=no of query
{
  int index = AmplifiedHashFunction(q, r_array);

  if (table[index].size() == 0){
    return;
  }

  list<VectorElement *>::iterator hitr1;
  list<VectorElement *>::iterator hitr2;

  //sort the lists so that we can get the closest neighbours after
  for (hitr1 = table[index].begin(); hitr1 != table[index].end(); ++hitr1)
  {
    VectorElement *vobj = *hitr1;
    vobj->distanceCurrQ = 0.0;

    // cout<< "q id" <<q->QueryTrickid[hashTableNumber] << endl;
    // cout<< "inp id" <<  vobj->QueryTrickid[hashTableNumber] << endl;
    // if (q->QueryTrickid[hashTableNumber] == vobj->QueryTrickid[hashTableNumber]){
    //  cout<< "q id" <<q->QueryTrickid[hashTableNumber] << endl;
    //  cout<< "inp id" <<  vobj->QueryTrickid[hashTableNumber] << endl;
    // }
    //if (q->QueryTrickid[hashTableNumber] == vobj->QueryTrickid[hashTableNumber]) continue;

    // vobj->setDistanceRandom();
    vobj->getL2Distance(q);
  }
  table[index].sort(cmp);

  
  int Ni = 0;
  for (hitr2 = table[index].begin(); hitr2 != table[index].end(); ++hitr2)
  {
    if (Ni == N) break;

    VectorElement *vobj = *hitr2;
    neighboursInfoTable[j]->arrayDistance[Ni] = vobj->distanceCurrQ;
    neighboursInfoTable[j]->arrayId[Ni] = vobj->id;
    Ni++;
  }

}

void LSHash::RangeSearch(VectorElement *q, int *r_array, int j, double range) //j=no of query
{

  int index = AmplifiedHashFunction(q, r_array);

  if (table[index].size() == 0){
    return;
  }

  list<VectorElement *>::iterator hitr1;
  list<VectorElement *>::iterator hitr2;

  set<int> visited;

  //sort the lists so that we can get the closest neighbours after
  for (hitr1 = table[index].begin(); hitr1 != table[index].end(); ++hitr1)
  {
    VectorElement *vobj = *hitr1;
    vobj->getL2Distance(q);
  }
  table[index].sort(cmp);

  
  for (hitr2 = table[index].begin(); hitr2 != table[index].end(); ++hitr2)
  {
    
    VectorElement *vobj = *hitr2;
    if (vobj->distanceCurrQ <= range){
      bool visit_check = visited.find(vobj->id) != visited.end();
      if (visit_check) continue;
      
      //myLogFile <<"id" << vobj->id << endl;
      //myLogFile <<"dist" << vobj->distanceCurrQ << endl;
      if (cluster_mode == true && vobj->assigned == true) continue;

      range_list.push_back(vobj);
      
      if (cluster_mode == true){
        vobj->assigned = true;
        vobj->assigned_clusters.push_back(current_cluster);
        assigned_total++;
      }
      
      visited.insert(vobj->id);
    }
    
  }



}

void LSHash::insertItem(VectorElement *key, int *r_array)
{

  int index = AmplifiedHashFunction(key, r_array);
  table[index].push_back(key);
}


// function to display hash table
void LSHash::displayHash()
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
      myLogFile << " --> " << endl;
      myLogFile << "id" << x->id << endl;
      x->displayVectorElementArray();
      //x->displayDistanceCurrQ();
    }
  }
  myLogFile << endl;
}

// void Hash::displayNeighbours(int query_rows, int N)
// {
//   coutLineWithMessage();
//   for (int i = 0; i < query_rows; i++)
//   {
//     for (int j = 0; j < N; j++)
//     {
//       cout << "id: " << neighboursInfoTable[i]->arrayId[j] << endl;
//       cout << "distance: " << neighboursInfoTable[i]->arrayDistance[j] << endl;
//     }
//     coutLineWithMessage("query end");
//   }
//   coutLineWithMessage();
// }

void LSHash::displayNeighbours(int N)
{
  myLogFile << "hash table no: " << this->hashTableNumber << endl;
  for (int i = 0; i < N; i++)
  {
    myLogFile << "id is: " << this->neighboursInfoTable[0]->arrayId[i] << "  "
              << "distance is: " << this->neighboursInfoTable[0]->arrayDistance[i] << endl;
  }
}

// void Hash::DeleteNeighborsInfo(){
//   cout << query_rows_field << endl;
//   for (int i = 0; i < query_rows_field; i++)
//   {
      
//    delete[] neighboursInfoTable[i]; //WHAT IS WRONG HERE?
    
//   }
  
//   delete[] neighboursInfoTable;
// }

LSHash::~LSHash()
{

  for (unsigned k = 0; k < k_arg; k++)
  {
    delete[] this->array_of_v[k];
  }

  delete[] this->array_of_v;
  delete[] this->array_of_t;
  delete[] table;

  //cout << query_rows_field << endl;
  //for (int i = 0; i < query_rows_field; i++)
  //{
      
  //  delete[] neighboursInfoTable[i]; WHAT IS WRONG HERE?
    
  //}
  
  //delete[] neighboursInfoTable;
    
}