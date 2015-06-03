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

//enum used to store the type of line a string is. Use parser::lType(string line) to determine type.
enum lineType { BLANK=1, CODE, SCOMMENT, MCOMMENT, BCURL, ECURL, PREPROCESSOR, ELSE };
//CODE is any line that is not blank and not one of the other special cases.


//TODO: redesign these classes to be reused for formatting multiple files
//TODO: add more error checking to ifile and ofile.
class ifile {
protected:
	ifstream* stream; //pointer to filestream
	
	string* curLine; //line that was just read
	string* nextLine; //peek at the next line to be read
	bool eof; //end of file bit
public:
	ifile(char Name[]); //creates the object and opens the file
	void close(); //closes a file
	string curLn(); //view current line wihtout moving read head
	string getLn(); //retrieves next line
	bool hasNext(); //returns true file has next line
	string peekLn(); //peeks at next line without moving read head
}; //end class ifile

class ofile {
protected:
	ofstream* stream; //pointer to filestream
public:
	ofile(char Name[]);
	void close(); //closes a file
	void putLine(string line); //writes string to file (no newline at end)
	void operator<<(string line); //writes string to file (no newline at end)
}; //end class ofile

class parser {
protected:
	ifile* input; //pointer to my custom input file object
	ofile* output; //pointer to my custom output file object
public:
	parser() {}
	void begin(char inFile[], char outFile[]); //begins the parsing process. Pass filenames as c_strings
	string clean(string in); //cleans double spaces (except in comments), and trailing blanks
	bool endSComm(string in); //determines if the line ends in a single line comment
	lineType lType(string line); //determines the type of line. Returns enum lineType
	string parseLine(string in); //main parsing function. //located in file parseline.cpp
	string safeSplit(string in); //used for SafeParse mode, runs a simple parse breaking lines by adding a \n delimiter (non-recursive)
	string tabs(int cur_nest); //returns the appropriate number of tabs
}; //end class parser
