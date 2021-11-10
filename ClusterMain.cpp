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
//#include "HyperCube.h"
// #include "Helpers.h"
#include "Cluster.h"
#include "KMeans.h"

//#define FILE_NAME_INPUT "DataTest.txt"
//#define FILE_NAME_QUERY "QueryTest.txt"
#define FILE_NAME_INPUT "input_small_id"


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

    
    int k = 4;
    int kdim = 3;
    int w = 4;
    int N = 0;
    int M = 5000;
    int probes = 5;

    int NUMBER_OF_HASH_TABLES = 5;
    int NUMBER_OF_BUCKETS = 500;

    string assigner = "LSH";

    KMeans kmeans_obj(assigner,k);
    kmeans_obj.initialization(Input_Array,how_many_rows);

    int* r_array; 
    if (kmeans_obj.assigner == "HyperCube"){
        kmeans_obj.KMeans_Hyper = new HyperCube(kdim,how_many_columns,w, N, M, probes, 0.0);
        kmeans_obj.KMeans_Hyper->cluster_mode = true;

        for (int j = 0; j < how_many_rows; j++)
        {
            kmeans_obj.KMeans_Hyper->insertItem(Input_Array[j]);
        }
    }else if (kmeans_obj.assigner == "LSH"){
        kmeans_obj.KMeans_Hash_Array = new LSHash *[NUMBER_OF_HASH_TABLES];
        kmeans_obj.hashes = NUMBER_OF_HASH_TABLES;

        for (int i = 0; i < NUMBER_OF_HASH_TABLES; i++)
        {
            kmeans_obj.KMeans_Hash_Array[i] = new LSHash(NUMBER_OF_BUCKETS, how_many_columns, kdim,w);
            kmeans_obj.KMeans_Hash_Array[i]->cluster_mode = true;
        }

        unsigned seed = chrono::steady_clock::now().time_since_epoch().count();
        default_random_engine e(seed);
        uniform_int_distribution<> Ur(0, INT_MAX);
        r_array = new int[kdim]; 

        
        for (int i = 0; i < kdim; i++)
        {
            int r_val = Ur(e);
            r_array[i] = r_val;
        }
        
        kmeans_obj.r_array = r_array;
        for (int i = 0; i < NUMBER_OF_HASH_TABLES; i++)
        {
            for (int j = 0; j < how_many_rows; j++)
            {
                kmeans_obj.KMeans_Hash_Array[i]->insertItem(Input_Array[j], r_array);
            }
        }
    }


    for (int i = 0; i < 4; i++) //4 == ITERANTIONS 
    {
        if (kmeans_obj.assigner == "Classic"){
            kmeans_obj.ClassicAssignment(Input_Array, how_many_rows);
        }else if (kmeans_obj.assigner == "HyperCube") {
            kmeans_obj.ReverseAssignment(Input_Array, how_many_rows);
        }else{
            kmeans_obj.ReverseAssignment(Input_Array, how_many_rows);
        }
        
        kmeans_obj.update(how_many_columns);
        
        for (int j = 0; j < how_many_rows; j++)
        {
            Input_Array[j]->assigned = false;
            Input_Array[j]->assigned_clusters.clear();
        }


        if (i!=3){
            cout << "before last" <<endl;
            for (int k1 = 0; k1 < k; k1++){
                kmeans_obj.ClusterArray[k1]->cluster_elements.clear();
            }
        }
        
    }
    
    double silhouette_total = kmeans_obj.silhouette(how_many_rows);

    //CHECK FOR ONE TABLE
    //Hash h(2500, how_many_columns);

    //int k = 3;
    
    //int d = pow(2,k);

    //Cube_Obj.displayCube();


    


    //---DELETE MEMORY---


    
    // for (int i = 0; i < how_many_rows; i++)
    // {
    //     Input_Array[i]->displayId();
    // }
    //cout << "del" << endl;
    for (int i = 0; i < how_many_rows; i++)
    {
        delete Input_Array[i];
    }
    delete[] Input_Array;

    if (kmeans_obj.assigner == "HyperCube"){
        delete kmeans_obj.KMeans_Hyper;
    }else if (kmeans_obj.assigner == "LSH"){
        delete[] r_array;
        for (int i = 0; i < NUMBER_OF_HASH_TABLES; i++)
            {
                delete kmeans_obj.KMeans_Hash_Array[i];
            }
            delete[] kmeans_obj.KMeans_Hash_Array;
    }

    //myLogFile.close();

    // delete Vector_obj;
    // delete Vector_obj2;
    // delete Vector_obj3;


    return 0;
}