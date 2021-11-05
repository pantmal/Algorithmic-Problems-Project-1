#include "idDistancePair.h"
#include <iostream>
using namespace std;
idDistancePair::idDistancePair(int x, int y)
{
    this->id = x;
    this->distance = y;
}
void idDistancePair::setId(int x)
{
    id = x;
}
void idDistancePair::setDistance(int x)
{
    id = x;
}
int idDistancePair::getId()
{
    return id;
}
int idDistancePair::getDistance()
{
    return distance;
}