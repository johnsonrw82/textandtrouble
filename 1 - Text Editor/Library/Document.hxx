/**
* File:		Document.hxx
* Author:	Ryan Johnson
* Email:	johnsonrw82@cs.fullerton.edu
* Purpose:	This file is the header extension containing the implementation of the interface described in Document.hpp.
*			The functions provided are those of typical operations found in a word processing application.
*			Supported operations are:
*			-snapshot/undo
*			-search/replace
*			-erase
*			-copy/paste
*/

#ifndef DOCUMENT_HXX
#define DOCUMENT_HXX

// includes
#include <vector>
#include <forward_list>
#include <utility>
#include <string>
#include <sstream>
#include <algorithm>
#include <regex>
#include <iomanip>

#include "Library/Document.hpp"

namespace Library {
	// stream operator overload
	inline std::ostream & operator<< (std::ostream & s, const Document & document) {
		s << document._document;

		return s;
	}
	
	// push a snapshot of the document onto the stack
	inline void Document::snap() {
		_snapshots.push(_document);
	}
	
	// undo the specified quantity of operations
	inline void Document::undo(std::size_t quantity) {
		// do nothing if no quantity or greater or equal to the stack size
		if (quantity > 0 && quantity < _snapshots.size() ) {
			for (unsigned int i = 0; i < quantity; i++) {
				_document = _snapshots.top(); // pop off the documents until the desired count is reached
				_snapshots.pop();
			}
		}
	}

	// loads data from the stream into the document
	inline void Document::load(std::istream & in ) {
		_document.clear(); // clear the document

                // read the buffer directly
                std::ostringstream oss;
                oss << in.rdbuf();
                // use stringstream since the underlying container is std::string
                _document = oss.str();
        }
}

#endif
