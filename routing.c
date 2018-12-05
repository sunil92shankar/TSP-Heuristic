/*
 * routing.c
 *
 *  Created on: Nov 13, 2018
 *      Author: matrix
 */

#include<stdio.h>
#include<stdlib.h>
#include"routing_header.h"
#include"Euler.h"
#include"interface.h"
//#define SIZE 8//V is size of graph

/*
 * function to run the Christofides algorithm
 */
int* calculate(double** AdjMat_Wt,int *sizetour,double*MSTlen,int SIZE)
{
	if(AdjMat_Wt==NULL||sizetour==NULL||MSTlen==NULL||SIZE<0){//parameter check
		printf("Invalid parameters\n");;
		return NULL;
	}

	//variable declared and memory allocated. Variable for 2D matrix for unique cycles in the multigraph. each row of the matrix representing  unique cycle
	int **cycle_matrix =(int **)malloc(1000*sizeof(int *));
    	if(cycle_matrix==NULL){//malloc success check
        printf("malloc error\n");
        return NULL;
    }
	for (int i=0; i<1000; i++){
		cycle_matrix[i] = (int *)malloc(1000 * sizeof(int));
        if(cycle_matrix[i]==NULL){//malloc success check
            printf("malloc error\n");
            return NULL;
        }
	}
	for(int i=0;i<1000;i++)
		for(int j=0;j<1000;j++){
			cycle_matrix[i][j]=-1;//initializing cycle_matrix
		}

	//variable to store flag for whether a cycle has been visited in the Eulerian tour or not
	int *visited =(int *)malloc(1000*sizeof(int));//malloc
    	if(visited==NULL){//malloc success check
        printf("malloc error\n");
        return NULL;
    }
	for (int i=0; i<1000; i++){
			visited[i] = 0;//initialization of declared variable/array
    }
	//printing adjacency matrix
    printf("\n printing adj matrix \n");
    for(int i=0;i<SIZE;i++)
    {
        for(int j=0;j<SIZE;j++)
        {
            printf(" %lf ", AdjMat_Wt[i][j]);
        }
        printf("\n");
    }
    
    //array of type edge declared using malloc to store edges descibed in adjacency matrix
	edge* edges = (edge*)malloc((((SIZE)*(SIZE-1))/2)*sizeof(edge));
	if(edges==NULL){//malloc success check
		printf("malloc error\n");
		return NULL;
	}

	int count=0;//variable to track number of edges in graph
	for(int i=0;i<SIZE;i++)//traverse through nodes
		for(int j=0;j<SIZE;j++){
			if(j>i){
				edges[count].source=i;
				edges[count].destination=j;
				edges[count].weight=AdjMat_Wt[i][j];//initialize edges array
				count++;//update  edge count
			}
		}

	//sort all edges in increasing order of edge costs
	int x = arrange_asc(edges,count);//function to implement sorting of edges
	if(x!=1){
		printf("unsuccessful sorting\n");
	}

	//print edges in sorted order
    printf("\n printing edges of adj mat in ascending order \n");
    for(int i=0;i< SIZE*(SIZE-1)/2;i++)
    {
        printf("Edge %d - Source %d, Destination %d, Weight %lf \n", i+1, edges[i].source,edges[i].destination, edges[i].weight);
    }

	//setting node parents for union find/cycle detection
	int *parent = (int *)malloc(SIZE*sizeof(int));//memory allocation
    if(parent==NULL){//malloc success chek
        printf("malloc error\n");
        return NULL;
    }
	for(int i=0;i<SIZE;i++){
		parent[i]=i;//set each node as its own parent/set
	}
    
	//code to generate MST
    printf("\n creating MST \n");
	edge * MST = createMST(edges, parent, SIZE);//call function to generate MST
	if(MST==NULL){
		printf("unsuccessful MST creation\n");
		return NULL;
	}
    printf("\n printing edges of MST \n");//print MST edges
    for(int i=0;i< (SIZE-1);i++)
    {
        printf("Edge %d - Source %d, Destination %d, Weight %lf \n", i+1, MST[i].source,MST[i].destination, MST[i].weight);
    }
    
	int *degree_MSTvertices = (int*)malloc(SIZE*sizeof(int));//array to store degree of MST vertices
    if(degree_MSTvertices==NULL){//malloc success check
        printf("malloc error\n");
        return NULL;
    }
	int numOddvertices=0;//variable to store number of odd vertices in MST

	//store MST length
	for(int i=0;i<SIZE-1;i++){
		(*MSTlen)+=MST[i].weight;
	}

	//obtain degree of vertices
	//printf("\nDegrees of vertices in MST\n");
	for(int i=0;i<SIZE;i++){//traverse through nodes
		degree_MSTvertices[i]=0;//initialize array that stores vertex degrees
		for(int j=0;j<SIZE-1;j++){//traverse through nodes
			if(MST[j].destination==i){//check if edge is incident to vertex i
				degree_MSTvertices[i]++;//update vertex degree
			}
			else if(MST[j].source==i){//check if edge is incident to vertex j
				degree_MSTvertices[i]++;
			}//update vertex degree
		}
		if(degree_MSTvertices[i]%2==1){//oddvertex check
			numOddvertices++;//update number of odd degree vertices
		}
		//printf("Degree of vertex %d is %d\n",i,degree_MSTvertices[i]);
	}

	int* oddvertexID = (int*)malloc(numOddvertices*sizeof(int));//array to hold IDs of odd vertices
    if(oddvertexID==NULL){//malloc success check
        printf("malloc error\n");
        return NULL;
    }
	//store odd vertices
	int oddcount=0;//count number of of odd degree vertices
    printf("\n odd degree vertices in MST \n");
	for(int i=0;i<SIZE;i++){//traverse through degrees of vertices
		if(degree_MSTvertices[i]%2==1){//check for odd vertex
				oddvertexID[oddcount]=i;//stored node ID od odd vertices
				printf("vertex %d is odd degree vertex\n",oddvertexID[oddcount]);
				oddcount++;//update odd vertex count
		}
	}

	//array to hold edge detais for edges incident on both sides to odd degree vertices
	edge* oddedges = (edge*)malloc((((numOddvertices*(numOddvertices-1))/2))*sizeof(edge));
    if(oddedges==NULL){//malloc success check
        printf("malloc error\n");
        return NULL;
    }
	int oddedgecount=0;//variable to count nnumber of edges connecting odd vertices
	for(int i=0;i<numOddvertices-1;i++)//traverse through odd edges/edges connecting odd degree veertices
		for(int j=0;j<numOddvertices;j++){
			if(j>i){
				oddedges[oddedgecount].source=oddvertexID[i];//initialize edges connecting odd vertices
				oddedges[oddedgecount].destination=oddvertexID[j];
				oddedges[oddedgecount].weight=AdjMat_Wt[oddvertexID[i]][oddvertexID[j]];
				oddedgecount++;//update count of edges connecting odd degree vertices
			}
		}

    /*
	printf("\nEdge weights Check for edges connected to one or two odd vertices\n");
	for(int j=0;j<oddedgecount;j++){
		printf("%lf\n",oddedges[j].weight);
	}*/

	//sort all edges (odd edges) in increasing order of edge costs
	x = arrange_asc(oddedges,oddedgecount);
	if(x!=1){
		printf("unsuccessful sorting\n");
	}

	//create prority queue for low cost perfect matching
	PQ* Q=createPQ(oddedgecount);
	for(int i=0;i<oddedgecount;i++)//add oddedges to the priorty queue
			push(Q, oddedges[i].source,oddedges[i].destination, oddedges[i].weight);

	int* selected = (int *)malloc(SIZE*sizeof(int));//variable (array) to keep track of edges in the perfect matching
    if(selected==NULL){//malloc success check
        printf("malloc error\n");
        return NULL;
    }
	for(int i=0;i<SIZE;i++){//traverse through nodes
		int check=0;
		for (int j=0;j<numOddvertices;j++){//traverse through odd vertices
			if(oddvertexID[j]==i){
					check=-1;
					selected[i]=0;//mark nodes that are odd degreed for use in perfect matching generation
			}
		}
		if(check==0)
			selected[i]=1;//mark nodes that need not be used in perfect matching
	}

	//edges array to be used for Minimum cost perfect matching
	edge* MCPM_edges = (edge*)malloc((numOddvertices/2)*sizeof(edge));//[MCMP Min Cost Perfect Matching]
    if(MCPM_edges==NULL){//malloc success check
        printf("malloc error\n");
        return NULL;
    }

    //code to obtain perfect matching using greedy heuristic
	int MCPMedgecount=0;//variable to store perfect matching edges count
	while(Q->head!=NULL){
		node temp = pop(Q);//pop edge details from priority queue
		if((selected[temp.source]==0) && (selected[temp.destination]==0)){//check if edge is connected to odd degree vertex
			MCPM_edges[MCPMedgecount].destination=temp.destination;//initialize MCMP edge
			MCPM_edges[MCPMedgecount].source=temp.source;
			MCPM_edges[MCPMedgecount].weight=temp.weight;
			MCPMedgecount++;//update MCMP count
			selected[temp.source]=1;//update node status such that edges incident to this node are not considered for addition into the perfect matching again
			selected[temp.destination]=1;
		}
	}

	//print MCPM edges
    printf("\n edges in Minimum Cost Perfect Matching \n");
    for(int i = 0; i< MCPMedgecount; i++)
    {
        printf("low cost perfect matching edge %d: source %d destination %d \n",i+1, MCPM_edges[i].source, MCPM_edges[i].destination );
    }

    //edges array for mmultigraph edges
	edge* Multigraph_edges = (edge*)malloc((numOddvertices/2+(SIZE-1))*sizeof(edge));
    if(Multigraph_edges==NULL){
        printf("malloc error\n");
        return NULL;
    }

    //initializing the Multigraph from MST and MCPM
	for(int i=0;i<(SIZE-1);i++){
		Multigraph_edges[i] = MST[i];
	}
	int MCPM_count=0;
	for(int i=SIZE-1;i<((numOddvertices/2)+(SIZE-1));i++){
		Multigraph_edges[i] = MCPM_edges[MCPM_count];
		MCPM_count++;
	}
    
	//print multigraph edges
	printf("\nMultigraph edges\n");
	for(int i=0;i<((numOddvertices/2)+(SIZE-1));i++){
		printf("Multigraph edge %d source %d dest %d weight %lf\n", i,Multigraph_edges[i].source,Multigraph_edges[i].destination,Multigraph_edges[i].weight);
	}
	
	//declare and initialize adjacency matrix (with values corresponding to number of edges rather than edge weight)
	int **tempGraph =(int **)malloc(SIZE*sizeof(int *));
    if(tempGraph==NULL){
        printf("malloc error\n");
        return NULL;
    }
	for (int i=0; i<SIZE; i++){
			tempGraph[i] = (int *)malloc(SIZE * sizeof(int));
        if(tempGraph[i]==NULL){
            printf("malloc error\n");
            return NULL;
        }
	}
	for(int i=0;i<SIZE;i++)
		for(int j=0;j<SIZE;j++){
			tempGraph[i][j]=0;//initialize element as zero
		}

	//Check for edges incident to nodes and update the adjacency matrix appropriately
	int edgecount=0;
	for(int i=0;i<SIZE;i++)
		for(int j=0;j<(SIZE);j++){
			if(i<j)
			for(int k=0;k<((numOddvertices/2)+(SIZE-1));k++){
				if((Multigraph_edges[k].source==i && Multigraph_edges[k].destination==j)||(Multigraph_edges[k].destination==i && Multigraph_edges[k].source==j)){
						edgecount++;
						tempGraph[i][j]++;//if edge of multigraph is incident to nodes i,j, update corresponding element in
						tempGraph[j][i]++;//adjacency matrix
				}
			}
		}

    /*
	printf("Edgecount %d\n",edgecount);
	for(int i=0;i<SIZE;i++){
			printf("\n");
			for(int j=0;j<SIZE;j++){
				printf("%d ",tempGraph[i][j]);
			}
		}
	*/

	//declaring and initalizing the max size of the eulerian circuit
	int circuitsize=edgecount+1;
	/*if(SIZE%2==0)
		{circuitsize=SIZE*(SIZE-2)/2;}
	else
	{circuitsize=SIZE*(SIZE-1)/2;}*/

	const int multgraphsize=circuitsize;

	int circuitpos=0;//variable to note current circuit element being updated
	int* circuit = (int*)malloc(sizeof(int)*multgraphsize);//array to hold eulerian circuit
    if(circuit==NULL){//malloc success check
        printf("malloc error\n");
        return NULL;
    }
	for(int i=0;i<circuitsize;i++){
		circuit[i]=-1;//initialize circuit array (eulerian circuit)
	}

	int cyclecount=0;
	for(int i=0;i<SIZE;i++){//traverse through nodes
		int eul_count=0;//variable to keep track of number of elements in a cycle detected
		int loc_check=0;//count to check number of incident edge to row vertex/element
		int exist_check=0;//variable to check f row element is also founf in previously determined cycles
		for(int j=0;j<SIZE;j++)
		{
			if(tempGraph[i][j]>0)//check number of edges  incident to row element
			{
				loc_check+=tempGraph[i][j];//count nomber of edges incident to row element
			}
			for(int k=0;k<1000;k++)//loop through 1000x1000 matrix intended to store one cycle per row
			{
				for(int l=0;l<1000;l++)
				if(i==cycle_matrix[k][l])//check if i is in cycle stored in cycle_matrix
					exist_check=1;//update flag to show element from row of adjacency matrix is incident to detected cycle
			}
		}

		//check if a row has even number of edges in every run, while also checking that an element of the row is founf in another detected cycle
		if(((loc_check%2==0)&&loc_check!=0) && ((cyclecount>0 && exist_check==1)||(cyclecount==0 && exist_check==0))){
		euler_circuit_alter(i,i, tempGraph,SIZE, &eul_count, cycle_matrix, cyclecount);//recursive function to detect and update cycles in the multigraph
		cyclecount++;//update number of unique cycles detected
		i=-1;//reset run to start from i=0
		/*
		for(int k=0;k<10;k++){
			printf("\n");
		for(int t=0;t<10;t++){
			printf("%d",cycle_matrix[k][t]);
		}}*/
		}
	}

/*	for(int i=0;i<SIZE;i++){
			printf("\n");
			for(int j=0;j<SIZE;j++){
				printf("%d ",tempGraph[i][j]);
			}
		}*/
    
	circuitFn(cycle_matrix, cyclecount, circuit, visited,cycle_matrix[0][0], &circuitpos);//recursive function to merge unique cycles and generate eulerian tour
    printf("\n\nEulerian tour in reverse\n");
	for(int i=0;i<(circuitpos);i++){
        printf("%d -- ",circuit[i]);//print eulerian tour in reverse
    }
	printf("\n");
	/*
    	//print adjacency matrix after all manipulations. All cells must be zero.
    	printf("\nadjacency matrix after all manipulations\n");
		for(int i=0;i<SIZE;i++){
			printf("\n");
			for(int j=0;j<SIZE;j++){
				printf("%d ",tempGraph[i][j]);
			}
		}*/

    *sizetour = circuitpos;//update size of eulerian tour in pointer sent by main()
    //free all allocated memory
	free(tempGraph);
	free(Multigraph_edges);
	free(selected);
	free(MCPM_edges);
	free(Q);
	free(oddedges);
	free(oddvertexID);
	free(degree_MSTvertices);
	free(MST);
	free(parent);
	free(edges);
    free(visited);
    for(int i = 0;i<1000;i++)
    {
        free(cycle_matrix[i]);
    }
	return circuit;
}


