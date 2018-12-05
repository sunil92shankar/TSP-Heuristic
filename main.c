//
//  main.c
//  nearest
//
//  Created by Kartik Kothari on 11/7/18.
//  Copyright © 2018 Kartik Kothari. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "interface.h"

#define N 100
#define pro_det 0.9
//double pro_det;

typedef struct Node Node;

struct Node
{
    int node_id;
    int flag;       // flag = 0 if not detected, flag = 1 if detected
    double xco;
    double yco;
};

double s;           // s is side length of triangle - will also be used to construct the vectors
double v1x, v2x, v2y;
int current_nodes;

// create N nodes
// 1st node has coordinate (0,0)

// defining vectors that will allow traversal along triangular grid
// consider them as global variables - and then use them whenever needed.
void vector_construction(double side)
{
    v1x = side;
    v2x = -0.5*side;
    v2y = sqrt(3.0)/2.0*side;
}

// generating new nodes
/* we hop on to new ones i.e. consider generation from points that have already been generated, we will need to randomly choose a node from those generated i.e. a random integer from 0 to current_nodes-1 where current_nodes stores the number of nodes that have already been created.
 every time we land on a node, we select 2 independent random numbers amongst {-1, 0, 1} which gives us direction of traversal in x and y
 once we have the two numbers we use coordinates of the node we are on along with vectors defined above to create a new node
 but we will need to ensure we are not creating an already existing node
 we do this through scanning through the array of nodes generated and checking whether the newly generated node is present in that
 if it is not found it is added to the array, else, we generate a new node again
 */

Node make_node(int number, Node ref) // input is current_nodes
{
    Node temp;
    // select directions for x and y
    int x1, y1;
    x1 = rand()%3 - 1;
    y1 = rand()%3 - 1;
    
    temp.xco = ref.xco + (double)x1*v1x + (double)y1*v2x;
    temp.yco = ref.yco + (double)y1*v2y;
    temp.node_id = number;
    temp.flag = 0;
    
    return temp;
}

// function to scan whether node with number n1 is already present in the array of nodes called records
int scan_list(Node *records, int n1)
{
    int j;
    for(j = 0; j < n1; j++)
    {
        if( (records[n1].xco==records[j].xco)&&(records[n1].yco==records[j].yco) )
        {
            return 0;
        }
    }
    return 1;
}

// Detected nodes and randomizing their positions
// Once the new nodes are generated, we assign flag values as 1 or 0 for each of them by generating a random number between 0 and 1 and then checking whether it lies between 0 and pro_det. If so, then flag = 1, else flag = 0;
// For flags that become 1, we also randomize the positions of the nodes by adding a delta x and delta y.
// delta x is going to be a random number in [-1,1] times a fraction of s (s/10 here). Similar for delta y.
int det_nodes(Node *cen)
{
    double num, delx, dely;
    num = ((double)rand()/(double)RAND_MAX);
    if(num<=pro_det)                // node has been detected
    {
        cen->flag = 1;
        // randomly assigned delx
        delx = (2.0*((double)rand()/(double)RAND_MAX) - 1.0)*s/10.0;
        cen->xco = cen->xco + delx;
        // randomly assigned dely
        dely = (2.0*((double)rand()/(double)RAND_MAX) - 1.0)*s/10.0;
        cen->yco = cen->yco + dely;
        return 1;
    }
    else
    {
        return 0;
    }
}

double **malloc_matrix(int n2)
{
    int i;
    double **mt = (double**)malloc(n2*sizeof(double*));
    if(mt==NULL)
    {
        printf("\n malloc failed \n");
        return NULL;
    }
    for(i = 0;i < n2;i++)
    {
        mt[i] = (double*)malloc(n2*sizeof(double));
        if(mt[i]==NULL)
        {
            printf("\n malloc failed \n");
            return NULL;
        }
    }
    return mt;
}

// Function to print matrix
void print_matrix(int n1, double **a)   // prints matrix a
{
    int i,j;
    for(i = 0;i < n1;i++)
    {
        for(j = 0;j < n1;j++)
        {
            printf("%f\t", *( *(a+i) + j));
        }
        printf("\n");
    }
}

// Function to free memory allocated to matrix
void free_matrix(int n1, double **a)
{
    int i;
    for(i = 0;i < n1;i++)
    {
        free(a[i]);
    }
    free(a);
}

// Function to calculate distance between two nodes
double distance(Node p1, Node p2)
{
    double dist;
    dist = pow( (p1.xco - p2.xco), 2.0) + pow( (p1.yco - p2.yco) , 2.0);
    dist = sqrt(dist);
    return dist;
}

int main(int argc, const char * argv[]) {
    // insert code here...
    //printf("Hello, World!\n");
    srand(time(0));
    
    FILE* ofp;
    char* outputfile = "./varying.txt";
    ofp = fopen(outputfile,"w");
    
//    int z = 0, y = 0;
//
//    for(z = 1; z < 10; z = z + 1)
//{
//    pro_det = (double)(z)/10.0;
    
    Node* list = (Node*)malloc(sizeof(Node)*N);
    if(list==NULL){
        printf("malloc error\n");
    }
    s = 10;
    // initializing node 0 to (0,0)
    list[0].node_id = 0;
    list[0].xco = 0.0;
    list[0].yco = 0.0;
    list[0].flag= 0;
    current_nodes = 1;
    vector_construction(s);
    
    int i, num, ctr;            //ctr is used here as flag to check whether new node is unique or already exists in list
    Node center;
    for(i = 1;i<N;i++)
    {
        ctr = 0;
        while (ctr==0)
        {
            num = rand()%current_nodes;
            center = list[num];
            list[i] = make_node(current_nodes, center);
            // scan if list[i] is present in list[0] to list[i-1]
            ctr = scan_list(list, i);
        }
        current_nodes++;
    }
    
    //print nodes (id, xco, yco) to check - with 50 nodes.
    printf("node id\t x-coo\t y-coo\n");
    for(i = 0;i<N;i++)
    {
        printf("%d\t %lf\t %lf\t", list[i].node_id, list[i].xco, list[i].yco);
        printf("\n");
    }
    
    ctr = 0;
    // definition of ctr is changed - ctr is now used for counting number of nodes detected i.e. with flag 1
    // Making the secondary graph - representing it as adjacency matrix
    // We scan through nodes that have flag 1 and get a count of them - N2. make an array of structs to store them - graphnodes[N2]
    
    for(i = 0;i<N;i++)
    {
        ctr += det_nodes(&list[i]);
    }
    printf("number of detected nodes %d\n", ctr);
    
    Node *graphnodes = (Node*)malloc(sizeof(Node)*ctr);
    if(graphnodes==NULL){
        printf("malloc error\n");
    }
    int j = 0;
    for(i = 0;i<ctr;i++)
    {
        while(list[j].flag==0)
        {
            j++;
        }
        graphnodes[i] = list[j];
        j++;
    }
    
    printf(" All detected Nodes \n");
    printf("x-coo\t y-coo\n");
    for(i = 0;i<ctr;i++)
    {
        printf("%lf\t %lf\t", graphnodes[i].xco, graphnodes[i].yco);
        printf("\n");
    }
    
    free(list);
    
    // we make a double matrix of N2xN2 - adjmat[N2][N2]
    // we fill up the adjmat[i][j] with distance between graphnodes[i] and graphnodes[j] - just access coordinates of both graphnodes and distance formula.
    // obviously this matrix is symmetric and we only need to fill in upper or lower half - then we can just use adjmat[i][j] = adjmat[j][i] to fill in the rest.
    
    // building adjacency matrix for detected nodes
    double** adjmat;
    adjmat = malloc_matrix(ctr);
    for(i = 0;i<ctr;i++)
    {
        for(j = 0;j<ctr;j++)
        {
            adjmat[i][j] = distance(graphnodes[i], graphnodes[j]);
        }
    }
    
    //print_matrix(ctr, adjmat);
    int* tour;
    int size_tour = -1;
    double MST_len = -1.0;
    int size = ctr;
    
    // running the routing algorithms here
    tour = calculate(adjmat, &size_tour, &MST_len, size);
    if(tour==NULL){
        printf("operation failed\n");
	return -1;
    }

    int tmp = -1;
    
    // flip the tour
    for(i = 0;i<size_tour/2;i++)
    {
        tmp = tour[i];
        tour[i] = tour[size_tour - i - 1];
        tour[size_tour - 1 - i] = tmp;
    }
    
    printf("\n size of tour %d and mstlen %lf \n", size_tour, MST_len);
//    for(i = 0;i<size_tour;i++)
//    {
//        printf(" node %d : %d \n",i, tour[i]);
//    }
    
    // finding out number of unique nodes in tour
    int ctr_cyc = 0, flag1 = 0;
    for(i = 0; i< size_tour;i++)
    {
        flag1 = 0;
        for(j = 0;j<i;j++)
        {
            if(tour[i]==tour[j])
            {
                flag1 = 1;
                break;
            }
        }
        if(flag1==0)
        {ctr_cyc++;}
    }
    
    printf(" number of unique elements are %d \n", ctr_cyc);
    
    // making the hamiltonian cycle (final solution) by shortcutting
    ctr_cyc++;          // this additional increment since duplicates remove the fact that last and first node are the same in the tour
                        // we will need this in making the hamiltonian cycle
    int *cycle = (int*)malloc(sizeof(int)*ctr_cyc);
    if(cycle==NULL){
        printf("malloc error\n");
    }
    
    cycle[0] = tour[0];
    cycle[ctr_cyc-1] = cycle[0];    // last element of cycle will be same as starting element
    int k = 0;
    for(i = 0;i<size_tour;i++)
    {
        flag1 = 0;
        for(j = 0;j<i;j++)
        {
            if(tour[i]==tour[j])
            {
                flag1 = 1;
                break;
            }
        }
        if(flag1==0)
        {
            cycle[k] = tour[i];k++;
        }
    }
    
//    printf(" nodes in H-cycle \n");
//    for(i = 0;i<ctr_cyc;i++)
//    {
//        printf(" node %d : %d \n",i, cycle[i]);
//    }
    printf(" nodes in H-cycle \n");
    printf("x-coo\t y-coo\n");
    for(i = 0;i<ctr_cyc;i++)
    {
        printf("%d %lf\t %lf\t", cycle[i], graphnodes[cycle[i]].xco, graphnodes[cycle[i]].yco);
        printf("\n");
    }
    
    // calculating length of path computed by Christofides heuristic
    double zlen = 0.0;
    for(i = 0;i<ctr_cyc-1;i++)
    {
        zlen += distance(graphnodes[cycle[i]], graphnodes[cycle[i+1]]);
    }
    
    // calculating deviation
    printf(" solution path length %lf \n", zlen);
    double dev = (zlen - MST_len)/zlen*100.0;
    double dev1 = (zlen - MST_len)/MST_len*100.0;
    printf(" deviation is %lf \n", dev1);
    
    // free-ing the memory allocated
    free(cycle);
    free(tour);
    free_matrix(ctr, adjmat);
    free(graphnodes);
    
    fprintf(ofp, "%lf %lf %lf %lf \n", pro_det, MST_len, zlen, dev1);
//}
    fclose(ofp);
    
    return 0;
}
