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

#define FILE_NAME "test.txt"

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
    how_many_rows++;
    //TODO: Find out the shit with rows
    //TODO: Minimize the loops
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


    //LIST TESTING

    list<VectorElement*> team1;  
    list<VectorElement*>::iterator hitr1;


    string string1test = "1 1 2 3 4 5";
    string string2test = "2 6 7 8 9 10";
    string string3test = "1 1 2 3 4 5";
    VectorElement * Vector_obj = new VectorElement(5,string1test);  
    VectorElement * Vector_obj2 = new VectorElement(5,string2test);
    VectorElement * Vector_obj3 = new VectorElement(5,string3test);
    team1.push_back(Vector_obj);  
    team1.push_back(Vector_obj2);  
    team1.push_back(Vector_obj);  

    for(hitr1=team1.begin();hitr1!=team1.end();hitr1++){
        
        VectorElement * vobj = *hitr1;
        vobj->displayVectorElementArray();
    }

    
    //HASH TESTING


    //int a[] = {15, 11, 27, 8, 12};
    //int n = sizeof(a)/sizeof(a[0]);
 
    // insert the keys into the hash table
    //Hash h(7);   // 7 is count of buckets in
                // hash table
    //for (int i = 0; i < n; i++)
    //h.insertItem(Vector_obj); 
    // h.insertItem(Vector_obj2);
    //h.insertItem(Vector_obj3);  
    

    //h.displayHash();
    //h.deleteItem(Vector_obj);

    // display the Hash table
    //h.displayHash();

    //delete[] a;

    //DISTRIBUTIONS

    unsigned seed = chrono::steady_clock::now().time_since_epoch().count(); 
    default_random_engine e (seed); 
    normal_distribution<double> N(0,1); 
    
    string v_str = "1 ";
    double *v = new double[5]; 

    int n = 5;
    cout<<"\nNormal distribution for "<<n<<" samples (mean=0, standard deviation=1) =>\n";
    for (int i=0; i<n; i++)
    {
      double n_val = N(e);
      cout<<i<<". "<<n_val <<"\n"; 
    
      v[i] = n_val;
      //v_str += to_string(n_val) +' ';

    }
    //v_str.pop_back();
    
    
    int w = 4;
    uniform_real_distribution<> U(0.0, --w);
    w++;
    double t = U(e);
    cout<<"t "<<t<<"\n"; 
    

    //HASH LIST
    //int L = 5;

    //Hash** Hash_Array = new Hash*[L];
    //for (int i=0; i<L;i++){
    //    Hash_Array[i] = new Hash(2)
    //} 


    Hash h(2, 5, 5);

    for (int i=0; i<how_many_rows;i++){
        h.insertItem(Input_Array[i]); 
    } 

    //h.insertItem(r_obj);

    //cout << "got" << endl;

    h.displayHash();



    //delete r_obj;

    delete Vector_obj;
    delete Vector_obj2;
    delete Vector_obj3;

    //for (unsigned h = 0; h < height; h++){
    //    delete[] dynamic_array_of_dynamic_arrays[ h ];
    //}

    //delete[] dynamic_array_of_dynamic_arrays;

    delete[] v;

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
