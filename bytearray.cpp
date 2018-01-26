/*
 * Class: ByteArray
 * Author: Marc Stahl
 * Description: An array of bytes (unsigned chars)
 * History:
 *   MKS    2018-Jan-19   Original coding
 */

#include <cstring>
#include <stdlib.h>
#include "bytearray.h"
#include <iostream>

using namespace std;

// Constructor: Create a ByteArray and fill it with the contents that are passed in.
ByteArray::ByteArray(const unsigned char *contents, const unsigned int size) :
    m_contents( (unsigned char *) malloc(size)),
    m_size(size)
{
    memcpy(m_contents, contents, m_size);
}


// Constructor: Create a ByteArray and fill it with null bytes (0000 0000).
ByteArray::ByteArray(const unsigned int size) :
    m_contents( (unsigned char *) malloc(size)),
    m_size(size)
{
    memset(m_contents, 0, m_size);
}


// Constructor: Copy the contents and size of the passed ByteArray, into this ByteArray
ByteArray::ByteArray(const ByteArray &originalByteArray) :
    m_contents( (unsigned char *) malloc(originalByteArray.m_size)),
    m_size(originalByteArray.m_size)
{
    originalByteArray.getContents(m_contents);
}


// Destructer: Free the allocated memory of the contents array.
ByteArray::~ByteArray()
{
    free(m_contents);
}


// Copy the contents from one ByteArray object to another
void ByteArray::operator=(const ByteArray RHS)
{
    if (m_size != RHS.m_size)
    {
        m_size = RHS.m_size;
        free(m_contents);
        m_contents = (unsigned char *) malloc(m_size);
    }

    setContents(RHS.m_contents);
}


// Get byte array size
unsigned int ByteArray::size() const
{
    return m_size;
}

// Assign the passed contents array to this ByteArray
void ByteArray::setContents(const unsigned char* newContents)
{
    memcpy(m_contents, newContents, m_size);
}

// Assign the contents of this ByteArray to the passed
void ByteArray::getContents(unsigned char* contents) const
{
    memcpy(contents, m_contents, m_size);
}
