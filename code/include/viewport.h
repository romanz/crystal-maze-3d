// ======================================================== //
//                                                          //
// Filename : VIEWPORT.H                                    //
// Content  : Header file of 3D viewport declaration        //
// Author   : Roman Zeyde                                   //
// Date     : 21/01/2000                                    //
//                                                          //
// ======================================================== //

#ifndef __VIEWPORT_H
#define __VIEWPORT_H

#include <Defines.h>
#include <Matrix.h>
#include <Vector.h>
#include <Pixel.h>

#include <Graphics.h>

// TViewport projects World coords to Screen coords
// using perspective projection from (0,0,-d)

class TViewport
{

public:

	TViewport( // Min/Max Coords of Screen & World borders
	 const TVector &wmin, const TVector &wmax, Real d, // for perspective
	 const TPixel &smin, const TPixel &smax
	);

	TPixel Project(const TVector &p) const; // Projection function

	Real WorldLeft()   const { return worldMin.CrdX();  }
	Real WorldBottom() const { return worldMin.CrdY();  }
	Real WorldRight()  const { return worldMax.CrdX();  }
	Real WorldTop()    const { return worldMax.CrdY();  }

	Real WorldWidth()  const { return worldWidth;       }
	Real WorldHeight() const { return worldHeight;      }

	int ScreenLeft()   const { return screenMin.CrdX(); }
	int ScreenTop()    const { return screenMin.CrdY(); }
	int ScreenRight()  const { return screenMax.CrdX(); }
	int ScreenBottom() const { return screenMax.CrdY(); }

	int ScreenWidth()  const { return screenWidth;      }
	int ScreenHeight() const { return screenHeight;     }

	Real PixelWidth()  const { return pixelWidth;       }
	Real PixelHeight() const { return pixelHeight;      }

	// Projection distance vector
	const TVector &Distance() const { return distance;  }

	void Set() // Viewport becomes active
	{
		setviewport(
		 screenMin.CrdX(), screenMin.CrdY(),
		 screenMax.CrdX(), screenMax.CrdY(),
		 True
		);
	}

	void Clear() // Viewport is cleared
	{
		clearviewport();
	}

protected:

	TVector worldMin, worldMax;
	Real worldWidth, worldHeight;

	TPixel screenMin, screenMax;
	int screenWidth, screenHeight;

	Real pixelWidth, pixelHeight; // (World) / (Screen)

	Real projectionDistance; // Projection distance value
	TVector distance; // Projection distance vector

};

extern TViewport Viewport; // At least one viewport has to be used

inline
TPixel TViewport::Project(const TVector &p) const // Projection function
{
	// w = z / d + 1 = Perspective factor for perspective effect
	// if w is negative, w = 1 to enforce normal projection
	Real w = p.CrdZ() >= 0 ? p.CrdZ() / projectionDistance + 1 : 1;

	// Projected point = Original point / w

	// ScreenX = (ProjectedX - WorldLeft) / WorldWidth * ScreenWidth
	// ScreenY = (WorldTop - ProjectedY) / WorldHeight * ScreenHeight

	// (Screen Y is opposite to World Y)

	return TPixel
	(
		(p.CrdX() / w - worldMin.CrdX()) / pixelWidth,
		(worldMax.CrdY() - p.CrdY() / w)  / pixelHeight
	);
}

#endif