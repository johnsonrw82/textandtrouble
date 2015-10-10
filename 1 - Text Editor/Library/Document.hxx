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
	// borrowed this mechanism of using nameless namespaces from the HW2 sample solution
	namespace {
		// forward list implmentation of size
		template<class T>
		inline auto size(const std::forward_list<T>& cont) {
			return std::distance(cont.cbegin(), cont.cend());
		}

		// generic implementation of size
		template<class ContainerType>
		inline auto size(const ContainerType& cont) {
			return cont.size();
		}

		// template function for insertion into fwd list
		template<class Iterator, class T>
		inline typename std::forward_list<T>::iterator insert(std::forward_list<T> & cont,
			typename std::forward_list<T>::const_iterator insertPoint,
			const Iterator & insertBegin,
			const Iterator & insertEnd) {

			// advance to find the point just before the insertion point, then insert after that
			auto insertBefore = std::next(cont.before_begin(), std::distance(cont.cbegin(), insertPoint));
		
			return cont.insert_after(insertBefore, insertBegin, insertEnd);
			
		}

		// template function for insertion into container
		template<class ContainerType, class Iterator>
		inline typename ContainerType::iterator insert(ContainerType & cont,
			typename ContainerType::const_iterator insertPoint,
			const Iterator & insertBegin,
			const Iterator & insertEnd) {

			return cont.insert(insertPoint, insertBegin, insertEnd);
		}

		// template function for erasing from a fwd list
		template<class Iterator, class T>
		inline typename std::forward_list<T>::iterator erase(std::forward_list<T> & cont,
			const Iterator & eraseBegin,
			const Iterator & eraseEnd) {

			// find the point just before erase begin
			auto eraseBefore = std::next(cont.before_begin(), std::distance(cont.cbegin(), eraseBegin));
			
			return cont.erase_after(eraseBefore, eraseEnd);
		}

		// template function for erasing from a container
		template<class ContainerType, class Iterator>
		inline typename ContainerType::iterator erase(ContainerType & cont,
			const Iterator & eraseBegin,
			const Iterator & eraseEnd) {

			return cont.erase(eraseBegin, eraseEnd);
		}		
	}

	// stream operator overload
	template <template<class, class> class T, class U>
	inline std::ostream & operator<< (std::ostream & s, const Document<T, U> & document) {
		for (auto word : document._document) {
			s << word << '\n';
		}

		return s;
	}


	// default constructor
	template < template<class, class> class T1, class T2 >
	Document<T1, T2>::Document() = default;
	
	// constructor initializing with input stream
	template < template<class, class> class T1, class T2 >
	Document<T1, T2>::Document(std::istream & in) {
		load(in);
	}

	// constructor initializing with string
	template < template<class, class> class T1, class T2 >
	Document<T1,T2>::Document(const std::string & phrase) {
		// construct an input stringstream and load with that
		std::istringstream iss(phrase);
		load(iss);
	}

	// copy constructor
	template < template<class, class> class T1, class T2 >
	Document<T1, T2>::Document(const Document & other) : 
		_document(other._document), _snapshots(other._snapshots), _copy_buffer(other._copy_buffer) {}

	// rvalue reference copy constructor
	template < template<class, class> class T1, class T2 >
	Document<T1, T2>::Document(Document && other) : 
		_document(other._document), _snapshots(other._snapshots), _copy_buffer(other._copy_buffer) {}

	// destructor
	template < template<class, class> class T1, class T2 >
	Document<T1, T2>::~Document() noexcept
	{

	}

	// operator overloads ------------------------------------------------
	// operator = overload for lvalue reference
	// use the copy-and-swap idiom, to guarantee that no exception will be thrown during construction
	template < template<class, class> class T1, class T2 >
	Document<T1,T2> & Document<T1, T2>::operator=(const Document &  rhs) {
		Document<T1, T2> copy(rhs);  // copy
		// swap
		std::swap(_copy_buffer, copy._copy_buffer);
		std::swap(_snapshots, copy._snapshots);
		std::swap(_document, copy._document);
		// return
		return *this;
	}

	// operator = overload for rvalue reference
	template < template<class, class> class T1, class T2 >
	Document<T1, T2> & Document<T1, T2>::operator=(Document &&  rhs) {
		this->_copy_buffer = std::move(rhs._copy_buffer);
		this->_document = std::move(rhs._document);
		this->_snapshots = std::move(rhs._snapshots);		

		return *this;
	}
	
	// push a snapshot of the document onto the stack
	template < template<class, class> class T1, class T2 >
	inline void Document<T1,T2>::snap() {
		_snapshots.push(_document);
	}
	
	// undo the specified quantity of operations
	template < template<class, class> class T1, class T2 >
	inline void Document<T1, T2>::undo(std::size_t quantity) {
		// do nothing if no quantity or greater or equal to the stack size
		if (quantity > 0 && quantity < _snapshots.size() ) {
			for (unsigned int i = 0; i < quantity; i++) {
				_document = _snapshots.top(); // pop off the documents until the desired count is reached
				_snapshots.pop();
			}
		}
	}

	// function specialization for forward_list, which uses a different mechanism for adding data to the list
	template<>
	inline void Document<std::forward_list>::load(std::istream & stream) {
		_document.clear(); // clear the document

		ValueType val;
		// iterator pointing to before the beginning of the list
		auto itr = _document.before_begin();

		// while stream data remains
		while (stream >> val) {
			// emplace item after current position, and return the new position
			itr = _document.emplace_after(itr, std::move(val));
		}
	}

	// default template version for load, which loads data from the stream into the document
	template < template<class, class> class T1, class T2 >
	inline void Document<T1, T2>::load(std::istream & stream ) {
		_document.clear(); // clear the document

		ValueType val;
		while (stream >> val) {
			_document.emplace_back(val);
		}
	}

	// copy a range of text between start phrase and end phrase
	template < template<class, class> class T1, class T2 >
	void Document<T1,T2>::copy_range(const ValueType & startingPhrase, const ValueType & endingPhrase) {
		// determine if the phrases are valid
		if ( !(startingPhrase.empty() || endingPhrase.empty()) ) {
			// turn these phrases into documents and use the underlying containers
			const ContainerType startDoc(Document(startingPhrase)._document);
			const ContainerType endDoc(Document(endingPhrase)._document);

			// search for the position of the starting phrase
			auto startPos = std::search(_document.cbegin(), _document.cend(),
				startDoc.cbegin(), startDoc.cend());

			// if the position was found before the end, we can locate the ending phrase next
			if (startPos != _document.cend()) {
				auto endPos = std::search(std::next(startPos, Library::size(startDoc)), _document.cend(),
					endDoc.cbegin(), endDoc.cend());

				// if the end position was found before the document end, we can assign this range to the copy buffer
				if (endPos != _document.cend()) {
					_copy_buffer.assign(startPos, std::next(endPos, Library::size(endDoc)));
				}
			}
		}
	}

	// paste the contents of copy_buffer after the occurence of phrase
	template < template<class, class> class T1, class T2 >
	void Document<T1, T2>::paste_after(const ValueType & pasteAfterPhrase) {
		// check if the phrase is empty
		if (!pasteAfterPhrase.empty()) {
			const ContainerType pasteDoc(Document(pasteAfterPhrase)._document);

			// search for the position
			auto pos = std::search(_document.cbegin(), _document.cend(),
				pasteDoc.cbegin(), pasteDoc.cend());

			// if the position is found before the end of the document, we can paste
			if (pos != _document.cend()) {
				// library function to insert to generic container				
				Library::insert(_document, std::next(pos, Library::size(pasteDoc)), _copy_buffer.cbegin(), _copy_buffer.cend());
			}
		}
	}

	// erase all instances of phrase
	template < template<class, class> class T1, class T2 >
	void Document<T1, T2>::erase(const ValueType & phrase) {
		// check to make sure phrase is valid
		if (!phrase.empty()) {
			// turn the phrase into a document to use the underlying container
			const ContainerType phraseDoc(Document(phrase)._document);

			// find the position of the first occurence of phrase in the document
			auto pos = std::search(_document.cbegin(), _document.cend(),
				phraseDoc.cbegin(), phraseDoc.cend());

			while (pos != _document.end()) {
				// erase the range from pos to the next element after the phrase, and return the next element from that
				pos = Library::erase(_document, pos, std::next(pos, Library::size(phraseDoc)));
				// search starting from the returned position
				pos = std::search(pos, _document.cend(),
					phraseDoc.cbegin(), phraseDoc.cend());
			}
		}
	}

	// substitute all instances of a phrase with another phrase
	template < template<class, class> class T1, class T2 >
	void Document<T1, T2>::substitute(const ValueType & oldPhrase, const ValueType & newPhrase) {
		// check validity
		if (!(oldPhrase.empty() || newPhrase.empty())) {
			// turn the phrases into documents and get underlying containers
			const ContainerType oldPhraseDoc(Document(oldPhrase)._document);
			const ContainerType newPhraseDoc(Document(newPhrase)._document);

			// find the position of oldPhrase
			auto pos = std::search(_document.cbegin(), _document.cend(),
				oldPhraseDoc.cbegin(), oldPhraseDoc.cend());

			// if we're not at the end of the document
			while (pos != _document.end()) {
				// erase the old phrase
				pos = Library::erase(_document, pos, std::next(pos, Library::size(oldPhraseDoc)));
				// insert the new one at the position returned from last operation
				pos = Library::insert(_document, pos, newPhraseDoc.cbegin(), newPhraseDoc.cend());

				// keep searching for the phrase
				pos = std::search(pos, _document.cend(),
					oldPhraseDoc.cbegin(), oldPhraseDoc.cend());
			}
		}

	}
        
        template < template<class, class> class T1, class T2 >
	void Document<T1, T2>::showStats(std::ostream & os) {
                // statistics container
                StatContainerType           _statistics;
                typename ContainerType::size_type maxOcc = 0;
                
                // define regular expression to match one or more alphanumeric characters
                std::regex wordRegex("[0-9a-zA-Z]+");
                // read through each word and gather the statistics
                for ( const auto word : _document ) {
                        std::string tmp(word);  // make a copy of the word to use in the loop

                        // match results
                        std::smatch results;
                        // split the word on non alphanumeric characters
                        while ( std::regex_search(tmp, results, wordRegex ) ) {
                                // loop through all the matches
                                for ( auto match : results ) {
                                        // dont want to modify the contents of the word
                                        std::string lower(match);
                                        // transform to lowercase
                                        std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);
                                        
                                        // increment the number of occurrences and store in the map
                                        ++_statistics[lower];
                                        
                                        if (_statistics[lower] > maxOcc) {
                                                maxOcc = _statistics[lower];
                                        }
                                }
                                
                                // use the rest of the string in subsequent matching
                                tmp = results.suffix().str();
                        }
                }
                
                // lambda compare function, used for ordering the set
                auto compareTo = [](const auto * lhs, const auto * rhs) {
                        // order by count first
                        if ( lhs->second != rhs->second ) {
                                return lhs->second > rhs->second;
                        }
                        // then by word
                        return lhs->first < rhs->first;
                };
                
                // create the chart object
                std::set<typename StatContainerType::value_type *, decltype(compareTo)> chart(compareTo);
                
                // make pair set from the map
                for ( auto & pair : _statistics ) {
                        chart.insert(&pair);
                }
                
                // save the flags of the stream
                auto fmtFlags = os.flags();
                
                // format the chart, but need to establish some sizing
                const unsigned int keySize = 20;
                const unsigned int countSize = 5;
                const unsigned int histSize = 80; // standard length of most terminals
                
                // compute the scaled size based on max occurrences
                typename ContainerType::size_type scaleValue = maxOcc / histSize + 1;
                
                // display the chart header
                os << std::setw(keySize) << std::right << "Word " << " | " <<
                        std::setw(countSize) << std::right << "Count" <<
                        std::left << "   (* is rounded to nearest " << scaleValue << " occurrences)\n" <<
                        std::string(keySize + countSize + histSize + 5, '-') << '\n';
                
                // chart is now sorted, let's display the info
                for ( const auto & pair : chart ) {
                        os << std::setw(keySize) << std::right << pair->first.substr(0, keySize) <<
                                (pair->first.size() > keySize ? "* |" : " | ") <<
                                std::setw(countSize) << std::right << pair->second <<
                                "   " << std::left << std::string((pair->second + scaleValue/2) / scaleValue, '*') << '\n';
                                
                }
                
                // restore
                os.setf(fmtFlags);
        }
}

#endif
