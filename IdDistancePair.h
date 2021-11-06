#ifndef IDDISTANCEPAIR_H
#define IDDISTANCEPAIR_H

class idDistancePair
{
    int id;
    double distance;

public:
    idDistancePair(int, double);
    void setId(int);
    void setDistance(int);
    int getId();
    double getDistance();
    bool operator==(idDistancePair &);
};

#endif //IDDISTANCEPAIR_H