#include "pvm3.h"
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>
#include "./const.h"
#include <time.h>

int tids[SLAVENUM];
int nproc;
int pid;

// A utility function to print the constructed distance array 
void printSolution(int dist[]) 
{ 
	printf("Vertex \t\t Distance from Source\n"); 
	for (int i = 0; i < V; i++) 
		printf("%d \t\t %d\n", i, dist[i]); 
} 

// Function that implements Dijkstra's single source shortest path algorithm 
// for a graph represented using adjacency matrix representation 
void dijkstra(int src) 
{ 


	int dist[V]; // The output array. dist[i] will hold the shortest 
	// distance from src to i 

	int sptSet[V]; // sptSet[i] will be true if vertex i is included in shortest 
	// path tree or shortest distance from src to i is finalized 

	// Initialize all distances as INFINITE and stpSet[] as false 
	for (int i = 0; i < V; i++) 
		dist[i] = INT_MAX, sptSet[i] = 0; 

	// Distance of source vertex from itself is always 0 
	dist[src] = 0; 

	int procx = 0;
	// Find shortest path for all vertices 
	for (int count = 0; count < V - 1; count++) {
		pvm_initsend(PvmDataDefault);
		pvm_pkint(&pid, 1, 1);
		pvm_pkint(dist, V, 1);
		pvm_pkint(sptSet, V, 1);
		pvm_send(tids[procx],MSG_MSTR);
		// printf("Send: %d \n", procx);
		if((procx++) == nproc) procx = 0;


		int i = pvm_recv(-1, MSG_SLV);
		if( i < 0 ) {
			perror("Nieoczekiwany blad");
			exit(0);
		}
		pvm_upkint(dist, V, 1);
		pvm_upkint(sptSet, V, 1);

		// for(int x=0; x<V ; x++) {
		// 	printf("Master: otrzymalem %d %d\n", dist[x], sptSet[x]);
		// }

	} 

	// print the constructed distance array 
	printSolution(dist); 
} 

int main() {

	clock_t begin = clock();

	pid = pvm_mytid();

	nproc = pvm_spawn(SLAVENAME, NULL, PvmTaskDefault, "", SLAVENUM, tids);


	
	dijkstra(0); 


	clock_t end = clock();
	double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
	printf ("Execution time %f seconds.\n", time_spent );

	pvm_exit();



}