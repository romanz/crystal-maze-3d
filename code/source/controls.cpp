// ======================================================== //
//                                                          //
// Filename : CONTROLS.CPP                                  //
// Content  : Source file of screen controls                //
// Author   : Roman Zeyde                                   //
// Date     : 08/04/2000                                    //
//                                                          //
// ======================================================== //

#include <Controls.h>

TControl::~TControl()
{
}

TControls::TControls() : TControl(), TList<TControl *>()
{
}

TControls &TControls::operator << (TControl &c)
{
	AddToTail(&c);
	return *this;
}

// Check if any control was updated
int TControls::Update()
{
	int x = 0;
	TListPos<TControl *> p(*this);

	for (++p; p; ++p)
		x |= p.Current()->Update(); // OR-ing the result to "x"

	return x; // if at least one control is updated
}

void TControls::Show()
{
	TListPos<TControl *> p(*this);

	for (++p; p; ++p)
		p.Current()->Show(); // Shows every control
}

const char *TControls::Tag() // Returns current control tag
{
	TListPos<TControl *> i(*this, False); // Iterate from end
	const char *s = "";

	for (--i; i; --i)
		if (i.Current()->In(Mouse))
			if ( *(s = i.Current()->Tag()) )
				break;

	return s;
}

TRectControl::TRectControl(TPixel plt, TPixel prb) : lt(plt), rb(prb)
{
}

void TRectControl::Fill(int c)
{
	setfillstyle(SOLID_FILL, c);
	bar(lt.CrdX(), lt.CrdY(), rb.CrdX(), rb.CrdY());
}

void TRectControl::Draw(int l, int t, int r, int b)
{
	setfillstyle(SOLID_FILL, l);
	bar(lt.CrdX(), lt.CrdY(), lt.CrdX(), rb.CrdY());

	setfillstyle(SOLID_FILL, t);
	bar(lt.CrdX(), lt.CrdY(), rb.CrdX(), lt.CrdY());

	setfillstyle(SOLID_FILL, r);
	bar(rb.CrdX(), lt.CrdY(), rb.CrdX(), rb.CrdY());

	setfillstyle(SOLID_FILL, b);
	bar(lt.CrdX(), rb.CrdY(), rb.CrdX(), rb.CrdY());
}

TButton::TButton(TPixel lt, TPixel rb) :
 TRectControl(lt, rb),
 state(Up), active(True)
{
}

int TButton::Update() // Updating button "state" and "in" variables
{
	if (!active) // if the button isn't active
		return False;

	// Storing values for change check
	TState previousState = state;

	TState m = Mouse.Left();

	if (In(Mouse)) // Mouse is "in" of the button
	{
		if (m & 1) // Mouse is "down"
		{
			if (state & 1) // Button is "down"
				state = Down;
			else
				state = Pressed; // Button is "up"
		}
		else // Mouse is "up"
		{
			if (state & 1) // Button is "down"
				state = Released;
			else
				state = Up; // Button is "up"
		}
	}
	else // Mouse is "out" of the button
		state = Up;

	// Update if "state" or "in" is changed
	return (previousState & 1) != (state & 1);
}

void TButton::Show()
{
	Fill(LIGHTGRAY);

	// Push effect
	if (state & 1)
		Draw(DARKGRAY, DARKGRAY, WHITE, WHITE);
	else
		Draw(WHITE, WHITE, DARKGRAY, DARKGRAY);
}

TStrButton::TStrButton(TPixel lt, TPixel rb, const char *s, const char *t):
TButton(lt, rb), strData(strdup(s))
{
	tag = t;
	initStrCoords();
}

TStrButton::~TStrButton()
{
	delete []strData;
}

// Shows the control (button + caption)
void TStrButton::Show()
{
	TButton::Show(); // Show the button

	settextstyle(SMALL_FONT, HORIZ_DIR, 4); // Default text style
	setcolor(active ? BLACK : DARKGRAY);
	outtextxy( // Show the caption
	 strCoords.CrdX() + (state & 1),
	 strCoords.CrdY() + (state & 1),
	 strData
	);
}

// Changes the caption
void TStrButton::Str(const char *s)
{
	delete []strData;
	strData = strdup(s);
	initStrCoords();
}

// Initializes caption coordinates
void TStrButton::initStrCoords()
{
	strCoords = TPixel(
	 (lt.CrdX() + rb.CrdX() - textwidth(strData) ) / 2,
	 (lt.CrdY() + rb.CrdY() - textheight(strData) ) / 2
	);
}

TPanel::TPanel(TPixel lt, TPixel rb, int style, int f, const char *t):
 TRectControl(lt, rb), Style(style > 0 ? 1 : style < 0 ? -1 : 0), fill(f)
{
	tag = t;
}

void TPanel::Show() // Shows the panel
{
	Fill(fill);

	switch (Style)
	{
		case 1:
			Draw(WHITE, WHITE, DARKGRAY, DARKGRAY);
			break;

		case -1:
			Draw(DARKGRAY, DARKGRAY, WHITE, WHITE);
			break;
	}
}

TLabel::TLabel(
 TPixel lt, TPixel rb,
 const char *s, int style, int f,
 const char *t
):
 TPanel(lt, rb, style, f),
 string(strdup(s)), changed(True)
{
	tag = t;
	Assert(string);
}

void TLabel::Show()
{
	TPanel::Show(); // Overloaded function

	setcolor(BLACK);
	settextstyle(SMALL_FONT, HORIZ_DIR, 4); // Default text style

	position = TextPos();
	outtextxy(position.CrdX(), position.CrdY(), string);

	changed = False;
}

void TLabel::operator = (const char *s) // Sets new label
{
	if (strcmp(string, s))
	{
		delete []string;
		Assert((string = strdup(s)) != NULL);
		changed = True;
	}
}

TCaption::TCaption(TPixel lt, TPixel rb, const char *str, const char *t):
 TLabel(lt, rb, str, -1, CYAN, t)
{
}

void TCaption::Show()
{
	TPanel::Show();

	settextstyle(SMALL_FONT, HORIZ_DIR, 10); // Text style

	position = TextPos();

	// 3D-effect text

	setcolor(BLACK);
	outtextxy(position.CrdX() + 1, position.CrdY() - 11, string);

	setcolor(WHITE);
	outtextxy(position.CrdX(),     position.CrdY() - 10, string);

	setcolor(BLACK);
	outtextxy(position.CrdX() - 1, position.CrdY() -  9, string);

	changed = False;
}
