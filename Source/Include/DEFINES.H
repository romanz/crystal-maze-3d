// ======================================================== //
//                                                          //
// Filename : DEFINES.H                                     //
// Content  : Header file of standart defines               //
// Author   : Roman Zeyde                                   //
// Date     : 23/01/2000                                    //
//                                                          //
// ======================================================== //

#ifndef __DEFINES_H
#define __DEFINES_H

#include <stddef.h>
#include <assert.h>

// Assertion function for assertion made in program
#define Assert(x) assert(x)

// Math macros
#define SQUARE(x) ((x)*(x))
#define CUBE(x) ((x)*(x)*(x))

// Boolean constants
enum { False, True };

// Type defines
typedef unsigned char Byte;
typedef unsigned int  Word;
typedef unsigned long DoubleWord;
typedef size_t        TSize;
typedef float         Real;

#endif
