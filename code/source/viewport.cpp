// ======================================================== //
//                                                          //
// Filename : VIEWPORT.CPP                                  //
// Content  : Source file of 3D viewport declaration        //
// Author   : Roman Zeyde                                   //
// Date     : 21/01/2000                                    //
//                                                          //
// ======================================================== //

#include <Viewport.h>

TViewport::TViewport(
 const TVector &wmin, const TVector &wmax, Real d,
 const TPixel &smin, const TPixel &smax
):

 worldMin(wmin), worldMax(wmax),
 screenMin(smin), screenMax(smax),

 worldWidth(worldMax.CrdX() - worldMin.CrdX()),
 worldHeight(worldMax.CrdY() - worldMin.CrdY()),

 screenWidth(screenMax.CrdX() - screenMin.CrdX()),
 screenHeight(screenMax.CrdY() - screenMin.CrdY()),

 pixelWidth(worldWidth / screenWidth),
 pixelHeight(worldHeight / screenHeight),

 projectionDistance(d),
 distance(0, 0, -d)

{
}
