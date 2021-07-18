/************************
* $ID$
* File:  CS19B021_A6.cpp - Creation of a class template for representing undirected weighted graphs with limited functionality.
*
* Purpose: A miniaturized class template called graph to represent undirected weighted graph is implemented with edge type as the only parameter.
*          Graph is stored using adjacency list representation. The implemented functions are AddV() to add a new vertex with id = first unused id
*          starting from 0; AddE(a,b,w) to add an edge of weight w between vertices a and b; DelE(a,b) to delete the edge between a and b; BFS(a) to
*          perform BFS on the graph starting with a using a queue and print the BFS order of 'a'-reachable vertices; SSP(a) to print the length of 
*          the shortest path from a to all vertices. In BFS, all the vertices adjacent to the current vertex will be considered in asc. order according to 
*          the weights of the edge joining them to the current vertex(if two adjacent vertices are connected to the current vertex by equal weight 
*          edges, then the vertex with the lower id will be considered first). The SSP algorithm assumes edge weights are non-negative and will display 
*          the value of edgeType{INT_MAX} for unreachable vertices. Also, in AddE() and delE(), appropriate error messages are generated and operation 
*          is aborted if operation is illegal such as access to a non-existent vertex id and deletion of edge which is not present.
*
* Author: K V VIKRAM
*
* Created: [2021-03-22 13:27] by vikram
* 
* Last Modified: [2021-03-23 15:12] by vikram
*
* Input Format:
*   The first line of the input contains an integer denoting number of vertices initially in the graph.The next line has Q.
*   The next Q lines will have any queries as following:
*       ADDV
*       ADDE a b w
*       DELE a b
*   Next 2 lines will have any queries from the following:
*       BFS a
*       SSP b
*    Multiple edges are disallowed. ADDE and DELE between same two vertices are "appropriately" given in input.
*
* Constraints:
*   The number of vertices in the graph <= 500
*   a and b are in {0,1,2,3...}
*   1 <= w <= 1000
*
* Output Format:
*   Only BFS(a) and SSP(a) will generate outputs acc. to their descriptions in purpose.
*
* Assumption: Sufficient dynamic memory is assumed to be available. This exception is neither caught nor handled.
*
* Practice:   Array iterators in loops are typically named i or j. STL container iterators are named itr.
*
* Compilation:  g++ CS19B021_A6.cpp -o solution, ./solution.
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

/* Begin {Definition of class template Graph} */
/*  Graph<edgeType> - 
    Data Members - adjList - vector of list(an adjacency list) of pair<int,edgeType>(an edge) and  vertexCount
    Private Mutator - InsList(int,pair<int,edgeType>), DelList(int,int)
    Accessor - BFS(int), SSP(int)
    MUtator - AddV(), AddE(int,int,edgeType), DelE(int,int)
    Constructor - Default: Graph() and parametrised: Graph(int)
    Assumptions - edgeType is well-ordered where <,>,= is well-defined. edgeType  has a default constructor and a
    parametrised constructor of the form edgeType(int).cout has been overloaded to work on edgeType type.
*/
template <class edgeType>
class Graph
{
private:
    vector<list<pair<int,edgeType>>> adjLists;                //vector of adjacency lists.adjacency list entry = pair of (adj vertex id and edgeWeight)
    int vertexCount;                                        //number of vertices in graph. vertices have ids 0,1...,(vertexCount-1)
    void InsList(int,pair<int,edgeType>);                    //to insert a new entry in a vertex's adjacency list
    bool DelList(int,int);                                    //to delete an entry in a vertex's adjacency list. returns true if deletion is done, else returns false
public:
    Graph();                                                //initializes a graph with 0 vertices
    Graph(int);                                                //initializes an empty graph with 'arg' vertices
    void AddV();                                            //adds a new vertex to the graph
    void AddE(int,int,edgeType);                            //adds an edge to the graph
    void DelE(int,int);                                        //deletes an edge from the graph
    void BFS(int);                                            //performs BFS(a) as in documentation
    void SSP(int);                                            //performs SSP(a) as in documentation
};
/* End {Definition of class template Graph} */

/* Begin {Implementation of class template Graph} */

/*  Graph() - for initialization with no of vertices as 0
    ARGS - void
*/
template <class edgeType>
Graph<edgeType>::Graph()
{
    adjLists.clear();                                                //no entries in adjLists
    vertexCount = 0;                                                //no of vertices in graph is 0
}

/*  Graph(int) - for initialization with no of vertices as 'arg'
    ARGS - int noOfVertices
*/
template <class edgeType>
Graph<edgeType>::Graph(int noOfVertices)
{
    adjLists.resize(noOfVertices);                                    //adjLists will have 'arg' many entries
    for(auto itr = adjLists.begin();itr!=adjLists.end();++itr)
    {
        (*itr).clear();                                                //clearing each list in adjLists
    }
    vertexCount = noOfVertices;                                        //updating vertexCount
}

/*  AddV() - for addition of new vertex with vertex id = first unused id in {0,1,...}
    ARGS - void
    RET - void
*/
template <class edgeType>
void Graph<edgeType>::AddV()
{
    ++vertexCount;                                                //vertexCount update
    adjLists.resize(vertexCount);                                //space for new vertex's(id = old vertexCount) adj list.
    (adjLists.back()).clear();                                    //adj list of new vertex is emptied
}

/*  InsList() - for addition of a new entry p in vertex v's adjacency list. insertion of p is done such that
                the list is sorted acc. to incident edge weight. If two vertices are joined to v by equal weight edges, then
                the entries will be sorted acc. vertex id.
    ARGS - int v, pair<int,edgeType> p
    RET - void
*/
template <class edgeType>
void Graph<edgeType>::InsList(int v,pair<int,edgeType> p)
{
    auto itr = adjLists[v].begin();                                            //itr for v's adj list
    for(;itr!=adjLists[v].end();++itr)                                        //loop to traverse v's adj list to find the ins position of p
    {
        if((*itr).second < p.second)                                        //continue if cur entry's weight < p's weight..... c1
            continue;
        else if((*itr).second == p.second && (*itr).first < p.first)        //continue if cur entry's weight = p's weight but cur entry's vertex
            continue;                                                        //id < p's vertex id...... c2
        else
            break;                                                            //here, both c1 and c2 are false. hence, itr is crct ins pos for p
    }
    adjLists[v].insert(itr,p);                                                //p is inserted in v's adj list at position of itr
    return;
}

/*  AddE() - for addition of a new edge of weight w between vertices a and b in the graph. puts error message in stderr if a
             or b is not a valid index and aborts AddE operation.
    ARGS - int a,int b and edgeType w
    RET - void
*/
template <class edgeType>
void Graph<edgeType>::AddE(int a, int b, edgeType w)
{
    if(a>=vertexCount || b>=vertexCount)                            //if a or b is invalid
    {
        cerr << "Invalid Input. Operation not done" << endl;        //error msg is pushed to stderr
        return ;                                                    //abort operation
    }
    else
    {
        InsList(a,pair<int,edgeType>{b,w});                            //stmts to add {b,w} in a's list and vice versa
        InsList(b,pair<int,edgeType>{a,w});
    }
}

/*  DelList() - deletes the entry for the edge ab from a's adj list if it is present and returns true. if edge ab is absent,
                returns false
    ARGS - int a,int b 
    RET - bool
*/
template <class edgeType>
bool Graph<edgeType>::DelList(int a, int b)
{
    for(auto itr = adjLists[a].begin();itr != adjLists[a].end();++itr)            //loop to traverse a's adj list
    {
        if((*itr).first == b)                                                    //if entry of edge ab is found
        {
            adjLists[a].erase(itr);                                                //ab's entry is deleted and returns true
            return true;
        }
    }
    return false;                                                                //if ab is not an edge, return false
}

/*  DelE() - deletes the edge between vertices a and b. puts appropriate error msg in std err if 1) a or b is invalid or 2) edge 
             ab does not exist in graph
    ARGS - int a,int b 
    RET - void
*/
template <class edgeType>
void Graph<edgeType>::DelE(int a, int b)
{
    if(a>=vertexCount || b>=vertexCount)                                    //if a or b is invalid
    {
        cerr << "Invalid Input. Operation not done" << endl;                //error msg is put in stderr
        return ;                                                            //abort operation
    }
    else
    {
        if(DelList(a,b) == false)                                            //remove entry of edge ab from a's adj list if present
        {                                                                    //if DelList(a,b) returns false, we have error condition 2
            cerr << "Edge is not present. Operation not done" << endl;
            return ;                                                        //abort operation
        }
        DelList(b,a);                                                        //remove entry of edge ab from b's adj list(here, edge ab is 
        return ;                                                            //definitely present)
    }
}

/*  BFS() - performs BFS(arg) as in documentation using STL queue. We note that each adj list in adjLists is sorted acc. to edge weight
            by implementation of InsList().
    ARGS - int v
    RET - void
*/
template <class edgeType>
void Graph<edgeType>::BFS(int v)
{
    bool* visited;                                                //array for maintaining visitation detail of all vertices during BFS.
    visited = new bool[vertexCount];                            //visted[i] = true iff vertex i has already been processed through BFSqueue
    queue<int> BFSQueue;                                        //queue of vertices for BFS
    vector<int> BFSOrder;                                        //vector for storing BFS order of v-reachable vertices
    int curVertex;                                                //curVertex = vertex under consideration in loop.
    for(int i=0;i<vertexCount;i++)                                //loop to set all vertices as unvisited
    {
        visited[i] = false;
    }
    BFSOrder.clear();
    BFSQueue.push(v);                                            //v is visited and v is pushed into the queue
    visited[v] = true;
    while(!BFSQueue.empty())
    {
        curVertex = BFSQueue.front();                            //curVertex = vertex in front of queue
        BFSOrder.push_back(curVertex);                            //curVertex is put in end of BFSorder and is popped from BFSqueue
        BFSQueue.pop();
        auto itr = adjLists[curVertex].begin();                    //iterator itr for curVertex's adj list
        while(itr!=adjLists[curVertex].end())                    //loop to traverse curVertex's adj list
        {
            if(visited[(*itr).first] == false)                    //if the vertex at itr has not been visited, we push it into queue and make it 
            {                                                    //visited
                BFSQueue.push((*itr).first);
                visited[(*itr).first] = true;
            }
            ++itr;                                                //itr update
        }
    }
    for(auto itr = BFSOrder.begin();itr<BFSOrder.end();++itr)    //loop to print BFS order of v-reachable vertices
    {
        cout << *itr << ' ';
    }
    cout << endl;
    delete[] visited;                                            //space allocated for visited array is freed
    return;
}

// Functor template for the priority queue used in SSP(). Defines greater than relation for pair<int,edgeType>. 
// The priority queue obtained using this functor will be a minheap acc. to edgeType field of pair<int,edgeType>.
template <class edgeType>
struct GreaterThan
{
    bool operator()(pair<int,edgeType>& pair1,pair<int,edgeType>& pair2)
    {
        return (pair1.second > pair2.second);                //pair1 > pair2 iff pair1's 2nd field > pair2's 2nd field
    }
};

/*  SSP() - performs SSP(arg) as in documentation using STL priority_queue. All edge weights must be non-negative.
    ARGS - int v
    RET - void
*/
template <class edgeType>
void Graph<edgeType>::SSP(int v)
{
    edgeType* splArr = new edgeType[vertexCount];                                                //array for storing shortest path lengths of all vertices
    bool* processed = new bool[vertexCount];                                                    //processed[i] = true iff vertex i is fully processed(its SPL is final)
    priority_queue<pair<int,edgeType>, vector<pair<int,edgeType>>, GreaterThan<edgeType>> pq;    
    //pq = priority queue(minheap) of pair<int,edgeType> for choosing next vertex(the vertex i with the smallest splArr[i])
    pair<int,edgeType> curPair;                                                                    //current pair under consideration in loop
    int curVertex;                                                                                //curPair.first = curPair's vertex
    edgeType curVertexSP;                                                                        //curPair.second = curVertex's final SPL from v
    for(int i = 0;i<vertexCount;++i)                                                            //loop to set all distances in splArr to infinity and make all 
    {                                                                                            //vertices unprocessed
        splArr[i] = edgeType{INT_MAX};
        processed[i] = false;
    }
    splArr[v] = edgeType{0};                                                                    //SPL of v from v is 0
    pq.push(pair<int,edgeType> {v,edgeType{0}});                                                //(v,0) is pushed into pq
    while(!pq.empty())                                                                            //while pq is not empty
    {
        curPair = pq.top();                                                                        //curPair is found = pair with min 2nd entry in pq
        pq.pop();                                                                                //curPair is deleted from pq
        curVertex = curPair.first;                                                                //curVertex is found
        if(processed[curVertex]==true)                                                            //if curVertex has already been processed, we ignore curPair
            continue;
        processed[curVertex] = true;                                                            //set curVertex as processed and set curVertexSP
        curVertexSP = curPair.second;                                                            //to curPair.second
        auto itr = adjLists[curVertex].begin();                                                    //iterator itr for curVertex's adj list
        while(itr!=adjLists[curVertex].end())                                                    //loop to traverse curVertex's adj list
        {
            if(curVertexSP+(*itr).second < splArr[(*itr).first])                                //if SPL(curVertex) + weight of (curVertex,itr vertex)
            {                                                                                    //is less than SPL(itr vertex), we update SPL(itr vertex)
                splArr[(*itr).first] = curVertexSP+(*itr).second;                                //and push a redundant pair(itr vertex,itr vertex's new SPL)
                pq.push(pair<int,edgeType>{(*itr).first,splArr[(*itr).first]});                    //into pq
            }
            ++itr;                                                                                //loop update
        }
    }
    for(int i=0;i<vertexCount;++i)                                                                //loop to print SPL of all vertices
    {
        cout << splArr[i] << ' ';
    }
    cout << endl;
    delete[] splArr;                                                                            //stmts to free all dynamic memory allocated
    delete[] processed;
    return;
}
/* End {Implementation of class template Graph} */

int main()
{
    int initialVertexNumber;                            //initial number of vertices in graph
    size_t queryCount;                                    //number of queries of the types:ADDV,ADDE,DELE
    string query;                                        //var to store a query
    int vertex1,vertex2,edgeWeight;                        //vars to store arguments of a query
    size_t loopCtr;                                        //loop control variable
    cin >> initialVertexNumber;            
    Graph<int> G{initialVertexNumber};                    //graph G of class Graph<int> is initialized with "initialVertexNumber" many vertices
    cin >> queryCount;
    loopCtr = 0;
    while(loopCtr<queryCount)                            //Loop to perform all ADDE(a,b,w), DELE(a,b) and ADDV queries
    {
        cin >> query;
        if(query == "ADDE")
        {
            cin >> vertex1 >> vertex2 >> edgeWeight;
            G.AddE(vertex1,vertex2,edgeWeight);
        }
        else if(query == "DELE")
        {
            cin >> vertex1 >> vertex2;
            G.DelE(vertex1,vertex2);
        }
        else if(query == "ADDV")
            G.AddV();
        else
        {
            cerr << "Invalid Query. Terminating!" << endl;
            exit(-1);
        }
        ++loopCtr;
    }
    loopCtr = 0;
    while(loopCtr<2)                                //Loop to perform 2 queries of the type: BFS(a) or SSP(a)
    {
        cin >> query;
        if(query == "SSP")
        {
            cin >> vertex1;
            G.SSP(vertex1);
        }
        else if(query == "BFS")
        {
            cin >> vertex1;
            G.BFS(vertex1);
        }
        else
        {
            cerr << "Invalid Query. Terminating!" << endl;
            exit(-1);
        }
        ++loopCtr;
    }
    return 0;
}

