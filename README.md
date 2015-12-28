# CS240assemblysudokusolver
Final Project for CS240 Computer Organization at Bryn Mawr College, partner: Jordan Henck
LC-3 Assembly. Takes in an empty Sudoku board (with 0's representing empty spots), and uses backtracking algorithm to solve the board 
and return a completed sudoku board. If you entered in a wrong number, or a completed board can not be made, it will return the board
as you typed it.

The backtracking algorithm essentially brute-forces each possible number which could go in an empty spot. After checking if a number is 
in that row and that column, it inserts the first possible option, then moves on to the next 0. If it encounters something that shows 
it has made a mistake, it will "backtrack" and remove what it has put in up to the spot where there won't be a mistake. (Hope that
makes sense. You can Google backtracking algorithm to learn more.)
