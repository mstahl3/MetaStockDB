/*
 * Class: equityInDB
 * Author: Marc Stahl
 * Description: Implementation of virtual methods from Equity base class.
 * History:
 *   MKS    2017-OCT-27   Original coding
 */

#include <string.h>
#include "equityindb.h"


//----------------------------------------------------------------------------
// Equity information

// Get the stock description
std::string EquityInDB::description() const {
    return m_description;
}

// Set the stock description;
void EquityInDB::description(const std::string newDescription) {
    m_description = newDescription;
}

//  Get which fields are active
bool EquityInDB::fieldDateActive() const {
    return m_activeFields.dateActive();
}

bool EquityInDB::fieldOpenActive() const {
    return m_activeFields.openActive();
}

bool EquityInDB::fieldHighActive() const {
    return m_activeFields.highActive();
}

bool EquityInDB::fieldLowActive() const {
    return m_activeFields.lowActive();
}

bool EquityInDB::fieldCloseActive() const {
    return m_activeFields.closeActive();
}

bool EquityInDB::fieldVolumeActive() const {
    return m_activeFields.volumeActive();
}

bool EquityInDB::fieldOpenInterestActive() const {
    return m_activeFields.openInterestActive();
}

bool EquityInDB::fieldTimeActive() const {
    return m_activeFields.timeActive();
}


//----------------------------------------------------------------------------
// Trading day information

// Get the date of the first trading day for this equity
Date EquityInDB::firstTradingDayDate() const  {
    return m_tradingHistory.firstDate();
}

// Get the date of the last trading day for this equity
Date EquityInDB::lastTradingDayDate() const  {
    return m_tradingHistory.lastDate();
}

// Adds the passed trading day data to the list of trading days
// Returns true if passed trading data succesfully added
bool EquityInDB::addTradingDayData(const Date date, const Time time, const float open, const float close, const float high, const float low, const unsigned long volume, const float openInterest) {
    return m_tradingHistory.addTradingDayData(TradingDay(date, time, open, close, high, low, volume, openInterest));
}

// Reset at start of list, and copy first item in the list
// into the parameter.  Return true if at least one trading is in the list,
// and the data from that trading day is returned
bool EquityInDB::getFirstTradingDayData(Date &date, Time &time, float &open, float &close, float &high, float &low, unsigned long &volume, float &openInterest) {
    TradingDay tradingDay;
    //If a trading day is available, then copy it's data into the parameters of this function
    if (m_tradingHistory.getFirstTradingDayData(tradingDay))
    {
        date = tradingDay.date();
        time = tradingDay.time();
        open = tradingDay.open();
        close = tradingDay.close();
        high = tradingDay.high();
        low = tradingDay.low();
        volume = tradingDay.volume();
        openInterest = tradingDay.openInterest();
        return true;
    }
    //If a trading day is unavailable, then set all the parameters to default objects or zero
    else
    {
        date = Date();
        time = 0;
        open = 0;
        close = 0;
        high = 0;
        low = 0;
        volume = 0;
        openInterest = 0;
        return false;
    }
}


// Advance the iterator, and if there is another element
// then copy element into the parameters.  Return true if iterator was advanced
// to the next trading day, and the data from that trading day is returned
bool EquityInDB::getNextTradingDayData(Date &date, Time &time, float &open, float &close, float &high, float &low, unsigned long &volume, float &openInterest)  {
    TradingDay tradingDay;
    //If a trading day is available, then copy it's data into the parameters of this function
    if (m_tradingHistory.getNextTradingDayData(tradingDay))
    {
        date = tradingDay.date();
        time = tradingDay.time();
        open = tradingDay.open();
        close = tradingDay.close();
        high = tradingDay.high();
        low = tradingDay.low();
        volume = tradingDay.volume();
        openInterest = tradingDay.openInterest();
        return true;
    }
    //If a trading day is unavailable, then set all the parameters to default objects or zero
    else
    {
        date = Date();
        time = 0;
        open = 0;
        close = 0;
        high = 0;
        low = 0;
        volume = 0;
        openInterest = 0;
        return false;
    }
}

