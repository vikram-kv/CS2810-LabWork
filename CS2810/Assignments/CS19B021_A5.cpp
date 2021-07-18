/************************
* $ID$
* File:  CS19B021_A5.cpp - To understand the practical applications of divide and conquer paradigm by implementing a few algorithms of this type in the class
*                          BloomBrain.
*
* Purpose: class BloomBrain is used to represent rooms and their occupant's(if any) seraphic number in a meditation center. The center has N rooms with atmost
*          one occupant each. There are two events called quality talks and crossover. The rule of quality talk: Two people can have the quality talk if and only if 
*          the person who is occupying a room with a lesser room number has more (>) seraphic number than the other. A crossover is a game between a guru and a group 
*          members. The guru will not lose if his seraphic number >= sum of member's seraphic number.A group is valid : Let m1 be the member in the group with lowest room 
*          number n1 and m2 be the member with the highest room number n2. Now a valid group should contain all the members occupying room n1 till room n2.Now, we use
*          divide and conquer paradigm to find the number of possible quality talks and the minimum seraphic number of the guru so that he does not lose even against the
*          best valid group.
*
* Implemented functions:
*       update : Takes an array of size N as input which indicates the corresponding seraphic number of members and prints the number of quality talks possible 
*                (between existing members) after the update. The first update is such that it makes all rooms occupied and subsequent updates will not give a
*                non-zero value to an unoccupied room. The status of any room depends only on join/leave after first update.
*       join : It will take one integer value as input which indicates the initial seraphic number of the new joining member. This new member will be given the 
*              last (highest number) unoccupied room.
*       leave : It will take one integer value as input, the member occupying that room number will leave the centre. The room will be emptied with a 0 seraphic number.
*       occupied : This will output the number of existing members in the centre.
*       play : print the minimum seraphic number of guru reqd so that he does not lose in calmness crossover game.
*
* Author: K V VIKRAM
*
* Created: [2021-03-09 11:26] by vikram
* 
* Last Modified: [2021-03-09 11:26] by vikram
*
* Input Format:
*       First line will have a value N : number of rooms
*       Second line will have a value Q : number of queries
*       5 different queries are possible.
*           update : for this query in the next line there will be N values, each separated by a single space.
*           join : for this query there will be a value s in the same line
*           leave : for this query there will be a value idx in the same line
*           occupied : there won’t be any input for this query
*           play : there won’t be any input for this query
*
* Constraints:
*   5 <= N <= 10^5
*   1 <= Q <= 200
*   -10^3 <= s <= 10^3, where s indicates a seraphic number
*   0<=  idx < N
*
* Output Format:
*   All outputs to any query occur in a newline.
*   update : prints the number of quality talks possible between existing members.
*   occupied : prints the number of existing members in the center
*   play : prints the minimum seraphic number of the guru such that the guru will not lose even against the best possible group
*
* Assumption: Sufficient dynamic memory is assumed to be available.This exception is neither caught nor handled.Input strictly follows input format
*
* Practice:   Array iterators in loops are typically named i or j.
*
* Compilation:  g++ CS19B021_A5.cpp -o solution, ./solution.
*
* Bugs:  No major bugfixes or new releases
*************/

#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>
using namespace std;

/*  Room - Used to represent a room in the center by storing the occupant's seraphic number and occupancy status.
    Members - int seraphicNum,bool status
*/
struct Room
{
    int seraphicNum;                //occupant seraphic number
    bool status;                    //room status is true iff room is occupied
};

/* SumsType - Used as a return type for computeMaxSubArrSum() function. Stores the blocksum,prefixsum,suffixsum and max subarray sum
   Members - blockSum,prefixSum,suffixSum and maxSubArrSum
   Let bIndex and eIndex be passed to computeMaxSubArrSum. Also let R refer to rooms[bIndex..eIndex].
   A subarray of X[] is a contiguous block of elements of X[].Let sum(X[]) be over seraphic numbers of X[].
*/
struct SumsType
{
    int blockSum;                   //sum(R)
    int prefixSum;                  //max(sum(subarray of R that start at bIndex))
    int suffixSum;                  //max(sum(subarray of R that end at eIndex))
    int maxSubArrSum;               //max(sum(subarray of R))
};

/* Begin {Definition of class BloomBrain} */
/*  BloomBrain - class of centers of meditation with functionality as defined in "purpose" and "implemented functions" segments of documentation.
    Data Members - Room* rooms,size_t roomCount, size_t occupancy
    public accessors - occupied(),play()
    public mutators - update(int*,size_t),join(int),leave(int)
    constructor - parametrised: BloomBrain(size_t), default: none
    destructor - ~BloomBrain()
    private functions: accessors - max(int,int),max(int,int,int),computeMaxSubArrSum(int,int),computeQualityTalks(),
                                   talksCountAndSort(int*,int*,size_t,size_t),crossTalksAndMerge(int*,int*,size_t,size_t,size_t)
*/
class BloomBrain
{
    Room* rooms;                                                    //ptr to array of rooms in institute
    size_t roomCount;                                               //number of rooms in institute
    size_t occupancy;                                               //number of currently occupied rooms
    int max(int,int);                                               //returns the maximum of 2 integers(args)
    int max(int,int,int);                                           //returns the maximum of 3 integers(args)
    long computeQualityTalks();                                     //computes number of quality talks possible and returns it
    long talksCountAndSort(int*,int*,size_t,size_t);                //computes no of quality talks in an array segment and sorts it
    long crossTalksAndMerge(int*,int*,size_t,size_t,size_t);        //computes no of cross talks between two sorted subarrays and merges them into a single sorted array
    SumsType computeMaxSubArrSum(int,int);                          //computes maximum subarray sum in the "array" rooms[i].seraphicNum for 'arg1' <= i <= 'arg2'
public:
    BloomBrain(size_t);                                             //constructor to initialize an obj with 'arg' as roomCount
    ~BloomBrain();                                                  //destructor to de-allocate memory used for storing rooms array
    void update(int*,size_t);                                       //to perform the update operation with 'arg1' as new array and 'arg2' as the update number
    void join(int);                                                 //join operation with seraphic number of the new member as 'arg'
    void leave(int);                                                //leave operation to vacate room no 'arg'
    void occupied();                                                //number of members(occupied rooms) in the institute
    void play();                                                    //play operation
};
/* End {Definition of class BloomBrain} */


/* Begin {Implementation of class BloomBrain} */
/*  BloomBrain(size_t) - initializes a new obj with 'arg' no of rooms and occupancy as 0 
    ARGS - size_t noOfRooms
*/
BloomBrain::BloomBrain(size_t noOfRooms)
{
    rooms = new Room[noOfRooms];
    roomCount = noOfRooms;
    occupancy = 0;
}

/*  ~BloomBrain() - frees the dynamic memory allocated to store rooms[]
    ARGS - void
*/
BloomBrain::~BloomBrain()
{
    delete[] rooms;
}

/*  max() - returns the maximum of two integers
    ARGS - int a, int b
    RET - int
*/
int BloomBrain::max(int a,int b)
{
    if(a>b)
        return a;
    else
        return b;
}

/*  max() - returns the maximum of three integers
    ARGS - int a, int b, int c
    RET - int
*/
int BloomBrain::max(int a,int b,int c)
{
    return max(max(a,b),c);
}

/*  join() - adds a new member with seraphic number 'arg' to the unoccupied room with the largest index
    ARGS - int newSeraphicNum
    RET - void
*/
void BloomBrain::join(int newSeraphicNum)
{
    for(size_t j = roomCount-1;j>=0;j--)                    //start from last room and iterate towards room 0
    {
        if(rooms[j].status == false)                        //if an unoccupied room is found
        {
            rooms[j].status = true;                         //its status is changed to occupied
            rooms[j].seraphicNum = newSeraphicNum;          //the occupant seraphic num = 'arg'
            ++occupancy;                                    //total no of occupied rooms is inc by 1
            break;                                          //loop terminates
        }
    }
    return;                                 
}

/*  leave() - the member with the room no 'arg' leaves the institute. The room is made unoccupied and its seraphicNum is set to 0
    ARGS - int roomIdx
    RET - void
*/
void BloomBrain::leave(int roomIdx)
{
    rooms[roomIdx].status = false;                          //roomIdx is vacated
    rooms[roomIdx].seraphicNum = 0;                         //new seraphic number of room is 0
    --occupancy;                                            //total no of occupied is dec by 1
    return;
}

/*  occupied() - prints the number of occupied rooms(data member occupancy) in the institute
    ARGS - void
    RET - void
*/
void BloomBrain::occupied()
{
    cout << occupancy << endl;
    return;
}

/*  play() - prints the minimum seraphic number of the guru such that the guru will not lose even against the best possible group.
             the best possible group is formed by the members of the subarray(of rooms[]) with the maximum sum of seraphic numbers.
             this sum must be the seraphic number of the guru for him to not lose. Now, maximum subarray sum is found by divide and 
             conquer paradigm using the function computeMaxSubArrSum().
    ARGS - void
    RET - void
*/
void BloomBrain::play()
{
    SumsType roomSums = computeMaxSubArrSum(0,roomCount-1);         //blockSum,prefixSum,suffixSum and maxSubArrSum of whole "array" of rooms[].seraphicNum is found
    cout << roomSums.maxSubArrSum << endl;                          //guru seraphic number = max sub array sum is printed
    return;
}

/*  update() - copies the argument array newArr[] into the "array" rooms[].seraphicNum entrywise.If the updateNo is 1(first update),
               then all the rooms in the institute are set to occupied.finally, the number of quality talks is computed by calling
               computeQualityTalks() and it is printed.
    ARGS - int* newArr, size_t updateNo
    RET - void
*/
void BloomBrain::update(int* newArr,size_t updateNo)
{
    if(updateNo == 1)                                    //as per input rules, on first update, all rooms are set to occupied
    {
        for(size_t i=0;i<roomCount;i++)
            rooms[i].status = true;
        occupancy = roomCount;
    }
    for(size_t i=0;i<roomCount;i++)                        //loop to copy newArr into rooms[].seraphicNum entrywise
    {
        rooms[i].seraphicNum = newArr[i];        
    }
    cout << computeQualityTalks() << endl;                //no of quality talks is displayed
    return;
}

/*    computeQualityTalks() - returns the number of quality talks possible among members in the institute. does this using the talksCountAndSort()
                            function
    ARG - void
    RET - long qualityTalks
*/
long BloomBrain::computeQualityTalks()
{
    long qualityTalks = 0;                                                            //var to store no of quality talks possible
    if(occupancy == 0)                                                                //if there are no members in the institute, return 0
        return qualityTalks;
    int* occRoomArr = new int[occupancy];                                            //occRoomArr - holds the seraphic number of members in occupied rooms of institute. occRoomArr
    size_t tempPtr = 0;                                                                //has the seraphic numbers in same order as rooms[] array.tempPtr - temp ptr to fill occRoomArr
    for(size_t i=0;i<roomCount;i++)                                                    //loop to fill occRoomArr
    {
        if(rooms[i].status == true)                                                    //if rooms[i] is occupied, its occupant's seraphic value is added to occRoomArr
            occRoomArr[tempPtr++] = rooms[i].seraphicNum;
    }
    int* tempArray = new int[occupancy];                                            //tempArray - temp array to facilitate merging in crossTalksAndMerge() function.
    qualityTalks = talksCountAndSort(occRoomArr,tempArray,0,occupancy-1);            //qualityTalks is computed by talksCountAndSort() function.
    delete[] tempArray;                                                                //space allocated to occRoomArr and tempArray are freed
    delete[] occRoomArr;
    return qualityTalks;                                                            //no of quality talks is returned
}

/*    talksCountAndSort() - returns the number of quality talks possible in arr[lIndex..hIndex] and also sorts it using a algorithm that is a slightly
                          modified version of mergesort. uses tempArr as an auxiliary array to pass to crossTalksAndMerge() function. It divides 
                          arr[lIndex..hIndex] into two segments, arr[lIndex..mIndex](left) and arr[mIndex+1..hIndex](right) where mIndex = int avg of lIndex 
                          and hIndex, solves the two segments by sorting both and finding the totaltalks within both of them. Now clearly, 
                          talks(arr[lIndex..hIndex]) = talks(left) + talks(right) + crosstalks. here, crosstalks are valid talks between a member in right 
                          and a member in left. Now, in the conquering phase, the function crossTalksCountAndMerge() gives the number of crossTalks and also 
                          merges left and right into a single sorted arr[lIndex..hIndex].
    ARG - int* arr, int* tempArr, size_t lIndex, size_t hIndex
    RET - long totalTalks
*/
long BloomBrain::talksCountAndSort(int* arr,int* tempArr,size_t lIndex,size_t hIndex)
{
    if(lIndex == hIndex)                                                            //termination criteria of recursion. array of size 1 is sorted and 0 talks are possible within it
    {
        return 0;
    }
    else
    {
        long totalTalks = 0;                                                        //var to count total talks
        size_t mIndex = (lIndex+hIndex)/2;                                            //mIndex is taken as floor(avg of lIndex and hIndex). used as boundary index for dividing
        totalTalks += talksCountAndSort(arr,tempArr,lIndex,mIndex);                    //left is sorted and talks(left) is added to totalTalks
        totalTalks += talksCountAndSort(arr,tempArr,mIndex+1,hIndex);                //right is sorted and talks(right) is added to totalTalks
        totalTalks += crossTalksAndMerge(arr,tempArr,lIndex,mIndex,hIndex);            //left and right are merged. crossTalks is added to totalTalks
        return totalTalks;                                                            //totalTalks is returned
    }
}

/*    crossTalksAndMerge() - merges the sorted segments arr[bIndex..mIndex] (left) and arr[mIndex+1..eIndex] (right) into a single sorted arr[bIndex..eIndex]
                           and returns the number of cross quality talks which is found simulataneously as the merging is done. To do this, we note that
                           just before an element of right, arr[j] with j>mIndex, is inserted, exactly the remaining elements in left are > than arr[j].
                           So, if i is the ptr of left used for merging, exactly mIndex - i + 1 cross talks are possible between arr[j] and left. So we find
                           the sum for all such j. Finally, we note that if any one of left or right is completely exhausted, then no additional cross talks
                           need to be included.
    ARG - int* arr, int* tempArr, size_t lIndex,size_t mIndex, size_t hIndex
    RET - long crossTalks
*/
long BloomBrain::crossTalksAndMerge(int* arr,int* tempArr,size_t bIndex,size_t mIndex,size_t eIndex)
{
    size_t leftPtr = bIndex;                                        //ptr to left portion arr[bIndex..mIndex]
    size_t rightPtr = mIndex + 1;                                    //ptr to right portion arr[mIndex+1..eIndex]
    size_t tempPtr = bIndex;                                        //ptr to fill entries in tempArr
    long crossTalks = 0;                                            //no of cross talks possible
    while(leftPtr <= mIndex && rightPtr <= eIndex)                    //while both left portion and right portion are not exhausted
    {
        if(arr[leftPtr]<= arr[rightPtr])                            //if arr[leftPtr] is lte than arr[rightPtr], no cross talks are added
        {
            tempArr[tempPtr++] = arr[leftPtr++];                    //arr[leftPtr] is just put into tempArray(in its final sorted position)
        }
        else
        {                                                            //here, arr[leftPtr] is the first element in left that is > arr[rightPtr].so, a cross talk
            crossTalks += mIndex - leftPtr + 1;                        //between elems at leftPtr,leftPtr+1..mIndex and arr[rightPtr] is possible and the list is complete.
            tempArr[tempPtr++] = arr[rightPtr++];                    //So, no of crosstalks is inc by mIndex - leftPtr + 1 and arr[rightPtr] is put into tempArr
        }
    }
    while(leftPtr <= mIndex)                                        //remaining elems of left(if any) are put into tempArr
        tempArr[tempPtr++] = arr[leftPtr++];
    while(rightPtr <= eIndex)                                        //remaining elems of right(if any) are put into tempArr
        tempArr[tempPtr++] = arr[rightPtr++];
    for(size_t i = bIndex;i<=eIndex;i++)                            //elements of tempArr[bIndex..eIndex] are copied into arr[bIndex..eIndex] entrywise.
    {
        arr[i] = tempArr[i];
    }
    return crossTalks;                                                //no of cross quality talks is returned
}

/*    computeMaxSubArrSum() - recursive function which computes the max subarray sum taken over all subarrays of the "array" rooms[].seraphicNum,
                            contained within bIndex and eIndex(both included). based on divide and conquer paradigm. we first note that the function
                            returns a structure comprised of various sums(BS - block sum, PS - max prefix sum, SS - max suffix sum and MSS - max subarray
                            sum) with detailed definitions given as comments in struct SumsType definition.In the dividing phase, the function 
                            splits array[bIndex..eIndex](total) into array[bIndex..mIndex] (left) and array[mIndex+1..eIndex] (right), where mIndex = floor
                            of avg of bIndex and eIndex.After this, it solves each of them by returning the various sums for each of them. Now in the 
                            conquering phase, we note that BS(total) = BS(left)+BS(right) (eq1); PS(total) = max(PS(left),BS(left) + PS(right)) (eq2); 
                            SS(total) = max(SS(right),SS(left) + BS(right)) (eq3); MSS(total) = max(MSS(left),MSS(right),SS(left)+PS(right)) (eq4).
    ARG - int bIndex, int eIndex
    RET - SumsType
*/
SumsType BloomBrain::computeMaxSubArrSum(int bIndex,int eIndex)
{
    if(bIndex == eIndex)                                                            //termination condition reached on array of size 1
    {
        SumsType retValue;                                                            //var to store the return value
        retValue.prefixSum = retValue.suffixSum = rooms[bIndex].seraphicNum;        //Here, retValue has all sums{BS,PS,SS,MSS} = arr[bIndex] = rooms[bIndex].seraphicNum
        retValue.blockSum = retValue.maxSubArrSum = rooms[bIndex].seraphicNum;
        return retValue;
    }
    else
    {
        int mIndex = (bIndex+eIndex)/2;                                                                                            //boundary index mIndex is computed
        SumsType leftSums = computeMaxSubArrSum(bIndex,mIndex);                                                                    //left segment is solved
        SumsType rightSums = computeMaxSubArrSum(mIndex+1,eIndex);                                                                //right segment is solved
        SumsType retValue;                                                                                                        //var to store the return value
        retValue.blockSum = leftSums.blockSum + rightSums.blockSum;                                                                //eq1
        retValue.prefixSum = max(leftSums.prefixSum, leftSums.blockSum + rightSums.prefixSum);                                    //eq2
        retValue.suffixSum = max(rightSums.suffixSum, leftSums.suffixSum + rightSums.blockSum);                                    //eq3
        retValue.maxSubArrSum = max(leftSums.maxSubArrSum, rightSums.maxSubArrSum, leftSums.suffixSum + rightSums.prefixSum);    //eq4
        return retValue;                                                                                                        //retValue (having MSS in it) is returned
    }
}
/* End {Implementation of class BloomBrain} */




/* BloomBrainOperations() - perform 'queryCount' no of operations (update,join,leave,occupied and play) on passed object obj which has 'roomCount' no of rooms
   ARG - BloomBrain& obj, int queryCount, size_t roomCount
   RET - void
*/
void BloomBrainOperations(BloomBrain& obj,int queryCount,size_t roomCount)
{
    string query;                                                    //to store query fetched from stdin
    int* tempArr = new int[roomCount];                                //array to store input array for update operation
    int tempArg;                                                    //var to store idx or s for leave and join operations resp.
    int updateNo = 0;                                                //counter to count the number of update operations done
    for(int i=0;i<queryCount;i++)
    {
        cin >> query;
        if(query == "update")                                        //update operation
        {
            ++updateNo;                                                //update No is inc by 1 for each update
            for(size_t i = 0;i<roomCount;i++)                        //entries of tempArr are fetched from stdin
                cin >> tempArr[i];
            obj.update(tempArr,updateNo);                            //seraphic numbers of members are updated
        }
        else if(query == "join")                                    //join s operation
        {
            cin >> tempArg ;                                        //s is scanned
            obj.join(tempArg);
        }
        else if(query == "leave")                                    //leave idx operation
        {
            cin >> tempArg;                                            //idx is scanned
            obj.leave(tempArg);
        }
        else if(query == "occupied")                                //occupied operation
        {
            obj.occupied();
        }
        else if(query == "play")                                    //play operation
        {
            obj.play();
        }
        else                                                         //illegal operation
        {
            exit(-1);
        }
    }
    delete[] tempArr;                                                //space allocated to tempArr is freed
    return;
}

int main() 
{
    size_t roomCount;                                                    //no of rooms in the institute
    int queryCount;                                                        //no of queries from usr
    cin >> roomCount;                                                    //roomCount and queryCount fetched from stdin
    cin >> queryCount;
    BloomBrain MumbaiCenter(roomCount);                                    //Mumbai center is initialized with "roomCount" rooms
    BloomBrainOperations(MumbaiCenter,queryCount,roomCount);            //function to perform the operations as in documentation
    return 0;
}
