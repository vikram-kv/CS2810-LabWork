/************************
* $ID$
* File:  CS19B021_A2.cpp - To define a class Matrix with binarySearch() and matrixSearch() operations and to define an inherited class MatrixMedian with findMedian() and upperBound() operations.
*
* Purpose:  class Matrix - n*m matrix with rows sorted in asc. order.
*            binarySearch(x,k) - Searches for an element(x) in a row(k) using binary search and returns the largest index j such that A[k][j] <= x. If x < A[k][j], returns -1.
*            matrixSearch(x) - Searches for elem x in the matrix. returns the first (i,j) in row-major representation such that A[i][j] = x.If x is not found, returns (-1,-1).
*            class MatrixMedian - inherited from Matrix
*            upperBound(x,k) - uses inherited binarySearch to find the smallest index j such that A[k][j] > x. Returns -1 if x >= A[k][colCount-1].
*            findMedian() - finds the median of the matrix entries in expected O(nlogm) time and in expected O(1) space.
*
* Author: K V VIKRAM
*
* Created: [2021-02-15 15:01]  by vikram
* 
* Last Modified: [2021-02-16 09:01]  by vikram
*
* Input Format:
* First line of the input contains two space-separated integers n and m denoting the number of rows and the number of columns respectively of the input matrix A.
* Following n lines contain m space-separated integers each, describing the rows of the matrix.
* Next line contains Q, the number of queries to be performed on the matrix.
* Each of the next Q lines represents a single query of the format:
*   i) BS x k : Call binarySearch(x, k)
*   ii) MS x : Call matrixSearch(x)
*   iii) Median : Call findMedian()
*
* Constraints:
* 1 <= n,m <= 10^5
* n*m <= 10^6
* n*m is an odd number
* -2^31 <= all matrix entries, x <= 2^31 - 1
* 0 <= k <= (n-1)
* 1 <= Q <= 100
*
* Output Format:
* Output of each query in a new line.
* BS x k : Print Output of binarySearch(x,k)
* MS x : Print i j (space separated) if element is found, else print -1 -1
* Median : Print median element 
* 
* Assumption: Sufficient dynamic memory is assumed to be available.This exception is neither caught nor handled.
*
* Practice:   Array iterators in loops are typically named i or j. Specifically, for matrices , i is used for rowIndex and j for colIndex.
*
* Compilation:  g++ CS19B021_A2.cpp -o solution, ./solution.
*
* Bugs:     No major bugfixes or new releases
*************/

#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>
using namespace std;

/* Begin {Definition of Matrix} */
/*
    IndexPair - To represent a pair of indices in a matrix.
    Data Members - rowIndex(int), colIndex(int)
    Member Function - printPair() - displays rowIndex + ' ' + colIndex + '\n'
    Constructor - default - printPair() : sets both rowIndex and colIndex to 0
                  parameterised - printPair(int,int) : sets rowIndex to param1 and colIndex to param2
*/
struct IndexPair
{
    int rowIndex,colIndex;
    void printPair()
    {
        cout << rowIndex << ' ' << colIndex << endl;
    }
    IndexPair()
    {
        rowIndex=colIndex=0;
    }
    IndexPair(int a,int b)
    {
        rowIndex=a;
        colIndex=b;
    }
};
/* End {Definition of IndexPair} */

/* Begin {Definition of Matrix} */
/*  Matrix - ADT to store a 2D integer matrix in which rows are sorted in ascending order and implement operations of binarySearch() and matrixSearch().
    Data Members - rowCount(int),colCount(int),value(int**)
    Mutator- getMatrix()
    Accessor- binarySearch(int,int), matrixSearch(int)
    Constructor - Matrix(int,int)
    Destructor - ~Matrix()
*/
class Matrix
{
protected:
    int rowCount,colCount;          //rowCount - no of rows. colCount - no of columns
    int** value;                    //value - double ptr to store 2D matrix of size rowCount*colCount
public:
    Matrix(int,int);                //Parametrised constructor to allocate space for 2D matrix and set all entries to 0
    void getMatrix();               //Gets matrix entries from stdout
    int binarySearch(int,int);      //Performs binary search() as in documentation
    IndexPair matrixSearch(int);    //Searches the matrix for a particular value
    ~Matrix();                      //Deallocates space from memory
};
/* End {Definition of Matrix} */

/* Begin {Implementation of Matrix} */

/*  Matrix(int,int) - Creates an object to allocate space and store a a*b matrix. Sets all entries to 0.
    ARGS - int a - number of rows, int b - number of columns 
*/
Matrix::Matrix(int a,int b)
{
    rowCount = a;
    colCount = b;
    value = new int*[rowCount];
    for(int i=0;i<rowCount;i++)
    {
        value[i] = new int[colCount];
    }
}

/*  getMatrix() - Gets the entries of value from stdin(Assumed that input integers are space or '\n' separated)
    ARGS - void
    RET - void 
*/
void Matrix::getMatrix()
{
    for(int i=0;i<rowCount;i++)
    {
        for(int j=0;j<colCount;j++)
        {
            cin >> value[i][j];
        }
    }
}

/*  binarySearch() - Searches for an element(x) in a row(k) using binary search and returns the largest index j such that value[k][j] <= x. If x < all elements, returns -1.
    ARGS - x (int), k (int)
    RET - int 
    NOTE - value[k][minIndex..maxIndex] is the segment under consideration in the loop, in the sense that reqd j is st minIndex <= j <= maxIndex.
           the length of segment decreases after every iteration, so termination is guaranteed. To ensure this, minIndex = ceil((minIndex+maxIndex)/2).
*/
int Matrix::binarySearch(int x,int k)
{
    if(x < value[k][0])                                         //When x < all elements in kth row, return -1
        return -1;
    else                                                        //There is atleast one element lte x
    {
        int minIndex = 0, maxIndex = colCount - 1, midIndex;    //minIndex - lower index, maxIndex - upper index and midIndex - middle Index of array segment under consideration
        while(minIndex < maxIndex)                              //If segment is of size 1, j = minIndex = maxIndex
        {
            midIndex = (minIndex+maxIndex+1)/2;                 //minIndex = ceil((minIndex+maxIndex)/2)
            if(value[k][midIndex]>x)                            //j is in the segment value[k][minIndex..(midIndex-1)]
                maxIndex = midIndex-1;
            else                                                //j is in the segment value[k][midIndex..(maxIndex)]
                minIndex = midIndex;
        }
        return minIndex;
    }
}

/*  matrixSearch() - Searches for an element x in a matrix. Returns the first (i,j) in row-major order st value[i][j] = x if found.Otherwise, ret (-1,-1).
    ARGS - int x
    RET - IndexPair 
*/
IndexPair Matrix::matrixSearch(int x)
{
    int upperBound;                                         //The first index j st value[i[j] > x. Can be colCount, which means x >= all entries in (i+1)th row
    for(int i=0;i<rowCount;i++)                                    //Iterate over rows of the matrix
    {
        upperBound = binarySearch(x,i)+1;                   //setting upperBound for the (i+1)th row
        if(upperBound == 0 || value[i][upperBound-1]!= x)   //if x is not found in the (i+1)th row
            continue;
        else                                                //if x is found in the (i+1)th row
        {
            int j;
            for(j=upperBound-2;j>=0;j--)                    //loop to return the smallest j st value[i][j] = x
            {
                if(value[i][j] != x)
                    break;
            }
            return IndexPair(i,j+1);                        //returning the reqd index pair for x
        }
    }
    return IndexPair(-1,-1);                                //Here, x is not found in the whole matrix
}

/*  ~Matrix() - Destroys Matrix objects by de-allocating dynamic memory.
    ARGS - void 
*/
Matrix::~Matrix()
{
    for(int i=0;i<rowCount;i++)
    {
        delete[] value[i];
    }
    delete[] value;
}

/* End {Implementation of Matrix} */

/* Begin {Definition of MatrixMedian} */
/*  MedianMatrix - Class inherited from Matrix with visibility public
    Data Members - inherited : protected - value(int**),rowCount(int),colCount(int)
    Mutator- inherited : getMatrix()
    Accessor- inherited : binarySearch(int,int), matrixSearch(int)
    Constructor - MedianMatrix(int,int) which invokes Matrix's constructor
    Destructor - default one which invokes Matrix's destructor
*/
class MatrixMedian : public Matrix
{
public:
    int upperBound(int,int);                            //Finds the upperbound of a given number in a given row
    int findMedian();                                   //Finds the median of all the integers in the matrix
    MatrixMedian(int a,int b) : Matrix(a,b) {};         //Constructor to initialise data members(inherited) by invoking base class constructor
};
/* End {Definition of MatrixMedian} */

/* Begin {Implementation of MatrixMedian} */
/*  upperBound -  use the inherited binarySearch() method to find the smallest index j such that value[k][j] > x. Returns -1 if x >= value[k][colCount-1]
    ARGS - int x, int k
    RET - int
*/
int MatrixMedian::upperBound(int x,int k)
{
    int ret = binarySearch(x,k);            //binary search gives the index of the largest number <= x in array. so, ub = this index + 1(if it is in row)
    if(ret == rowCount - 1)                 //ub is out of row
        return -1;
    else 
        return ret+1;                       //ub is returned
}

/*  findMedian -  use the inherited binarySearch() method to find the median of all integers in the matrix
    ARGS - void
    RET - int
    TECH - take care to avoid overflow out of INT's range.
*/
int MatrixMedian::findMedian()
{
    int small=value[0][0],large=value[0][colCount-1],mid;                   //small - smallest number,large- largest number and mid = (small + large) div 2.
    for(int i=0;i<rowCount;i++)                                             //loop to set small and large to their values
    {
        if(value[i][0]<small)                                               //finding the smallest elem by comparing all numbers in column 1
            small = value[i][0];
        if(value[i][colCount-1]>large)                                      //finding the largest elem by comparing all numbers in last column
            large = value[i][colCount-1];
    }
    int reqPos = (rowCount*colCount+1)/2;                                   //position of median in a set of ordered integers with odd size
    int curPos;                                                             //position of elem under consideration in loop. calc as number of numbers <= current number
    mid = (large>0&&small<0?(large+small)/2:(small+(large-small)/2));       //taking care to avoid overflow. if num of opp signs are added, + can be used. otherwise, - shd be used. this is reqd for first time only.
    while(small<large)
    {
        curPos = 0;
        for(int i=0;i<rowCount;i++)                                         //loop to find curPos using binarySearch()
            curPos += binarySearch(mid,i)+1;
        if(curPos<reqPos)                                                   //if the curPos is lt reqPos, median must be in mid+1 .. large
            small = mid + 1;
        else                                                                //otherwise, median must be in small .. mid
            large = mid;
        mid = small+(large-small)/2;                                        //mid is computed. works even if small and large are of same sign and have large magnitudes.
    }
    return small;
}
/* End {Implementation of MatrixMedian} */


int main()
{
    int n,m;                                                //rowCount - n and colCount - m
    cin >> n >> m;
    MatrixMedian M(n,m);                                    //Input matrix M is constructed with size n*m
    M.getMatrix();
    string query;                                           //= BS , MS or Median
    int queryCount;                                         //no of queries Q
    cin >> queryCount;
    int x,k;                                                //x = search elem, k = search row
    IndexPair pair;                                         //stores indices returned by matrixSearch()
    for(int i = 0;i<queryCount;i++)                         //loop to exe all queries
    {
        cin >> query;
        if(query  == "BS")                                  //binarySearch()
        {
            cin >> x >> k;
            cout << M.binarySearch(x,k) << endl;
        }
        else if(query == "MS")                              //matrixSearch()
        {
            cin >> x;
            pair = M.matrixSearch(x);
            pair.printPair();
        }
        else if(query == "Median")                          //findMedian()
        {
            cout << M.findMedian() << endl;
        }
        else                                                //illegal query
        {
            exit(-1);
        }
    }
    return 0;
}