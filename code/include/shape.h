// ======================================================== //
//                                                          //
// Filename : SHAPE.H                                       //
// Content  : Header file of 3D shape classes declaration   //
// Author   : Roman Zeyde                                   //
// Date     : 07/04/2000                                    //
//                                                          //
// ======================================================== //

#ifndef __SHAPE_H
#define __SHAPE_H

#include <vector.h>
#include <matrix.h>
#include <viewport.h>
#include <fstream.h>

#include <tarray.h>
#include <tlist.h>

//
// Vertex is world and screen coordinate class, combines surfaces
//

class TVertex
{

public:

	TVertex() : v(), p() {}

	TVector &World() { return v; }
	const TVector &World() const { return v; }

	TPixel &Screen() { return p; }
	const TPixel &Screen() const { return p; }

	// Performs transformation on the vertex
	void Perform(const TMatrix &m);

	// Projects the vertex on the viewport
	void Project();

protected:

	TVector v;
	TPixel p;

};

// Performs transformation on the vertex
inline
void TVertex::Perform(const TMatrix &m)
{
	v *= m;
}

// Projects the vertex on the viewport
inline
void TVertex::Project()
{
	p = Viewport.Project(v);
}

// Surface is an 3D polygon that contains pointers to its vertexes
class TSurface : public TArray<TVertex *>
{

public:

	TSurface() : // Empty construcor
	 TArray<TVertex *>(0),
	 fill(BLACK),
	 border(BLACK)
	{
	}

	// Creates a surface from list of points, fill and border colors
	TSurface(const TList<TVertex *> &list, Byte f, Byte b);

	~TSurface() {}

	Real Middle(); // Calculates "middle-point" depth value

	void Show(); // Shows the shape on the viewport

protected:

	Byte fill, border;

};

// Clipping [a, b] line over (z = 0) plane
inline
TVector LineClip(const TVector &a, const TVector &b)
{
	return (a * fabs(b.CrdZ()) + b * fabs(a.CrdZ()) ) /
			 (    fabs(a.CrdZ()) +     fabs(b.CrdZ()) );
}

// Polyhedron is 3D body that consists of polygons (surfaces)
class TPolyhedron
{

public:

	TPolyhedron(const char *path); // Data file constructor (text format)

	~TPolyhedron() {} // Destructor

	// Returns reference to data members
	TArray<TVertex> &Vertexes() { return vertexes; }
	TArray<TSurface> &Surfaces() { return surfaces; }

	// Returns const reference to data members
	const TArray<TVertex> &Vertexes() const { return vertexes; }
	const TArray<TSurface> &Surfaces() const { return surfaces; }

	// Transforming polyhedron by matrix
	void operator *= (const TMatrix &m);

	// Projects polyhedron points on viewport
	void Project();

protected:

	TPolyhedron() {} // Empty contructor (used only in derived classes)

	TArray<TVertex> vertexes; // Vertexes
	TArray<TSurface> surfaces; // Surfaces

private:

	TSurface ReadSurface(ifstream &in); // Reading a surface from ifstream

};

// Viewer is used to show correctly 3D polyhedrons
class TViewer : public TList<TPolyhedron *>
{

public:

	TViewer(); // Default constructor

	void Update(); // Updates surface array from polyhedron list

	void Sort(); // Sorts the surfaces (using QuickSort)
	void Show(); // Showing the polygons on the viewport

protected:

	TArray<TSurface *> surfaces; // Array of pointers to surfaces
};

#endif // __SHAPE_H
