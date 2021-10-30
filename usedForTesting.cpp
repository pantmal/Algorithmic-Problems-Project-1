#include <iostream>
#include <string>

using namespace std;

class staticClass{
    static int number;
    int temp;
    static int* pointer;
    public:
    static void setNumber(int temp){
        number=temp;
    }
    static int getNumber(){
        return number;
    }
};

int staticClass::number=0;

int euclidean_mod(int a,int b){
  int r = a % b;
  return r >= 0 ? r : r + abs(b);
}
int main(){
    // staticClass sC;
    // int input;
    // cin>>input;
    // sC.setNumber(input);
    // cout<<sC.getNumber()<<endl;
    cout<<euclidean_mod(-7,3)<<endl;
    return 0;
}