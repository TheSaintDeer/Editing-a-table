Commands for changing the selection
The selection of cells can exceed the boundaries of the input table. 
In this case, the table will be expanded with the required number of rows and columns containing empty cells until the current selection.

[R, C] - cell selection on row R and column C.
[R, _] - selection of the whole line R.
[_, C] - selection of the whole column C.
[R1, C1, R2, C2] - selection of the window, ie all cells on row R and column C, for which R1 <= R <= R2, C1 <= C <= C2. If instead of the number R2 resp. 
  C2 will be a hyphen, thus replacing the maximum line resp. column in the table.
[_, _] - select the whole table.
[min] - in an existing cell selection finds a cell with the minimum numeric value and sets the selection to it.
[max] - similar to the previous command, but finds the cell with the maximum value.
[find STR] - in an existing cell selection, selects the first cell whose value contains the substring STR.
[_] - resets the selection from the temporary variable (see below).


Commands for editing the table structure

irow - inserts one blank row above the selected cells.
arow - adds one blank row below the selected cells.
drow - deletes the selected rows.
icol - inserts one empty column to the left of the selected cells.
acol - adds one empty column to the right of the selected cells.
dcol - delete selected columns.
Note inserted rows and columns have an effect beyond the selected cells, adding or removing columns or rows of the entire table.


Commands for editing cell contents

set STR - sets the cell value to the string STR. 
  The string STR can be enclosed in quotation marks and can contain special characters preceded by a slash (see table format)
clear - the content of the selected cells will be deleted (the cells will have empty content)
swap [R, C] - swaps the contents of the selected cell with the cell on row R and column C
sum [R, C] - saves the sum of values of selected cells (corresponding to the format% g in printf) in the cell on row R and column C. 
  Selected cells without a number will be ignored (as if they were not selected).
avg [R, C] - same as sum, but the arithmetic mean of the selected cells is stored
count [R, C] - same as sum, but the number of non-empty cells from the selected cells is stored
len [R, C] - stores the string length of the currently selected cell in the cell on row R and column C.


Commands for working with temporary variables
The spreadsheet allows you to work with 10 so-called temporary variables identified as _0 to _9. 
Temporary variables have an empty value when the program is started. One temporary variable identified as an underscore _ is for cell selection 
(i.e., it remembers which cells were selected).

def _X - the value of the current cell will be set to the temporary variable X (where X can be a value from 0 to 9)
use _X - the current cell will be set to the value from the temporary variable X (where X identifies the temporary variable _0 to _9)
inc _X - the numeric value in the temporary variable will be incremented by 1. 
  If the temporary variable does not contain a number, the resulting value of the variable will be set to 1.
[set] - sets the current cell selection to the temporary variable _ (only which cells are selected, not their contents)


Commands for controlling a sequence of commands
The following commands are optional and are for experimentation, inspiration, and amusement only:

goto + N - skips N of the following commands in the command sequence (note: goto +1 is a command without effect)
goto -N - returns in the sequence of commands by N commands (note: goto -0 is a program loop without effect).
iszero _X + -N - skips (in case of + N) or returns (in case of -N) n commands in the sequence, but only if the value of the temporary variable _X (0 <= X <= 9) is equal to 0 .
sub _X _Y - subtracts the value of the variable _Y from the value in the temporary variable _X. Stores the result in the variable _X.
