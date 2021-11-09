
#ifndef VECTORELEMENT_H
#define VECTORELEMENT_H

#include <string>
#include <iostream>
#include <sstream>
#include <fstream>

class VectorElement
{
public:
    int id; //name/id of the element
    int size;
    double *arrayVectorElement;
    double distanceCurrQ;
    unsigned int *QueryTrickid;
    double silhouette_score;

public:
    VectorElement(int, std::string, int);
    ~VectorElement();
    void setDistance(int x)
    {
        distanceCurrQ = x;
    }
    void setDistanceRandom();
    void displayId();
    void displayVectorElementArray();
    void displayDistanceCurrQ();
    void getL2Distance(VectorElement *);
};

#endif /* VECTORELEMENT_H */