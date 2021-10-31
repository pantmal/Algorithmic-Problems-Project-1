#ifndef HELPERS_H
#define HELPERS_H
#include <list>

extern std::ofstream myLogFile;

int testL2();
class neighboursInfo
{
public:
    std::list<int> id;
    std::list<int> distance;
    void displayID();
    void displayDistance();
};

#endif /* HELPERS_H */