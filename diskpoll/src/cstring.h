#ifndef __CSTRING_HPP
#define __CSTRING_HPP

#ifdef INCS_NEED_DOT_H
#include <string.h>
#include <iostream.h>
#else
#include <string>
#include <iostream>
#endif

class CString
{ protected: char *buffer;
             size_t length;
             size_t alloc_len;
  public:    CString();
             CString(const CString &);
             CString(const char *);
             ~CString();

             size_t Length() const;

             operator const char *() const;
             operator const char *();
             operator char*();
             CString  substr(size_t,size_t) const;
             CString& operator +=(const CString &);
             CString& operator +=(const char&);
             CString& operator =(const CString &);
             char&   charAt(const size_t index) const;
             char&   setCharAt(const size_t index, const char&);
//             char& operator[](const size_t index);
//             char& operator[](const size_t index) const;
             int operator== (const CString &) const;
             int operator== (const char *) const;
             int operator== (char *) const;


             friend CString operator + (const CString &, const CString &);
             friend ostream& operator << (ostream&, const CString&);
             friend istream& operator >> (istream&, CString&);

};

CString upcase(const CString& src);

#define NULLSTRING CString((char *)NULL)

#endif
