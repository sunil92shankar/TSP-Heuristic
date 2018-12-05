Author          : Sunil Jaishankar, Kartik Kothari
Created         : December 4, 2018




Affiliation          : Georgia Institute of Technology




Description
-------------


Surveillance Routing of Drones on a 2D Grid Based Graph


(For background) Traveling Salesman Problem (TSP) looks to solve the problem of a minimum distance/cost path that a traveling salesman can take to visit a certain number of cities (nodes) and return to the source node. Since the exact problem is NP hard, various heuristic solutions have been implemented as an approximation.


This is an application which provides a heuristic solution to the TSP by implementing Christofides algorithm in a special case where a 2D grid-based graph is considered. 


The application consists of the following components.


Program 1 (routing.c) - This program implements the routing algorithms in terms of formulating an MST, and running the Christofides heuristic and returns an Euler circuit through the function called calculate. 


Program 2 (main.c) - This is the central component where graph generation takes place. Node detection, position randomization and generation of adjacency matrix is implemented here. Once the Eulerian circuit is received, shortcutting is implemented to give Hamiltonian cycle. Analytics is also provided here.


Header 1 (Euler.h) - This is a header file providing functioning of detecting unique cycles in the multigraph (intermediate step in routing algorithm) and combines them to come up with an Eulerian circuit.


Header 2 (routing_header.h) - It defines the data structures and functions for implementing MST, Low Cost Perfect Matching and multigraph generation.


Header 3 (interface.h) - It provides the interface between the graph generation and the routing algorithms by providing the calculate function which returns an integer pointer to the Eulerian circuit created by routing algorithms.




Installation
------------


To install, simply run


        gcc main.c routing.c -std=c99 -o test


Note: The -std=c99 flag is required on some C compilers
to allow variable declarations at arbitrary locations in
a function. 
Note: routing.c, main.c, interface.h, routing_header.h and Euler.h must be placed in the same folder.


Execution
-----------


    ./test


Notes
-----------


Although the presented format of the code doesn’t take user inputs, one can easily change the probability of node detection and number of nodes in 
primary node set since they have been implemented through #define in the program. The probability of detection is denoted by “pro_det” and number of 
nodes is denoted by “N”.