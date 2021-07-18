/************************
* $ID$
* File:  CS19B021_A3.cpp - Defines classes for an integer stack, undirected graph. Performs a few operations on each class.
*
* Purpose: Creation of class Stack for an integer stack, class Graph for an undirected, simple and unweighted graph. stack is realized as a linked list.
*          Implements stack operations of push,pop,top,isEmpty.Implements graph using adjacency matrix. performs graph operations of adding/removing an edge.
*          Using recursive DFS, detects presence of cycle in graph.Using stack based DFS, finds no of components in graph and if a path exists between two given vertices,
*
* Author: K V VIKRAM
*
* Created: [2021-02-22 12:32] by vikram
* 
* Last Modified: [2021-02-22 21:14] by vikram
*
* Input Format:
* First line contains Q, the number of operations to be performed on the stack. Each of the next Q lines represents a single operation of the format:
* push x -> Push(x)
* pop -> Pop()
* top -> Top()
* empty -> isEmpty()
* Next line of the input contains an integer n denoting the number of vertices in the graph. Next line of the input contains an integer m denoting the number of graph operations to be performed. 
* Each of the next m lines represents a single graph operation of the format:
* add u v -> G += (u, v)
* del u v -> G -= (u, v)
* cycle -> detectCycle()
* components -> components()
* reach u v -> reach(u, v)
*
* Constraints:
* 1 <= n <= 100
* 0 <= u <= n-1
* 0 <= v <= n-1
* 0 <= x <= INT_MAX {(2^31) - 1}
* 0 <= Q <= 100
* 0 <= m <= 100
*
* Output Format:
* pop : Print the returned element on new line
* top : Print the returned element on new line
* empty : Print the returned element on new line
* cycle : Print the returned element on new line
* components : Print the returned element on new line
* reach : Print the returned element on new line 
*
* Assumption: Sufficient dynamic memory is assumed to be available.This exception is neither caught nor handled.
*
* Practice:   Array iterators in loops are typically named i or j. Specifically, for matrices, i is used for rowIndex and j for colIndex.
*
* Compilation:  g++ CS19B021_A3.cpp -o solution, ./solution.
*
* Bugs:     No major bugfixes or new releases
*************/

#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>
using namespace std;

/* Begin {Definition of StackNode} */
/*
    StackNode - Node of a linked list of integers. Here, it will be used for a stack
    Data Members - data(int),next(StackNode*)
    Constructor - StackNode(int value) - sets data = value and next = nullptr
*/
struct StackNode
{
    int data;                            //value of node
    StackNode* next;                    //ptr to next node in linked list
    StackNode(int value)
    {
        data = value;
        next = nullptr;
    }
};
/* End {Definition of StackNode} */

/* Begin {Definition of Stack} */
/*  Stack - ADT stack of integers with operations of push,pop,top and isEmpty.
    Data Members - head(StackNode*)
    Mutator- Push(int),Pop()
    Accessor- Top(),isEmpty()
    Constructor - Stack()
    Destructor - ~Stack()
*/
class Stack
{
    StackNode* head;                        //ptr to topmost node of the stack
public:
    Stack();                                //initializes to empty stack
    void Push(int);                            //Push Op
    int Top();                                //Peek Op
    int Pop();                                //Pop Op
    int isEmpty();                            //checks if stack is empty
    ~Stack();                                //deletes all stack nodes
};
/* End {Definition of Stack} */


/* Begin {Implementation of Stack} */

/*  Stack() - Sets head to nullptr, indicating an empty stack
    ARGS - void
*/
Stack::Stack()
{
    head = nullptr;
}

/*  Push() - Inserts a new node at the top of the stack
    ARGS - int x
    RET - void
*/
void Stack::Push(int x)
{
    StackNode* newNode = new StackNode(x);
    newNode->next = head;
    head = newNode;
}

/*  Top() - Returns the value at the topmost node of the stack if stack is not empty.Otherwise, returns -1
    ARGS - void
    RET - int
*/
int Stack::Top()
{
    if(head == nullptr)
        return -1;
    else
        return (head->data);
}

/*  Pop() - Removes the topmost node from the stack and returns its value if stack is not empty.Otherwise, returns -1
    ARGS - void
    RET - int
*/
int Stack::Pop()
{
    if(head == nullptr)
        return -1;
    else
    {
        int retVal = head->data;
        StackNode* temp = head;
        head = head->next;
        delete temp;
        return retVal;
    }
}

/*  isEmpty() - Checks if stack is empty. Returns 1 if so, else returns 0
    ARGS - void
    RET - int
*/
int Stack::isEmpty()
{
    if(head==nullptr)
        return 1;
    else
        return 0;
}

/*  ~Stack() - Deletes all the nodes in the stack 
    ARGS - void 
*/
Stack::~Stack()
{
    StackNode* temp = head;
    while(head!=nullptr)
    {
        temp = head;
        head = head->next;
        delete temp;
    }
}

/* End {Implementation of Stack} */

/* Begin {Definition of Graph} */
/*  Graph - ADT to store an undirected graph in adjacency matrix representation with adjMatrix[i][j] = 1 if edge (i,j) is in graph and otherwise, 0.
    Data Members - vertexCount(int), adjMatrix(int**)
    Mutator- operators - +=(std::pair<int,int>) and -=(std::pair<int,int>)
    Accessor- private member functions - cycleFindHelper(int,int,bool*),compAndReachHelper(Stack&,bool*)
              public member functions - detectCycle(),components(),reach(int,int)
    Constructor - Graph(int)
    Destructor - ~Graph()
*/
class Graph
{
    int vertexCount;                                        //number of vertices in graph
    int** adjMatrix;                                        //double ptr to store adjacency matrix
    bool cycleFindHelper(int,int,bool*);                    //helper function for detectCycle()
    void compAndReachHelper(Stack&,bool*);                    //helper function for components(),reach()
public:
    Graph(int n);                                            //initialises vertexCount and allocates memory for adjMatrix
    void operator+=(std::pair<int,int>);                    //adds an edge to the graph
    void operator-=(std::pair<int,int>);                    //deletes an edge from the graph
    int detectCycle();                                        //detects presence of a cycle in graph
    int components();                                        //finds no of connected components in graph
    int reach(int,int);                                        //finds out whether a path exists between two given vertices
    ~Graph();                                                //deletes allocated memory of adjMatrix
};
/* End {Definition of Graph} */

/* Begin {Implementation of Graph} */

/*  cycleFindHelper() - recursive function to help detect cycles in a single component. cycle exists in a component if there is 
                        a back edge from the current vertex to an already seen vertex. 
    ARGS - curVertex(int) - current vertex, preVertex(int) - parent of current vertex in DFS tree, seen(bool*) - seen[i] = 1 if i was already seen
    RET - bool
*/
bool Graph::cycleFindHelper(int curVertex,int preVertex,bool* seen)
{
    seen[curVertex] = true;                                                        //visit current vertex
    for(int i=0;i<vertexCount;i++)                                                //to iterate over all vertices
    {
        if(adjMatrix[curVertex][i] == 1)                                        //for vertices adj. to current vertex
        {
            if(seen[i] == true && i != preVertex)                                //if vertex i is already seen and is not the parent of current vertex
                return true;                                                    // a cycle has been found. control breaks
            else if(seen[i] == false && cycleFindHelper(i,curVertex,seen))        //if vertex i is unseen, it is visited next(DFS) and if a cycle is found in the call for vertex i
                return true;                                                    //then too, return 1 as a cycle exists
        }
    }
    return false;                                                                //as no cycle has been found in this call
}

/*  compAndReachHelper() - function to perform DFS(visitation happens in reverse order from vertexCount .. 1) using stack of vertices. returns control when stack becomes empty
                           ,i.e, when all vertices in the same connected component as initial S.top() have been visited.
    ARGS - S(Stack&), seen(bool*)
    RET - void
*/
void Graph::compAndReachHelper(Stack& S,bool* seen)
{
    while(!S.isEmpty())                                                //if there are vertices to be visited in DFS tree rooted at initial S.top()
    {
        int vertex = S.Pop();                                        //the most recently seen node is popped
        if(seen[vertex]==false)                                        //if vertex is not already seen. this can occur as we are using stack, wherein the same vertex may have been pushed multiple times
        {
            seen[vertex] = true;                                    //visit vertex
            for(int j=0;j<vertexCount;j++)
            {
                if(adjMatrix[vertex][j]==1&&seen[j]==false)            //for unseen vertices j adjacent to vertex
                {    
                    S.Push(j);                                        //j is pushed
                }
            }
        }
    }
}

/*  Graph() - initializes vertexCount with n, allocates memory for adjMatrix and sets its entries to 0
    ARGS - int n
*/
Graph::Graph(int n)
{
    vertexCount = n;
    adjMatrix = new int*[vertexCount];
    for(int i=0;i<vertexCount;i++)
    {
        adjMatrix[i] = new int[vertexCount];
        for(int j=0;j<vertexCount;j++)
            adjMatrix[i][j] = 0;                        
    }
}

/*  operator +=() - adds the edge given by vertexPair to the graph
    ARGS - std::pair<int,int> vertexPair
    RET - void
*/
void Graph::operator+=(std::pair<int,int> vertexPair)
{
    int u = vertexPair.first,v = vertexPair.second;
    adjMatrix[u][v] = 1;
    adjMatrix[v][u] = 1;
}

/*  operator -=() - removes the edge given by vertexPair from the graph
    ARGS - std::pair<int,int> vertexPair
    RET - void
*/
void Graph::operator-=(std::pair<int,int> vertexPair)
{
    int u = vertexPair.first,v = vertexPair.second;
    adjMatrix[u][v] = 0;
    adjMatrix[v][u] = 0;
}

/*  detectCycle() - detects presence of cycle by DFS using cycleFindHelper() iover all components. returns 1 if cycle is found. else, returns 0
    ARGS - void
    RET - int
*/
int Graph::detectCycle()
{
    bool seen[vertexCount];                                            //seen[i] is true if i has already been seen during recursion. else, it is false
    for(int i=0;i<vertexCount;i++)
        seen[i] = false;
    for(int i=0;i<vertexCount;i++)
    {
        if(seen[i] == false && cycleFindHelper(i,-1,seen))            //since i is in a new component and a root of dfs tree, its parent is set to be -1(illegal)
            return 1;                                                //if a cycle has been found in i's component
    }
    return 0;                                                        //here, no cycle exists
}

/*  components() - returns the number of connected components in the graph using compAndReachHelper
    ARGS - void
    RET - int
*/
int Graph::components()
{
    bool seen[vertexCount];                                            //seen[i] is true if i has already been seen. else, it is false
    int count = 0;                                                    //no of components
    Stack DFSstack;                                                    //stack of vertices for performing dfs
    for(int i=0;i<vertexCount;i++)
        seen[i] = false;
    for(int i=0;i<vertexCount;i++)
    {
        if(seen[i] == false)                                        //if i is unseen
        {
            count++;                                                //i is in a new component
            DFSstack.Push(i);                                    
            compAndReachHelper(DFSstack,seen);                        //perform dfs in i's component with dfs tree root as i(DFSstack.Top())
        }
    }
    return count;
}

/*  reach() - returns 1 if there is a path in graph between vertices u and v. else, returns 0. clearly, a (u,v) path exists iff u,v are in same component
    ARGS - int u, int v
    RET - int
*/
int Graph::reach(int u,int v)
{
    bool seen[vertexCount];                                            //seen[i] is true if i has already been seen. else, it is false
    Stack DFSstack;                                                    //stack of vertices for performing dfs
    for(int i=0;i<vertexCount;i++)
        seen[i] = false;
    DFSstack.Push(u);
    compAndReachHelper(DFSstack,seen);                                //if v is in same component as u, then at end of this stmt, seen[v] = true
    return (int)seen[v];
}

/*  ~Graph() - Deallocates memory used for storing adjMatrix
    ARGS - void
*/
Graph::~Graph()
{
    for(int i=0;i<vertexCount;i++)
        delete[] adjMatrix[i];
    delete[] adjMatrix;
}

/* End {Implementation of Graph} */

/*  StackMenu() - function for performing the stack operations on an integer stack. operations performed are as in documentation
    ARGS - Stack& S - reference to input stack and int Q - no of queries
    RET - void
*/
void StackMenu(Stack& S,int Q)
{
    string query;                                        //input query
    int x;                                                //for storing push value from user
    for(int i=0;i<Q;i++)
    {
        cin >> query;
        if(query == "push")                                //Push(x) Op
        {
            cin >> x;
            S.Push(x);
        }
        else if(query == "pop")                            //Pop Op
        {
            cout << S.Pop() << endl;
        }
        else if(query == "top")                            //Top Op
        {
            cout << S.Top() << endl;
        }
        else if(query == "empty")                        //isEmpty Op    
        {
            cout << S.isEmpty() << endl;
        }
        else                                            //Illegal Op
        {
            exit(-1);
        }
    }
}

/*  GraphMenu() - function for performing the graph operations as in documentation 
    ARGS - Graph& G - reference to the input stack, int m - no of queries
    RET - void
*/
void GraphMenu(Graph& G,int m)
{
    string query;                                        //input query
    pair<int,int> edge;                                    //edge = (u,v) to be added or removed
    int u,v;                                            //inputs for the reach() function
    for(int i=0;i<m;i++)
    {
        cin >> query;
        if(query == "add")                                //adds input edge to graph
        {
            cin >> edge.first >> edge.second;
            G+=edge;
        }
        else if(query == "del")                            //removes input edge from graph
        {
            cin >> edge.first >> edge.second;
            G-=edge;
        }
        else if(query == "cycle")                        //finds if a cycle is there in graph
        {
            cout << G.detectCycle() << endl;
        }
        else if(query == "components")                    //finds no of components in graph
        {
            cout << G.components() << endl;
        }
        else if(query == "reach")                        //finds if (u,v) path exists in graph
        {
            cin >> u >> v;
            cout << G.reach(u,v) << endl;
        }
        else                                            //illegal Op
        {
            exit(-1);
        }
    }
}

int main()
{
    //Stack Section
    Stack S;
    int stackQueryCount;
    cin >> stackQueryCount;
    StackMenu(S,stackQueryCount);

    //Graph Section
    int noOfVertex;
    int graphQueryCount;
    cin >> noOfVertex >> graphQueryCount; 
    Graph G(noOfVertex);
    GraphMenu(G,graphQueryCount);

    return 0;
}

