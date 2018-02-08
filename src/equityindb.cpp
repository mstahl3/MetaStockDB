/*
 * Class: EquityInDB
 * Author: Marc Stahl
 * Description: This class holds meta data about an equity, and also contains the object which holds the financial data. EquityInDB is a subsclass of equity.
 * History:
 *   MKS    2018-Jan-19   Original coding
 */

#include <stdio.h>
#include <string.h>
#include <iostream>

#include "equityindb.h"
#include "bytearray.h"

// MASTER filler field lengths
#define MASTER_FILLER1_LENGTH                            48
#define MASTER_FILLER2_LENGTH                            2
#define MASTER_FILLER3_LENGTH                            1
#define MASTER_FILLER4_LENGTH                            1
#define MASTER_FILLER5_LENGTH                            1

// EMASTER filler field lengths
#define EMASTER_FILLER1_LENGTH                           187
#define EMASTER_FILLER2_LENGTH                           3
#define EMASTER_FILLER3_LENGTH                           1
#define EMASTER_FILLER4_LENGTH                           1
#define EMASTER_FILLER5_LENGTH                           8
#define EMASTER_FILLER6_LENGTH                           16
#define EMASTER_FILLER7_LENGTH                           4
#define EMASTER_FILLER8_LENGTH                           4
#define EMASTER_FILLER9_LENGTH                           38
#define EMASTER_FILLER10_LENGTH                          1
#define EMASTER_FILLER11_LENGTH                          53

// XMASTER filler field lengths
#define XMASTER_FILLER1_LENGTH                           6
#define XMASTER_FILLER2_LENGTH                           2
#define XMASTER_FILLER3_LENGTH                           2
#define XMASTER_FILLER4_LENGTH                           129
#define XMASTER_FILLER5_LENGTH                           1
#define XMASTER_FILLER6_LENGTH                           1
#define XMASTER_FILLER7_LENGTH                           5
#define XMASTER_FILLER8_LENGTH                           13
#define XMASTER_FILLER9_LENGTH                           1
#define XMASTER_FILLER10_LENGTH                          2
#define XMASTER_FILLER11_LENGTH                          7
#define XMASTER_FILLER12_LENGTH                          16
#define XMASTER_FILLER13_LENGTH                          3
#define XMASTER_FILLER14_LENGTH                          29





// Constructor: This constructor receives as parameters all of the data that is read from MASTER, and then copies the parameter data into it's member variables.
EquityInDB::EquityInDB(
        const unsigned long int FDATFileNum,
        const unsigned long int fileType,
        const unsigned char fieldLength,
        const unsigned int numFields,
        const ByteArray MASTERFiller2,
        const string description,
        const ByteArray MASTERFiller3,
        const unsigned char CT_V2_8_FLAG,
        const Date firstDate,
        const Date lastDate,
        const EquityInDB::EInterdayPeriodicity interdayPeriodicity,
        const EquityInDB::EIntradayPeriodicity intradayPeriodicity,
        const string symbol,
        const ByteArray MASTERFiller4,
        const unsigned char flag,
        const ByteArray MASTERFiller5) :
    m_dataFileNumber(FDATFileNum),
    m_dataFileType(EquityInDB::EDataFileTypeFDAT),  // Since in MASTER, must be using FDAT file type
    m_fileType(fileType),
    m_recordLength(fieldLength),
    m_activeFields(numFields),
    m_description(description),
    m_CT_V2_8_FLAG(CT_V2_8_FLAG),
    m_interdayPeriodicity(interdayPeriodicity),
    m_intradayPeriodicity(intradayPeriodicity),
    m_symbol(symbol),
    m_flag(flag),

    m_IDCode(0),
    m_autoRun(0),
    m_intradayStartTime(0),
    m_intradayEndTime(0),
    m_lastDivPaid(0),
    m_lastDivAdjRate(0),

    //MASTER file fillers
    m_MASTERFiller2(MASTERFiller2),
    m_MASTERFiller3(MASTERFiller3),
    m_MASTERFiller4(MASTERFiller4),
    m_MASTERFiller5(MASTERFiller5),

    //EMASTER file fillers
    m_EMASTERFiller2(ByteArray(EMASTER_FILLER2_LENGTH)),
    m_EMASTERFiller3(ByteArray(EMASTER_FILLER3_LENGTH)),
    m_EMASTERFiller4(ByteArray(EMASTER_FILLER4_LENGTH)),
    m_EMASTERFiller5(ByteArray(EMASTER_FILLER5_LENGTH)),
    m_EMASTERFiller6(ByteArray(EMASTER_FILLER6_LENGTH)),
    m_EMASTERFiller7(ByteArray(EMASTER_FILLER7_LENGTH)),
    m_EMASTERFiller8(ByteArray(EMASTER_FILLER8_LENGTH)),
    m_EMASTERFiller9(ByteArray(EMASTER_FILLER9_LENGTH)),
    m_EMASTERFiller10(ByteArray(EMASTER_FILLER10_LENGTH)),
    m_EMASTERFiller11(ByteArray(EMASTER_FILLER11_LENGTH)),

    //XMASTER file fillers
    m_XMASTERFiller5(ByteArray(XMASTER_FILLER5_LENGTH)),
    m_XMASTERFiller6(ByteArray(XMASTER_FILLER6_LENGTH)),
    m_XMASTERFiller7(ByteArray(XMASTER_FILLER7_LENGTH)),
    m_XMASTERFiller8(ByteArray(XMASTER_FILLER8_LENGTH)),
    m_XMASTERFiller9(ByteArray(XMASTER_FILLER9_LENGTH)),
    m_XMASTERFiller10(ByteArray(XMASTER_FILLER10_LENGTH)),
    m_XMASTERFiller11(ByteArray(XMASTER_FILLER11_LENGTH)),
    m_XMASTERFiller12(ByteArray(XMASTER_FILLER12_LENGTH)),
    m_XMASTERFiller13(ByteArray(XMASTER_FILLER13_LENGTH)),
    m_XMASTERFiller14(ByteArray(XMASTER_FILLER14_LENGTH)),
    m_tradingHistory(TradingHistory(firstDate,lastDate))
{
    //    cout << "In contructor equityindb" << endl;
}

// Constructor: This constructor receives as parameters all of the data that is read from XMASTER, and then copies the parameter data into it's member variables.
EquityInDB::EquityInDB(
        const ByteArray XMASTERFiller5,
        const string symbol,
        const string description,
        const ByteArray XMASTERFiller6,
        const ByteArray XMASTERFiller7,
        const ByteArray XMASTERFiller8,
        const EInterdayPeriodicity interdayPeriodicity,
        const ByteArray XMASTERFiller9,
        const unsigned long int FDATFileNum,
        const ByteArray XMASTERFiller10,
        const unsigned char activeFieldsBitmask,
        const ByteArray XMASTERFiller11,
        const Date firstDate,
        const ByteArray XMASTERFiller12,
        const Date lastDate,
        const ByteArray XMASTERFiller13,
        const ByteArray XMASTERFiller14) :
    m_dataFileNumber(FDATFileNum),
    m_dataFileType(EquityInDB::EDataFileTypeMWD), // Since in XMASTER, must be using MWD file type
    m_fileType(0),  // Field is unused in XMASTER, so set to 0 just for initialization
    m_recordLength(4),  // All fields in MWD are 4 bytes long
    m_activeFields(activeFieldsBitmask),
    m_description(description),
    m_CT_V2_8_FLAG(0),  // Field is unused in XMASTER, so set to 0 just for initialization
    m_interdayPeriodicity(interdayPeriodicity),
    m_intradayPeriodicity(EIntradayPeriodicityNone),
    m_symbol(symbol),
    m_flag(0),  // Field is unused in XMASTER, so set to 0 just for initialization

    m_IDCode(0),  // Field is unused in XMASTER, so set to 0 just for initialization
    m_autoRun(0),  // Field is unused in XMASTER, so set to 0 just for initialization
    m_intradayStartTime(0),  // Field is unused in XMASTER, so set to 0 just for initialization
    m_intradayEndTime(0),  // Field is unused in XMASTER, so set to 0 just for initialization
    m_lastDivPaid(0),  // Field is unused in XMASTER, so set to 0 just for initialization
    m_lastDivAdjRate(0),  // Field is unused in XMASTER, so set to 0 just for initialization

    //MASTER file fillers
    m_MASTERFiller2(ByteArray(MASTER_FILLER2_LENGTH)),
    m_MASTERFiller3(ByteArray(MASTER_FILLER3_LENGTH)),
    m_MASTERFiller4(ByteArray(MASTER_FILLER4_LENGTH)),
    m_MASTERFiller5(ByteArray(MASTER_FILLER5_LENGTH)),

    //EMASTER file fillers
    m_EMASTERFiller2(ByteArray(EMASTER_FILLER2_LENGTH)),
    m_EMASTERFiller3(ByteArray(EMASTER_FILLER3_LENGTH)),
    m_EMASTERFiller4(ByteArray(EMASTER_FILLER4_LENGTH)),
    m_EMASTERFiller5(ByteArray(EMASTER_FILLER5_LENGTH)),
    m_EMASTERFiller6(ByteArray(EMASTER_FILLER6_LENGTH)),
    m_EMASTERFiller7(ByteArray(EMASTER_FILLER7_LENGTH)),
    m_EMASTERFiller8(ByteArray(EMASTER_FILLER8_LENGTH)),
    m_EMASTERFiller9(ByteArray(EMASTER_FILLER9_LENGTH)),
    m_EMASTERFiller10(ByteArray(EMASTER_FILLER10_LENGTH)),
    m_EMASTERFiller11(ByteArray(EMASTER_FILLER11_LENGTH)),

    //XMASTER file fillers
    m_XMASTERFiller5(XMASTERFiller5),
    m_XMASTERFiller6(XMASTERFiller6),
    m_XMASTERFiller7(XMASTERFiller7),
    m_XMASTERFiller8(XMASTERFiller8),
    m_XMASTERFiller9(XMASTERFiller9),
    m_XMASTERFiller10(XMASTERFiller10),
    m_XMASTERFiller11(XMASTERFiller11),
    m_XMASTERFiller12(XMASTERFiller12),
    m_XMASTERFiller13(XMASTERFiller13),
    m_XMASTERFiller14(XMASTERFiller14),
    m_tradingHistory(firstDate,lastDate)
{
}

// This method receives as parameters all of the data that is read from EMASTER, and then copies the parameter data into it's member variables.
void EquityInDB::setEMASTERData(const unsigned char IDCode,
                            const ByteArray EMASTERFiller2,
                            const ByteArray EMASTERFiller3,
                            const unsigned char autoRun,
                            const ByteArray EMASTERFiller4,
                            const ByteArray EMASTERFiller5,
                            const ByteArray EMASTERFiller6,
                            const ByteArray EMASTERFiller7,
                            const ByteArray EMASTERFiller8,
                            const float intradayStartTime,
                            const ByteArray EMASTERFiller9,
                            const float intradayEndTime,
                            const ByteArray EMASTERFiller10,
                            const unsigned long int lastDivPaid,
                            const float lastDivAdjRate,
                            const ByteArray EMASTERFiller11)
{
    m_IDCode = IDCode;
    m_autoRun = autoRun;
    m_intradayStartTime = intradayStartTime;
    m_intradayEndTime = intradayEndTime;
    m_lastDivPaid = lastDivPaid;
    m_lastDivAdjRate = lastDivAdjRate;

    //EMASTER file fillers
    m_EMASTERFiller2 = EMASTERFiller2;
    m_EMASTERFiller3 = EMASTERFiller3;
    m_EMASTERFiller4 = EMASTERFiller4;
    m_EMASTERFiller5 = EMASTERFiller5;
    m_EMASTERFiller6 = EMASTERFiller6;
    m_EMASTERFiller7 = EMASTERFiller7;
    m_EMASTERFiller8 = EMASTERFiller8;
    m_EMASTERFiller9 = EMASTERFiller9;
    m_EMASTERFiller10 = EMASTERFiller10;
    m_EMASTERFiller11 = EMASTERFiller11;
}


// Destructor
EquityInDB::~EquityInDB() {
}


// Convert the data file enum to an extension string
std::string EquityInDB::dataFileType2String(const EquityInDB::EDataFileTypes dataFileType) {
    std::string extension;
    if (dataFileType == EquityInDB::EDataFileTypeFDAT) extension="DAT";
    else if (dataFileType == EquityInDB::EDataFileTypeMWD) extension="MWD";
    else extension="???";
    return extension;
}


// Convert the interday periodicity to a string
std::string EquityInDB::InterdayPeriodicity2String(const EquityInDB::EInterdayPeriodicity interdayPeriodicity) {
    std::string periodicity;
    switch (interdayPeriodicity) {
    case EquityInDB::EInterdayPeriodicityDaily: periodicity = "Daily"; break;
    case EquityInDB::EInterdayPeriodicityMonthly: periodicity = "Monthly"; break;
    case EquityInDB::EInterdayPeriodicityNone: periodicity = "None"; break;
    case EquityInDB::EInterdayPeriodicityQuarterly: periodicity = "Quarterly"; break;
    case EquityInDB::EInterdayPeriodicityWeekly: periodicity = "Weekly"; break;
    case EquityInDB::EInterdayPeriodicityYearly: periodicity = "Yearly"; break;
    }
    return periodicity;
}


// Convert the interday periodicity to a string
std::string EquityInDB::IntradayPeriodicity2String(const EquityInDB::EIntradayPeriodicity intradayPeriodicity) {
    std::string periodicity;
    switch (intradayPeriodicity) {
    case EquityInDB::EIntradayPeriodicity1hr: periodicity = "1 Hour"; break;
    case EquityInDB::EIntradayPeriodicity1min: periodicity = "1 Minute"; break;
    case EquityInDB::EIntradayPeriodicity5min: periodicity = "5 Minutes"; break;
    case EquityInDB::EIntradayPeriodicity10min: periodicity = "10 Minutes"; break;
    case EquityInDB::EIntradayPeriodicityNone: periodicity = "None"; break;
    }
    return periodicity;
}

//----------------------------------------------------------------------------
// Getters

unsigned long EquityInDB::TDFFileNum() const
{
    return m_dataFileNumber;
}

EquityInDB::EDataFileTypes EquityInDB::dataFileType() const
{
    return m_dataFileType;
}

unsigned long int EquityInDB::fileType() const
{
    return m_fileType;
}

unsigned char EquityInDB::fieldLength() const
{
    return m_recordLength;
}


ActiveFields EquityInDB::activeFields() const
{
    return m_activeFields;
}

unsigned char EquityInDB::CT_V2_8_FLAG() const
{
    return m_CT_V2_8_FLAG;
}


EquityInDB::EInterdayPeriodicity EquityInDB::interdayPeriodicity() const
{
    return m_interdayPeriodicity;
}

EquityInDB::EIntradayPeriodicity EquityInDB::intradayPeriodicity() const
{
    return m_intradayPeriodicity;
}

unsigned char EquityInDB::flag() const
{
    return m_flag;
}

TradingHistory* EquityInDB::tradingHistory()
{
    return &m_tradingHistory;
}

bool EquityInDB::loaded() const
{
    return m_loaded;
}

unsigned char EquityInDB::IDCode() const
{
    return m_IDCode;
}

unsigned char EquityInDB::autoRun() const
{
    return m_autoRun;
}

float EquityInDB::intradayStartTime() const
{
    return m_intradayStartTime;
}

float EquityInDB::intradayEndTime() const
{
    return m_intradayEndTime;
}

unsigned long int EquityInDB::lastDivPaid() const
{
    return m_lastDivPaid;
}

float EquityInDB::lastDivAdjRate() const
{
    return m_lastDivAdjRate;
}

ByteArray EquityInDB::MASTERFiller2() const
{
    return m_MASTERFiller2;
}

ByteArray EquityInDB::MASTERFiller3() const
{
    return m_MASTERFiller3;
}

ByteArray EquityInDB::MASTERFiller4() const
{
    return m_MASTERFiller4;
}

ByteArray EquityInDB::MASTERFiller5() const
{
    return m_MASTERFiller5;
}

ByteArray EquityInDB::EMASTERFiller2() const
{
    return m_EMASTERFiller2;
}

ByteArray EquityInDB::EMASTERFiller3() const
{
    return m_EMASTERFiller3;
}

ByteArray EquityInDB::EMASTERFiller4() const
{
    return m_EMASTERFiller4;
}

ByteArray EquityInDB::EMASTERFiller5() const
{
    return m_EMASTERFiller5;
}

ByteArray EquityInDB::EMASTERFiller6() const
{
    return m_EMASTERFiller6;
}

ByteArray EquityInDB::EMASTERFiller7() const
{
    return m_EMASTERFiller7;
}

ByteArray EquityInDB::EMASTERFiller8() const
{
    return m_EMASTERFiller8;
}

ByteArray EquityInDB::EMASTERFiller9() const
{
    return m_EMASTERFiller9;
}

ByteArray EquityInDB::EMASTERFiller10() const
{
    return m_EMASTERFiller10;
}

ByteArray EquityInDB::EMASTERFiller11() const
{
    return m_EMASTERFiller11;
}

ByteArray EquityInDB::XMASTERFiller5() const
{
    return m_XMASTERFiller5;
}

ByteArray EquityInDB::XMASTERFiller6() const
{
    return m_XMASTERFiller6;
}

ByteArray EquityInDB::XMASTERFiller7() const
{
    return m_XMASTERFiller7;
}

ByteArray EquityInDB::XMASTERFiller8() const
{
    return m_XMASTERFiller8;
}

ByteArray EquityInDB::XMASTERFiller9() const
{
    return m_XMASTERFiller9;
}

ByteArray EquityInDB::XMASTERFiller10() const
{
    return m_XMASTERFiller10;
}

ByteArray EquityInDB::XMASTERFiller11() const
{
    return m_XMASTERFiller11;
}

ByteArray EquityInDB::XMASTERFiller12() const
{
    return m_XMASTERFiller12;
}

ByteArray EquityInDB::XMASTERFiller13() const
{
    return m_XMASTERFiller13;
}

ByteArray EquityInDB::XMASTERFiller14() const
{
    return m_XMASTERFiller14;
}


// Print the current equity
void EquityInDB::print() {
    // Compute filename to show file
    std::string fileName;
    if (m_dataFileType == EquityInDB::EDataFileTypes::EDataFileTypeFDAT) {
        fileName = "F" + to_string(m_dataFileNumber) + ".DAT";
    } else {
        fileName = "C" + to_string(m_dataFileNumber) + ".MWD";
    }

    cout << endl;
    cout << "Equity" << endl;
    cout << "------" << endl;
    cout << "Symbol...................................: " << m_symbol << endl;
    cout << "Description..............................: " << m_description << endl;
    cout << "Data file Type...........................: " << m_dataFileType << " (" << dataFileType2String(m_dataFileType) << ")" << endl;
    cout << "Data file Number.........................: " << m_dataFileNumber << endl;
    cout << "Data file name...........................: " << fileName << endl;
    cout << "Length of one record in data file........: " << static_cast<unsigned>(m_recordLength) << endl;
    cout << "Number of fields in data file............: " << static_cast<unsigned>(m_activeFields.numFields()) << endl;
    cout << "Active fieldsin data file................: " << m_activeFields.asBinaryAscii()   << " (" << m_activeFields.asLettersAscii() << ")" << endl;
    cout << "CT_V2_8_FLAG.............................: " << static_cast<unsigned>(m_CT_V2_8_FLAG) << endl;
    cout << "Interday Periodicity.....................: " << static_cast<char>(m_interdayPeriodicity) << " (" << InterdayPeriodicity2String(m_interdayPeriodicity) << ")" << endl;
    cout << "Intraday Periodicity.....................: " << static_cast<unsigned int>(m_intradayPeriodicity) << " (" << IntradayPeriodicity2String(m_intradayPeriodicity) << ")" << endl;
    cout << "Flag.....................................: " << static_cast<unsigned>(m_flag) << endl;
    cout << "ID Code..................................: " << static_cast<unsigned>(m_IDCode) << endl;
    cout << "AutoRun..................................: " << static_cast<unsigned>(m_autoRun) << endl;
    cout << "Intraday Periodicity Start Time(as float): " << m_intradayStartTime << endl;
    cout << "Intraday Periodicity End Time(as float)..: " << m_intradayEndTime << endl;
    cout << "Last Dividend Paid.......................: " << m_lastDivPaid << endl;
    cout << "Last Dividend Adjustment Rate............: " << m_lastDivAdjRate << endl;

    // Print the trading history
    m_tradingHistory.print(m_activeFields);

}

