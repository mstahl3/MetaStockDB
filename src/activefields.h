/*
 * Class: ActiveFields
 * Author: Marc Stahl
 * Description: stores the file format for an FDAT or MWD file.
 * History:
 *   MKS    2018-Jan-19   Original coding
 */

#ifndef ACTIVEFIELDS_H
#define ACTIVEFIELDS_H

#include <iostream>

class ActiveFields
{
public:
    // Construct the object, enabling specific fields based on the number of fields in the FDAT file
    ActiveFields(const unsigned int numFields);

    // Construct the object, enabling specific fields based on the bitmak specified
    ActiveFields(const unsigned char testBitMask);

    // Default constructor
    ActiveFields();

    //indicate on the screen the status of each field
    void print() const;

    //bitmask setter
    void bitMask(const unsigned char fieldBitMask);

    //bitmask getter
    unsigned char bitMask() const;

    // Return the current bitmask as an 8 digit binary of format 0b11111111
    std::string asBinaryAscii() const;

    // Return the current bitmask as a series of letters
    std::string  asLettersAscii() const;

    // Return true if the passed bitMask is a valid bitmask
    static bool validBitMask(const unsigned char bitMask);

    //  Getters
    bool dateActive() const;
    bool openActive() const;
    bool highActive() const;
    bool lowActive()  const;
    bool closeActive() const;
    bool volumeActive() const;
    bool openInterestActive() const;
    bool timeActive() const;


    // Return the offset from the start of the record to the given field, taking into account which preceeding fields are active
    unsigned char dateOffset() const;
    unsigned char timeOffset() const;
    unsigned char openOffset() const;
    unsigned char highOffset() const;
    unsigned char lowOffset() const;
    unsigned char closeOffset() const;
    unsigned char volumeOffset() const;
    unsigned char openInterestOffset() const;

    //get the number of active fields that this object indicates
    unsigned char numFields() const;

    //get the size of a record that this object indicates
    unsigned char recordSize() const;


private:
    // Flags whether field is active
    bool m_dateActive;
    bool m_openActive;
    bool m_highActive;
    bool m_lowActive;
    bool m_closeActive;
    bool m_volumeActive;
    bool m_openInterestActive;
    bool m_timeActive;

    //the number of active fields that this object indicates
    unsigned char m_numFields;

    // Mapping of bitmask bits to active fields
    enum EActiveFieldBits {
        EActiveFieldBit_date =         0x01,
        EActiveFieldBit_open =         0x02,
        EActiveFieldBit_high =         0x04,
        EActiveFieldBit_low =          0x08,
        EActiveFieldBit_close =        0x10,
        EActiveFieldBit_volume =       0x20,
        EActiveFieldBit_openInterest = 0x40,
        EActiveFieldBit_time =         0x80  // Unsure if this bit is really time
    };
};

#endif // ACTIVEFIELDS_H
