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
#include "Hash.h"
#include "Helpers.h"
#include "idDistancePair.h"

#define FILE_NAME_INPUT "DataTest.txt"
#define FILE_NAME_QUERY "QueryTest.txt"

#define NUMBER_OF_HASH_TABLES 5
#define NUMBER_OF_BUCKETS 1
#define NUMBER_OF_NEIGHBOURS 10

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
    myfile.close();
    myfile.clear();

    myLogFile << "columns==== " << how_many_columns << endl;
    myLogFile << "rows==== " << how_many_rows << endl;

    //--------DATA COLLECTED----------

    //CHECK FOR ONE TABLE
    //Hash h(2500, how_many_columns);

    int k_input = 5;
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

    //CHECK FOR ONE TABLE
    // for (int j = 0; j < how_many_rows; j++)
    // {
    //     h.insertItem(Input_Array[j], r_array);
    // }

    // h.displayHash();
    // coutLineWithMessage();

    //HASH LIST
    int L = 1;

    Hash **Hash_Array = new Hash *[NUMBER_OF_HASH_TABLES];
    for (int i = 0; i < NUMBER_OF_HASH_TABLES; i++)
    {
        Hash_Array[i] = new Hash(NUMBER_OF_BUCKETS, how_many_columns);
    }

    for (int i = 0; i < NUMBER_OF_HASH_TABLES; i++)
    {
        for (int j = 0; j < how_many_rows; j++)
        {
            Hash_Array[i]->insertItem(Input_Array[j], r_array);
        }
    }

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

    for (int i = 0; i < NUMBER_OF_HASH_TABLES; i++) //for each hash table
    {
        Hash_Array[i]->initNeighboursInfo(query_rows, NUMBER_OF_NEIGHBOURS);
    }
    for (int i = 0; i < NUMBER_OF_HASH_TABLES; i++) //for each hash table
    {
        for (int j = 0; j < query_rows; j++) //for eacrh q of the queryset
        {
            Hash_Array[i]->calculateDistanceAndFindN(Query_Array[j], r_array, j, NUMBER_OF_NEIGHBOURS);
        }
    }
    // for (int i = 0; i < NUMBER_OF_HASH_TABLES; i++)
    // {
    //     Hash_Array[i]->displayNeighbours(query_rows, NUMBER_OF_NEIGHBOURS);
    //     cout << "HT end" << endl;
    // }
    // -----PRINT NEIGHBOURS INFO-- --coutLineWithMessage("NEIGHBOURS ARRAY");
    for (int i = 0; i < NUMBER_OF_NEIGHBOURS; i++)
    {
        cout << "Id: " << Hash_Array[0]->neighboursInfoTable[0]->arrayId[i] << endl;
        cout << "Distance: " << Hash_Array[0]->neighboursInfoTable[0]->arrayDistance[i] << endl;
    }
    // list<VectorElement *>::iterator hitr1;
    // for (hitr1 = Hash_Array[0]->table[0].begin(); hitr1 != Hash_Array[0]->table[0].end(); ++hitr1)
    // {
    //     VectorElement *vobj = *hitr1;
    // cout << "id is: " << vobj->id << endl;
    // cout << "query trick id is: " << vobj->QueryTrickid[0] << endl;
    // cout << "init dist:" << vobj->distanceCurrQ << endl;
    // }
    // DISPLAY NEAREST NEIGHBOURS FOR EVERY QUERY
    list<idDistancePair> PairList;
    cout << "nearest neighbours displayed are: " << NUMBER_OF_NEIGHBOURS << endl;
    for (int i = 0; i < query_rows; i++)
    {
        cout << "query number: " << i + 1 << endl;
        for (int j = 0; j < NUMBER_OF_HASH_TABLES; j++)
        {
            // cout << "---Hash Table: " << j + 1 << " ---" << endl;
            for (int k = 0; k < NUMBER_OF_NEIGHBOURS; k++)
            {
                idDistancePair *Pair = new idDistancePair(Hash_Array[j]->neighboursInfoTable[i]->arrayId[k], Hash_Array[j]->neighboursInfoTable[i]->arrayDistance[k]);
                PairList.push_back(*Pair);
                // cout << "neighbour: " << k + 1 << " ";
                // cout << "id:" << Hash_Array[j]->neighboursInfoTable[i]->arrayId[k] << " ";
                // cout << "distance: " << Hash_Array[j]->neighboursInfoTable[i]->arrayDistance[k] << endl;
                delete Pair;
            }
        }
        PairList.sort(cmpListPair);
        list<idDistancePair>::iterator hitr1;
        int currNeighbours = 0;
        coutLineWithMessage("PAIR LIST");
        for (hitr1 = PairList.begin(); hitr1 != PairList.end(); ++hitr1)
        {
            if (currNeighbours == NUMBER_OF_NEIGHBOURS)
                break;
            // idDistancePair vobj = hitr1;
            cout << " list id: " << hitr1->getId() << endl;
            cout << "list distance: " << hitr1->getDistance() << endl;
            currNeighbours++;
        }

        //reset the list for new q
        PairList.clear();
    }

    // for (int i = 0; i < query_rows; i++)
    // {
    //     cout << "DISPLAY DISTANCE" << endl;
    //     Query_ArrayNeighbours[i]->displayDistance();
    //     cout << "DISPLAY ID" << endl;
    //     Query_ArrayNeighbours[i]->displayID();
    //     cout << "this works" << endl;
    // }
    // for (int i = 0; i < query_rows; i++)
    // {
    //     cout << "DISPLAY DISTANCE" << endl;
    //     neighboursArray[i].displayDistance();
    //     cout << "DISPLAY ID" << endl;
    //     neighboursArray[i].displayID();
    //     cout << "this works" << endl;
    // }
    //----CHECK DATA----

    // for (int i = 0; i < NUMBER_OF_HASH_TABLES; i++) //for each hash table
    // {
    //     for (int j = 0; j < query_rows; j++) //for eacrh q of the queryset
    //     {
    //         cout << "Hash Table: " << i << endl;
    //         Hash_Array[i]->displayHash();
    //     }
    // }

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

    // delete Vector_obj;
    // delete Vector_obj2;
    // delete Vector_obj3;

    return 0;
}
