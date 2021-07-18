/************************
* $ID$
* File: CS19B021_A8.cpp - Find the edit distance between two strings using dynamic programming.The edit distance 
*                          between two strings is the minimum number of letter insertions, letter deletions, 
*                          and letter substitutions required to transform one string into the other.
*
* Purpose: Implement the DP-based algorithm given in Jeff's book for finding the edit distance between two strings. Defines
*           a class called EDD that stores two strings s1 and s2 and an integer matrix. EDD's member functions are
*              i) assign(a,b): Sets s1 = a and s2 = b.
*             ii) compdist(): Computes the edit distance between the two strings stored in the class. Throws an exception 
*                             if the strings are not initialized (i.e., no prior call to assign is made). The algorithm for 
*                             computing edit distance is taken from Jeff's book.
*
*            iii) dist(): Returns the edit distance between the two strings. If not available, updates the matrix variable 
*                         inside the class by calling compdist() and then returns the edit distance. Throws an exception 
*                         if the strings are not initialized.
*
*             iv) table(): Prints the table if available. If not available, runs compdist() and then outputs the table. 
*                          Throws an exception if the strings are not initialized.
*
* Reference Text: Algorithms by Jeff Erickson
*
* Author: K V VIKRAM
*
* Created: [2021-04-12 17:47] by vikram
* 
* Last Modified: [2021-04-13 20:25] by vikram
*
* Input Format:
* Line 1 : number of queries, q.
* Next lines: 'q' strings from the set = {assign, compdist, dist, table} each in a newline
* Every assign query will be followed by two strings s1 and s2.
*
* Constraints:
* 0 < No. of queries <= 10^5
* 0 < length of s1, s2 <= 100
*
* Output Format:
* For dist() query - edit distance is printed in a new line.
* For table() query - memoized matrix as in Jeff's Book is printed.(rows are separated by a newline and elements in a row are separated by ' ')
* For any query, if strings are not found, an exception "strings not found!" is thrown to stderr.
* 
* Assumption: Sufficient dynamic memory is assumed to be available. This exception is neither caught nor handled.
*
* Practice:   Array iterators in loops are typically named i or j. Specifically, for matrices , i is used for rowIndex and j for colIndex.
*
* Compilation:  g++ CS19B021_A8.cpp -o solution, ./solution.
*
* Bugs:     No major bugfixes or new releases
*************/

#include <iostream>
using namespace std;

/* Begin {Definition of class EDD} */
/*  EDD - Used to store two strings and compute the editDistance between the strings by DP. Can also print the
          edit distances between all prefixs of the two strings in a table format(This table is needed to achieve
          memoization).
    Data Members - strings s1 and s2, bools valid and status, const int maxSize,int editDistance and int** matrix
    Member Functions - assign(string,string),compdist(), dist() and table()
    Constructor - parametrised: EDD(size_t)
    Destructor - ~EDD()
*/
class EDD
{
    string s1,s2;                           //strings between which edit distance is required
    bool valid;                             //flag to indicate if a valid assign() call was made or not
    const size_t maxSize;                   //max allowed size of s1 and s2
    int** matrix;                           //ptr to 2D matrix to store edit distances between prefixes of s1 and s2 
    bool status;                            //status = true iff editDistance is up-to-date
    int editDistance;                       //var to store edit distance of s1 and s2
public:
    EDD(size_t);                            //constructor to initialize all var and allocate mem for matrix
    void assign(string,string);             //for assign() operation
    void compdist();                        //for compdist() operation
    int dist();                             //for dist() operation
    void table();                           //for table() operation
    ~EDD();                                 //de-alloc mem of matrix
};
/* End {Definition of class EDD} */

/* Begin {Implementation of class EDD} */

/*  EDD() - Initializes all variables and allocates space for matrix var
    Args - size_t size
*/
EDD::EDD(size_t size):maxSize{size}            //maxSize = value of arg
{
    s1 = s2 = "";                              //s1 and s2 are empty strings initially
    status = valid = false;                    //cur s1 and s2 are not valid and the status is false
    //stmts to allocate space for a 2D integer array of dimns [maxSize+1][maxSize+1] referenced by matrixs
    matrix = new int*[maxSize+1];
    for(size_t i = 0;i<=maxSize;++i)
        matrix[i] = new int[maxSize+1];
    //stmts for initialisation of entries of matrix which corr. to base case of recursive definition of edit distance
    //of two strings,i.e, edit[i][0] = i(As i deletions are needed to transform a given i-length string to an empty one)
    //and edit[0][j] = j(As j insertions are needed to transform an empty string to a given j-length string)
    for(size_t j = 0;j<=maxSize;++j)
    {
        matrix[0][j] = j;
        matrix[j][0] = j;
    }
    editDistance = -1;                         //illegal assignment of editDistance
}

/*  assign() - sets s1 to a and s2 to b if their sizes are <= maxSize. Otherwise, s1 and s2 are set to ""
               as the arguments are too long. Sets valid and status variables appropriately in both the cases.
    Args - string a, string b
    Ret - void 
*/
void EDD::assign(string a,string b)
{
    //stmts to set s1 and s2 to a and b resp. and set status to false as compdist() has not been run yet
    s1 = a;
    s2 = b;
    status = false;
    if(s1.size()>maxSize || s2.size()>maxSize)        //if s1 or s2 has length > maxSize
    {
        s1 = s2 = "";
        valid = false;                                //s1 and s2 are invalid
        return ;
    }
    valid = true;                                     //here, s1 and s2 are valid
}

/*  compdist() - computes the edit distance between s1 and s2 and stores it in editDistance.The matrix(memoization table)
                 is also updated in the process
    Args - void
    Ret - void
    Algorithm - Let m = s1.size and n = s2.size.For any i and j such that 0 <= i <= m and 0 <= j <= n, matrix[i][j] is the
                edit distance between i-length prefix of s1 and j-length prefix of s2. Clearly, matrix[i][0] = i as 'i' deletions
                are needed for the transformation and similarly, matrix[0][j] = j as 'j' insertions are needed for the
                transformation. For finding matrix[i][j],in general, we note that if the last operation was
                    i) insertion - insVal = matrix[i][j-1] + 1
                    ii) deletion - delVal = matrix[i-1][j] + 1
                    iii) substitution - repVal = matrix[i-1][j-1] if s1[i] = s2[j](free substitution). Otherwise, 
                                        repVal = matrix[i-1][j-1] + 1
                For optimality, matrix[i][j] = min{insVal,delVal,repVal}. We note that row-wise evaluation ensures that all the
                reqd values for finding matrix[i][j] are already computed and memoized.
*/
void EDD::compdist()
{
    try
    {
        if(valid == false)                                  //exception criteria
            throw("strings not found!");
        size_t size1 = s1.length();                         //size1,size2 = size of s1 and s2 resp.
        size_t size2 = s2.length();
        int insVal,delVal,repVal;                           //edit distances corr. to insertion,deletion and replacement resp.
        //nested loops for row after row evaluation to fill all entries in matrix[1..size1][1..size2]
        for(size_t i = 1;i<=size1;++i)
        {
            for(size_t j = 1;j<=size2;++j)
            {
                insVal = matrix[i][j-1] + 1;                //insVal computation
                delVal = matrix[i-1][j] + 1;                //delVal computation
                //repVal computation
                if(s1[i-1] == s2[j-1])
                    repVal = matrix[i-1][j-1];
                else
                    repVal = matrix[i-1][j-1] + 1;
                //stmts for setting matrix[i][j] to min{insVal,delVal,repVal}
                if(insVal <= delVal && insVal <= repVal)
                    matrix[i][j] = insVal;
                else if(delVal <= repVal)
                    matrix[i][j] = delVal;
                else
                    matrix[i][j] = repVal;
            }
        }
        editDistance = matrix[size1][size2];                //for s1 -> s2 transform, edit dist = matrix[size1][size2]
        status = true;                                      //as editDistance is up-to-date
    }
    catch(const char* str)                                  //catch block for const char* exception 
    {
        cerr << str << endl;
    }
    return ;
}

/*  dist() - returns edit distance of s1 and s2 if available(If a compdist() call for the current s1 and s2 was 
             already made).Otherwise, calls compdist() and then returns edit distance of s1 and s2
    Args - void
    Ret - int
*/
int EDD::dist()
{
    try
    {
        if(valid == false)                            //exception criteria
            throw("strings not found!");
        if(status == false)                           //if edit distance is not available for current s1 and s2
            compdist();
        return editDistance;                          //edit distance is returned
    }
    catch(const char* str)                            //catch block for const char* exception 
    {
        cerr << str << endl;
        return -1;                                    //illegal value to indicate that an exception was thrown
    }
}


/*  table() - displays the edit distance table in a the format given in header if the table is available(If a compdist()
              call for the current s1 and s2 was already made).Otherwise, calls compdist() and prints the table. 
    Args - void
    Ret - void 
    Exception - If valid = false (If the preceding assign() call was invalid or no assign() call was made), an exception 
                "strings not found!" is thrown to stderr.
*/
void EDD::table()
{
    try
    {
        if(valid == false)                            //exception criteria
            throw("strings not found!");
        if(status == false)                           //if table is not available for current s1 and s2
            compdist();
        size_t size1 = s1.length();                   //size1,size2 = size of s1 and s2 resp.
        size_t size2 = s2.length();
        //Nested loops to print reqd. entries of matrix,i.e, matrix[1..size1][1..size2]
        for(size_t i = 1;i<=size1;++i)
        {
            for(size_t j = 1;j<=size2;++j)
                cout << matrix[i][j] << ' ';
            cout << endl;
        }
    }
    catch(const char* str)                            //catch block for const char* exception
    {
        cerr << str << endl;
    }
}

/*  ~EDD() - Destroys EDD objects by de-allocating dynamic memory alloc to matrix(int**)
    ARGS - void 
*/
EDD::~EDD()
{
    for(size_t i = 0;i<=maxSize;++i)
        delete[] matrix[i];
    delete[] matrix;
}
/* End {Implementation of class EDD} */

int main()
{
    size_t queryCount;                              //number of queries
    string query;                                   //var to store a query
    string str1,str2;                               //var to store input strings for assign() query
    int editDist;                                   //var to store edit distance returned by dist() query
    EDD eddObj{100};                                //obj of EDD for performing the queries
    cin >> queryCount;                        
    for(size_t i = 0;i<queryCount;++i)              //Loop to perform all input queries
    {
        cin >> query;
        if(query == "assign")                       //assign() operation
        {
            cin >> str1 >> str2;
            eddObj.assign(str1,str2);
        }
        else if(query == "compdist")                //compdist() operation
            eddObj.compdist();
        else if(query == "dist")                    //dist() operation
        {
            editDist = eddObj.dist();
            if(editDist != -1)                      //if editDist = -1, an exception was thrown by dist() call
                cout << editDist << endl;
        }
        else
            eddObj.table();                         //table() operation
    }
    return 0;
}
