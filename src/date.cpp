/*
 * Class: date
 * Author: Marc Stahl
 * Description: Holds a year-month-day value, and offer arithmatic functions applied to this date
 * History:
 *   MKS    2018-Jan-19   Original coding
 */

#include "date.h"
#include <iostream>
#include <string>
#include <iomanip>
#include <sstream>


// Constructor: Assigns a starting day, month, and year to the object.
Date::Date(const unsigned short int year, const unsigned char month, const unsigned char day) :
    m_day(day),
    m_month(month),
    m_year(year)
{
}


// Constructor: Converts the epoch to a day, month and year and stores them in the member variables
Date::Date(const time_t epoch)
{
    tm* mytmPtr;

    //convert epoch to day month year
    mytmPtr = gmtime(&epoch);

    //initialize member variables to the new day month year
    m_year = mytmPtr->tm_year + 1900;
    m_month = mytmPtr->tm_mon + 1;
    m_day = mytmPtr->tm_mday;
}


// Constructor: Create an invalid date
Date::Date()
{
    m_day = 0;
    m_month = 0;
    m_year = 0;
}

// Format m_day, m_month, and m_year as a single string, and return the string.
string Date::asString(const EDateFormats format) const
{
    // If invalid date then cannot format as a string
    if ((m_day == 0) && (m_month == 0) && (m_year == 0)) return "ZERO DATE";

    string dateAsString;
    char temp[40];
    tm mytm;
    mytm.tm_sec = 0;
    mytm.tm_min = 0;
    mytm.tm_hour = 0;
    mytm.tm_wday = 0;
    mytm.tm_yday = 0;
    mytm.tm_year = m_year - 1900;
    mytm.tm_mon = m_month - 1;
    mytm.tm_mday = m_day;
    mytm.tm_isdst = 0;


    switch (format) {
            // YYYY-MMM-DD
        case EDateFormatYYYYMMMDD:
            strftime(temp, 80, "%Y %B %d", &mytm);
            dateAsString = string(temp);
            break;
            //YY-MM-DD
        case EDateFormatYYMMDD:
            strftime(temp, 80, "%y %b %d", &mytm);
            dateAsString = string(temp);
            break;
            //EEEEEE
        case EDateFormatEpoch:
            dateAsString = to_string(asEpoch());
    }

    return dateAsString;
}

// Returns this object's day
unsigned char Date::day() const
{
    return m_day;
}

// Returns this object's month
unsigned char Date::Month() const
{
    return m_month;
}

// Returns this object's year
unsigned short int Date::Year() const
{
    return m_year;
}


// Returns this object's date as an epoch
time_t Date::asEpoch() const
{
    tm mytm;

    //initialize irrelevant variables to 0 so they don't interfere with calculations
    mytm.tm_sec = 0;
    mytm.tm_min = 0;
    mytm.tm_hour = 0;
    mytm.tm_wday = 0;
    mytm.tm_yday = 0;

    //fill tm struct with values from member variables
    mytm.tm_year = m_year - 1900;
    mytm.tm_mon = m_month - 1;
    mytm.tm_mday = m_day;

    //make this tm struct consistent with UTC(which includes no daylight savings), because gmtime works in UTC.
    mytm.tm_isdst = 0;

    return mktime(&mytm);
}



// If this object's date is greater than the comparisonDate, then return true. Otherwise return false.
bool Date::operator>(const Date comparisonDate) const
{
    return (asEpoch() > comparisonDate.asEpoch());
}


// If this object's date is less than the comparisonDate, then return true. Otherwise return false.
bool Date::operator<(const Date comparisonDate) const
{
    return (asEpoch() < comparisonDate.asEpoch());
}

// create and return a new date object which has this object's date plus 'days' number of days.
Date Date::operator+(const unsigned char days) const
{
    return (Date(asEpoch() + days*60*60*24));
}

// create and return a new date object which has this object's date minus 'days' number of days.
Date Date::operator-(const unsigned char days) const
{
    return (Date(asEpoch() - days*60*60*24));
}

// subtract comparisonDate's day from this object's day, and return the result.
unsigned char Date::operator-(const Date comparisonDate) const
{
    return ((asEpoch() - comparisonDate.asEpoch())/(60*60*24));
}

// If this object and comparisonDate are the exact same date, then return true. Otherwise return false.
bool Date::operator==(const Date comparisonDate) const
{
    return (asEpoch() == comparisonDate.asEpoch());
}

// If this object and comparisonDate are different dates, then return true. Otherwise return false.
bool Date::operator!=(const Date comparisonDate) const
{
    return (asEpoch() != comparisonDate.asEpoch());
}

// True if this is a valid date (i.e. not 0/0/0)
bool Date::isValid() const
{
    return ((m_day == 0) && (m_month == 0) && (m_year == 0));
}
