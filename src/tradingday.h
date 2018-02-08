/*
 * Class: TradingDay
 * Author: Marc Stahl
 * Description: This class holds data for one stock on one day.
 * History:
 *   MKS    2018-Jan-19   Original coding
 */

#ifndef TRADINGDAY_H
#define TRADINGDAY_H

#include "activefields.h"
#include "date.h"

class TradingDay
{

public:
    // Create a trading day with the starting values passed
    TradingDay(const Date date, const float time, const float open, const float close, const float high, const float low, const unsigned long volume, const float openInterest);

    // Construct: Empty constructor, default values
    TradingDay();

    // Returns date of this trading day
    Date date() const;

    // Returns time of the trade on this trading day
    float time() const;

    // Returns open value of this trading day
    float open() const;

    //returns close value of this trading day
    float close() const;

    //returns high value of this trading day
    float high() const;

    //returns low value of this trading day
    float low() const;

    //returns volume value of this trading day
    unsigned long volume() const;

    // Return the open interest for this trading day
    float openInterest() const;

    // Print a single line of active fields
    void print(const ActiveFields activeFields) const;

private:

    //the date of the trading day
    Date m_date;

    // The time of the trade (guess at format)
    float m_time;

    //price of the stock at the start of the trading day
    float m_open;

    //price of the stock at the end of the trading day
    float m_close;

    //price of the stock at it's highest point during the trading day
    float m_high;

    //price of the stock at it's lowest point during the trading day
    float m_low;

    //number of stocks traded that day
    unsigned long int m_volume;

    // Open interest for this trading day
    float m_openInterest;

};


#endif // TRADINGDAY_H
