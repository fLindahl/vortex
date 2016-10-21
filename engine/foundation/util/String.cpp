//  String.cpp
//  A String class for the Vortex Engine
//  by Anton Grönberg
//  Copyright (c) 2016
//

#include "String.h"

#pragma mark - constructors/destructors
namespace Util
{
    String::String( )
    {
        reset();
    }

    String::String( const char * s )
    {
        copy_str(s);
    }

    String::String( const String & old )
    {
        copy_str(old);
    }


    String::~String()
    {
        reset();
    }

// move constructor
    String::String( String && other ) _NOEXCEPT
    {
        reset();
        str = other.str;
        str_len = other.str_len;
        other.str = nullptr;
        other.str_len = 0;
        other.reset();
    }

#pragma mark - private methods

    void String::ResetSplitArr() const
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

    void String::AppendSplitArr(const String &s) const
    {
        if (splitCount >= STRING_MAX_SPLIT) return;
        if (!splitCount)
        {
            splitArray.reset(new _bwsp[STRING_MAX_SPLIT + 1]);
        }
        splitArray[splitCount] = std::make_shared<String>(s);
        ++splitCount;
    }

#pragma mark - public methods

    const char * String::alloc_str( size_t sz )
    {
        if (str)
            reset();
        str_len = (sz > STRING_MAX_LEN) ? STRING_MAX_LEN : sz;
        str = new char[str_len + 1]();  // new char[]() fills with 0
        return str;
    }

    void String::reset()
    {
        ResetSplitArr();
        if(str)
            delete [] str;
        str = nullptr;
        str_len = 0;
    }

    void String::swap(String & other)
    {
        std::swap(str, other.str);
        std::swap(str_len, other.str_len);
    }

    const char * String::c_str() const
    {
        return str;
    }

    const char * String::copy_str( const char * s) {
        if(s) {
            size_t len = strnlen(s, STRING_MAX_LEN);
            alloc_str(len);
            strncpy((char *)str, s, len);
            str_len = len;
        }
        return str;
    }

#pragma mark - operators

// copy-and-swap assignment
    String & String::operator = ( String other ) {
        swap(other);
        return *this;
    }

    String & String::operator += ( const char * rhs ) {
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

    String & String::operator += ( const String & rhs ) {
        operator+=(rhs.c_str());
        return *this;
    }

    const char String::operator[] ( const int index ) const {
        if(index < 0) return 0;
        if(index >= (int) str_len) return 0;
        else return str[index];
    }

#pragma mark - comparison operators

    bool String::operator == ( const String & rhs ) const {
        if( std::strncmp(this->c_str(), rhs.c_str(), STRING_MAX_LEN) == 0 ) return true;
        else return false;
    }

    bool String::operator != ( const String & rhs ) const {
        if( std::strncmp(this->c_str(), rhs.c_str(), STRING_MAX_LEN) != 0 ) return true;
        else return false;
    }

    bool String::operator > ( const String & rhs ) const {
        if( std::strncmp(this->c_str(), rhs.c_str(), STRING_MAX_LEN) > 0 ) return true;
        else return false;
    }

    bool String::operator < ( const String & rhs ) const {
        if( std::strncmp(this->c_str(), rhs.c_str(), STRING_MAX_LEN) < 0 ) return true;
        else return false;
    }

    bool String::operator >= ( const String & rhs ) const {
        if( std::strncmp(this->c_str(), rhs.c_str(), STRING_MAX_LEN) >= 0 ) return true;
        else return false;
    }

    bool String::operator <= ( const String & rhs ) const {
        if( std::strncmp(this->c_str(), rhs.c_str(), STRING_MAX_LEN) <= 0 ) return true;
        else return false;
    }

#pragma mark - conversion operators

    String::operator const char * () const
    {
        return c_str();
    }

#pragma mark - Utility methods

    bool String::HaveValue() const
    {
        if(str)
            return true;
        else
            return false;
    }

// string format
    String & String::Format( const char * format , ... )
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
    String & String::Trim()
    {
        const static char * whitespace = "\x20\x1b\t\r\n\v\b\f\a";

        if(!have_value())
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
                tr[i] = str[begin++];
            }
        }

        str_len = strlen(str);
        return *this;
    }

    String String::Lower() const {
        String rs = *this;
        for (size_t i = 0; rs.str[i]; ++i) {
            rs.str[i] = tolower(rs.str[i]);
        }
        return rs;
    }

    String String::Upper() const {
        String rs = *this;
        for (size_t i = 0; rs.str[i]; ++i)
        {
            rs.str[i] = toupper(rs.str[i]);
        }
        return rs;
    }

    const char & String::Back() const {
        return str[length() - 1];
    }

    const char & String::Front() const {
        return str[0];
    }

#pragma mark find and replace methods

    long int String::CharFind(const char &match)
    {
        for (size_t i = 0; str[i]; ++i)
        {
            if(str[i] == match)
                return i;
        }
        return -1;
    }

    const String & String::CharRepl( const char & match, const char & repl )
    {
        for (size_t i = 0; str[i]; ++i)
        {
            if(str[i] == match)
                str[i] = repl;
        }
        return *this;
    }

    String String::Substr( size_t start, size_t length )
    {
        String rs;
        char * buf;
        if ((length + 1) > STRING_MAX_LEN || (start + length) > STRING_MAX_LEN)
            return rs;
        if (length > str_len - start)
            return rs;
        if (!str)
            return rs;

        buf = new char[length + 1]();
        memcpy(buf, str + start, length);
        rs = buf;
        delete [] buf;

        return rs;
    }

    long String::Find( const String & match ) const
    {
        char * pos = strstr(str, match.c_str());
        if(pos)
            return (long) ( pos - str );
        else
            return -1;
    }

    const String String::Replace( const String & match, const String & repl )
    {
        String rs;
        long f1 = find(match);
        if (f1 >= 0)
        {
            size_t pos1 = (size_t) f1;
            size_t pos2 = pos1 + match.length();
            String s1 = pos1 > 0 ? substr(0, pos1) : "";
            String s2 = substr(pos2, length() - pos2);
            rs = s1 + repl + s2;
        }
        return rs;
    }

#pragma mark split methods

// non-destructive split
    const String::split_ptr & String::Split( const char match ) const
    {
        const char match_s[2] = { match, 0 };
        return Split(match_s, -1);
    }

    const String::split_ptr & String::Split( const char * match ) const
    {
        return Split(match, -1);
    }

    const String::split_ptr & String::Split( const char * match, int max_split ) const
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

        return split_array;
    }

// zero-based index of _split_array
    const String & String::SplitItem(size_t index)
    {
        if(splitCount > index)
            return *splitArray[index];
        else
            return *this;
    }

#pragma mark - non-member operator overloads

    String operator + ( const String & lhs, const String & rhs ) {
        String rs = lhs;
        rs += rhs;
        return rs;
    }

#ifdef _MSC_VER

    #pragma mark - MS missing standard functions

int vasprintf(char ** ret, const char * format, va_list ap)
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

