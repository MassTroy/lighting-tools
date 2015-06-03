/*
	COPYRIGHT 2010, 2011 TROY MASS
	
	This file is part of lightning-format.

	lightning-format is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	lightning-format is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with lightning-format. If not, see <http://www.gnu.org/licenses/>.
*/

//includes
#include "include.h"

//global variables
int nest; //how many indented {} blocks
extern int prefs[]; //preferences data array

int main ( int argc, char* argv[] ) {
	try {
		//TODO: FEATURE: add and parse new "-" command args
		if (argc >= 2) { //was at least 1 arg entered?
			string store = "";
			for (int i=0; i<strlen(argv[1]); i++) store += argv[1][i]; //copy arg into string store
			if (contains(store, "--help")) {
				help(); //display help content
				return 0;
			} else if (contains(store, "--copying")) {
				copying(); //displaces copyright information
				return 0;
			} else if (contains(store, "--display")) {
				displayOpts(); //displays the currently set options
				return 0;
			} else if (contains(store, "--edit")) {
				editOpts(); //displays the currently set options
				return 0;
			}
		} else {
			cout << "\nYou must enter at least one command line arguement\n"
				<< "\tenter \"--help\" for more information\n\n";
			return 0;
		}
	
		//set global
		//default Opts loaded automatically
		loadOpts(); //calls the load options command (overrrides defaults)
		nest = prefs[6]; //initializes nest
	
		if (prefs[3]!=0) cout << "\n\nWelcome to the " << NAME << ", a simple source code formatter\n\n";
	
		//files
		char inputFile[200] = "";
		char outputFile[200] = "";
		if (argc >= 3) { //input AND output files given
			strcpy(inputFile,argv[1]);
			strcpy(outputFile,argv[2]);
			cout << "Formatting from \"" << inputFile << "\" to \"" << outputFile << "\" ... ";
		} else if (argc >= 2) { //only input file given
			strcpy(inputFile,argv[1]);
			strcpy(outputFile, inputFile);
			if (prefs[1] == 1) { //if formatting back to input file
				char exten[5] =  ".bak";
				strcat(inputFile, exten);
				//next line copies the input file
				backup(outputFile, inputFile); //These are suppose to look backwards
				//formmating is done from the copy to the original
				cout << "Formatting \"" << outputFile << "\" ...\n";
			} else {
				char temp[200] =  "FORMATTED_";
				strcat(temp, outputFile);
				strcpy(outputFile,temp);
				cout << "Formatting from \"" << inputFile << "\" to \"" << outputFile << "\" ... ";
			}
		}
		
		parser pObj; //creates the parser object
		pObj.begin(inputFile, outputFile); //begins the parse function
	
		//if (prefs[3]!=0) cout << "Done!\n";
	
		if (prefs[3]!=0) {
			cout << "\nThank you for formatting\n\n";
			cout << "\t" << NAME << " version: " << VERSION << "\n"
				<< "\tCompiled: " << __TIMESTAMP__ << "\n"
				<< "\tCOPYRIGHT " << COPYRIGHT << "\n\n\n";
		}
	} catch (string e) { cout << "\nERROR 1: An exception was thrown: " << e << "\n"; return 1; }
	catch (char e[]) { cout << "\nERROR 2: An exception was thrown: " << e << "\n"; return 2; }
	//TODO: The last catch statement has been commented out due to errors already outputting a statement
	//The errors should ultimately be caught by the 2 catch blocks above
	catch (...) { 
		//cout << "\nERROR 3: An unknown exception was thrown\n"; 
		return 3;
	} //TODO: ERROR: why is it only catching unknown errors?
	
	cout << "\n";
	return 0;
} //end: void main (int argc, char* argv[])



