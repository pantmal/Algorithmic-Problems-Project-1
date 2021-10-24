
#ifndef VECTORELEMENT_H
#define VECTORELEMENT_H

#include <string>
#include <iostream>
#include <sstream>
#include <fstream>

std::ofstream myLogFile;

class VectorElement
{
public:
    int id; //name/id of the element
    int size;
    int *arrayVectorElement;

public:
    VectorElement(int, std::string);
    ~VectorElement();
    void displayId()
    {
        // myLogFile << "id is: " << id << std::endl;
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
    int temp;
    int counter = 0;
    size = dimensions;
    arrayVectorElement = new int[size];
    std::stringstream sso(fileLine);
    sso >> id;
    while (sso >> temp)
    {
        //pass in the arrayvectorelement the appropriate values
        // std::cout << "CONSTRUCTOR VALUES---:" << id << std::endl;
        // sso >> arrayVectorElement[counter];
        arrayVectorElement[counter] = temp;
        counter++;
    }
}

VectorElement::~VectorElement(){

    delete [] arrayVectorElement;

}

#endif /* VECTORELEMENT_H */