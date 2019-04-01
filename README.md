# sudoku-solver
Written by Andrew Stange. 4/1/19

A brute force Sudoku solver for 9x9 puzzles.  Written as a personal side project while learning C++.  Uses a linked list, basic OOP, and 2-dimensional arrays.  A sample puzzle has been provided.


 Basic algorithm:
    Once file has been read in and converted into a 2-D array, program finds the first open space, generates an array of potential values to fill the space by examining the row/column/box of the open space.
    Then, the program selects the first potential value, inserts it, and moves to next open space.
    When a contradiction is found and there are no potential values for an open space, then the program goes back to the last space that was filled and tries the next potential value from the array.
    This continues until all open spaces can be filled or until all possible fills have been attempted.
 
 
 Thoughts on future edits:
    Program compiles/runs as it, but in the future I would like to accomodate larger puzzles and improve speed/efficiency.
    Some easy fixes to improve speed could include avoiding copying the 2-D arrays every time a new Puzzle object is created.
    A potential solution is to have one master version of the puzzle with each empty space  represented by its own object. Each space would have member variables for value, column, row, and box number.  
    The later 3 variables will allow for much faster updating of the puzzle following a contradiction or the selection of a value.
    Another more drastic approach would be to rewrite the program in a manner that looks at the problem as a whole rather than one open space at a time.  
    This should allow the program to be shortened significantly and would improve overall performance.  
    Although more difficult, this approach should be acheivable once the solution in the line above is completed.  
    Once each open space has easily accessible column/row/box values, comparing entire columns/rows/boxes to find missing values will be significantly easier.
 
 
 Input file format:
    Separate numbers with a space.  Each line of the problem gets its own line in the input file.  Empty spaces are represented by 0s.
    A sample input file has been included in this Github repository.
 
