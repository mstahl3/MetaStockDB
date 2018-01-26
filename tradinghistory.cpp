/*
 * Class: TradingHistory
 * Author: Marc Stahl
 * Description: Stores the data for one stock over zero or more days.
 *     This class has a list of TradingDay objects.
 * History:
 *   MKS    2018-Jan-19   Original coding
 */

#include "tradinghistory.h"
#include <iostream>
#include <string>
#include "activefields.h"


using namespace std;

// Constructor: Create a trading history for the specified first and last
//     trading days.
// Does not load any data
TradingHistory::TradingHistory(
        const Date firstTradingDayInData,
        const Date lastTradingDayInData) :
    m_loaded(false),
    m_tradingDataItValid(false),
    m_firstTradingDayInData(firstTradingDayInData),
    m_lastTradingDayInData(lastTradingDayInData)
{
}


// Return number of trading days held in this object
unsigned long TradingHistory::days() const {
    return m_tradingData.size();
}


// Setter for trading history loaded
void TradingHistory::loaded(const bool isLoaded) {
    m_loaded = isLoaded;
}

// Create a single horizontal divider line to match the active fields
string TradingHistory::dividerLine(const ActiveFields activeFields) const {

    // Line for output
    std::string line = "+";

    // If date is active add date
    if (activeFields.dateActive()) line += "-------------+";

    // If time is active add time
    if (activeFields.timeActive()) line += "--------------+";

    // If open is active add time
    if (activeFields.openActive()) line += "---------+";

    // If high is active add time
    if (activeFields.highActive()) line += "---------+";

    // If low is active add time
    if (activeFields.lowActive()) line += "---------+";

    // If close is active add time
    if (activeFields.closeActive()) line += "---------+";

    // If volume is active add time
    if (activeFields.volumeActive()) line += "----------+";

    // If low is active add time
    if (activeFields.openInterestActive())  line += "---------+";

    return line;
}


// Create a single horizontal header line to match the active fields
std::string TradingHistory::headerLine(const ActiveFields activeFields) const {

    // Line for output
    std::string line = "+";

    // If date is active add date
    if (activeFields.dateActive()) line += " Date        |";

    // If time is active add time
    if (activeFields.timeActive()) line += " Time         |";

    // If open is active add time
    if (activeFields.openActive()) line += " Open    |";

    // If high is active add time
    if (activeFields.highActive()) line += " High    |";

    // If low is active add time
    if (activeFields.lowActive()) line += " Low     |";

    // If close is active add time
    if (activeFields.closeActive()) line += " Close   |";

    // If volume is active add time
    if (activeFields.volumeActive()) line += " Volume   |";

    // If low is active add time
    if (activeFields.openInterestActive())  line += " Open Int|";

    return line;
}


// Adds the passed trading day data to the list of trading days
// Ensure new data is added to list in chronological order
// Update m_firstTradingDayInData and m_lastTradingDayInData if the new
// data extends the date range of the list
// Returns true if succesfully added new day data
bool TradingHistory::addTradingDayData(TradingDay newDayData)
{
    // If list is empty then set everything
    if (m_tradingData.begin() == m_tradingData.end()) {
        m_firstTradingDayInData = newDayData.date();
        m_lastTradingDayInData = newDayData.date();
        m_tradingData.push_back(newDayData);
    }

    // Else there is some data in the list
    else {

        // Create an interator starting at the end
        std::list<TradingDay>::reverse_iterator tradingDayIterator;

        // Go through list until reach end (before first item),
        // or found a date which is <= newDataData date
        for(tradingDayIterator = m_tradingData.rbegin(); ( (tradingDayIterator != m_tradingData.rend()) and ((*tradingDayIterator).date() > newDayData.date()) ); tradingDayIterator++);

        // If hit the begining of the list (before first element), add
        // to front and update range
        if (tradingDayIterator == m_tradingData.rend()) {
            m_firstTradingDayInData = newDayData.date();
            m_tradingData.push_front(newDayData);
        }

        // Else insert into list middle or end
        else {

            // If this date alread exists then
            if ((*tradingDayIterator).date() == newDayData.date()) return false;


            // If still at the end of the list, add to back and update range
            if (tradingDayIterator == m_tradingData.rbegin()) {
                m_lastTradingDayInData = newDayData.date();
                m_tradingData.push_back(newDayData);
            }

            // else insert into the middle of the list
            else m_tradingData.insert(tradingDayIterator.base(), newDayData);
        }
    }
    return true;
}


// Reset at start of list, and copy first item in the list
// into the parameter.  Return true if success, false otherwise
bool TradingHistory::getFirstTradingDayData(TradingDay& tradingDayData)
{
    // If empty list, no first element
    if (m_tradingData.empty()) return false;

    // Get first element
    m_tradingDataIt = m_tradingData.begin();
    tradingDayData = *m_tradingDataIt;

    // Set flag that iterator is valid
    m_tradingDataItValid = true;
    return true;
}


// Advance the iterator, and if there is another element
// then copy element into the parameter.
// Return true if success, false otherwise
bool TradingHistory::getNextTradingDayData(TradingDay& tradingDayData)
{
    // If iterator is not valid, return failure
    if (!m_tradingDataItValid) return false;

    // Advance the iterator
    m_tradingDataIt++;

    // If reached end, invalidate the iterator and return false
    if (m_tradingDataIt == m_tradingData.end()) {
        m_tradingDataItValid = false;
        return false;
    }

    // Copy last element as my data
    tradingDayData = *m_tradingDataIt;
    return true;
}

// Getter for date of first day of trading
Date TradingHistory::firstDate() const
{
    return m_firstTradingDayInData;
}

// Getter for date of last day of trading
Date TradingHistory::lastDate() const
{
    return m_lastTradingDayInData;
}

// Print all the contents of this object (all info from all trading days)
void TradingHistory::print(const ActiveFields activeFields) {

    TradingDay currentTradingDay;

    // Print the header
    std::cout << "First Date...............................: " << m_firstTradingDayInData.asString(Date::EDateFormatYYYYMMMDD) << endl;
    std::cout << "Last Date................................: " << m_lastTradingDayInData.asString(Date::EDateFormatYYYYMMMDD) << endl;
    std::cout << "Trading data loaded......................: " << (m_loaded?"Yes":"No") << endl;
    std::cout << "Number of trading days...................: " << m_tradingData.size() << endl;
    std::cout << dividerLine(activeFields) << std::endl;
    std::cout << headerLine(activeFields) << std::endl;
    std::cout << dividerLine(activeFields) << std::endl;

    // Loop through all available trading days and print them
    for(bool dataAvailable = getFirstTradingDayData(currentTradingDay); dataAvailable; dataAvailable = getNextTradingDayData(currentTradingDay))
        currentTradingDay.print(activeFields);

    // Print the footer
    std::cout << dividerLine(activeFields) << std::endl;

}
