
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

public:
    VectorElement(int, std::string);
    ~VectorElement();
    void displayId();
    void displayVectorElementArray();
};


#endif /* VECTORELEMENT_H */