/************************
* $ID$
* File:  CS19B021_A4.cpp - Creation of a class template Vector which is like STL vector but with reduced functionality.
*
* Purpose: Definition and implementation of a class template Vector<T> which implements a self-resizing(capacity 
*          only increases) list of entries of T with operations of insertion/deletion at end, finding front/rear 
*          element,finding size/capacity, finding the element at a given position and sorting the list by quicksort 
*          routine(assuming T is a comparable type for which <,>,= is well-defined.)
*
* Author: K V VIKRAM
*
* Created: [2021-03-01 15:47] by vikram
* 
* Last Modified: [2021-03-02 18:30] by vikram
*
* Input Format:
*   First line contains any one of the following operation to initialize the vector:
*     vector c -> Vector(c)
*     vector c x -> Vector(c, x)
*     vector -> Vector()
*
*   Next line contains Q, the number of operations to be performed on the list.
*   Each of the next Q lines represents a single operation of the format:
*     push x -> push(x)
*     pop -> pop()
*     front -> front()
*     back -> back()
*     capacity -> capacity()
*     size -> size()
*     sort -> sort()
*     element i -> operator[] (i)
*
* Constraints:
*   1 <= c <= 1000
*   0 <= x <= INT_MAX {(2^31) - 1}
*   1 <= Q <= 1000
*   0 <= i <= c-1
*
* Output Format:
*   front : Print the returned element on new line
*   back : Print the returned element on new line
*   capacity: Print the returned element on new line
*   size: Print the returned element on new line
*   sort : Print the sorted elements on a new line
*   element : Print the returned element on new line
*
* Assumption: Sufficient dynamic memory is assumed to be available.This exception is neither caught nor handled.
*
* Practice:   Array iterators in loops are typically named i or j.
*
* Compilation:  g++ CS19B021_A4.cpp -o solution, ./solution.
*
* Bugs:  No major bugfixes or new releases
*************/

#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>
#include<sstream>
using namespace std;

/* Begin {Definition of class template Vector} */
/*  Vector<T> - Implements a list ADT similar to a stack(ins/del at rear end only) with associated operations as in documentation
    Data Members - vec, currentSize, currentCapacity
    Public Mutator - sort(),push(T),pop()
    Protected/Private Mutator - partition(),sortHelper()
    Accessor- front(),back(),operator[size_t],size(),capacity(),printVector()
    Constructor - Vector(size_t),Vector(size_t,T),Vector()
    Destructor - ~Vector()
    Assumptions - T is well-ordered type where <,>,= is well-defined. T has a default constructor and a
    parametrised constructor of the form T{int}.cout has been overloaded to work on T type.
*/
template <class T> class Vector
{
private:
    size_t currentSize;                             //current number of elements in vector
    size_t currentCapacity;                         //current capacity of vector.
    T* vec;                                         //ptr to array of elements of type T
    void sortHelper(int,int);                       //helper function to do quicksort. used by sort()
protected:
    int partition(int,int,T);                       //helper function to do partitioning step in quicksort routine. used by sortHelper()
public:
    Vector(size_t);                                 //allocates space for an array of "arg" many elements of type T. vec is ptr to this array
    Vector(size_t,T);                               //allocates space for an array of "arg1" many elements of T and sets each of them to "arg2". vec is ptr to this array
    Vector();                                       //allocates space for a single element of type T
    size_t capacity();                              //returns currentSize
    size_t size();                                  //returns currentCapacity
    void push(T);                                   //inserts a "arg" at the unassigned slot in the array. if array is full,it is resized before insertion 
    void pop();                                     //deletes the last element in the array
    T front();                                      //returns the first elem in array if it is non-empty.else, returns T{-1}.
    T back();                                       //returns the last elem in array if it is non-empty.else, returns T{-1}.
    T operator[](size_t);                           //returns the "arg+1"th elem in array from left
    void sort();                                    //wrapper function of sortHelper(). sorts the whole array
    void printVector();                             //prints the contents of array sep by ' ' and terminated with '\n'
    ~Vector();                                      //de-allocates memory allocated for the vec array
};
/* End {Definition of class template Vector} */

/* Begin {Implementation of class template Vector} */

/*  Vector(size_t) - parametrised constructor to alloc space for a vector of capacity "reqCapacity" and type T. no entries are entered into the vector
                     default constructor of type T is used while allocating space
    ARGS - size_t reqCapacity
*/
template <class T> 
Vector<T>::Vector(size_t reqCapacity)
{
    vec = new T[reqCapacity]{};
    currentCapacity = reqCapacity;
    currentSize = 0;
}


/*  Vector(size_t,T) - parametrised constructor to alloc space for a vector of capacity "reqCapacity" and type T. All entries of vector are set to "initValue".
    ARGS - size_t reqCapacity, T initValue
*/
template <class T>
Vector<T>::Vector(size_t reqCapacity,T initValue)
{
    vec = new T[reqCapacity];
    for(size_t i=0;i<reqCapacity;i++)
    {
        vec[i] = initValue;
    }
    currentCapacity = currentSize = reqCapacity;
}

/*  Vector() - default constructor to alloc space for a vector of capacity 1 and type T.No entry is made in the vector.
    ARGS - void
*/
template <class T>
Vector<T>::Vector()
{
    vec = new T[1]{};
    currentCapacity = 1;
    currentSize = 0;
}

/*  ~Vector(size_t,T) - de-allocates dynamic memory allocated to store vector(space allocated for the array vec)
    ARGS - void
*/
template <class T>
Vector<T>::~Vector()
{
    delete[] vec;
}

/*  capacity() - returns the member currentCapacity of the vector
    ARGS - void
    RET - size_t
*/
template <class T>
size_t Vector<T>::capacity()
{
    return currentCapacity;
}

/*  size() - returns the current number of elements(the member currentSize) in the vector
    ARGS - void
    RET - size_t
*/
template <class T>
size_t Vector<T>::size()
{
    return currentSize;
}

/*  push(T) - Inserts x in the first unassigned slot of vector if it is not full.Otherwise, it doubles vector's capacity and then inserts x
              in the first unassigned slot.
    ARGS - T x
    RET - void
*/
template <class T>
void Vector<T>::push(T x)
{
    if(currentSize==currentCapacity)                            //if vector is full
    {
        T* newArray = new T[2*currentCapacity]{};               //space for an array of double the capacity of vector is allocated
        for(size_t i=0;i<currentCapacity;i++)
        {
            newArray[i] = vec[i];                               //entries of vec[] are copied into new array
        }
        newArray[currentCapacity] = x;                          //x is inserted
        delete[] vec;                                           //old array(vec) is deleted
        vec = newArray;                                         //vec is set to point to the new array
        currentCapacity*=2;                                     //capacity update
        ++currentSize;                                          //size update
    }
    else                                                        //here, vector has space to insert x
    {
        vec[currentSize] = x;
        ++currentSize;
    }
}

/*  pop() - deletes the last element in the vector. If vector is empty, nothing is done
    ARGS - void
    RET - void
*/
template <class T>
void Vector<T>::pop()
{
    if(currentSize==0)                  //empty vector
    {
        return ;
    }
    else
    {
        --currentSize;                  //currentSize is decremented by 1. Effectively, the last element of vector is "deleted".
    }
}

/*  front() - returns the first element in the vector. If vector is empty, T{-1} is returned
    ARGS - void
    RET - T
*/
template <class T>
T Vector<T>::front()
{
    return(currentSize==0?(T{-1}):vec[0]);
}

/*  back() - returns the last element in the vector. If vector is empty, T{-1} is returned
    ARGS - void
    RET - T
*/
template <class T>
T Vector<T>::back()
{
    return(currentSize==0?(T{-1}):vec[currentSize-1]);
}

/*  operator []() - returns the (index+1)th element in the vector. If element is unassigned, T{-1} is returned
    ARGS - size_t index
    RET - T
*/
template <class T>
T Vector<T>::operator[](size_t index)
{
    if(index<currentSize)                   //here, element is in the vector
        return vec[index];
    else                                    //here, element is not in the vector
        return T{-1};
}

/*  printVector() - prints the elements in the vector separated by ' ' and adds a newline finally
    ARGS - void
    RET - void
*/
template <class T>
void Vector<T>::printVector()
{
    for(size_t i = 0;i<currentSize;i++)
    {
        cout << vec[i] << ' ';
    }
    cout <<  endl;
}

/*  partition() - partitions vec[begIndex..endIndex] into vec[begIndex..(p-1)](seg1) of elem <= pivot
                  and vec[(p+1)..endIndex](seg2) of elements > pivot with vec[p] = pivot for some p in
                  {begIndex,begIndex+1,..endIndex}. Uses Lomuto's Partition Scheme.
    ARGS - int begIndex,int endIndex,T pivot
    RET - int pivotLocation
*/
template <class T>
int Vector<T>::partition(int begIndex,int endIndex,T pivot)
{
    int itr = (begIndex+1);                         //iterator to traverse over elements of vec[begIndex..endIndex]
    int mItr = (begIndex+1);                        //move iterator - stores position in vec to which an element <= pivot needs to be moved when found. Locates interface between seg1 and seg2
    T temp;                                         //temporary variable for swapping
    for(;itr<=endIndex;itr++)                       //traverse over all entries in vec[begIndex..endIndex] using itr
    {
        if(vec[itr] <= pivot)                       //if an elem <= pivot has been found, it must be moved to position mItr
        {
            temp = vec[itr];                        //stmts to swap entries at itr and mItr
            vec[itr] = vec[mItr];
            vec[mItr] = temp;
            mItr++;                                 //next move can be made at mItr + 1
        }
    }
    vec[begIndex] = vec[mItr-1];                    //the element(<= pivot) at boundary of seg1 is placed in the front to make space for pivot
    vec[mItr-1] = pivot;                            //pivot is placed at its final position
    return(mItr-1);                                 //position of pivot is returned
}

/*  sortHelper() - performs quicksort on vec[begIndex..endIndex] using vec[begIndex] (first element) as pivot. helper function which is wrapped over
                   by sort().
    ARGS - int begIndex,int endIndex
    RET - void
    Runtime - avg case - O(nlogn) and worst case - O(n^2) where n = size of array under consideration
*/
template <class T>
void Vector<T>::sortHelper(int begIndex,int endIndex)
{
    if(begIndex>=endIndex)                      //if array segment is empty or has only one elem, terminate without doing anything
    {
        return ;
    }
    else
    {
        T pivot = vec[begIndex];                                            //pivot is found
        int pivotLocation = partition(begIndex,endIndex,pivot);             //partitioning about pivot is done and pivotLocation is found
        sortHelper(begIndex,pivotLocation-1);                               //recursive quicksort on vec[begIndex..(pivotLocation-1)]
        sortHelper(pivotLocation+1,endIndex);                               //recursive quicksort on vec[(pivotLocation+1)..endIndex]
        return ;
    }
}

/*  sort() - sorts the whole array vec[] using quicksort routine as in sortHelper(). wrapper of sortHelper()
    ARGS - void
    RET - void
*/
template <class T>
void Vector<T>::sort()
{
    sortHelper(0,currentSize-1);
}

/*  VectorOps() - function template to perform operations on a Vector<T> which are listed in class template defn of Vector and documentation. 
    ARGS - Vector<T>* vecPtr, int queryCount
    RET - void
*/
template <class T>
void VectorOps(Vector<T>* vecPtr,int queryCount)
{
    string query;                                       //input query
    int pushValue;                                      //value to insert in push op
    size_t index;                                       //index in element op
    for(int i=0;i<queryCount;i++)
    {
        cin >> query;                                   //query is obtained
        if(query == "push")                             //push x op
        {
            cin >> pushValue;
            vecPtr->push(pushValue);                    //pushValue is pushed
        }
        else if(query == "pop")                         //pop op
        {
            vecPtr->pop();
        }
        else if(query == "front")                       //front op
        {
            cout << vecPtr->front() << endl;            //first elem is printed
        }
        else if(query == "back")                        //back op
        {
            cout << vecPtr->back() << endl;             //last elem is printed
        }
        else if(query == "element")                     //element i op
        {
            cin >> index;                               
            cout << (*vecPtr)[index] << endl;           //the (index+1)th entry of vector is displayed
        }
        else if(query == "capacity")                    //capacity op
        {
            cout << vecPtr->capacity() << endl;         //currentCapacity is printed
        }
        else if(query == "size")                        //size op
        {
            cout << vecPtr->size() << endl;             //current no of elems in vector is printed
        }
        else if(query == "sort")                        //sort op
        {
            vecPtr->sort();                             //vector elems are sorted
            vecPtr->printVector();                      //sorted vector is displayed
        }
        else
        {
            exit(-1);                                   //illegal op
        }
    }
}

int main() 
{
    Vector<int>* myVector;                                      //ptr to type Vector<int>
    string word;                                                //var to scan first line of input
    string seg;                                                 //segment to get "vector" from word using sstream obj ss in firstline.
    stringstream ss;                                            //ss - obj of sstream
    size_t cpcty{0};                                            //capacity of myVector. initialized with illegal value(acc. input format)
    int initValue{-1};                                          //initValue - initialization value of myVector. also initialized with illegal value
    getline(cin,word);                                          //first line is scanned
    ss << word;                                                 //word is pushed into ss
    ss >> seg >> cpcty >> initValue;                            //seg ="vector" , capacity(if specified) and initValue(if specified) are scanned from ss
    if(cpcty>0)                                                 //if word was not "vector "
    {
        if(initValue>=0)                                        //if word was not "vector " + c(capacity)
        {
            myVector = new Vector<int>{cpcty,initValue};        //myVector points to Vector<int> obj with capacity = size = cpcty and all entries = initValue
        }
        else
        {
            myVector = new Vector<int>{cpcty};                  //myVector points to Vector<int> obj with capacity = cpcty and size = 0
        }
    }
    else
    {
        myVector = new Vector<int>{};                           //myVector points to Vector<int> obj with capacity = 1 and size = 0
    }
    int queryCount;                                             //number of queries to be done
    cin >> queryCount;
    VectorOps<int>(myVector,queryCount);                        //function call to perform queries
    delete myVector;                                            //myVector is deleted
    return 0;
}