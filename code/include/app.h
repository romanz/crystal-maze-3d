// ======================================================== //
//                                                          //
// Filename : APP.H                                         //
// Content  : Header file of the maze application           //
// Author   : Roman Zeyde                                   //
// Date     : 21/04/2000                                    //
//                                                          //
// ======================================================== //

#ifndef __APP_H
#define __APP_H

#include <game.h>
#include <time.h>

// Application is one game in one maze
class App
{

public:

	App(const char *MazeFile); // Data file constructor
	~App(); // Destructor

	TSize Run(); // Runs the game

protected:

	int KeyInput(); // Gets input from keyboard
	int ButtonInput(); // Gets input from the buttons

	void UpdateInfo(); // Updates information labels

	TSize crystals; // Number of crystals collected

	TPanel panel; // Panel
	TMaze maze; // Game viewport
	TStrButton forward, back, left, right, up, down; // Movement buttons
	TLabel control, move, turn; // Movement labels
	TLabel info, found, pos, dist, timer; // Information lables

	TLabel status; // Status

	TCaption caption; // Caption of the game

	TControls controls; // List of controls

	clock_t Start; // Start time of the game

	TStrButton done; // Exit button

};

enum { CLK_TCK = CLOCKS_PER_SEC };

// Timer is used to measure specified time periods
class Timer
{

public:

	// Default constructor
	Timer() : t(0) {}

	// Starts the timer for x seconds
	void Start(Real x) { t = clock() + x*CLK_TCK; }

	// Is the timer done?
	int Done() { return clock() > t; }

protected:

	clock_t t;

};

#endif
