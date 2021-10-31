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
    cout << "---ELEMENT ARRAY---" << std::endl;
    for (int i = 0; i < size; i++)
    {
        cout << "VE element no: " << arrayVectorElement[i] << std::endl;
    }
}
VectorElement::VectorElement(int dimensions, std::string fileLine)
{
    int temp;
    int counter = 0;
    distanceCurrQ = 0;
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
    x %= 10000001;
    distanceCurrQ = x + 1;
}

VectorElement::~VectorElement()
{

    delete[] arrayVectorElement;
}