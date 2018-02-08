/*
 * Class: Main
 * Author: Marc Stahl
 * Description: Iterates through all equities in the database and prints the symbols and descriptions.
 * History:
 *   MKS    2018-Jan-19   Original coding
 */

#include "metastockdb.h"
#include <iostream>

using namespace std;

int main(int argc, char *argv[])
{
    // This pointer is used to iterate through each equity in myDataBase
    Equity* equityPtr;

    // Create a MetaStockDB object with all the data from the proprietary files at the given directory
    MetaStockDB myDataBase("/tmp/MyMetaStockDbDirectory", false, 0);

    // If find a first equity
    if (myDataBase.getFirstEquity(&equityPtr)) {
        // Loop through remaining equities, and print symbol and description
        do {
            cout << "Symbol: " << equityPtr->symbol() << endl;
            cout << "Description: " << equityPtr->description() << endl << endl;
        }
        while(myDataBase.getNextEquityPtr(&equityPtr));
    } else
        cout << "Database is empty" << endl;

    return 0;
}


