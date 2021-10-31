
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
    int *arrayVectorElement;
    int distanceCurrQ;

public:
    VectorElement(int, std::string);
    ~VectorElement();
    void setDistance(int x)
    {
        distanceCurrQ = x;
    }
    void setDistanceRandom();
    void displayId();
    void displayVectorElementArray();
    void displayDistanceCurrQ();
};

#endif /* VECTORELEMENT_H */