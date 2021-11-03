#include <iostream>
#include <string>
#include "VectorElement.h"
#include <list>
#include <vector>
#include <algorithm>
#include <ostream>

using namespace std;

class staticClass
{
    static int number;
    int temp;
    static int *pointer;

public:
    static void setNumber(int temp)
    {
        number = temp;
    }
    static int getNumber()
    {
        return number;
    }
};

int staticClass::number = 0;

int euclidean_mod(int a, int b)
{
    int r = a % b;
    return r >= 0 ? r : r + abs(b);
}
//struct SortByX
//{
//   bool operator()(VectorElement *L, VectorElement *R) { return L->distanceCurrQ < R->distanceCurrQ; }
//};

bool cmp(VectorElement *L, VectorElement *R)
{
    return L->distanceCurrQ < R->distanceCurrQ;
}
class neighboursInfo
{
public:
    static int counterId;
    static int counterDistance;
    int *arrayId;
    int *arrayDistance;
    int displayID()
    {
        return *arrayId;
    }
    int displayDistance()
    {
        return *arrayDistance;
    }
    void passTheValues(int k)
    {
        for (int i = 0; i < k; i++)
        {

            this->arrayId[i] = counterId++;
            this->arrayDistance[i] = counterDistance++;
        }
    }
    neighboursInfo(int k)
    {
        this->arrayId = new int[k];
        this->arrayDistance = new int[k];
    }
};
int neighboursInfo::counterId = 0;
int neighboursInfo::counterDistance = 100;

int main()
{
    int n = 2; //number of queries
    int k = 2; //number of neighbours
    neighboursInfo **neighbourArray = new neighboursInfo *[n];
    for (int i = 0; i < n; i++)
    {
        neighbourArray[i] = new neighboursInfo(k);
    }
    for (int i = 0; i < n; i++)
    {
        neighbourArray[i]->passTheValues(k);
    }
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < k; j++)
        {
            cout << neighbourArray[i]->arrayId[0] << endl;
            cout << neighbourArray[i]->arrayDistance[0] << endl;
        }
    }
    return 0;
}

// int main()
// {
//     //LIST TESTING
//     list<VectorElement *> team1;
//     list<VectorElement *>::iterator hitr1;

//     string string1test = "1 1 2 3 4 5";
//     string string2test = "10 10 20 30 40 50";
//     string string3test = "100 100 200 300 400 500";
//     VectorElement *Vector_obj = new VectorElement(5, string1test);
//     VectorElement *Vector_obj2 = new VectorElement(5, string2test);
//     VectorElement *Vector_obj3 = new VectorElement(5, string3test);
//     Vector_obj->setDistance(1000);
//     Vector_obj2->setDistance(10000);
//     Vector_obj3->setDistance(100000);
//     team1.push_back(Vector_obj3);
//     team1.push_back(Vector_obj);
//     team1.push_back(Vector_obj2);

//     team1.sort(cmp);

//     for (hitr1 = team1.begin(); hitr1 != team1.end(); hitr1++)
//     {
//         VectorElement *vobj = *hitr1;
//         vobj->displayVectorElementArray();
//         cout << "thhis works" << endl;
//     }

//     // cout << team1.size() << endl;
//     // cout << euclidean_mod(-7, 3) << endl;
//     return 0;
// }

// cout << team1.size() << endl;

// staticClass sC;
// int input;
// cin>>input;
// sC.setNumber(input);
// cout<<sC.getNumber()<<endl;
//for (hitr1 = team1.begin(); hitr1 != team1.end(); hitr1++)
//{
//    VectorElement *vobj = *hitr1;
// vobj->displayVectorElementArray();
//}

//std::sort(team1.begin(), team1.end(), SortByX());

//team1.sort([](const VectorElement *f, const VectorElement *s)
//         { return f->distanceCurrQ < s->distanceCurrQ; });