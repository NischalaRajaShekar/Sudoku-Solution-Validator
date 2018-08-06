#include <fstream>
#include <iostream>
#include <cstdlib>
#include <vector>
#include <iomanip>
#include <ctime>
#include <chrono>
#include <unistd.h>
#include <pthread.h>
using namespace std;
using namespace std::chrono;

//displays the sudoku on the console
void printSudoku(vector< vector <int> > sudoku);

//thread calls this function to validate a row
void *validateRow_Col(void* param);

//thread calls this function to validate a grid
void *validateGrid(void* param);

/*this array to be shared by all rows. If row[i] is valid they will update validRows[i] to true*/
bool validRows[9];

/*this array to be shared by all columns. If column[i] is valid they will update validColumns[i] to true*/
bool validColumns[9];

/*This is an array for all grids. If grid[i] is valid they will update validRows[i] to true.*/
bool validGrids[9];

/*Global boolean variable to validate row and column*/
bool row_col = true;

//sudoku grid for all threads to share
vector < vector <int> > sudoku(9,vector<int>(9,0));

/* structure for passing data to threads */
typedef struct
{
int row;
int column;
int grid_number;
} parameters;

int main()
{
ifstream input;
string file_name = "sudoku.in";
steady_clock::time_point start_time = steady_clock::now();
input.open(file_name);
if (input.fail())
{
cout << "Failed to open sudoku file " << endl;
return -1;
}
char nextInput = input.peek();
while(nextInput != EOF)
{
for(int i=0; i < 9 ; i++)
{
for(int j=0; j < 9 ; j++)
{
input >> sudoku[i][j];
}
}
nextInput = input.peek();
}

printSudoku(sudoku);

//Create First 3X3 grid
pthread_t grid1Validator; //thread identifier for grid 1
pthread_attr_t attr; // a set of thread attributes
pthread_attr_init(&attr); //get the default attributes
parameters *grid1data = (parameters *) malloc(sizeof(parameters));
grid1data->row = 0;
grid1data->column = 0;
grid1data->grid_number = 1;
pthread_create(&grid1Validator, &attr, validateGrid, grid1data);


//Create Second 3X3 grid
pthread_t grid2Validator; //thread identifier for grid 1
parameters *grid2data = (parameters *) malloc(sizeof(parameters));
grid2data->row = 0;
grid2data->column = 3;
grid2data->grid_number = 2;
pthread_create(&grid2Validator, &attr, validateGrid, grid2data);


//Create Third 3X3 grid
pthread_t grid3Validator; //thread identifier for grid 1
parameters *grid3data = (parameters *) malloc(sizeof(parameters));
grid3data->row = 0;
grid3data->column = 6;
grid3data->grid_number = 3;
pthread_create(&grid3Validator, &attr, validateGrid, grid3data);


//Create Fourth 3X3 grid
pthread_t grid4Validator; //thread identifier for grid 1
parameters *grid4data = (parameters *) malloc(sizeof(parameters));
grid4data->row = 3;
grid4data->column = 0;
grid4data->grid_number = 4;
pthread_create(&grid4Validator, &attr, validateGrid, grid4data);


//Create Fifth 3X3 grid
pthread_t grid5Validator; //thread identifier for grid 1
parameters *grid5data = (parameters *) malloc(sizeof(parameters));
grid5data->row = 3;
grid5data->column = 3;
grid5data->grid_number = 5;
pthread_create(&grid5Validator, &attr, validateGrid, grid5data);


//Create Sixth 3X3 grid
pthread_t grid6Validator; //thread identifier for grid 1
parameters *grid6data = (parameters *) malloc(sizeof(parameters));
grid6data->row = 3;
grid6data->column = 6;
grid6data->grid_number = 6;
pthread_create(&grid6Validator, &attr, validateGrid, grid6data);


//Create Seventh 3X3 grid
pthread_t grid7Validator; //thread identifier for grid 1
parameters *grid7data = (parameters *) malloc(sizeof(parameters));
grid7data->row = 6;
grid7data->column = 0;
grid7data->grid_number = 7;
pthread_create(&grid7Validator, &attr, validateGrid, grid7data);


//Create Eighth 3X3 grid
pthread_t grid8Validator; //thread identifier for grid 1
parameters *grid8data = (parameters *) malloc(sizeof(parameters));
grid8data->row = 6;
grid8data->column = 3;
grid8data->grid_number = 8;
pthread_create(&grid8Validator, &attr, validateGrid, grid8data);


//Create Ninth 3X3 grid
pthread_t grid9Validator; //thread identifier for grid 1
parameters *grid9data = (parameters *) malloc(sizeof(parameters));
grid9data->row = 6;
grid9data->column = 6;
grid9data->grid_number = 9;
pthread_create(&grid9Validator, &attr, validateGrid, grid9data);

//Wait for all threads to complete execution
pthread_join(grid1Validator,NULL);
pthread_join(grid2Validator,NULL);
pthread_join(grid3Validator,NULL);
pthread_join(grid4Validator,NULL);
pthread_join(grid5Validator,NULL);
pthread_join(grid6Validator,NULL);
pthread_join(grid7Validator,NULL);
pthread_join(grid8Validator,NULL);
pthread_join(grid9Validator,NULL);

//Check if all grids were declared valid
bool invalidGridPresent = false;
for(int grid_count =0; grid_count <9 ; grid_count++)
{
if(!validGrids[grid_count])
invalidGridPresent = true;
}

pthread_t rowValidator[9]; //thread identifier
pthread_attr_t attr1; // a set of thread attributes
pthread_attr_init(&attr1); //get the default attributes

for(int k = 0; k < 9 ; k++)
{
parameters *data = (parameters *) malloc(sizeof(parameters));
data->row = k;
data->column = 0; //column data wont be used as this is row validator. setting to zero.
pthread_create(&rowValidator[k], &attr1, validateRow_Col, data);
}
//waits for the threads to join
for(int l = 0; l < 9; l++)
{
pthread_join(rowValidator[l],NULL);
}
//checks if all rows were declared valid
bool invalidRowPresent = false;
for(int rows =0; rows <9 ; rows++)
{
if(!validRows[rows])
invalidRowPresent = true;
}

//set to false to validate for columns
row_col = false;
pthread_t colValidator[9]; //thread identifier
pthread_attr_t attr2; // a set of thread attributes
pthread_attr_init(&attr2); //get the default attributes

for(int m = 0; m < 9 ; m++)
{
parameters *col_data = (parameters *) malloc(sizeof(parameters));
col_data->row = 0; //row data wont be used as this is column validator. setting to zero.
col_data->column = m;
pthread_create(&colValidator[m], &attr2, validateRow_Col, col_data);
}
//waits for the threads to join
for(int l = 0; l < 9; l++)
{
pthread_join(colValidator[l],NULL);
}
//checks if all columns were declared valid
bool invalidColPresent = false;
for(int columns = 0; columns < 9 ; columns++)
{
if(!validColumns[columns])
invalidColPresent = true;
}

//if no invalid columns were present declare all columns valid
if(!invalidGridPresent && !invalidRowPresent && !invalidColPresent)
{
cout << "Sudoku solution is valid!" << endl;
}
steady_clock::time_point end_time = steady_clock::now();
double time_span = duration_cast<milliseconds>(end_time - start_time).count();
cout << "\nTotal time for validaton is "<< time_span << " milliseconds."<<endl;
return 0;
}

/*Function that is run by the threads that validate a row*/
void *validateRow_Col(void* param)
{
parameters *myParam = (parameters*) param;
//find which row we are checking
int rowNumber = myParam->row;
//find which column we are checking
int colNumber = myParam->column;
//boolean array such that if the number 6 is found in the row, numberFound[6] is set to true
bool numberFound[10];
int currentValue = 0;
//check each column for that row and each row for that column
for(int index = 0; index < 9; index++)
{
if(row_col == true)
{
//get the value in that column
currentValue = sudoku[rowNumber][index];
}
else
{
//get the value in that row
currentValue = sudoku[index][colNumber];
}
//if that value is between 1 and 9, set numberFound[currentValue] as true.
//example if 6 is found set numberFound[6] to true
if(currentValue >=1 && currentValue <=9)
{
numberFound[currentValue] = true;
}
}
//check if all numbers are found
bool error = false;
for(int num = 1; num <=9; num++)
{
if(!numberFound[num])
{
if(row_col == true)
{
cout << "Row " << rowNumber+1 << " is missing " << num << endl;
}
else
{
cout << "Column " << colNumber+1 << " is missing " << num << endl;
}
error = true;
}
}

if(!error)
{
if(row_col == true)
{
//if there are no errors declare row valid; update the valid Row array
validRows[rowNumber] = true;
}
else
{
//if there are no errors declare column valid; update the valid Column array
validColumns[colNumber] = true;
}
}
pthread_exit(0);
}

/*Function that is run by the threads that validate a row*/
void *validateGrid(void* param)
{
parameters *myparam = (parameters*) param;
int startRow = myparam->row;
int startCol = myparam->column;
int grid_id = myparam->grid_number;
bool numberFound[10];
for (int i = startRow; i < startRow + 3; ++i)
{
for (int j = startCol; j < startCol + 3; ++j)
{
int currentValue = sudoku[i][j];
if(currentValue >=1 && currentValue <=9)
{
numberFound[currentValue] = true;
}
}
}

//check if all numbers in a grid are found
bool error = false;
for(int num = 1; num <=9; num++)
{
if(!numberFound[num])
{
cout << "Grid " << grid_id << " is missing " << num << endl;
			error = true;
		}
	}
	//if there are no errors declare a grid valid; update the valid Grid array
	if(!error)
	{
		validGrids[grid_id-1] = true;
	}

	pthread_exit(0);
}

/*Prints the vector that contains the sudoku onto the console*/
void printSudoku(vector< vector <int> > sudoku)
{
	cout << "Sudoku to be validated" << endl << endl;
	for(int i=0; i < 9 ; i++)
	{
		cout << "  ------------------------------------" << endl;
		for(int j=0; j < 9 ; j++)
		{
			cout << setw(2) << " | " << sudoku[i][j] ;
		}

		cout << " | " << endl;
	}
	cout << "  ------------------------------------" << endl;
}
