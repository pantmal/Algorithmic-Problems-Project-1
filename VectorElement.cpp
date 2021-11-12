#include <cmath>

#include "VectorElement.h"
#include "Helpers.h"

using namespace std;

//std::ofstream myLogFile;

void VectorElement::displayId()
{
    // myLogFile << "id is: " << id << std::endl;
}

void VectorElement::displayVectorElementArray()
{
    // myLogFile << "---ELEMENT ARRAY---" << std::endl;
    for (int i = 0; i < size; i++)
    {
        myLogFile << arrayVectorElement[i] << ", ";
    }
}

VectorElement::VectorElement(int dimensions, std::string fileLine, int numberOfHashTables)
{

    assigned = false;

    QueryTrickid = new unsigned int[numberOfHashTables]; //gets set when the vector element gets into a bucket
    
    id = 0;
    double temp;
    int counter = 0;
    distanceCurrQ = 0;
    
    size = dimensions;
    arrayVectorElement = new double[size];
    
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

void VectorElement::displayDistanceCurrQ()
{
    cout << distanceCurrQ << endl;
}

void VectorElement::setDistanceRandom()
{
    unsigned long x;
    x = rand();
    x <<= 15;
    x ^= rand();
    x %= 10000000;
    distanceCurrQ = x + 1;
}

void VectorElement::getL2Distance(VectorElement *q)
{
    double temp;
    double square;
    double distance = 0.0;
    for (int i = 0; i < size; i++)
    {
        temp = this->arrayVectorElement[i] - q->arrayVectorElement[i];
        square = pow(temp, 2.0);
        distance += square;
    }

    this->distanceCurrQ = sqrt(distance);
}

VectorElement::~VectorElement()
{

    delete[] arrayVectorElement;
    delete[] QueryTrickid;
}