#include <string>
#include <list>
#include <iostream>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <typeinfo>
#include <cstdio>
#include <chrono>
#include <random>
#include <climits>

//#include "Neighbours.h"
#include "VectorElement.h"
#include "HyperCube.h"
// #include "Hash.h"
// #include "Helpers.h"

#define FILE_NAME_INPUT "DataTest.txt"
#define FILE_NAME_QUERY "QueryTest.txt"

#define NUMBER_OF_HASH_TABLES 1
#define NUMBER_OF_BUCKETS 1
#define NUMBER_OF_NEIGHBOURS 5

using namespace std;

int main(int argc, char *argv[])
{
    //set up test logfile
    //ofstream myLogFile;
    bool justOnce = true;
    int how_many_columns = 0;
    int how_many_rows = 0;
    int temp;
    string mystring;
    string tempString;

    //myLogFile.open("logFile.txt");

    ifstream myfile;
    //OPEN DATASET FILE TO COUNT NUMBER OF ROWS
    myfile.open(FILE_NAME_INPUT);
    how_many_rows = count(istreambuf_iterator<char>(myfile), istreambuf_iterator<char>(), '\n');
    //how_many_rows++;
    myfile.close();
    myfile.clear();
    myfile.open(FILE_NAME_INPUT);
    if (myfile.is_open())
    {
        while (myfile)
        {
            getline(myfile, mystring);
            stringstream sso(mystring);
            sso >> temp;
            while (justOnce && sso >> tempString)
            {
                how_many_columns++; //calculate the number of columns(dimension of the vector without the id)
            }
            justOnce = false;
        }
    }
    myfile.close();
    myfile.clear();
    myfile.open(FILE_NAME_INPUT);
    VectorElement **Input_Array = new VectorElement *[how_many_rows];
    int i = 0;
    if (myfile.is_open())
    {
        while (myfile)
        {
            getline(myfile, mystring);
            stringstream sso(mystring);
            // sso >> temp;
            //fill the table of vectors with input from file
            if (i < how_many_rows)
            {
                Input_Array[i] = new VectorElement(how_many_columns, mystring,0);
                i++;
            }
        }
    }
    myfile.close();
    myfile.clear();

    cout << "columns==== " << how_many_columns << endl;
    cout << "rows==== " << how_many_rows << endl;

    //--------DATA COLLECTED----------

    //CHECK FOR ONE TABLE
    //Hash h(2500, how_many_columns);

    int k = 3;
    int w = 4;
    int N = 20;
    int M = 30;
    int probes = 1;
    //int d = pow(2,k);

    HyperCube Cube_Obj(k,how_many_columns,w, N, M, probes);

    for (int j = 0; j < how_many_rows; j++)
    {
       Cube_Obj.insertItem(Input_Array[j]);
    }

    //Cube_Obj.displayCube();


    //COLUMNS?????? epanaipologismo????
    // //OPEN QUERY FILE AND PARSE EACH LINE
    ifstream myfilequery;
    myfilequery.open(FILE_NAME_QUERY);
    int query_rows = count(istreambuf_iterator<char>(myfilequery), istreambuf_iterator<char>(), '\n');
    //how_many_rows++;
    cout << "Query rows are: " << query_rows << endl;
    myfilequery.close();
    myfilequery.clear();
    myfilequery.open(FILE_NAME_QUERY);
    VectorElement **Query_Array = new VectorElement *[query_rows];
    int query_i = 0;
    if (myfilequery.is_open())
    {

        while (myfilequery)
        {
            getline(myfilequery, mystring);
            stringstream sso(mystring);
            // sso >> temp;
            if (query_i < query_rows)
            {
                Query_Array[query_i] = new VectorElement(how_many_columns, mystring,0);
                query_i++;
            }
        }
    }
    myfilequery.close();

    Cube_Obj.initNeighboursInfo(query_rows);

    for (int i = 0; i < query_rows; i++)
    {
        //cout << "got" << endl;
        Cube_Obj.getFirstProbe(Query_Array[i],i);
        //cout << "got2" << endl;
    }

    for (int i = 0; i < query_rows; i++)
    {
        for (int Ni = 0; Ni < N; Ni++)
        {
            cout << "id is: " << Cube_Obj.neighboursInfoTable[i]->arrayId[Ni]  << endl;
            cout << "dist is: " << Cube_Obj.neighboursInfoTable[i]->arrayDistance[Ni]  << endl;
        }
    }

    //---DELETE MEMORY---


    
    // for (int i = 0; i < how_many_rows; i++)
    // {
    //     Input_Array[i]->displayId();
    // }
    cout << "del" << endl;
    for (int i = 0; i < how_many_rows; i++)
    {
        delete Input_Array[i];
    }
    delete[] Input_Array;

    cout << "del" << endl;
    for (int i = 0; i < query_rows; i++)
    {
        delete Query_Array[i];
    }

    delete[] Query_Array;

    //myLogFile.close();

    // delete Vector_obj;
    // delete Vector_obj2;
    // delete Vector_obj3;


    return 0;
}
