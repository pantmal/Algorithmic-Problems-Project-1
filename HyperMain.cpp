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
#include "IdDistancePair.h"
#include "Helpers.h"

//#define FILE_NAME_INPUT "DataTest.txt"
//#define FILE_NAME_QUERY "QueryTest.txt"
#define FILE_NAME_INPUT "input_small_id"
#define FILE_NAME_QUERY "query_small_id"


#define NUMBER_OF_NEIGHBOURS 5
#define RANGE 350

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

    myLogFile.open("logFile.txt");

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
    int w = 100;
    int N = 5;
    int M = 900;
    int probes = 7;
    //int d = pow(2,k);

    HyperCube Cube_Obj(k,how_many_columns,w, N, M, probes, RANGE);

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
        myLogFile << "Query: " << Query_Array[i]->id << endl;

        list<idDistancePair> PairListBF;
        std::chrono::steady_clock::time_point begin_bf = std::chrono::steady_clock::now();
        
        using clock = std::chrono::system_clock;
        using sec = std::chrono::duration<double>;

        const auto before_BF = clock::now();
        for (int l = 0; l < how_many_rows; l++) //for each hash table
        {
            Input_Array[l]->getL2Distance(Query_Array[i]);
            idDistancePair *Pair = new idDistancePair(Input_Array[l]->id, Input_Array[l]->distanceCurrQ);
            PairListBF.push_back(*Pair);
            delete Pair;
        }
        PairListBF.sort(cmpListPair);
        const sec duration_BF = clock::now() - before_BF;
        // list<idDistancePair>::iterator hitrbf;
        // int currNeighboursbf = 0;
        // for (hitrbf = PairListBF.begin(); hitrbf != PairListBF.end(); ++hitrbf)
        // {
        //     if (currNeighboursbf == N) break;
        //     // idDistancePair vobj = hitr1;
        //     myLogFile << "Real Neighbour id: " << hitrbf->getId() << endl;
        //     myLogFile << "Real Neighbour distance: " << hitrbf->getDistance() << endl;
        //     currNeighboursbf++;
        // }
        
        

        //cout << "got" << endl;
        const auto before_NN = clock::now();
        Cube_Obj.getFirstProbe(Query_Array[i],i,"NN");
        //cout << "got2" << endl;
        //
        list<idDistancePair> PairList;
        
        int actual_N = Cube_Obj.Ni;
        // if (Cube_Obj.probes_counter == probes){
        //     actual_N = Cube_Obj.Ni;
        // }else if (Cube_Obj.Mi == M){
        //     actual_N = M;
        // }

        for (int Ni = 0; Ni < actual_N; Ni++)
        {
            //cout << "id is: " << Cube_Obj.neighboursInfoTable[i]->arrayId[Ni]  << endl;
            //cout << "dist is: " << Cube_Obj.neighboursInfoTable[i]->arrayDistance[Ni]  << endl;
            idDistancePair *Pair = new idDistancePair(Cube_Obj.neighboursInfoTable[i]->arrayId[Ni], Cube_Obj.neighboursInfoTable[i]->arrayDistance[Ni]);
            if (Pair->getDistance() == -1 || Pair->getDistance() == 0) break; //for sure?
            PairList.push_back(*Pair);
            delete Pair;
        }
        PairList.sort(cmpListPair);
        const sec duration_NN = clock::now() - before_NN;
        
        int currNeighbours = 0;
        coutLineWithMessage("NEAREST NEIGHBOURS ARE: ");
        
        list<idDistancePair>::iterator hitr1 = PairList.begin();
        list<idDistancePair>::iterator hitrbf2 = PairListBF.begin();
        for (; hitr1 != PairList.end() && hitrbf2 != PairListBF.end(); ++hitr1,++hitrbf2)
        {
            if (currNeighbours == N) break;
            // idDistancePair vobj = hitr1;
            myLogFile << "Nearest neighbor-"<<(currNeighbours+1) <<": " << hitr1->getId() << endl;
            myLogFile << "distanceHypercube: " << hitr1->getDistance() << endl;
            myLogFile << "distanceTrue: " << hitrbf2->getDistance() << endl;

            currNeighbours++;
        }
        
        myLogFile << "tHypercube = " << duration_NN.count() << "[s]" << endl;
        myLogFile << "tTrue = " << duration_BF.count() << "[s]" << endl;

        myLogFile << "R-near neighbors: " << endl;

        list<VectorElement * > cube_range_list;
        Cube_Obj.range_list = cube_range_list;
        Cube_Obj.range = RANGE;

        Cube_Obj.getFirstProbe(Query_Array[i],i,"range");    

        cube_range_list = Cube_Obj.range_list;
        list<VectorElement *>::iterator hitrR;
        for (hitrR = cube_range_list.begin(); hitrR != cube_range_list.end(); ++hitrR)
        {
            VectorElement *vobj = *hitrR;
            myLogFile << vobj->id << endl;
            //myLogFile <<"dist" << vobj->distanceCurrQ << endl;
        }
    }

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

    //cout << "del" << endl;
    for (int i = 0; i < query_rows; i++)
    {
        delete Query_Array[i];
    }

    delete[] Query_Array;

    myLogFile.close();


    return 0;
}
