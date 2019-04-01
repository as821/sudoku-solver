/*
 
 Written by Andrew Stange.  04/01/19.
 
 Basic algorithm:
    Once file has been read in and converted into a 2-D array, program finds the first open space, generates an array of potential values to fill the space by examining the row/column/box of the open space.
    Then, the program selects the first potential value, inserts it, and moves to next open space.
    When a contradiction is found and there are no potential values for an open space, then the program goes back to the last space that was filled and tries the next potential value from the array.
    This continues until all open spaces can be filled or until all possible fills have been attempted.
 
 
 Thoughts on future edits:
    Program compiles/runs as it, but in the future I would like to accomodate larger puzzles and improve speed/efficiency.
    Some easy fixes to improve speed could include avoiding copying the 2-D arrays every time a new Puzzle object is created.
    A potential solution is to have one master version of the puzzle with each empty space  represented by its own object. Each space would have member variables for value, column, row, and box number.  The later 3 variables will allow for much faster updating of the puzzle following a contradiction or the selection of a value.
    Another more drastic approach would be to rewrite the program in a manner that looks at the problem as a whole rather than one open space at a time.  This should allow the program to be shortened significantly and would improve overall performance.  Although more difficult, this approach should be acheivable once the solution in the line above is completed.  Once each open space has easily accessible column/row/box values, comparing entire columns/rows/boxes to find missing values will be significantly easier.
 
 
 Input file format:
    Separate numbers with a space.  Each line of the problem gets its own line in the input file.  Empty spaces are represented by 0s.
    A sample input file has been included in this Github repository.
 
 */




#include <iostream>
#include <fstream>
#include <string>
#include <time.h>

using namespace std;



//Puzzle class declaration
class Puzzle {
private:
    int** puzzle;
    int** oldPuzzle;
    
    bool containOne;
    bool containTwo;
    bool containThree;
    bool containFour;
    bool containFive;
    bool containSix;
    bool containSeven;
    bool containEight;
    bool containNine;
    int numOptions;
    
    Puzzle* previous;
    
    static int icount;
    static int spaceCount;
    
public:
    void input();
    void reset(int** puzzle, int** &oldPuzzle);
    void testFormat();
    void largerTest(bool &repetition, bool &fillSuccess, bool &deadEnd);
    void testing(int i, int j, bool &deadEnd);
    bool compare();
    void inference(bool &fillSuccess, bool &deadEnd);
    void optionTesting(int i, int j);
    void output();
    bool fullTable();
    
    //default constructor
    Puzzle();
    
    //copy constructor
    Puzzle(Puzzle &object);
    
    //destructor
    ~Puzzle();
};




//static member definitions
int Puzzle::icount = 0;
int Puzzle::spaceCount = 0;

//member function defintions
//input() definition
void Puzzle::input() {
    //initialize spaceCount
    Puzzle::spaceCount = 0;

    //open file and insert values into array
    ifstream inFile;
    string fileName;
    
    cout << "input filename" << endl;
    cin >> fileName;
    
    //open file
    inFile.open(fileName.c_str());
    
    //insert values to array
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            inFile >> this->puzzle[i][j];
            if (this->puzzle[i][j] == 0) {
                Puzzle::spaceCount++;
            }
        }
    }
    //close file
    inFile.close();
    
    //set puzzle and oldPuzzle equal
    this->reset(this->puzzle, this->oldPuzzle);
}   //end input() definition




//reset() function definition
void Puzzle::reset(int** puzzle, int** &oldPuzzle) {
    //set puzzle and oldPuzzle equal
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            oldPuzzle[i][j] = puzzle[i][j];
        }
    }
} //end reset function




//testingFormat() function definition
void Puzzle::testFormat() {
    
    bool repetition = false;
    bool fillSuccess = false;
    bool deadEnd = false;
    
    while(Puzzle::spaceCount > 0) {
        //contains basic while loop and check to ensure no repetition
        this->largerTest(repetition, fillSuccess, deadEnd);
        
        if (repetition == true) {
            repetition = false; //reset repetition
            this->inference(fillSuccess, deadEnd);
            
            if (this->fullTable()) {
                fillSuccess = true;
                break;
            }
        }
    }
} //end of testingFormat() definition




//largerTesting() definition
void Puzzle::largerTest(bool &repetition, bool &fillSuccess, bool &deadEnd) {
    //while loop to continue to test problem until no empty spaces exist
    while(Puzzle::spaceCount > 0) {
        //move through sudoku problem and examine rows, columns, and boxes to add values
        for (int i = 0; i < 9; i++) {
            for (int j = 0; j < 9; j++) {
                if (this->puzzle[i][j] == 0) {
                    this->testing(i, j, deadEnd);
                }
                if (deadEnd == true) {
                    return;
                }
            }
        }
        
        //ensure that the program is not repeating itself.  If testing() runs and no values are added, then inference() is needed
        if (this->compare() == true) {
            repetition = true;
            break;
        }
        
        this->reset(puzzle, oldPuzzle);   //reset oldPuzzle so it can be compared to any new changes made to puzzle
    }   //end while loop
} //end of largerTesting() definition




//testing() definition
void Puzzle::testing(int i, int j, bool &deadEnd)
{
    //variables
    int c_box1 = 0;
    int c_box2 = 0;
    
    int r_box1 = 0;
    int r_box2 = 0;
    
    int fillVal = 0;
    int fillCount = 0;
    
    bool one = false;
    bool two = false;
    bool three = false;
    bool four = false;
    bool five = false;
    bool six = false;
    bool seven = false;
    bool eight = false;
    bool nine = false;
    
    
    
    //real work
    //column check
    for (int k = 0; k < 9; k++) {
        if (puzzle[k][j] == 1)
            one = true;
        else if (puzzle[k][j] == 2)
            two = true;
        else if (puzzle[k][j] == 3)
            three = true;
        else if (puzzle[k][j] == 4)
            four = true;
        else if (puzzle[k][j] == 5)
            five = true;
        else if (puzzle[k][j] == 6)
            six = true;
        else if (puzzle[k][j] == 7)
            seven = true;
        else if (puzzle[k][j] == 8)
            eight = true;
        else if (puzzle[k][j] == 9)
            nine = true;
    }
    
    
    //row check
    for (int w = 0; w < 9; w++)
    {
        if (puzzle[i][w] == 1)
            one = true;
        else if (puzzle[i][w] == 2)
            two = true;
        else if (puzzle[i][w] == 3)
            three = true;
        else if (puzzle[i][w] == 4)
            four = true;
        else if (puzzle[i][w] == 5)
            five = true;
        else if (puzzle[i][w] == 6)
            six = true;
        else if (puzzle[i][w] == 7)
            seven = true;
        else if (puzzle[i][w] == 8)
            eight = true;
        else if (puzzle[i][w] == 9)
            nine = true;
    }
    
    //find box
    if (j <= 2) {
        c_box1 = 0;
        c_box2 = 2;
    }
    else if(j <= 5) {
        c_box1 = 3;
        c_box2 = 5;
    }
    else {
        c_box1 = 6;
        c_box2 = 8;
    }
    
    
    if (i <= 2) {
        r_box1 = 0;
        r_box2 = 2;
    }
    else if(i <= 5) {
        r_box1 = 3;
        r_box2 = 5;
    }
    else {
        r_box1 = 6;
        r_box2 = 8;
    }
    
    
    //box check
    for (int w = c_box1; w <= c_box2; w++) {
        for (int k = r_box1; k <= r_box2; k++) {
            if (puzzle[k][w] == 1)
                one = true;
            else if (puzzle[k][w] == 2)
                two = true;
            else if (puzzle[k][w] == 3)
                three = true;
            else if (puzzle[k][w] == 4)
                four = true;
            else if (puzzle[k][w] == 5)
                five = true;
            else if (puzzle[k][w] == 6)
                six = true;
            else if (puzzle[k][w] == 7)
                seven = true;
            else if (puzzle[k][w] == 8)
                eight = true;
            else if (puzzle[k][w] == 9)
                nine = true;
        }
    }
    
    //filling of space
    if (one == false) {
        fillVal = 1;
        fillCount++;
    }
    
    if (two == false) {
        fillVal = 2;
        fillCount++;
    }
    
    if (three == false) {
        fillVal = 3;
        fillCount++;
    }
    
    if (four == false) {
        fillVal = 4;
        fillCount++;
    }
    
    if (five == false) {
        fillVal = 5;
        fillCount++;
    }
    
    if (six == false) {
        fillVal = 6;
        fillCount++;
    }
    
    if (seven == false) {
        fillVal = 7;
        fillCount++;
    }
    
    if (eight == false) {
        fillVal = 8;
        fillCount++;
    }
    
    if (nine == false) {
        fillVal = 9;
        fillCount++;
    }
    
    
    //fill space
    if(fillCount == 1) {
        puzzle[i][j] = fillVal;
        Puzzle::spaceCount--;
    }
    else {
        if (fillCount == 0) {
            //no fill options.  Dead end found
            deadEnd = true;
            return;
        }
    }
}   //end testing() defintion




//inference() definition
void Puzzle::inference(bool &fillSuccess, bool &deadEnd) {
    Puzzle::icount++;
    
    //create an array to contain options for selected space
    int options[numOptions];
    
    //ints to hold the location of the first selected space
    int col = 0;
    int row = 0;
    
    //var for largerTest call
    bool repetition = 0;
    bool complete = false;
    
    //find a space and record fill options
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            if(puzzle[i][j] == 0) {
                this->optionTesting(i, j);
                
                //initialize count to hold subscript of array
                int subscriptCount = 0;
                
                //fill array with options
                if (containOne) {
                    options[subscriptCount] = 1;
                    subscriptCount++;
                    containOne = false;
                }
                
                if (containTwo) {
                    options[subscriptCount] = 2;
                    subscriptCount++;
                    containTwo = false;
                }
                
                if (containThree) {
                    options[subscriptCount] = 3;
                    subscriptCount++;
                    containThree = false;
                }
                
                if (containFour) {
                    options[subscriptCount] = 4;
                    subscriptCount++;
                    containFour = false;
                }
                
                if (containFive) {
                    options[subscriptCount] = 5;
                    subscriptCount++;
                    containFive = false;
                }
                
                if (containSix) {
                    options[subscriptCount] = 6;
                    subscriptCount++;
                    containSix = false;
                }
                
                if (containSeven) {
                    options[subscriptCount] = 7;
                    subscriptCount++;
                    containSeven = false;
                }
                
                if (containEight) {
                    options[subscriptCount] = 8;
                    subscriptCount++;
                    containEight = false;
                }
                
                if (containNine) {
                    options[subscriptCount] = 9;
                    subscriptCount++;
                    containNine = false;
                }
                
                //save row and column location of selected space
                col = j;
                row = i;
                
                //space has been found and used
                complete = true;
            }   //end of if statement
            
            //if a space has been found, then break inner loop
            if (complete == true) {
                break;
            }
            
        }   //end of nested for loop
        
        //if a space has been found, then break outer loop
        if (complete == true) {
            break;
        }
    }   //end of outer for loop
    
    
    
    //reset numOptions for following iterations
    int thisNumOptions = this->numOptions;
    this->numOptions = 0;
    
    
    int initialSpace = Puzzle::spaceCount;
    
    //split off possibilities for selected space
    for (int k = 0; k < thisNumOptions; k++) {
        //reset deadEnd fromprevious iterations
        deadEnd = false;
        
        //declare new instance of the class using copy constructor
        Puzzle newObject = *this;
        
        //only change between puzzle and newPuzzle is value "k".  Will change with each iteration
        newObject.puzzle[row][col] = options[k];        //must alter the array before here
        Puzzle::spaceCount--;
        
        //have newObject.previous point to the object "above" it that called this inference()
        newObject.previous = this;
        
        //set oldPuzzle = puzzle
        newObject.reset(newObject.puzzle, newObject.oldPuzzle);
        
        
        //while loop same as in main()
        while(Puzzle::spaceCount > 0) {
            //ensure no infinite loop to a deadend
            if (Puzzle::icount < Puzzle::spaceCount) {
                newObject.largerTest(repetition, fillSuccess, deadEnd);
                if (repetition == true) {
                    //if a solution has been found
                    if (newObject.fullTable()) {
                        fillSuccess = true;
                        newObject.reset(newObject.puzzle, newObject.previous->puzzle);   //set puzzle of node before calling object with the solution values newObject.puzzle
                        return;
                    }
                    
                    newObject.inference(fillSuccess, deadEnd);
                }
            }
            
            else {
                //deallocate memory
                newObject.~Puzzle();
                Puzzle::icount = 0;
                
                //reset spaceCount
                Puzzle::spaceCount = initialSpace;
                break;  //this k value is a deadend.  Now move to next iteration of "k" loop
            }
            
            //if a dead end exists, delete this path and try next k value
            if (deadEnd == true) {
                Puzzle::spaceCount = initialSpace;
                
                //deallocate memory
                newObject.~Puzzle();
                break;
            }
            
            //to catch inference statements below this level that are returning
            if (newObject.fullTable()) {
                fillSuccess = true;
                newObject.reset(newObject.puzzle, newObject.previous->puzzle);   //set puzzle of node before calling object with the solution values newObject.puzzle
                return;
            }
        }
    }
}   //end of inference function




//compare() definition
bool Puzzle::compare()
{
    bool same = true;
    //insert values to array
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            if (this->puzzle[i][j] != this->oldPuzzle[i][j]) {
                same = false;
                break;
            }
        }
        
        if (same == false) {
            break;
        }
    }
    return same;
}   //end of compare() definition




//optionTesting() definition
void Puzzle::optionTesting(int i, int j) {
    //variables
    int c_box1 = 0;
    int c_box2 = 0;
    
    int r_box1 = 0;
    int r_box2 = 0;
    
    
    bool one = false;
    bool two = false;
    bool three = false;
    bool four = false;
    bool five = false;
    bool six = false;
    bool seven = false;
    bool eight = false;
    bool nine = false;
    
    
    //real work
    //column check
    for (int k = 0; k < 9; k++) {
        if (puzzle[k][j] == 1)
            one = true;
        else if (puzzle[k][j] == 2)
            two = true;
        else if (puzzle[k][j] == 3)
            three = true;
        else if (puzzle[k][j] == 4)
            four = true;
        else if (puzzle[k][j] == 5)
            five = true;
        else if (puzzle[k][j] == 6)
            six = true;
        else if (puzzle[k][j] == 7)
            seven = true;
        else if (puzzle[k][j] == 8)
            eight = true;
        else if (puzzle[k][j] == 9)
            nine = true;
    }
    
    
    //row check
    for (int w = 0; w < 9; w++) {
        if (puzzle[i][w] == 1)
            one = true;
        else if (puzzle[i][w] == 2)
            two = true;
        else if (puzzle[i][w] == 3)
            three = true;
        else if (puzzle[i][w] == 4)
            four = true;
        else if (puzzle[i][w] == 5)
            five = true;
        else if (puzzle[i][w] == 6)
            six = true;
        else if (puzzle[i][w] == 7)
            seven = true;
        else if (puzzle[i][w] == 8)
            eight = true;
        else if (puzzle[i][w] == 9)
            nine = true;
    }
    
    //find box
    if (j <= 2) {
        c_box1 = 0;
        c_box2 = 2;
    }
    else if(j <= 5) {
        c_box1 = 3;
        c_box2 = 5;
    }
    else {
        c_box1 = 6;
        c_box2 = 8;
    }
    
    
    if (i <= 2) {
        r_box1 = 0;
        r_box2 = 2;
    }
    else if(i <= 5) {
        r_box1 = 3;
        r_box2 = 5;
    }
    else {
        r_box1 = 6;
        r_box2 = 8;
    }
    
    
    //box check
    for (int w = c_box1; w <= c_box2; w++) {
        for (int k = r_box1; k <= r_box2; k++) {
            if (puzzle[k][w] == 1)
                one = true;
            else if (puzzle[k][w] == 2)
                two = true;
            else if (puzzle[k][w] == 3)
                three = true;
            else if (puzzle[k][w] == 4)
                four = true;
            else if (puzzle[k][w] == 5)
                five = true;
            else if (puzzle[k][w] == 6)
                six = true;
            else if (puzzle[k][w] == 7)
                seven = true;
            else if (puzzle[k][w] == 8)
                eight = true;
            else if (puzzle[k][w] == 9)
                nine = true;
        }
    }
    
    //find possible fills
    if (one == false) {
        containOne = true;
        numOptions++;
    }
    
    if (two == false) {
        containTwo = true;
        numOptions++;
    }
    
    if (three == false) {
        containThree = true;
        numOptions++;
    }
    
    if (four == false) {
        containFour = true;
        numOptions++;
    }
    
    if (five == false) {
        containFive = true;
        numOptions++;
    }
    
    if (six == false) {
        containSix = true;
        numOptions++;
    }
    
    if (seven == false) {
        containSeven = true;
        numOptions++;
    }
    
    if (eight == false) {
        containEight = true;
        numOptions++;
    }
    
    if (nine == false) {
        containNine = true;
        numOptions++;
    }
}   //end optionTesting function




//output() definition
void Puzzle::output() {
    int count = 0;
    
    for(int i = 0; i < 9; i++) {
        for(int j = 0; j < 9; j++) {
            count++;
            cout << this->puzzle[i][j] << ' ';
            if(count % 9 == 0)
                cout << endl;
        }
    }
} //end output() definition



//fullTable() defintion
bool Puzzle::fullTable() {
    bool full = true;
    
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            //searching for an open space, represented by a 0
            if (this->puzzle[i][j] == 0)
                full = false;
        }
    }
    return full;
}




//default constructor
Puzzle::Puzzle() {
    //format puzzle as a 2 dimensional array
    this->puzzle = new int *[9];
    for (int i = 0; i < 9; i++) {
        this->puzzle[i] = new int [9];
    }
    
    //format oldPuzzle as a 2 dimensional array
    this->oldPuzzle = new int *[9];
    for (int i = 0; i < 9; i++) {
        this->oldPuzzle[i] = new int [9];
    }
    
    this->containOne = false;
    this->containTwo = false;
    this->containThree = false;
    this->containFour = false;
    this->containFive = false;
    this->containSix = false;
    this->containSeven = false;
    this->containEight = false;
    this->containNine = false;
    this->numOptions = 0;
    
    this->previous = NULL;
}   //end default constructor defintion




//copy constructor definition
Puzzle::Puzzle(Puzzle &object) {
    //format puzzle as a 2 dimensional array
    this->puzzle = new int *[9];
    for (int i = 0; i < 9; i++) {
        this->puzzle[i] = new int [9];
    }
    
    //initialize with object values
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            this->puzzle[i][j] = object.puzzle[i][j];
        }
    }
    
    //format oldPuzzle as a 2 dimensional array
    this->oldPuzzle = new int *[9];
    for (int i = 0; i < 9; i++) {
        this->oldPuzzle[i] = new int [9];
    }
    
    //initialize with object values
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            this->oldPuzzle[i][j] = object.oldPuzzle[i][j];
        }
    }
    
    
    this->containOne = object.containOne;
    this->containTwo = object.containTwo;
    this->containThree = object.containThree;
    this->containFour = object.containFour;
    this->containFive = object.containFive;
    this->containSix = object.containSix;
    this->containSeven = object.containSeven;
    this->containEight = object.containEight;
    this->containNine = object.containNine;
    this->numOptions = object.numOptions;
    
    this->previous = object.previous;
}




//destructor definition
Puzzle::~Puzzle() {
    //delete puzzle
    for (int i = 0; i < 9; i++) {
        this->puzzle[i] = NULL;
    }
    
    //delete oldPuzzle
    for (int i = 0; i < 9; i++) {
        this->oldPuzzle[i] = NULL;
    }
    
    
    //delete previous
    this->previous = NULL;
    delete this->previous;
}




//main function
int main() {
    clock_t tStart = clock();
    
    //create Puzzle class instance
    Puzzle obj;
    
    //collect values from a file
    obj.input();
    
    //test for solutions
    obj.testFormat();
    
    cout << "Time taken: " << (double)(clock() - tStart)/CLOCKS_PER_SEC << endl;
    
    //output solution
    cout << "\nsolution is:\n" << endl;
    obj.output();
    
    cout << endl;
    
    return(0);
}
