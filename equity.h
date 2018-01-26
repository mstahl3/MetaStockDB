/*
 * Class: equity
 * Author: Marc Stahl
 * Description: Base class to expose a simple interface to an equity.  Used
 *   by EquityInDB class to add the full range of private methods and
 *   members.
 * History:
 *   MKS    2018-Jan-19   Original coding
 */

#ifndef EQUITY_H
#define EQUITY_H

#include <string>
#include "globaltypes.h"
#include "equity.h"
#include "date.h"


class Equity
{
public:


//----------------------------------------------------------------------------
// Equity information

    // Get the stock description
    virtual std::string description() const = 0;

    // Set the stock description;
    virtual void description(const std::string newDescription) = 0;





    //----------------------------------------------------------------------------
    // Trading day information


    //  Get which fields are active
    virtual bool fieldDateActive() const = 0;
    virtual bool fieldOpenActive() const = 0;
    virtual bool fieldHighActive() const = 0;
    virtual bool fieldLowActive()  const = 0;
    virtual bool fieldCloseActive() const = 0;
    virtual bool fieldVolumeActive() const = 0;
    virtual bool fieldOpenInterestActive() const = 0;
    virtual bool fieldTimeActive() const = 0;


// Get the date of the first trading day for this equity
    virtual Date firstTradingDayDate() const = 0;

// Get the date of the last trading day for this equity
    virtual Date lastTradingDayDate() const = 0;

    // Adds the passed trading day data to the list of trading days
    // Returns true if succesfully added new day data
    virtual bool addTradingDayData(const Date date, const Time time, const float open, const float close, const float high, const float low, const unsigned long volume, const float openInterest) = 0;


    // Reset the iterator at start of list, and copy first item in the list
    // into the parameter.  Return true if success, false otherwise
    virtual bool getFirstTradingDayData(Date &date, Time &time, float &open, float &close, float &high, float &low, unsigned long &volume, float &openInterest) = 0;


    // Advance the iterator, and if there is another element
    // then copy element into the parameter.
    // Return true if success, false otherwise
    virtual bool getNextTradingDayData(Date &date, Time &time, float &open, float &close, float &high, float &low, unsigned long &volume, float &openInterest) = 0;


protected:

    // Hide destructor to prevent deletion of the object
    virtual ~Equity() {}

};

#endif // EQUITY_H
