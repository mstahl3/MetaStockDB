/*
 * Class: date
 * Author: Marc Stahl
 * Description: This class represents a date
 * History:
 *   MKS    2018-Jan-19   Original coding
 */

#ifndef DATE_H
#define DATE_H

#include <ctime>
#include <string>
using namespace std;

class Date
{

public:

    // Formats for asString
    enum EDateFormats {
        EDateFormatEpoch,
        EDateFormatYYMMDD,
        EDateFormatYYYYMMMDD
    };

    // Constructor: Create object with passed date as 3 variables
    Date(const unsigned short int year, const unsigned char month, const unsigned char day);

    // Constructor: Create object with passed date as epoch
    Date(const time_t epoch);


    // Constructor: Create an invalid date
    Date();

    // Prints the contained date as a string, in the format specified by 'format'
    string asString(const EDateFormats format) const;

    // Returns this object's day
    unsigned char day() const;

    // Returns this object's month
    unsigned char Month() const;

    // Returns this object's year
    unsigned short int Year() const;

    // If this object's date is greater than the comparisonDate, then return true. Otherwise return false.
    bool operator>(const Date comparisonDate) const;

    // If this object's date is less than the comparisonDate, then return true. Otherwise return false.
    bool operator<(const Date comparisonDate) const;

    // create and return a new date object which has this object's date plus 'days' number of days.
    Date operator+(const unsigned char days) const;

    // create and return a new date object which has this object's date minus 'days' number of days.
    Date operator-(const unsigned char days) const;

    // subtract comparisonDate's day from this object's day, and return the result.
    unsigned char operator-(const Date comparisonDate) const;

    // If this object and comparisonDate are the exact same date, then return true. Otherwise return false.
    bool operator==(const Date comparisonDate) const;

    // If this object and comparisonDate are different dates, then return true. Otherwise return false.
    bool operator!=(const Date comparisonDate) const;

    // True if this is a valid date (i.e. not 0/0/0)
    bool isValid() const;

private:
    // day of the month.
    unsigned char m_day;

    // month of the year.
    unsigned char m_month;

    // year of the gregorian calendar.
    unsigned short int m_year;

    // Returns epoch
    time_t asEpoch() const;

};


#endif // DATE_H
