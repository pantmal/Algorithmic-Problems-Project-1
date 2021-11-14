#include <iostream>
#include <fstream>
#include <list>
#include <vector>
#include <string>

#include "Neighbours.h"
#include "Helpers.h"

//Constructor and destructor for neighboursInfo class

neighboursInfo::neighboursInfo(int N)
{
    this->arrayId = new std::string[N];
    this->arrayDistance = new double[N];
    //std::cout << "constr" <<std::endl;
}

neighboursInfo::~neighboursInfo()
{
   
    //  delete[] arrayId;
    //  delete[] arrayDistance;
    
}

using namespace std;