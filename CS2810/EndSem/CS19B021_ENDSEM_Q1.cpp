/************************
* $ID$
* File: CS19B021_ENDSEM_Q1.cpp - CS2810 EndSemester Exam Question 1. Code = c1v1
*
* Functionality: Given a simple undirected graph, we determine cardinality of largest possible set of vertices 
*                such that no two vertices in the set are directly connected to each other.
*
* Algorithm Used:
*   Given a vertex v from a graph G, we define N(v) = {v} U {u in V(G) | (u,v) is in E(G)}. Then the required 
*   cardinality is given by FUN(G) defined below.
*       FUN(G):
*           if(|V(G)|==0):
*               return 0
*           else:
*               v := a vertex of min degree in G
*               return (1 + [Max over {y in N(v)} of (FUN(G/N(y)))])
*
* Author: K V VIKRAM
*
* Created: [2021-05-03 16:43] by vikram
* 
* Last Modified: [2021-05-04 09:54] by vikram
*
* Formats And Constraints: We will assume the input follows input format and produce output acc. to 
*                          output format as given in the problem statement. We also assume that the 
*     	                   constraints hold for the given input.
*
* Practice: Array iterators in loops are typically named i or j. On a graph with 'v' vertices, vertices
*           are indexed as 0,1..,(v-1).
*
* Compilation: g++ CS19B021_ENDSEM_Q1.cpp -o solution, ./solution.
*************/

#include <iostream>
#include <vector>
#include <climits>
using namespace std;

/* Begin {Definition of class Graph} */
/*  Graph - defines a class Graph to represent simple undirected graphs. Has the functionality to
            delete N(v) from the graph for some vertex v and return the resulting graph
    Data Members - vertexCount, initVertexCount, adjMatrix, validArray
    Accessors - RemoveNeighbours(size_t)
    Mutators - InputEdges(size_t)
    Constructor - Graph(size_t)
    friend function - FUN(Graph)
*/
class Graph
{
    size_t vertexCount;                                 //current # of vertices in the graph
    size_t initVertexCount;                             //initial # of vertices in the graph(before any deletions)
    vector<vector<bool>> adjMatrix;                     //adjacency matrix of the graph
    vector<bool> validArray;                            //validArray[i] = true iff vertex i exists in the graph
public:
    Graph(size_t);                                      //constructor
    void InputEdges(size_t);                            //to input all the edges of the graph
    Graph RemoveNeighbours(size_t) const;               //return the graph formed after deleting N(arg) from the graph
    friend size_t FUN(Graph);                           //friend function to implement the FUN() algo on this class
};
/* End {Definition of class Graph} */

/* Begin {Implementation of class Graph} */
/*  Graph() - Initializes all data members
    ARGS - size_t count
*/    
Graph::Graph(size_t count)
{
    //initially all vertices are valid, so vertexCount=initVertexCount=count
    vertexCount = initVertexCount = count;
    adjMatrix.resize(count);                    //adjMatrix is resized for count rows
    validArray.resize(count);                   //validArray is resized for count entries
    for(size_t i = 0;i < count;++i)
    {
        adjMatrix[i].resize(count);             //The row adjMatrix[i] is resized for count entries
        validArray[i] = true;                   //All vertices are valid here
        for(size_t j = 0;j < count;++j)
        {
            adjMatrix[i][j] = false;            //All entries of adjMatrix are false to indicate |E(G)| = {}
        }
    }
}

/*  InputEdges() - function to get the edges of the graph from STDIN
    ARGS - size_t edgeCount
    RET - void
*/
void Graph::InputEdges(size_t edgeCount)
{
    size_t a,b;                                             //a,b - vertices
    for(size_t i=1;i<=edgeCount;++i)
    {
        cin >> a >> b;                                      //a,b are fetched
        adjMatrix[a][b] = adjMatrix[b][a] = true;           //ab edge is added to graph
    }
}

/*  RemoveNeighbours() - function to return the graph formed after deleting N(v) from the graph G = *this. 
                         N(v) is as defined in header. 
    ARGS - size_t v
    RET - Graph
    NOTE - We delete a vertex v by invalidating the vertex. More precisely, vertex 'i' is considered
    to be deleted iff validArray[i] = false.
*/
Graph Graph::RemoveNeighbours(size_t v) const
{
    Graph result = *this;                           //result = copy of G(calling object)
    size_t neighbourCount = 0;                      //# of neighbours of v
    //Loop over all indices from 0 to (initVertexCount-1)    
    for(size_t i=0;i<initVertexCount;++i)
    {
        if(validArray[i] && adjMatrix[v][i])        //if vertex 'i' is in G and vi is an edge in G
        {
            result.validArray[i] = false;           //invalidate i and inc neighbourCount by 1
            ++neighbourCount;
        }
    }
    result.validArray[v] = false;                   //invalidate v
    result.vertexCount -= neighbourCount + 1;       //vertexCount update after deleting N(v)
    return result;                                  //G/N(v) is returned
}
/* End {Implementation of class Graph} */

/*  FUN() - function to implement the FUN() algorithm given in header
    ARGS - Graph G
    RET - size_t
    Attributes - Recursive function. Friend of Graph class
*/
size_t FUN(Graph G)
{
	if(G.vertexCount == 0)                                       //termination condition - |V(G)| = 0
		return 0;                                                //if G has 0 vertices, FUN(G) = 0
	else
	{
        //stmts for finding a vertex with minimum degree in G
		size_t minDegree = INT_MAX;                                             //init to INT_MAX as we wish to minimize degree
		size_t minDegVertex;                                                    //vertex with degree = minDegree
        size_t curDegree;                                                       //temp var to store degree of current vertex in loop
        //Loop over all indices from 0 to (initVertexCount-1)
		for(size_t i=0;i<G.initVertexCount;++i)               
		{
			if(!G.validArray[i])                                                //if i is not a vertex in G, we skip to next iteration
                continue;
            else
			{
                curDegree = 0;
                //Loop over all indices from 0 to (initVertexCount-1)
                for(size_t j=0;j<G.initVertexCount;++j)
                    if(G.validArray[j] && G.adjMatrix[i][j])                    //if ij is an edge in G, inc curDegree by 1
                        ++curDegree;
                if(curDegree < minDegree)
                {
                    minDegree = curDegree;                                      //minDegree update
                    minDegVertex = i;                                           //minDegVertex update
                }
			}
		}

        //let minDegVertex = v
        //stmts for finding Max over {y in N(v)} of (FUN(G/N(y)))
        size_t maxFunVal = 0;                                                   //maxFunVal is init to min possible value FUN() can take(=0)
        size_t curFunVal;                                                       //temp var to store current FUN() value in loop
        //Loop over all indices from 0 to (initVertexCount-1)
        for(size_t i=0;i<G.initVertexCount;++i)
        {
            //condition for {i in N(v)} - i must be in G and ((i and v are adjacent) or i = v)
            if(G.validArray[i] && (G.adjMatrix[minDegVertex][i] || i == minDegVertex))
            {
                curFunVal = FUN(G.RemoveNeighbours(i));                         //compute FUN(G/N(i))
                maxFunVal = (curFunVal > maxFunVal)?curFunVal:maxFunVal;        //maxFunVal update
            }
        }
        return (1+maxFunVal);
	}
}

int main()
{
	size_t m,n;                            //m - # of cities(vertices), n - # of roads(edges)
	cin >> m >> n;
	Graph CityNetwork(m);                  //CityNetwork graph is init with m cities and 0 roads
	CityNetwork.InputEdges(n);             //CityNetwork graph's edges are obtained
	cout << FUN(CityNetwork);              //FUN(CityNetwork) is displayed
    return 0;
}