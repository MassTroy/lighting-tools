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


bool contains (string main, string key) //method to check if arg1 contains arg2
{
	int i=0;
	int j=0;
	bool started = false;
	for (i=0; i<main.length() && j<key.length(); i++) {
		if (main[i] == key[j]) {
			started = true;
			j++;
		} else {
			if (started) started = false; j=0;
		}
	}
	if (j==key.length()) return started;
	else return false;
}


void help () { //displays help content
	cout << "\n" << NAME << " is a tool for formatting text file source code.\n"
		<< "It supports languages such as C, C++, C#, and Java.\n"
		<< "Other Languages with similar formats can also be formatted.\n"
		<< "More info at <http://code.google.com/p/lightning-tools>\n\n"
		<< "Usage:\n"
		<< "\t" << EXE_NAME << " [source]\n"
		<< "\t\t-formats and overwrites source file leaving a backup file\n"
		<< "\t" << EXE_NAME << " [source] [dest]\n"
		<< "\t\t-formats source file and saves in dest\n\n"
		<< "\t--copying \t-Displays copyright information\n"
		<< "\t--display \t\t-Displays current options\n"
		<< "\t--edit \t\t-Edit the options file\n"
		<< "\t--help \t\t-Displays this help content\n\n";
	cout << "\t\t" << NAME << " version: " << VERSION << "\n"
		<< "\t\tCOPYRIGHT " << COPYRIGHT << "\n"
		<< "\t\tLicense GPLv3+: <http://www.gnu.org/licenses/gpl.html>\n"
		<< "\t\tCompiled: " << __TIMESTAMP__ << "\n\n";
}


void copying() { //displays copyright information
	cout << "\nCOPYRIGHT " << COPYRIGHT << "\n"
		<< "\nThis file is part of " << NAME << ".\n\n"
		<< NAME << " is free software: you can redistribute it and/or modify it \nunder the terms of the GNU General Public License as published by the Free \nSoftware Foundation, either version 3 of the License, or (at your option) any \nlater version.\n\n"
		<< NAME << " is distributed in the hope that it will be useful, but WITHOUT \nANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS \nFOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.\n\n"
		<< "You should have received a copy of the GNU General Public License along with \n" << NAME << ". If not, see <http://www.gnu.org/licenses/gpl.html>.\n\n";
	cout << "\t" << NAME << " version: " << VERSION << "\n"
		<< "\tCompiled: " << __TIMESTAMP__ << "\n\n";
}


void backup (char orgFile[], char bakFile[]) { //creates backup file
	if (prefs[3]!=0) cout << "Creating backup... ";
	ifstream readOld;
	readOld.open(orgFile, ios::in ); //this is the original file
	if (!readOld) { cout << "\nERROR: Can not open input file \"" << orgFile << "\""; throw "Input file could not be read"; }
	ofstream writeOld;
	writeOld.open(bakFile, ios::out ); //this is the backup file
	if (!writeOld) { cout << "\nERROR: Can not open backup file \"" << bakFile << "\""; throw "Output file could not be read"; }
	string line;
	while (readOld) { //read until eof
		getline(readOld, line);
		writeOld << line << "\n";
	}
	readOld.close();
	writeOld.close();
	if (prefs[3]!=0) cout << "Done!\n";
}
