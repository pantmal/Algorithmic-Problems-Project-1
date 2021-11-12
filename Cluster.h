
#ifndef CLUSTER_H
#define CLUSTER_H

#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <list>

#include "VectorElement.h"

using namespace std;

class Cluster
{

public:
    int id;
    int size;
    
    VectorElement* centroid;
    list<VectorElement*> cluster_elements;
    double silhouette_cluster;

public:

    Cluster(VectorElement*, int);

};

#endif /* CLUSTER_H */