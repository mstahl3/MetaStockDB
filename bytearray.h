/*
 * Class: ByteArray
 * Author: Marc Stahl
 * Description: Stores n bytes on the heap, where n is a user input.
 * History:
 *   MKS    2018-Jan-19   Original coding
 */

#ifndef BYTEARRAY_H
#define BYTEARRAY_H

class ByteArray
{
public:
    // Construct a ByteArray and fill it with the contents that are passed in.
    ByteArray(const unsigned char* contents, const unsigned int size);

    // Construct a ByteArray and fill it with null bytes (0000 0000).
    ByteArray(const unsigned int size);

    // Construct and copy the contents and size of the passed ByteArray, into this ByteArray
    ByteArray(const ByteArray &originalByteArray);

    // Free the contents char array
    ~ByteArray();

    // Copy the contents from one ByteArray object to another
    void operator=(const ByteArray rightByteArray);

    // Get byte array size
    unsigned int size() const;


    // Assign a new contents array to this ByteArray
    void setContents(const unsigned char *newContents);


    // Retrieve the contents of this ByteArray
    void getContents(unsigned char *contents) const;

private:
    // pointer to the data stored in byteArray
    unsigned char* m_contents;

    // size of the contents array
    unsigned int m_size;
};

#endif // BYTEARRAY_H
