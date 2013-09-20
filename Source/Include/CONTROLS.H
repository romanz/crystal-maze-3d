// ======================================================== //
//                                                          //
// Filename : CONTROLS.H                                    //
// Content  : Header file of screen controls                //
// Author   : Roman Zeyde                                   //
// Date     : 08/04/2000                                    //
//                                                          //
// ======================================================== //

#ifndef __CONTROLS_H
#define __CONTROLS_H

#include <TList.h>
#include <Pixel.h>
#include <Mouse.h>

#include <Screen.h>

#include <Graphics.h>
#include <String.h>

// Base class of all controls
class TControl
{

public:

	TControl(const char *t = "") : tag(t) {}
	virtual ~TControl();

	virtual int Update() = 0; // Has been the control been updated?
	virtual void Show() = 0; // Shows the control

	virtual int In(const TPixel &) { return False; }
	// Is the pixel in the control?

	virtual const char *Tag() { return tag; } // Empty string

protected:

	const char *tag;

};

// The main loop should call in each iteration "Update()"
// and then "Show()" if the result is "True".

// Collection of controls for easier handling
class TControls : public TControl, protected TList<TControl *>
{

public:

	TControls();
	TControls &operator << (TControl &c); // Add a control

	virtual int Update(); // Have the controls been updated?
	virtual void Show(); // Shows all the controls

	virtual const char *Tag(); // Returns current control tag
};

// Rectangular control base
class TRectControl : public TControl
{

public:

	TRectControl(TPixel plt, TPixel prb); // LeftTop,RightBottom constructor

	// Filling function
	void Fill(int c = WHITE);

	// Drawing border function
	void Draw(int l = WHITE, int t = WHITE, int r = WHITE, int b = WHITE);

	// Is the pixel in the control?
	virtual int In(const TPixel &p);

protected:

	TPixel lt, rb; // LeftTop, RightBottom

};

// Rectangle for drawing and filling rectangles
class TRectangle : public TRectControl
{

public:

	TRectangle(TPixel lt, TPixel rb) : TRectControl(lt, rb) {}

	void Show()
	{
		Fill();
		Draw();
	}

	int Update() { return False; }

};

// Interactive button class
class TButton : public TRectControl
{

public:

	TButton(TPixel lt, TPixel rb); // Constructor

	virtual int Update(); // Has the button been updated?
	virtual void Show(); // Shows the button

	int Click() { return state == Released; } // Is there a click?

	int &Active() { return active; } // Is the button active?

protected:

	TState state; // The state of the button
	int active; // The control is active (can be updated)
};

// A button with text caption
class TStrButton : public TButton
{

public:

	// LeftTop,RightBottom,String constructor
	TStrButton(TPixel lt, TPixel rb, const char *s, const char *t = "");
	~TStrButton();

	virtual void Show(); // Shows the button

	const char *Str() { return strData; } // Returns the caption
	void Str(const char *s); // Sets new caption

protected:

	TPixel strCoords; // Caption coordinates
	char *strData; // Caption itself

	void initStrCoords(); // Initializes caption coordinates
};

// 3D-looking panel
class TPanel : public TRectControl
{

public:

	TPanel(TPixel lt, TPixel rb, int style, int f, const char *t = "");

	virtual int Update() { return False; } // Can't be updated
	virtual void Show(); // Shows the panel

protected:

	int Style;
	int fill;

};

class TLabel : public TPanel
{

public:

	TLabel(
	 TPixel lt, TPixel rb,
	 const char *s, int style, int f,
	 const char *t = ""
	);

	~TLabel() { delete []string; }

	virtual void Show();

	virtual int Update() { return changed; } // Can't be updated

	void operator = (const char *s);

	operator const char *() { return string; }

protected:

	TPixel TextPos();

	TPixel position;
	char *string;

	int changed;

};

class TCaption : public TLabel
{

public:

	TCaption(TPixel lt, TPixel rb, const char *str, const char *t = "");
	virtual void Show();

protected:

	int font, color, back, size;
};

// Inlines

inline
int TRectControl::In(const TPixel &p)
{
	return
	 (p.CrdX() >= lt.CrdX()) &&
	 (p.CrdX() <= rb.CrdX()) &&
	 (p.CrdY() >= lt.CrdY()) &&
	 (p.CrdY() <= rb.CrdY());
}

inline
TPixel TLabel::TextPos()
{
	return TPixel(
		(rb.CrdX() + lt.CrdX() - textwidth (string)) / 2,
		(rb.CrdY() + lt.CrdY() - textheight(string)) / 2
	);
}

#endif // __CONTROLS_H