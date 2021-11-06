#include <iostream>
#include <fstream>
#include "Helpers.h"
#include <list>
#include <vector>
#include <sstream>
using namespace std;

int testL2()
{
    unsigned long x;
    x = rand();
    x <<= 15;
    x ^= rand();
    x %= 10000001;
    return x;
}
void coutLineWithMessage(std::string m)
{
    std::cout << "------" << m << "------" << std::endl;
}
void coutLineWithMessage(int k, std::string m)
{
    std::cout << "------" << m + " " << k << "------" << std::endl;
}
void logFileLineWithMessage(std::string m)
{
    myLogFile << "------" << m << "------" << std::endl;
}

// void neighboursInfo::displayID()
// {
//     if (id.empty())
//         cout << "ID list is empty" << endl;
//     std::list<int>::iterator it;
//     for (it = id.begin(); it != id.end(); ++it)
//     {
//         std::cout << *it;
//     }
// }
// void neighboursInfo::displayDistance()
// {
//     if (distance.empty())
//         cout << "Distance list is empty" << endl;
//     std::list<int>::iterator it;
//     for (it = id.begin(); it != id.end(); ++it)
//     {
//         std::cout << *it;
//     }
// }
bool cmpListPair(idDistancePair &L, idDistancePair &R)
{
    return L.getDistance() < R.getDistance();
}
bool cmpListPairR(idDistancePair &L, idDistancePair &R)
{
    return L.getId() < R.getId();
}
int string_into_int(const string &s)
{
    stringstream sso(s);
    int x = 0;
    sso >> x;
    return x;
}

std::ofstream myLogFile;