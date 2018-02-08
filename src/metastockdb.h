/*
 * Class: MetaStockDB
 * Author: Marc Stahl
 * Description: This class is responsible for reading data from files and populating the data structures in this program
 * History:
 *   MKS    2018-Jan-19   Original coding
 */

#ifndef METASTOCKDB_H
#define METASTOCKDB_H

#include <map>
#include <string>
#include <stdlib.h>
#include "msfileio.h"
#include "tradinghistory.h"
#include "equityindb.h"
#include "equity.h"

class MetaStockDB
{
public:

    // Errors which can occur
    enum EErrors {
        EErrorNone,  // OK - no errors
        EErrorMASTERFileOpenFailed,  // Failed to open file
        EErrorMASTERHeaderRead,  // Failed to read one of the header values in MASTER file
        EErrorMASTERRecordRead,  // Failed to read one of the record in MASTER file
        EErrorMASTERHeaderFillerRead,  // Failed to read filler data in MASTER header
        EErrorMASTERRecordFillerRead,  // Failed to read filler data in MASTER record
        EErrorMASTERRecordValue,  // Failed to intepret value of record in MASTER file

        EErrorEMASTERFileOpenFailed,  // Failed to open file
        EErrorEMASTERHeaderRead,  // Failed to read one of the header values in EMASTER file
        EErrorEMASTERRecordRead,  // Failed to read one of the record in EMASTER file
        EErrorEMASTERHeaderFillerRead,  // Failed to read filler data in EMASTER header
        EErrorEMASTERRecordFillerRead,  // Failed to read filler data in EMASTER record
        EErrorEMASTERRecordValue,  // Failed to intepret value of record in EMASTER file

        EErrorXMASTERFileOpenFailed,  // Failed to open file
        EErrorXMASTERHeaderRead,  // Failed to read one of the header values in XMASTER file
        EErrorXMASTERRecordRead,  // Failed to read one of the record in XMASTER file
        EErrorXMASTERHeaderFillerRead,  // Failed to read filler data in XMASTER header
        EErrorXMASTERRecordFillerRead,  // Failed to read filler data in XMASTER record
        EErrorXMASTERRecordValue,  // Failed to intepret value of record in XMASTER file
        EErrorXMASTERBitMaskValue,  // Invalid bitmask value of record in XMASTER file

        EErrorDBpathCreateFailed,             // Failed to create DB path
        EErrorEMASTERHeaderMismatch,          // Mismatch between EMASTER and MASTER header

        EErrorTradingDataFileOpenFailed,      // Failed to open trading day file
        EErrorTradingDataFileDoesntExist,     // Trading day history file does not exist
        EErrorTradingDataFileFieldRead,       // Failed to read from the trading data history file
        EErrorTradingDataFileDuplicateDate    // Attempt to add a duplicate date to trading data history in memory list
    };

    // Constructor: Builds a MetaStockDB object using MASTER, EMASTER, and XMASTER files at the path dbpath.
    // It also stores dbpath in a member variable because this path will be needed later to retrieve Trading Day Files(TDFs)
    MetaStockDB(const string dbpath, const bool lazyLoad, const unsigned char numBackups);
    ~MetaStockDB();

    // Return the last error code
    EErrors lastError() const;

    // Return the last error message
    std::string lastErrorMessage() const;

    // Reset at start of list, and copy first item in the list
    // into the parameter.  Return true if success, false otherwise
    bool getFirstEquity(Equity** equityPtr);

    // Advance the iterator, and if there is another element
    // then copy element into the parameter.
    // Return true if success, false otherwise
    bool getNextEquityPtr(Equity** equityPtr);

    // Return a pointer to the Equity object with the name specified.  Returns NULL if not found
    Equity * find(std::string equityName) const;

    // Print the entire metastock database
    void print();

private:

    // Last error which occured
    EErrors m_lastError;

    // Description of last error
    string m_lastErrorMessage;

    // Path to the database (including trailing slash)
    string m_DBpath;

    // Should FDAT files be lazy loaded
    bool m_lazyLoad;

    // Is this a new database
    bool m_isnew;

    // Number of backups to keep
    unsigned char m_numBackups;

    // Is there an error in the structure / accessof the DB
    bool m_DBerror;

    // Number of records in the Master file (0-255)
    unsigned long int m_MasterNumRecords;

    // Last F#.DAT number used (0-255) according to MASTER file
    unsigned long int m_MasterLastDataFileNumber;

    // Number of records in the XMaster file (256-65535)
    unsigned long int m_XMasterNumRecords;

    // Last F#.DAT number used (256-65535) according to XMASTER file
    unsigned long int m_XMasterLastDataFileNumber;

    ByteArray m_MASTERFiller1;   // Filler 1 in MASTER file header
    ByteArray m_EMASTERFiller1;  // Filler 1 in EMASTER file header
    ByteArray m_XMASTERFiller1;  // Filler 1 in XMASTER file header
    ByteArray m_XMASTERFiller2;  // Filler 2 in XMASTER file header
    ByteArray m_XMASTERFiller3;  // Filler 3 in XMASTER file header
    ByteArray m_XMASTERFiller4;  // Filler 4 in XMASTER file header

    // Container to hold all equities found in master files
    map<string, EquityInDB*> m_equityMap;

    // If the equity iterator has reached the end of the equity map, then false.
    // Otherwise true.
    bool m_equityItValid;

    // stores position of the current equity from m_equityMap
    map<string, EquityInDB*>::iterator m_equityIt;

    // Read the MASTER file
    bool readMasterFile();

    // Read the EMASTER file and put the data in the map member variable m_equities
    bool readEMasterFile();

    // Read the XMASTER file and put the data in the map member variable m_equities
    bool readXMasterFile();

    // Read the Fx.DAT files
    bool populateTradingData();




};

#endif // METASTOCKDB_H
