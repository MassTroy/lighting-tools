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
extern int nest;
extern int prefs[];

ifile::ifile(char Name[]) {
	curLine = new string("");
	nextLine = new string("");
	eof=false;
	stream = new ifstream();
	stream->open(Name,ios::in); //opens input filestream
	if (!stream->is_open()) { cout << "\nERROR: Can not open input file \"" << Name << "\"\n"; throw new string("Can not open input file"); }
	this->getLn(); //pulls in the first line to set curLine and nextLine at correct locations
}
void ifile::close () {
	stream->close(); //closes the filestream
	delete stream; //deallocates memory
}
string ifile::getLn() {
	curLine->assign(nextLine->data()); //moves line up
	nextLine->assign("");
	if (stream->good()) std::getline(*stream,*nextLine);
	else eof=true;
	return curLine->data();
}
string ifile::curLn() {
	return curLine->data();
}
string ifile::peekLn() {
	return nextLine->data();
}
bool ifile::hasNext() {
	return !eof;
}


ofile::ofile(char Name[]) {
	stream = new ofstream();
	stream->open(Name,ios::out | ios::trunc);
	if (!stream) { cout << "\nERROR: Can not open output file \"" << Name << "\"\n"; throw new string("Can not open output file"); }
}
void ofile::close() {
	stream->close();
	delete stream;
}
void ofile::putLine(string line) {
	*stream << line;
}
void ofile::operator<<(string line) {
	*stream << line;
}
