
CC=g++

CFLAGS=-c
all: lsh cube cluster

lsh: LSHMain.o LSHash.o Neighbours.o IdDistancePair.o Helpers.o VectorElement.o
	$(CC) -o lsh LSHMain.o LSHash.o Neighbours.o IdDistancePair.o Helpers.o VectorElement.o

cube: HyperMain.o HyperCube.o TableF.o Neighbours.o IdDistancePair.o Helpers.o VectorElement.o
	$(CC) -o cube HyperMain.o HyperCube.o TableF.o Neighbours.o IdDistancePair.o Helpers.o VectorElement.o

cluster: ClusterMain.o KMeans.o Cluster.o VectorElement.o LSHash.o HyperCube.o TableF.o Neighbours.o Helpers.o IdDistancePair.o
	$(CC) -o cluster -O2 ClusterMain.o KMeans.o Cluster.o VectorElement.o LSHash.o HyperCube.o TableF.o Neighbours.o Helpers.o IdDistancePair.o

LSHMain.o: LSHMain.cpp
	$(CC) $(CFLAGS) LSHMain.cpp

LSHash.o: LSHash.cpp
	$(CC) $(CFLAGS) LSHash.cpp

Neighbours.o: Neighbours.cpp
	$(CC) $(CFLAGS) Neighbours.cpp

IdDistancePair.o: IdDistancePair.cpp
	$(CC) $(CFLAGS) IdDistancePair.cpp

Helpers.o: Helpers.cpp
	$(CC) $(CFLAGS) Helpers.cpp

VectorElement.o: VectorElement.cpp
	$(CC) $(CFLAGS) VectorElement.cpp

HyperMain.o: HyperMain.cpp
	$(CC) $(CFLAGS) HyperMain.cpp

HyperCube.o: HyperCube.cpp
	$(CC) $(CFLAGS) HyperCube.cpp

TableF.o: TableF.cpp
	$(CC) $(CFLAGS) TableF.cpp

ClusterMain.o: ClusterMain.cpp
	$(CC) $(CFLAGS) ClusterMain.cpp

KMeans.o: KMeans.cpp
	$(CC) $(CFLAGS) KMeans.cpp

Cluster.o: Cluster.cpp
	$(CC) $(CFLAGS) Cluster.cpp

clean:
	rm -rf *o lsh cube cluster