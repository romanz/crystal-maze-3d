// ======================================================== //
//                                                          //
// Filename : TARRAY.H                                      //
// Content  : Header file of templated array                //
// Author   : Roman Zeyde                                   //
// Date     : 21/04/2000                                    //
//                                                          //
// ======================================================== //

#ifndef __TARRAY_H
#define __TARRAY_H

#include "defines.h"

// -------------
// An array of T
// -------------

template <class T>
class TArray
{

public:

	TArray(TSize n = 0); // Initial size contructor
	TArray(const TArray<T> &a); // Copy contructor

	void Resize(TSize n); // Creates new array with specified size

	// Is the array allocated and valid?
	operator int() const { return size ? True : False; }

	virtual ~TArray() { if (size) delete []vector; } // Virtual destructor

	TArray<T> &operator = (const TArray<T> &a); // Assignment operator

	// Subscription operator
	const T &operator [] (TSize i) const;
	T &operator [] (TSize i);

	TSize Size() const { return size; } // Size of the array

protected:

	T *vector; // Pointer to vector of pointers
	TSize size; // Array's size

};

// Creates an n-size array (0 - empty array)
template <class T>
TArray<T>::TArray(TSize n):
 vector(n ? new T[n] : NULL),
 size(n)
{
	if (size)
		Assert(vector);
}

// Copies given array values to this one
template <class T>
TArray<T>::TArray(const TArray<T> &a) :
 vector(a ? new T[a.size] : NULL),
 size(a.size)
{
	if (size)
		Assert(vector);

	for (TSize i = 0; i < size; i++)
		vector[i] = a.vector[i];
}

// Assigns given array values to this one
template <class T>
TArray<T> &TArray<T>::operator = (const TArray<T> &a)
{
	if (this != &a)
	{
		if (size != a.size)
		{
			if (size)
				delete []vector;

			// New size assignment and memory allocation
			if (a)
			{
				vector = new T[size = a.size];
				Assert(vector);
			}
		}

		for (TSize i = 0; i < size; i++)
			vector[i] = a.vector[i];
	}

	return *this; // For several assignments in one statement
}

// Returns element #i - const version
template <class T>
inline
const T &TArray<T>::operator [] (TSize i) const
{
	Assert(size && i < size);
	return vector[i];
}

// Returns element #i - variable version
template <class T>
inline
T &TArray<T>::operator [] (TSize i)
{
	Assert(size && i < size);
	return vector[i];
}

// Creates new n-size array
template <class T>
inline void TArray<T>::Resize(TSize n)
{
	if (size)
		delete []vector;

	vector = (size = n) != 0 ? new T[size] : NULL;
}

#endif
