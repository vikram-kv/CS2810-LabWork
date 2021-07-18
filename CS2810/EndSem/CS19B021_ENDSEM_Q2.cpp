/************************
* $ID$
* File: CS19B021_ENDSEM_Q2.cpp - CS2810 EndSemester Exam Question 2. Code = c2v1
*
* Functionality: 
*   1) Creation of a class Graph to represent simple undirected graphs using adjacency matrix 
*      representation. 
*   2) Given two graphs with the same vertex set : G1(V,E1) and G2(V,E2), the class should also 
*      support the below operations. 
*               a)ADD : G1 + G2 = G(V,E) where E = (E1 U E2)
*               b)MINUS: G1 - G2 = G(V,E) where E = (E1/E2)
*               c)MOD: G1 % G2 = G(V,E) where E = ((E1/E2) U (E2/E1))
*   3) On two graphs G1(V1,E1) and G2(V2,E2), the class will support the operation of DirectProduct(*).
*      We say G1*G2 = G where V(G) = V1 x V2 and two vertices (a,b) and (c,d) in G are connected by an
*      edge iff ac is in E1 and bd is in E2. For this operation, we will print the result acc. to output
*      format without storing it in a Graph object.
*
* Author: K V VIKRAM
*
* Created: [2021-05-03 14:54] by vikram
* 
* Last Modified: [2021-05-04 11:27] by vikram
*
* Formats And Constraints: We will assume the input follows input format and produce output acc. to 
*                          output format as given in the problem statement. We also assume that the 
*                          constraints hold for the given input.
*
* Practice: Array iterators in loops are typically named i or j. On a graph with 'v' vertices, vertices
*           are indexed as 1,2..,v. We use set<pair<size_t,size_t>> to store edge sets as 'set' STL container
*           is ordered in inc. order, which we desire.
*
* Compilation: g++ CS19B021_ENDSEM_Q2.cpp -o solution, ./solution.
*************/

#include <iostream>
#include <vector>
#include <utility>
#include <set>
#include <algorithm>
#define MOD_CONST 1000000007                      //constant defined in problem for generating magic number
using namespace std;
typedef pair<size_t,size_t> edge;                 //edge = pair of vertices(integers)
typedef pair<size_t,size_t> vPair;                //vPair = ordered pair of vertices

/* Begin {Definition of class Graph} */
/*  Graph - defines a class Graph to represent simple undirected graphs with the functionality as in 
            header
    Data Members - vertexCount and adjMatrix
    Private accessor - EdgeSet()
    Private mutator - AddEdge(size_t,size_t)
    Public accessors - PrintEdges()
    Public mutators - InputGraph(size_t)
    Constructor - Graph(size_t)
    Operators - operator +(Graph&), operator -(Graph&), operator %(Graph&) and operator *(Graph&)
*/
class Graph
{
    size_t vertexCount;                     //# of vertices in the graph
    vector<vector<bool>> adjMatrix;         //adjacency matrix of the graph
    void AddEdge(size_t,size_t);            //to add an edge between 2 vertices
    set<edge> EdgeSet();                    //to return the edge set of the graph
public:
    Graph(size_t count);                    //constructor
    void InputEdges(size_t);                //to input all the edges of the graph
    void PrintEdges();                      //to print all the edges of the graph
    Graph operator +(Graph&);               //ADD(+) operator
    Graph operator -(Graph&);               //MINUS(-) operator
    Graph operator %(Graph&);               //MOD(%) operator
    void operator *(Graph&);                //DirectProduct(*) operator
};
/* End {Definition of class Graph} */

/* Begin {Implementation of class Graph} */

/*  AddEdge() - Adds the edge between vertices a and b to the graph
    ARGS - size_t a,size_t b
    RET - void
*/
void Graph::AddEdge(size_t a,size_t b)
{
    adjMatrix[a][b] = true;
    adjMatrix[b][a] = true;
}

/*  EdgeSet() - Returns the edge set of the graph
    ARGS - void
    RET - set<edge> edgeSet
*/
set<edge> Graph::EdgeSet()
{
    set<edge> edgeSet;
    //Loops to consider each edge exactly once by looking at entries above the diagonal
    //of adjMatrix only. Output format is followed as edge ij is stored as (i,j) where
    //i < j
    for(size_t i = 1;i <= vertexCount;++i)
        for(size_t j = i;j <= vertexCount;++j)
            if(adjMatrix[i][j])
                edgeSet.emplace(i,j);
    return edgeSet;
}

/*  Graph() - initializes vertexCount with count, allocates memory for adjMatrix and sets its entries to false
    ARGS - size_t count
    NOTE - adjMatrix will be a (count+1)*(count+1) matrix where the first row and first column is unused. This
    is for ease of access as vertices are numbered starting from 1.
*/
Graph::Graph(size_t count)
{
    vertexCount = count;                        //vertexCount is init to count
    adjMatrix.resize(count+1);                  //adjMatrix is resized for count+1 rows
    //Loop to resize each list adjList[i] (1 <= i <= count) to store count+1 entries. Each
    //entry adjList[i][j] of adjList[i] (1 <= j <= count) is set to false to indicate that the edge
    //set is empty
    for(size_t i = 1;i <= count;++i)
    {
        adjMatrix[i].resize(count+1);
        for(size_t j = 1;j <= count;++j)
            adjMatrix[i][j] = false;
    }
}

/*  InputEdges() - function to get the edges of the graph from STDIN
    ARGS - size_t edgeCount
    RET - void
*/
void Graph::InputEdges(size_t edgeCount)
{
    size_t v1,v2;                           //v1,v2 - vertices
    for(size_t i=1;i<=edgeCount;++i)
    {
        cin >> v1 >> v2;                    //v1 and v2 are fetched and edge v1v2
        AddEdge(v1,v2);                     //is added to the graph
    }
}

/*  PrintEdges() - function to print the edges of a graph acc. to output format
    ARGS - void
    RET - void
*/
void Graph::PrintEdges()
{
    set<edge> edgeSet = EdgeSet();                          //edge set of graph is computed
    for(const auto& itr : edgeSet)                          //edges are printed acc. to output format
        cout << itr.first << ' ' << itr.second << endl;
    if(edgeSet.size() == 0)                                 //if graph has no edges, "GRAPH EMPTY" is printed
        cout << "GRAPH EMPTY" << endl;
    return ;
}

/*  operator +() - for the ADD(+) operation on two graphs with same # of vertices
    ARGS - Graph& G2 : ref to second operand
    RET - Graph G : G = G1 + G2 where G1 = *this = calling object
*/
Graph Graph::operator +(Graph& G2)
{
    Graph resG{this->vertexCount};                              //resG = G is init to have |V(G1)| vertices
    for(size_t i = 1;i <= this->vertexCount;++i)
        for(size_t j = 1;j <= this->vertexCount;++j)
            if(this->adjMatrix[i][j] || G2.adjMatrix[i][j])     //condition - edge ij is in E(G1) or E(G2)
                resG.AddEdge(i,j);
    return resG;                                                //G = G1 + G2 is returned
}

/*  operator -() : for the MINUS(-) operation on two graphs with same # of vertices
    ARGS - Graph& G2 : ref to second operand
    RET - Graph G : G = G1 - G2 where G1 = *this = calling object
*/
Graph Graph::operator -(Graph& G2)
{
    Graph resG{this->vertexCount};                              //resG = G is init to have |V(G1)| vertices
    for(size_t i = 1;i <= this->vertexCount;++i)
        for(size_t j = 1;j <= this->vertexCount;++j)
            if(this->adjMatrix[i][j] && !(G2.adjMatrix[i][j]))  //condition - edge ij is in E(G1) and not in E(G2)
                resG.AddEdge(i,j);
    return resG;                                                //G = G1 - G2 is returned
}

/*  operator %() - for the MOD(%) operation on two graphs with same # of vertices
    ARGS - Graph& G2 : ref to second operand
    RET - Graph G : G = G1 % G2 where G1 = *this = calling object
*/
Graph Graph::operator %(Graph& G2)
{
    Graph resG{this->vertexCount};                              //resG = G is init to have |V(G1)| vertices
    for(size_t i = 1;i <= this->vertexCount;++i)
        for(size_t j = 1;j <= this->vertexCount;++j)
            if(this->adjMatrix[i][j] != (G2.adjMatrix[i][j]))   //condition - edge ij is either in E(G1) or in E(G2)
                resG.AddEdge(i,j);
    return resG;                                                //G = G1 % G2 is returned
}

/*  operator *() - for the DirectProduct(*) operation on two graphs G1 = *this and G2 = arg
    ARGS - Graph& G2 : ref to second operand
    RET - void
    NOTE - We use resultEdgeSet to store edge set of G = G1*G2 for demonstration only. This has
    be commented out as the output of this operation does not need this.
*/
void Graph::operator *(Graph& G2)
{
    set<edge> edgeSet1 = this->EdgeSet();                           //edge set of G1
    set<edge> edgeSet2 = G2.EdgeSet();                              //edge set of G2
    //set<pair<vPair,vPair>> resultEdgeSet;                         //edge set of G = G1*G2
    vector<int> magicNumberList;                                    //magic number list
    //Nested Loops to iterate over edge sets of G1(outer) and G2(inner)
    for(const auto& itr1 : edgeSet1)
        for(const auto& itr2 : edgeSet2)
        {
            //let itr1 = (a,b) and itr2 = (c,d).
            //Stmts for the edge between (a,c) and (b,d) in G
        	//resultEdgeSet.emplace(vPair{itr1.first,itr2.first},vPair{itr1.second,itr2.second});
        	magicNumberList.emplace_back((itr1.first*itr2.first*itr1.second*itr2.second)%MOD_CONST);

            //Stmts for the edge between (a,d) and (b,c) in G
        	//resultEdgeSet.emplace(vPair{itr1.first,itr2.second},vPair{itr1.second,itr2.first});
        	magicNumberList.emplace_back((itr1.first*itr2.second*itr1.second*itr2.first)%MOD_CONST);
    	}
    sort(magicNumberList.begin(),magicNumberList.end());            //sorting magicNumList
    //printing magicNumList
    for(const auto& itr : magicNumberList)
        cout << itr << endl;
}
/* End {Implementation of class Graph} */

int main()
{
    //variables to store input parameters as defined in problem statement
    size_t N, M1;                           //N - # of vertices, M1 - # of edges in G1
    size_t M2, Q;                           //M2 - # of edges in G2, Q - # of queries
    string query;                           //var to store a query

    //Getting N and initializing operand graphs G1,G2
    cin >> N;
    Graph G1{N},G2{N};

    //Getting M1 and edges of G1
    cin >> M1;
    G1.InputEdges(M1);

    //Getting M2 and edges of G2
    cin >> M2;
    G2.InputEdges(M2);

    cin >> Q;                               //Getting Q
    for(size_t i = 1;i <= Q;++i)            //Loop to get and execute queries
    {
        cin >> query;
        if(query == "ADD")
            (G1 + G2).PrintEdges();
        else if(query == "MINUS")
            (G1 - G2).PrintEdges();
        else if(query == "MOD")
            (G1 % G2).PrintEdges();
        else
            G1*G2;
    }
    return 0;
}