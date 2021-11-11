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

#include <list>
#include <vector>

#include "Neighbours.h"
#include "VectorElement.h"
#include "LSHash.h"
#include "Helpers.h"
#include "IdDistancePair.h"

//#define FILE_NAME_INPUT "DataTest.txt"
//#define FILE_NAME_QUERY "QueryTest.txt"
#define FILE_NAME_INPUT "input_small_id"
#define FILE_NAME_QUERY "query_small_id"

#define NUMBER_OF_HASH_TABLES 5
#define NUMBER_OF_BUCKETS 500
#define NUMBER_OF_NEIGHBOURS 5
#define RANGE 400

// int NUMBER_OF_HASH_TABLES = 5;
// int NUMBER_OF_BUCKETS = 10;
// int NUMBER_OF_NEIGHBOURS = 5;
// int RANGE = 100;

using namespace std;
//this is some text
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
    coutLineWithMessage(how_many_rows, "dataset rows are:");
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
    else
    {
        cout << "error with opening input file" << endl;
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
                Input_Array[i] = new VectorElement(how_many_columns, mystring, NUMBER_OF_HASH_TABLES);
                i++;
            }
        }
    }
    else
    {
        cout << "error with REopening input file" << endl;
    }
    myfile.close();
    myfile.clear();

    myLogFile << "columns==== " << how_many_columns << endl;
    myLogFile << "rows==== " << how_many_rows << endl;

    //--------DATA COLLECTED----------

    //CHECK FOR ONE TABLE
    //Hash h(2500, how_many_columns);

    int k_input = 5;
    int w_arg = 100;

    //h.set_k_arg(5); //dont forget to update k and w args

    // R ARRAY IS HERE
    unsigned seed = chrono::steady_clock::now().time_since_epoch().count();
    default_random_engine e(seed);

    uniform_int_distribution<> Ur(0, INT_MAX);
    int *r_array = new int[k_input]; // check for dynamic h.get_k_arg()

    for (int i = 0; i < k_input; i++)
    {
        int r_val = Ur(e);
        r_array[i] = r_val;
    }

    
    LSHash **Hash_Array = new LSHash *[NUMBER_OF_HASH_TABLES];
    for (int i = 0; i < NUMBER_OF_HASH_TABLES; i++)
    {
        Hash_Array[i] = new LSHash(NUMBER_OF_BUCKETS, how_many_columns,k_input,w_arg);
    }

    for (int i = 0; i < NUMBER_OF_HASH_TABLES; i++)
    {
        for (int j = 0; j < how_many_rows; j++)
        {
            Hash_Array[i]->insertItem(Input_Array[j], r_array);
        }
    }
    //these are correct
    // for (int i = 0; i < NUMBER_OF_HASH_TABLES; i++) //for each hash table
    // {

    //     myLogFile << "Hash Table: " << i << endl;
    //     Hash_Array[i]->displayHash();
    // }

    //COLUMNS?????? epanaipologismo????
    //OPEN QUERY FILE AND PARSE EACH LINE
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
                Query_Array[query_i] = new VectorElement(how_many_columns, mystring, NUMBER_OF_HASH_TABLES);
                query_i++;
            }
        }
    }
    else
    {
        cout << "error with opening query file" << endl;
    }
    myfilequery.close();



    //USED FOR TESTING
    // for (int i = 0; i < NUMBER_OF_HASH_TABLES; i++)
    // {
    //     for (int j = 0; j < query_rows; j++)
    //     {
    //         Hash_Array[i]->insertItem(Query_Array[j], r_array);
    //     }
    // }

    // cout << "got2" << endl;
    // coutLineWithMessage();
    coutLineWithMessage("READING FILES IS DONE");
    coutLineWithMessage("INITIALISING NEIGHBOURS");
    for (int i = 0; i < NUMBER_OF_HASH_TABLES; i++) //for each hash table
    {
        Hash_Array[i]->initNeighboursInfo(query_rows, NUMBER_OF_NEIGHBOURS);
    }

    list<idDistancePair> PairList;
    coutLineWithMessage("CALCULATING DISTANCES");
    coutLineWithMessage("NEAREST NEIGHBOURS SEARCH");
    for (int i = 0; i < query_rows; i++) //for each hash table
    {
        myLogFile << "Q: " << Query_Array[i]->id << endl;

        //start time
        std::chrono::steady_clock::time_point begin_bf = std::chrono::steady_clock::now();
        
        using clock = std::chrono::system_clock;
        using sec = std::chrono::duration<double>;

        const auto before_BF = clock::now();
        list<idDistancePair> PairListBF;
        for (int l = 0; l < how_many_rows; l++) //for each hash table
        {
            Input_Array[l]->getL2Distance(Query_Array[i]);
            idDistancePair *Pair = new idDistancePair(Input_Array[l]->id, Input_Array[l]->distanceCurrQ);
            PairListBF.push_back(*Pair);
            delete Pair;
        }
        PairListBF.sort(cmpListPair);
        
        list<idDistancePair>::iterator hitrbf;
        int currNeighboursbf = 0;
        for (hitrbf = PairListBF.begin(); hitrbf != PairListBF.end(); ++hitrbf)
        {
            if (currNeighboursbf == NUMBER_OF_NEIGHBOURS) break;
            // idDistancePair vobj = hitr1;
            myLogFile << "Real Neighbour id: " << hitrbf->getId() << endl;
            myLogFile << "Real Neighbour distance: " << hitrbf->getDistance() << endl;
            currNeighboursbf++;
        }
        //get time
        const sec duration_BF = clock::now() - before_BF;
        myLogFile << "Time Brute Force = " << duration_BF.count() << "[s]" << endl;

        //start time
        const auto before_NN = clock::now();
        for (int j = 0; j < NUMBER_OF_HASH_TABLES; j++) //for eacrh q of the queryset
        {
            
            Hash_Array[j]->calculateDistanceAndFindN(Query_Array[i], r_array, i, NUMBER_OF_NEIGHBOURS);
            // cout << "---Hash Table: " << j + 1 << " ---" << endl;
            for (int k = 0; k < NUMBER_OF_NEIGHBOURS; k++)
            {
                idDistancePair *Pair = new idDistancePair(Hash_Array[j]->neighboursInfoTable[i]->arrayId[k], Hash_Array[j]->neighboursInfoTable[i]->arrayDistance[k]);
                if (Pair->getDistance() == -1 || Pair->getDistance() == 0) break; //for sure?
                PairList.push_back(*Pair);
                delete Pair;
            }
        }

        // PairList.erase(unique(PairList.begin(), PairList.end()), PairList.end());
        PairList.sort(cmpListPair);
        auto last = std::unique(PairList.begin(), PairList.end());
        PairList.erase(last, PairList.end());
        
        int currNeighbours = 0;
        coutLineWithMessage("NEAREST NEIGHBOURS ARE: ");
        list<idDistancePair>::iterator hitr1;
        for (hitr1 = PairList.begin(); hitr1 != PairList.end(); ++hitr1)
        {
            if (currNeighbours == NUMBER_OF_NEIGHBOURS) break;
            // idDistancePair vobj = hitr1;
            myLogFile << "Neighbour id: " << hitr1->getId() << endl;
            myLogFile << "Neighbour distance: " << hitr1->getDistance() << endl;
            currNeighbours++;
        }
        //get time
        const sec duration_NN = clock::now() - before_NN;
        myLogFile << "Time LSH = " << duration_NN.count() << "[s]" << endl;
        
        //reset the list for new q
        PairList.clear();      
    }

    for (int i = 0; i < query_rows; i++) //for each hash table
    {
        myLogFile << "RANGE for q: " << Query_Array[i]->id << endl;
        //myLogFile << "Q: " << i << endl;
        
        for (int j = 0; j < NUMBER_OF_HASH_TABLES; j++) //for eacrh q of the queryset
        {
            list<VectorElement * > lsh_range_list;
            Hash_Array[j]->range_list = lsh_range_list;
            //Hash_Array[j]->range = RANGE;
            
            Hash_Array[j]->RangeSearch(Query_Array[i], r_array, i, RANGE);            
            
            lsh_range_list = Hash_Array[j]->range_list;
            list<VectorElement *>::iterator hitr1;
            for (hitr1 = lsh_range_list.begin(); hitr1 != lsh_range_list.end(); ++hitr1)
            {
                VectorElement *vobj = *hitr1;
                myLogFile <<"id" << vobj->id << endl;
                myLogFile <<"dist" << vobj->distanceCurrQ << endl;
            }
        }

        

    }


   
    //----CHECK DATA----

    //---CHECK NEIGHBOURS DATA----
    // coutLineWithMessage("NEIGHBOURRS");
    // Hash_Array[0]->displayNeighbours(NUMBER_OF_NEIGHBOURS);
    // coutLineWithMessage("NEIGHBOURS");

    //---DELETE MEMORY---

    

    delete[] r_array;

    for (int i = 0; i < how_many_rows; i++)
    {
        Input_Array[i]->displayId();
    }

    for (int i = 0; i < how_many_rows; i++)
    {
        delete Input_Array[i];
    }
    delete[] Input_Array;

    for (int i = 0; i < NUMBER_OF_HASH_TABLES; i++)
    {
        delete Hash_Array[i];
    }
    delete[] Hash_Array;

    for (int i = 0; i < query_rows; i++)
    {
        delete Query_Array[i];
    }

    delete[] Query_Array;

    myLogFile.close();

    return 0;
}
