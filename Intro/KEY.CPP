#include "key.h"

#include <dos.h>
#include <conio.h>

#define KEYBOARD_INTR 0x09
#define KEYBOARD_PORT 0x60

// Initialize keys array with 0's
int keys[256] = {0};

// Old interrupt function pointer
void interrupt (*OldInt)(...);

// New interrupt function
void interrupt NewInt(...)
{
	// Scan code and e0 (extended scan) flag
	static int scan, e0 = 0;

	// Read scan code from 0x60 port (keyboard port)
	scan = inp(KEYBOARD_PORT);

	// Bits' meaning
	// ~~~~~~~~~~~~~
	// #0-#6: scan code of the key
	// #7   : Pressed = 0, Released = 1

	if (scan != 0xE0) // If it's not e0 (not extended)
	{
		// Store key status at specified index
		// 0x00-0x7F = Normal keys (Ored with e0 = 0)
		// 0x80-0xFF = Extended keys (ORed with e0 = 10000000)
		keys[scan & 0x7F | e0] = scan & 0x80 ? 0 : 1;
		e0 = 0; // Restore e0 value to 0
	}
	else
		e0 = 0x80; // Update e0 value to 10000000

	OldInt(); // Call old interrupt function
}

// Sets new interrupt function after saving the old one
void SetNewInt()
{
	OldInt = getvect(KEYBOARD_INTR); // Save old interrupt function
	setvect(KEYBOARD_INTR, NewInt); // Set the new one
}

// Restores old interrupt function
void SetOldInt()
{
	setvect(KEYBOARD_INTR, OldInt); // Restore old interrupt function
}

// Constructs new keyboard class
TKeyboard::TKeyboard()
{
	SetNewInt(); // Sets new interrupt function
}

// Destructs given keyboard class
TKeyboard::~TKeyboard()
{
	SetOldInt(); // Sets old interrupt function
}

// Returns key status (Down = 1, Up = 0)
int TKeyboard::Key(ScanCode code)
{
	return keys[code]; // Key status array
}

// Flushes keyboard buffer
void TKeyboard::Clear()
{
	while (kbhit())
		getch();
}

// The only keyboard in the program
TKeyboard Keyboard;