//
//  interface.h
//  nearest
//
//  Created by Kartik Kothari on 11/22/18.
//  Copyright © 2018 Kartik Kothari. All rights reserved.
//

#ifndef interface_h
#define interface_h

/*
 Function to run Christofides algorithm.
 Function returns an integer pointer to an Eulerian tour - which will be 'short-cutted' in main to give solution fot the Christofides Algorithm
 @param AdjMat_Wt of type double pointer to an adjacency matrix
 @param sizetour, integer pointer to hold address of variable indicating the size of the eulerian tour
 @param MSTlen, double pointer to hold length of the MST
 @param SIZE of type in to indicates size of the detected node graph
 @return pointer to the eulerian tour if operation successful, NULL otherwise
 */
int* calculate(double** AdjMat_Wt,int* sizetour,double* MSTlen,int SIZE);

#endif /* interface_h */
