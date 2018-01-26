# MetaStock Database Library

```text
		By Marc Stahl <mstahl3@uwo.ca>
        Copyright (C) 2018 Marc Stahl
```
## WHAT IS THE METASTOCK DATABASE LIBRARY ?

MetaStockDB is a C++ library which provides an interface for creating, reading,
updating, and deleting financial data held in MetaStock databases.  The 
library is designed to be simple to use yet powerful, allowing developers
to easily access all aspects of the MetaStock database format.

The library includes a number of helper classes which make it easy to
encapsulate dates and trading data, but minimizes the number of classes
which must be learned in order to start using the library.


## WHAT CAN IT DO ?
The MetaStockDB library is designed to provide CRUD (create, read, update, and delete)
access to any and all MetaStock data.  The project is still a work in progress,
but at this time it will read into memory the entire contents of a MetaStock database,
and allow read access to all the data.

As well, the library permits adding of additional trading day data to the library.  However,
the library does not yet support saving (so such changes are lost).  See below for more details.


## WHAT CAN IT NOT DO ?
The MetaStockDB library does not yet permit writing (saving) of data to the database
stored on disk.  As well, many add/delete/update functions are not yet available.


## SUPPORTED PLATFORMS
The MetaStockDB is designed to use only POSIX compatible functions, and as a 
result should run on any POSIX compliant operating system (including
Windows and Linux).  At this time the product has only been developed and tested
on Linux, but the code includes conditional compilation for Windows specific
attributes (eg: path divider character).

The library should compile with any C++03 (and later) compiler.  Development
occurs with a C++03 compiler but the design should allow compilation without
change for later versions as well.


## WHAT'S INCLUDED

The library includes the following files.  Note that those marked as "internal" are for use within the library
only and the developer using this library does not need to understand them

|-- MetaStockDB
|   |-- activefields.cpp                             Internal: Class to manage active fields for an equity
|   |-- activefields.h
|   |-- bytearray.cpp                                Internal: Class to handle an array of bytes
|   |-- bytearray.h
|   |-- date.cpp                                     Class to store a single date and perform functions on that date
|   |-- date.h
|   |-- equity.h                                     Class to store all information about a single equity
|   |-- equityindb.cpp                               Internal: Class to store the equity data, and provide functionality
|   |                                                          to manipulate the files / data
|   |-- equityindb.h
|   |-- equityindb-interface.cpp                     Internal: Override of base class functions to create a simple
|   |                                                          interface to an equity
|   |-- globaltypes.h                                Internal: Shared types
|   |-- metastockdb.cpp                              Class containing all methods for accessing the database
|   |-- metastockdb.h
|   |-- msfileio.cpp                                 Internal: Helper functions to read/write proprietary type formats
|   |-- msfileio.h
|   |-- readme.md
|   |-- tradingday.cpp                               Internal: Class to store a single day trading info for a single day
|   |-- tradingday.h
|   |-- tradinghistory.cpp                           Internal: Class to store all available trading data for one stock
|   `-- tradinghistory.h


## WHATS NEXT
Work continues on the library and I plan to implement all remaing CRUD features.

As I don't have access to all possible databases formats, I have not yet tested
large datbases (more than 255 stocks, and containing an XMASTER file), or
databases containing stocks with periodicity other than daily.  If you have
databases in these formats please upload them so I can test them.

I also plan to add performance optimization including lazy loading, so that
large databases need not be totally loaded into memory; only equities which
are read/manipulated will be read into memory.  

I also plan to add a 'generations' feature, which allows each save of the
database to create a backup directory holding the previous generation.  This
allows for easy restoration of data following a crash or development bug.
