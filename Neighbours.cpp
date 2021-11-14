#include <iostream>
#include <fstream>
#include <list>
#include <vector>

#include "Neighbours.h"
#include "Helpers.h"

neighboursInfo::neighboursInfo(int N)
{
    this->arrayId = new int[N];
    this->arrayDistance = new double[N];
    //std::cout << "constr" <<std::endl;
}
neighboursInfo::~neighboursInfo()
{
   
    // std::cout << "we get here" << std::endl;
    // delete[] arrayId;
    // std::cout << "we get here2" << std::endl;
    // delete[] arrayDistance;
    
}

using namespace std;