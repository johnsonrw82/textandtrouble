/******************************************************************************
** (C) Copyright 2015 by Thomas Bettens. All Rights Reserved.
**
** DISCLAIMER: The authors have used their best efforts in preparing this
** code. These efforts include the development, research, and testing of the
** theories and programs to determine their effectiveness. The authors make no
** warranty of any kind, expressed or implied, with regard to these programs or
** to the documentation contained within. The authors shall not be liable in
** any event for incidental or consequential damages in connection with, or
** arising out of, the furnishing, performance, or use of these libraries and
** programs.  Distribution without written consent from the authors is
** prohibited.
******************************************************************************/

/**************************************************
** Intermediate C++ Homework Assignment #2 Possible Solution
**
** Thomas Bettens
** Last modified:  29-March-2015
** Last modified:  29-April 2014
** Last Verified:  29-April 2014
** Verified with:  VC++2015 CTP 6, GCC 4.9.2,  Clang 3.5
***************************************************/


#ifndef DOCUMENT_HPP
#define DOCUMENT_HPP

#include <cstddef>      // size_t
#include <iostream>
#include <stack>
#include <string>
#include <vector>

namespace Library
{
  /****************************************************************
  ** One of the goals of this assignment is to easily change the underlying container type. Homework 1 demonstrated how a type alias
  ** can be used to localize the changes necessary when changing the underlying container. We haven't formally treated template
  ** classes in detail yet, I did introduce them in the Fundamentals course. I want to re-introduce the topic again here. Templates
  ** are easy to read, but can be intimidating to write. You are welcome to implement this assignment using the pre-processor
  ** technique presented in homework 1. But by providing you this class template header file, my hope is that you'll embrace this
  ** class template technique in your solution. In the long run, a template solution is a better way to go and you'll see more of
  ** them as the course moves forward. Instead of a constant type definition, think of the template parameters as a variable type
  ** definition to be resolved at compile time. Functionally, All we're doing here is allowing the client of this class to choose
  ** the underlying container. All we have to do is to design the class to be container neutral.
  *****************************************************************/

  // Allows using Document like this:
  //        Document<std::vector>, or Document<std::list>
  //   but sadly, not this:
  //        Document<std::array> 
  template < template<class, class> class T1 = std::vector, class T2 = std::string >

  class Document
  {
    template <template<class, class> class T, class U>
    friend std::ostream & operator<< (std::ostream & s, const Document<T, U> & document);


    public:
      using ValueType     = T2;
      using ContainerType = T1< ValueType, std::allocator<ValueType> >;

      // Constructors and destructor
      Document();
      Document( std::istream & in );
      Document( const std::string & phrase );

      Document( const Document &  other );
      Document(       Document && other );

      Document & operator=(const Document &  rhs);
      Document & operator=(      Document && rhs);

      ~Document() noexcept;



      // Load this object with the contents of a stream
      void load(std::istream & stream = std::cin);


      // Snapshot a point in time the document can be restored to later
      void snap();
      void undo( std::size_t quantity = 1U);


      // Copy a section of the document so it can be inserted later
      void copy_range   (const ValueType & startingPhrase, const ValueType & endingPhrase); 
      void paste_after  (const ValueType & pasteAfterPhrase);                              


      // Remove all occurrences of a phrase. The assignment wants a delete function, but "delete" is a C++ reserved key word.  So,
      // following the STL's precedence, I used erase instead.
      void erase        (const ValueType & phrase);


      // Replace all occurrences of a phrase with another phrase
      void substitute   (const ValueType & oldPhrase, const ValueType & newPhrase);



    private:
      ContainerType               _document;
      std::stack<ContainerType>   _snapshots;      // Undo buffer allowing multiple levels of undo
      ContainerType               _copy_buffer;    // Copy buffer holding a copy of some portion of the document

  };  // class Document
}  // namespace Library





// Inline functions and template definitions must be part of the header file. To help keep the size of the header file manageable
// and to keep implementation definitions separated from interface declaration, the header file has been divided into two.
// Library/Document.hxx is not intended to be included by clients and serves only to allow Document and related declarations to be
// separated from implementation whist making inline function and template definitions visible to all translation units. This
// technique is primary used as a teaching tool. I don't see this in real-world applications often, but it's out there. It allows me
// to distribute the declarations without the definitions. Normally, the contents of "Library/Document.hxx" would be physically
// entered below instead of telling the pre-processor to include the contents.
#include "Library/Document.hxx"       // inline and function Document definitions

#endif
