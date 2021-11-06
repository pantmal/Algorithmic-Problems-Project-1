#include "idDistancePair.h"
#include <iostream>
using namespace std;
bool idDistancePair::operator==(idDistancePair &Pair1)
{
    if (this->getId() != Pair1.getId())
        return false;
    return true;
}
idDistancePair::idDistancePair(int x, double y)
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
double idDistancePair::getDistance()
{
    return distance;
}