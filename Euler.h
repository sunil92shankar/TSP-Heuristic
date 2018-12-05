/*
 * Euler.h
 *
 *  Created on: Nov 22, 2018
 *      Author: SUNIL
 */

#ifndef EULER_H_
#define EULER_H_

/*
 * Function to detect and store unique cycles in the multigraph
 * @param start, the node from which to traverse (initial)
 * @param pos, the from which to traverse in every iteration
 * @param tempGraph, the 2D array for an adjacency matrix where the weight is the number of nodes incident to the edge
 * @param, SIZE of type in for the number of nodes in the multigraph
 * @param count of type int*, tracking number of elements in every unique cycle
 * @param, mat_cycle, a 2D array of size 1000x1000 holding details of unique cycles
 * @param, cyclecount of type int tracking number of ycles
 */
int euler_circuit_alter(int start,int pos, int** tempGraph,int SIZE, int*count, int** mat_cycle,int cyclecount) {
	if(start<0||pos<0||tempGraph==NULL||SIZE<0||count==NULL||mat_cycle==NULL||cyclecount<0){
		printf("operation failed\n");
		return -1;
	}
	mat_cycle[cyclecount][*count]=pos;//update cycle element
	if(*count!=0 && pos==start)//check if the element is not the first element and the pos==start
		return -1;
	(*count)++;
/*
	printf("\n");
	for(int i=0;i<SIZE;i++){
			printf("\n");
			for(int j=0;j<SIZE;j++){
				printf("%d ",tempGraph[i][j]);
			}
		}*/

	for(int i=0;i<SIZE;i++)//traverse adjacency matrix
	{
		if(tempGraph[pos][i]>0)
		{
            tempGraph[pos][i]--;//update matrix
            tempGraph[i][pos]--;
		           // printf("entering %d\n",i);
            euler_circuit_alter(start,i, tempGraph, SIZE, count, mat_cycle, cyclecount);//recursive call
            break;
		}
	}
	return 0;
}

/*
 * Function to merge cycle and return (in pointer) an eulerian circuit in reverse
 * @param cycle_Mat, the 2D matrixof the cycles
 * @param cycle_count, the number of cycles
 * @param circuit of type int, an array of the circuit to be passed back
 * @param int* visited to keep track of cycles that are included in the eulerian tour
 * @param checkElement of type int, the element to compare cycles against
 * @param circuitpos, indicating the current position being filled in the circuit array
 */
void circuitFn(int** cycle_Mat, int cycle_count, int* circuit, int* visited, int checkElement, int *circuitpos){
	if(cycle_Mat==NULL||cycle_count<0||circuit==NULL||visited==NULL||circuitpos==NULL){
		printf("Invalid Parameters\n");
	}
	for(int i=0;i<cycle_count;i++)//traverse through cycles
	{
		if(visited[i]==0 && cycle_Mat[i][0]==checkElement)//not visited cycle
		{
			visited[i]=1;//mark as visited
			int j=0;
			while(cycle_Mat[i][j]>-1){//traverse through all nodes in cycle

				circuit[*circuitpos]=cycle_Mat[i][j];//update circuit
				(*circuitpos)++;
				for(int k=0;k<cycle_count;k++){//traverse through unvisited yet connected cycles
					if(visited[k]!=1 && cycle_Mat[k][0]==cycle_Mat[i][j]){
						(*circuitpos)--;
						circuitFn(cycle_Mat, cycle_count, circuit, visited,cycle_Mat[k][0],circuitpos);
					}
				}
				j++;
			}
		}
	}
}

#endif /* EULER_H_ */
