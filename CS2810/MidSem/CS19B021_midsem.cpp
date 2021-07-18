/************************
* $ID$
* File:  CS19B021_midsem.cpp - create a class Refrigerator with a few data members, define a comparison operator on it, 
*                              perform a O(nlogn) sort on a Refrigerator array and find the "newVariance" of the sorted
*                              refrigerator array.
*
* Purpose: We create a class with attributes - integers: model number, price and capacity; real numbers: energy rating and customer
*          rating. We define some member functions of Refrigerator. Now, '<' operator is overloaded to apply to this class. We
*          say, for fridges a and b, a < b iff one of the following conditions apply: 1) b has higher energy rating than a <or> 2) a 
*          and b have the same energy rating but b has higher customer rating than a <or> 3) a and b have equal energy and customer ratings
*          but b has lower price/capacity ratio than a. We then define a mergesort procedure on an array of Refrigerators using '<' operator 
*           to sort the array in descending order. Finally, we define a function to find "newVariance" = average of squares of deviations 
*           of model numbers of array entries from median's(floor(N/2)th element) model number.
*
* Author: K V VIKRAM
*
* Created: [2021-03-15 14:23] by vikram
* 
* Last Modified: [2021-03-15 21:35] by vikram
*
* Input Format:
*    Line 1: number N that will represent number of Refrigerators
*    Next N lines will contain the records in the order(model number, price, capacity, energy rating, consumer rating).
*
* Constraints:
*    0 <= N <= 100
*    The maximum value of any entry will be 200000
*
* Output Format:
*    Line 1: model numbers of sorted array of refrigerators separated by ' '
*    Line 2: ceil(newVariance)
*
* Practice:   Array iterators in loops are typically named i or j. Sort implicitly means "sort in descending order" anywhere in the code.
*
* Compilation:  g++ CS19B021_midsem.cpp -o solution, ./solution.
*
* Bugs:  No major bugfixes or new releases
*************/

#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>
using namespace std;


/* Begin {Definition of class Refrigerator} */
/*  Refrigerator - defines a class Refrigerator with attributes as in documentation and a few member functions
    Data Members - modelNo(int), price(int), capacity(int), eRating(float) and cRating(float)
    public accessors - retModelNo()
    public mutators - getDetails()
    constructor - Default: Refrigerator()
    friend operator - operator <()
*/
class Refrigerator
{
    int modelNo;                                                        //model number
    int price;                                                          //price
    int capacity;                                                       //capacity
    float eRating;                                                      //energy rating
    float cRating;                                                      //customer rating
public:
    Refrigerator();                                                     //default constructor - sets all data members to illegal values
    void getDetails();                                                  //gets all data members from stdin
    int retModelNo();                                                   //returns the model number
    friend bool operator<(const Refrigerator&,const Refrigerator&);     //friend operator '<' overloaded to apply to this class
};
/* End {Definition of class Refrigerator} */


/* Prototypes of Other Functions */
void Merge(Refrigerator*,Refrigerator*,size_t,size_t,size_t);          //merge routine in mergesort procedure
void MergeSortRange(Refrigerator*,Refrigerator*,size_t,size_t);        //performs mergesort on given range of array elements
void MergeSort(Refrigerator*,size_t);                                  //wrapper function of MergeSortRange. does mergesort on whole array
float ComputeNewVariance(Refrigerator*,size_t);                        //computes newVariance
bool operator<(const Refrigerator&,const Refrigerator&);               //defines a '<' ordering on class of Refrigerators


/* Begin {Implementation of class Refrigerator} */
/*  Refrigerator() - constructor to set all data members to illegal values(-1 in case of integer fields and -1.0 otherwise) 
    ARGS - void
*/
Refrigerator::Refrigerator()
{
    modelNo = price = capacity = -1;
    eRating = cRating = -1.0;
}

/*  getDetails() - gets all data members from stdin. order of input is acc. to input format
    ARGS - void
    RET - void
*/
void Refrigerator::getDetails()
{
    cin >> modelNo >> price >> capacity;
    cin >> eRating >> cRating;
}

/*  retModelNo() - returns modelNo(model number)
    ARGS - void
    RET - int
*/
int Refrigerator::retModelNo()
{
    return modelNo;
}
/* End {Implementation of class Refrigerator} */

/*  operator<() - relational operator on Refrigerator class. We say a < b when a,b satisfy one of the conditions in documentation.
    ARGS - const Refrigerator& a and const Refrigerator& b
    RET - bool 
*/
bool operator<(const Refrigerator& a,const Refrigerator& b)
{
    if(a.eRating < b.eRating)                                                //condition 1 for a < b
        return true;
    else if(a.eRating > b.eRating)
        return false;
    else
    {
        if(a.cRating < b.cRating)                                            //condition 2 for a < b
            return true;
        else if(a.cRating > b.cRating)
            return false;
        else
        {
            if(((float)a.price)/a.capacity > ((float)b.price)/b.capacity)    //condition 3 for a < b
                return true;
            else
                return false;
        }
    }
}

/*  Merge() - merges the sorted segments arr[bIndex..mIndex] (left) and arr[mIndex+1..eIndex] (right) into a single sorted arr[bIndex..eIndex].
    ARG - Refrigerator* arr, Refrigerator* tempArr, size_t bIndex,size_t mIndex, size_t eIndex
    RET - void
*/
void Merge(Refrigerator* arr,Refrigerator* tempArr,size_t bIndex,size_t mIndex,size_t eIndex)
{
    size_t leftPtr = bIndex;                                //ptr to left portion of arr
    size_t rightPtr = mIndex + 1;                           //ptr to right portion of arr
    size_t tempPtr = bIndex;                                //ptr to tempArr
    while(leftPtr <= mIndex && rightPtr <= eIndex)          //while both left and right portions are not exhausted
    {
        if(arr[leftPtr] < arr[rightPtr])                    //if current element in left is smaller than that of right
        {
            tempArr[tempPtr++] = arr[rightPtr++];           //current element of right is inserted in tempArr
        }
        else
        {
            tempArr[tempPtr++] = arr[leftPtr++];            //current element of left is inserted in tempArr
        }
    }
    while(leftPtr <= mIndex)                                //remaining elements of left(if any) are put into tempArr
        tempArr[tempPtr++] = arr[leftPtr++];
    while(rightPtr <= eIndex)                               //remaining elements of right(if any) are put into tempArr
        tempArr[tempPtr++] = arr[rightPtr++];
    for(size_t i = bIndex;i<=eIndex;i++)                    //tempArr[bIndex..eIndex] is copied into arr[bIndex..eIndex]
    {
        arr[i] = tempArr[i];
    }
    return ;
}

/*  MergeSortRange() - Sorts arr[bIndex..eIndex]. Uses tempArr as an auxiliary array to pass to Merge() function. It divides arr[bIndex..eIndex] 
                       into two segments, arr[bIndex..mIndex](left) and arr[mIndex+1..eIndex](right) where mIndex = int avg of bIndex and eIndex. It
                       then sorts the two segments. Now, in the conquering phase, the function Merge() merges left and right into a single sorted portion
                       arr[bIndex..eIndex].
    ARG - Refrigerator* arr, Refrigerator* tempArr, size_t bIndex, size_t eIndex
    RET - void
*/
void MergeSortRange(Refrigerator* arr,Refrigerator* tempArr,size_t bIndex,size_t eIndex)
{
    if(bIndex==eIndex)                                          //termination criteria. array of size 1 is sorted
        return ;
    else
    {
        size_t mIndex = (bIndex+eIndex)/2;                      //mIndex is computed
        MergeSortRange(arr,tempArr,bIndex,mIndex);              //left is sorted
        MergeSortRange(arr,tempArr,mIndex+1,eIndex);            //right is sorted
        Merge(arr,tempArr,bIndex,mIndex,eIndex);                //left and right are merged
        return;
    }
}

/*  MergeSort() - Sorts arr completely. Creates an auxiliary array to facilitate merging and then calls MergeSortRange() to do this.
    ARG - Refrigerator* arr, size_t arrSize
    RET - void
*/
void MergeSort(Refrigerator* arr,size_t arrSize)
{
    Refrigerator* tempArr = new Refrigerator[arrSize]{};        //aux array for merging purpose
    MergeSortRange(arr,tempArr,0,arrSize-1);                    //arr is sorted
    delete[] tempArr;                                           //aux array is deleted
    return ;
}

/*  ComputeNewVariance() - Computes newVariance(as in documentation) of a sorted array arr of size arrSize.
    ARG - Refrigerator* arr, size_t arrSize
    RET - float
*/
float ComputeNewVariance(Refrigerator* arr,size_t arrSize)
{
    int medPos = arrSize/2;                                        //median's index
    int medModelNo = arr[medPos].retModelNo();                     //median's model number
    int sumSqrDeviation = 0;                                       //sum of squares of deviations of model numbers from medModelNo
    int difference;                                                //deviation of current(in loop) model no from median model no
    for(size_t i = 0;i<arrSize;i++)
    {
        difference = (arr[i].retModelNo()-medModelNo);             //current element's deviation is computed
        sumSqrDeviation += difference*difference;                  //sumSqrDeviation is updated
    }
    float retValue = ((float)sumSqrDeviation/arrSize);             //newVariance is found and returned
    return retValue;
}

int main() 
{
    Refrigerator* fridgeArr;                                        //array to store inputted refrigerators
    size_t noOfModels;                                              //number of models of refrigerators
    float newVariance;                                              //var to store newVariance

    //Stmts to get INPUT
    cin >> noOfModels;                                              //number of models is obtained
    fridgeArr = new Refrigerator[noOfModels]{};                     //required space for fridgeArr is allocated
    for(size_t i=0;i<noOfModels;i++)                                //values of attributes of every fridge is obtained
        fridgeArr[i].getDetails();

    //Stmts to perform computations
    MergeSort(fridgeArr,noOfModels);                                //fridgeArr is sorted
    newVariance = ComputeNewVariance(fridgeArr,noOfModels);         //newVariance is computed

    //Stmts to display OUTPUT
    for(size_t i=0;i<noOfModels;i++)                                //model mumbers of sorted fridgeArr is printed
        cout << fridgeArr[i].retModelNo() << ' ';
    cout << endl;
    cout << ceil(newVariance) << endl;                              //ceil(newVariance) is displayed

    delete[] fridgeArr;                                             //frees space used by fridgeArr
    return 0;
}
