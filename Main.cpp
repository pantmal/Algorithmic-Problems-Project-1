#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <typeinfo>
#include "VectorElement.h"

#define FILE_NAME "input_small_id"

using namespace std;

int main(int argc, char *argv[])
{
    bool justOnce = true;
    int how_many_columns = 0;
    int how_many_rows = 0;
    int temp;
    string mystring;
    string tempString;
    ifstream myfile;
    myfile.open(FILE_NAME);
    how_many_rows = count(istreambuf_iterator<char>(myfile), istreambuf_iterator<char>(), '\n');
    how_many_rows++;
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
    if (myfile.is_open())
    {
        while (myfile)
        {
            getline(myfile, mystring);
            stringstream sso(mystring);
            sso >> temp;
            VectorElement VE(how_many_columns, mystring);
            VE.displayVectorElementArray();
            // VE.displayId();
        }
    }
    myfile.close();
    myfile.clear();

    cout << "columns==== " << how_many_columns << endl;
    cout << "rows==== " << how_many_rows << endl;
    // VectorElement VE(how_many_columns);

    return 0;
}

// Visitor** varr= new Visitor*[K]; //varr array will store all the visitors

// 	for(int i=0; i<K;i++){ //give every visitor a floor and office preference
// 		int f_pr=rand()%4+1;
// 		int o_pr=rand()%10+1;
// 		varr[i]=new Visitor(o_pr,f_pr);

// 	}
