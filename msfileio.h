/*
 * Class: MSFileIO
 * Author: Marc Stahl
 * Description: This class has helper functions. Some of the functions read data from a file, and others convert
 *     a variable from one type to another.
 * History:
 *   MKS    2018-Jan-19   Original coding
 */

#ifndef MSFILEIO_H
#define MSFILEIO_H

#include <iostream>
#include <fstream>

#include "bytearray.h"
#include "date.h"

#ifdef _WIN32
define PATHSEPERATOR '\\'
#else
#define PATHSEPERATOR '/'
#endif

using namespace std;

class MSFileIO
{
public:


    // Type of integers / floats which can be read from file
    enum EVariablesTypes {
        EVariableTypeCVS,     // 4 byte IEEE single float
        EVariableTypeCVSR,    // 4 byte IEEE single float, reversed byte order
        EVariableTypeMBF32,   // 4 byte MS Basic 32-bit float
        EVariableTypeCVL,     // 4 byte unsigned IEEE (long) integer
        EVariableTypeUShort,  // 2 byte unsigned IEEE (short) integer
        EVariableTypeUByte    // 1 byte unsigned integer
    };

    //Default Constructor
    MSFileIO();

    // Read bytes from the specified file and convert it to an unsigned int.
    // Return true/false to indicate if successfull.
    static bool readUIntFromFile(ifstream &file, const unsigned int offset, unsigned long &resultUInt, const EVariablesTypes integerType);

    // Read 1 byte from the specified file and convert it to an unsigned char.
    // Return true/false to indicate if successfull.
    static bool readUByteFromFile(ifstream &file, const unsigned int offset, unsigned char &resultUChar);

    // Read specified number of bytes from the specified file and place in an array of unsigned char
    // Return true/false to indicate if successfull.
    static bool readByteArrayFromFile(ifstream &file, const unsigned int offset, ByteArray &resultByteArray);

    // Read 4 bytes from the specified file and convert it to a date.
    // Return true/false to indicate if successfull.
    static bool readDateFromFile(ifstream &file, const unsigned int offset, Date &resultDate, const EVariablesTypes varType);

    // Read 'byteFieldSize' bytes from the specified file and convert it to a string. Note that all strings stored in Master, EMaster
    // , and XMaster are null padded, with null's stretching from the end of the string to the end of the byte field.
    // Return true/false to indicate if successfull.
    static bool readStringFromFile(ifstream &file, const unsigned int offset, const int byteFieldSize, string &resultString);

    // Read 4 bytes from the specified file as 32bit MBF and convert it to a float
    // Return true/false to indicate if successfull.
    static bool readFloatFromFile(ifstream &file, const unsigned int offset, float &resultFloat, const EVariablesTypes floatType);

    // Converts from a CVS floating point number to a floating point number
    // Note that CVS already in ieee single floating point format
    static bool CVSToFloat(unsigned char inputBytes[], float &resultFloat, const bool reversed);

    // Converts from a MBF32 floating point number to a floating point number
    static bool MBF32ToFloat(const unsigned char inputBytes[4], float &resultFloat);

    // Tests if a path exists
    static bool DBPathExists(const string pathname);

    // Create a DB path (including all subdirectories)
    static bool makeDBPath(const string pathName);

    // Tests if a file exists in the path
    static bool fileInDBPathExists(const string pathName, const string fileName);

    // Trim white space from both ends of string
    static void trim(string &str);



private:
    // Extracts date information from a Microsoft floating point number, and assigns this info
    // to the date 'resultDate'.
    static bool floatToDate(const float dateAsFloat, Date &resultDate);

    // Create the directory .
    // Return true for success, false for error
//    static bool makeDir(const char *path, mode_t mode);

};

#endif // MSFILEIO_H
