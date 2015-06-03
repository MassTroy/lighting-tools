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

#pragma once
#define NAME "lightning-format" //Written name of the program
#define EXE_NAME "lformat" //filename for the executable
#define VERSION "2.0.1" //version number
#define COPYRIGHT "(C) 2010, 2011 Troy Mass" //copyright tag
//#define DEBUG 1

//standard includes
#include <iostream> //cout
#include <string> //string data type
#include <fstream> //ifstream and ofstream
#include <cstring> //strcat and strcop
#include <sstream> //used for turning a string into a stream to pull an int from it //TODO: OPTIMIZE: come up with a better way to do this
using namespace std;

//project includes
#include "classes.h"

//function prototypes
//functions.cpp
bool contains(string main, string key); //method to check if arg1 contains arg2
void help(); //displays help content
void copying(); //displays copyright information
void backup (char orgFile[], char bakFile[]); //creates backup file
//options.cpp
void loadOpts(); //loads options from file
void editOpts(); //edits the options and saves them
void displayOpts(); //displays the option configuration

/*
	Better practice forms for tagging comments: 
		the following are ways to tag comments to make them easy to find later
	
	//TODO: 
		anything that needs to be done
	//TODO: FEATURE: 
		new features to be added
	//TODO: ERROR: 
		errors that need to be corrected
	//TODO: OPTIMIZE: 
		code that needs to be optimized for readability, function, memory useage, or speed
	//TODO: STUB: 
		shows that a function has been stubbed out but not implimented
	//OPTIMIZE
		lower priority optimizations or things that could be optimized but are more readable as is.
	//FEATURE: 
		lower priority features to be added
	
	
	//DESCRIPTION: 
		describes a section of code. Descriptions should be a few sentances describing what the code is doing and why and 
		any important information about the code. It should be placed inside a multiline comment block and indented.
	//DETAILS: 
		multiline paragraph comment giving more details on another tag such as FEATURE, ERROR, OPTIMIZE, or STUB. Things to list 
		could include specific causes of an error, how a feature should work, or ideas on how to fix something. 
	
	
	////////////////////////////// SECTION //////////////////////////////
		used to mark beginning of sections of code in long functions. Length is not important, but around 80 chars is best.
*/


