##  Traveling Salesman Problem (TSP) in C++

This project implements the Traveling Salesman Problem (TSP) using dynamic programming techniques. The TSP is a classic optimization problem in computer science and operations research, known for its NP-hard complexity. The goal is to find the shortest possible route that visits a set of cities and returns to the origin city.
Implementation Details

We leverage dynamic programming to efficiently explore possible routes. This method significantly reduces the computation time compared to brute-force approaches by storing previously computed results.
The project explores two implementations of disjoint sets and DFS, which are used to manage and group cities during the search:

    Disjoint Sets as a Binary Search Tree (BST): This implementation organizes the cities in a binary tree structure, facilitating quick lookups and unions of disjoint sets.

    Disjoint Sets as a List: This alternative implementation uses a list-based approach to manage disjoint sets, offering a simpler structure while still maintaining the ability to efficiently combine sets.

Usage

To run the program, clone this repository and follow these steps:

    Ensure you have a C++ compiler installed.
    Navigate to the project directory.
    Compile the source files.
    Run the executable with the desired input of cities and distances.


The project is still a work in progress.
