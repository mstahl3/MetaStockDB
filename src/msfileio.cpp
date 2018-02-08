/*
 * Class: MSFileIO
 * Author: Marc Stahl
 * Description: This class offers helper functions. Some of the functions read
 *   data from a file, and others convert a variable from one type to another.
 * History:
 *   MKS    2018-Jan-19   Original coding
 */

#include <sys/types.h>
#include <sys/stat.h>
#include <string>
#include <string.h>

#include "bytearray.h"
#include "msfileio.h"


// Read n bytes from the specified file and convert it to an unsigned int.
// n is 2 or 4 depending on the integerType parameter.
// Return true/false to indicate if successfull.
bool MSFileIO::readUIntFromFile(ifstream &file, const unsigned int offset, unsigned long int &resultUInt, const EVariablesTypes integerType)
{
    resultUInt = 0;  //  Initialize the unsigned integer to 0 as default

    // If not a valid integer type, then fail
    if ((integerType != EVariableTypeCVL) && (integerType != EVariableTypeUShort)) return false;

    if (!file.seekg(offset))
    {
        cout << "could not set file pointer in stream";
        return false; // Set the file pointer in the stream, and return false if could not
    }

    // Determine how many bytes to read from file based on integer type
    unsigned char numBytesToRead;
    if (integerType == EVariableTypeCVL) numBytesToRead = 4;
    else if (integerType == EVariableTypeUShort) numBytesToRead = 2;
    else numBytesToRead = 1;

    unsigned char* buffer = new unsigned char[numBytesToRead]; // Buffer to hold read bytes
    file.read((char*) buffer, numBytesToRead); // Read 2 bytes from the file
    if (file.gcount() != numBytesToRead)
    {
        cout << "did not read expected number of bytes (expected " << numBytesToRead << " got " << file.gcount() << ")";
        return false; // If did not read expected number of bytes, must be error.
    }

    // Convert array of bytes back into integer
    if (integerType == EVariableTypeCVL) {
        resultUInt = buffer[3];
        resultUInt = (resultUInt << 8) + buffer[2];
        resultUInt = (resultUInt << 8) + buffer[1];
        resultUInt = (resultUInt << 8) + buffer[0];
    } else {
        resultUInt = buffer[1];  // Move byte field contents from char array to an unsigned int
        resultUInt = (resultUInt << 8) + buffer[0];
    }

    delete[] buffer;

    return true;
}


// Read 1 byte from the specified file and convert it to an unsigned char.
// Return true/false to indicate if successfull.
bool MSFileIO::readUByteFromFile(ifstream &file, const unsigned int offset, unsigned char &resultUChar)
{
    resultUChar = 0;  // Initialize the unsigned integer to 0 as default
    if (!file.seekg(offset)) return false;  // Set the file pointer in the stream, and return false if could not

    file.read((char*) &resultUChar, 1);  // Read 1 byte from the file
    if (file.gcount() != 1) return false;  // If did not read expected number of bytes, must be error.

    return true;
}


// Read specified number of bytes from the specified file and place in an array of unsigned char
// Return true/false to indicate if successfull.
bool MSFileIO::readByteArrayFromFile(ifstream &file, const unsigned int offset, ByteArray &resultByteArray)
{
    if (!file.seekg(offset)) return false;  // Set the file pointer in the stream, and return false if could not
    unsigned char* buffer = new unsigned char[resultByteArray.size()];  // Buffer to hold read byte
    file.read((char*) buffer, resultByteArray.size());  // Read 1 byte from the file
    if (file.gcount() != resultByteArray.size()) return false;  // If did not read expected number of bytes, must be error.

    resultByteArray.setContents(buffer);
    delete[] buffer;
    return true;
}


// Read 'byteFieldSize' bytes from the specified file and convert it to a
// string. Note that all strings stored in Master, EMaster
// , and XMaster are null padded, with null's stretching from the end of the string to the end of the byte field.
// Return true/false to indicate if successfull.
bool MSFileIO::readStringFromFile(ifstream &file, const unsigned int offset, const int byteFieldSize, string &resultString)
{
    resultString = "";  // Initialize the string to empty
    if (!file.seekg(offset)) return false;  // Set the file pointer in the stream, and return false if could not

    char* buffer = new char[byteFieldSize + 1];  // Buffer to hold read bytes. Is 1 longer than byte field, incase
    // the string takes up the entire byte field, and there is no room
    // for a null terminator.
    file.read(buffer, byteFieldSize);  // Read 'byteFieldSize' bytes from the file
    if (file.gcount() != byteFieldSize) return false;  // If did not read expected number of bytes, must be error.
    buffer[byteFieldSize] = '\0';  // incase string length = byte field length.

    resultString = string(buffer);  // convert buffer to std::string.
    delete[] buffer;
    return true;
}



// Converts from a MBF32 floating point number to a floating point number
bool MSFileIO::MBF32ToFloat(const unsigned char inputBytes[4], float &resultFloat)
{
    unsigned char *msbin = (unsigned char *) inputBytes;
    unsigned char *ieee = (unsigned char *) &resultFloat;

    /* any msbin w/ exponent of zero = zero */
    if (msbin[3] == 0) {
        resultFloat = 0.0f;
        return true;
    }

    // Catch underflow
    //    if (msbin[3] <= 2) return false;

    // Reset the resultant float to all 0's
    for (int i=0; i<4; i++) ieee[i] = 0;

    // Initialize sign and exponent
    unsigned char sign = 0x00;
    unsigned char ieee_exp = 0x00;

    // Transfer the sign, exponent, and mantissa to new float
    ieee[3] |= sign;
    ieee_exp = msbin[3] - 2;    /* actually, msbin[3]-1-128+127 */
    ieee[3] |= ieee_exp >> 1;
    ieee[2] |= ieee_exp << 7;
    ieee[2] |= msbin[2] & 0x7f;
    ieee[1] = msbin[1];
    ieee[0] = msbin[0];

    return true;
}



// Converts from a CVS floating point number to a floating point number
// Note that CVS already in ieee single floating point format
bool MSFileIO::CVSToFloat(unsigned char inputBytes[4], float &resultFloat, const bool reversed)
{
    // If bytes are in reverse order, correct the order
    if (reversed) {
        // CSV Bytes are in reverse order expected by IEEE 754
        unsigned char tempByte = inputBytes[0];
        inputBytes[0] = inputBytes[3];
        inputBytes[3] = tempByte;
        tempByte = inputBytes[1];
        inputBytes[1] = inputBytes[2];
        inputBytes[2] = tempByte;
    }

    // Copy the 4 bytes over unchanged
    resultFloat = static_cast<float>(*(float *) inputBytes);

    // Always success
    return true;
}



// Read 4 bytes from the specified file as 32bit MBF and convert it to a float
// Return true/false to indicate if successfull.
bool MSFileIO::readFloatFromFile(ifstream &file, const unsigned int offset, float &resultFloat, const EVariablesTypes floatType)
{
    bool returnValue;

    resultFloat = 0;  // Initialize to an invalid date

    // If not a valid float type, then fail
    if ((floatType != EVariableTypeCVS) && (floatType != EVariableTypeCVSR) && (floatType != EVariableTypeMBF32)) return false;

    if (!file.seekg(offset)) return false;  // Set the file pointer in the stream, and return false if could not
    unsigned char* buffer = new unsigned char[4];  // Buffer to hold read bytes
    file.read((char *) buffer, 4);  // Read 4 bytes from the file

    if (file.gcount() != 4) return false;  // If did not read expected number of bytes, must be error.

    //Convert the read field to the type indicated in the floatType parameter
    switch (floatType) {
    case EVariableTypeMBF32:
        returnValue = MSFileIO::MBF32ToFloat(buffer, resultFloat);
        delete[] buffer;
        return returnValue;
    case EVariableTypeCVS:
    case EVariableTypeCVSR:
        returnValue = MSFileIO::CVSToFloat(buffer, resultFloat, (floatType == EVariableTypeCVSR) );
        delete[] buffer;
        return returnValue;
    default:
        delete[] buffer;
        return false;  // Unknown float type
    }
}


//remove all spaces from the front and back of a string
void MSFileIO::trim(string &str)
{
    size_t first = str.find_first_not_of(' ');
    if (string::npos == first) return;
    size_t last = str.find_last_not_of(' ');
    str = str.substr(first, (last - first + 1));
}



// Extracts date information from a Microsoft floating point number, and assigns this info
// to the date 'resultDate'.
bool MSFileIO::floatToDate(const float dateAsFloat, Date &resultDate)
{
    // Initialize return to an invalid date
    resultDate = Date();

    string str = std::to_string(static_cast<unsigned long>(dateAsFloat));

    if ((str.length() == 6) || (str.length() == 7))
    {
        unsigned int offset = ( ((str.at(0) == '1') && (str.length() > 6)) ? 1 : 0);
        unsigned int temp = std::stoi(str.substr(offset, 2));
        unsigned int year = temp + ( (temp >= 80) ? 1900 : 2000);
        unsigned int month = std::stoi(str.substr(2 + offset, 2));
        unsigned int day = std::stoi(str.substr(4 + offset, 2));
        resultDate = Date(year, month, day);
        return true;
    }
    else return false;
}



// Read 4 bytes from the specified file and convert it to a date.
// Return true/false to indicate if successfull.
bool MSFileIO::readDateFromFile(ifstream &file, const unsigned int offset, Date &resultDate, const EVariablesTypes varType)
{
    resultDate = Date();  // Initialize an invalid date
    float tempFloat;  // Temporary holder for the read in float
    unsigned long int tempInt;

    //Call the read function for the type indicated in the varType parameter
    switch(varType)
    {
    case EVariableTypeMBF32 :
    case EVariableTypeCVS :
    case EVariableTypeCVSR :
        if (!MSFileIO::readFloatFromFile(file, offset, tempFloat, varType)) return false;
        break;
    case EVariableTypeCVL :
        if (!MSFileIO::readUIntFromFile(file, offset, tempInt, varType)) return false;
        tempFloat = tempInt;
        break;
    default :
        return false;
        break;
    }

    // Create a date object from the information in the byte field.
    return MSFileIO::floatToDate(tempFloat, resultDate);
}




// Tests if a path exists
bool MSFileIO::DBPathExists(const string pathname) {

    bool existsAndAccessible = true;

    struct stat info;

    if( stat( pathname.c_str(), &info ) != 0 ) existsAndAccessible = false;   // Can't access it
    else if ( info.st_mode & S_IFDIR )  existsAndAccessible = true;
    else existsAndAccessible = false;  // Not a directory
    return existsAndAccessible;
}


// Tests if a file exists in the path
bool MSFileIO::fileInDBPathExists(const string pathName, const string fileName) {
    struct stat buffer;
    return (stat ((pathName+fileName).c_str(), &buffer) == 0);
}


// Create a DB path (including all subdirectories if required)
// Returns true if succesfull
bool MSFileIO::makeDBPath(const string pathName)
{
    char tmp[1024];  // The full path to be created
    char *p = NULL;  // Pointer to a character it tmp. Iterates through all characters in tmp
    size_t len; // Length of tmp

    // Make a copy of the pathName
    snprintf(tmp, sizeof(tmp),"%s",pathName.c_str());
    len = strlen(tmp);

    // If last char is seperator, cut it off
    if(tmp[len - 1] == PATHSEPERATOR) tmp[len - 1] = 0;

    // Loop through path name
    for(p = tmp + 1; *p; p++)

        // If found a seperator (subdir), make that dir
        if(*p == PATHSEPERATOR) {
            *p = 0; // Replace seperator with EOS to make subpath
            if (mkdir(tmp, 0x666) != 0) return false;
            *p = PATHSEPERATOR; // Restore seperator
        }

    // Create final and full path
    #ifdef _WIN32
    return (_mkdir(tmp) == 0);
    #else
    return (mkdir(tmp, 0x666) == 0);
    #endif
}

