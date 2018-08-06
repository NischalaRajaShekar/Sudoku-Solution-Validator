# Sudoku-Solution-Validator
Sudoku is one of the most famous Japanese number logic puzzle of the last couple decades. We all must have encountered a Sudoku puzzle in magazines or newspapers.  Today a software programmer in us is also intrigued by this puzzle, but in a unique way. The natural human way to think for solution is to validate either row by row, or column by column or grid by grid, in a sequential manner. This is somewhat time consuming. Since each row check (or column or grid checks) is completely independent of the other row checks (or columns or grids), we decided to validate all the rows, columns and grids parallelly rather than sequentially, using threads, hoping to make the overall validation of the sudoku faster.

Rules of Sudoku –

•	Each row should have numbers from 1 to 9 only once.
•	Each column should have numbers from 1 to 9 only once.
•	Each grid should have numbers from 1 to 9 only once.
All above conditions should satisfy for the solution to be valid.
