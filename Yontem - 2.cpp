#include "mpi.h"
#include <iostream>
#include <stdio.h>
#include <iomanip>
#include <math.h>
using namespace std;

int main(int argc,char *argv[]){

 int myRank,size,N;
 double araSonuc=0.0,lokalPiSayisi=0.0,piSayisi=0.0;
 MPI_Status status;

 MPI_Init(&argc,&argv);

 MPI_Comm_size(MPI_COMM_WORLD,&size);
 MPI_Comm_rank(MPI_COMM_WORLD,&myRank);

 if(myRank == 0){
	cout << "Terim sayisini giriniz : ";
	cin >> N;
 }

 MPI_Bcast(&N,1,MPI_INT,0,MPI_COMM_WORLD);

 for(int i=(myRank+1);i<=N;i+=size){ // Çevrimsel Daðýtým Tekniði Kullanýlýyor !!! Master hesaplamalara dahil ediliyor !!!
	araSonuc = pow(((double)i-0.5)/N,2);
	lokalPiSayisi += (1.0/(1.0+araSonuc));
 }

 MPI_Reduce(&lokalPiSayisi,&piSayisi,1,MPI_DOUBLE,MPI_SUM,0,MPI_COMM_WORLD);

 if(myRank == 0){
	piSayisi*=(4.0/N);
	cout << "Hesaplanan piSayisi : " << setprecision(15) << piSayisi << endl;
 }

 MPI_Finalize();

 return 0;
}
