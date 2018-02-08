/*
 * Class: TradingDay
 * Author: Marc Stahl
 * Description: This class holds data for one stock on one day.
 * History:
 *   MKS    2018-Jan-19   Original coding
 */

#include <iostream>
#include <fstream>
#include <string>
#include "date.h"
#include "tradingday.h"
#include "activefields.h"

// Set the starting values for this TradingDay object
TradingDay::TradingDay(
        const Date date,
        const float time,
        const float open,
        const float close,
        const float high,
        const float low,
        const unsigned long volume,
        const float openInterest) :
    m_date(date),
    m_time(time),
    m_open(open),
    m_close(close),
    m_high(high),
    m_low(low),
    m_volume(volume),
    m_openInterest(openInterest)
{

}

// Default constructor
TradingDay::TradingDay() :
    m_date(Date()),
    m_time(0),
    m_open(0),
    m_close(0),
    m_high(0),
    m_low(0),
    m_volume(0),
    m_openInterest(0)
{

}

//---------------------------------------------------------------------------------
//Getters
Date TradingDay::date() const
{
    return m_date;
}


float TradingDay::time() const
{
    return m_time;
}


float TradingDay::open() const
{
    return m_open;
}


float TradingDay::close() const
{
    return m_close;
}


float TradingDay::high() const
{
    return m_high;
}


float TradingDay::low() const
{
    return m_low;
}


unsigned long TradingDay::volume() const
{
    return m_volume;
}


float TradingDay::openInterest() const
{
    return m_openInterest;
}


// Print a single line of active fields
void TradingDay::print(const ActiveFields activeFields) const {

    // C style string to hold a line fragment
    char buffer[255];

    // Line for output
    string line = "|";

    // If date is active add date
    if (activeFields.dateActive()) {
        sprintf(buffer," %10s ",m_date.asString(Date::EDateFormatYYYYMMMDD).c_str());
        line += std::string(buffer) + "|";
    }

    // If time is active add time
    if (activeFields.timeActive()) {
        sprintf(buffer," %5.4f ",m_time);
        line += std::string(buffer) + "|";
    }

    // If open is active add time
    if (activeFields.openActive()) {
        sprintf(buffer," %5.4f ",m_open);
        line += std::string(buffer) + "|";
    }

    // If high is active add time
    if (activeFields.highActive()) {
        sprintf(buffer," %5.4f ",m_high);
        line += std::string(buffer) + "|";
    }

    // If low is active add time
    if (activeFields.lowActive()) {
        sprintf(buffer," %5.4f ",m_low);
        line += std::string(buffer) + "|";
    }

    // If close is active add time
    if (activeFields.closeActive()) {
        sprintf(buffer," %5.4f ",m_close);
        line += std::string(buffer) + "|";
    }

    // If volume is active add time
    if (activeFields.volumeActive()) {
        sprintf(buffer," %8lu ",m_volume);
        line += std::string(buffer) + "|";
    }

    // If low is active add time
    if (activeFields.openInterestActive()) {
        sprintf(buffer," %5.4f ",m_low);
        line += std::string(buffer) + "|";
    }

    std::cout << line << std::endl;
}

