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
}
neighboursInfo::~neighboursInfo()
{
}

using namespace std;