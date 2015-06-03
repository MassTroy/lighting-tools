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
#ifdef __WIN32__
	#include <windows.h> //used for registry key reading
	#include <stdlib.h> //used for getenv()
#endif
#ifdef __unix__
	#include <unistd.h>
	#include <sys/types.h>
	#include <pwd.h>
#endif

int prefs[14] = {
	1, 	//0 dummy pref
			//program prefs
	1, 	//1 same_file = save formatted version in same file
	1, 	//2 create_backup = create a backup file of unformatted
	1, 	//3 verbose = output what it is doing to the screen
			//style prefs
	0, 	//4 inline_curly = curly braces in line with control statement
	0, 	//5 code_after_curl = first line of code on same line as { 		//NOT IMPLIMENTED
	0, 	//6 start_tabs = how far to start indenting at
	0, 	//7 convert_comments = changes one line comment to multi-line style
	0, 	//8 tabs_to_spaces = use spaces instead of tabs. 0=false, nonezero is number of spaces to replace with

			//removal prefs
	0, 	//9 remove_blank = removes blank lines
	0, 	//10 remove_comments = removes comments entirely
	0, 	//11 remove_tabs = removes all tabs
	0, 	//12 remove_endln = everything on one line. Sets convert_comments, remove_lines, and remove_tabs to 1 (true)
	0 	//13 remove_code = outputs only human-readable (comments and outputs) portions for spellchecking 		//NOT IMPLIMENTED
};

void loadOpts() {
	//streams
	ifstream optFile;
	
	char* optsFileName = "lformat-prefs.txt";
	
	//TODO: ERROR!!!! must have exact file path to options file.
	/* 	
		//DETAILS: the options file only works if in the current directory or if given an exact file path. 
		Either the path needs to be hard coded in (platform and installation directory specific) or some other method 
		needs to be devised. Perhaps a registry key pointing to the options file.
		Add a patch, create a command line arg to specify path. The patch could also be a feature such that specifying 
		different options files for each format is possible.
	*/
	
	/*
		//DESCRIPTION: The program supports multiple options files. By default it will choose the file in the current 
		working directory. Otherwise it will revert to using the file located in the install directory. 
		
		LINUX: To determine where the file is, it will read the file "~/.lightning-format/lightning-format.conf", 
		then it tries "/etc/lightning-format.conf" which will contain the path to the options file without any 
		spaces before or after.
		
		WINDOWS: Read a registry key to determine the location of the conf file.
	*/
	optFile.open(optsFileName, ios::in ); //working directory
	if (!optFile.is_open()) {
		#ifdef __unix__
			string homedir = "";
			if (true) { //get user's home directory
				passwd *pw = getpwuid(getuid());
				homedir = pw->pw_dir;
			}
			
			ifstream confFile;
			string confFileName = homedir + "/.lightning-format/lightning-format.conf"; //current user conf
			confFile.open(confFileName.c_str(), ios::in );
			string path = homedir + "/.lightning-format/lformat-prefs.txt"; //tryies this path if not overridden by either conf file
			if (confFile) {
				getline(confFile, path);
				if (path[path.length()-1] == '/') {
					path += optsFileName;
				}
			} else {
				confFileName = "/etc/lightning-tools/lightning-format.conf"; //all users conf
				confFile.open(confFileName.c_str(), ios::in );
				if (confFile) {
					getline(confFile, path);
					path = path.substr(0,path.length()-1); //silly line but for some reason is needed
					if (path[path.length()-1] == '/') {
						path += optsFileName;
					}
				}
			}
			optFile.open(path.c_str(), ios::in ); //installation configured directory
			if (optFile.is_open()) {
				cout << "Using Options file: \"" << path << "\"\n";
			}
		#endif
		#ifdef __WIN32__
			string path = getenv("SYSTEMDRIVE");
			path += "\\bin\\ltools\\"; //tryies this path if not overridden by registry key data
			
			/*
				//TODO: Registry key reading is NOT functioning
				// Read the registry key
				char buf[255] = {0};
				HKEY hKey = 0; DWORD dwType = 0; DWORD dwBufSize = sizeof(buf); //variables needed to open the key
				const char* subkey = "SOFTWARE\\lightning-tools";
				const char* keyName = "PrefPath";
		
				if( RegOpenKey(HKEY_LOCAL_MACHINE,subkey,&hKey) == ERROR_SUCCESS) { //open the key
					dwType = REG_SZ;
					if( RegQueryValueEx(hKey,keyName,0, &dwType, (BYTE*)buf, &dwBufSize) == ERROR_SUCCESS) { //read the value
						path = buf;
						//cin.ignore(); //not sure why this is here, but it might be needed
					} else
						RegCloseKey(hKey);
				}
				//OPTIMIZE: Create a all users configuration directory
			*/
			
			if (path[path.length()-1] == '\\') { //if last letter is a backslash
				path += optsFileName; //append filename
			}
			optFile.open(path.c_str(), ios::in ); //installation configured directory
			if (optFile.is_open()) {
				cout << "Using Options file: \"" << path << "\"\n";
			}
		#endif
	} else {
		cout << "Using Options file: \"" << optsFileName << "\"\n";
	}
	
	if (optFile.is_open()) {
		while (optFile) { //read until eof 
			string in;
			getline(optFile, in); //reads line of file into in
		
			string name="", val="";
			bool b = false; //flag for difference between name and val
			int len = in.length();
		
			for (int i=0; i<len; i++) {
				char wo = in[i];
				char pe = ' '; //peek char
				if (i<len-1) pe = in[i+1]; //fills peek char
				if ( (int)wo <= 32 ) {
					//ignore spaces and tabs
				} else if (wo == '=') {
					b = true;
				} else if ((wo == '/' && pe == '/') || wo == '#') { //supports both double slash comments and # comments
					i=len+1;
					break;
				} else {
					if (b == false) {
						name += wo;
					} else {
						val += wo;
					}
				}
				//TODO: make option names non case sensitive
				//TODO: create ignore code for bad or blank VALUES
			} //done parsing
		
			if (name.length() == 0) {
				//do nothing
			} else if ( contains(name,"same_file") ) {
				istringstream(val) >> prefs[1];
			} else if ( contains(name,"create_backup") ) {
				istringstream(val) >> prefs[2];
			} else if ( contains(name,"verbose") ) {
				istringstream(val) >> prefs[3];
			} else if ( contains(name,"inline_curly") ) {
				istringstream(val) >> prefs[4];
			} else if ( contains(name,"start_tabs") ) {
				istringstream(val) >> prefs[6];
			} else if ( contains(name,"convert_comments") ) {
				istringstream(val) >> prefs[7];
			} else if ( contains(name,"tabs_to_spaces") ) {
				istringstream(val) >> prefs[8];
			} else if ( contains(name,"remove_blank") ) {
				istringstream(val) >> prefs[9];
			} else if ( contains(name,"remove_comments") ) {
				istringstream(val) >> prefs[10];
			} else if ( contains(name,"remove_tabs") ) {
				istringstream(val) >> prefs[11];
			} else if ( contains(name,"remove_endln") ) {
				istringstream(val) >> prefs[12];
			} else { //not reconized
				cout << "WARNING: Unreconized option: " << name << "=" << val << "\n";
			} //bad option
		}

		optFile.close();
	} else {
		cout << "\nWARNING: No Options file \"" << optsFileName << "\"";
	}
	
	//repair
	if (prefs[12] != 0) {
		prefs[7] = prefs[9] = prefs[11] = 1; //Sets convert_comments, remove_lines, and remove_tabs to 1 (true)
	}
	/**/
} //end: void loadOpts()

void editOpts() {
	//TODO: STUB: create command line option editor tool
	cout << "Sorry, this feature is not implimented yet :(\n";
} //end: void editOpts()

void displayOpts() {
	cout << "same_file=" << prefs[1];
	cout << "\ncreate_backup=" << prefs[2];
	cout << "\nverbose=" << prefs[3];
	cout << "\ninline_curly=" << prefs[4];
	cout << "\nstart_tabs=" << prefs[6];


	cout << "\nconvert_comments=" << prefs[7];
	cout << "\ntabs_to_spaces=" << prefs[8];
	cout << "\nremove_lines=" << prefs[9];
	cout << "\nremove_comments=" << prefs[10];
	cout << "\nremove_tabs=" << prefs[11];
	cout << "\nremove_endln=" << prefs[12];
} //end: void displayOpts()
