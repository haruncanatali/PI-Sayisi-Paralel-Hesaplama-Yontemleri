#include "mpi.h"
#include <iostream>
#include <iomanip>
#include <math.h>
using namespace std;

int main(int argc,char *argv[]){

 int myRank,size,N,scatterer,chunkSize;
 double lokalPiSayisi=0.0,piSayisi=0.0,araSonuc=0.0;
 int  *sendBuffer,*receiveBuffer;
 double *results;


 MPI_Init(&argc,&argv);

 MPI_Comm_size(MPI_COMM_WORLD,&size);
 MPI_Comm_rank(MPI_COMM_WORLD,&myRank);

 if(myRank == 0){
	cout << "Dagiticinin numarasini giriniz : ";
	cin >> scatterer;
	cout << "Terim Sayisini giriniz :";
	cin >> N;

	if(N%size !=0){
		cout << "Scatter kullanmak icin herbir islemcinin payina esit sayida veri dusmeli!!" << endl;
		MPI_Abort(MPI_COMM_WORLD,99);
	}
	else{
		chunkSize = N/size;
	}
 }

 MPI_Bcast(&scatterer,1,MPI_INT,0,MPI_COMM_WORLD);
 MPI_Bcast(&N,1,MPI_INT,0,MPI_COMM_WORLD);
 MPI_Bcast(&chunkSize,1,MPI_INT,0,MPI_COMM_WORLD);

 if(myRank == scatterer){
	sendBuffer = new int[N];
	results = new double[size];
	for(int i=1;i<=N;i++){
		sendBuffer[i] = i;
	}
 }

 receiveBuffer = new int[chunkSize];
 MPI_Scatter(sendBuffer,chunkSize,MPI_INT,receiveBuffer,chunkSize,MPI_INT,scatterer,MPI_COMM_WORLD); // Blok dağıtım

 for(int i=0;i<chunkSize;i++){
	araSonuc = pow(((double)receiveBuffer[i]-0.5)/N,2);
	lokalPiSayisi += (1.0/(1.0+araSonuc));
 }

 MPI_Gather(&lokalPiSayisi,1,MPI_DOUBLE,results,1,MPI_DOUBLE,scatterer,MPI_COMM_WORLD);

 if(myRank == scatterer){
	for(int i=0;i<size;i++){
		piSayisi += results[i];
	}

	piSayisi*=(4.0/N);

	cout << "Hesaplanan PI Sayisi : " << setprecision(15) << piSayisi << endl;
 }

 MPI_Finalize();

 if(myRank == scatterer){
	delete [] sendBuffer;
	delete [] results;
 }

 delete [] receiveBuffer;

 return 0;
}
