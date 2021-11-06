#include <iostream>
#include <fstream>
#include <list>
#include <vector>

#include "Neighbours.h"
#include "Helpers.h"

neighboursInfo::neighboursInfo(int N)
{
    this->arrayId = new int[N]{-1};
    this->arrayDistance = new double[N]{-1.0};
}
neighboursInfo::~neighboursInfo()
{
}

using namespace std;