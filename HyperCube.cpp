
#include <list>
#include <ostream>
#include <iostream>
#include <chrono>
#include <random>
#include <cmath>
#include <string>
#include <bitset>
#include <bits/stdc++.h>

#include "VectorElement.h"
#include "TableF.h"
#include "HyperCube.h"
#include "Neighbours.h"
#include "Helpers.h"


string getBinaryString(int num, int bits) {
    vector<char> digits(bits);
    for (int i = 0; i < bits; ++i) {
      digits.push_back(num % 2 + '0');
      num >>= 1;  
    }
    return string(digits.rbegin(), digits.rend());
}

int hammingDistance(string str1, string str2){
  int i = 0, count = 0;
  while (str1[i] != '\0')
  {
    if (str1[i] != str2[i])
      count++;
    i++;
  }
  return count;
}

HyperCube::HyperCube(int k_arg, int v_size, int w_arg, int N_arg, int M_arg, int probes_arg, double range_arg){

  N = N_arg;
  Ni = 0;

  M = M_arg;
  Mi = 0;
  
  probes = probes_arg;
  probes_counter = 0;

  range = range_arg;

  hammingCounter = 0;

  w_cube = w_arg;
  k_cube = k_arg;

  int dims = pow(2,k_arg);
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
    TableOfValuesF[i] = new TableF();
   }

  cluster_mode = false;
  current_cluster = 0;
  assigned_total = 0;
}

void HyperCube::initNeighboursInfo(int query_rows)
{ //N =no of neighbours

  
  this->neighboursInfoTable = new neighboursInfo*[query_rows];
  

  for (int i = 0; i < query_rows; i++)
  {
    
    this->neighboursInfoTable[i] = new neighboursInfo(N);
    
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
        //cout << "found index" << endl;
        return checkpos;
    }


}

int HyperCube::indexBuilder(VectorElement *key){

    string index_s = "";
    int k_arg = k_cube;
    int *h_array = new int[k_arg];
    for (int i = 0; i < k_arg; i++){
        double *array_start = key->arrayVectorElement;
        double inner_prod = inner_product(array_start, array_start + (key->size), this->array_of_v[i], 0.0); //~=[-11,11]
        h_array[i] = floor((inner_prod + array_of_t[i]) / w_cube); 
    }

    for (int i = 0; i < k_arg; i++){
        //cout << h_array[i] << endl;
        int bit = functionF(h_array[i], i);
        string bit_s = to_string(bit);    
        index_s += bit_s;
    }

    
    //cout << "int " << stoi(index_s,nullptr,2) << endl;

    int index = stoi(index_s,nullptr,2);

    return index;


}


void HyperCube::insertItem(VectorElement *key){

    int index = indexBuilder(key);
    table[index].push_back(key);

}

void HyperCube::getFirstProbe(VectorElement *key, int j, string search){

  probesCandidates.clear();
  visited.clear();
  hammingCounter = 0;
  Ni = 0;
  Mi = 0;
  probes_counter = 0;

  int index = indexBuilder(key);
  
  init_index = index;

  if (search == "NN"){
    calculateDistanceAndFindN(key,j,init_index);
  }
  else{
    RangeSearch(key,j,init_index);
  }

}


void HyperCube::getNextProbe(VectorElement *key, int j, int index, string search){

  if (probesCandidates.empty()){

    hammingCounter++;

    for (int bucket = 0; bucket < buckets; bucket++){
      if (index == bucket) continue;

      if (!visited.empty()){
        bool visit_check = visited.find(bucket) != visited.end();
        if (visit_check){
          //cout << "yup..." << endl;
          continue;
        }
          
      }
      

      string index_s = getBinaryString(index, k_cube);
      string bucket_s = getBinaryString(bucket, k_cube);

      int dist = hammingDistance(index_s, bucket_s);

      //visited.insert(bucket);

      if (dist == hammingCounter){
        probesCandidates.push_back(bucket);
      }      
    }

    for (list<int>::iterator i=probesCandidates.begin(); i!=probesCandidates.end(); i++)
    {
      int item = *i;
      visited.insert(item);
    }

  }

  unsigned seed = chrono::steady_clock::now().time_since_epoch().count();
  default_random_engine e(seed);
  uniform_int_distribution<> U(0,probesCandidates.size()-1);
  //cout << "size " << probesCandidates.size() << endl;
  int random_element = U(e);
      
  list<int>::iterator it = probesCandidates.begin();
  advance(it,random_element);
  int item = *it;
  //cout << item << endl;
  probesCandidates.erase(it);

  //cout << item << endl;

  if (search == "NN"){
    calculateDistanceAndFindN(key,j,item);
  }
  else{
    RangeSearch(key,j,item);
  }


}

void HyperCube::calculateDistanceAndFindN(VectorElement *q, int j, int index){

  //int index = indexBuilder(key);
  if (table[index].size() == 0){
    return;
  }

  list<VectorElement *>::iterator hitr1;
  list<VectorElement *>::iterator hitr2;

  for (hitr1 = table[index].begin(); hitr1 != table[index].end(); ++hitr1)
  {
    VectorElement *vobj = *hitr1;
    vobj->getL2Distance(q);
    //cout << "init dist:" << vobj->distanceCurrQ << endl;
  }
  table[index].sort(cmp);

  
  for (hitr2 = table[index].begin(); hitr2 != table[index].end(); hitr2++)
  {

    if (Ni == N){
      break;
    }

    if (Mi == M){
      break;
    }

  
    VectorElement *vobj = *hitr2;
    neighboursInfoTable[j]->arrayDistance[Ni] = vobj->distanceCurrQ;
    neighboursInfoTable[j]->arrayId[Ni] = vobj->id;


    Ni++;
    Mi++;
    
  }

  probes_counter++;

  if ( Ni < N){
    if (Mi != M){
      if (probes_counter != probes ){
        // cout << "got2 " << Ni << " " << Mi << " " << probes_counter <<endl;  
        // cout << "index " << index << endl;
        getNextProbe(q,j,init_index,"NN");
      }
    }
  }


}

void HyperCube::RangeSearch(VectorElement *q, int j, int index) //j=no of query
{

  if (table[index].size() == 0){
    return;
  }

  list<VectorElement *>::iterator hitr1;
  list<VectorElement *>::iterator hitr2;

  set<int> visited;

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

      if (Mi == M) break;

      bool visit_check = visited.find(vobj->id) != visited.end();
      if (visit_check) continue;

      if (cluster_mode == true && vobj->assigned == true) continue;

      
      //myLogFile <<"id" << vobj->id << endl;
      //myLogFile <<"dist" << vobj->distanceCurrQ << endl;
      range_list.push_back(vobj);
      
      if (cluster_mode == true){
        vobj->assigned = true;
        vobj->assigned_clusters.push_back(current_cluster);
        assigned_total++;
      }
      
      
      visited.insert(vobj->id);

      Mi++;
    }
    
  }

  probes_counter++;

  
  if (Mi != M){
    if (probes_counter != probes ){
      // cout << "got2 " << Ni << " " << Mi << " " << probes_counter <<endl;  
      // cout << "index " << index << endl;
      getNextProbe(q,j,init_index,"range");
    }
  }
  


}


void HyperCube::displayCube(){
    for (int i = 0; i < buckets; i++){
    cout << "INDEX: " << i << endl;
    for (auto x : table[i])
    {
      cout << " --> " << x << endl;
      x->displayVectorElementArray();
    }
  }

  //for (int i = 0; i < 3; i++){
  //  TableOfValuesF[i]->displayHash();
  //}

}

HyperCube::~HyperCube(){

    
  for (unsigned k = 0; k < k_cube; k++)
  {
      delete[] this->array_of_v[k];
  }

  delete[] this->array_of_v;
  delete[] this->array_of_t;

  for (int i = 0; i < k_cube; i++){
      delete TableOfValuesF[i];
  }
  delete[] TableOfValuesF;

  delete[] table;
}

//f

//indexBuilder

//insert

//des