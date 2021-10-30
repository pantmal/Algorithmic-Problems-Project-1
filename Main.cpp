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

#include "VectorElement.h"
#include "Hash.h"
#include "Helpers.h"

#define FILE_NAME_INPUT "input_small_id"
#define FILE_NAME_QUERY "query_small_id"

using namespace std;

int main(int argc, char *argv[]){
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
    VectorElement** Input_Array = new VectorElement*[how_many_rows]; 
    int i = 0;
    if (myfile.is_open())
    {
        while (myfile)
        {
            getline(myfile, mystring);
            stringstream sso(mystring);
            // sso >> temp;
            //fill the table of vectors with input from file
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


    //LIST TESTING
    // list<VectorElement*> team1;  
    // list<VectorElement*>::iterator hitr1;


    // string string1test = "1 1 2 3 4 5";
    // string string2test = "2 6 7 8 9 10";
    // string string3test = "1 1 2 3 4 5";
    // VectorElement * Vector_obj = new VectorElement(5,string1test);  
    // VectorElement * Vector_obj2 = new VectorElement(5,string2test);
    // VectorElement * Vector_obj3 = new VectorElement(5,string3test);
    // team1.push_back(Vector_obj);  
    // team1.push_back(Vector_obj2);  
    // team1.push_back(Vector_obj);  

    // for(hitr1=team1.begin();hitr1!=team1.end();hitr1++){
        
    //     VectorElement * vobj = *hitr1;
    //     vobj->displayVectorElementArray();
    // }

    // cout << team1.size() << endl;
    
    //Hash h(2500, how_many_columns);
    int k_input = 5;
    //h.set_k_arg(5); //dont forget to update k and w args


    // R ARRAY IS HERE
    unsigned seed = chrono::steady_clock::now().time_since_epoch().count(); 
    default_random_engine e (seed); 
    
    uniform_int_distribution<> Ur(0, INT_MAX);
    int* r_array = new int[k_input];  // check for dynamic h.get_k_arg()
 
    for (int i=0; i<k_input; i++){
      int r_val = Ur(e);    
      r_array[i]=r_val;
    }
    
    //HASH LIST
    int L = 5;

    Hash** Hash_Array = new Hash*[L];
    for (int i=0; i<L;i++){
        Hash_Array[i] = new Hash(2500, how_many_columns);
    } 
    
    for (int i=0; i<L;i++){
        for (int j=0; j<how_many_rows;j++){
            Hash_Array[i]->insertItem(Input_Array[j], r_array); 
        } 
    }



    //COLUMNS?????? epanaipologismo????
    //OPEN QUERY FILE AND PARSE EACH LINE
    ifstream myfilequery;
    myfilequery.open(FILE_NAME_QUERY);
    int query_rows = count(istreambuf_iterator<char>(myfilequery), istreambuf_iterator<char>(), '\n');
    //how_many_rows++;
    myfilequery.close();
    myfilequery.clear();
    myfilequery.open(FILE_NAME_QUERY);
    VectorElement** Query_Array = new VectorElement*[query_rows]; 
    int query_i = 0;
    if (myfilequery.is_open()){
        
        while (myfilequery){
            getline(myfilequery, mystring);
            stringstream sso(mystring);
            // sso >> temp;
            if (query_i<query_rows){
                Query_Array[query_i] = new VectorElement(how_many_columns, mystring);  
                query_i++;
            }
        }
    }
    myfilequery.close();


    for (int i=0; i<L;i++){
        for (int j=0; j<query_rows;j++){
            Hash_Array[i]->insertItem(Query_Array[j], r_array); 
        } 
    }










    delete [] r_array;

    for (int i=0; i<how_many_rows;i++){
        Input_Array[i]->displayId();
    } 

    for (int i=0; i<how_many_rows;i++){
        delete Input_Array[i];
    }
    delete [] Input_Array;

    for (int i=0; i<L;i++){
        delete Hash_Array[i];
    }
    delete [] Hash_Array;


    for (int i=0; i<query_rows;i++){
        delete Query_Array[i];
    }

    delete [] Query_Array;

    myLogFile.close();

    // delete Vector_obj;
    // delete Vector_obj2;
    // delete Vector_obj3;

    return 0;
}
