// ======================================================== //
//                                                          //
// Filename : MAZE.H                                        //
// Content  : Header file of the game menu                  //
// Author   : Roman Zeyde                                   //
// Date     : 23/04/2000                                    //
//                                                          //
// ======================================================== //

#ifndef __MENU_H
#define __MENU_H

#include <TList.h>
#include <TArray.h>
#include <TString.h>
#include <Controls.h>

#include <Game.h>

class THiscore
{

public:

	THiscore() : name(), score(0) {}
	THiscore(const TString &n, TSize s) : name(n), score(s) {}

	int operator <  (const THiscore &h) const { return score <  h.score; }
	int operator >  (const THiscore &h) const { return score >  h.score; }
	int operator == (const THiscore &h) const { return score == h.score; }
	int operator != (const THiscore &h) const { return score != h.score; }
	int operator <= (const THiscore &h) const { return score <= h.score; }
	int operator >= (const THiscore &h) const { return score >= h.score; }

	const char *Name() { return name; }
	TSize Score() { return score; }

protected:

	TString name;
	TSize score;

};

typedef TListPos<THiscore> THiscoreListPos;

class THiscoreList : public TList<THiscore>
{

public:

	THiscoreList(TSize n) : size(n) {}

	void Insert(const THiscore &h)
	{
		Place(h).Add(h, False); // Before place

		if (Count() > size)
			RemoveFromTail();
	}

	int isRecord(const THiscore &h) // Is h a record to add?
	{
		return Count() < size || Place(h);
	}

protected:

	TSize size;

	THiscoreListPos Place(const THiscore &h);

};

class TTable : public TPanel
{

public:

	TTable(TPixel lt, TPixel rb, THiscoreList &hs):
	 TPanel(
	  lt, rb, -1, WHITE,
	  "Hiscore table: player name - hiscore time (seconds)"
	 ),
	 hiscores(hs)
	{
	}

	virtual void Show();

protected:

	THiscoreList &hiscores;

};

class TMenu
{

public:

	TMenu();
	~TMenu() {}

	int Run(); // 0 - OK, 1 - no mazes

protected:

	TSize ReadMazes(); // returns the number of mazes
	void UpdateHiscores();

	const char *MazeName(TSize i);

	TString &HiscoresName();

	void UpdateInfo();

	TPanel panel, maze_panel;
	TCaption caption;

	TStrButton next, prev;
	TLabel maze_name, position;

	TString hiscores_name;

	TStrButton load, done;

	TTable table;
	THiscoreList hiscores;

	TLabel status;

	TControls controls;

	TArray<TString> mazes;
	TSize index;

};

inline
const char *TMenu::MazeName(TSize i)
{
	return mazes[i];
}

const TSize MaxName = 20;

class TInput
{

public:

	TInput();
	~TInput() {}

	TString Run();

protected:

	void UpdateInfo();

	TString toString(char c);

	int Add(char c) // returns True if c was added, else returns False
	{
		if (data.Length() < MaxName)
		{
			data += toString(c);
			return True;
		}
		else
			return False;
	}

	int Remove()
	{
		if (data.Length() > 0)
		{
			data[data.Length()-1] = '\0'; // Delete last char
			return True;
		}
		else
			return False;
	}


	TString data;

	TControls controls;

	TPanel panel;
	TLabel congratulations, prompt, text;

	TStrButton done;

};

#endif // __MENU_H