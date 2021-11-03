#ifndef NEIGHBOURS_H
#define NEIGHBOURS_H

class neighboursInfo
{
public:
    static int counterId;
    static int counterDistance;
    int *arrayId;
    double *arrayDistance;
    void passTheValues(int k) //redeclare
    {
        for (int i = 0; i < k; i++)
        {

            this->arrayId[i] = counterId++;
            this->arrayDistance[i] = counterDistance++;
        }
    }
    neighboursInfo(int N);
    ~neighboursInfo(); //TODO
};

#endif //NEIGHBOURS_H