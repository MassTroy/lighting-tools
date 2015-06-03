//includes
#include "include.h"
#include "something else"
#define zero 0

extern int nest;





class test {
public:
	string parseLine(string in);
};

int main() {
	try {
		int i = 0;
		switch (i) {
			case 0:
				i--;
				break;
			case 1:
				i++;
				break;
			default:
				i+=5;
		}
		return 0;
	} catch (...) {
		throw;
	}
}

string test::parseLine(string in) {
	if (1 == 1) { }
	else if (0 == 0) {} else {  /*There is nothing here*/ }
	// comments with     lots 		of spaces 	and tabs
	if (0 == 0)
	{
		int i; int k;
		//code out-of-line {{{{{{after int i; int k;
	}
	else
	{
		int j;
		//more code out-of-line after int j;
		cout << "This is"
			<< "a multiline"
			<< "code segment";
	}
	
	//comment after 1 blank
	if (0 == 0) {
		int a=0; } else {
	}int k;
	/*
		This is a multiline comment
		it should be 
		put spanning multiple lines
		other wise the name wouldnt make
		any sense
		It is not inline and comes after int k;
	*/
	for (int i=0; i<10; i++) {
		i =/*midline comment*/ i;
		//comment on separate line after i = i;
		i--; //comment after code after i--;
		i++;
	}
}

int anotherFunc () {
	cout << "This function does nothing";
	/*
		multi-line     comment
		with	 	lots of    spaces
		and		tabs	!
	*/
	return 0;
}
