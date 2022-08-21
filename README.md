# spkmeans 
## intro 
Implementation of the normalized spectral clustering algorithm.<br/>
Calculation implementation in C. <br/>
Interface implemented: <br/>
a. _In C_ <br/>
b. _In python_ - using the C-Python API methodology <br/>
<br/>
## files <br/>
- _spkmeans.py_ - Python interface of the code
- _spkmeans.h_ - C header file
- _spkmeans.c_ - C interface of the code
- _spkmeansmodule.c_ - Python C API wrapper
- _setup.py_ - The setup file - of the C-PYTHON API
## Run using C
1. Compile the kmeans.c file using the command line below: 
```
gcc -ansi -Wall -Wextra -Werror -pedantic-errors spkmeans.c -lm -o spkmeans 
```
2. Run the command line below: 
```
./spkmeans [k] [goal] [file name]
```
while:
1. k (int, < N): Number of required clusters for kmeans. If equal 0, use the heuristic of the spectral clustering.
2. goal (enum): Can get the following values:
- spk: Perform kmeans - full spectral process in k=0, regular kmeans otherwise.  _Kmeans here takes first K points as initialization centroids_.  
- wam: Calculate and output the Weighted Adjacency Matrix  
- ddg: Calculate and output the Diagonal Degree Matrix of the Weighted Adjacency Matrix  
- lnorm: Calculate and output the Normalized Graph Laplacian of the Diagonal Degree Matrix
- jacobi: Calculate and output the eigenvalues and eigenvectors of the lnorm matrix 
3. file name: The path to the file that will contain N observations, the file extension is .txt
or .csv.

## Run using python 
1. Setup the python library on your pc. using the command line below: 
```
python setup.py build_ext --inplace
```
2. Run the command line below: 
```
python3 spkmeans.py [k] [goal] [file name]
```
while:
1. k (int, < N): Number of required clusters for kmeans. If equal 0, use the heuristic of the spectral clustering.
2. goal (enum): Can get the following values:
- spk: Perform kmeans - a full spectral process in k=0, regular kmeans otherwise.  _Kmeans here use the  K-means++ method to define the initialization centroids._
- wam: Calculate and output the Weighted Adjacency Matrix  
- ddg: Calculate and output the Diagonal Degree Matrix of the Weighted Adjacency Matrix  
- lnorm: Calculate and output the Normalized Graph Laplacian of the Diagonal Degree Matrix
- jacobi: Calculate and output the eigenvalues and eigenvectors of the lnorm matrix 
3. file name: The path to the file that will contain N observations, the file extension is .txt
