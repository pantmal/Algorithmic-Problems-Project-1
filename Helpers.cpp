#include <iostream>
#include <fstream>
#include "Helpers.h"
#include <list>
#include <vector>

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
std::ofstream myLogFile;