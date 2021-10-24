#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <typeinfo>
#include <cstdio>
#include "VectorElement.h"

#define FILE_NAME "input_b_id"

using namespace std;

extern ofstream myLogFile;


int main(int argc, char *argv[])
{
    //set up test logfile
    //ofstream myLogFile;
    myLogFile.open("logFile.txt");

    bool justOnce = true;
    int how_many_columns = 0;
    int how_many_rows = 0;
    int temp;
    string mystring;
    string tempString;
    ifstream myfile;
    myfile.open(FILE_NAME);
    how_many_rows = count(istreambuf_iterator<char>(myfile), istreambuf_iterator<char>(), '\n');
    //how_many_rows++;
    //TODO: Find out the shit with rows
    myfile.close();
    myfile.clear();
    myfile.open(FILE_NAME);
    if (myfile.is_open())
    {
        while (myfile)
        {
            getline(myfile, mystring);
            stringstream sso(mystring);
            sso >> temp;
            while (justOnce && sso >> tempString)
            {
                how_many_columns++;
            }
            justOnce = false;
        }
    }
    myfile.close();
    myfile.clear();
    myfile.open(FILE_NAME);
    VectorElement** Input_Array = new VectorElement*[how_many_rows]; 
    int i = 0;
    if (myfile.is_open())
    {
        while (myfile)
        {
            getline(myfile, mystring);
            stringstream sso(mystring);
            sso >> temp;

            //TODO: find out why there is an extra loop
            if (i<how_many_rows){
                Input_Array[i] = new VectorElement(how_many_columns, mystring);  
                i++;
            }
                
        }
    }
    myfile.close();
    myfile.clear();

    myLogFile << "columns==== " << how_many_columns << endl;
    myLogFile << "rows==== " << how_many_rows << endl;

    for (int i=0; i<how_many_rows;i++){
        Input_Array[i]->displayId();
    } 

    for (int i=0; i<how_many_rows;i++){
        delete Input_Array[i];
    }
    delete [] Input_Array;

    myLogFile.close();

    return 0;
}
