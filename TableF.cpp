#include <list>
#include <ostream>
#include <iostream>


TableF::TableF(){

  this->buckets = 2;
  table = new list<int >[buckets];
    
}

int TableF::checkItem(int item){

    int pos = -1;
    for (int i = 0; i < buckets; i++){
        list<int>::iterator hitr1;
        
        for (hitr1= table[index].begin();hitr1 != table[index].end(); hitr1++) {
            if (*hitr1 == item){
                pos = i;
                break;
            }
        }
    }

    return pos;
}

void TableF::insertItem(int item, int pos){

    table[pos].push_back(item);

}

void TableF::displayHash(){
    for (int i = 0; i < buckets; i++){
    cout << "INDEX: " << i << endl;
    for (auto x : table[i])
    {
      cout << " --> " << x << endl;
    }
  }

}

~TableF::TableF(){

    delete[] table;
}