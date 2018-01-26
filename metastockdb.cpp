/*
 * Class: MetaStockDB
 * Author: Marc Stahl
 * Description: This class is responsible for reading data from files and populating the data structures in this program
 * History:
 *   MKS    2018-Jan-19   Original coding
 */

#include <iostream>
#include <fstream>
#include <string>
#include "metastockdb.h"
#include "msfileio.h"
#include "bytearray.h"
#include "equityindb.h"
#include "activefields.h"

using namespace std;

// MASTER header offsets
#define MASTER_NUMRECORDS_FILE_OFFSET                    0      // Length: 2
#define MASTER_LARGEST_FDATNUM_FILE_OFFSET               2      // Length: 2
#define MASTER_FILLER1_FILE_OFFSET                       4      // Length: 48
#define MASTER_RECORD_SIZE                               53     // Size of a single MASTER record

// MASTER record offsets
#define MASTER_FDAT_FILENUM_RECORD_OFFSET                0      // Length: 1
#define MASTER_FILETYPE_RECORD_OFFSET                    1      // Length: 2
#define MASTER_BYTEFIELD_LENGTH_RECORD_OFFSET            3      // Length: 1
#define MASTER_NUMFIELDS_RECORD_OFFSET                   4      // Length: 1
#define MASTER_FILLER2_RECORD_OFFSET                     5      // Length: 2, Unknown
#define MASTER_DESCRIPTION_RECORD_OFFSET                 7      // Length: 16
#define MASTER_FILLER3_RECORD_OFFSET                     23     // Length: 1, Unknown
#define MASTER_CT_V2_8_FLAG_RECORD_OFFSET                24     // Length: 1
#define MASTER_FIRST_DATE_RECORD_OFFSET                  25     // Length: 4
#define MASTER_LAST_DATE_RECORD_OFFSET                   29     // Length: 4
#define MASTER_INTERDAY_P_RECORD_OFFSET                  33     // Length: 1
#define MASTER_INTRADAY_P_RECORD_OFFSET                  34     // Length: 2
#define MASTER_SYMBOL_RECORD_OFFSET                      36     // Length: 14
#define MASTER_FILLER4_RECORD_OFFSET                     50     // Length: 1, Unknown
#define MASTER_FLAG_RECORD_OFFSET                        51     // Length: 1
#define MASTER_FILLER5_RECORD_OFFSET                     52     // Length: 1, Unknown

// EMASTER header offsets
#define EMASTER_NUMRECORDS_FILE_OFFSET                   0      // Length: 2
#define EMASTER_LARGEST_FDATNUM_FILE_OFFSET              2      // Length: 2
#define EMASTER_FILLER1_FILE_OFFSET                      4      // Length: 187, Unknown
#define EMASTER_RECORD_SIZE                              192    // Size of a single EMASTER record

// EMASTER record offsets
#define EMASTER_ID_CODE_RECORD_OFFSET                    0      // Length: 2
#define EMASTER_FDAT_FILENUM_RECORD_OFFSET               2      // Length: 1, Duplicates data in MASTER
#define EMASTER_FILLER2_RECORD_OFFSET                    3      // Length: 3, Unknown
#define EMASTER_NUM_ACTIVE_FIELDS_RECORD_OFFSET          6      // Length: 1, Duplicates data in MASTER
#define EMASTER_BITMASK_ACTIVE_FIELDS_RECORD_OFFSET      7      // Length: 1
#define EMASTER_FILLER3_RECORD_OFFSET                    8      // Length: 1, Unknown
#define EMASTER_AUTO_RUN_RECORD_OFFSET                   9      // Length: 1
#define EMASTER_FILLER4_RECORD_OFFSET                    10     // Length: 1, Unknown
#define EMASTER_SYMBOL_RECORD_OFFSET                     11     // Length: 13, Duplicates data in MASTER
#define EMASTER_FILLER5_RECORD_OFFSET                    24     // Length: 8, Unknown
#define EMASTER_DESCRIPTION_RECORD_OFFSET                32     // Length: 16, Duplicates data in MASTER
#define EMASTER_FILLER6_RECORD_OFFSET                    48     // Length: 16, Unknown
#define EMASTER_FIRST_DATE_RECORD_OFFSET                 64     // Length: 4, Duplicates data in MASTER
#define EMASTER_FILLER7_RECORD_OFFSET                    68     // Length: 4, Unknown
#define EMASTER_LAST_DATE_RECORD_OFFSET                  72     // Length: 4, Duplicates data in MASTER
#define EMASTER_FILLER8_RECORD_OFFSET                    76     // Length: 4, Unknown
#define EMASTER_INTRADAY_START_TIME_RECORD_OFFSET        80     // Length: 4
#define EMASTER_INTRADAY_END_TIME_RECORD_OFFSET          84     // Length: 4
#define EMASTER_FILLER9_RECORD_OFFSET                    88     // Length: 38, Unknown
#define EMASTER_FIRST_DATE_LONG_RECORD_OFFSET            126    // Length: 4, Long format of FIRST DATE
#define EMASTER_FILLER10_RECORD_OFFSET                   130    // Length: 1, Unknown
#define EMASTER_LAST_DIV_PAID_RECORD_OFFSET              131    // Length: 4
#define EMASTER_LAST_DIV_ADJUSTMENT_RATE_RECORD_OFFSET   135    // Length: 4
#define EMASTER_FILLER11_RECORD_OFFSET                   139    // Length: 53, Unknown

// XMASTER header offsets
#define XMASTER_FILLER1_FILE_OFFSET                      4      // Length: 6
#define XMASTER_NUMRECORDS_FILE_OFFSET                   10     // Length: 2
#define XMASTER_FILLER2_FILE_OFFSET                      12     // Length: 2
#define XMASTER_FILLER3_FILE_OFFSET                      16     // Length: 2
#define XMASTER_LARGEST_FDATNUM_FILE_OFFSET              18     // Length: 2
#define XMASTER_FILLER4_FILE_OFFSET                      20     // Length: 129
#define XMASTER_RECORD_SIZE                              150    // Size of a single XMASTER record

// XMASTER record offsets
#define XMASTER_FILLER5_RECORD_OFFSET                    0      // Length: 1
#define XMASTER_SYMBOL_RECORD_OFFSET                     1      // Length: 14
#define XMASTER_DESCRIPTION_RECORD_OFFSET                16     // Length: 23
#define XMASTER_FILLER6_RECORD_OFFSET                    40     // Length: 1
#define XMASTER_FILLER7_RECORD_OFFSET                    42     // Length: 5
#define XMASTER_FILLER8_RECORD_OFFSET                    47     // Length: 13
#define XMASTER_INTERDAY_P_RECORD_OFFSET                 61     // Length: 1
#define XMASTER_FILLER9_RECORD_OFFSET                    62     // Length: 1
#define XMASTER_FDAT_FILENUM_RECORD_OFFSET               65     // Length: 2
#define XMASTER_FILLER10_RECORD_OFFSET                   67     // Length: 2
#define XMASTER_BITMASK_ACTIVE_FIELDS_RECORD_OFFSET      70     // Length: 1
#define XMASTER_FILLER11_RECORD_OFFSET                   71     // Length: 7
#define XMASTER_FIRST_DATE_LONG_RECORD_OFFSET            80     // Length: 4
#define XMASTER_FIRST_DATE_RECORD_OFFSET                 84     // Length: 4
#define XMASTER_FILLER12_RECORD_OFFSET                   86     // Length: 16
#define XMASTER_LAST_DATE_LONG_RECORD_OFFSET             104    // Length: 4
#define XMASTER_LAST_DATE_RECORD_OFFSET                  108    // Length: 4
#define XMASTER_FILLER13_RECORD_OFFSET                   112    // Length: 3
#define XMASTER_FILLER14_RECORD_OFFSET                   120    // Length: 29

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

//The offset into the first record of a TDF where the number of records resides
#define TRADINGDATAFILE_NUM_RECORDS_OFFSET               2



// Open a metastock database at the specified path
// Reads the master file(s) and dat files (if not lazy loading)
// If path exists but no files, or path does not exist, assume creating a new DB
// If lazyLoad, then read the equity data from the master file(s) now but only
// read from the FDAT files when needed
// Creates an map of equities kept in memory
// Number of backups to save (backups only created once per instance)
MetaStockDB::MetaStockDB(const string dbpath, const bool lazyLoad, const unsigned char numBackups) :
    m_lastError(EErrorNone),
    m_lastErrorMessage(""),
    m_DBpath(dbpath),
    m_lazyLoad(lazyLoad),
    m_isnew(true),
    m_numBackups(numBackups),
    m_DBerror(false),
    m_MasterNumRecords(0),
    m_MasterLastDataFileNumber(0),
    m_XMasterNumRecords(0),
    m_XMasterLastDataFileNumber(0),
    //Create new ByteArray objects to represent the filler fields
    m_MASTERFiller1(ByteArray(MASTER_FILLER1_LENGTH)),
    m_EMASTERFiller1(ByteArray(EMASTER_FILLER1_LENGTH)),
    m_XMASTERFiller1(ByteArray(XMASTER_FILLER1_LENGTH)),
    m_XMASTERFiller2(ByteArray(XMASTER_FILLER2_LENGTH)),
    m_XMASTERFiller3(ByteArray(XMASTER_FILLER3_LENGTH)),
    m_XMASTERFiller4(ByteArray(XMASTER_FILLER4_LENGTH))
{

    // Ensure DB path has a trailing slash
    if ( (m_DBpath.empty()) || (m_DBpath.back() != PATHSEPERATOR ) ) m_DBpath += PATHSEPERATOR;

    // If encountered an error reading ?MASTER file
    bool readMasterOK = false;

    // Check if path exists.  If not create it as new DB
    if (MSFileIO::DBPathExists(m_DBpath)) {

        // If the MASTER exists
        if (MSFileIO::fileInDBPathExists(m_DBpath,"MASTER")) {

            // This is not a new database
            m_isnew = false;

            // Attempt to read the MASTER file
            readMasterOK = readMasterFile();

            // If MASTER read ok, check for an EMASTER
            if ( (readMasterOK) && (MSFileIO::fileInDBPathExists(m_DBpath,"EMASTER")) ) {

                // Attempt to read the EMASTER file
                readMasterOK = readEMasterFile();

                // If EMASTER read ok, check for an EMASTER
                if ( (readMasterOK) && (MSFileIO::fileInDBPathExists(m_DBpath,"XMASTER")) ) {

                    // Attempt to read the XMASTER file - set flag if error
                    readMasterOK = readXMasterFile();
                } // If XMASTER exists
            }  // if EMASTER exists
        }  // If MASTER exists
    }  // If DB path exists

    // Path does not exist so create path
    else {
        MSFileIO::makeDBPath(m_DBpath);

        // Success if read the path
        if (!MSFileIO::DBPathExists(m_DBpath)) {
            m_lastError = EErrorDBpathCreateFailed;
            m_lastErrorMessage = "Error creating the DB path '"+m_DBpath+"'";
        }
    }

    // If this is NOT a new database, and all ?MASTER read ok, and not lazy loading, then load
    // data now
    if (( !m_isnew) && (readMasterOK) && (!m_lazyLoad)) {
        populateTradingData();
    }
}


// Destructor
MetaStockDB::~MetaStockDB()
{
    map<string, EquityInDB*>::iterator it;

    //Delete all EquityInDB objects pointed to by the map
    for(it = m_equityMap.begin(); it != m_equityMap.end(); it++)
        delete (it->second);

    //Delete the map
    m_equityMap.clear(); // Just for clarity, not neccessary
}


//============================================================================
// Getters


// Return the last error code
MetaStockDB::EErrors MetaStockDB::lastError() const {
    return m_lastError;
}


// Return the last error message
std::string MetaStockDB::lastErrorMessage() const {
    return m_lastErrorMessage;
}


//============================================================================
// MASTER file

// Read the MASTER file and put the data in the map member variable m_equities
bool MetaStockDB::readMasterFile()
{

    bool errorOccured;  // were there any problems that occured in this function
    unsigned char tempInterdayPeriodicity;
    unsigned long int tempIntradayPeriodicity;
    ifstream file(m_DBpath+"MASTER", ios::in|ios::binary);

    //These variables are for holding the data that is read from MASTER before it is added to the map
    unsigned long int TDFFileNum;
    unsigned long int fileType;
    unsigned char fieldLength;
    unsigned char numFields;
    ByteArray MASTERFiller2(MASTER_FILLER2_LENGTH);
    string description;
    ByteArray MASTERFiller3(MASTER_FILLER3_LENGTH);
    unsigned char CT_V2_8_FLAG;
    Date firstDate;
    Date lastDate;
    EquityInDB::EInterdayPeriodicity interdayPeriodicity;
    EquityInDB::EIntradayPeriodicity intradayPeriodicity;
    string symbol;
    ByteArray MASTERFiller4(MASTER_FILLER4_LENGTH);
    unsigned char flag;
    ByteArray MASTERFiller5(MASTER_FILLER5_LENGTH);
    unsigned char tempUByte;


    errorOccured = false;  // Assume no error
    while (file.is_open())
    {
        // read the 3 pieces of data from the header record
        if (!MSFileIO::readUIntFromFile(file, MASTER_NUMRECORDS_FILE_OFFSET, m_MasterNumRecords, MSFileIO::EVariableTypeUShort ))
        {
            errorOccured = true;
            m_lastError = EErrorMASTERHeaderRead;
            m_lastErrorMessage = "Error reading MASTER_NUMRECORDS_FILE_OFFSET in header from MASTER file";
            break;
        }
        if (!MSFileIO::readUIntFromFile(file, MASTER_LARGEST_FDATNUM_FILE_OFFSET, m_MasterLastDataFileNumber, MSFileIO::EVariableTypeUShort))
        {
            errorOccured = true;
            m_lastError = EErrorMASTERHeaderRead;
            m_lastErrorMessage = "Error reading LARGEST_FDATNUM_FILE_OFFSET in header from MASTER file";
            break;
        }
        if (!MSFileIO::readByteArrayFromFile(file, MASTER_FILLER1_FILE_OFFSET, m_MASTERFiller1))
        {
            errorOccured = true;
            m_lastError = EErrorMASTERHeaderFillerRead;
            m_lastErrorMessage = "Error reading FILLER1 in header from MASTER file";
            break;
        }

        // Loop through remaining records to read equities
        for(unsigned int  recordNum = 1; recordNum <= m_MasterNumRecords; recordNum++)
        {
            // Attempt to read all essential data from file, or break with failure
            if (!MSFileIO::readUByteFromFile(file, recordNum * MASTER_RECORD_SIZE + MASTER_FDAT_FILENUM_RECORD_OFFSET, tempUByte))
            {
                m_lastError = EErrorMASTERRecordRead;
                m_lastErrorMessage = "Error reading FDAT_FILENUM_RECORD_OFFSET in record from MASTER file";
                errorOccured = true;
                break;
            }
            //Move this datum from an unsigned char to the least significant byte of an unsigned long.
            //This operation must be done because MASTER stores TDFFileNum in 1 byte, but EquityInDB must store it as a 4 byte long.
            TDFFileNum = 0;
            TDFFileNum = tempUByte;

            if (!MSFileIO::readUIntFromFile(file, recordNum * MASTER_RECORD_SIZE + MASTER_FILETYPE_RECORD_OFFSET, fileType, MSFileIO::EVariableTypeUShort))
            {
                m_lastError = EErrorMASTERRecordRead;
                m_lastErrorMessage = "Error reading FILETYPE_RECORD_OFFSET in record from MASTER file";
                errorOccured = true;
                break;
            }
            if (!MSFileIO::readUByteFromFile(file, recordNum * MASTER_RECORD_SIZE + MASTER_BYTEFIELD_LENGTH_RECORD_OFFSET, fieldLength))
            {
                m_lastError = EErrorMASTERRecordRead;
                m_lastErrorMessage = "Error reading BYTEFIELD_LENGTH_RECORD_OFFSET in record from MASTER file";
                errorOccured = true;
                break;
            }
            if (!MSFileIO::readUByteFromFile(file, recordNum * MASTER_RECORD_SIZE + MASTER_NUMFIELDS_RECORD_OFFSET, numFields))
            {
                m_lastError = EErrorMASTERRecordRead;
                m_lastErrorMessage = "Error reading NUMFIELDS_RECORD_OFFSET in record from MASTER file";
                errorOccured = true;
                break;
            }
            if (!MSFileIO::readByteArrayFromFile(file, recordNum * MASTER_RECORD_SIZE + MASTER_FILLER2_RECORD_OFFSET, MASTERFiller2))
            {
                m_lastError = EErrorMASTERRecordFillerRead;
                m_lastErrorMessage = "Error reading FILLER2 in record from MASTER file";
                errorOccured = true;
                break;
            }
            if (!MSFileIO::readStringFromFile(file, recordNum * MASTER_RECORD_SIZE + MASTER_DESCRIPTION_RECORD_OFFSET, 16, description))
            {
                m_lastError = EErrorMASTERRecordRead;
                m_lastErrorMessage = "Error reading DESCRIPTION_RECORD_OFFSET in record from MASTER file";
                errorOccured = true;
                break;
            }
            if (!MSFileIO::readByteArrayFromFile(file, recordNum * MASTER_RECORD_SIZE + MASTER_FILLER3_RECORD_OFFSET, MASTERFiller3))
            {
                m_lastError = EErrorMASTERRecordFillerRead;
                m_lastErrorMessage = "Error reading FILLER3 in record from MASTER file";
                errorOccured = true;
                break;
            }
            if (!MSFileIO::readUByteFromFile(file, recordNum * MASTER_RECORD_SIZE + MASTER_CT_V2_8_FLAG_RECORD_OFFSET, CT_V2_8_FLAG))
            {
                m_lastError = EErrorMASTERRecordRead;
                m_lastErrorMessage = "Error reading record from MASTER file";
                errorOccured = true;
                break;
            }
            if (!MSFileIO::readDateFromFile(file, recordNum * MASTER_RECORD_SIZE + MASTER_FIRST_DATE_RECORD_OFFSET, firstDate, MSFileIO::EVariableTypeMBF32))
            {
                m_lastError = EErrorMASTERRecordRead;
                m_lastErrorMessage = "Error reading FIRST_DATE_RECORD_OFFSET in record from MASTER file";
                errorOccured = true;
                break;
            }
            if (!MSFileIO::readDateFromFile(file, recordNum * MASTER_RECORD_SIZE + MASTER_LAST_DATE_RECORD_OFFSET, lastDate, MSFileIO::EVariableTypeMBF32))
            {
                m_lastError = EErrorMASTERRecordRead;
                m_lastErrorMessage = "Error reading LAST_DATE_RECORD_OFFSET in record from MASTER file";
                errorOccured = true;
                break;
            }
            if (!MSFileIO::readUByteFromFile(file, recordNum * MASTER_RECORD_SIZE + MASTER_INTERDAY_P_RECORD_OFFSET, tempInterdayPeriodicity))
            {
                m_lastError = EErrorMASTERRecordRead;
                m_lastErrorMessage = "Error reading INTERDAY_P_RECORD_OFFSET in record from MASTER file";
                errorOccured = true;
                break;
            }
            if (!MSFileIO::readUIntFromFile(file, recordNum * MASTER_RECORD_SIZE + MASTER_INTRADAY_P_RECORD_OFFSET, tempIntradayPeriodicity, MSFileIO::EVariableTypeUShort))
            {
                m_lastError = EErrorMASTERRecordRead;
                m_lastErrorMessage = "Error reading INTRADAY_P_RECORD_OFFSET in record from MASTER file";
                errorOccured = true;
                break;
            }
            if (!MSFileIO::readStringFromFile(file, recordNum * MASTER_RECORD_SIZE + MASTER_SYMBOL_RECORD_OFFSET, 14, symbol))
            {
                m_lastError = EErrorMASTERRecordRead;
                m_lastErrorMessage = "Error reading SYMBOL_RECORD_OFFSET in record from MASTER file";
                errorOccured = true;
                break;
            }
            if (!MSFileIO::readByteArrayFromFile(file, recordNum * MASTER_RECORD_SIZE + MASTER_FILLER4_RECORD_OFFSET, MASTERFiller4))
            {
                m_lastError = EErrorMASTERRecordFillerRead;
                m_lastErrorMessage = "Error reading FILLER4 in record from MASTER file";
                errorOccured = true;
                break;
            }
            if (!MSFileIO::readUByteFromFile(file, recordNum * MASTER_RECORD_SIZE + MASTER_FLAG_RECORD_OFFSET, flag))
            {
                m_lastError = EErrorMASTERRecordRead;
                m_lastErrorMessage = "Error reading FLAG_RECORD_OFFSET in record from MASTER file";
                errorOccured = true;
                break;
            }
            if (!MSFileIO::readByteArrayFromFile(file, recordNum * MASTER_RECORD_SIZE + MASTER_FILLER5_RECORD_OFFSET, MASTERFiller5))
            {
                m_lastError = EErrorMASTERRecordFillerRead;
                m_lastErrorMessage = "Error reading FILLER5 in record from MASTER file";
                errorOccured = true;
                break;
            }

            // Trim trailing spaces from string fields
            MSFileIO::trim(description);
            MSFileIO::trim(symbol);

            // Convert interday periodicity(unsigned char) to valid enum or fail
            if  (  (tempInterdayPeriodicity == 'N') || (tempInterdayPeriodicity == 'D') || (tempInterdayPeriodicity == 'W') ||
                   (tempInterdayPeriodicity == 'M') || (tempInterdayPeriodicity == 'Q') || (tempInterdayPeriodicity == 'Y')  )
                interdayPeriodicity = static_cast<EquityInDB::EInterdayPeriodicity>(tempInterdayPeriodicity);
            else
            {
                m_lastError = EErrorMASTERRecordValue;
                m_lastErrorMessage = "Error interpreding InterdayPeriodicity in record from MASTER file";
                errorOccured = true;
                break;
            }

            // Convert intraday periodicity(unsigned int) to valid enum or fail
            if  (  (tempIntradayPeriodicity == EquityInDB::EIntradayPeriodicityNone) ||
                   (tempIntradayPeriodicity == EquityInDB::EIntradayPeriodicity1min) ||
                   (tempIntradayPeriodicity == EquityInDB::EIntradayPeriodicity5min) ||
                   (tempIntradayPeriodicity == EquityInDB::EIntradayPeriodicity10min) ||
                   (tempIntradayPeriodicity == EquityInDB::EIntradayPeriodicity1hr)  )
                intradayPeriodicity = static_cast<EquityInDB::EIntradayPeriodicity>(tempIntradayPeriodicity);
            else
            {
                m_lastError = EErrorMASTERRecordValue;
                m_lastErrorMessage = "Error interpreding IntradayPeriodicity in record from MASTER file";
                errorOccured = true;
                break;
            }

            // Add the object to our map container
            m_equityMap.insert(std::pair<string, EquityInDB*>(symbol, new EquityInDB(TDFFileNum, fileType, fieldLength, static_cast<unsigned int>(numFields), MASTERFiller2,
                                                                             description, MASTERFiller3, CT_V2_8_FLAG, firstDate, lastDate,
                                                                             interdayPeriodicity, intradayPeriodicity, symbol, MASTERFiller4,
                                                                             flag, MASTERFiller5)));

        }
        break;
    }
    // If the file is open then close it
    if (file.is_open())
        file.close();
    else
    {
        m_lastError = EErrorMASTERFileOpenFailed;
        m_lastErrorMessage = "Failed to open MASTER file '" + m_DBpath + "'";
        errorOccured = true;
    }

    // Return whether file was read entirely without a problem
    return !errorOccured;

}


//============================================================================
// EMASTER file

// Read the EMASTER file and put the data in the map member variable m_equities
bool MetaStockDB::readEMasterFile()
{

    bool errorOccured; // were there any problems that occured in this function
    map<string, EquityInDB*>::iterator currentEquityIterator; // an iterator that stores the position of the equity currently in use.
    ifstream file(m_DBpath+"EMASTER", ios::in|ios::binary); // the EMASTER file.
    string currentSymbol; // this is the map key. It is read from the file and used to find the correct equity in the map.

    // Info from EMASTER that is also in MASTER; gets compared to ensure a match.
    unsigned long int EMASTERNumRecords;
    unsigned long int EMASTERLastFDATNum;
    unsigned long int tempTDFFileNum;
    unsigned char tempNumFields;
    unsigned char tempActiveFieldsBitmask;
    string tempDescription;
    Date tempFirstDate;
    Date tempLastDate;

    // Info from EMASTER that is only in EMASTER; gets assigned to corresponding equity in map.
    unsigned char IDCode;
    ByteArray EMASTERFiller2(EMASTER_FILLER2_LENGTH);
    ByteArray EMASTERFiller3(EMASTER_FILLER3_LENGTH);
    unsigned char autoRun;
    ByteArray EMASTERFiller4(EMASTER_FILLER4_LENGTH);
    ByteArray EMASTERFiller5(EMASTER_FILLER5_LENGTH);
    ByteArray EMASTERFiller6(EMASTER_FILLER6_LENGTH);
    ByteArray EMASTERFiller7(EMASTER_FILLER7_LENGTH);
    ByteArray EMASTERFiller8(EMASTER_FILLER8_LENGTH);
    float intradayStartTime;
    ByteArray EMASTERFiller9(EMASTER_FILLER9_LENGTH);
    float intradayEndTime;
    ByteArray EMASTERFiller10(EMASTER_FILLER10_LENGTH);
    unsigned long int lastDivPaid;
    float lastDivAdjRate;
    ByteArray EMASTERFiller11(EMASTER_FILLER11_LENGTH);

    //Temporary holding place for 1 byte of data that is read from the file
    unsigned char tempUByte;

    //Error flag starts as false, and if there is an error, it it set to true
    errorOccured = false;
    while (file.is_open())
    {
        //--------------------------------------------------------------------
        // read the header
        if (!MSFileIO::readUIntFromFile(file, EMASTER_NUMRECORDS_FILE_OFFSET, EMASTERNumRecords, MSFileIO::EVariableTypeUShort))
        {
            errorOccured = true;
            m_lastError = EErrorEMASTERHeaderRead;
            m_lastErrorMessage = "Error reading NUMRECORDS_FILE_OFFSET in header from EMASTER file";
            break;
        }
        if (!MSFileIO::readUIntFromFile(file, EMASTER_LARGEST_FDATNUM_FILE_OFFSET, EMASTERLastFDATNum, MSFileIO::EVariableTypeUShort))
        {
            errorOccured = true;
            m_lastError = EErrorEMASTERHeaderRead;
            m_lastErrorMessage = "Error reading LARGEST_FDATNUM_FILE_OFFSET in header from EMASTER file";
            break;
        }
        if (!MSFileIO::readByteArrayFromFile(file, EMASTER_FILLER1_FILE_OFFSET, m_EMASTERFiller1))
        {
            errorOccured = true;
            m_lastError = EErrorEMASTERHeaderFillerRead;
            m_lastErrorMessage = "Error reading FILLER1 in header from EMASTER file";
            break;
        }

        if ( (m_MasterNumRecords != EMASTERNumRecords) || (m_MasterLastDataFileNumber != EMASTERLastFDATNum) )
        {
            errorOccured = true;
            m_lastError = EErrorEMASTERHeaderMismatch;
            m_lastErrorMessage = "EMASTER header does not match MASTER header";
            break;
        }

        // Loop through remaining records to read equities
        for(unsigned int recordNum = 1; recordNum <= m_MasterNumRecords; recordNum++)
        {
            // Read the stock symbol from EMASTER. The stock symbol will be used as a key to find the corresponding stock in the map m_equities.
            if ( !MSFileIO::readStringFromFile(file, (recordNum * EMASTER_RECORD_SIZE) + EMASTER_SYMBOL_RECORD_OFFSET, 13, currentSymbol) )
            {
                m_lastError = EErrorEMASTERRecordRead;
                m_lastErrorMessage = "Error reading SYMBOL_RECORD_OFFSET in record from MASTER file";
                errorOccured = true;
                break;
            }
            MSFileIO::trim(currentSymbol);

            // Return an iterator pointing to the current symbol in the map
            currentEquityIterator = m_equityMap.find(currentSymbol);
            if (currentEquityIterator == m_equityMap.end())
            {
                m_lastError = EErrorEMASTERRecordValue;
                m_lastErrorMessage = "Symbol '"+currentSymbol+"' in EMASTER not found in MASTER";
                errorOccured = true;
                break;
            }

            //-------------------------------------------------------------------------------------------------
            // Read all EMASTER data that is also in MASTER

            if (!MSFileIO::readUByteFromFile(file, recordNum * EMASTER_RECORD_SIZE + EMASTER_FDAT_FILENUM_RECORD_OFFSET, tempUByte))
            {
                m_lastError = EErrorEMASTERRecordRead;
                m_lastErrorMessage = "Error reading FDAT_FILENUM_RECORD_OFFSET in record from EMASTER file";
                errorOccured = true;
                break;
            }

            //Move this datum from an unsigned char to the least significant byte of an unsigned long.
            //This operation must be done because MASTER stores TDFFileNum in 1 byte, but EquityInDB must store it as a 4 byte long.
            tempTDFFileNum = 0;
            tempTDFFileNum = tempUByte;

            // Read number of fields from EMASTER file, for current equity
            if (!MSFileIO::readUByteFromFile(file, recordNum * EMASTER_RECORD_SIZE + EMASTER_NUM_ACTIVE_FIELDS_RECORD_OFFSET, tempNumFields))
            {
                m_lastError = EErrorEMASTERRecordRead;
                m_lastErrorMessage = "Error reading NUMFIELDS_RECORD_OFFSET in record from EMASTER file";
                errorOccured = true;
                break;
            }
            if (!MSFileIO::readStringFromFile(file, recordNum * EMASTER_RECORD_SIZE + EMASTER_DESCRIPTION_RECORD_OFFSET, 16, tempDescription))
            {
                m_lastError = EErrorEMASTERRecordRead;
                m_lastErrorMessage = "Error reading DESCRIPTION_RECORD_OFFSET in record from EMASTER file";
                errorOccured = true;
                break;
            }
            if (!MSFileIO::readDateFromFile(file, recordNum * EMASTER_RECORD_SIZE + EMASTER_FIRST_DATE_RECORD_OFFSET, tempFirstDate, MSFileIO::EVariableTypeCVS))
            {
                m_lastError = EErrorEMASTERRecordRead;
                m_lastErrorMessage = "Error reading FIRST_DATE_RECORD_OFFSET in record from EMASTER file";
                errorOccured = true;
                break;
            }
            if (!MSFileIO::readDateFromFile(file, recordNum * EMASTER_RECORD_SIZE + EMASTER_LAST_DATE_RECORD_OFFSET, tempLastDate, MSFileIO::EVariableTypeCVS))
            {
                m_lastError = EErrorEMASTERRecordRead;
                m_lastErrorMessage = "Error reading LAST_DATE_RECORD_OFFSET in record from EMASTER file";
                errorOccured = true;
                break;
            }

            //--------------------------------------------------------------------------------------------------
            //Compare EMASTER and MASTER data and set the error messege if they don't match.
            if ((currentEquityIterator->second)->TDFFileNum() != tempTDFFileNum)
            {
                m_lastError = EErrorEMASTERHeaderMismatch;
                m_lastErrorMessage = "Master and EMaster contradict. MASTER: FDATFileNum: " + to_string((currentEquityIterator->second)->TDFFileNum()) +
                        ". EMASTER: FDATFileNum: " + to_string(tempTDFFileNum);
                errorOccured = true;
                break;
            }

            // Compare number of fields specified in EMASTER to that read from MASTER
            if (((currentEquityIterator->second)->activeFields()).numFields() != tempNumFields)
            {
                m_lastError = EErrorEMASTERHeaderMismatch;
                m_lastErrorMessage = "Master and EMaster contradict. MASTER: NumByteFields: " + to_string((currentEquityIterator->second)->activeFields().numFields()) +
                        ". EMASTER: NumByteFields: " + to_string(tempNumFields);
                errorOccured = true;
                break;
            }

            // Compare description read from EMASTER to that read from MASTER
            if ((currentEquityIterator->second)->description() != tempDescription)
            {
                m_lastError = EErrorEMASTERHeaderMismatch;
                m_lastErrorMessage = "Master and EMaster contradict. MASTER: description: " + (currentEquityIterator->second)->description() +
                        ". EMASTER: description: " + tempDescription;
                errorOccured = true;
                break;
            }

            // Compare the first date read from EMASTER to that read from MASTER
            if ((currentEquityIterator->second)->tradingHistory()->firstDate() != tempFirstDate)
            {
                m_lastError = EErrorEMASTERHeaderMismatch;
                m_lastErrorMessage = "Master and EMaster contradict. MASTER: firstDate: " + ((currentEquityIterator->second)->tradingHistory()->firstDate()).asString(Date::EDateFormatYYYYMMMDD) +
                        ". EMASTER: firstDate: " + tempFirstDate.asString(Date::EDateFormatYYYYMMMDD);
                errorOccured = true;
                break;
            }

            // Compare the last date read from EMASTER to that read from MASTER
            if ((currentEquityIterator->second)->tradingHistory()->lastDate() != tempLastDate)
            {
                m_lastError = EErrorEMASTERHeaderMismatch;
                m_lastErrorMessage = "Master and EMaster contradict. MASTER: lastDate: " + ((currentEquityIterator->second)->tradingHistory()->lastDate()).asString(Date::EDateFormatYYYYMMMDD) +
                        ". EMASTER: lastDate: " + tempLastDate.asString(Date::EDateFormatYYYYMMMDD);
                errorOccured = true;
                break;
            }

            //------------------------------------------------------------------------------------------
            // Read all EMASTER data that is only in EMASTER
            if (!MSFileIO::readUByteFromFile(file, recordNum * EMASTER_RECORD_SIZE + EMASTER_ID_CODE_RECORD_OFFSET, IDCode))
            {
                m_lastError = EErrorEMASTERRecordRead;
                m_lastErrorMessage = "Error reading ID_CODE_RECORD_OFFSET in record from EMASTER file";
                errorOccured = true;
                break;
            }
            if (!MSFileIO::readByteArrayFromFile(file, recordNum * EMASTER_RECORD_SIZE + EMASTER_FILLER2_RECORD_OFFSET, EMASTERFiller2))
            {
                m_lastError = EErrorEMASTERRecordFillerRead;
                m_lastErrorMessage = "Error reading FILLER2 in record from EMASTER file";
                errorOccured = true;
                break;
            }
            if (!MSFileIO::readUByteFromFile(file, recordNum * EMASTER_RECORD_SIZE + EMASTER_BITMASK_ACTIVE_FIELDS_RECORD_OFFSET, tempActiveFieldsBitmask))
            {
                m_lastError = EErrorEMASTERRecordRead;
                m_lastErrorMessage = "Error reading ACTIVE_BYTEFIELDS_RECORD_OFFSET in record from EMASTER file";
                errorOccured = true;
                break;
            }
            if (!MSFileIO::readByteArrayFromFile(file, recordNum * EMASTER_RECORD_SIZE + EMASTER_FILLER3_RECORD_OFFSET, EMASTERFiller3))
            {
                m_lastError = EErrorEMASTERRecordFillerRead;
                m_lastErrorMessage = "Error reading FILLER3 in record from EMASTER file";
                errorOccured = true;
                break;
            }
            if (!MSFileIO::readUByteFromFile(file, recordNum * EMASTER_RECORD_SIZE + EMASTER_AUTO_RUN_RECORD_OFFSET, autoRun))
            {
                m_lastError = EErrorEMASTERRecordRead;
                m_lastErrorMessage = "Error reading AUTO_RUN_RECORD_OFFSET in record from EMASTER file";
                errorOccured = true;
                break;
            }
            if (!MSFileIO::readByteArrayFromFile(file, recordNum * EMASTER_RECORD_SIZE + EMASTER_FILLER4_RECORD_OFFSET, EMASTERFiller4))
            {
                m_lastError = EErrorEMASTERRecordFillerRead;
                m_lastErrorMessage = "Error reading FILLER3 in record from EMASTER file";
                errorOccured = true;
                break;
            }
            if (!MSFileIO::readByteArrayFromFile(file, recordNum * EMASTER_RECORD_SIZE + EMASTER_FILLER5_RECORD_OFFSET, EMASTERFiller5))
            {
                m_lastError = EErrorEMASTERRecordFillerRead;
                m_lastErrorMessage = "Error reading FILLER5 in record from EMASTER file";
                errorOccured = true;
                break;
            }

            if (!MSFileIO::readByteArrayFromFile(file, recordNum * EMASTER_RECORD_SIZE + EMASTER_FILLER6_RECORD_OFFSET, EMASTERFiller6))
            {
                m_lastError = EErrorEMASTERRecordFillerRead;
                m_lastErrorMessage = "Error reading FILLER6 in record from EMASTER file";
                errorOccured = true;
                break;
            }
            if (!MSFileIO::readByteArrayFromFile(file, recordNum * EMASTER_RECORD_SIZE + EMASTER_FILLER7_RECORD_OFFSET, EMASTERFiller7))
            {
                m_lastError = EErrorEMASTERRecordFillerRead;
                m_lastErrorMessage = "Error reading FILLER7 in record from EMASTER file";
                errorOccured = true;
                break;
            }
            if (!MSFileIO::readByteArrayFromFile(file, recordNum * EMASTER_RECORD_SIZE + EMASTER_FILLER8_RECORD_OFFSET, EMASTERFiller8))
            {
                m_lastError = EErrorEMASTERRecordFillerRead;
                m_lastErrorMessage = "Error reading FILLER8 in record from EMASTER file";
                errorOccured = true;
                break;
            }
            if (!MSFileIO::readFloatFromFile(file, recordNum * EMASTER_RECORD_SIZE + EMASTER_INTRADAY_START_TIME_RECORD_OFFSET, intradayStartTime, MSFileIO::EVariableTypeCVSR))
            {
                m_lastError = EErrorEMASTERRecordRead;
                m_lastErrorMessage = "Error reading INTRADAY_START_TIME_RECORD_OFFSET in record from EMASTER file";
                errorOccured = true;
                break;
            }
            if (!MSFileIO::readByteArrayFromFile(file, recordNum * EMASTER_RECORD_SIZE + EMASTER_FILLER9_RECORD_OFFSET, EMASTERFiller9))
            {
                m_lastError = EErrorEMASTERRecordFillerRead;
                m_lastErrorMessage = "Error reading FILLER9 in record from EMASTER file";
                errorOccured = true;
                break;
            }
            if (!MSFileIO::readFloatFromFile(file, recordNum * EMASTER_RECORD_SIZE + EMASTER_INTRADAY_END_TIME_RECORD_OFFSET, intradayEndTime, MSFileIO::EVariableTypeCVSR))
            {
                m_lastError = EErrorEMASTERRecordRead;
                m_lastErrorMessage = "Error reading INTRADAY_END_TIME_RECORD_OFFSET in record from EMASTER file";
                errorOccured = true;
                break;
            }
            if (!MSFileIO::readByteArrayFromFile(file, recordNum * EMASTER_RECORD_SIZE + EMASTER_FILLER10_RECORD_OFFSET, EMASTERFiller10))
            {
                m_lastError = EErrorEMASTERRecordFillerRead;
                m_lastErrorMessage = "Error reading FILLER10 in record from EMASTER file";
                errorOccured = true;
                break;
            }
            if (!MSFileIO::readUIntFromFile(file, recordNum * EMASTER_RECORD_SIZE + EMASTER_LAST_DIV_PAID_RECORD_OFFSET, lastDivPaid, MSFileIO::EVariableTypeCVL))
            {
                m_lastError = EErrorEMASTERRecordRead;
                m_lastErrorMessage = "Error reading LAST_DIV_PAID_RECORD_OFFSET in record from EMASTER file";
                errorOccured = true;
                break;
            }
            if (!MSFileIO::readFloatFromFile(file, recordNum * EMASTER_RECORD_SIZE + EMASTER_LAST_DIV_ADJUSTMENT_RATE_RECORD_OFFSET, lastDivAdjRate, MSFileIO::EVariableTypeCVSR))
            {
                m_lastError = EErrorEMASTERRecordRead;
                m_lastErrorMessage = "Error reading LAST_DIV_ADJUSTMENT_RATE_RECORD_OFFSET in record from EMASTER file";
                errorOccured = true;
                break;
            }
            if (!MSFileIO::readByteArrayFromFile(file, recordNum * EMASTER_RECORD_SIZE + EMASTER_FILLER11_RECORD_OFFSET, EMASTERFiller11))
            {
                m_lastError = EErrorEMASTERRecordFillerRead;
                m_lastErrorMessage = "Error reading FILLER11 in record from EMASTER file";
                errorOccured = true;
                break;
            }

            // Check if read in bitmask matches activeFields bitmask created in MASTER
            if (tempActiveFieldsBitmask != (currentEquityIterator->second)->activeFields().bitMask() )
            {
                m_lastError = EErrorEMASTERHeaderMismatch;
                m_lastErrorMessage = "Master and EMaster contradict. MASTER: active fields bitmask: " +
                        std::to_string(static_cast< int >((currentEquityIterator->second)->activeFields().bitMask())) +
                        ". EMASTER: active fields bitmask: " +
                        std::to_string(static_cast< int >(tempActiveFieldsBitmask));
                errorOccured = true;
                break;
            }

            //copy all the data just read in this function, into the EquityInDB object
            (currentEquityIterator->second)->setEMASTERData(
                        IDCode,
                        EMASTERFiller2,
                        EMASTERFiller3,
                        autoRun,
                        EMASTERFiller4,
                        EMASTERFiller5,
                        EMASTERFiller6,
                        EMASTERFiller7,
                        EMASTERFiller8,
                        intradayStartTime,
                        EMASTERFiller9,
                        intradayEndTime,
                        EMASTERFiller10,
                        lastDivPaid,
                        lastDivAdjRate,
                        EMASTERFiller11);
        }
        break;
    }
    // If the file is open then close it
    if (file.is_open())
        file.close();
    else
    {
        cout << "ERROR: File did not open." << endl << endl;
        errorOccured = true;
    }

    // FOR TESTING
    //    cout << "LAST ERROR:" << m_lastError << ", MESSAGE: " << m_lastErrorMessage <<std::endl;

    // Return whether file was read entirely without a problem
    return !errorOccured;

}


//============================================================================
// XMASTER file

// Read the XMASTER file and put the data in the map member variable m_equities
bool MetaStockDB::readXMasterFile()
{
    bool errorOccured;  // were there any problems that occured in this function

    //temporary holding places for unformatted data or comparison data.
    unsigned char tempInterdayPeriodicity;
    unsigned char tempActiveFieldsBitmask;
    Date tempFirstDateLong;
    Date tempLastDateLong;


    //Active fields object is created from the bitmask, printed, and then discarded.
    ActiveFields* activeFields = 0;

    //These variables hold info that is read from XMASTER, before it is added to the map
    ByteArray XMASTERFiller5(XMASTER_FILLER5_LENGTH);
    string symbol;
    string description;
    ByteArray XMASTERFiller6(XMASTER_FILLER6_LENGTH);
    ByteArray XMASTERFiller7(XMASTER_FILLER7_LENGTH);
    ByteArray XMASTERFiller8(XMASTER_FILLER8_LENGTH);
    EquityInDB::EInterdayPeriodicity interdayPeriodicity;
    ByteArray XMASTERFiller9(XMASTER_FILLER9_LENGTH);
    unsigned long int TDFFileNum;
    ByteArray XMASTERFiller10(XMASTER_FILLER10_LENGTH);
    ByteArray XMASTERFiller11(XMASTER_FILLER11_LENGTH);
    Date firstDate;
    ByteArray XMASTERFiller12(XMASTER_FILLER12_LENGTH);
    Date lastDate;
    ByteArray XMASTERFiller13(XMASTER_FILLER13_LENGTH);
    ByteArray XMASTERFiller14(XMASTER_FILLER14_LENGTH);


    ifstream file(m_DBpath+"XMASTER", ios::in|ios::binary);

    errorOccured = false;  // Assume no error
    while (file.is_open())
    {
        //--------------------------------------------------------------------
        // read the header contained in the first record
        if (!MSFileIO::readByteArrayFromFile(file, XMASTER_FILLER1_FILE_OFFSET, m_XMASTERFiller1))
        {
            errorOccured = true;
            m_lastError = EErrorXMASTERHeaderFillerRead;
            m_lastErrorMessage = "Error reading FILLER1_FILE_OFFSET in header from XMASTER file";
            break;
        }
        if (!MSFileIO::readUIntFromFile(file, XMASTER_NUMRECORDS_FILE_OFFSET, m_XMasterNumRecords, MSFileIO::EVariableTypeUShort ))
        {
            errorOccured = true;
            m_lastError = EErrorXMASTERHeaderRead;
            m_lastErrorMessage = "Error reading NUMRECORDS_FILE_OFFSET in header from XMASTER file";
            break;
        }
        if (!MSFileIO::readByteArrayFromFile(file, XMASTER_FILLER2_FILE_OFFSET, m_XMASTERFiller2))
        {
            errorOccured = true;
            m_lastError = EErrorXMASTERHeaderFillerRead;
            m_lastErrorMessage = "Error reading FILLER2_FILE_OFFSET in header from XMASTER file";
            break;
        }
        if (!MSFileIO::readByteArrayFromFile(file, XMASTER_FILLER3_FILE_OFFSET, m_XMASTERFiller3))
        {
            errorOccured = true;
            m_lastError = EErrorXMASTERHeaderFillerRead;
            m_lastErrorMessage = "Error reading FILLER3_FILE_OFFSET in header from XMASTER file";
            break;
        }
        if (!MSFileIO::readUIntFromFile(file, XMASTER_LARGEST_FDATNUM_FILE_OFFSET, m_XMasterLastDataFileNumber, MSFileIO::EVariableTypeUShort))
        {
            errorOccured = true;
            m_lastError = EErrorXMASTERHeaderRead;
            m_lastErrorMessage = "Error reading LARGEST_FDATNUM_FILE_OFFSET in header from XMASTER file";
            break;
        }
        if (!MSFileIO::readByteArrayFromFile(file, XMASTER_FILLER4_FILE_OFFSET, m_XMASTERFiller4))
        {
            errorOccured = true;
            m_lastError = EErrorXMASTERHeaderFillerRead;
            m_lastErrorMessage = "Error reading FILLER4_FILE_OFFSET in header from XMASTER file";
            break;
        }


        // Loop through remaining records to read equities
        for(unsigned int recordNum = 1; recordNum <= m_XMasterNumRecords; recordNum++)
        {
        // Attempt to read all data from file, or break with failure
            if (!MSFileIO::readByteArrayFromFile(file, recordNum * XMASTER_RECORD_SIZE + XMASTER_FILLER5_RECORD_OFFSET, XMASTERFiller5))
            {
                errorOccured = true;
                m_lastError = EErrorXMASTERRecordFillerRead;
                m_lastErrorMessage = "Error reading FILLER5_RECORD_OFFSET in header from XMASTER file";
                break;
            }
            if (!MSFileIO::readStringFromFile(file, recordNum * XMASTER_RECORD_SIZE + XMASTER_SYMBOL_RECORD_OFFSET, 14, symbol))
            {
                m_lastError = EErrorXMASTERRecordRead;
                m_lastErrorMessage = "Error reading SYMBOL_RECORD_OFFSET in record from XMASTER file";
                errorOccured = true;
                break;
            }
            if (!MSFileIO::readStringFromFile(file, recordNum * XMASTER_RECORD_SIZE + XMASTER_DESCRIPTION_RECORD_OFFSET, 23, description))
            {
                m_lastError = EErrorXMASTERRecordRead;
                m_lastErrorMessage = "Error reading DESCRIPTION_RECORD_OFFSET in record from XMASTER file";
                errorOccured = true;
                break;
            }
            if (!MSFileIO::readByteArrayFromFile(file, recordNum * XMASTER_RECORD_SIZE + XMASTER_FILLER6_RECORD_OFFSET, XMASTERFiller6))
            {
                errorOccured = true;
                m_lastError = EErrorXMASTERRecordFillerRead;
                m_lastErrorMessage = "Error reading FILLER6_RECORD_OFFSET in header from XMASTER file";
                break;
            }
            if (!MSFileIO::readByteArrayFromFile(file, recordNum * XMASTER_RECORD_SIZE + XMASTER_FILLER7_RECORD_OFFSET, XMASTERFiller7))
            {
                errorOccured = true;
                m_lastError = EErrorXMASTERRecordFillerRead;
                m_lastErrorMessage = "Error reading FILLER7_RECORD_OFFSET in header from XMASTER file";
                break;
            }
            if (!MSFileIO::readByteArrayFromFile(file, recordNum * XMASTER_RECORD_SIZE + XMASTER_FILLER8_RECORD_OFFSET, XMASTERFiller8))
            {
                errorOccured = true;
                m_lastError = EErrorXMASTERRecordFillerRead;
                m_lastErrorMessage = "Error reading FILLER8_RECORD_OFFSET in header from XMASTER file";
                break;
            }
            if (!MSFileIO::readUByteFromFile(file, recordNum * XMASTER_RECORD_SIZE + XMASTER_INTERDAY_P_RECORD_OFFSET, tempInterdayPeriodicity))
            {
                m_lastError = EErrorXMASTERRecordRead;
                m_lastErrorMessage = "Error reading INTERDAY_P_RECORD_OFFSET in record from XMASTER file";
                errorOccured = true;
                break;
            }
            if (!MSFileIO::readByteArrayFromFile(file, recordNum * XMASTER_RECORD_SIZE + XMASTER_FILLER9_RECORD_OFFSET, XMASTERFiller9))
            {
                errorOccured = true;
                m_lastError = EErrorXMASTERRecordFillerRead;
                m_lastErrorMessage = "Error reading FILLER9_RECORD_OFFSET in header from XMASTER file";
                break;
            }
            if (!MSFileIO::readUIntFromFile(file, recordNum * XMASTER_RECORD_SIZE + XMASTER_FDAT_FILENUM_RECORD_OFFSET, TDFFileNum, MSFileIO::EVariableTypeUShort))
            {
                m_lastError = EErrorXMASTERRecordRead;
                m_lastErrorMessage = "Error reading FDAT_FILENUM_RECORD_OFFSET in record from XMASTER file";
                errorOccured = true;
                break;
            }
            if (!MSFileIO::readByteArrayFromFile(file, recordNum * XMASTER_RECORD_SIZE + XMASTER_FILLER10_RECORD_OFFSET, XMASTERFiller10))
            {
                errorOccured = true;
                m_lastError = EErrorXMASTERRecordFillerRead;
                m_lastErrorMessage = "Error reading FILLER10_RECORD_OFFSET in header from XMASTER file";
                break;
            }
            if (!MSFileIO::readUByteFromFile(file, recordNum * XMASTER_RECORD_SIZE + XMASTER_BITMASK_ACTIVE_FIELDS_RECORD_OFFSET, tempActiveFieldsBitmask))
            {
                m_lastError = EErrorXMASTERRecordRead;
                m_lastErrorMessage = "Error reading ACTIVE_BYTEFIELDS_RECORD_OFFSET in record from XMASTER file";
                errorOccured = true;
                break;
            }
            if (!MSFileIO::readByteArrayFromFile(file, recordNum * XMASTER_RECORD_SIZE + XMASTER_FILLER11_RECORD_OFFSET, XMASTERFiller11))
            {
                errorOccured = true;
                m_lastError = EErrorXMASTERRecordFillerRead;
                m_lastErrorMessage = "Error reading FILLER11_RECORD_OFFSET in header from XMASTER file";
                break;
            }
            if (!MSFileIO::readDateFromFile(file, recordNum * XMASTER_RECORD_SIZE + XMASTER_FIRST_DATE_LONG_RECORD_OFFSET, tempFirstDateLong, MSFileIO::EVariableTypeCVL))
            {
                m_lastError = EErrorXMASTERRecordRead;
                m_lastErrorMessage = "Error reading FIRST_DATE_LONG_RECORD_OFFSET in record from XMASTER file";
                errorOccured = true;
                break;
            }
            if (!MSFileIO::readDateFromFile(file, recordNum * XMASTER_RECORD_SIZE + XMASTER_FIRST_DATE_RECORD_OFFSET, firstDate, MSFileIO::EVariableTypeCVSR))
            {
                m_lastError = EErrorXMASTERRecordRead;
                m_lastErrorMessage = "Error reading FIRST_DATE_RECORD_OFFSET in record from XMASTER file";
                errorOccured = true;
                break;
            }
            if (!MSFileIO::readByteArrayFromFile(file, recordNum * XMASTER_RECORD_SIZE + XMASTER_FILLER12_RECORD_OFFSET, XMASTERFiller12))
            {
                errorOccured = true;
                m_lastError = EErrorXMASTERRecordFillerRead;
                m_lastErrorMessage = "Error reading FILLER12_RECORD_OFFSET in header from XMASTER file";
                break;
            }
            if (!MSFileIO::readDateFromFile(file, recordNum * XMASTER_RECORD_SIZE + XMASTER_LAST_DATE_LONG_RECORD_OFFSET, tempLastDateLong, MSFileIO::EVariableTypeCVL))
            {
                m_lastError = EErrorXMASTERRecordRead;
                m_lastErrorMessage = "Error reading LAST_DATE_LONG_RECORD_OFFSET in record from XMASTER file";
                errorOccured = true;
                break;
            }
            if (!MSFileIO::readDateFromFile(file, recordNum * XMASTER_RECORD_SIZE + XMASTER_LAST_DATE_RECORD_OFFSET, lastDate, MSFileIO::EVariableTypeCVSR))
            {
                m_lastError = EErrorXMASTERRecordRead;
                m_lastErrorMessage = "Error reading LAST_DATE_RECORD_OFFSET in record from XMASTER file";
                errorOccured = true;
                break;
            }
            if (!MSFileIO::readByteArrayFromFile(file, recordNum * XMASTER_RECORD_SIZE + XMASTER_FILLER13_RECORD_OFFSET, XMASTERFiller13))
            {
                errorOccured = true;
                m_lastError = EErrorXMASTERRecordFillerRead;
                m_lastErrorMessage = "Error reading FILLER13_RECORD_OFFSET in header from XMASTER file";
                break;
            }
            if (!MSFileIO::readByteArrayFromFile(file, recordNum * XMASTER_RECORD_SIZE + XMASTER_FILLER14_RECORD_OFFSET, XMASTERFiller14))
            {
                errorOccured = true;
                m_lastError = EErrorXMASTERRecordFillerRead;
                m_lastErrorMessage = "Error reading FILLER14_RECORD_OFFSET in header from XMASTER file";
                break;
            }
            if (! ActiveFields::validBitMask(tempActiveFieldsBitmask))
            {
                errorOccured = true;
                m_lastError = EErrorXMASTERBitMaskValue;
                m_lastErrorMessage = "Error interpreting active fields bitmask in header from XMASTER file";
                break;
            }

            // Trim trailing spaces from string fields
            MSFileIO::trim(description);
            MSFileIO::trim(symbol);

            // Create an activeFields object, based on the bitmask read for this equity in XMASTER
            activeFields = new ActiveFields(tempActiveFieldsBitmask);
            activeFields->print();


            // Convert interday periodicity(unsigned char) to valid enum or fail
            if  (  (tempInterdayPeriodicity == 'N') || (tempInterdayPeriodicity == 'D') || (tempInterdayPeriodicity == 'W') ||
                   (tempInterdayPeriodicity == 'M') || (tempInterdayPeriodicity == 'Q') || (tempInterdayPeriodicity == 'Y')  )
                interdayPeriodicity = static_cast<EquityInDB::EInterdayPeriodicity>(tempInterdayPeriodicity);
            else
            {
                m_lastError = EErrorXMASTERRecordValue;
                m_lastErrorMessage = "Error interpreding InterdayPeriodicity in record from XMASTER file";
                errorOccured = true;
                break;
            }

            // Add the struct to our map container
            m_equityMap.insert(std::pair<string, EquityInDB*>
                               (symbol,
                                new EquityInDB(
                                    XMASTERFiller5,
                                    symbol,
                                    description,
                                    XMASTERFiller6,
                                    XMASTERFiller7,
                                    XMASTERFiller8,
                                    interdayPeriodicity,
                                    XMASTERFiller9,
                                    TDFFileNum,
                                    XMASTERFiller10,
                                    tempActiveFieldsBitmask,
                                    XMASTERFiller11,
                                    firstDate,
                                    XMASTERFiller12,
                                    lastDate,
                                    XMASTERFiller13,
                                    XMASTERFiller14)));
        }
        break;
    }
    // If the file is open then close it
    if (file.is_open())
        file.close();
    else
    {
        m_lastError = EErrorXMASTERFileOpenFailed;
        m_lastErrorMessage = "Failed to open XMASTER file '" + m_DBpath + "'";
        errorOccured = true;
    }

    // Return whether file was read entirely without a problem
    return !errorOccured;

}




//============================================================================
// FDAT/MWD files

// Read the trading data from FDAT/MWD files for each equity held in the map
bool MetaStockDB::populateTradingData()
{
    bool errorOccured = false; // were there any problems that occured in this function
    map<string, EquityInDB*>::iterator equityIterator; // an iterator that stores the position of the equity currently in use.
    ifstream file;
    string fileName;

    Date date;
    float time;
    float open;
    float close;
    float high;
    float low;
    float tempVolume;
    unsigned long volume;
    float openInterest;
    ActiveFields activeFields;
    unsigned short recordSize;
    unsigned long numRecords;

    // Loop through equities help in map
    for(equityIterator = m_equityMap.begin(); equityIterator != m_equityMap.end(); equityIterator++)

        // Block should go into function, param equitymetadata
    {

        // Construct data file filename
        if ((equityIterator->second)->dataFileType() == EquityInDB::EDataFileTypes::EDataFileTypeFDAT) {
            fileName = "F" + to_string((equityIterator->second)->TDFFileNum()) + ".DAT";
        } else {
            fileName = "C" + to_string((equityIterator->second)->TDFFileNum()) + ".MWD";
        }

        // Check if file exists
        if (! MSFileIO::fileInDBPathExists(m_DBpath,fileName)) {

            m_lastError = EErrorTradingDataFileDoesntExist;
            m_lastErrorMessage = "Error: file " + fileName + " does not exist";
            errorOccured = true;
            break;
        }

        // Open the file
        file.open(m_DBpath+fileName, ios::in|ios::binary);
        if (!file.is_open())
        {
            m_lastError = EErrorTradingDataFileOpenFailed;
            m_lastErrorMessage = "Error: file " + fileName + " did not open";
            errorOccured = true;
            break;
        }

        //Retrieve info from the map, about the records in this TDF file
        activeFields = (equityIterator->second)->activeFields();
        recordSize = activeFields.recordSize();

        //Read the number of records in this file, from the first record
        if (!MSFileIO::readUIntFromFile(file, TRADINGDATAFILE_NUM_RECORDS_OFFSET, numRecords, MSFileIO::EVariableTypeUShort))
        {
            m_lastError = EErrorTradingDataFileFieldRead;
            m_lastErrorMessage = "Error reading number of records from start of file " + fileName;
            errorOccured = true;
            break;
        }


        // This while loop exists so that this block of code can be exited easily with a break statement.
        // If there is an error reading a field from the file, then exit this block.
        while (file.is_open())
        {
            //If the file opened properly
            for (unsigned short recordNum = 1; recordNum < numRecords; recordNum++)
            {

                // Reset all fields to default values, so unread values save as 0 in the trading day
                date = Date();
                time = 0;
                open = 0;
                close = 0;
                high = 0;
                low = 0;
                tempVolume = 0;
                volume = 0;
                openInterest = 0;

                // Read the time from the data file, only if active
                if ( (activeFields.dateActive()) &&
                     (!MSFileIO::readDateFromFile(file, recordNum * recordSize + activeFields.dateOffset(), date, MSFileIO::EVariableTypeMBF32)) ) {
                    m_lastError = EErrorTradingDataFileFieldRead;
                    m_lastErrorMessage = "Error reading date in record from the file " + fileName;
                    errorOccured = true;
                    break;
                }

                // Read the time from the data file, only if active
                if ( (activeFields.timeActive()) &&
                     (!MSFileIO::readFloatFromFile(file, recordNum * recordSize + activeFields.timeOffset(), time, MSFileIO::EVariableTypeMBF32)) ) {
                    m_lastError = EErrorTradingDataFileFieldRead;
                    m_lastErrorMessage = "Error reading time in record from the file " + fileName;
                    errorOccured = true;
                    break;
                }

                // Read the open value from the data file, only if active
                if ( (activeFields.openActive()) &&
                     (!MSFileIO::readFloatFromFile(file, recordNum * recordSize + activeFields.openOffset(), open, MSFileIO::EVariableTypeMBF32)) ) {
                    m_lastError = EErrorTradingDataFileFieldRead;
                    m_lastErrorMessage = "Error reading open in record from the file " + fileName;
                    errorOccured = true;
                    break;
                }

                // Read the close value from the data file, only if active
                if ( (activeFields.closeActive()) &&
                     (!MSFileIO::readFloatFromFile(file, recordNum * recordSize + activeFields.closeOffset(), close, MSFileIO::EVariableTypeMBF32)) ) {
                    m_lastError = EErrorTradingDataFileFieldRead;
                    m_lastErrorMessage = "Error reading close in record from the file " + fileName;
                    errorOccured = true;
                    break;
                }

                // Read the close value from the data file, only if active
                if ( (activeFields.highActive()) &&
                     (!MSFileIO::readFloatFromFile(file, recordNum * recordSize + activeFields.highOffset(), high, MSFileIO::EVariableTypeMBF32)) ) {
                    m_lastError = EErrorTradingDataFileFieldRead;
                    m_lastErrorMessage = "Error reading high in record from the file " + fileName;
                    errorOccured = true;
                    break;
                }

                // Read the low value from the data file, only if active
                if ( (activeFields.lowActive()) &&
                     (!MSFileIO::readFloatFromFile(file, recordNum * recordSize + activeFields.lowOffset(), low, MSFileIO::EVariableTypeMBF32)) ) {
                    m_lastError = EErrorTradingDataFileFieldRead;
                    m_lastErrorMessage = "Error reading low in record from the file " + fileName;
                    errorOccured = true;
                    break;
                }

                // Read the close value from the data file, only if active
                if ( (activeFields.volumeActive()) &&
                     (!MSFileIO::readFloatFromFile(file, recordNum * recordSize + activeFields.volumeOffset(), tempVolume, MSFileIO::EVariableTypeMBF32)) ) {
                    m_lastError = EErrorTradingDataFileFieldRead;
                    m_lastErrorMessage = "Error reading volume in record from the file " + fileName;
                    errorOccured = true;
                    break;
                } else {
                    volume = static_cast<unsigned long int>(tempVolume);
                }

                if (! (equityIterator->second)->tradingHistory()->addTradingDayData(TradingDay(date, time, open, close, high, low, volume, openInterest))) {
                    m_lastError = EErrorTradingDataFileDuplicateDate;
                    m_lastErrorMessage = "Error found in duplicate trading day data date ("+date.asString(Date::EDateFormatYYYYMMMDD)+")";
                    errorOccured = true;
                    break;
                }

                // Set data loaded flag to true
                (equityIterator->second)->tradingHistory()->loaded(true);
            }
            file.close();
            break;
        }
    }

    // Return whether file was read entirely without a problem
    return !errorOccured;

}




// Return a pointer to the Equity object with the name specified.  Returns NULL if not found
Equity * MetaStockDB::find(std::string equityName) const
{
    map<string, EquityInDB*>::const_iterator targetIt;

    // Calls the find function from std::map
    targetIt = m_equityMap.find(equityName);
    //if the equityInDB is not found, return null
    if (targetIt == m_equityMap.end())
        return NULL;
    //otherwise, upcast the equityInDB into an equity, and return it
    else
        return static_cast<Equity*>(targetIt->second);
}




// Print the entire metastock database
void MetaStockDB::print()
{
    map<string, EquityInDB*>::iterator currentEquityIterator; // an iterator that stores the position of the equity currently in use.

    std::cout << "Database" << endl;
    std::cout << "--------" << endl;
    std::cout << "Database........................: " << m_DBpath << endl;
    std::cout << "Database is new.................: " << (m_isnew?"Yes":"No") << endl;
    std::cout << "Number of MASTER/EMASTER records: " << m_MasterNumRecords << endl;
    std::cout << "Number of XMASTER records.......: " << m_XMasterNumRecords << endl;
    std::cout << "Last XMASTER data file number...: " << m_XMasterNumRecords + 1 << endl;

    // Loop through equities help in map
    for(currentEquityIterator = m_equityMap.begin(); currentEquityIterator != m_equityMap.end(); currentEquityIterator++)
        (currentEquityIterator->second)->print();

}
