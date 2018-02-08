/*
 * Class: ActiveFields
 * Author: Marc Stahl
 * Description: Track which fields are active in an MWD/DAT file
 * History:
 *   MKS    2018-Jan-19   Original coding
 */

#include "activefields.h"
#include <string>




// Construct the object, enabled specific fields based on the number of fields specified
// the fields date, open, high, low, and close are always true, so they are set to true in the initializer.
//the other fields are set based on the parameter, so they are initialized to false
ActiveFields::ActiveFields(const unsigned int numFields):
    m_dateActive(true),  // Default values
    m_openActive(true),
    m_highActive(true),
    m_lowActive(true),
    m_closeActive(true),
    m_volumeActive(false),
    m_openInterestActive(false),
    m_timeActive(false),
    m_numFields(numFields)
{
    // Based on the number of fields specified, enable the proper fields
    switch (m_numFields) {
    default:
    case 8:
        m_timeActive = true;
    case 7:
        m_openInterestActive = true;
    case 6:
        m_volumeActive = true;
    case 5:
        break;
    }
}


// Construct the object, enable specific fields based on the bitmask specified
ActiveFields::ActiveFields(const unsigned char testBitMask) {
    bitMask(testBitMask);
}

// Default constructor
ActiveFields::ActiveFields() {
}


// Print the status of each field
void ActiveFields::print() const
{
    std::cout << "Active fields: date: " << m_dateActive << " open: " << m_openActive << " high: " << m_highActive
              << " low: " << m_lowActive << " close: " << m_closeActive << " volume: " << m_volumeActive
              << " open Interest: " << m_openInterestActive << "\n";
}


// Return true if the passed bitMask is a valid bitmask.
// There are only 4 valid formats for a record in a Trading Day File(TDF), and therefore there are only 4 valid bitmasks.
bool ActiveFields::validBitMask(const unsigned char bitMask) {
    return (bitMask == (EActiveFieldBit_date & EActiveFieldBit_open & EActiveFieldBit_high & EActiveFieldBit_low & EActiveFieldBit_close)) ||
           (bitMask == (EActiveFieldBit_date & EActiveFieldBit_open & EActiveFieldBit_high & EActiveFieldBit_low & EActiveFieldBit_close & EActiveFieldBit_volume)) ||
           (bitMask == (EActiveFieldBit_date & EActiveFieldBit_open & EActiveFieldBit_high & EActiveFieldBit_low & EActiveFieldBit_close & EActiveFieldBit_volume & EActiveFieldBit_openInterest)) ||
           (bitMask == (EActiveFieldBit_date & EActiveFieldBit_open & EActiveFieldBit_high & EActiveFieldBit_low & EActiveFieldBit_close & EActiveFieldBit_volume & EActiveFieldBit_openInterest & EActiveFieldBit_time));
}



// bitmask setter. Pass in a bitmask and the active fields data is extracted and stored in boolean variables.
void ActiveFields::bitMask(const unsigned char fieldBitMask)
{
    //extract each bit and store in boolean member variable
    m_dateActive = fieldBitMask & EActiveFieldBit_date;
    m_openActive = fieldBitMask & EActiveFieldBit_open;
    m_highActive = fieldBitMask & EActiveFieldBit_high;
    m_lowActive = fieldBitMask & EActiveFieldBit_low;
    m_closeActive = fieldBitMask & EActiveFieldBit_close;
    m_volumeActive = fieldBitMask & EActiveFieldBit_volume;
    m_openInterestActive = fieldBitMask & EActiveFieldBit_openInterest;
    m_timeActive = fieldBitMask & EActiveFieldBit_time;
}


// bitmask getter. Converts the active fields data from booleans to a bitmask, and returns the bitmask.
unsigned char ActiveFields::bitMask() const
{
    unsigned char fieldBitMask = 0;

    //set each bit in bitmask
    fieldBitMask += m_dateActive * EActiveFieldBit_date;
    fieldBitMask += m_openActive * EActiveFieldBit_open;
    fieldBitMask += m_highActive * EActiveFieldBit_high;
    fieldBitMask += m_lowActive * EActiveFieldBit_low;
    fieldBitMask += m_closeActive * EActiveFieldBit_close;
    fieldBitMask += m_volumeActive * EActiveFieldBit_volume;
    fieldBitMask += m_openInterestActive * EActiveFieldBit_openInterest;
    fieldBitMask += m_timeActive * EActiveFieldBit_time;

    return fieldBitMask;
}


// Return the current bitmask as an 8 digit binary of format 0b11111111
std::string ActiveFields::asBinaryAscii() const {

    std::string ascii = "0b";
    if (m_timeActive * EActiveFieldBit_time) ascii += "1";
    else ascii += "0";
    if (m_openInterestActive * EActiveFieldBit_openInterest) ascii += "1";
    else ascii += "0";
    if (m_volumeActive * EActiveFieldBit_volume) ascii += "1";
    else ascii += "0";
    if (m_closeActive * EActiveFieldBit_close) ascii += "1";
    else ascii += "0";
    if (m_lowActive * EActiveFieldBit_low) ascii += "1";
    else ascii += "0";
    if (m_highActive * EActiveFieldBit_high) ascii += "1";
    else ascii += "0";
    if (m_openActive * EActiveFieldBit_open) ascii += "1";
    else ascii += "0";
    if (m_dateActive * EActiveFieldBit_date) ascii += "1";
    else ascii += "0";

    return ascii;
}


// Return the current bitmask as a comma seperated list of letters
std::string ActiveFields::asLettersAscii() const {

    std::string ascii;
    if (m_timeActive * EActiveFieldBit_time) ascii += "T";
    if (m_openInterestActive * EActiveFieldBit_openInterest) ascii += "Oi";
    if (m_volumeActive * EActiveFieldBit_volume) ascii += "V";
    if (m_closeActive * EActiveFieldBit_close) ascii += "C";
    if (m_lowActive * EActiveFieldBit_low) ascii += "L";
    if (m_highActive * EActiveFieldBit_high) ascii += "H";
    if (m_openActive * EActiveFieldBit_open) ascii += "O";
    if (m_dateActive * EActiveFieldBit_date) ascii += "D";

    return ascii;
}


//  Getters
bool ActiveFields::dateActive() const
{
    return m_dateActive;
}

bool ActiveFields::timeActive() const
{
    return m_timeActive;
}


bool ActiveFields::openActive() const
{
    return m_openActive;
}

bool ActiveFields::highActive() const
{
    return m_highActive;
}

bool ActiveFields::lowActive() const
{
    return m_lowActive;
}

bool ActiveFields::closeActive() const
{
    return m_closeActive;
}

bool ActiveFields::volumeActive() const
{
    return m_volumeActive;
}

bool ActiveFields::openInterestActive() const
{
    return m_openInterestActive;
}


// Return the offset from the start of the record to the given field, taking into account which preceeding fields are active
unsigned char ActiveFields::dateOffset() const
{
return 0;
}

unsigned char ActiveFields::timeOffset() const
{
    return 4 * (m_dateActive?1:0);
}

unsigned char ActiveFields::openOffset() const
{
    return 4 * ((m_dateActive?1:0) + (m_timeActive?1:0));
}

unsigned char ActiveFields::highOffset() const
{
    return 4 * ((m_dateActive?1:0) + (m_timeActive?1:0) + (m_openActive?1:0));
}

unsigned char ActiveFields::lowOffset() const
{
    return 4 * ((m_dateActive?1:0) + (m_timeActive?1:0) + (m_openActive?1:0) + (m_highActive?1:0));
}

unsigned char ActiveFields::closeOffset() const
{
    return 4 * ((m_dateActive?1:0) + (m_timeActive?1:0) + (m_openActive?1:0) + (m_highActive?1:0) + (m_lowActive?1:0));
}

unsigned char ActiveFields::volumeOffset() const
{
    return 4 * ((m_dateActive?1:0) + (m_timeActive?1:0) + (m_openActive?1:0) + (m_highActive?1:0) + (m_lowActive?1:0) + (m_closeActive?1:0));
}

unsigned char ActiveFields::openInterestOffset() const
{
    return 4 * ((m_dateActive?1:0) + (m_timeActive?1:0) + (m_openActive?1:0) + (m_highActive?1:0) + (m_lowActive?1:0) + (m_closeActive?1:0) + (m_volumeActive?1:0));
}




// Get the number of active fields that this object indicates
unsigned char ActiveFields::numFields() const
{
    return m_numFields;
}



// Get the size of a record that this object indicates
unsigned char ActiveFields::recordSize() const
{
    return m_numFields * 4;
}





