/*
 * Class: Main
 * Author: Marc Stahl
 * Description: Finds a stock and prints it's description.
 * History:
 *   MKS    2018-Jan-19   Original coding
 */

#include "metastockdb.h"
#include <iostream>

using namespace std;


int main(int argc, char *argv[])
{
    Equity* equityPtr;

    // Create a MetaStockDB object, which will read all of the data from the proprietary files at the given directory
    MetaStockDB myDataBase("/tmp/MyMetaStockDbDirectory", false, 0);

    // Find the specified equity, and then print the description if found
    equityPtr = myDataBase.find("^GSPTTTS");
    if (equityPtr)
        cout << "Equity description: " << equityPtr->description() << endl;
    else
        cout << "Equity not found" << endl;

    return 0;
}