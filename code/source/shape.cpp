// ======================================================== //
//                                                          //
// Filename : SHAPE.CPP                                     //
// Content  : Source file of 3D shape classes declaration   //
// Author   : Roman Zeyde                                   //
// Date     : 08/04/2000                                    //
//                                                          //
// ======================================================== //

#include <shape.h>

// Creates a surface from list of points, fill and border colors
TSurface::TSurface(const TList<TVertex *> &list, Byte f, Byte b) :
 TArray<TVertex *>(list.Count()),
 fill(f),
 border(b)
{
	TListPos<TVertex *> p(list, True);
	TSize i;

	for (i = 0; ++p; ++i)
		operator[](i) = p.Current();
}

// Calculates "middle-point" depth value
Real TSurface::Middle()
{
	TVector v;

	for (TSize i = 0; i < Size(); i++)
		v += operator[](i)->World();

	return (v / Size() - Viewport.Distance()).Length();
}

// Shows the shape on the viewport
void TSurface::Show()
{
	if (!operator int()) // Exit if the surface is not defined (from TArray)
		return;

	TPixel *p = new TPixel[2*Size()]; // Clipping array
	Assert(p);

	static const TVertex *v1, *v2; // Iterators

	//
	// Clipping the surface to the array:
	// 1. First vertex of edge is added if (Z > 0).
	// 2. Edge is checked for clipping with (Z = 0).
	//    If it does, the clipping point is added.
	//

	v2 = operator[](0);

	TSize n = 0;
	for (TSize i = 0; i < Size();)
	{
		v1 = v2;
		v2 = operator[]( ++i%Size() );

		if (v1->World().CrdZ() >= 0) // (1)
			p[n++] = v1->Screen();

		if ( // (2)
		 v1->World().CrdZ() > 0 && v2->World().CrdZ() < 0 ||
		 v1->World().CrdZ() < 0 && v2->World().CrdZ() > 0
		)
			p[n++] = Viewport.Project(	LineClip(v1->World(), v2->World()) );
	}

	setcolor(border);
	setfillstyle(SOLID_FILL, fill);

	if (n) // If there is any point in the array
		fillpoly(n, (int *)p);

	delete []p;
}

// Data file constructor (text format)
//
// Data format:
// -----------

// [NumOfPoints]
// { [X] [Y] [Z] } x NumOfPoints

// [NumOfPolygons]
// {
// 	[NumOfVertexes] { [Index] } x NumOfVertexes
//    [Fill] [Border]
// } x NumOfPolygons

// Notes:
// [X] - real value (3.5, -1.04, 8)
// [NumOf...], [Index], [Fill, Color] - unsigned value (5, 19, 0)
// { ... } x N - repeat ... N times

TPolyhedron::TPolyhedron(const char *path) // Data file constructor
{
	ifstream in(path);
	Assert(in); // Check file

	TSize number, index;
	Real x, y, z;

	Assert(in >> number); // Check number of vertexes
	vertexes.Resize(number); // Create number-sized array

	for (index = 0; index < number; index++) // For every vertex
	{
		Assert(in >> x >> y >> z); // Read coordinates
		vertexes[index].World() = TVector(x, y, z); // Update vertex array
	}

	Assert(in >> number); // Check number of surfaces
	surfaces.Resize(number); // Create number-sized array

	for (index = 0; index < number; index++) // For every surface
		surfaces[index] = ReadSurface(in);
}

// Reading a surface from ifstream
TSurface TPolyhedron::ReadSurface(ifstream &in)
{
	TList<TVertex *> list;
	TSize number, index, vertex;

	TSize fill, border;

	Assert(in >> number); // Check number of vertexes in a surface

	for (index = 0; index < number; index++)
	{
		Assert(in >> vertex);
		list << &vertexes[vertex];
	}

	Assert(in >> fill >> border); // Check fill and border colors

	return TSurface(list, fill, border); // Create new surface
}

// Used for comparing surfaces
enum { Bigger = -1, Equal = 0, Less = 1 };

int CompareFunc(const void *p1, const void *p2)
{
	static Real d;

	d = ((TSurface *) p1)->Middle() - ((TSurface *)p2)->Middle();

	return
		d > 0 ?
			Bigger :
		d < 0 ?
			Less :
//		d == 0 ?
			Equal;
}

void TPolyhedron::operator *= (const TMatrix &m)
{
	for (TSize i = 0; i < vertexes.Size(); i++)
	{
		vertexes[i].Perform(m);
		vertexes[i].Project();
	}
}

// Default constructor
TViewer::TViewer() : TList<TPolyhedron *>(), surfaces(0)
{
}

// Updates the array with the surfaces addresses
void TViewer::Update()
{
	TListPos<TPolyhedron *> p(*this);

	// n - Total number of surfaces
	TSize n = 0;
	for (; ++p; n += p.Current()->Surfaces().Size() );

	surfaces.Resize(n); // Resize the array

	for (p.Reset(), n = 0; ++p;) // Copy the surfaces addresses
		for (TSize i = 0; i < p.Current()->Surfaces().Size(); i++)
			surfaces[n++] = &p.Current()->Surfaces()[i];
}

// Comparing between 2 pointers to array of pointers to surfaces
int Compare(const void *p1, const void *p2)
{
	static Real d;

	d = (* (TSurface **) p1)->Middle() - (* (TSurface **) p2)->Middle();

	return
		d > 0 ?
			Bigger :
		d < 0 ?
			Less :
//		d == 0 ?
			Equal;
}

// Calling QuickSort for surfaces array
void TViewer::Sort()
{
	qsort(&surfaces[0], surfaces.Size(), sizeof(surfaces[0]), Compare);
	// Parameters:
	// 1. Address of first element
	// 2. Number of elements
	// 3. Size of one element
	// 4. Comparing function
}

// Showing the polygons on the viewport
void TViewer::Show()
{
	for (TSize i = 0; i < surfaces.Size(); i++)
		surfaces[i]->Show();
}
