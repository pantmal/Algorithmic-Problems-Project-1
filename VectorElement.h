
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
    void displayId()
    {
        std::cout << "id is: " << id << std::endl;
    }
    void displayVectorElementArray()
    {
        std::cout << "---ELEMENT ARRAY---" << std::endl;
        for (int i = 0; i < size; i++)
        {
            std::cout << "VE element no: " << arrayVectorElement[i] << std::endl;
        }
    }
};

VectorElement::VectorElement(int dimensions, std::string fileLine)
{
    int counter = 0;
    arrayVectorElement = new int[size];
    size = dimensions;
    std::stringstream sso(fileLine);
    sso >> id;
    while (sso >> id)
    {
        //pass in the arrayvectorelement the appropriate values
        // std::cout << "CONSTRUCTOR VALUES---:" << id << std::endl;
        // sso >> arrayVectorElement[counter];
        arrayVectorElement[counter] = id;
        counter++;
    }
}

#endif /* VECTORELEMENT_H */