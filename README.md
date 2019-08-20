# sudoku-solver
Sudoku solver using C++17

My wife's grandfather, now 96 years old, takes to solving Sudoku puzzles with enthusiasm and looks forward to the daily newspaper. He also competes with his daughter, also 65+ currently. I caught on the bug and took on to Sudoku too. Being a game based on deduction, every time I played the game, I thought I can have this solved programmatically. Sorry, but we programmers bring in programming even into our games! It was fun to use the newer C++17 constructs and see how the program figures out cell by cell the numbers that should go into the cells filling up the entire board. 

Sudoku is a compelling game... You just get hooked on to it. If you wish to learn more about it, please see www.sudoku.com 

## Input
Provide input on the console with the no of filled up cells as the first number followed by triplets of zero-based x-coordinate, y-coordinate and number to be filled for the starting cells. 

```
26

0 0 8

0 7 5

1 4 2

1 5 5

1 6 7

1 7 9

2 2 2

2 4 4

2 6 6

2 8 8

3 3 5

3 5 7

4 2 3

4 6 4

5 3 4

5 5 9

6 0 7

6 2 5

6 4 9

6 6 2

7 1 9

7 2 4

7 3 6

7 4 1

8 1 8

8 8 3
```

Here, the first line has the no of filled cells at starting position of the board. 
This is followed by 26 lines of <x-coordinate> <y-coordinate> <value>
  i.e. the first line 0 0 8 means the first cell of the board has 8; in terms of coordinates, board[0][0] = 8
  similarly, the last line 8 8 3 means the last cell of the board has 3, i.e. board[8][8] = 3

Alternatively, you may provide all the input in a single line. 
The above input would look like this: 

```
26
0 0 8   0 7 5   1 4 2   1 5 5   1 6 7   1 7 9   2 2 2   2 4 4   2 6 6   2 8 8   3 3 5   3 5 7   4 2 3   4 6 4   5 3 4   5 5 9   6 0 7   6 2 5   6 4 9   6 6 2   7 1 9   7 2 4   7 3 6   7 4 1   8 1 8   8 8 3
```

## Output
The output of the program is a solved board. 

```
-------------------------------------
| 8 | 4 | 9 | 1 | 7 | 6 | 3 | 5 | 2 |
-------------------------------------
| 1 | 3 | 6 | 8 | 2 | 5 | 7 | 9 | 4 |
-------------------------------------
| 5 | 7 | 2 | 9 | 4 | 3 | 6 | 1 | 8 |
-------------------------------------
| 4 | 2 | 8 | 5 | 6 | 7 | 1 | 3 | 9 |
-------------------------------------
| 9 | 5 | 3 | 2 | 8 | 1 | 4 | 7 | 6 |
-------------------------------------
| 6 | 1 | 7 | 4 | 3 | 9 | 8 | 2 | 5 |
-------------------------------------
| 7 | 6 | 5 | 3 | 9 | 8 | 2 | 4 | 1 |
-------------------------------------
| 3 | 9 | 4 | 6 | 1 | 2 | 5 | 8 | 7 |
-------------------------------------
| 2 | 8 | 1 | 7 | 5 | 4 | 9 | 6 | 3 |
-------------------------------------
```

## Feedback
Let me know if you have feedback: https://ankursatle.wordpress.com/contact/
