/**
* File:		main.cpp
* Author:	Ryan Johnson
* Email:	johnsonrw82@cs.fullerton.edu
* Purpose:	Main file containing main routine and simulated edit session code, which tests and compares
*		the times of a large text editing operation using different STL containers and prints them
*		to stdout.
*/

#include "Library/Document.hpp"
#include "Library/Timer.hpp"

#include <typeinfo>
#include <forward_list>
#include <deque>
#include <vector>
#include <list>

using Library::Document;

namespace {
	// template function that simulates a document edit session
	template<class T>
	void testDocument(T && doc, std::istream & is) {
		std::cout << "Testing Document with " << typeid(T).name() << "\n------------------------------------------------\n";		

		// reset the stream so it can be used again
		is.clear();
		is.seekg(0);

		// load the document from the stream
		doc.load(is);

		// snapshot of the document
		Library::Timer("Elapsed Time of ::snap() \t= "), doc.snap();;
		
		// make a change
		Library::Timer("Elapsed Time of ::erase() \t= "), doc.erase("a string");
		
		// undo the change
		Library::Timer("Elapsed Time of ::undo(1) \t= "), doc.undo(1);

		// copy range
		Library::Timer("Elapsed Time of ::copy_range() \t= "), doc.copy_range("is a directive", "argument less");

		// paste after
		Library::Timer("Elapsed Time of ::paste_after() \t= "), doc.paste_after("whose remaining");

		// substitution
		Library::Timer("Elapsed Time of ::substitute() \t= "), doc.substitute("effective length", "ineffective measurement");

		// erase a not-found phrase
		Library::Timer("Elapsed Time of ::erase() \t= "), doc.erase("there's no way this phrase is in there");

		std::cout << '\n';
	}
}

// main routine
int main() {
	// test the document functions using each of the STL containers specified
	//testDocument(Document<std::vector>(), std::cin);
	//testDocument(Document<std::list>(), std::cin);
	//testDocument(Document<std::deque>(), std::cin);
	//testDocument(Document<std::forward_list>(), std::cin);

        // initialize another document from stdin
        Document<std::vector> d(std::cin);
        // show stats for the document, print to stdout
        d.showStats(std::cout);
        
	return 0;
}

