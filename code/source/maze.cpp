// ======================================================== //
//                                                          //
// Filename : MAZE.CPP                                      //
// Content  : Source file of 3D maze classes                //
// Author   : Roman Zeyde                                   //
// Date     : 11/04/2000                                    //
//                                                          //
// ======================================================== //

#include <Maze.h>
#include <Movement.h>

TVertex &TMazeView::Vertex(TSize i, TSize j, TSize k)
{
	// Use the macro from MOVEMENT.H
	TSize index = MAP_3D(i,j,k,size+1);
	Assert(index < vertexes.Size());

	return vertexes[index];
}

#define BORDER DARKGRAY

// Overriding by using Polyhedron default constructor
TMazeView::TMazeView(TSize n, const char *path) :
 TPolyhedron(), size(n),
 Base(0.5, 0.5, 0.5),
 VecX(1.5, 0.5, 0.5),
 VecY(0.5, 1.5, 0.5),
 VecZ(0.5, 0.5, 1.5)
{
	vertexes.Resize(CUBE(size+1));

	TSize i, j, k, pos = 0;
	int wall, color;

	// Initializing vertexes
	for (i = 0; i <= size; i++)
		for (j = 0; j <= size; j++)
			for (k = 0; k <= size; k++)
			{
				vertexes[pos].Screen() = Viewport.Project(
				 vertexes[pos].World() = TVector(i, j, k)
				);

				pos++;
			}

	ifstream input(path); // Open walls file
	TList<TVertex *> VertexList; // Vertex list (for each polygon)

	TList<TSurface> SurfaceList; // Surface list

	Assert(input);

	// Reading X normal walls (same X coordinate)

	for (k = 0; k <= size; k++)
		for (i = 0; i < size; i++)
			for (j = 0; j < size; j++, pos++)
			{
				if (! (wall = (k == 0 || k == size)) ) // Not an edge wall
				{
					input >> wall; // Read from file
					color = BLUE + k - 1; // BLUE - CYAN
				}
				else
					color = LIGHTRED; // LIGHTRED edge wall

				VertexList.Clear()
				<< &Vertex(k,i,  j  )
				<< &Vertex(k,i,  j+1)
				<< &Vertex(k,i+1,j+1)
				<< &Vertex(k,i+1,j  );


				if (wall) // X
					SurfaceList << TSurface(VertexList, color, BORDER);
			}

	// Reading Y normal walls (same Y coordinate)

	for (k = 0; k <= size; k++)
		for (i = 0; i < size; i++)
			for (j = 0; j < size; j++, pos++)
			{
				if (! (wall = (k == 0 || k == size)) ) // Not an edge wall
				{
					input >> wall; // Read from file
					color = RED + k - 1; // RED - BROWN
				}
				else
					color = LIGHTMAGENTA; // LIGHTMAGENTA edge wall

				VertexList.Clear()
				<< &Vertex(i,  k,j  )
				<< &Vertex(i,  k,j+1)
				<< &Vertex(i+1,k,j+1)
				<< &Vertex(i+1,k,j  );

				if (wall) // Y
					SurfaceList << TSurface(VertexList, color, BORDER);
			}

	// Reading Z normal walls (same Z coordinate)

	for (k = 0; k <= size; k++)
		for (i = 0; i < size; i++)
			for (j = 0; j < size; j++, pos++)
			{
				if (! (wall = (k == 0 || k == size)) ) // Not an edge wall
				{
					input >> wall; // Read from file
					color = LIGHTBLUE + k - 1;  // LIGHTBLUE - LIGHTCYAN
				}
				else
					color = YELLOW; // YELLOW edge wall

				VertexList.Clear()
				<< &Vertex(i,  j,  k)
				<< &Vertex(i,  j+1,k)
				<< &Vertex(i+1,j+1,k)
				<< &Vertex(i+1,j,  k);

				if (wall) // Z
					SurfaceList << TSurface(VertexList, color, BORDER);
			}

	// Updating surfaces array
	surfaces.Resize(SurfaceList.Count());
	pos = 0;

	for (TListPos<TSurface> lp(SurfaceList); ++lp; ++pos)
		surfaces[pos] = lp.Current();

	SurfaceList.Clear();
}