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

//g++ -combine main.cpp functions.cpp classes.cpp parseLine.cpp options.cpp -o tformat

/* list of options for reference only. Do not uncomment!
//int prefs[14] = {
//	1, 	//0 dummy pref
//			//program prefs
//	1, 	//1 same_file = save formatted version in same file
//	1, 	//2 create_backup = create a backup file of unformatted
//	1, 	//3 verbose = output what it is doing to the screen
//			//style prefs
//	0, 	//4 inline_curly = curly braces in line with control statement
//	0, 	//5 code_after_curl = first line of code on same line as { 		//NOT IMPLIMENTED
//	0, 	//6 start_tabs = how far to start indenting at
//	0, 	//7 convert_comments = changes one line comment to multi-line style
//	0, 	//8 tabs_to_spaces = use spaces instead of tabs. 0=false, nonezero is number of spaces to replace with
//
//			//removal prefs
//	0, 	//9 remove_blank = removes blank lines
//	0, 	//10 remove_comments = removes comments entirely
//	0, 	//11 remove_tabs = removes all tabs
//	0, 	//12 remove_endln = everything on one line. Sets convert_comments, remove_lines, and remove_tabs to 1 (true)
//	0 	//13 remove_code = outputs only human-readable (comments and outputs) portions for spellchecking 		//NOT IMPLIMENTED
//*/

//New features to add soon
//TODO: FEATURE: Add ternary support (and conversion)
//TODO: FEATURE: Add option for single line blocks (place 1 line blocks in line
//TODO: FEATURE: indent switch/select blocks even further
//TODO: FEATURE: add support for multiline array blocks and multiline arguements. Perhaps parse looking for comma at the end?

//includes
#include "include.h"

extern int nest; //how many indented {} blocks
extern int prefs[]; //preferences data array

string parser::parseLine(string in) {
	if (prefs[3]!=0) { cout << "."; }
	//strings
	string out = ""; //working output string
	string substring = ""; //string before recursively parsed
	string subline = ""; //any recursed strings
	string final = ""; //string just before outputing
	
	//parsing states
	const int start_nest = nest;
	int cur_nest = nest; //how much the current line should be nested
	int paren_nest = 0; //how many nested parenthesis
	bool dquote = false; //inside of a "" block
	bool squote = false; //inside of a '' block
	static bool mcom = false; //inside a multi-line comment
	bool scom = false; //inside a single line comment
	
	//temporary flags
	bool includeTabs = true; //include tabs before this line?
	bool includeNL = true; //include nl before this line?
	bool forcePostNL = false; //force a nl after this line
	static bool dentNext = false; //indent the next line for multiline statements of code
	bool dentThis = dentNext | mcom; //corresponds to dentNext. also dents this if in a mcom
	bool fCom = false; //did I find a comment on this line //used for removing comments
	bool addEndMCom = false; //used for converting comments.
	bool recurse = false; //used to flag a recurse outside of the parse loop
	static bool endedNL = false; //was a nl forced at the end of the last line

	//loop variables
	int len = in.length(); //length of input string
	int i=0; //current possition
	bool fChar = false; //used for ignoreing leading spaces and tabs
	bool fEnd = false; //foundEnd Of Line: used when a end of line symbol is found
	bool tEnd = false; //triggerEnd Of Line: forcefully triggers the end of line (found '\n' flag)
	
	////////////////////////////// PRE-PARSE //////////////////////////////
	if (prefs[11] != 0) includeTabs = false;
	if (prefs[12] != 0) { //removing all NL?
		includeNL = includeTabs = false;
	}
	lineType t = lType(in); //determine line type
	dentNext = false;
	if (t == PREPROCESSOR) {
		includeNL = true;
		if (prefs[12] != 0) forcePostNL = true;
	} else if (t == SCOMMENT && !mcom && prefs[10] != 0) {
		includeNL = includeTabs = false;
		i = len; //skips parsing to speed things up
		fCom = true;
	} else if (mcom || t == MCOMMENT) {
		fCom = true;
	} else if (t == BLANK) { //IMPORTANT BLOCK. Sets no dentNext for these items
		//do not merge the if below with the one above!
		if (prefs[9] != 0) { //removing blank lines?
			includeNL = includeTabs = false;
			i = len;
		}
	} else if (t == SCOMMENT) {
		//do not merge the if below with the one above!
		if (prefs[10] != 0) { //removing comments
			includeNL = includeTabs = false;
			i = len;
		}
	} else {
		dentNext=true;
	}
	
	////////////////////////////// BEGIN PARSE //////////////////////////////
	for ( ; i<len; i++) {
		register char wo = in[i]; //fills working char
		char pe = ' '; //peek char
		if (i<len-1) pe = in[i+1]; //fills peek char
		
		if ( !fChar && (int)wo <= 32 && wo != '\n') { //ignore leading whitespace
			//do nothing
		} else if (tEnd || fEnd) { ////////////////////////////// END OF LINE //////////////////////////////  //if found the end of a line
			substring = in.substr(i, len-i); //contains the remainder of the line
			if (!tEnd) {
				lineType lt = lType(substring);
				if (lt == SCOMMENT || lt == MCOMMENT || lt == BLANK) {
					i--; //reparse this char
					fEnd = false; //continue parsing the line normally
				}
			}
			if (tEnd || fEnd) { //CODE
				recurse = true; //tell it to recurse after its done
				i = len; //ends the parsing loop
			}
		} else if (wo == '\n') {
			//OPTIMIZE: searching for newline could be useless thanks to how the new safeParse works.
			#ifdef DEBUG //use this block to test
				cout << "Found NL ";
			#endif
			tEnd = true;
			dentNext = false;
		} else { ////////////////////////////// PARSE CODE //////////////////////////////  //Parse all code
			if (scom) {
				if (prefs[10]==0) { //dont remove comments
					out += wo;
				}
			} else if (mcom) { //inside a multi-line comment
				if (wo == '*' && pe == '/') { //look for end of multi line if in one
					dentNext = false;
					dentThis = false;
					mcom = false;
					i++;
					if (prefs[10]==0) { //dont remove comments
						out += wo;
						out += pe;
					}
					//nest--;
				} else if (prefs[10]==0) { //dont remove comments
					out += wo;
				}
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
					if (prefs[10]==0) { //allows comments
						if (prefs[7] != 0) { //convert comments
							out += "/*";
							i++;
							addEndMCom = true; //after next nl, it will add the [star-slash]
						} else {
							out += wo;
							out += pe;
							i++;
						}
					}
					scom = true;
					fCom = true;
				} else if (pe== '*') { //look for multi-line comments
					mcom = true;
					//nest++;
					if (prefs[10]==0) { //If allowed to output comments
						out += wo;
						out += pe;
						i++;
					}
					dentNext = true;
					fCom = true;
				} else out += wo;
			} else if (wo == '{') { //look for begin curly
				if (prefs[4] == 0 && fChar) { //if out of line curlies
					i--; //repass this char
				} else {
					out += wo;
					cur_nest = nest;
					nest++;
				}
				dentThis = false;
				fEnd = true;
				dentNext = false;
				/*
				//code to handle if you type " ... { } ... "
				//TODO: if adding back in, this will need to be fixed
				string substr = in.substr(i+1,len-i-1);
				lineType lt = lType(substr);
				if (lt == ECURL && prefs[4] != 0) { //keep "...{final += ' '; }..." in line
					while (wo != '}') { //searches to the '}' and outputs it as well
						i++;
						wo = in[i];
						out += wo;
					}
				} else {
					nest++;
					fEnd = true;
					if (prefs[4] != 0 && fChar == false) { //make this inline if it wasnt inline already (aka: it was first char)
						includeNL = false;
						includeTabs = false;
					}
				}
				*/
			} else if (wo == '}') { //look for end curly
				if (prefs[4] == 0 && fChar) { //if out of line curlies
					i--; //repass this char
					fEnd = true;
				} else {
					out += wo;
					if (pe == ';') { //supports end of class " }; "
						out += pe;
						i++;
					}
					nest--;
					cur_nest = start_nest-1;string substr = in.substr(i+1,len-i-1);
					lineType lt = lType(substr);
					if (!(lt == ELSE && prefs[4] != 0)) { //keep "} else..." in line
						fEnd = true;
					}
					dentThis = false;
				}
				dentNext=false;
			} else { //PARSE CODE that does not require special outputting
				out += wo;
				if ( wo == ';' && paren_nest <= 0 ) { //look for end semicolon
					fEnd = true;
					dentNext = false;
				} else if (wo == ',') { //TODO: make this better support multiline parameter lists and array initializers.
					dentNext = false;
				} else if ( wo == ':') {
					if (pe == ':') {
						out += pe;
						i++;
					} else {
						fEnd = true;
						dentNext = false;
						cur_nest = start_nest-1;
					}
				} else if (wo == '(' ) { //look for begin parenthesis
					paren_nest++;
				} else if (wo == ')' ) { //look for end parenthesis
					paren_nest--;
				} else if (wo == '\"') { //look for begin dquote
					dquote = true;
				} else if (wo == '\'') { //look for begin squote
					squote = true;
				} 
				// else { //do nothing special }
			} //end else parse some code
			fChar = true; //used to ignore leading tabs and spaces
		} //end else parse code
	} //end: for i loop
	
	////////////////////////////// POST-PARSE //////////////////////////////
	if (addEndMCom) {
		out += "*/";
	}
	
	if (!endedNL) { //if a new line was not forced on the end of the last line
		if (includeNL) final += '\n'; //add newline if prefs dont say to remove all endlns
		else final += ' '; //add space if not adding newline
	}
	
	if (forcePostNL) {
		out += "\n";
		endedNL = true;
	} else endedNL = false;
	
	if (recurse) subline = this->parseLine(substring); //recurse if needed
	lineType lto = lType(out);
	if (fCom && lto == BLANK) includeNL = includeTabs = false; //this means that all that was on the line was a comment and it was removed
	
	if (includeTabs) {
		if (dentThis) final += this->tabs(cur_nest+1);
		else final += this->tabs(cur_nest);
	}
	
	final += out;
	final += subline;
	return final;
} //end: string parseLine(string in)
