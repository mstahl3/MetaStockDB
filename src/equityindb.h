/*
 * Class: EquityInDB
 * Author: Marc Stahl
 * Description: This class holds meta data about an equity, and also contains the object which holds the financial data. EquityInDB is a subsclass of equity.
 * History:
 *   MKS    2018-Jan-19   Original coding
 */

#ifndef EQUITYINDB_H
#define EQUITYINDB_H

#include <string>
#include "tradinghistory.h"
#include "date.h"
#include "bytearray.h"
#include "activefields.h"
#include "equity.h"


class EquityInDB : public Equity
{
public:

    // Type of file hold daily trading data
    enum EDataFileTypes {
        EDataFileTypeFDAT,
        EDataFileTypeMWD
    };

    // Valid interday periodicity (mapped to Letter used in file)
    enum EInterdayPeriodicity {
        EInterdayPeriodicityNone = 'N',
        EInterdayPeriodicityDaily = 'D',
        EInterdayPeriodicityWeekly = 'W',
        EInterdayPeriodicityMonthly = 'M',
        EInterdayPeriodicityQuarterly = 'Q',
        EInterdayPeriodicityYearly = 'Y'
    };

    // Valid intraday periodicity (mapped to value used in file)
    enum EIntradayPeriodicity {
        EIntradayPeriodicityNone = 0,
        EIntradayPeriodicity1min = 1,
        EIntradayPeriodicity5min = 5,
        EIntradayPeriodicity10min = 10,
        EIntradayPeriodicity1hr = 60
    };

    //constructor: set the data for this equity which comes from MASTER.
    EquityInDB(const unsigned long int FDATFileNum,
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
           const ByteArray MASTERFiller5);

    // Constructor: set the data for this equity which comes from XMASTER.
    EquityInDB(const ByteArray XMASTERFiller5,
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
           const ByteArray XMASTERFiller14);

    // Destructor required since this is subclass
    ~EquityInDB();

    //set the data for this equity which comes from EMASTER.
    void setEMASTERData(const unsigned char IDCode,
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
                        const ByteArray EMASTERFiller11);


    // Convert the data file type to an extension string
    static std::string dataFileType2String(const EquityInDB::EDataFileTypes dataFileType);

    // Convert the interday periodicity to a string
    static std::string InterdayPeriodicity2String(const EquityInDB::EInterdayPeriodicity interdayPeriodicity);

    // Convert the intraday periodicity to a string
    static std::string IntradayPeriodicity2String(const EquityInDB::EIntradayPeriodicity interdayPeriodicity);

    // GETTERS: each member variable is accessible from outside this object if you use these getters.
    unsigned long int TDFFileNum() const; // Returns the file number of this equities DAT or MWD file
    EDataFileTypes dataFileType() const; // Indicates if this equities Trading Data File(TDF) is DAT or MWD
    unsigned long int fileType() const; // Returns the fileType data field(Unsure what it means, but it has to be in the EMASTER file)
    unsigned char fieldLength() const; // Returns the length of one field in the FDAT file
    ActiveFields activeFields() const; // Returns the active fields object, which stores info about this equities TDF
    unsigned char CT_V2_8_FLAG() const;
    EInterdayPeriodicity interdayPeriodicity() const;
    EIntradayPeriodicity intradayPeriodicity() const;
    unsigned char flag() const;
    TradingHistory* tradingHistory();
    bool loaded() const;
    unsigned char IDCode() const;
    unsigned char activeFieldsBitmask() const;
    unsigned char autoRun() const;
    float intradayStartTime() const;
    float intradayEndTime() const;
    unsigned long int lastDivPaid() const;
    float lastDivAdjRate() const;

    ByteArray MASTERFiller2() const;
    ByteArray MASTERFiller3() const;
    ByteArray MASTERFiller4() const;
    ByteArray MASTERFiller5() const;

    ByteArray EMASTERFiller2() const;
    ByteArray EMASTERFiller3() const;
    ByteArray EMASTERFiller4() const;
    ByteArray EMASTERFiller5() const;
    ByteArray EMASTERFiller6() const;
    ByteArray EMASTERFiller7() const;
    ByteArray EMASTERFiller8() const;
    ByteArray EMASTERFiller9() const;
    ByteArray EMASTERFiller10() const;
    ByteArray EMASTERFiller11() const;

    ByteArray XMASTERFiller5() const;
    ByteArray XMASTERFiller6() const;
    ByteArray XMASTERFiller7() const;
    ByteArray XMASTERFiller8() const;
    ByteArray XMASTERFiller9() const;
    ByteArray XMASTERFiller10() const;
    ByteArray XMASTERFiller11() const;
    ByteArray XMASTERFiller12() const;
    ByteArray XMASTERFiller13() const;
    ByteArray XMASTERFiller14() const;

    // Print the current equity
    void print();

    //----------------------------------------------------------------------------
    // Override interface methods

        // Get the stock description
        std::string description() const;

        // Set the stock description
        void description(const std::string newDescription);

        // Get the stock symbol
        std::string symbol() const override;

        //  Get which fields are active
        bool fieldDateActive() const;
        bool fieldOpenActive() const;
        bool fieldHighActive() const;
        bool fieldLowActive()  const;
        bool fieldCloseActive() const;
        bool fieldVolumeActive() const;
        bool fieldOpenInterestActive() const;
        bool fieldTimeActive() const;


    //----------------------------------------------------------------------------
    // Trading day information


        // Get the first trading day
        Date firstTradingDayDate() const;

        // Get the last trading day
        Date lastTradingDayDate() const;

        // Adds the passed trading day data to the list of trading days
        // Returns true if succesfully added new day data
        bool addTradingDayData(const Date date, const Time time, const float open, const float close, const float high, const float low, const unsigned long volume, const float openInterest);

        // Reset at start of list, and copy first item in the list
        // into the parameter.  Return true if success, false otherwise
        bool getFirstTradingDayData(Date &date, Time &time, float &open, float &close, float &high, float &low, unsigned long &volume, float &openInterest);

        // Advance the iterator, and if there is another element
        // then copy element into the parameter.
        // Return true if success, false otherwise
        bool getNextTradingDayData(Date &date, Time &time, float &open, float &close, float &high, float &low, unsigned long &volume, float &openInterest);


private:

    unsigned long int m_dataFileNumber;  // Fx.DAT number (x)
    EDataFileTypes m_dataFileType;  // .DAT or .MWD file extension
    unsigned long int m_fileType;  // File type (reserved/unused)
    unsigned char m_recordLength;  // length of one field in the FDAT file.
    ActiveFields m_activeFields;  // object holding active fields bitmask / count

    std::string m_description;  // description of the company which this stock represents.
    unsigned char m_CT_V2_8_FLAG;  // If this is 89(Int) then V28 is true. If this is 42(Int) then Autorun is True.
    EquityInDB::EInterdayPeriodicity m_interdayPeriodicity;  // The frequency with which stock data is retrieved from the internet (frequency >= 1 day).
    EquityInDB::EIntradayPeriodicity m_intradayPeriodicity;  // The frequency with which stock data is retrieved from the internet (frequency < 1 day).
    std::string m_symbol;  // The symbol representing this equity.
    unsigned char m_flag;  // Not sure what this flag means.
    bool m_loaded;  // Is the underlying equity loaded from file (or attempted)

    // Extra fields from EMASTER
    unsigned char m_IDCode; // Unsure what this does
    //    unsigned char m_activeFieldsBitmask;
    unsigned char m_autoRun; // Unsure what this does
    float m_intradayStartTime; // Start time for an intraday periodicity
    float m_intradayEndTime; // End time for an intraday periodicity
    unsigned long int m_lastDivPaid; // Last Dividend paid
    float m_lastDivAdjRate; // Last Dividend Adjustment rate

    // Filler fields in MASTER
    ByteArray m_MASTERFiller2;  // Filler 2 in MASTER record
    ByteArray m_MASTERFiller3;  // Filler 3 in MASTER record
    ByteArray m_MASTERFiller4;  // Filler 4 in MASTER record
    ByteArray m_MASTERFiller5;  // Filler 5 in MASTER record

    // Filler fields in EMASTER
    ByteArray m_EMASTERFiller2;  // Filler 2 in EMASTER record
    ByteArray m_EMASTERFiller3;  // Filler 3 in EMASTER record
    ByteArray m_EMASTERFiller4;  // Filler 4 in EMASTER record
    ByteArray m_EMASTERFiller5;  // Filler 5 in EMASTER record
    ByteArray m_EMASTERFiller6;  // Filler 6 in EMASTER record
    ByteArray m_EMASTERFiller7;  // Filler 7 in EMASTER record
    ByteArray m_EMASTERFiller8;  // Filler 8 in EMASTER record
    ByteArray m_EMASTERFiller9;  // Filler 9 in EMASTER record
    ByteArray m_EMASTERFiller10;  // Filler 10 in EMASTER record
    ByteArray m_EMASTERFiller11;  // Filler 11 in EMASTER record

    // Filler fields in XMASTER
    ByteArray m_XMASTERFiller5;  // Filler 5 in XMASTER record
    ByteArray m_XMASTERFiller6;  // Filler 6 in XMASTER record
    ByteArray m_XMASTERFiller7;  // Filler 7 in XMASTER record
    ByteArray m_XMASTERFiller8;  // Filler 8 in XMASTER record
    ByteArray m_XMASTERFiller9;  // Filler 9 in XMASTER record
    ByteArray m_XMASTERFiller10;  // Filler 10 in XMASTER record
    ByteArray m_XMASTERFiller11;  // Filler 11 in XMASTER record
    ByteArray m_XMASTERFiller12;  // Filler 12 in XMASTER record
    ByteArray m_XMASTERFiller13;  // Filler 13 in XMASTER record
    ByteArray m_XMASTERFiller14;  // Filler 14 in XMASTER record

    TradingHistory m_tradingHistory;  // Trading history for this equity


};

#endif // EQUITYINDB_H
