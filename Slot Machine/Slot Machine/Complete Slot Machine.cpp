/*
Author: Patrick Collins
©️license MIT
https://github.com/Paddylonglegs/
*/


// INCLUDES ///////////////////////////////////////////////

#include <stdio.h> 

#include <stdlib.h> 

#include <ctype.h>  

#include <conio.h> 

#include <windows.h> 

#include <time.h> 

#include <string> 

#include <iostream> 

using namespace std;



// DEFINES //////////////////////////////////////////////// 



#define MAX_X        110  // maximum x position for player 

#define SCROLL_POS   29  // the point that scrolling occurs 



// PROTOTYPES ///////////////////////////////////////////// 



void init_Graphics();

void Set_Color(int fcolor, int bcolor);

void Draw_String(int x, int y, string st);


// GLOBALS //////////////////////////////////////////////// 

CONSOLE_SCREEN_BUFFER_INFO con_info;   // holds screen info 

HANDLE hconsole;         // handle to console 

bool game_running = true; // state of game, False=stop, True=run 

int scroll_number = 0;

char slots[5][5] =
{
{'!', '!', '!', '!', '!' } ,   //Characters for row 1 (0)
{'$', '$','$', '$', '$'} ,   // Characters for row 2 (1)
{'\xA3', '\xA3' , '\xA3', '\xA3', '\xA3'} ,   // Characters for row 3. \xA3 is � in	ASCII (2)
{'*', '*','*', '*', '*'} , // Characters for row 4 (3)
{'%', '%','%', '%', '%'} , //Characters for row 5 (4)
};

bool column_spinning[5] = { true, true, true, true, true }; //creating array just for the columns

bool waitForAnswer = false; //waiting for user input if to continue playing or not

bool prizeIsGiven = false; //bool for if statement the checking for a win

int credit = 30; //user credit the user starts of with

bool jackpot = false;

bool miniJackPot = false;


// FUNCTIONS ////////////////////////////////////////////// 


// This function initializes the console graphics engine 
void init_Graphics()

{
	COORD console_size = { 110, 30 }; // size of console 



									 // seed the random number generator with time 

	srand((unsigned)time(NULL));



	// open i/o channel to console screen 

	hconsole = CreateFile(TEXT("CONOUT$"), GENERIC_WRITE | GENERIC_READ,

		FILE_SHARE_READ | FILE_SHARE_WRITE,

		0L, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0L);



	// make sure we are in 80x25 

	SetConsoleScreenBufferSize(hconsole, console_size);



	// get details for console screen                        

	GetConsoleScreenBufferInfo(hconsole, &con_info);



} // end init_Graphics 



  /////////////////////////////////////////////////////////// 


//This function sets the color of the console output 
void Set_Color(int fcolor, int bcolor = 0)
{
	SetConsoleTextAttribute(hconsole, (WORD)((bcolor << 4) |

		fcolor));

} // Set_Color 



  /////////////////////////////////////////////////////////// 


// This function draws a string on the console at the given x,y 
void Draw_String(int x, int y, string s)
{

	COORD cursor_pos; // Used to pass coords 



	// Set printing position 

	cursor_pos.X = x;

	cursor_pos.Y = y + scroll_number;

	SetConsoleCursorPosition(hconsole, cursor_pos);

	// print the string in current color 

	cout << s;


} // end Draw_String 



  /////////////////////////////////////////////////////////// 


//Clears the screen, resets and wipes anything on it
void Clear_Screen(void)

{

	// this function clears the screen 



	// set color to white on black 

	Set_Color(15, 0);



	// clear the screen 

	for (int index = 0; index <= 30; index++)

		Draw_String(0, SCROLL_POS, "\n");
	//system("cls");


} // end Clear_Screen 

///////////////////////////////////////////////////////

//Function for spinning the slot machine
void spinningSlots()
{
	int temp;
	for (int column = 0; column < 5; column++) //first for loop is for columns
	{
		//Rotating the characters in the array
		if (column_spinning[column])
		{
			temp = slots[5 - 1][column];

			for (int row = 5 - 1; row > 0; row--) //once first for loop gets here it executes this for loop. Second for loop returns row of characters
			{
				slots[row][column] = slots [row - 1] [column] ; //going down the rows, why there is -1.
			}
			//second for loop ends

			slots[0][column] = temp; //makes top row the bottom row in the array
		}

	}

} //end spinningSlots

///////////////////////////////////////////////////////////

//Simply a funciton to display the slots. 
void displaySlots()
{
	for (int row = 0; row < 5; row++) //once first for loop gets here it executes this for loop. Second for loop returns row of characters
	{       
		for (int column = 0; column < 5; column++) //first for loop is for columns
		{
			
			if (column == 0)
			{
				Set_Color(10, 0);
			}
			else if (column == 1)
			{
				Set_Color(10, 0);
			}
			else if (column == 2)
			{
				Set_Color(7, 0);
			}
			else if (column == 3)
			{
				Set_Color(6, 0);
			}
			
			/*if (row == 2)
			{
				Set_Color(0, 15);
			}*/

			cout << " | " << slots[row][column] << " ";
		}
		cout <<  " | " << endl;
	}
} //end displaySlots

///////////////////////////////////////////////////////////

//Reseting the values to its original state before being spun
void resetSlots() 
{
	char element;

	for (int row = 0; row < 5; row++)
	{
		if (row == 0)
		{
			element = '!';
		}
		else if (row == 1)
		{
			element = '$';
		}
		else if (row == 2)
		{
			element = '\xA3';
		}
		else if (row == 3)
		{
			element = '*';
		}
		else if (row == 4)
		{
			element = '%';
		}

		for (int column = 0; column < 5; column++)
		{
			slots[row][column] = element;
		}
	}
} //end resetSlots

///////////////////////////////////////////////////////////

//Checking if the user has pressed the keys listed below
void checkKey()
{
	char key = 0;            // player input data 

	if (_kbhit())
	{
		key = toupper(_getch());

		// Get player input to stop rotating each column
		if (key == '1')
		{
			//Stop first column
			column_spinning[0] = false;
		}

		if (key == '2')
		{
			//stop Second column
			column_spinning[1] = false;

		}

		if (key == '3')
		{
			// stop Third column
			column_spinning[2] = false;
		}

		if (key == '4')
		{
			// stop Fourth column
			column_spinning[3] = false;
		}

		if (key == '5')
		{
			// stop Fifth column
			column_spinning[4] = false;
		}

		//user input to continue playing the slot machine
		if (key == 'R')
		{
			//Reset
			for (int column = 0; column < 5; column++)
			{
				column_spinning[column] = true;
			}

			resetSlots();
			prizeIsGiven = false;
			waitForAnswer = false;
			credit = credit - 10;
		}
		//user input to stop playing the slot machine
		if (key == 'E')
		{
			//Exit
			waitForAnswer = false;
			game_running = false;
		}
	}

} //end checkKey

///////////////////////////////////////////////////////////

//Function for checking if the user won a prize
void checkWin()
{
	if (column_spinning[0] == false && column_spinning[1] == false && column_spinning[2] == false && column_spinning[3] == false && column_spinning[4] == false && prizeIsGiven == false) //if all columns have stopped rotating
	{
		int charCheck = 0;
		char charType;

		for (int charCode = 0; charCode < 5; charCode++)
		{
			//assigning numbers to characters 
			if (charCode == 0)
			{
				charType = '!';
			}
			else if (charCode == 1)
			{
				charType = '$';
			}
			else if (charCode == 2)
			{
				charType = '\xA3';
			}
			else if (charCode == 3)
			{
				charType = '*';
			}
			else if (charCode == 4)
			{
				charType = '%';
			}

			for (int column = 0; column < 5; column++)
			{
				if (charType == slots[1][column])
				{
					charCheck++;
				}
			}

			if (charCheck == 5)
			{
				jackpot = true;
			}

			else if (charCheck == 3)
			{
				miniJackPot = true;
			}


			charCheck = 0; // reset

		}

		prizeIsGiven = true; //End of checking 
	}


} //end checkKey

///////////////////////////////////////////////////////////



// MAIN GAME LOOP ///////////////////////////////////////// 

int main()
{
	SetConsoleOutputCP(1252); //printing in ASCII

	 // set up the console text graphics system 

	init_Graphics();

	// clear the screen 

	Clear_Screen();

	// SECTION: main event loop, this is where all the action takes place, the general loop is switching array elements around and displaying contents.

	//Charge user for their first game/spin
	credit = credit - 10;

	while (credit >= 0 && game_running == true) // if the credit is less than zero and the user doesn't want to play again, this will not be run.
	{

		checkKey();

		//Rolling slots
		spinningSlots();

		//Checking for a win	
		checkWin();

	    // SECTION: draw everything 

		Set_Color(10, 0); //4 is red, 12 orange maybe

		Draw_String(rand() % MAX_X, SCROLL_POS, ""); // keeps rows&columns in place

		scroll_number++;

		// SECTION: synchronize to a constant frame rate 
		Sleep(500);

		//End sequence once all five columns have been stopped and the decision on where to go next.
		if (prizeIsGiven == false)
		{
			Draw_String(0, 0, "Press 1 to stop the first column, 2 for second...One spin costs 10 gold\n");
			Draw_String(0, 1, "Your pot of irish gold: " + to_string(credit) + string("\n"));
			Draw_String(0, 2, "");
			displaySlots();
		}
		else
		{
			Clear_Screen(); //Wipe screen

			Set_Color(10, 0); //Set colour to light green, same as before wipe

			Draw_String(0, 0, "");

			//Awarding the user if won, or wishing luck if didn't get awarded anything.
			if (jackpot == true)
			{
				credit = credit + 100;
				cout << "Congrats you hit the jackpot!" << endl;
				jackpot = false;
			}
			else if (miniJackPot == true)
			{
				credit = credit + 50;
				cout << "Congrats you hit the mini jackpot!" << endl;
				miniJackPot = false;
			}
			else
			{
				cout << "Too bad kiddo, better luck next time" << endl;
			}

			//Show the options to the user
			cout << "Press R to replay, or press E to exit: " << endl;

			//Show Slots
			displaySlots();


			//Waiting for answer
			waitForAnswer = true;
			do 
			{
				checkKey(); //Keeping calling until right key press
			} while (waitForAnswer);

			Clear_Screen(); //Wipe the screen


		}


	}

	// SECTION: player has chose to exit the game and stop playing

	//Drawing nice Shamrock for player
	Clear_Screen();
	Set_Color(10, 0);

	Draw_String(55, 12, "     .-. .-.");
	Draw_String(55, 13, "    (   |   )");
	Draw_String(55, 14, "  .-.:  |  ;,-.");
	Draw_String(55, 15, " (_ __`.|.'_ __)");
	Draw_String(55, 16, " (    ./Y\x5c.    )");
	Draw_String(55, 17, "  `-.-' | `-.-'");
	Draw_String(55, 18, "        |       ");
	Draw_String(55, 19, "        |       ");
	Draw_String(48, 20, "C R E A T E D  B Y  P A D D Y");
	Draw_String(47, 22, "T H A N K S  F O R  P L A Y I N G!");



	return 0; //END

} // end main 
