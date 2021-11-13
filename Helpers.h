#ifndef HELPERS_H
#define HELPERS_H
#include <list>
#include "IdDistancePair.h"
#include <string>

extern std::ofstream myLogFile;

void coutLineWithMessage(std::string m = "");
void coutLineWithMessage(int, std::string m = "");
void logFileLineWithMessage(std::string m = "");
bool cmpListPair(idDistancePair &, idDistancePair &);
bool cmpListPairR(idDistancePair &, idDistancePair &);
int string_into_int(std::string &s);
int args_string_to_int(std::string &x);
double args_string_to_double(std::string &x);

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