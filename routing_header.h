/*
 * routing_header.h
 *
 *  Created on: Nov 21, 2018
 *      Author: SUNIL
 */

#ifndef ROUTING_HEADER_H_
#define ROUTING_HEADER_H_

//Data Structure to hold parent/set details in the implementation of Union Find
typedef struct nodeParent parent;
struct nodeParent{
	int nodeID;//ID of node
	int parentID;//Set/Parent of ID
};

//Data structure to hold edge details
typedef struct graph_edge edge;
struct graph_edge{
	int source;//source node
	int destination;//destination ID
	float weight;//edge weight
};

/*
 * Function using insertion sort to arrange list of edges in ascending order by weight
 * @param edges of type edge*, an array of edges passed to the function as argument to be arranged
 * @param count of type int indicating the size of the edges array
 * return 1 indicating successful sorting, -1 otherwise
 */
int arrange_asc(edge* edges, int count){
	if (edges==NULL || count<0){//parameter check
		return -1;
	}
	edge check;//temporary variable of type edge to be used for comparison and storing
	int k;//counter for insertion sort
	for(int i=1;i<count;i++)//traverse through all edges but the first one
	{
		check=edges[i];//temporary variable is now an edge i
		k=i-1;
		while(k>=0 && edges[k].weight>check.weight)//looping for insertion sort
		{
			edges[k+1]=edges[k];
			k=k-1;
		}
		edges[k+1]=check;
	}
	return 1;
};

/*
 * Function to initalize parent (set)data structure for every node set
 * @param N of type int indicating the number of nodes
 * return pointer to parent type if successful, NULL otherwise
 */
parent * initParents(int N){
	if(N<0){//parameter check
		printf("operation failed\n");
		return NULL;
	}
	parent *X=(parent *) malloc(N*sizeof(parent));//memory allocation for parent array
	for(int i=0;i<N;i++){//initialization of parent variables
		X[i].nodeID=i;
		X[i].parentID=i;
	}
	return X;
}

/*
 * Function to return the parent ID or set ID of a node
 * @param nodeID, the node for which the set information is to be returned
 * @param parent of type int *, a pointer to the parent data type to be compared with the node ID
 * return
 */
int returnParent(int nodeID, int* parent){
	if(nodeID<0 || parent==NULL){//parameter check
		printf("operation failed\n");
		return -1;
	}
	int parentreturn=-1;//variable to hold new set/parent ID to return
	if(parent[nodeID]==nodeID)//if the node is a set in itself
		parentreturn=nodeID;
	else
		parentreturn=returnParent(parent[nodeID],parent);//(recursive call) return parent/set of node ID if its set/parent is other than itself

	return parentreturn;//return parent of the node sent as argument
}

/*
 * Function to create MST
 * @param edges, a pointer to edge data type, an array of edges from which the MST is to be generated
 * @param parent of type int*, an array of parents (set indicators) for the nodes in the edges
 * @param SIZE,of type int - the number of nodes
 * return pointer to array of MST edges if successful, NULL otherwise
 */
edge* createMST(edge *edges, int *parent, int SIZE){
	if(edges==NULL||parent==NULL||SIZE<0){
		printf("operation unsuccessful\n");
		return NULL;
	}
	//variable to hold edges of the minimum spanning tree
	edge* MST = (edge *)malloc((SIZE-1)*sizeof(edge));
    if(MST==NULL){//check success of malloc
        printf("malloc error\n");
        return NULL;
    }
	int counter=0;//variable to count MST edges
	for(int i=0;counter!=SIZE-1;i++){
		edge temp=edges[i];//temporary variable of type edge to hold edge information
		//Adding temp to ST must not create cycles. This needs to be checked
		int parent1 = returnParent(temp.source, parent);//parent of source of edge
		int parent2 = returnParent(temp.destination, parent);//parent of destination of edge
		if(parent1==parent2){//checking for cycle formation
            printf("Addition of edge (%d, %d) causes cycle formation. Skip to next edge\n", temp.source, temp.destination);
		}
		else{//cycle not formed, add edge to MST
			MST[counter]=temp;
			parent[parent1]=parent2;
			counter++;
		}
	}
	return  MST;
}

//data type to define the nodes in the priority Queue for Minimum Cost Perfect Matching (greedy approach)
typedef struct PQnode node;
struct PQnode{//every Priority queue node holds edge information
	int source;//edge start/source
	int destination;//edge destination
	float weight;//edge weight
	node *next;//pointer to next node in the priority queue
};

//priority Queue data structure
typedef struct PrioQ PQ;
struct PrioQ{
	int size;//size of priority queue
	node *head;//pointer to the highest priority element in the priority Queue
};

/*
 * Function to create a priority queue
 * @param size of type int indicating the size of the Priority Queue to be created
 * return pointer to priority queue if operation successful, NULL otherwise
 */
PQ* createPQ(int size){
	if(size<0){//parameter check
		printf("operation unsuccessful");
		return NULL;
	}
	PQ *Q= (PQ*) malloc(sizeof(PQ));//allocate memory for priority queue
    if(Q==NULL){//check for malloc success
        printf("malloc error\n");
        return NULL;
    }
	Q->head=NULL;
	Q->size=size;
	return Q;
}

/*
 * Function to add elements to the priority queue
 * @param Q of type PQ*, a pointer to the priority queue to which the element is to be added
 * @param source, the source node of the edge to be added to the PQ
 * @param dest, destination of edge to be added to PQ
 * @param weight, edge weight of the edge to be added to the PQ
 * return 1 if successful, -1 otherwise
 */
int push(PQ* Q, int source, int dest, float weight){
	if(Q==NULL||source<0||dest<0||weight<0){
		printf("operation failed\n");
		return -1;
	}
	node *newnode=(node*)malloc(sizeof(node));//allcoate memory for new node to be added to the PQ
    if(newnode==NULL){//malloc success check
        printf("malloc error\n");
        return -1;
    }
    //initializing new node
	newnode->source=source;
	newnode->destination=dest;
	newnode->weight=weight;
	newnode->next=NULL;

	//Operations to add element to the prioriity queue
	if(Q->head==NULL){
		Q->head=newnode;
	}
	else if (newnode->weight<Q->head->weight){
		node *temp=Q->head;
		Q->head=newnode;
		Q->head->next=temp;
	}
	else{
		node *temp=Q->head;
		while(temp->next!=NULL && temp->next->weight<=newnode->weight){
			temp=temp->next;
		}
        newnode->next = temp->next;
        temp->next = newnode;
	}
	return 1;
}

/*
 * Function to pop highest priority element from a PQ
 * @param Q of type PQ*, a pointer to the priority queue on which to perform pop operation
 * return node that is popped if successful, NULL otherwise
 */
node pop(PQ* Q){
	node val;
	val.destination=-1;
	val.source=-1;
	val.weight=-1;
	if(Q==NULL){//parameter check
		printf("operation failed\n");
		return val;
	}
	//popping operations
    node* temp = Q->head;
    val = *(Q->head);
    Q->head=Q->head->next;
    free(temp);
    return val;
}

#endif /* ROUTING_HEADER_H_ */
