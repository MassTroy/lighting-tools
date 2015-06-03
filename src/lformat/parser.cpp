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

#include "include.h"
extern int nest; //how many indented {} blocks
extern int prefs[]; //preferences data array

void parser::begin(char inFile[], char outFile[]) {
	try {
		//file streams
		if (prefs[3]!=0) cout << "\nOpening files... ";
		input = new ifile(inFile);
		output = new ofile(outFile);
		if (prefs[3]!=0) cout << "Done!\n";
	
		if (prefs[3]!=0) cout << "Formatting\n\n";
	
		//add the signature
		/*
			//DESCRIPTION: A signature is appended to the top of every file listing that it was formatted using this program. 
			The signature is in a comment form and lists the program name and version number. The date could also be added. 
			The old signature on the file must be junked to prevent multiple signatures if formating multiple times. 
		*/
		string signature = "/*This code was formatted using ";
		signature += NAME;
		signature += " V";
		signature += VERSION;
		signature += "*/";//put \n here if needed
		output->putLine(signature);
		string partialTag = "formatted using "; //partial tag to search for
		partialTag += NAME; //adds the name of the program
		if (contains(input->peekLn(), partialTag)) {
			input->getLn(); //junks the old signature if it exists
			if (input->peekLn().compare("") == 0) input->getLn(); //junks the blank line entered
		}
		
		////////////////////////////////////// MAIN PARSING LOOP ////////////////////////////////////////
		
		
		//VARIABLES needed for main parsing loop
		string inputFileBuffer = ""; //stores a \n deliminated series of lines
		string toParseBuffer = ""; //single or double line ready to be fed in to the parser
		int iHead=0; //read head to inputFileBuffer
		while (input->hasNext()) { //main parsing loop
			bool ready = false; //ready to parse in
			/*	
				//DESCRIPTION: This function uses 2 buffers to control the flow of data. The outer while loop has 4 parts as follows: 
				1. fill inputFileBuffer. 2.fill toParseBuffer. 3. Prepare data for parsing. 4. Parse data if ready. 
				Section 1: 	Handles the safe parse function that splits long lines to reduce memory useage. It then fills inputFileBuffer with either a 
					single short line, or if the line is too long, a \n deliminated string of multiple lines separated by the safeSplit() function. The 
					buffer will ONLY be filled if emptied; it will NOT be filled if the read head (iHead) reaches the end. You MUST manually clear the 
					buffer when you want it to reload. See below for description of safe-parse mode.
				Section 2: 	Similar to the above section, the toParseBuffer will only be filled if empty. This is required because the program will loop 
					section 3 runs out of data and needs to have the inputFileBuffer filled.
				Section 3: 	Prepares the data before it is parsed. It checks to see if the next line in the file should be merged to make it easier for 
					the parseLine function later. When moving curly braces from out-of-line in the input file to in-line in the output file, it is required 
					to move them up. Section 3 handles that move. This function loops through the multiple lines stored in the inputFileBuffer and moves 
					them as necessary to the toParseBuffer. The loop has 2 termination conditions. It either breaks because it ran out of data in the 
					inputFileBuffer, or it sets ready=true telling section 4 that the toParseBuffer is ready to be parsed as is. 
				Section 4: 	When section 3 signals that the toParseBuffer is ready, section 4 then parses it. The line is then cleaned of any large 
					whitespaces and trailing whitespace. The program will add these extra spaces to control the look and to prevent merging words. 
					After cleaning, the line is then written to the file and the toParseBuffer is cleared.
				
				Lines follow the following order: read, [safeParse if length > 80], [append next if needed], parseLine, clean (removes 
				large whitespaces within and after lines), write to file.
			*/
			//TODO: OPTIMIZE: add code to dealloc unused portions of inputFileBuffer after they are parsed. Everything in front of read head possition rfInput
			//TODO: OPTIMIZE: stop rellying on garbage collector. Make all strings as pointers and use new and delete functions to preform allocation and deallocation
			//TODO: OPTIMIZE: Add threading support. Needs a dual buffer system. 1 thread reads and prepares data. The other thread parses the data.
			
			//Fill inputFileBuffer if it is empty
			if (inputFileBuffer.length() == 0) { //if inputFileBuffer is empty or completed get new line
				inputFileBuffer = " "; //add a space so the program can tell the difference between a blank line and empty buffer
				if (input->peekLn().length() > 80) { //safeParse mode
					/*
						//DESCRIPTION: Safeparse mode is to prevent memory overflow when reading extremely long lines. 
						It runs a non-recursive parse to determine safe places to split the lines. The function 
						string safeSplit(string in) performs this function and returns a \n deliminated string containing all 
						of the split lines. The lines are stored in the inputFileBuffer. 
						Shorter lines are parsed normally to increase speed since they will not cause issues. 
					*/
					inputFileBuffer += safeSplit(input->getLn()); //safeparse the new line and add it
				} else {
					inputFileBuffer += input->getLn();
				}
				iHead = 0; //reset read head
			}
			
			//Fill toParseBuffer with a single line if it is empty
			if (toParseBuffer.length() == 0) {
				while ( inputFileBuffer[iHead] != '\n' && iHead < inputFileBuffer.length()) {
					toParseBuffer += inputFileBuffer[iHead]; //reads current line into toParseBuffer
					iHead++;
				}
				iHead++; //jumps over \n flag
				if (iHead >= inputFileBuffer.length()) inputFileBuffer = ""; //clear the buffer to request more data
			}
		
			///////////////////////////////////////////////// PREPARE DATA ///////////////////////////////////////////////////////////
			while (!ready) {
				lineType ltToParse = lType(toParseBuffer);
				if (ltToParse == BLANK || endSComm(toParseBuffer) || ltToParse == PREPROCESSOR) { //imediately parse without moving up following lines
					ready = true;
				} else {
					string peek = "";
					int pHead = iHead; //create secondary read head for peek
					if (pHead < inputFileBuffer.length()) { //get peek from inputFileBuffer
						for ( ; inputFileBuffer[pHead] != '\n' && pHead < inputFileBuffer.length(); pHead++) peek += inputFileBuffer[pHead]; //gets peek line
						pHead++; //jumps over the \n flag
					} else { //out of data, break to fill inputFileBuffer
						inputFileBuffer = ""; //clear the buffer to request more data
						break; //jumps out of this loop to get more data without parsing
						/*
							//DESCRIPTION: if inputFileBuffer is empty, break out of this loop without parsing or clearing toParseBuffer. This forces the 
							program to refill inputFileBuffer, then this loop will be hit again to see if the next line should be merged. 
						*/
					}
					
					//Determine if following line should be merged
					lineType ltPeek = lType(peek);
					if (ltPeek == BCURL || ltPeek == ELSE) { //should peek line be merged?
						toParseBuffer += " "; //append a space for safety
						toParseBuffer += peek; // append the next line
						iHead = pHead; //move read head forward
					} else {
						ready = true;
					}
				}
			}
			
			if (ready) { //if toParseBuffer is ready to be parsed it, then parse, clean, write, and clear it
				//write 		//clean		//parse
				output->putLine(clean(this->parseLine(toParseBuffer))); //parse the buffer, clean it to remove large and trailing whitespace, and write it to the file
				toParseBuffer = ""; //clear toParseBuffer to be ready for next line
			}
		} //wash, rinse, and repeat
	
		//if (prefs[3]!=0) 
		cout << "Done\n";
	
		input->close();
		output->close();
		
		if (prefs[2] == 0) remove(inFile);
	} catch (...) { throw; }
}

bool parser::endSComm(string in) { //determines if the line ends in a single line comment
	int len = in.length(); //length of input string
	for (int i=0; i<len; i++) {
		register char wo = in[i]; //working character
		char pe = ' '; //peek char
		if (i<len-1) pe = in[i+1]; //fills peek char

		if (wo == '/') if (pe = '/') return true;
	}
	return false;
}

string parser::clean(string in) { //cleans leading blanks, double spaces, and trailing blanks
	string out = "";
	//parsing states
	bool dquote = false; //inside of a "" block
	bool squote = false; //inside of a '' block
	static bool mcom = false; //inside a multi-line comment
	bool fScom = false; //found a single line comment
	//loop variables
	int len = in.length(); //length of input string
	bool fChar = false; //used for ignoreing leading spaces and tabs
	
	////////////////////////////// BEGIN CLEAN //////////////////////////////
	for (int i=0; i<len; i++) {
		register char wo = in[i]; //working character
		char pe = ' '; //peek char
		if (i<len-1) pe = in[i+1]; //fills peek char
		
		if ((int)wo <= 32 && prefs[12] != 0 && wo != '\n') {
			if (!fChar && !mcom) {
				//do nothing. This removes leading whitespace if removing all nl. It must skip leading nl in order for #preprocessor to work
			} else {
				out += ' '; //outputs 1 space
				for (; i<len-1; i++) { if ((int)in[i+1] > 32 || in[i+1] == '\n') break; } //searches until end of controls
			}
		} else if (mcom) { //inside a multi-line comment
			out += wo;
			if (wo == '*' && pe == '/') { //look for end of multi line if in one
				mcom = false;
				out += pe;
				i++;
			}
		} else if ((int)wo == 32 && !fScom && !mcom && fChar) { //remove large whitespace (not leading or inside comments)
			out += ' '; //outputs 1 space
			for (; i<len-1; i++) { if ((int)in[i+1] > 32 || in[i+1] == '\n') break; }
		} else {
			out += wo;
			if (wo == '/') { //look for comments
				if (pe == '/') { //look for single-line comment
					for ( i=i+1; i<len; i++) out += in[i];
				} else if (pe == '*') { //look for multi-line comments
					mcom = true;
					out += pe;
					i++;
				}
			} else if (dquote == true || squote == true) { //inside a quote
				if (wo == '\\') { //look for escape charachters
					out += pe;
					i++;
				} else if (wo == '\"') { //look for end of dquote
					dquote = false;
				} else if (wo == '\'') { //look for end of squote
					squote = false;
				}
			} else if (wo == '\"') { //look for begin dquote
				dquote = true;
			} else if (wo == '\'') { //look for begin squote
				squote = true;
			}
			fChar = true; //used to ignore leading tabs and spaces
		} //end else parse
	} //end: for i loop
	
	//TODO: find a better way to clean trailing that doesnt involve reversing the string twice
	fChar = false;
	string reverse = "";
	for (int i=(out.length()-1); i>=0; i--) {
		register char wo = out[i]; //working character
		if ((int)wo == 32 && !fChar) { //remove trailing whitespace (space only, no controls)
			//do nothing
		} else {
			reverse += wo;
			fChar = true;
		}
	}
	string final = "";
	for (int i=reverse.length()-1; i>=0; i--) {
		register char wo = reverse[i]; //working character
		final += wo;
	}
	
	return final;
}

lineType parser::lType(string line) { //determines the type of line
	int len=line.length();
	for (int i=0; i<len; i++) {
		char wo = line[i]; //gets a single char //working character
		char pe = ' '; //peek char
		if (i<len-1) pe = line[i+1]; //fills peek char
		
		
		if ( (int)wo <= 32) { //ignore leading spaces, tabs, and controls
			//do nothing
		} else if (wo == '/') { //look for comments
			if (pe == '/') { //look for single-line comment
				return SCOMMENT;
			} else if (pe== '*') { //look for multi-line comments
				return MCOMMENT;
			} //cleans leading blanks, double spaces, and trailing blanks
		} else if (wo == '{') { //look for begin curly
			return BCURL;
		} else if (wo == '}') { //look for end curly
			return ECURL;
		} else if (wo == '#') {
			return PREPROCESSOR;
		} else if (wo == 'e' && i<len-5) {
			string substr = line.substr(i,4);
			if (substr.compare("else") == 0) return ELSE;
		} else if (wo == '\n') {
			return BLANK;
		} else return CODE;
	}
	return BLANK;
}

//string parseLine(string in); //main parsing function. //located in file parseline.cpp

string parser::safeSplit(string in) { //used for SafeParse mode, runs a simple parse breaking lines by adding a \n delimiter (non-recursive)
	string out = ""; //working output string
	//parsing states
	int paren_nest = 0; //how many nested parenthesis
	bool dquote = false; //inside of a "" block
	bool squote = false; //inside of a '' block
	static bool mcom = false; //inside a multi-line comment
	//loop variables
	int len = in.length(); //length of input string
	int i=0; //current possition
	bool fChar = false; //used for ignoreing leading spaces and tabs
	
	////////////////////////////// BEGIN PARSE //////////////////////////////
	for ( ; i<len; i++) {
		register char wo = in[i]; //fills working char
		char pe = ' '; //peek char
		if (i<len-1) pe = in[i+1]; //fills peek char
		
		if ( !fChar && (int)wo <= 32 && wo != '\n') { //ignore leading whitespace
			//do nothing
		} else { ////////////////////////////// PARSE CODE //////////////////////////////  //Parse all code
			if (mcom) { //inside a multi-line comment
				if (wo == '*' && pe == '/') { //look for end of multi line if in one
					mcom = false;
					i++;
					out += wo;
					out += pe;
				} else 
					out += wo;
			} else if (dquote == true || squote == true) { //inside a quote
				out += wo;
				if (wo == '\\') { //look for escape sequences
					out += pe;
					i++;
				} else if (wo == '\"') { //look for end of dquote
					dquote = false;
				} else if (wo == '\'') { //look for end of squote
					squote = false;
				}
			} else if (wo == '/') { //look for comments
				if (pe == '/') { //look for single-line comment
					for ( ; i<len; i++) out += in[i]; //put remainder of line
				} else if (pe== '*') { //look for multi-line comments
					mcom = true;
					out += wo;
					out += pe;
					i++;
				} else out += wo;
			} else if (wo == '{' || wo == '}') { //look for begin curly
				out += wo;
				if (pe == ';') {
					out += pe;
					i++;
				}
				out += '\n';
			} else if (wo == ';') {
				out += wo;
				if (paren_nest <= 0 && !dquote && !squote) {
					string substring = in.substr(i+1, len-i); //contains the remainder of the line
					lineType lt = lType(substring);
					if (lt != SCOMMENT && lt != MCOMMENT) {
						out += '\n';
					}
				}
			} else if (wo == 'e' && pe == 'l') {
				if (in.substr(i,4).compare("else") == 0) {
					out += " else ";
					i+=3;
				}
			} else {
				out += wo;
				if (wo == '(' ) { //look for begin parenthesis
					paren_nest++;
				} else if (wo == ')' ) { //look for end parenthesis
					paren_nest--;
				} else if (wo == '\"') { //look for begin dquote
					dquote = true;
				} else if (wo == '\'') { //look for begin squote
					squote = true;
				}
			} //end else parse some code
			fChar = true; //used to ignore leading tabs and spaces
		} //end else parse code
	} //end: for i loop
	
	return out;
}

string parser::tabs(int cur_nest) { //returns the appropriate number of tabs
	string out = "";
	if (cur_nest < 0) cout << "\nCannot place " << cur_nest << " tabs";
	if (prefs[8]!=0) { //use spaces instead of tabs?
		for (int j=0; j<cur_nest; j++) {
			for (int k=0; k<prefs[8]; k++) { //How many spaces?
				out += ' ';
			}
		}
	} else {
		for (int j=0; j<cur_nest; j++) out += '\t'; //places tabs
	}
	return out;
} //end: string tabs()


