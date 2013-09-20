// ======================================================== //
//                                                          //
// Filename : TLIST.H                                       //
// Content  : Header file of templated double linked list   //
// Author   : Roman Zeyde                                   //
// Date     : 21/04/2000                                    //
//                                                          //
// ======================================================== //

#ifndef __TLIST_H
#define __TLIST_H

#include <StdLib.h>

#include <Defines.h>

// Class defines

template <class T>
class TListNode;

template <class T>
class TListPos;

template <class T>
class TList;

// List node template contains the data and the pointers

template <class T>
class TListNode
{

public:

	TListNode() : next(this), prev(this) {} // Poits to itself
	virtual ~TListNode() {}

	friend TListPos<T>;
	friend TList<T>;

protected:

	T data; // Data of T type
	TListNode<T> *next, *prev; // next and previous nodes

};

// List position template is used to handle the nodes

template <class T>
class TListPos
{

public:

	// Start from head/tail
	TListPos(const TList<T> &l, int atHead = True);

	// Start from other TListPos<T>
	TListPos(const TListPos<T> &p);

	// Destructor
	virtual ~TListPos() {}

	// Restart from head/tail
	void Reset(int atHead = True);

	// Returns data - variable version
	T &Current() { return node->data; }

	// Returns data - const version
	const T &Current() const { return node->data; }

	// Assigns the position of given TListPos<T>
	void operator = (const TListPos<T> &p);

	// Comparing 2 TListPos<T> classes
	int operator == (const TListPos<T> &p) { return node == p.node; }
	int operator != (const TListPos<T> &p) { return node != p.node; }

	operator int() const; // Is the position valid?

	TListPos<T> &operator ++(); // Next position
	TListPos<T> &operator --(); // Previous position

	// Adds new element after/before the position
	void Add(const T &x, int addAfter = True);

	// Removes the current element and moves after/before it
	void Remove(int moveAfter = True);

protected:

	const TList<T> *list; // Pointer to given list
	TListNode<T> *node; // Pointer to current node

};

// List contructor
template <class T>
TListPos<T>::TListPos(const TList<T> &l, int atHead) : list(&l), node(NULL)
{
	Reset(atHead);
}

// Copy contructor
template <class T>
TListPos<T>::TListPos(const TListPos<T> &p) : list(p.list), node(p.node)
{
}

// Restart from head/tail
template <class T>
inline
void TListPos<T>::Reset(int atHead)
{
	node = atHead ? list->head.prev : list->tail.next;
}

// Assigns the position of given TListPos<T>
template <class T>
inline
void TListPos<T>::operator = (const TListPos<T> &p)
{
	node = p.node;
	list = p.list;
}

// Is the position valid?
// Note: only the head and the tail nodes have self-pointing
// pointers so they are the only non-valid positions
template <class T>
inline
TListPos<T>::operator int()
{
	return (node->prev != node) && (node->next != node);
}

// Next position
template <class T>
inline
TListPos<T> &TListPos<T>::operator ++()
{
	node = node->next;
	return *this;
}

// Previous position
template <class T>
inline
TListPos<T> &TListPos<T>::operator --()
{
	node = node->prev;
	return *this;
}

// Adds new element after/before the position
template <class T>
void TListPos<T>::Add(const T &x, int addAfter)
{
	TListNode<T> *beforeNode = node, *afterNode = node; // Neighbour nodes

	// Get actual after and before nodes
	if (addAfter)
		afterNode = afterNode->next;
	else
		beforeNode = beforeNode->prev;

	if (beforeNode != afterNode) // Can add the node between the two nodes
	{
		// Create new node for x
		TListNode<T> *newNode = new TListNode<T>;

		// Check memory allocation
		Assert(newNode);

		// Set new node data
		newNode->data = x;
		newNode->prev = beforeNode;
		newNode->next = afterNode;

		// Connect after and before nodes to new node
		afterNode->prev = newNode;
		beforeNode->next = newNode;
	}
}

// Removes the current element and moves after/before it
template <class T>
void TListPos<T>::Remove(int moveAfter)
{
	// Connect after and before nodes and detach the node from the list
	TListNode<T> *beforeNode = node->prev, *afterNode = node->next;

	if (operator int()) // The node is neither head nor tail
	{
		beforeNode->next = afterNode;
		afterNode->prev = beforeNode;

		delete node;

		node = (moveAfter) ? afterNode : beforeNode; // Update position
	}
}

// Double linked list template

template <class T>
class TList
{

public:

	// Default contructor
	TList() : head(), tail() { initList(); }

	// Copy contructor
	TList(const TList<T> &l) : head(), tail()
	{
		initList();
		operator = (l);
	}

	// Destructor
	virtual ~TList() { Clear(); }

	// Special Add() frunctions
	void AddToHead(const T &x);
	void AddToTail(const T &x);

	// This operator is used to allow statements like:
	// list << a << b << c;
	TList<T> &operator << (const T &x);

	// Special Remove() fnuctions
	void RemoveFromHead();
	void RemoveFromTail();

	// Assignment operator
	void operator = (const TList<T> &l);

	// Removes all elements from the list
	TList<T> &Clear();

	operator int() { return head.next != &tail; }
	// Is the list not empty?
	// It can be also checked as (tail.prev != &head)

	friend TListPos<T>;

	TSize Count() const; // Counts list's elements

protected:

	TListNode<T> head, tail;
	// It's a waste of space but it's easier
	// to implement Add() and Remove() methods.

private:

	void initList() // Connect head to tail
	{
		head.next = &tail;
		tail.prev = &head;
	}

};

template <class T>
inline
void TList<T>::AddToHead(const T &x)
{
	(TListPos<T>(*this, True)).Add(x, True); // Add after head
}

template <class T>
inline
void TList<T>::AddToTail(const T &x)
{
	(TListPos<T>(*this, False)).Add(x, False); // Add before tail
}

template <class T>
inline
TList<T> &TList<T>::operator << (const T &x)
{
	AddToTail(x); // Use AddToTail() to preserve order
	return *this;
}

template <class T>
inline
void TList<T>::RemoveFromHead()
{
	(++TListPos<T>(*this, True)).Remove(); // Remove node after head
}

template <class T>
inline
void TList<T>::RemoveFromTail()
{
	(--TListPos<T>(*this, False)).Remove(); // Remove node before tail
}

template <class T>
void TList<T>::operator = (const TList<T> &l)
{
	if (this != &l)
	{
		Clear(); // Clear the list before copy

		TListPos<T> i(l, True);

		for (++i; i; ++i)
			AddToTail( i.Current() ); // Copy every object to this list
	}
}

template <class T>
TList<T> &TList<T>::Clear()
{
	// Removes all the objects until the list is empty
	for (; operator int(); RemoveFromHead());

	return *this;
}

template <class T>
TSize TList<T>::Count() const
{
	TSize n = 0;
	TListPos<T> p(*this, True);

	for (++p; p; ++n, ++p);

	return n;
}

#endif
