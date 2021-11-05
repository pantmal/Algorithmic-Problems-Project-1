#ifndef HELPERS_H
#define HELPERS_H
#include <list>
#include "idDistancePair.h"

extern std::ofstream myLogFile;

void coutLineWithMessage(std::string m = "");
void coutLineWithMessage(int, std::string m = "");
void logFileLineWithMessage(std::string m = "");
bool cmpListPair(idDistancePair &, idDistancePair &);

int testL2();
// class neighboursInfo
// {
// public:
//     std::list<int> id;
//     std::list<int> distance;
//     void displayID();
//     void displayDistance();
// };

#endif /* HELPERS_H */