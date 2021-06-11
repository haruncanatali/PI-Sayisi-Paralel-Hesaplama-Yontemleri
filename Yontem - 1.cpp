#include "mpi.h"
#include <iostream>
#include <iomanip>
#include <stdio.h>
#include <math.h>
using namespace std;

int main(int argc,char *argv[]){

 int myRank,size,N;
 double piSayisi=0.0,araSonuc=0.0,localPiSayisi=0.0;
 MPI_Status status;

 MPI_Init(&argc,&argv);

 MPI_Comm_size(MPI_COMM_WORLD,&size);
 MPI_Comm_rank(MPI_COMM_WORLD,&myRank);

 if(myRank == 0){ 
 	cout << "Terim sayisini giriniz : ";
	cin >> N;

	for(int i=1;i<size;i++){
		MPI_Send(&N,1,MPI_INT,i,25,MPI_COMM_WORLD);
	}
	for(int i=1;i<size;i++){
		MPI_Recv(&localPiSayisi,1,MPI_DOUBLE,i,67,MPI_COMM_WORLD,&status);
		piSayisi += localPiSayisi;
	}
	piSayisi*= (4.0/N);
	cout << "Olusan PI Sayisi : " << setprecision(15) << piSayisi << endl; 
 }

 else{
	MPI_Recv(&N,1,MPI_INT,0,25,MPI_COMM_WORLD,&status);
	for(int i=myRank;i<=N;i+=(size-1)){ // Çevrimsel Daðýtým Tekniði Kullanýlýyor !!! Master hesaplamaya dahil deðil !!!
		araSonuc = pow(((double)i-0.5)/N,2);
		localPiSayisi += (1.0/(1.0+araSonuc));
	}
	MPI_Send(&localPiSayisi,1,MPI_DOUBLE,0,67,MPI_COMM_WORLD);
 }

 MPI_Finalize();


 return 0;
}

