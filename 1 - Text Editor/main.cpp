/**
* File:		main.cpp
* Author:	Ryan Johnson
* Email:	johnsonrw82@cs.fullerton.edu
* Purpose:	Main file containing main routine and simulated edit session code, which tests and compares
*		the times of a large text editing operation using std::string as the underlying document container.
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
	void testDocument(T && doc) {
		std::cout << "Testing Document with " << typeid(T).name() << "\n------------------------------------------------\n";

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

                // show stats
                Library::Timer("Elapsed Time of ::showStats() \t= "), doc.showStats(std::clog);
                
		std::cout << '\n';
	}
}

// main routine
int main() {
        // Make two documents
        Document d1(std::cin);
        Document d2 = d1;
        
	// test the document functions using each of the STL containers specified
	testDocument(d1);
        
        // show stats using second document (unmodified)
        d2.showStats();
               
	return 0;
}

