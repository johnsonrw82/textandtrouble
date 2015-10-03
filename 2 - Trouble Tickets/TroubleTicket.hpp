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


#ifndef TROUBLE_TICKET_HPP
#define TROUBLE_TICKET_HPP

#include <iostream>
#include <string>

class TroubleTicket
{
  public:
    // Inner type definitions
    struct Priority  // Scoped enumeration with functions
    {
      enum Type{ NONE, LOW, NORMAL, HIGH, CRITICAL };
      static std::string toString(Type priority);
      
      Priority()                              = delete;
      Priority(const Priority &)              = delete;
      Priority(const Priority &&)             = delete;
      Priority & operator=(const Priority &)  = delete;
      Priority & operator=(const Priority &&) = delete;
    };

    struct Status // Scoped enumeration with functions
    {
      enum Type{ NEW, OPEN, IN_WORK, RESOLVED, VERIFIED, CLOSED, REJECTED, HOLD };
      static std::string toString(Type status);

      Status()                            = delete;
      Status(const Status &)              = delete;
      Status(const Status &&)             = delete;
      Status & operator=(const Status &)  = delete;
      Status & operator=(const Status &&) = delete;
    };







    // Constructors
    TroubleTicket
    (
      std::string     const     & description   = "**No description provided**",
      Priority::Type  const       priority      = Priority::NORMAL, 
      Status::Type    const       status        = Status::NEW
    )
      : _description  ( description ), 
        _priority     ( priority ), 
        _status       ( status ), 
        _ticketNumber ( ++ticketIDs )
    {}




    // Logical operators
    bool operator<   (TroubleTicket const & rhs ) const {return _priority <  rhs._priority;}
    bool operator==  (TroubleTicket const & rhs ) const {return _priority == rhs._priority;}
    
    bool operator<=  (TroubleTicket const & rhs ) const {return *this < rhs || *this == rhs;}
    bool operator!=  (TroubleTicket const & rhs ) const {return !(*this == rhs);}
    bool operator>=  (TroubleTicket const & rhs ) const {return !(*this < rhs);}
    bool operator>   (TroubleTicket const & rhs ) const {return !(*this <= rhs);}


    // Queries/getters
    std::string     description() const {return _description;}
    std::string     resolution()  const {return _resolution;}
    Priority::Type  priority()    const {return _priority;}
    Status::Type    status()      const {return _status;}
    unsigned long   ticketNumber()const {return _ticketNumber;}


    // Setters
    void description  (std::string    const & s) {_description = s;}
    void resolution   (std::string    const & s) {_resolution  = s;}
    void priority     (Priority::Type const & p) {_priority    = p;}
    void status       (Status::Type   const & s) {_status      = s;}
    void ticketNumber (unsigned long  const & n) {_ticketNumber= n;}


    // misc
    std::string  toString() const;

  private:
    // Instance attributes
    std::string     _description; // description of the problem
    std::string     _resolution;  // description of the resolution to the problem
    Priority::Type  _priority;
    Status::Type    _status;
    unsigned long   _ticketNumber;

    // Class attributes
    static unsigned long ticketIDs;
};




inline
std::ostream & operator << (std::ostream & stream, TroubleTicket const & theTicket)
{
  stream << theTicket.toString();
  return stream;
}


inline
std::ostream & operator << (std::ostream & stream, TroubleTicket::Priority::Type const & priority)
{
  stream << TroubleTicket::Priority::toString(priority);
  return stream;
}


inline
std::ostream & operator << (std::ostream & stream, TroubleTicket::Status::Type const & status)
{
  stream << TroubleTicket::Status::toString(status);
  return stream;
}

#endif
