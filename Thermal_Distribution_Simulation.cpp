/*----------------------------------------*/
/* Name: Aman Sunesh, NetID: as18181      */
/* Date: November 19, 2023                */
/* Description: This program simulates    */
/* thermal  dissipation  in  a thin metal */
/* plate  with  constant  (isothermal)    */
/* temperatures  on  each  side  given a  */
/* simulation temperature and a point on  */
/* the  plate. The simulation calculates  */
/* the  thermal  distribution  across the */
/* system and  is  then  converted  to a  */
/* visual  representation  in  a  bitmap  */
/* image file. The program ensures proper */
/* memory cleanup after execution         */
/*----------------------------------------*/


#include <iostream>
#include <iomanip>
#include <cmath>
#include "Bitmap_Helper.h"
const int NUM_SIDES = 4;
const int NEW_RANGE = 255;

using namespace std;


//Function Declarations
double** create2DGrid(int& ROWS, int& COLS);
void displayGrid(double** arr, int ROWS, int COLS);
void setBoundaryConditions(double** arr, int ROWS, int COLS);
void stimulateGrid(double** arr, int ROWS, int COLS, int& x, int& y, int& constant);
void simulateThermalDissipation(double** arr, int ROWS, int COLS, int x, int y, int constant);
uint8_t** convertDoubleToUint(double** arr, int ROWS, int COLS);
bool isInvalidInput();
void deallocate2DGrid(double** arr, int ROWS);



int main()
{
	//Declare and Initialize Variables
	int choice;
	int ROWS = 0, COLS = 0, constant = 0;
	double** arr = nullptr;  //Pointer to a 2D array
	int x, y;

	//Flags to track which steps have been completed
	bool gridCreated = false;
	bool boundarySet = false;
	bool gridStimulated = false;

	//Main Simulation Loop
	do
	{
		//User Interface
		cout << endl;
		cout << "---------------------------------------------------" << endl;
		cout << "          THERMAL DISTRIBUTION SIMULATION          " << endl;
		cout << "---------------------------------------------------" << endl;
		cout << "1. Create the Dynamic 2D Grid" << endl;
		cout << "2. Set Boundary Conditions for the Grid" << endl;
		cout << "3. Stimulate the Grid at a Specific Point" << endl;
		cout << "4. Calculate the Thermal Distribution" << endl;
		cout << "5. Print the Thermal Grid" << endl;
		cout << "6. Exit" << endl;
		cout << "Enter your choice (1-6): ";
		cin >> choice;



		//Set precision for output values
		cout << fixed << setprecision(2);

		switch (choice)
		{
			case 1:
			{
				//Option 1: Create the Dynamic 2D Grid

				//If grid has not been created before
				if (arr == nullptr)
				{
					arr = create2DGrid(ROWS, COLS);
					gridCreated = true;
					break;
				}
				else
				{
					deallocate2DGrid(arr, ROWS);
					arr = create2DGrid(ROWS, COLS);
					break;
				}
			}

			case 2:
			{
				//Option 2: Set the Boundary Conditions for the Grid


				//Check if the user has created the grid
				if (!gridCreated)
				{
					cout << "Error: Create the grid first (Option 1)!" << endl;
					break;
				}

				setBoundaryConditions(arr, ROWS, COLS);
				boundarySet = true;
				break;
			}


			case 3:
			{
				//Option 3: Stimulate the Grid at a Specific Point

				//Check if the user has created the grid and set boundaries
				if (!gridCreated || !boundarySet)
				{
					cout << "Error: Create the grid and set boundaries first (Options 1 and 2)!" << endl;
					break;
				}

				stimulateGrid(arr, ROWS, COLS, x, y, constant);
				gridStimulated = true;
				break;
			}


			case 4:
			{
				//Option 4: Calculate the Thermal Distribution

				//Check if the user has created the grid, set boundaries, and stimulated the grid
				if (!gridCreated || !boundarySet || !gridStimulated)
				{
					cout << "Error: Create the grid, set boundaries, and stimulate the grid first (Options 1, 2, and 3)!" << endl;
					break;
				}

				simulateThermalDissipation(arr, ROWS, COLS, x, y, constant);
				break;
			}


			case 5:
			{
				//Option 5: Display the Thermal Grid

				//Check if the user has created the grid
				if (!gridCreated)
				{
					cout << "Error: Create the grid first (Option 1)!" << endl;
					break;
				}

				displayGrid(arr, ROWS, COLS);
				break;
			}


			case 6:
			{
				//Option 6: Exit

				cout << "End of the program!" << endl;
				//Switch statement breaks, Memory is deallocated in main before exiting the program

				break;

			}

			default:
			{
				cout << "Invalid Choice! Please enter either 1, 2, 3, 4, 5, or 6: ";

				//Clears the input buffer
				cin.clear();

				//To ensure that error message is printed only once
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				continue;
			}
		}



	} while (choice != 6);

	// Cleanup Allocated Memory

	deallocate2DGrid(arr, ROWS);

	return 0;

}


//Function to create a dynamic 2D grid
double** create2DGrid(int& ROWS, int& COLS)
{
	double** arr = nullptr;  //Pointer to a 2D array

	//Prompt for the number of rows and validate input
	cout << "Enter the number of rows of the grid: ";
	cin >> ROWS;

	while (isInvalidInput() || ROWS < 4)
	{
		cout << "Please enter a positive integer value, greater than 3, for number of rows: ";
		cin >> ROWS;
	}


	//Prompt for the number of columns and validate input
	cout << "Enter the number of columns of the grid: ";
	cin >> COLS;

	while (isInvalidInput() || COLS < 4)
	{
		cout << "Please enter a positive integer value, greater than 3, for number of columns: ";
		cin >> COLS;

	}


	//Allocate memory for the grid
	arr = new double* [ROWS];

	for (int i = 0; i < ROWS; i++)
	{
		arr[i] = new double[COLS];
	}

	return arr;
}


//Function to display the thermal grid
void displayGrid(double** arr, int ROWS, int COLS)
{
	for (int i = 0; i < ROWS; i++)
	{
		for (int j = 0; j < COLS; j++)
		{
			cout << left << setw(7) << arr[i][j] << " ";
		}

		cout << endl;
	}
}


//Function to set boundary conditions for the grid
void setBoundaryConditions(double** arr, int ROWS, int COLS)
{
	double sides[NUM_SIDES];

	//Input boundary temperatures
	cout << "Enter the initial temperature of the top side: ";
	cin >> sides[0];

	while (isInvalidInput() || sides[0] < 0 || sides[0]>255)
	{
		cout << "Please enter a positive value, between 0 and 255 (inclusive), for temperature of the top side: ";
		cin >> sides[0];

	}


	cout << "Enter the initial temperature of the bottom side: ";
	cin >> sides[1];

	while (isInvalidInput() || sides[1] < 0 || sides[1]>255)
	{
		cout << "Please enter a positive value, between 0 and 255 (inclusive), for temperature of the bottom side: ";
		cin >> sides[1];
	}



	cout << "Enter the initial temperature of the left side: ";
	cin >> sides[2];

	while (isInvalidInput() || sides[2] < 0 || sides[2] > 255)
	{
		cout << "Please enter a positive value, between 0 and 255 (inclusive), for temperature of the left side: ";
		cin >> sides[2];
	}



	cout << "Enter the initial temperature of the right side: ";
	cin >> sides[3];

	while (isInvalidInput() || sides[3] < 0 || sides[3] > 255)
	{
		cout << "Please enter a positive value, between 0 and 255 (inclusive), for temperature of the right side: ";
		cin >> sides[3];
	}


	//Set boundary temperatures in the grid
	for (int i = 0; i < ROWS; i++)
	{
		for (int j = 0; j < COLS; j++)
		{
			if (i == 0)
			{
				arr[i][j] = sides[0];
			}

			else if (i == ROWS - 1)
			{
				arr[i][j] = sides[1];
			}

			else if (j == 0)
			{
				arr[i][j] = sides[2];
			}

			else if (j == COLS - 1)
			{
				arr[i][j] = sides[3];
			}

			else
			{
				arr[i][j] = 0;
			}
		}
	}
}


//Function to stimulate the grid at a specific point
void stimulateGrid(double** arr, int ROWS, int COLS, int& x, int& y, int& constant)
{
	double stimulation_val;

	//Input stimulation coordinates
	cout << "Please note that 0 represents the first column and 0 represents the first row." << endl;

	cout << "Enter the x-coordinate (columns - left to right) point of stimulation : ";
	cin >> x;

	while (isInvalidInput() || x >= COLS - 1 || x <= 0)
	{
		cout << "Please enter a value between 1 and (number of columns - 2), both inclusive, for the x-coordinate: ";
		cin >> x;
	}


	cout << "Enter the y-coordinate (rows - top to bottom) point of stimulation : ";
	cin >> y;

	while (isInvalidInput() || y >= ROWS - 1 || y <= 0)
	{
		cout << "Please enter a value between 1 and (number of rows - 2), both inclusive, for the y-coordinate: ";
		cin >> y;
	}


	//Input stimulation value
	cout << "Enter the stimulation value: ";
	cin >> stimulation_val;

	while (isInvalidInput() || stimulation_val < 0 || stimulation_val > 255)
	{
		cout << "Please enter a positive value, between 0 and 255 (inclusive), for the stimulation value: ";
		cin >> stimulation_val;
	}

	//Check if user wants the stimulation to be constant or discrete
	cout << "Do you wish to keep the stimulation temperature constant?, Please enter 1 for YES or 0 for NO: ";
	cin >> constant;

	while (isInvalidInput() || constant < 0 || constant>1)
	{
		cout << "Please enter either 0 or 1: ";
		cin >> constant;
	}


	//Apply stimulation to the grid
	arr[y][x] = stimulation_val;

}


//Function to stimulate thermal dissipation in the grid
void simulateThermalDissipation(double** arr, int ROWS, int COLS, int x, int y, int constant)
{
	double tolerance = 0;
	bool equilibrium;

	//Input tolerance for equilibrium
	cout << "Enter the tolerance value: ";
	cin >> tolerance;

	while (isInvalidInput() || tolerance < 0 || tolerance > 255)
	{
		cout << "Please enter a positive value, between 0 and 255 (inclusive), for the tolerance value: ";
		cin >> tolerance;
	}


	//Thermal dissipation simulation loop
	do
	{
		equilibrium = true;

		//Iterate through the grid cells for thermal dissipation
		for (int i = 1; i < ROWS - 1; i++)
		{
			for (int j = 1; j < COLS - 1; j++)
			{
				//Continuous Stimulation
				if (constant == 1 && i == y && j == x)
				{
					continue;
				}

				else
				{
					double oldTemp = arr[i][j];
					double newTemp = (arr[i - 1][j] + arr[i + 1][j] + arr[i][j - 1] + arr[i][j + 1]) / 4;


					//Check for equilibrium
					if (abs(oldTemp - newTemp) >= tolerance)
					{
						arr[i][j] = newTemp;
						equilibrium = false;
					}


				}
			}
		}

	} while (!equilibrium);



	//Convert double data to unsigned integers
	uint8_t** (imageData) = convertDoubleToUint(arr, ROWS, COLS);

	//Store the thermal data in the form of an image in .bmp format
	writeBitmap("thermal_distribution.bmp", imageData, COLS, ROWS); //Assuming name of the image file is given


	//Cleanup allocated memory for bitmap data
	for (int i = 0; i < ROWS; ++i) {
		delete[] imageData[i];
	}
	delete[] imageData;

}

//Function to convert double data to unsigned integers
uint8_t** convertDoubleToUint(double** arr, int ROWS, int COLS)
{
	//Create a Uint_8 array
	uint8_t** imageData = nullptr;

	//Allocate memory for the new array
	imageData = new uint8_t * [ROWS];

	for (int i = 0; i < ROWS; i++)
	{
		imageData[i] = new uint8_t[COLS];
	}

	//Find the maximum and minimum values in the input array
	int maxval = arr[0][0];
	int minval = arr[0][0];


	//Iterate through the array to determine the range of values
	for (int i = 0; i < ROWS; i++)
	{
		for (int j = 0; j < COLS; j++)
		{
			//Update maxval if the current element is greater
			if (arr[i][j] > maxval)
			{
				maxval = arr[i][j];
			}

			//Update minval if the current element is smaller
			if (arr[i][j] < minval)
			{
				minval = arr[i][j];
			}
		}

	}

	//Calculate the range of values
	int range = maxval - minval;

	//Variables for value scaling and rounding
	int old_value;
	int new_value;

	/*
	   New Range = (NewMax - NewMin);
	   NewValue = ((((Old Value) - OldMin) * NewRange) / OldRange) + NewMin;
	*/

	//Convert each element from the input array to uint8_t and store in the new array
	for (int i = 0; i < ROWS; i++)
	{
		for (int j = 0; j < COLS; j++)
		{
			//Get the old (original) value from the input array
			int old_value = arr[i][j];

			//Scale the old value to fit the new range (0-255) and round to the nearest integer
			int new_value = static_cast<int>((old_value - minval) * NEW_RANGE / range + 0.5);

			//Store the scaled and rounded value as uint8_t in the new array
			imageData[i][j] = static_cast<uint8_t>(new_value);
		}
	}

	//Return the new 2D array of uint8_t
	return imageData;

}


//Function to check if the input is valid
bool isInvalidInput()
{
	bool invalidInput = false;

	//Check for invalid input
	if (cin.fail())
	{
		invalidInput = true;
		cout << "Error! Invalid Input! Please enter a valid numeric input!" << endl;
		cout << endl;

		//Clears the input buffer
		cin.clear();

		//To ensure that error message is printed only once
		cin.ignore(numeric_limits<streamsize>::max(), '\n');

	}

	else
	{
		invalidInput = false;
	}

	return invalidInput;
}

//Function to deallocate memory for the 2D grid
void deallocate2DGrid(double** arr, int ROWS)
{
	for (int i = 0; i < ROWS; ++i)
	{
		delete[] arr[i];
	}

	delete[] arr;
}
