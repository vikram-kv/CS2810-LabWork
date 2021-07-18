/************************
* $ID$
* File: CS19B021_A10.cpp - Develop a miniaturized version of the mine sweeper game with the description given below.
*
* Description:-
*       The game is played on an N x N grid of 'cells', each of which is either empty or contains an explosive mine. 
* The player's task is to locate all of the M mines without causing any of them to explode. The location of the mines 
* however, is a secret to the player. At the beginning of the game the player has no information as to whether any 
* given cell contains a mine or not. The only course of action is to 'reveal' the contents of a chosen cell. Should 
* the player choose to reveal a mine, the game is over and the player loses.
* However, if an empty cell is revealed, two useful things will happen.
*   - The player is shown how many of the revealed cell's eight neighbours contain mines.
*   - If the chosen cell has no neighbours which are mines, all of its neighbours are 'revealed'.
* 
*       We design a minesweeper game given the values of N (the size of the grid) and M (the number of mines) 
* and the locations of all the mines. Once these values are given, the board is set. Now the player begins 
* to play, and the positions where he chooses to click are given.
* For each 'click', we print one of the following:
*   - "Won" if all the cells except the mines have been revealed.
*   - "Lost" if a mine has been revealed.
*   - A number "x" where x is the number of cells that have been revealed up to this point which are not mines.
*
*       Once Won/Lost has been printed, the further locations do not matter and are ignored.
*
* Author: K V VIKRAM
*
* Created: [2021-04-26 10:13] by vikram
* 
* Last Modified: [2021-04-27 17:37] by vikram
*
* Input Format:
*   Line 1 - values of N and M separated by a space.
*   Line 2 - contains M values, which indicate the location of the mines assuming the 
*            NxN grid is numbered from 0 to from left to right, top to bottom.
*   Subsequent Lines - indicate the positions (in the form of coordinates) 
*                      the player has chosen to reveal.
*
* Constraints:
*   1 <= N <= 100
*   0 <= M < N*N 
*
* Output Format:
*   Result for each pair of co-ordinates the player has chosen to reveal in a newline
*   where result could be "Won" or "Lost" or a number.
*
* Assumption: Sufficient dynamic memory is assumed to be available. This exception is neither caught nor handled.
*
* Practice: Array iterators in loops are typically named i or j. Specifically, for matrices , i is used for rowIndex and j for colIndex.
*
* Compilation: g++ CS19B021_A10.cpp -o solution, ./solution.
*
* Bugs: No major bugfixes or new releases
*************/

#include <iostream>
using namespace std;

/*  Cell - Represents a unit square of the mine field
    Data Members - mFlag, rFlag
*/
struct Cell
{
    bool mFlag;                         //is set to true iff the cell is mined
    bool rFlag;                         //is set to true iff the cell has been revealed
    Cell()                              //constructor to init both the flags as false
    {
        mFlag = rFlag = false;
    }
};

/*Begin {Definition of class MineSweeper} */
/*  MineSweeper - class of minesweeper games. Stores a grid of cells in which some cells
                  are mined, allows the user to click and reveal a selected cell and displays
                  the result of each click in the format given in header.
    Data Members - mineField, dimension, mineCount, revealed, gameOver
    Private Member function - Process(), PlaceMine(), ValidIndices()
    Public Member Functions - Play()
    Constructor - MineSweeper()
    Destructor - ~MineSweeper()
*/
class MineSweeper
{
    Cell** mineField;                           //2D matrix of grid cells representing the mine field
    int dimension;                              //dimension of the mine field grid
    int mineCount;                              //no of mines in the mine field
    int revealed;                               //no of revealed cells in the mine field
    bool gameOver;                              //flag to indicate if game is over
    void Process(int,int);                      //func to "process" a cell
    bool ValidIndices(int,int);                 //checks whether a pair of indices is in the mine field
    void PlaceMine(int);                        //places a mine at a given position
public:
    MineSweeper(int,int);                       //constructor
    void Play(int,int);                         //func to "click" a particular cell and produce output for the click
    ~MineSweeper();                             //destructor
};
/*End {Definition of class MineSweeper} */

/*Begin {Implementation of class MineSweeper} */
/*  Process() : func to "process" the cell at (i,j), i.e, if the cell hasnt been revealed, we reveal it and
                in case, the cell has 0 mines in its neighbourhood, we "process" each cell in the neighbourhood
                recursively. Neighbourhood of a cell X is the set of cells with which X shares either a side or
                a corner.
    Args : int i - row index, int j - col index
    Ret : void
*/
void MineSweeper::Process(int i,int j)
{
    if(!mineField[i][j].rFlag)                      //if cell (i,j) has not been revealed
    {
        mineField[i][j].rFlag = true;               //cell (i,j) is revealed and 'revealed'
        ++revealed;                                 //member is incremented
        int mineNbrs = 0;                           //no of neighbours of (i,j) with mines. init to 0
        //Nested loops to scan (i,j)'s neighbourhood for mines and
        //determine mineNbrs. A validity check must be done on each
        //pair (a,b) and (a,b) must not equal (i,j)
        for(int a = i-1;a<=i+1;++a)
        {
            for(int b = j-1;b<=j+1;++b)
            {
                if(ValidIndices(a,b) && (a != i || b != j) && mineField[a][b].mFlag == true)
                    ++mineNbrs;
            }
        }
        if(mineNbrs == 0)                           //if (i,j) has 0 mines in its neighborhood
        {
            //Nested loops to reveal each unrevealed cell in (i,j)'s
            //neighbourhood. A validity check must be done on each
            //pair (a,b) before processing (a,b)
            for(int a = i-1;a<=i+1;++a)
            {
                for(int b = j-1;b<=j+1;++b)
                {
                    if(ValidIndices(a,b))
                        Process(a,b);
                }
            }
        }
    }
}

/*  ValidIndices() : Check whether the pair of indices (i,j) is a valid pair(i.e, lies within the grid)
    Args : int i - row index, int j - col index
    Ret : void
*/
bool MineSweeper::ValidIndices(int i,int j)
{
    return (0 <= i && i <= dimension-1 && 0 <= j && j <= dimension - 1);
}

/*  PlaceMine() : Places a mine at a given position 'minePos'. minePos will be in row-maj order 
                  with indexing starting at 0. So we find the indices (i,j) corr. to minePos 
                  and set mFlag of (i,j) to true.
    Args : int minePos - position at which a mine should be placed
    Ret : void
*/
void MineSweeper::PlaceMine(int minePos)
{
    int i,j;                    
    i = minePos/dimension;                          //row index = floor(position/dimension)
    j = minePos%dimension;                          //col index = (position%dimension)
    mineField[i][j].mFlag = true;                   //a mine is placed at (i,j)
}

/*  MineSweeper() : allocates mem for the grid using 'arg1' as dimension. Then accepts the positions of
                    'arg2' many mines and places them in the grid. Inits other data members appropriately 
    Args : int N - grid dimension, int M - no of mines
*/
MineSweeper::MineSweeper(int N,int M)
{
    dimension = N;                                  //dimension - N
    mineCount = M;                                  //no of mines - M
    //Stmts to make mineField an N*N grid of empty cells
    mineField = new Cell*[dimension];
    for(int i=0;i<dimension;++i)
        mineField[i] = new Cell[dimension]{};
    int minePos;                                    //position of a mine
    //Loop to get each mine's position and place it on the grid
    for(int i=0;i<mineCount;++i)
    {
        cin >> minePos;
        PlaceMine(minePos);
    }
    gameOver = false;                               //game is not over
    revealed = 0;                                   //no cells have been revealed
}

/*  Play() : func to reveal the cell at (i,j) and print "Won" if all cells except mines are revealed,
             print "Lost" if (i,j) cell has a mine, print the total number of cells revealed after (i,j)
             has been "clicked" if game is not over yet. If game is already over, the func just returns
    Args : int i - row index of clicked cell, int j - col index of clicked cell
    Ret : void
*/
void MineSweeper::Play(int i,int j)
{
    if(gameOver)                                            //if game is over, we just return
        return;
    else if(mineField[i][j].mFlag)                          //if (i,j) has a mine
    {    
        cout << "Lost" << endl;                             //player has lost and game is over
        gameOver = true;
    }
    else
    {//Here, (i,j) does not have a mine
        Process(i,j);                                       //(i,j) is processed
        if(revealed == dimension*dimension - mineCount)     //if all cells except mines have been revealed
        {
            cout << "Won" << endl;                          //player has won and game is over
            gameOver = true;
        }
        else                                                //here, game is not over
            cout << revealed << endl;                       //total no of revealed cells is printed
    }
}

/*  ~MineSweeper() : de-allocates all dynamic mem used to store mineField
    Args : void
*/
MineSweeper::~MineSweeper()
{
    for(int i=0;i<dimension;++i)
        delete[] mineField[i];
    delete[] mineField;
}
/*End {Implementation of class MineSweeper} */

int main()
{
    int N,M;                            //N - size of grid, M - no of mines
    cin >> N >> M;                      //N and M are inputted
    MineSweeper game1{N,M};             //game1 constructed - N*N grid with M mines
    int i,j;                            //i - row index, j - col index of cell to reveal
    //cin >> i >> j will be cast as true iff 2 integers have been read successfully
    while(cin >> i >> j)
        game1.Play(i,j);                //(i,j) is "clicked"
    return 0;
}