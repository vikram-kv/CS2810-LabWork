/************************
* $ID$
* File:  CS19B021_A1.cpp - Defines a class Matrix for integer square matrices and also implements operators which perform special matrix operations on the class.
*
* Purpose:  Class Matrix Implementation. Definition of Special Matrix Operations on the class Matrix.
*            Special Matrix Operations - 
*            i) (SPSUM) Special Sum '+' : This is defined on one matrix. Add row index(i) and column index(j) of each element of the matrix. 
*           If this sum is odd, swap the element A[i][j] with A[j][i]. Return the transformed matrix(by pointer).
*            ii) (MOD) Mod Multiplication '&': This defined on one matrix. If row index(i) is divisible by 3, find the max element of that row. 
*            If i gives 1 on division by 3, then find the minimum element of that row. Otherwise, find the floor of average of that row. 
*            Return the product of these elements.
*            iii) (MRS) Matrix Right Shift '>>' : This is defined on 2 matrices of same size. Given 'k' as input, 
*            Perform right shift operation on both the matrices(k times).Return the sum of these shifted matrices(by pointer).
*            Shifting is done in row-major fashion cyclically.
*            iv) (MADD) Special Matrix Addition '+' : This is also defined on 2 matrices of same size.Let A = opnd1 and B = opnd2 
*            Given 'x', If x = 1, return A^T+B. If x = 2, return A + B^T. (Here, X^T is the transpose of X).Both returns are by pointer.
*
* Author: K V VIKRAM
*
* Created: [2021-02-08 16:23]        by vikram
* 
* Last Modified:[2021-02-09 07:37]        by vikram 
*
* Input Format:
* Line 1: Q, Number of queries.
* Line 2: n, Number of rows/columns of the square matrix.
* Next 2n lines: Row elements of the two matrices(space separated). [First n lines are rows of first matrix A, next n lines are rows of second matrix B]
* Next Q lines: query operations to be performed [single query in each new line].
*      
* Constraints:
* 1<=Q<=8
* 1<=n<=50
* 1<=x<=2
* 0<=k<=50
* -1000 <=M[i][j]< 1000
*
* Output Format:
* SPSUM : n numbers which comma-separated, all in the same line.(Sum of elements rowwise of the result of operation on A)
* MOD : An integer in a single line.(Result of operation on A)
* MRS & MADD: Result Matrix is printed in the following n lines.(Result of resp. operations with opnd1 = A and opnd2 = B)
* 
* Assumption: Throughout the program, it is assumed that operations are performed on compatible matrices.Also, sufficient dynamic memory is assumed to be available. 
*             Both these exceptions are neither caught nor handled.
* Practice:   Array iterators in for loops are typically named i or j. Specifically, for matrices , i is used for rowIndex and j for colIndex.
*
* Compilation:  g++ CS19B021_A1.cpp -o solution, ./solution.
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
/*  Matrix - ADT to store a square matrix and to implement special matrix operations on square matrices. 
    Data Members - value (int**), matSize(int)
    Mutator- swap(),getMatrix(),transpose(), operator +=()
    Accessor- rowMax(),rowMin(),rowAvg(),printMatrix(),printRowSum(), operators - +(),&(),+(Matrix&),>>(Matrix&)
    Constructor - Matrix(),Matrix(int),Matrix(Matrix&)
    Destructor - ~Matrix()
*/
class Matrix
{
    int ** value;                    //double ptr to store the 2D array representing the square matrix
    int matSize;                    //dimension of the square matrix
    void swap(int*,int*);            //swaps two integers
    int rowMax(int);                //returns the max element in a row
    int rowMin(int);                //returns the min element in a row
    int rowAvg(int);                //returns floor(average of elements in a row)
public:
    void getMatrix();                //gets the matrix entries
    void printMatrix();                //prints the matrix row by row
    void printRowSum();                //prints the sum of elements row-wise separated by ,
    void transpose();                //mutates the matrix to its transpose
    Matrix();                        //Default constructor
    Matrix(int);                    //Constructor to set matSize and initialize entries to 0.
    Matrix(Matrix&);                //Copy constructor(non-default). Allocates space and copies entries. 
    void operator +=(Matrix&);      //Operator +=: A+=B is same as A = A + B              
    Matrix* operator +();            //Operator '+': SPSUM operation in documentation
    long long operator &();            //Operator '&': MOD operation in documentation
    Matrix* operator >>(Matrix&);    //Operator '>>': MRS operation in documentation
    Matrix* operator +(Matrix&);    //Operator '+': MADD operation in documentation
    ~Matrix();                        //Destructor to delete allocated dynamic memory
};
/* End {Definition of Matrix} */


/* Begin {Implementation of Matrix} */
/*  swap() - swaps matrix entries(*a and *b) using pointers
    ARGS - int* a, int* b 
    RET - void
*/
void Matrix::swap(int* a,int* b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

/*  rowMax() - returns the largest integer in the row with index rowIndex
    ARGS - int rowIndex
    RET - int 
*/
int Matrix::rowMax(int rowIndex)
{
    int max = value[rowIndex][0];
    for(int j=1;j<matSize;j++)
        if(value[rowIndex][j]>max)
            max = value[rowIndex][j];
    return max;
}

/*  rowMax() - returns the smallest integer in the row with index rowIndex
    ARGS - int rowIndex
    RET - int 
*/
int Matrix::rowMin(int rowIndex)
{
    int min = value[rowIndex][0];
    for(int j=1;j<matSize;j++)
        if(value[rowIndex][j]<min)
            min = value[rowIndex][j];
    return min;
}

/*  rowMax() - returns floor(average of entries in the row with index rowIndex)
    ARGS - int rowIndex
    RET - int 
*/
int Matrix::rowAvg(int rowIndex)
{
    int sum=0;
    for(int j=0;j<matSize;j++)
    {
        sum+=value[rowIndex][j];
    }
    return ((int)sum/matSize);
}

/*  getMatrix() - gets the entries of value from stdin(Assumed that input integers are space or '\n' separated)
    ARGS - void
    RET - void 
*/
void Matrix::getMatrix()
{
    for(int i=0;i<matSize;i++)
    {
        for(int j=0;j<matSize;j++)
            cin >> value[i][j];
    }
}

/*  printMatrix() - prints the entries of value array row by row, with entries separated by space and rows separated by newline
    ARGS - void
    RET - void 
*/
void Matrix::printMatrix()
{
    for(int i=0;i<matSize;i++)
    {
        for(int j=0;j<matSize;j++)
            cout << value[i][j] << ' ';
        cout << endl;
    }
}

/*  printRowSum() - prints s(0),s(1),...s(matSize-1) separated by ',' , where s(i) = sum of entries in row(i).
    ARGS - void
    RET - void 
*/
void Matrix::printRowSum()
{
    int curSum=0;
    for(int i=0;i<matSize;i++)
    {
        curSum=0;
        for(int j=0;j<matSize;j++)
        {
            curSum+=value[i][j];
        }
        if(i!=(matSize-1))
            cout << curSum << ",";
    }
    cout << curSum << endl;
}

/*  transpose() - Mutates the matrix to its transpose
    ARGS - void
    RET - void 
*/
void Matrix::transpose()
{
    //nested loops to swap value[i][j] and value[j][i] for j<i and (i,j) belong to N^2 where N={0,1,..,matSize-1}
    for(int i=0;i<matSize;i++)
    {
        for(int j=0;j<i;j++)
            swap(&value[i][j],&value[j][i]);
    }
}

/*  Matrix() - Initializes matSize with 0 and value with nullptr. No memory is allocated
    ARGS - void 
*/
Matrix::Matrix()
{
    matSize = 0;
    value = nullptr;
}

/*  Matrix() - Initializes matSize with size and value as a pointer to a 2D square array of dimension size.All entries of value are set to 0.
    ARGS - int size
*/
Matrix::Matrix(int size)
{
    matSize = size;
    value = new int*[size];            //value is set to base address of an array of 'size' many int* ptrs.
    for(int i=0;i<matSize;i++)
    {
        value[i] = new int[size];    //Each value[i] (int*) ptr is set to base address of an array of 'size' many integers corr. to row(i) of the represented matrix
        for(int j=0;j<matSize;j++)
            value[i][j] = 0;
    }
}

/*  Matrix() - User defined copy constructor. Allocates memory for value[][] 2D array and sets matSize to M.matSize. Corresponding entries of M.value[][] are copied to value[][]
    ARGS - Reference to a Matrix - Matrix& M
    REF - compiler provided copy constructor will cause errors due to scope issues
*/
Matrix::Matrix(Matrix& M)
{
    matSize = M.matSize;
    value = new int*[matSize];
    for(int i=0;i<matSize;i++)
    {
        value[i] = new int[matSize];
        for(int j=0;j<matSize;j++)
            value[i][j] = M.value[i][j];
    }
}

/* Binary Operator +=() - A += M is same as A = A + M, given A.matSize = M.matSize
   ARGS - Reference to a matrix - Matrix&
   RET - void
*/
void Matrix::operator +=(Matrix& M)
{
    for(int i=0;i<matSize;i++)
    {
        for(int j=0;j<matSize;j++)
        {
            value[i][j]+=M.value[i][j];
        }
    }
}

/* Unary Operator +=() - +A is the matrix obtained by performing SPSUM operation on A. 
                         Memory is alloc dynamically and address of +A is returned
   ARGS - void
   RET - Pointer to a Matrix - Matrix*
*/
Matrix* Matrix::operator +()
{
    Matrix *tempMatrix = new Matrix(*this);                //tempMatrix is a "copy" of the calling obj.
    //Iteration over lower left triangular half of value[][]
    for(int i=0;i<matSize;i++)
    {
        for(int j=0;j<i;j++)
        {
            if((i+j)%2==1)                                //swap condition - rIndex+cIndex = odd                        
                swap(&(tempMatrix->value[i][j]),&(tempMatrix->value[j][i]));
        }
    }
    return tempMatrix;                                    //tempMatrix after transformation is returned
}

/* Unary Operator &() - &A is the product obtained by performing MOD operation on A
   ARGS - void
   RET - long long product
*/
long long Matrix::operator &()
{
    long long product = 1;
    int curTerm;
    for(int i=0;i<matSize;i++)
    {
        if(i%3==0)                    //criteria for taking max
            curTerm = rowMax(i);
        else if(i%3==1)                //criteria for taking min
            curTerm = rowMin(i);
        else                         //criteria for taking (int)avg
            curTerm = rowAvg(i);
        product*=curTerm;
    }
    return product;
}

/* Binary Operator >>() - A >> B is the matrix obtained by performing MRS operation on A and B. Value of 'k'(as in documentation) is obtained inside, from stdin. 
                          Memory is alloc dynamically and address of A >> B is returned. Here, the addition and shifting of MRS is done simultaneously.
   ARGS - Reference to a matrix - Matrix& opnd2
   RET - Pointer to a Matrix - Matrix*
   LOGIC - Let M be a square matrix of size s. index of M[i][j] in row-major form is r = i*s+j. After shifting right by k,new position = i*s+j+k.As shifting is done
              cyclically, r' = (i*s+j+k)%(s^2).Let M[i][j] occur at M'[i'][j']. Now, j' = r' mod s = ((i*s+j+k)mod (s*s))mod s = (j+k) mod s. And i' = r' div s = (i+(j+k)div s)%s. Proof for the last equality:
              Let i*s+j+k = as^2 + b with b < s^2. Also, let b = cs+d with d<s. => c < s. Now, r' div s = b div s = c. Also, j+k = as^2 + b - i*s = as^2 + cs + d - i*s with d<s.
              => (j+k)div s = as + c - i. So, i+(j+k)div s = as + c with c < s. Hence, (i+((j+k)div s))%s = c and the equality is established.
*/
Matrix* Matrix::operator >>(Matrix& opnd2)
{
    int k;                                            //No of positions to shift through
    cin >> k;
    Matrix* tempMatrix = new Matrix(matSize);        //tempMatrix is a "copy" of the calling obj.
    for(int i=0;i<matSize;i++)
    {
        for(int j=0;j<matSize;j++)
        {
            tempMatrix->value[(i+(j+k)/matSize)%matSize][(j+k)%matSize] = this->value[i][j] + opnd2.value[i][j];
        }
    }
    return tempMatrix;                                //Transformed copy is returned by ptr
}

/* Binary Operator +() - A + B is the matrix obtained by performing MADD operation. 'x' is obtained from stdin. if x = 1, result R = A^T + B. otherwise(x=2),R = A + B^T.
                         Memory for result is alloc dynamically. R is returned by ptr.
   ARGS - Reference to a matrix - Matrix& opnd2
   RET - Pointer to a Matrix - Matrix*
*/
Matrix* Matrix::operator +(Matrix& opnd2)
{
    Matrix* resMatrix;
    int x;
    cin >> x;
    if(x==1)                           
    {
        resMatrix = new Matrix(*this);
        resMatrix->transpose();
        *resMatrix += opnd2;
    }
    else
    {
        resMatrix = new  Matrix(opnd2);
        resMatrix->transpose();
        *resMatrix += *this;
    }
    return resMatrix;
}

/*  ~Matrix() - Destroys Matrix objects by de-allocating dynamic memory.
    ARGS - void 
*/
Matrix::~Matrix()
{
    for(int i=0;i<matSize;i++)
       delete[] value[i];
    delete[] value;
}
/* End {Implementation of Matrix} */

/* SpecialMatrixOperations() - Performs the operations SPSUM,MOD,MRS,MADD as per query and displays the result.
   ARGS - Reference to a matrix - Matrix& A and Matrix& B(operands 1 and 2 resp.), int queryCount(no of queries)
   RET - void
*/
void SpecialMatrixOperations(Matrix& A,Matrix& B,int queryCount)
{
    int queryNo;                                    //Iterator over querys
    string query;                                   //Query from stdin. One of SPSUM,MOD,MRS,MADD
    Matrix* resMatrix;                              //Ptr to Matrix obtained by performing query  
    for(queryNo=1;queryNo<=queryCount;queryNo++)
    {
        cin >> query;
        if(query == "SPSUM")                        //SPSUM Op
        {
            resMatrix = +A;
            resMatrix->printRowSum();
            delete resMatrix;
        }
        else if(query == "MOD")                     //MOD Op
        {
            cout << &A << endl;
        }
        else if(query == "MRS")                     //MRS Op
        {
            resMatrix = A >> B;
            resMatrix->printMatrix();
            delete resMatrix;
        }
        else if(query == "MADD")                    //MADD Op
        {
            resMatrix = A + B;
            resMatrix->printMatrix();
            delete resMatrix;
        }
        else                                        //Invalid Op
        {
            exit(-1);
        }
    }
}

int main()
{
    int dimension,queryCount;                       //dimension - size of operand matrices. queryCount - no of operations requested
    cin >> queryCount;
    cin >> dimension;
    Matrix M1(dimension),M2(dimension);             //M1 - opnd1 and M2 - opnd2
    M1.getMatrix();                                 //Input for M1
    M2.getMatrix();                                 //Input for M2
    SpecialMatrixOperations(M1,M2,queryCount);      //Special Matrix Ops are done
    return 0;
}