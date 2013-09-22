// ======================================================== //
//                                                          //
// Filename : TSTRING.H                                     //
// Content  : Header file of string class                   //
// Author   : Roman Zeyde                                   //
// Date     : 16/09/2000                                    //
//                                                          //
// ======================================================== //

#ifndef __TSTRING_H
#define __TSTRING_H

#include <iostream.h>
#include <string.h>
#include <ctype.h>

#include "defines.h"

// Class declaration

class TString {
public:
	TString(TSize n = 0); // Size constructor
	TString(const char *s); // String constructor
	TString(const TString &s); // Copy constructor

	virtual ~TString() { delete []data; } // Virtual destructor

	TString &operator = (const TString &s); // Assignment operator

	operator const char *() const; // String conversion

	// Subscription operators
	const char &operator [](size_t i) const; // Constant
	char &operator [](size_t i); // Variable

	// Comparing functions
	int operator < (const TString &s) const;
	int operator > (const TString &s) const;
	int operator != (const TString &s) const;
	int operator == (const TString &s) const;
	int operator <= (const TString &s) const;
	int operator >= (const TString &s) const;

	// Converts string to upper/lower case
	TString &Upper();
	TString &Lower();

	// Concatenation operators
	TString operator + (const TString &s) const;
	TString &operator += (const TString &s)
	{
		return operator = ( operator + (s) );
	}

	// I/O operators
	friend ostream &operator << (ostream &o, const TString &s);
	friend istream &operator >> (istream &i, const TString &s);

	TSize Length() const { return strlen(data); } // Actual length
	TSize Size() const { return size; } // Formal size
	void Resize(TSize n); // Change formal size

protected:
	char *data;
	TSize size;
};

// String conversion
inline
TString::operator const char *() const
{
	return data;
}

// Size constructor
inline
TString::TString(TSize n) : data(new char[n+1]), size(n)
{
	Assert(data);
	*data = '\0';
}

// String constructor
inline
TString::TString(const char *s) : data(strdup(s)), size(strlen(data))
{
	Assert(data);
}

// Copy constructor
inline
TString::TString(const TString &s) : data(strdup(s)), size(strlen(data))
{
	Assert(data);
}

// Assignment operator
inline
TString &TString::operator = (const TString &s)
{
	if (this != &s)
	{
		if (size < s.size)
		{
			delete []data;
			data = new char[(size = s.size) + 1];
			Assert(data);
		}
		strcpy(data, s.data);
	}
	return *this;
}

// Constant subscription operator
inline
const char &TString::operator [](size_t i) const
{
	Assert(i <= size);
	return data[i];
}

// Variable subscription operator
inline
char &TString::operator [](size_t i)
{
	Assert(i <= size);
	return data[i];
}

// Concatenation operator
inline
TString TString::operator + (const TString &s) const
{
	TString temp(strlen(data) + strlen(s.data) + 1);

	strcpy(temp.data, data);
	strcat(temp.data, s.data);

	return temp;
}

// "Less" operator
inline
int TString::operator < (const TString &s) const
{
	return strcmp(data, s.data) < 0;
}

// "More" operator
inline
int TString::operator > (const TString &s) const
{
	return strcmp(data, s.data) > 0;
}

// "Not equal" operator
inline
int TString::operator != (const TString &s) const
{
	return strcmp(data, s.data) != 0;
}

// "Equal" operator
inline
int TString::operator == (const TString &s) const
{
	return strcmp(data, s.data) == 0;
}

// "Less or Equal" operator
inline
int TString::operator <= (const TString &s) const
{
	return strcmp(data, s.data) <= 0;
}

// "More or Equal" operator
inline
int TString::operator >= (const TString &s) const
{
	return strcmp(data, s.data) >= 0;
}

// Converts string to upper case
inline
TString &TString::Upper()
{
	for (char *p = data; *p; ++p)
		*p = toupper(*p);
	return *this;
}

// Converts string to lower case
inline
TString &TString::Lower()
{
	for (char *p = data; *p; ++p)
		*p = tolower(*p);
	return *this;
}

// Output operator
inline
ostream &operator << (ostream &o, const TString &s)
{
	o.operator << (s);
	return o;
}

// Input operator
inline
istream &operator >> (istream &i, const TString &s)
{
	i.getline(s.data, s.size + 1);
	return i;
}

// Change formal size
inline
void TString::Resize(TSize n)
{
	if (size != n)
	{
		delete []data;

		data = new char[(size = n) + 1];
		Assert(data);

		*data = '\0';
	}
}


#endif // __STRINGS_H
