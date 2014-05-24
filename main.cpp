#include <string>
#include <stdexcept>
#include <sstream>

//#define TESTING
#ifndef TESTING

#include <iostream>
#include <fstream>

class FileError : public std::runtime_error {
public:

    FileError( const std::string fileName ) : runtime_error( "" ), m_fileName( fileName ), m_message( "Error" ) {

    }

    virtual const char *what( ) const throw( ) {
        return m_message.c_str( );
    }

    virtual ~FileError( ) throw( ) {

    }

protected:
    std::string m_message;
    std::string m_fileName;
};

class FileOpenError : public FileError {
public:

    FileOpenError( const std::string &fileName ) : FileError( fileName ) {
        m_message = "Error: unable to open the file " + m_fileName;
    }
};

class FileReadError : public FileError {
public:

    FileReadError( const std::string &fileName, int lineNumber )
        : FileError( fileName ), m_lineNumber( lineNumber ) {
        m_message = "Error: unable to read the file " + m_fileName + " at the line " + intToString( m_lineNumber );
    }

private:
    int m_lineNumber;
    std::string intToString( int number ) {
        std::stringstream stream;
        stream << number;
        return stream.str( );
    }
};

class FileWriteError : public FileError {
public:

    FileWriteError( const std::string &fileName ) : FileError( fileName ) {
        m_message = "Error: unable to write file " + m_fileName;
    }
};

#endif // TESTING

class LogicError : public std::logic_error {
public:

    LogicError( ) : std::logic_error( "" ) {

    }

    virtual const char *what() const throw( ) {
        return m_message.c_str( );
    }

    virtual ~LogicError( ) throw( ) {

    }

protected:
    std::string m_message;
};

class OutOfRange : public LogicError {
public:

    OutOfRange( int argument, int beginOfRange, int endOfRange ) {
        std::string str_argument, str_beginOfRange, str_endOfRange;

        str_argument = intToString( argument );
        str_beginOfRange = intToString( beginOfRange );
        str_endOfRange = intToString( endOfRange );

        m_message = "Error: the argument " + str_argument +
                " don't hit to the range [" + str_beginOfRange +
                ", " + str_beginOfRange + "]";
    }

private:
    std::string intToString( int number ) {
        std::stringstream stream;
        stream << number;
        return stream.str( );
    }
};

#ifndef TESTING

class EmptyArgument : public LogicError {
public:

    EmptyArgument( ) {
        m_message = "Error: empty argument";
    }
};

void readData( const std::string &fileName, int &a, int &b, int &product )
throw( EmptyArgument, FileOpenError, FileReadError );

void writeResult( const std::string &fileName, const std::string &result )
throw( EmptyArgument, FileOpenError, FileWriteError );

bool isProduct( int a, int b, int product )
throw( OutOfRange );

int main( ) {

    std::string fileNameIn = "input.txt";
    int a = 0;
    int b = 0;
    int product = 0;

    // Read data from the file
    try {
        readData( fileNameIn, a, b, product );
    } catch( const FileError &e ) {
        std::cerr << e.what( ) << std::endl;
    } catch( const LogicError &e) {
        std::cerr << e.what( ) << std::endl;
    } catch( ... ) {
        std::cerr << "Error: unknown exception" << std::endl;
    }

    // Calc result
    std::string result;
    try {
        if ( isProduct( a, b, product ) ) {
            result = "YES";
        } else {
            result = "NO";
        }
    } catch( const LogicError &e ) {
        std::cerr << e.what( ) << std::endl;
    } catch( ... ) {
        std::cerr << "Error: unknown exception" << std::endl;
    }

    // Write data to the file
    std::string fileNameOut = "output.txt";
    try {
        writeResult( fileNameOut, result );
    } catch( const LogicError &e ) {
        std::cerr << e.what( ) << std::endl;
    } catch( const FileError &e ) {
        std::cerr << e.what( ) << std::endl;
    } catch( ... ) {
        std::cerr << "Error: unknown exception" << std::endl;
    }

    return 0;
}

void readData( const std::string &fileName, int &a, int &b, int &product )
throw( EmptyArgument, FileOpenError, FileReadError ) {
    // Check the file name
    if ( fileName.empty( ) ) {
        throw EmptyArgument( );
    }

    // Open the input file
    std::ifstream file;
    file.open( fileName.c_str( ) );
    if ( file.fail( ) ) {
        throw FileOpenError( fileName );
    }

    // Read data from the input file
    int lineNumber = 0;
    if ( !( file >> a >> b >> product ) ) {
        throw FileReadError( fileName, lineNumber );
    }
}

void writeResult( const std::string &fileName, const std::string &result )
throw( EmptyArgument, FileOpenError, FileWriteError ) {
    // Check arguments
    if ( fileName.empty() || result.empty( ) ) {
        throw EmptyArgument( );
    }

    // Open the output file
    std::ofstream file;
    file.open( fileName.c_str( ) );
    if ( file.fail( ) ) {
        throw FileOpenError( fileName );
    }

    // Write a result the output file
    if ( !( file << result << std::endl ) ) {
        throw FileWriteError( fileName );
    }
}

#endif // TESTING

bool isProduct( int a, int b, int product )
throw( OutOfRange ) {
    const int beginOfRange = 0;
    const int endOfRange = 100;
    const int beginOfRangeOfProduct = 0;
    const int endOfRangeOfProduct = 1000000;

    if ( ( a < beginOfRange ) || ( endOfRange < a ) ) {
        throw OutOfRange( a, beginOfRange, endOfRange );
    }

    if ( ( b < beginOfRange ) || ( endOfRange < b ) ) {
        throw OutOfRange( b, beginOfRange, endOfRange );
    }

    if ( ( product < beginOfRangeOfProduct ) || ( endOfRangeOfProduct < product ) ) {
        throw OutOfRange( product, beginOfRangeOfProduct, endOfRangeOfProduct );
    }

    bool result;

    if ( ( a * b ) == product ) {
        result = true;
    } else {
        result = false;
    }

    return result;
}
