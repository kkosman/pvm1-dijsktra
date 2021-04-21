
#include "pvm3.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <stdbool.h>
#include "./const.h"

// A utility function to find the vertex with minimum distance value, from 
// the set of vertices not yet included in shortest path tree 
int minDistance(int dist[], int sptSet[]) 
{ 
	// Initialize min value 
	int min = INT_MAX, min_index; 

	for (int v = 0; v < V; v++) 
		if (sptSet[v] == 0 && dist[v] <= min) 
			min = dist[v], min_index = v; 

	return min_index; 
} 

int main() {
	int pid;
	int dist[V];
	int sptSet[V];

	int i = pvm_recv(-1, MSG_MSTR);
	if( i < 0 ) {
		perror("Nieoczekiwany blad");
		exit(0);
	}
	pvm_upkint(&pid, 1, 1);
	pvm_upkint(dist, V, 1);
	pvm_upkint(sptSet, V, 1);
	for(int x=0; x<V ; x++) {
		printf("Slave: otrzymalem %d %d\n", dist[x], sptSet[x]);
	}	


	// Pick the minimum distance vertex from the set of vertices not 
	// yet processed. u is always equal to src in the first iteration. 
	int u = minDistance(dist, sptSet); 

	// Mark the picked vertex as processed 
	sptSet[u] = 1; 

	// Update dist value of the adjacent vertices of the picked vertex. 
	for (int v = 0; v < V; v++) 

		// Update dist[v] only if is not in sptSet, there is an edge from 
		// u to v, and total weight of path from src to v through u is 
		// smaller than current value of dist[v] 
		if (sptSet[v] == 0 && GRAPH[u][v] && dist[u] != INT_MAX 
			&& dist[u] + GRAPH[u][v] < dist[v]) 
			dist[v] = dist[u] + GRAPH[u][v]; 


	for(int x=0; x<V ; x++) {
		printf("Slave: wynik %d %d\n", dist[x], sptSet[x]);
	}
	pvm_initsend(PvmDataDefault);
	pvm_pkint(dist, V, 1);
	pvm_pkint(sptSet, V, 1);
	pvm_send(pid, MSG_SLV);
	pvm_exit();
}