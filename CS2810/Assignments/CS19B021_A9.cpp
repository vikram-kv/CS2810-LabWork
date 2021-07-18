/************************
* $ID$
* File: CS19B021_A9.cpp - Use a modified version of Floyd-Warshall algorithm to find the max value of 
*                         a target currency obtainable from a given amount of source currency in one or
*                         several steps. In each steps, currency of a country of color a can only be 
*                         converted to currency of a country of color b iff {a=b or color-a countries
*                         have a treaty with color-b countries}
*
* Purpose: Our goal is to convert some X units of a source currency into a target currency such that 
* the total value in the target currency is maximized. We are allowed to go through multiple intermediate 
* currencies. A few rules restrict currency exchanges. They are explained below.
*
*   Colours: There are a total of C colours denoted c[0], c[1], ..., c[C-1] and we have an undirected 
*   and unweighted graph T on those C colours representing which colour currencies can be legally 
*   exchanged with which ones. Formally, a currency of colour c[i] can be exchanged with a colour 
*   of currency c[j] iff c[i] and c[j] are neighbours in T.
*   
*   Currencies: There are a total of N currencies denoted n[0], n[1], ..., n[N-1]. 
*   Each currency n[i] is associated with a colour nc[i] that is an integer in the range 0 to C-1. 
*   We have an exchange rate table represented in the form of a directed and weighted complete graph G 
*   on those N currencies. Suppose we wish to change S units of money from currency n[a] to currency n[b]. 
*   Let c[i] and c[j] be their respective colours. Since the graph G is complete, there will be an edge 
*   from n[a] to n[b] of weight, say w. Then, S units of money in currency n[a] can be converted to Sw 
*   units of money in currency n[b] provided that c[i] and c[j] are neighbours in T. Otherwise, we are 
*   not allowed to convert from n[a] to n[b] directly. Note that we may still be able to convert indirectly 
*   through other intermediate currencies. Of course, each intermediate conversion must satisfy the colour 
*   conversion graph T.
*
* Author: K V VIKRAM
*
* Created: [2021-04-19 17:27] by vikram
* 
* Last Modified: [2021-04-20 11:34] by vikram
*
* Input Format:
*   ->First line - C, the number of colors.
*   ->Second line - m, the number of undirected edges in colors graph.
*   ->The next m lines - two space-separated integers each, indicating edge between those colors.
*   ->The next line - N, the number of currencies.
*   ->The following line - N integer values, nc[0], nc[1], ..., nc[N - 1], where nc[i] indicates 
*     color associated with currency i.
*   ->Next N lines should each contain N floating point values, indicating rows of adjacency matrix 
*     for complete graph T of currencies.
*   ->The next line - a positive integer q, the number of queries.
*   ->Next q lines - three integers, s, t, and X, where s is index of starting currency, t is index of 
*     target currency, and X is the amount of units of starting currency that must be converted into the 
*     target currency.
*
* Constraints:
*   1 <= C <= 15
*   C <= N <= 100
*   1 <= q <= 10000
*   0 <= s,t < N
*
* Output Format:
*   q lines, each containing the answer to the corresponding query.
*
*   Answer for a particular query will be one of the following:
*       -1, if conversion is not possible through any means, following the treaty.
*       a floating point value(upto 3 decimals), showing, maximum number of units of target currency.
*       INF, if there is a path from s to t through an increasing weight cycle.
*
* Assumption: Sufficient dynamic memory is assumed to be available. This exception is neither caught nor handled.
*
* Practice: Array iterators in loops are typically named i or j. Specifically, for matrices , i is used for rowIndex and j for colIndex.
*
* Compilation: g++ CS19B021_A9.cpp -o solution, ./solution.
*
* Bugs: No major bugfixes or new releases
*************/

#include <iostream>
#include <iomanip>
using namespace std;

/*Begin {Definition of class ExchangeComputer} */
/*  ExchangeComputer - stores a color graph, currency graph, currency-color assignment array, uses FW algorithm
                       to compute max exchange rate of pairs of currencies and interprets FW's result to print
                       the result of queries of the format given in header
    Data Members - colorGraph,colorCount,currencyGraph,currencyCount,colorArray,colorEdgeCount,flag
    Private Member function - ValidExchange(int,int)
    Public Member Functions - GetInputs(),QueryExecutor(size_t)
    Constructor - ExchangeComputer()
    Destructor - ~ExchangeComputer()
*/
class ExchangeComputer
{
    bool** colorGraph;                                          //2D matrix used to store color graph
    size_t colorCount;                                          //no of colors
    float** currencyGraph;                                      //2D matrix used to store currency graph
    size_t currencyCount;                                       //no of countries/currencies
    int* colorArray;                                            //array to store curr-color assignment
    size_t colorEdgeCount;                                      //no of edges in color graph
    bool flag;                                                  //flag to prevent multiple calls of GetInputs()
    //ValidExchange returns 1 if curr a can be directly exchanged for curr b
    bool ValidExchange(int a,int b)                             
    {
        return (colorGraph[colorArray[a]][colorArray[b]]);
    }
public:
    ExchangeComputer();                                         //constructor to init all data members
    void GetInputs();                                           //func to get inputs
    void QueryExecutor(size_t);                                 //func to execute queries
    ~ExchangeComputer();                                        //destructor to de-alloc dynamic mem
};
/*End {Definition of class ExchangeComputer} */

/*Begin {Implementation of class ExchangeComputer} */
/*  ExchangeComputer() - initializes all variables 
    Args - void
*/
ExchangeComputer::ExchangeComputer()
{
    //to init all ptrs to nullptr
    colorGraph = nullptr;
    currencyGraph = nullptr;
    colorArray = nullptr;
    //to init all counters to 0
    colorCount = 0;
    currencyCount = 0;
    //to init flag to false
    flag = false;
}

/*  GetInputs() - Obtains input from stdin. Input must strictly follow input format in header. Input
                  can be given only once. GetInputs() will terminate after displaying error msg from
                  2nd call onwards
    Args - void
*/
void ExchangeComputer::GetInputs()
{
    //if flag = true, input has already been obtained
    if(flag)
    {
        cerr << "Already Got Inputs" << endl;
        return;
    }

    cin >> colorCount;                                      //number of colors
    cin >> colorEdgeCount;                                  //number of edges in color graph
    //stmts to alloc memory for color graph's 2D adj matrix
    //the graph is undirected,simple and unweighted
    //all off-diagonal entries are set to false
    //all diagonal entries are set to true(countries of the same color can do currency exchange)
    colorGraph = new bool*[colorCount];
    for(size_t i=0;i<colorCount;++i)
    {
        colorGraph[i] = new bool[colorCount];
        for(size_t j=0;j<colorCount;++j)
            colorGraph[i][j] = false;
        colorGraph[i][i] = true;
    }

    int a,b;                                                //a,b - colors(vertices of color graph)
    //Loop to get all edges in color graph
    for(size_t i = 0;i<colorEdgeCount;++i)
    {
        cin >> a >> b;
        colorGraph[a][b] = true;
        colorGraph[b][a] = true;
    }

    cin >> currencyCount;                                   //number of currencies(countries)
    colorArray = new int[currencyCount];                    //alloc mem to colorArray
    for(size_t i=0;i<currencyCount;++i)                     //Loop to get colorArray
        cin >> colorArray[i];

    //Stmts to alloc memory for currency graph's 2D adj matrix 
    //The graph is directed and complete, hence it's entries are
    //also initialized
    currencyGraph = new float*[currencyCount];
    for(size_t i=0;i<currencyCount;++i)
    {
        currencyGraph[i] = new float[currencyCount];
        for(size_t j=0;j<currencyCount;++j)
            cin >> currencyGraph[i][j];
    }

    flag = true;                                            //flag = true as input can be given only once
}

/*  QueryExecutor() - Uses a modified version of Floyd-Warshall algorithm to compute the max exchange rate 
                      of any currency to another currency, store the results in a matrix. Then, for each of
                      "count" many queries, the values of the matrix are interpreted to print one of the
                      three possible outputs as given in output format. At each step of exchange,the "color" 
                      criteria is followed.
    Args - size_t count
    Algorithm - Let number of vertices in given graph be N. The vertices are numbered from 0 to N-1.
                Let matrix(i,j,k) = the max product of weights of i->j using only the vertices numbered
                from 0 to k-1. Now, our modified Floyd-Warshall Algorithm is described by the recursive 
                formula given as
                    matrix(i,j,k) = max(matrix(i,j,k),matrix(i,k,k-1)*matrix(k,j,k-1)) for 1 < k < N
                    matrix(i,i,0) = 1.0
                    matrix(i,j,0) = w(edge(i,j)) for i != j
                Termination occurs after iteration for k = N-1 is done. We note that if matrix(i,i,N-1) is > 1
                ,then matrix[i][i] = 1 was replaced at some stage by a num > 1 indicating a cycle with prod
                of edges > 1 is reachable from/passes through i.
*/
void ExchangeComputer::QueryExecutor(size_t count)
{
    float** matrix;                                                     //used to store 2D matrix of the FW algo
    //stmts to alloc mem to and init entries of matrix
    matrix = new float*[currencyCount];
    for(size_t i=0;i<currencyCount;++i)
    {
        matrix[i] = new float[currencyCount];
        for(size_t j=0;j<currencyCount;++j)
        {
            //if i->j is allowed, matrix[i][j] = exchange rate of(i->j). Otherwise, matrix[i][j] = 0
            matrix[i][j] = ValidExchange(i,j)?currencyGraph[i][j]:0.0;
        }
    }
    float temp;                                                         //temporary var
    //Stmts to implement FW algo
    for(size_t k=0;k<currencyCount;++k)
    {
        for(size_t i=0;i<currencyCount;++i)
        {
            for(size_t j=0;j<currencyCount;++j)
            {
                temp = matrix[i][k]*matrix[k][j];
                if(temp > matrix[i][j])                                 //if exchgRate(i->k)*exchgRate(k->j) {=temp} >
                    matrix[i][j] = temp;                                //exchgRate(i->j), we update exchgRate(i->j) to temp
            }
        }
    }
    size_t s,d;                                                         //s = src curr and d = dest curr                              
    float X;                                                            //X = value of src curr to be converted
    //flag to indicate if a heavy cycle with prod of edges > 1.0 has been found on some s->d path
    bool heavyCyclePath = false;
    for(size_t q=0;q<count;++q)
    {
        cin >> s >> d >> X;                                             //s,d and X are fetched
        if(matrix[s][d]==0)                                             //matrix[s][d] is 0.0 iff no s->d path exists
            cout << -1 << endl;
        else
        {   //Here, we can say that s->d conversion is possible
            heavyCyclePath = false;                                         //init of heavyCyclePath
            //Loop to scan the diagonal
            //In iteration i, we check whether a i->i heavy cycle exists(cond = matrix[i][i]>1.0)
            //and for each such cycle, we check whether a s->i path exists(cond = matrix[s][i]>0.0)
            //and whether i->d path exists(cond = matrix[i][d]>0.0). If all 3 conditions hold,
            //we can use the loop i->i innumerably many times and reach d to get infinite money of d
            //provided X>0
            for(size_t i=0;i<currencyCount;++i)
                if(matrix[i][i]>1.0 && matrix[s][i]!=0 && matrix[i][d]!=0)
                {
                    heavyCyclePath = true;
                    break;
                }
            if(heavyCyclePath == true && X>0.0)                             //INF money condition
                cout << "INF" << endl;
            else                                                            //finite money condition
                cout << fixed << setprecision(3) << matrix[s][d]*X << endl; //dest money = exchgRate(s->d)*X
        }
    }

    //stmts to de-alloc mem of matrix
    for(size_t i = 0;i<currencyCount;++i)
        delete[] matrix[i];
    delete[] matrix;
}

/*  ~ExchangeComputer() - de-allocates all dynamic mem used by data members
    Args - void
*/
ExchangeComputer::~ExchangeComputer()
{
    //stmts to de-alloc mem of color graph's adj matrix
    for(size_t i = 0;i<colorCount;++i)
        delete[] colorGraph[i];
    delete[] colorGraph;

    //stmts to de-alloc mem of currency graph's adj matrix
    for(size_t i = 0;i<currencyCount;++i)
        delete[] currencyGraph[i];
    delete[] currencyGraph;

    //stmt to de-alloc mem of color array
    delete[] colorArray;
}
/*End {Implementation of class ExchangeComputer} */

int main()
{
    size_t queryCount;                          //number of queries
    ExchangeComputer myObj;                     //myObj = obj of class ExchangeComputer
    myObj.GetInputs();                          //inputs to myObj
    cin >> queryCount;                          //queryCount is got from stdin
    myObj.QueryExecutor(queryCount);            //queryCount queries are executed
    return 0;
}