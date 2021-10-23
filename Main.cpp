#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <algorithm>
#include "VectorElement.h"

using namespace std;

int main(int argc, char *argv[])
{
    bool justOnce = true;
    int how_many_columns = 0;
    int how_many_rows = 0;
    int temp;
    string mystring;
    string tempString;
    ifstream myfile; //("query_small_id");
    myfile.open("test.txt");
    // how_many_rows = count(istreambuf_iterator<char>(myfile), istreambuf_iterator<char>(), '\n');
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
            if (mystring != "")
            {
                how_many_rows++;
            }
            justOnce = false;
        }
    }
    myfile.close();
    myfile.clear();
    myfile.open("test.txt");
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

// int countColumns(string row)
// {
//     int numberOfColumns = 1;
//     bool previousWasSpace = false;

//     for (int i = 0; i < row.size(); i++)
//     {
//         if (row[i] == ' ' || row[i] == '\t')
//         {
//             if (!previousWasSpace)
//                 numberOfColumns++;

//             previousWasSpace = true;
//         }
//         else
//         {
//             previousWasSpace = false;
//         }
//     }

//     return numberOfColumns;
// }

// ifstream file("MyFile.txt"); //open your file
// string line;
// getline(file, line); //read the first line of your file to string
// stringstream s;
// s << line; //send the line to the stringstream object...

// int how_many_columns = 0;
// double value;

// while (s >> value)
//     how_many_columns++; //while there's something in the line, increase the number of columns

// cout << how_many_columns;
// plik.close();