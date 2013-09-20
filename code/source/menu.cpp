// ======================================================== //
//                                                          //
// Filename : MENU.CPP                                      //
// Content  : Source file of game menu                      //
// Author   : Roman Zeyde                                   //
// Date     : 23/04/2000                                    //
//                                                          //
// ======================================================== //

#include <App.h>
#include <Menu.h>
#include <ASCII.h>

#include <Dir.h>
#include <Dos.h>

#include <Alloc.h>
#include <Conio.h>
#include <CType.h>
#include <FStream.h>
#include <StrStrea.h>

#include <Screen.h>
#include <Viewport.h>

// Maze viewport
TViewport Viewport(

 // World Min(x,y), Max(x,y), Dist(z)
 TVector(-0.4, -0.3), TVector( 0.4,  0.3), 1.0,

 // Screen Min(x,y), Max(x,y)
 TPixel(  0.3 * Screen.Width(), 0.3 * Screen.Height() ),
 TPixel(  0.7 * Screen.Width(), 0.7 * Screen.Height() )

);

THiscoreListPos THiscoreList::Place(const THiscore &h)
{
	for (THiscoreListPos p(*this); ++p && h > p.Current(); );
	return p;
}

void TTable::Show()
{
	TPanel::Show();

	char str[80];

	int i = -1;
	TListPos<THiscore> p(hiscores);

	for (++p ; p || i <= 0; ++i)
	{
		if (i < 0)
		{
			ostrstream(str, sizeof(str)) <<   "Top 10 players" << ends;
		}
		else if (!i)
		{
			ostrstream(str, sizeof(str)) << "------------------" << ends;
		}
		else
		{
			THiscore &h = p.Current();
			++p;

			ostrstream(str, sizeof(str))
			 << "(" << i << ") "
			 << h.Name() << " - "  << h.Score()
			 << ends;
		}

		setcolor((i+1)%8);
		outtextxy(
		 ( lt.CrdX() + rb.CrdX() - textwidth(str) ) / 2,
			lt.CrdY() + 2 + (i + 1) * 10,
			str
		);
	}
}

TSize TMenu::ReadMazes()
{
	ffblk f;
	TList<TString> list;

	for (int done = findfirst("*.MAZ", &f, 0); !done; done = findnext(&f))
		list << TString(f.ff_name);

	TSize i = 0;

	mazes.Resize(list.Count());

	for (
	 TListPos<TString> p(list);
	 ++p;
	 mazes[i++] = p.Current()
	);

	return mazes.Size();
}

TMenu::TMenu():

 panel(
  Screen.Pixel(0,0), Screen.Pixel(100,100),
  0, LIGHTGRAY, "Point any control to get instant help"
 ),

 caption(
  Screen.Pixel( 1, 5), Screen.Pixel(99,25),
  "Crystal Maze 3D 2000"
 ),

 maze_panel(
  Screen.Pixel(2,31), Screen.Pixel(48,70),
  -1, LIGHTGRAY, "Maze selection"
 ),

 next(
  Screen.Pixel(26,50), Screen.Pixel(45,55),
  "Next", "Switch to next maze"
 ),

 prev(
  Screen.Pixel( 5,50), Screen.Pixel(24,55),
  "Previous", "Switch to previous maze"
 ),

 maze_name(
  Screen.Pixel( 5,35), Screen.Pixel(45,45),
  "", -1, LIGHTGREEN, "Maze file name"
 ),

 position(
  Screen.Pixel( 5,57), Screen.Pixel(45,62),
  "", -1, LIGHTBLUE, "Current maze number out of total"
 ),

 load(
  Screen.Pixel( 5,75), Screen.Pixel(45,85),
  "Play", "Plays current maze"
 ),

 done(
  Screen.Pixel(55,75), Screen.Pixel(95,85),
  "Exit", "Exits the program"
 ),

 status(Screen.Pixel(0,95), Screen.Pixel(100,100), "", -1, LIGHTGRAY),

 hiscores(10), // Top 10 hiscores

 table(Screen.Pixel(52,31), Screen.Pixel(98,70), hiscores),

 index(0)
{
	ReadMazes();

	controls
	 << panel << caption << maze_panel
	 << next << prev << maze_name << position
	 << load << done << table << status
	;
}

const TSize Delay = 13;

int TMenu::Run()
{
	if (!mazes.Size())
		return 1;

	for (int to_update = False; ; )
	{
		Mouse.Update();

		if (kbhit())
			switch (getch())
			{
				case Escape:
					return 0;

				case 0:
					switch (getch())
					{
					}
			}

		status = controls.Tag();

		if (controls.Update() || to_update)
		{
			to_update = False;

			if (next.Click())
				index = index == mazes.Size() - 1 ? 0 : index + 1;

			if (prev.Click())
				index = index == 0 ? mazes.Size() - 1 : index - 1;

			UpdateInfo();
			UpdateHiscores(); // Update hiscore list

			controls.Show(); // Shows the controls

			Mouse.Hide(); // Hide mouse

			Screen.Switch(Delay); // Switch pages
			Mouse.Page(Screen.Visual()); // Switch mouse

			Mouse.Show(); // Show mouse
		}

		if (done.Click())
			return 0;

		if (load.Click())
		{
			TSize t = App(MazeName(index)).Run();

			if ( t && hiscores.isRecord(THiscore("", t)) )
				hiscores.Insert(THiscore(TInput().Run(), t));

			to_update = True;
		}
	}
}

void TMenu::UpdateInfo()
{
	char str[80];

	ostrstream(str, sizeof(str))
	 << "Maze file name: "
	 << MazeName(index)
	 << ends;

	maze_name = str;

	ostrstream(str, sizeof(str))
	 << "Maze index: "
	 << index + 1 << " of " << mazes.Size()
//	  << " " << coreleft()
	 << ends;

	position = str;
}

inline
TString &TMenu::HiscoresName()
{
	char *s = strdup(MazeName(index));
	Assert(s); // Check for NULL

	strcpy(strrchr(s, '.'), ".HI"); // Replace ".MAZ" by ".HI"

	hiscores_name = s;

	delete []s;

	return hiscores_name;
}

void TMenu::UpdateHiscores()
{
	fstream f;

	// Write old hiscores to file
	f.open(hiscores_name, ios::out);

	for (THiscoreListPos p(hiscores); ++p; )
	{
		THiscore &h = p.Current();
		f << h.Name() << " " << h.Score() << "\n";
	}

	f.close();

	HiscoresName();

	char name[80] = "";
	TSize score = 0;

	// Read new hiscores from file
	f.open(hiscores_name, ios::in);

	for (hiscores.Clear(); f; )
		if (f >> name >> score)
			hiscores.Insert(THiscore(name, score));

	f.close();
}

TInput::TInput():

 panel(
  Screen.Pixel(25, 30), Screen.Pixel(75, 65),
  1, LIGHTGRAY
 ),

 congratulations(
  Screen.Pixel(26, 31), Screen.Pixel(74, 36),
  "Congratulations - You\'ve set a record", -1, LIGHTBLUE
 ),

 prompt(
  Screen.Pixel(31, 40), Screen.Pixel(69, 45),
  "Please enter your name:", 0, LIGHTGRAY
 ),

 text(
  Screen.Pixel(31, 46), Screen.Pixel(69, 51),
  "", -1, WHITE
 ),

 done(
  Screen.Pixel(40, 56), Screen.Pixel(60, 61),
  "OK"
 )

{
	controls << panel << congratulations << prompt << text << done;
}

TString TInput::toString(char c)
{
	char s[2] = { c, '\0' };
	return TString(s);
}

TString TInput::Run()
{
	char c = ' ';
	for (int to_update = False; ; )
	{
		Mouse.Update();

		if (kbhit())
			switch (c = getch())
			{
				case Enter:
				case Escape:

					if (data.Length())
						return data;
					else
						return TString("Anonymous");

				case 0:
					switch (c = getch())
					{
					}
					break;

				case BackSpace:
					to_update = Remove();
					break;

				default:
					if (isalnum(c))
						to_update = Add(c);
			}

		if (controls.Update() || to_update)
		{
			to_update = False;

			UpdateInfo();
			controls.Show(); // Shows the controls

			Mouse.Hide(); // Hide mouse

			Screen.Switch(Delay); // Switch pages
			Mouse.Page(Screen.Visual()); // Switch mouse

			Mouse.Show(); // Show mouse
		}

		if (done.Click())
		{
			if (data.Length())
				return data;
			else
				return TString("Anonymous");
		}
	}
}

void TInput::UpdateInfo()
{
	text = data + toString('|');
}
