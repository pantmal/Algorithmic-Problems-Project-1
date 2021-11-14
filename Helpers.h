#ifndef HELPERS_H
#define HELPERS_H
#include <list>
#include "IdDistancePair.h"
#include <string>

extern std::ofstream myLogFile;

int euclidean_mod(int a, unsigned int b);
std::string getBinaryString(int num, int bits);
int hammingDistance(std::string str1, std::string str2);
int binarySearch(double array[], double target, int size);
void coutLineWithMessage(std::string m = "");
void coutLineWithMessage(int, std::string m = "");
void logFileLineWithMessage(std::string m = "");
bool cmpListPair(idDistancePair &, idDistancePair &);
bool cmpListPairR(idDistancePair &, idDistancePair &);
int string_into_int(std::string &s);
int args_string_to_int(std::string &x);
double args_string_to_double(std::string &x);

int testL2();

#endif /* HELPERS_H */