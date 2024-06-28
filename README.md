Project 1 Writeup

The following is the write-up for this personal project completed by Archan Tulpule.

Project Description: Solution to NP-hard Set-cover problem for undergraduate class CSE 431 at MSU. 

Skills demonstrated: C++, NP-completeness, depth-first search, algorithmic analysis, branching & bounding techniques for tackling exponential-time problems.

Solution description: 

The set cover problem is a classic NP-hard problem that includes choosing the minimum number of sets of some arbitrary elements such that the union of those subsets includes all the elements in the universe. In this specific formualtion, the problem was to find the minimum number of subsets such that all the key terms were met after being given the full list of key terms. 

The file SetCover.cpp contains the implementation of SetCover class declared in the header file SetCover.hpp. Branch and bound algorithms are straightforward algorithms that try to find a solution to a problem by pruning and bounding the search space so as to reduce the computation time. This solution includes a few preprocess steps such as removing redundant subsets (duplicates and subsets of other found supersets) and sorting in order of decreasing subset size. The depth-first search naturally lends itself to the solution implementation since it allows effective updating and use of bounding. 

The main algorothmic tools used:
1) Bounding- keeping a track of the current "best" solution, and automatically pruning branches that would only incur a worse solution; and boudning
2) Branching- pruning branches that can't possibly reach a solution, ie remove branches if the sum of the current solution set and elements in the remaining subsets is less than the length of elements in the universe.

Note: The code in main.py is the original python code for the homework assignment I completed in my CSE 431 class. In this project, the code was adapted into C++ in the SetCover.cpp and SetCover.hpp files.

The project was made and compiled with MacOS Clang++ compiler