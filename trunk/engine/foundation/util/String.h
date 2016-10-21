//  String.h
//  A String class for the Vortex Engine
//  by Anton Grönberg
//  Copyright (c) 2016
//

#ifndef __BWLIB__String__
#define __BWLIB__String__

#ifdef _MSC_VER
// disable _s warnings
#define _CRT_SECURE_NO_WARNINGS
// disable pragma warnings
#pragma warning( disable : 4068 )
// standard function missing from MS library
#include <cstdarg>
int vasprintf(char ** ret, const char * format, va_list ap);
#else
#define _NOEXCEPT noexcept
#endif

#include <cstdlib>
#include <cstring>
#include <cstdarg>
#include <cctype>
#include <memory>

#define STRING_MAX_LEN 65535
#define STRING_MAX_SPLIT 1023

// simple smart C-string
namespace Util
{
    class String {
    public:

        String();                             // default constructor
        String( const char * s );             // c-string
        String( const String & );           // copy constructor
        String( String &&) _NOEXCEPT;       // move constructor
        ~String();

        // data management
        const char * alloc_str( size_t sz );    // smart alloc string
        void reset();                           // reset data
        void swap(String & b);                // member function swap
        const char * c_str() const;             // getter
        const char * copy_str( const char * );  // alloc & copy

        // operators
        String & operator = ( String );             // copy-and-swap assignment
        String & operator += ( const char * );        // concatenation operator
        String & operator += ( const String & );    // concatenation operator
        const char operator[] ( const int ) const;      // subscript operator

        // comparison operators
        bool operator == ( const String & ) const;
        bool operator != ( const String & ) const;
        bool operator > ( const String & ) const;
        bool operator < ( const String & ) const;
        bool operator >= ( const String & ) const;
        bool operator <= ( const String & ) const;

        // conversion operators
        operator const char * () const;             // c-string type

        // utility methods
        bool HaveValue() const;
        size_t Length() const { return _str_len; }
        size_t Size() const { return _str_len; }
        String & Format( const char * format, ... );
        String & Trim();
        String Lower() const;
        String Upper() const;
        const char & Back() const;
        const char & Front() const;

        // find and replace methods
        long int CharFind( const char & match ) const;
        const String & CharRepl( const char & match, const char & repl );
        String Substr( size_t start, size_t length );
        long Find( const String & match ) const;
        const String Replace( const String & match, const String & repl );

        // split methods
        const split_ptr & Split( const char * match ) const;
        const split_ptr & Split( const char match ) const;
        const split_ptr & Split( const char * match, int max_split ) const;
        const String & SplitItem( size_t index ) const;
        size_t SplitCount() const { return _split_count; }

    private:
        char * str = nullptr;
        size_t str_len = 0;

        // this is a poor man's vector
        // shared ptr to array of shared pointer String objects
        typedef std::shared_ptr<String> stringSp;
        typedef std::unique_ptr<stringSp[]> splitPtr;
        mutable split_ptr splitArray;
        mutable size_t splitCount = 0;

        // private methods
        void ResetSplitArr() const;
        void AppendSplitArr(const String & s) const;



    };

// non-member operator overloads
    String operator + ( const String & lhs, const String & rhs );
}


#endif  // __BWLIB__String__
