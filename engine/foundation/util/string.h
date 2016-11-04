#pragma once
//  String.h
//  A String class for the Vortex Engine
//  by Anton Grönberg
//  Copyright (c) 2016
//

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

#include <cstring>
#include <cstdarg>
#define STRING_MAX_LEN 65535
#define STRING_MAX_SPLIT 1023

// simple smart C-string
namespace Util
{
    class String
    {

    private:
        char * str = nullptr;
        size_t str_len = 0;

        // this is a poor man's vector
        // shared ptr to array of shared pointer String objects
        typedef std::shared_ptr<String> stringSp;
        typedef std::unique_ptr<stringSp[]> splitPtr;
        mutable splitPtr splitArray;
        mutable size_t splitCount = 0;

        // private methods
        void ResetSplitArr() const;
        void AppendSplitArr(const String & s) const;



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
		bool operator == (const char* ) const;
        bool operator != ( const String & ) const;
        bool operator > ( const String & ) const;
        bool operator < ( const String & ) const;
        bool operator >= ( const String & ) const;
        bool operator <= ( const String & ) const;

        // conversion operators
        operator const char * () const;             // c-string type

        // utility methods
		bool Append(const char*);
		bool Append(const String&);

        bool HaveValue() const;
        size_t length() const { return str_len; }
        size_t size() const { return str_len; }
        String & format( const char * format, ... );
        String & trim();
        String lower() const;
        String upper() const;
        const char & back() const;
        const char & front() const;

        // find and replace methods
        long int CharFind( const char & match ) const;
        const String & CharRepl( const char & match, const char & repl );
        String Substr( size_t start, size_t length );
        long Find(const String & match) const;
        const String Replace( const String & match, const String & repl );

        // split methods
        const splitPtr & Split( const char * match ) const;
        const splitPtr & Split( const char match ) const;
        const splitPtr & Split( const char * match, int max_split ) const;
        const String & SplitItem( size_t index ) const;
        size_t SplitCount() const { return splitCount; }

        //Tokenize
        static Util::Array<Util::String> Tokenize(Util::String str, Util::String delim);

        static const size_t npos = -1;
    };

    inline String::String( )
    {
        reset();
    }

    inline String::String( const char * s )
    {
        copy_str(s);
    }

    inline String::String( const String & old )
    {
        copy_str(old);
    }

    // move constructor
    inline String::String( String && other ) _NOEXCEPT
    {
        reset();
        str = other.str;
        str_len = other.str_len;
        other.str = nullptr;
        other.str_len = 0;
        other.reset();
    }

    inline String::~String()
    {
        reset();
    }

// non-member operator overloads
    String operator + ( const String & lhs, const String & rhs );


    inline void String::ResetSplitArr() const
    {
        if (splitCount)
        {
            // dtor the elements in the array
            while(splitCount)
            {
                splitArray[--splitCount].reset();
            }
            splitArray.reset();
            splitCount = 0;
        }
    }

    inline void String::AppendSplitArr(const String &s) const
    {
        if (splitCount >= STRING_MAX_SPLIT)
            return;
        if (!splitCount)
        {
            splitArray.reset(new stringSp[STRING_MAX_SPLIT + 1]);
        }
        splitArray[splitCount] = std::make_shared<String>(s);
        ++splitCount;
    }


    inline const char * String::alloc_str( size_t sz )
    {
        if (str)
            reset();
        str_len = (sz > STRING_MAX_LEN) ? STRING_MAX_LEN : sz;
        str = new char[str_len + 1]();  // new char[]() fills with 0
        return str;
    }

    inline void String::reset()
    {
        ResetSplitArr();
        if(str)
            delete [] str;
        str = nullptr;
        str_len = 0;
    }

    inline void String::swap(String & other)
    {
        std::swap(str, other.str);
        std::swap(str_len, other.str_len);
    }

    inline const char * String::c_str() const
    {
        return str;
    }

    inline const char * String::copy_str( const char * s) {
        if(s) {
            size_t len = strnlen(s, STRING_MAX_LEN);
            alloc_str(len);
            strncpy((char *)str, s, len);
            str_len = len;
        }
        return str;
    }


// copy-and-swap assignment
    inline String & String::operator = ( String other ) {
        swap(other);
        return *this;
    }

    inline String & String::operator += ( const char * rhs ) {
        if(rhs) {
            size_t newlen = str_len + strnlen(rhs, STRING_MAX_LEN);
            if (newlen > STRING_MAX_LEN) newlen = STRING_MAX_LEN;

            size_t rhslen = newlen - str_len;
            if(rhslen < 1) return *this;

            char * buf = new char[newlen + 1]();
            if(str && str_len) memcpy(buf, str, str_len);
            memcpy(buf + str_len, rhs, rhslen);
            copy_str(buf);
            delete [] buf;
        }
        return *this;
    }

    inline String & String::operator += ( const String & rhs ) {
        operator+=(rhs.c_str());
        return *this;
    }

    inline const char String::operator[] ( const int index ) const {
        if(index < 0) return 0;
        if(index >= (int) str_len) return 0;
        else return str[index];
    }


    inline bool String::operator == ( const String & rhs ) const {
        if( std::strncmp(this->c_str(), rhs.c_str(), STRING_MAX_LEN) == 0 ) return true;
        else return false;
    }

	inline bool String::operator == (const char* rhs) const {
		if (std::strncmp(this->c_str(), rhs, STRING_MAX_LEN) == 0) return true;
		else return false;
	}

    inline bool String::operator != ( const String & rhs ) const {
        if( std::strncmp(this->c_str(), rhs.c_str(), STRING_MAX_LEN) != 0 ) return true;
        else return false;
    }

    inline bool String::operator > ( const String & rhs ) const {
        if( std::strncmp(this->c_str(), rhs.c_str(), STRING_MAX_LEN) > 0 ) return true;
        else return false;
    }

    inline bool String::operator < ( const String & rhs ) const {
        if( std::strncmp(this->c_str(), rhs.c_str(), STRING_MAX_LEN) < 0 ) return true;
        else return false;
    }

    inline bool String::operator >= ( const String & rhs ) const {
        if( std::strncmp(this->c_str(), rhs.c_str(), STRING_MAX_LEN) >= 0 ) return true;
        else return false;
    }

    inline bool String::operator <= ( const String & rhs ) const {
        if( std::strncmp(this->c_str(), rhs.c_str(), STRING_MAX_LEN) <= 0 ) return true;
        else return false;
    }

	inline bool String::Append(const char* cstr)
	{
		this->operator+=(cstr);
		return true;
	}
	
	inline bool String::Append(const String& rhs)
	{
		this->operator+=(rhs.c_str());
		return true;
	}

    inline String::operator const char * () const
    {
        return c_str();
    }

    inline bool String::HaveValue() const
    {
        if(str)
            return true;
        else
            return false;
    }

// string format
    inline String & String::format( const char * format , ... )
    {
        char * buffer;

        va_list args;
        va_start(args, format);

        vasprintf(&buffer, format, args);
        copy_str(buffer);
        free(buffer);   // vasprintf uses malloc
        return *this;
    }

// trim leading and trailing spaces
    inline String & String::trim()
    {
        const static char * whitespace = "\x20\x1b\t\r\n\v\b\f\a";

        if(!HaveValue())
            return *this; // make sure we have a string

        size_t begin = 0;
        size_t end = length() - 1;

        for (begin = 0; begin <= end; ++begin)
        {
            if (strchr(whitespace, str[begin]) == nullptr)
            {
                break;
            }
        }

        for ( ; end > begin; --end)
        {
            if (strchr(whitespace, str[end]) == nullptr)
            {
                break;
            }
            else
            {
                str[end] = '\0';
            }
        }

        if (begin)
        {
            for (size_t i = 0; str[i]; ++i)
            {
                str[i] = str[begin++];
            }
        }

        str_len = strlen(str);
        return *this;
    }

    inline String String::lower() const {
        String rs = *this;
        for (size_t i = 0; rs.str[i]; ++i) {
            rs.str[i] = tolower(rs.str[i]);
        }
        return rs;
    }

    inline String String::upper() const {
        String rs = *this;
        for (size_t i = 0; rs.str[i]; ++i)
        {
            rs.str[i] = toupper(rs.str[i]);
        }
        return rs;
    }

    inline const char & String::back() const {
        return str[length() - 1];
    }

    inline const char & String::front() const {
        return str[0];
    }

    inline long int String::CharFind(const char &match) const
    {
        for (size_t i = 0; str[i]; ++i)
        {
            if(str[i] == match)
                return i;
        }
        return -1;
    }

    inline const String & String::CharRepl( const char & match, const char & repl )
    {
        for (size_t i = 0; str[i]; ++i)
        {
            if(str[i] == match)
                str[i] = repl;
        }
        return *this;
    }

    inline String String::Substr( size_t start, size_t length ) {
        String rs;
        char *buf;
        if ((length + 1) > STRING_MAX_LEN || (start + length) > STRING_MAX_LEN)
            return rs;
        if (length > str_len - start)
            return rs;
        if (!str)
            return rs;

        buf = new char[length + 1]();
        memcpy(buf, str + start, length);
        rs = buf;
        delete[] buf;

        return rs;
    }

    inline long String::Find(const String & match) const
    {
        const char * pos = strstr(str, match.c_str());
        if(pos)
            return (long) ( pos - str );
        else
            return -1;
    }

    inline const String String::Replace( const String & match, const String & repl )
    {
        String rs;
        long f1 = Find(match);
        if (f1 >= 0)
        {
            size_t pos1 = (size_t) f1;
            size_t pos2 = pos1 + match.length();
            String s1 = pos1 > 0 ? Substr(0, pos1) : "";
            String s2 = Substr(pos2, length() - pos2);
            rs = s1 + repl + s2;
        }
        return rs;
    }

// non-destructive split
    inline const String::splitPtr & String::Split( const char match ) const
    {
        const char match_s[2] = { match, 0 };
        return Split(match_s, -1);
    }

    inline const String::splitPtr & String::Split( const char * match ) const
    {
        return Split(match, -1);
    }

    inline const String::splitPtr & String::Split( const char * match, int max_split ) const
    {
        ResetSplitArr();
        if (length() < 1)
            return splitArray;
        if (max_split < 0)
            max_split = STRING_MAX_SPLIT;

        size_t match_len = strnlen(match, STRING_MAX_LEN);
        if(match_len >= STRING_MAX_LEN)
            return splitArray;

        char * mi;              // match index
        char * pstr = str;     // string pointer
        while (( mi = strstr(pstr, match)) && --max_split )
        {
            if(mi != pstr) {
                size_t lhsz = mi - pstr;
                char * cslhs = new char[lhsz + 1]();
                memcpy(cslhs, pstr, lhsz);
                AppendSplitArr(cslhs);
                delete [] cslhs;
                pstr += lhsz;
            }
            pstr += match_len;
        }

        if (*pstr != '\0') {
            AppendSplitArr(pstr);
        }

        return splitArray;
    }

// zero-based index of _split_array
    inline const String & String::SplitItem(size_t index) const
    {
        if(splitCount > index)
            return *splitArray[index];
        else
            return *this;
    }

    inline Util::Array<String> String::Tokenize(Util::String str, Util::String delim)
    {
        Util::Array<String> tokens;

        char* ptr = const_cast<char*>(str.c_str());
        const char* token;
        while (0 != (token = strtok(ptr, delim.c_str())))
        {
            tokens.Append(token);
            ptr = 0;
        }

        return tokens;
    }

    inline String operator + ( const String & lhs, const String & rhs ) {
        String rs = lhs;
        rs += rhs;
        return rs;
    }

#ifdef _MSC_VER

    #pragma mark - MS missing standard functions

inline int vasprintf(char ** ret, const char * format, va_list ap)
{
    int len;
    char *buffer;

    len = _vscprintf(format, ap) + 1;
    buffer = (char *) malloc(len * sizeof(char));
    if (!buffer) return 0;
    vsprintf_s(buffer, len, format, ap);
    *ret = buffer;
    return len -1;
}

#endif
}