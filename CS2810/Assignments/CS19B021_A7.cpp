/************************
* $ID$
* File:  CS19B021_A7.cpp - Use Prim's and Kruskal algorithms to find the minimum spanning tree/forest 
*                           of a given simple,undirected and weighted graph.
*
* Functionality:
*
*1) Class Template Graph:-(Parameter = edge weight type)
*      Stores an undirected weighted simple graph using adjacency lists. Supports the following operations:
*
*       AddV() : Add a new vertex at the end(i.e,its index will be the first unused integer in {0,1,2,...})
*       AddE(a,b,w) : Add an edge from vertex a to vertex b of weight w.
*       DelE(a,b): Delete the edge from vertex a to vertex b.
*
*    Both AddE and DelE will generate appropriate error messages for invalid operations.
*
*2) Class Template MST:-(Parameter = edge weight type = T)
*      Derived from Graph<T>. Has a member tree for storing MST/MSF in adjacency list form. Supports the following operations:
*
*        Prims(v): Computes a minimum weight spanning tree of the graph using prim's algorithm starting at the vertex v 
*                  and stores the. Sum of weights of edges in the MST is printed. If two vertices have same key value, 
*                  the one with lower vertex number is considered first. Will write an error message to the stderr if 
*                  'v' is invalid. Will work correctly for connected graphs only(No Min. Spanning forest will be generated
*                  for disconnected graphs).
*        Kruskal(): Computes a minimum weight spanning forest of the graph using Kruskal's algorithm and stores the tree. 
*                   Sum of weights of all the edges in the minimum spanning forest is printed. While considering the edges 
*                   in increasing order, if two edges (u1,v1) and (u2,v2) have same weight, then (u1, v1) is considered first 
*                   iff u1 < u2 or {u1 = u2 and v1 < v2}.
*        TreeUptodate(): This procedure checks if the minimum spanning tree/forest is up-to-date i.e. check if no edges/vertices 
*                        were added/deleted to the graph after Prims/Kruskal's was run. Prints YES if the tree is up-to-date, 
*                        NO otherwise. If Prims/Kruskal has not been executed once then the tree is considered not up-to-date. 
*                        If the addition/deletion of edge does not affect the structure of MST, then also the tree is considered 
*                        not up-to-date.
*        DisplayTree(): Outputs the edges of the spanning tree in the ascending order of edge weight. Each edge is printed in the 
*                       format u v w such that u < v. If two edges (u1,v1){st u1 < v1} and (u2,v2){st u2 < v2} have same edge 
*                       weights, then (u1,v1) is printed first iff u1<u2 or {u1 = u2 and v1 < v2}. 
*
* Author: K V VIKRAM
*
* Created: [2021-04-05 14:22] by vikram
* 
* Last Modified: [2021-04-06 20:37] by vikram
*
* Input Format:
*     Line 1: integer N denoting number of vertices initially in the graph.
*     Line 2: integer Q, representing the number of queries. 
*     The next Q lines will have one query each which will be one of:
*       ADDV
*       ADDE a b w
*       DELE a b
*       Prims v
*       Kruskal
*       TreeUptodate
*       DisplayTree
*
* Constraints:
*    1 <= No of vertices <= 500
*    a,b,v belong to {0,1,2,...}
*    1 <= Q <= 1000
*    1 <= w <= 5000
*
* Output Format:
*    Prims v: Prints the weight of MST.
*    Kruskal: Prints the weight of MST.
*    TreeUptodate: Prints YES if the tree is up-to-date, NO otherwise.
*    DisplayTree: Prints the edges of the spanning tree/forest in ascending order with ties broken as in functionality.
*                 Each edge (u, v) having weight w is printed in a new line in a format u v w
*
* Assumption: Sufficient dynamic memory is assumed to be available. This exception is neither caught nor handled.
*
* Practice:   Array iterators in loops are typically named i or j. STL container iterators are named itr.
*
* Compilation:  g++ CS19B021_A7.cpp -o solution, ./solution.
*
* Bugs:  No major bugfixes or new releases
*************/

#include <vector>
#include <iostream>
#include <climits>
#include <queue>
#include <list>
#include <utility>
using namespace std;

//    Structure Template Edge(parameter = edge weight type) - Represents an edge.
//  u,v are end-vertices(type = int) of the edge and w is weight of the edge(type = T).
//  Has a default constructor and parametrised constructor
//  We note the valid initialisations will have u,v such that u < v
template <class T>
struct Edge
{
    T w;                                //weight
    int u,v;                            //end vertices
    Edge()                              //default constructor - sets u,v,w to illegal values
    {
        w = T{-1};
        u = v = -1;
    }
    Edge(int a,int b, T weight)         //parametrised Constructor - sets u,v and w to a,b and weight respectively
    {
        u = a;
        v = b;
        w = weight;
    }
};

// Operator template(parameter = edge weight type = T) to define > operator for the type Edge<T>
// We say for two edges a = (u1,v1) of weight w1 and b = (u2,v2) of weight w2, a > b iff
// w1 > w2 or {w1 = w2 and u1 > u2} or {w1 = w2 and u1 = u2 and v1 > v2}. Here, we note that a and
// b are "valid" in the sense that u1 < v1 and u2 < v2.
template <class T>
bool operator>(const Edge<T>& a,const Edge<T>& b)
{
    if(a.w > b.w)
        return true;
    else if(a.w == b.w)
    {
        if(a.u != b.u)
           return a.u > b.u;
        else
            return a.v > b.v;
    }
    else
        return false;
}

// Functor template to use STL priority queue as a min heap. Overloads the function call operator to make it 
// return true iff param1 > param2 where param1 and param2 are of type T
template <class T>
struct GreaterThan
{
    bool operator()(const T& a,const T& b)
    {
        return (a > b);
    }
};

/* Begin {Definition of class DisjointSets} */
/*  DisjointSets :  Used to define a UNION-FIND data structure for Kruskal's algorithm.
    Data Members - parent(int*) and count(int)
    Mutators - Find(int) and Union(int,int)
    Constructor - parametrised: DisjointSets(int)
    Destructor - ~DisjointSets()
    Note: Number of elements = count. Initially, all sets are singletons. Elements are
    numbered from 0 to count - 1. For any element 'i', parent[i] will have the id of i's parent
    in the tree used to represent i's set. The root 'r' of the tree representing a set, will have 
    parent[r] = r. The find algorithm implements path compression for lower avg access time.
*/
class DisjointSets
{
    int* parent;            //ptr to array of integers to store parent of each element
    int count;              //number of elements
public:
    DisjointSets(int);      //constructor. Makes all elements singleton sets
    int Find(int);          //returns the root of arg's tree after path compression
    void Union(int,int);    //merges the sets of arg1 and arg2 if they are different
    ~DisjointSets();        //destructor. deallocates space used by parent array
};
/* End {Definition of class DisjointSets} */

/* Begin {Implementation of class DisjointSets} */
/*    DisjointSets(int) - Sets count(number of elements) to size, allocates necessary space for parent array 
                        and makes each element a singleton set by setting parent[i] = i.
    Args - int size
*/
DisjointSets::DisjointSets(int size)
{
    count = size;
    parent = new int[size];
    for(int i = 0; i < size; ++i)
    {
        parent[i] = i;
    }
}

/*    Find(int) - Finds the representative(root) of num's set and returns it. Performs path compression
                to improve runtime of subsequent finds on num. Path compression is done by updating parent[num]
                to root of num's set.
    Args - int num = element id.
    Ret - int {root of num's set}
    Assumption - 0 <= num <= count - 1 (element id is valid).
*/
int DisjointSets::Find(int num)
{
    if(parent[num] == num)
        return num;
    else
    {
        parent[num] = Find(parent[num]);
        return parent[num];
    }
}

/*    Union(int,int) - Merges the sets of num1 and num2 to a single set if num1 and num2 belong to different sets
                      by setting parent of {root of num2's set} to {root of num1'2 set}
    Args - int num1,int num2 = element id's
    Ret - void
    Assumptions - 0 <= num1,num2 <= count - 1 (element ids are valid).
*/
void DisjointSets::Union(int num1,int num2)
{
    int root1 = Find(num1);
    int root2 = Find(num2);
    if(root1 != root2)
    {
        parent[root2] = root1;
    }
}

/*    ~DisjointSets() - deallocates space used by parent array
*/
DisjointSets::~DisjointSets()
{
    delete[] parent;
}
/* End {Implementation of class DisjointSets} */

/* Begin {Definition of class template Graph} */
/*  class template Graph(parameter = edge weight type = T) - To store a simple, undirected and weighted graph
    Data Members - adjLists(vector<list<pair<T,int>>>) and vertexCount(int)
    Mutator - AddV(), AddE(int,int,T), DelE(int,int)
    Constructor - Parametrised: Graph(int)
    Assumptions - T is well-ordered where <,>,= is well-defined. T has a default constructor and a
    parametrised constructor of the form T(int).
    Note: List is used to store adjacency list because of fast insertions/deletions.
*/
template <class T>
class Graph
{
protected:
    vector<list<pair<T,int>>> adjLists;    //vector of adjacency lists.list node = pair of (adj vertex id and edgeWeight)
    int vertexCount;                       //number of vertices in graph. vertices have ids 0,1...,(vertexCount-1)
public:
    Graph(int);                            //initializes an empty graph with 'arg' vertices
    void AddV();                           //adds a new vertex to the graph
    bool AddE(int,int,T);                  //adds an edge to the graph
    bool DelE(int,int);                    //deletes an edge from the graph
};
/* End {Definition of class template Graph} */

/* Begin {Implementation of class template Graph} */

/*  Graph(int) - for initialization of an empty graph with no of vertices as 'arg'
    Args - int noOfVertices
*/
template <class T>
Graph<T>::Graph(int noOfVertices)
{
    adjLists.clear();
    adjLists.resize(noOfVertices);
    vertexCount = noOfVertices;
}

/*  AddV() - for addition of new vertex with vertex id = first unused id in {0,1,...}
    Args - void
    Ret - void
*/
template <class T>
void Graph<T>::AddV()
{
    ++vertexCount;
    adjLists.resize(vertexCount);
    (adjLists.back()).clear();
}

/*  AddE() - for addition of a new edge of weight w between vertices a and b in the graph. If a or b is not a valid index
    then an error message is put in stderr and the function returns false.Otherwise, the new edge is placed at the end of 
    both a and b's adj lists and the function returns true.
    Args - int a,int b and edgeType w
    Ret - bool 
*/
template <class T>
bool Graph<T>::AddE(int a, int b, T w)
{
    if(a>=vertexCount || b>=vertexCount)                        //if a or b is invalid
    {
        cerr << "Invalid Input. Operation not done" << endl;    //error msg is pushed to stderr
        return false;
    }
    else
    {
        adjLists[a].emplace_back(w,b);                          //stmts to add {w,b} in a's list and vice versa
        adjLists[b].emplace_back(w,a);
        return true;
    }
}

/*  DelE() - deletes the edge between vertices a and b.if 1) a or b is invalid or 2) edge ab does not exist in graph,
             the corresponding error message is put in stderr and the function returns false. Otherwise, the edge ab 
             is deleted and the function returns true.
    Args - int a,int b 
    Ret - bool
*/
template <class T>
bool Graph<T>::DelE(int a, int b)
{
    string invInput = "Invalid Input. Operation not done";                        //error condition 1's msg
    string absEdge = "Edge is not present. Operation not done";                   //error condition 2's msg
    try
    {
        if(a>=vertexCount || b>=vertexCount)                                      //if a or b is invalid, we have error condition 1
            throw invInput;
        else
        {
            bool flag = false;                                                   //indicates whether ab exists or not
            for(auto itr = adjLists[a].begin();itr!=adjLists[a].end();++itr)     //loop to traverse a's adj list
            {
                if(itr->second == b)                                             //if entry of edge ab is found
                {
                    flag = true;                                                 //we set flag to true and delete entry of ab
                    adjLists[a].erase(itr);                                      //and break from loop
                    break;
                }
            }
            if(flag == false)                                                    //if ab doesnt exist, we have error condition 2
                throw absEdge;
            //Stmts to delete ab from b's adj list
            for(auto itr = adjLists[b].begin();itr!=adjLists[b].end();++itr)
            {
                if(itr->second == a)
                {
                    flag = true;
                    adjLists[b].erase(itr);
                    break;
                }
            }
            return true;                                                        //ret true as ab existed and was deleted
        }
    }
    catch(string errMsg)                                                        //catch mechanism to send crct err msg to stderr
    {
        cerr << errMsg << endl;
        return false;                                                           //ret false as operation was invalid
    }
}
/* End {Implementation of class template Graph} */

/* Begin {Definition of class template MST} */
/*  class template MST(parameter = edge weight type = T) - To store the MST/MSF of a graph in adj list representation
    Inheritance - Inherited protectedly from the class Graph<T>
    Inherited data members - protected - adjLists, vertexCount
    Inherited member functions - protected - Graph<T>::{AddV(), AddE(int,int,T) and DelE(int,int)}
    Non-inherited members:-
        Data Members - tree(vector<list<pair<T,int>>>) and updateStatus(bool)
        Private Mutator - AddTreeEdge(int,int,T)
        Mutator - AddV(), AddE(int,int,T), DelE(int,int), Prims(int) and Kruskal()
        Accessor - TreeUptoDate(), DisplayTree()
        Constructor - Parametrised: MST(int)
    Assumptions - T is well-ordered where <,>,= is well-defined. T has a default constructor and a
    parametrised constructor of the form T(int).cout has been overloaded to work on edgeType type.
    Note: List is used to store adjacency list because of fast insertions/deletions.
*/
template <class T>
class MST : protected Graph<T>
{
    vector<list<pair<T,int>>> tree;    //vector of adjacency lists of MST/MSF.list node = pair of (adj vertex id and edgeWeight)
    bool updateStatus;                 //updateStatus = true iff MST/MSF is up-to-date
    void AddTreeEdge(int,int,T);       //adds an edge of weight arg3 between vertices arg1 and arg2 to the MST/MSF
public:
    MST(int x):Graph<T>{x}             //constructor to call Graph<T>{int} with argument x and set updateStatus to false
    {updateStatus = false;}
    void AddV();                       //performs Graph<T>::AddV() and sets updateStatus to false
    void AddE(int,int,T);              //performs Graph<T>::AddE(int,int,w) and changes updateStatus appropriately
    void DelE(int,int);                //performs Graph<T>::DelE(int,int) and changes updateStatus appropriately
    void Prims(int v);                 //performs Prims(v) operation as in header
    void Kruskal();                    //performs Kruskal() operation as in header
    void TreeUptodate();               //performs TreeUptodate() operation as in header
    void DisplayTree();                //performs DisplayTree() operation as in header
};
/* End {Definition of class template MST} */

/* Begin {Implementation of class template MST} */

/*  AddTreeEdge() - for addition of edge of weight w between vertices a and b in MST/MSF
    Args - int a,int b, T w
    Ret - void
*/
template <class T>
void MST<T>::AddTreeEdge(int a,int b,T w)
{
    tree[a].emplace_back(w,b);               //stmts to add {w,b} at end of a's adj list in tree and vice versa
    tree[b].emplace_back(w,a);
}

/*  AddV() - calls Graph<T>::AddV() and sets updateStatus to false
    Args - void
    Ret - void
*/
template <class T>
void MST<T>::AddV()
{
    Graph<T>::AddV();
    updateStatus = false;                   //MST is not up-to-date after addition of a new vertex
}

/*  AddE() - calls Graph<T>::AddE(a,b,w) and sets updateStatus to false if the call returns true
    Args - int a,int b, T w
    Ret - void
*/
template <class T>
void MST<T>::AddE(int a,int b,T w)
{
    if(Graph<T>::AddE(a,b,w)==true)        //if addition of edge ab of weight w to the graph is successful,
        updateStatus = false;              //the MST/MSF is not up-to-date
}

/*  DelE() - calls Graph<T>::DelE(a,b) and sets updateStatus to false if the call returns true
    Args - int a,int b
    Ret - void
*/
template <class T>
void MST<T>::DelE(int a,int b)
{
    if(Graph<T>::DelE(a,b)==true)        //if deletion of edge ab from the graph is successful,
        updateStatus = false;            //the MST/MSF is not up-to-date
}

/*  Prims() - Performs Prims(v) operation as in header and sets updateStatus to true
    Args - int v
    Ret - void
    Algorithm - Prim's Greedy Minimum Spanning Tree algorithm using minheap data structure.
    Note - MST is generated for connected graphs. MSF is not generated for disconnected graphs. So, it does
           not "work" for disconnected graphs
*/
template <class T>
void MST<T>::Prims(int v)
{
    //stmts to check v's validity and terminate after displaying error msg if v is invalid
    if(v >= this->vertexCount)
    {
        cerr << "Invalid Vertex Number... Query Not Executed." << endl;
        return ;
    }
    tree.clear();                                                //MST is cleared
    tree.resize(this->vertexCount);                              //MST is resized to store vertexCount many adj lists

    //bool array 'processed' of size vertexCount with all entries initialized to false
    //processed[i] will be set to true after vertex i is added to partially constructed MST
    vector<bool> processed(this->vertexCount,false);

    //array to store the value of edge of least weight connecting a vertex to the
    //partially constructed MST. Initially, all entries are set to T{INT_MAX}(No connection)
    vector<T> minConnectEdge(this->vertexCount,T{INT_MAX});

    //minConnectVertex[i] = vertex u if the edge ui is the edge corresponding to the weight minConnectEdge(i) for i != v
    //Initially, all entries are set to -1(illegal vertex id)
    vector<int> minConnectVertex(this->vertexCount,-1);
    processed[v] = true;                                        //v is added to the MST first
    typedef pair<T,int> qPair;                                  //qPair = type of elements to be inserted in minheap pq defined below
    //pq is a minheap of qPairs which will be used for picking the edge to be added to partial MST
    priority_queue<qPair,vector<qPair>,greater<qPair>> pq;
    qPair curPair;                                              //current qPair under consideration in while loop
    int curVertex;                                              //= curPair.second
    T MSTWeight = T{0};                                         //weight of all edges in MST. Initialized to T{0}
    for(const auto& itr : this->adjLists[v])                    //Loop to traverse v's adjList
    {
        //For a neighbour u of v, minConnectEdge[u] = weight of edge uv and minConnectVertex[u] = v
        //After this, the qPair (weight of uv,u) is pushed into the minheap pq
        minConnectEdge[itr.second] = itr.first;
        minConnectVertex[itr.second] = v;
        pq.emplace(itr.first,itr.second);
    }
    while(!pq.empty())                                           //till pq becomes empty
    {
        curPair = pq.top();                                      //curPair corr. to the "least" qPair
        pq.pop();                                                //pq's root(=curPair) is deleted
        curVertex = curPair.second;                              //curVertex is set
        if(!processed[curVertex])                                //if curVertex has not been already added to partial MST
        {
            //curVertex is added to the partial MST using the edge "(minConnectVertex[curVertex],curVertex)" of weight
            //"curPair.first" and MSTWeight is updated
            processed[curVertex] = true;
            AddTreeEdge(minConnectVertex[curVertex],curVertex,curPair.first);
            MSTWeight += curPair.first;
            //Stmts to traverse curVertex's adjList and check whether a connection from curVertex to one of its
            //neighbours(say u) is cheaper than minConnectEdge[u]. If this is the case, we update the entries corr.
            //to u in minConnectEdge[] and minConnectVertex, and then we push the qPair "(weight of edge joining 
            //curVertex and u,u)" into pq
            for(const auto& itr : this->adjLists[curVertex])
            {
                if(itr.first < minConnectEdge[itr.second])
                {
                    pq.emplace(itr.first,itr.second);
                    minConnectEdge[itr.second] = itr.first;
                    minConnectVertex[itr.second] = curVertex;
                }
            }
        }
    }
    cout << MSTWeight << endl;                                  //weight of MST is printed
    updateStatus = true;                                        //tree is now up-to-date
}

/*  Kruskal() - Performs Kruskal operation as in header and sets updateStatus to true
    Args - void
    Ret - void
    Algorithm - Kruskal's Greedy Minimum Spanning Forest algorithm using Union-Find and minheap data structure.
*/
template <class T>
void MST<T>::Kruskal()
{
    tree.clear();                                           //MSF is cleared
    tree.resize(this->vertexCount);                         //MSF is resized to store vertexCount many adj lists
    //edgeQueue is a min heap of Edge<T> objects
    priority_queue<Edge<T>,vector<Edge<T>>,GreaterThan<Edge<T>>> edgeQueue;
    Edge<T> curEdge;                                        //current edge under process in loop
    int v1,v2;                                              //end-vertices of current edge
    T MSTWeight = T{0};                                     //Weight of all edges in MSF. Initialized to T{0}
    //vertexGroups is a collection of disjoint sets of vertices of the graph
    //Initially, all sets are singletons(per constructor defn)
    DisjointSets vertexGroups{this->vertexCount};
    //Loop to push all edges in the graph to edgeQueue
    for(int i = 0; i < this->vertexCount; ++i)              //Loop to traverse all vertices in graph
    {
        for(const auto& itr : this->adjLists[i])            //Loop to traverse the adj list of vertex i in graph
        {
            if(i < itr.second)                              //Check to ensure each edge is inserted one time only
                edgeQueue.emplace(i,itr.second,itr.first);
        }
    }
    //Loop to pop the "least" edge from edgeQueue, add it to the MSF if its end-vertices are in two different
    //disjoint sets in vertexGroups till edgeQueue becomes empty.
    //We note that if the picked edge connects two different component-trees of partially constructed MSF,
    //then it wont form a cycle and will instead connect two small component-trees to form a larger component-tree
    while(!edgeQueue.empty())
    {
        curEdge = edgeQueue.top();                          //curEdge is set
        edgeQueue.pop();
        v1 = curEdge.u;                                     //v1 and v2 are set
        v2 = curEdge.v;
        if(vertexGroups.Find(v1)!=vertexGroups.Find(v2))    //if v1 and v2 belong to different component-trees
        {
            //stmts to merge the sets corr. to v1 and v2, add curEdge to MSF and update MSTWeight
            vertexGroups.Union(v1,v2);
            AddTreeEdge(v1,v2,curEdge.w);
            MSTWeight += curEdge.w;
        }
    }
    cout << MSTWeight << endl;                              //weight of MSF is printed
    updateStatus = true;                                    //tree is now up-to-date
}

/*  TreeUptodate() - Prints YES if MST/MSF is up-to-date. Otherwise, prints NO.
    Args - void
    Ret - void
*/
template <class T>
void MST<T>::TreeUptodate()
{
    string dispMsg = updateStatus ? "YES":"NO";
    cout << dispMsg << endl;
}

/*  DisplayTree() - Performs DisplayTree() operation as in header
    Args - void
    Ret - void
*/
template <class T>
void MST<T>::DisplayTree()
{
    //edgeSet is a minheap of edges.
    priority_queue<Edge<T>,vector<Edge<T>>,GreaterThan<Edge<T>>> edgeSet;
    for(int i = 0; i < Graph<T>::vertexCount; ++i)            //Loop to traverse all vertices in MST/MSF
    {
        for(const auto& itr : tree[i])                        //Loop to traverse the adj list of vertex i in MST/MSF
        {
            if(i < itr.second)                                //Check to ensure each edge is considered one time only
            {
                edgeSet.emplace(i,itr.second,itr.first);      //As i < itr.second, the valid edge (i,itr.second) of 
            }                                                 //weight itr.first is pushed into edgeSet
        }
    }
    Edge<T> curEdge;                                          //var to store the current edge under process in while loop
    //Loop to remove the "least" edge from edgeSet and display the edge in the format u v w in a newline
    //till edgeSet becomes empty
    while(!edgeSet.empty())
    {
        curEdge = edgeSet.top();
        edgeSet.pop();
        cout << curEdge.u << " " << curEdge.v << " " << curEdge.w << endl;
    }
}
/* End {Implementation of class template MST} */

int main()
{
    int initialVertexNumber;                //initial number of vertices in graph
    size_t queryCount;                      //number of queries
    string query;                           //var to store a query
    int u,v,w;                              //vars to store arguments of a query. u,v for vertices and w for edge weight
    cin >> initialVertexNumber;            
    MST<int> G{initialVertexNumber};        //MST G of class MST<int> is initialized with underlying graph having "initialVertexNumber" many vertices
    cin >> queryCount;
    for(size_t i = 0;i<queryCount;++i)      //Loop to perform all the input queries
    {
        cin >> query;
        if(query == "ADDE")                 //AddE(a,b,w) operation
        {
            cin >> u >> v >> w;
            G.AddE(u,v,w);
        }
        else if(query == "DELE")            //DelE(a,b) operation
        {
            cin >> u >> v;
            G.DelE(u,v);
        }
        else if(query == "ADDV")            //AddV() operation
            G.AddV();
        else if(query == "Prims")           //Prims(v) operation
        {
            cin >> u;
            G.Prims(u);
        }
        else if(query == "TreeUptodate")    //TreeUptodate() operation
            G.TreeUptodate();
        else if(query == "DisplayTree")     //DisplayTree() operation
            G.DisplayTree();
        else                                //Kruskal() operation
            G.Kruskal();
    }
    return 0;
}