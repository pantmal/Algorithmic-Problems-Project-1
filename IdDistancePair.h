#ifndef IDDISTANCEPAIR_H
#define IDDISTANCEPAIR_H

class idDistancePair
{
    int id;
    double distance;

public:
    idDistancePair(int, int);
    void setId(int);
    void setDistance(int);
    int getId();
    int getDistance();
};

#endif //IDDISTANCEPAIR_H