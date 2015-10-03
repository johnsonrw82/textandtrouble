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


#include <iostream>
#include <sstream>
#include <string>

#include "TroubleTicket.hpp"

//Class attribute storage
unsigned long TroubleTicket::ticketIDs = 0UL;


//Member function definitions
std::string  TroubleTicket::toString() const
{
  std::ostringstream temp;

  temp  << "ID=" << _ticketNumber << ", Priority=" << _priority << ", Status=" << _status << '\n'
        << "Description:\n" << _description << '\n';

  if( ! _resolution.empty() )  temp  << "Resolution:\n" << _resolution << '\n';

  return temp.str();
}





std::string TroubleTicket::Priority::toString(Type p)
{
  //The problem with this implementation is keeping the names of the enumerated types in sync with the
  // order and number of enumeration values.  There are better ways to do this.
  static char const * names[] = 
  {
    "None", "Low", "Normal", "High", "Critical", nullptr
  };

  return names[p];
}





std::string TroubleTicket::Status::toString(Type s)
{
  static char const * names[] = 
  {
    "New", "Open", "In Work", "Resolved", "Verified", "Closed", "Rejected", "Hold", nullptr
  };

  return names[s];
}

