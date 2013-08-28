/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * Project:     Glesly: GLES-based rendering library
 * Purpose:     A Geodesic Dome, based on an Icosahedron to show a surfaced sphere, e.g. the Earth
 * Author:      György Kövesdi (kgy@teledigit.eu)
 * Licence:     GPL (see file 'COPYING' in the project root for more details)
 * Comments:    
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <stdint.h>

#include "icosahedron-base.h"

using namespace Glesly;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
 *                                                                                       *
 *       Class IcosahedronBase:                                                          *
 *                                                                                       *
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/// Positions of the basic Icosahedron (X,Y,Z)
/*! Such an <a href="http://en.wikipedia.org/wiki/Icosahedron">Icosahedron</a> can be built from form three
 *  orthogonal golden rectangles (see the web page). These are the vertices of those rectangles. However,
 *  the displayed Icosahedron is built from triangles, not rectangles. This is just for simplicity.<br>
 *  There are 12 vertices.
 *  \see \ref basic_triangles   The triangle vertices of the basic Icosahedron
 *  */
const IcosahedronBase::Vec3 IcosahedronBase::basic_positions[NO_OF_VERTICES] = {
    // ----- X: -------- Y: -------- Z: ---------- Lat: -------- Lon: --------
    {       0.0f,       0.0f,       1.0f,       0.0f,           4.5f*lon_step   },  // Vertex =  0 (South Pole #1)
    {       0.0f,       0.0f,       1.0f,       0.0f,           3.5f*lon_step   },  // Vertex =  1 (South Pole #2)
    {       0.0f,       0.0f,       1.0f,       0.0f,           2.5f*lon_step   },  // Vertex =  2 (South Pole #3)
    {       0.0f,       0.0f,       1.0f,       0.0f,           1.5f*lon_step   },  // Vertex =  3 (South Pole #4)
    {       0.0f,       0.0f,       1.0f,       0.0f,           0.5f*lon_step   },  // Vertex =  4 (South Pole #5)

    {       0.0f,       p_1,        p_6,        lat_step,       5.0f*lon_step   },  // Vertex =  5
    {       p_3,        p_2,        p_6,        lat_step,       4.0f*lon_step   },  // Vertex =  6
    {       p_5,       -p_4,        p_6,        lat_step,       3.0f*lon_step   },  // Vertex =  7
    {      -p_5,       -p_4,        p_6,        lat_step,       2.0f*lon_step   },  // Vertex =  8
    {      -p_3,        p_2,        p_6,        lat_step,       1.0f*lon_step   },  // Vertex =  9
    {       0.0f,       p_1,        p_6,        lat_step,       0.0f            },  // Vertex = 10 (Mirror of 5)

    {       p_5,        p_4,       -p_6,        1.0f-lat_step,  4.5f*lon_step   },  // Vertex = 11
    {       p_3,       -p_2,       -p_6,        1.0f-lat_step,  3.5f*lon_step   },  // Vertex = 12
    {       0.0f,      -p_1,       -p_6,        1.0f-lat_step,  2.5f*lon_step   },  // Vertex = 13
    {      -p_3,       -p_2,       -p_6,        1.0f-lat_step,  1.5f*lon_step   },  // Vertex = 14
    {      -p_5,        p_4,       -p_6,        1.0f-lat_step,  0.5f*lon_step   },  // Vertex = 15
    {       p_5,        p_4,       -p_6,        1.0f-lat_step, -0.5f*lon_step   },  // Vertex = 16 (Mirror of 11)

    {       0.0f,       0.0f,      -1.0f,       1.0f,           4.0f*lon_step   },  // Vertex = 17 (North Pole #1)
    {       0.0f,       0.0f,      -1.0f,       1.0f,           3.0f*lon_step   },  // Vertex = 18 (North Pole #2)
    {       0.0f,       0.0f,      -1.0f,       1.0f,           2.0f*lon_step   },  // Vertex = 19 (North Pole #3)
    {       0.0f,       0.0f,      -1.0f,       1.0f,           1.0f*lon_step   },  // Vertex = 20 (North Pole #4)
    {       0.0f,       0.0f,      -1.0f,       1.0f,           0.0f*lon_step   }   // Vertex = 21 (North Pole #5)

};

/// The vertices of triangles to build an Icosahedron
/*! There are \ref NO_OF_TRIANGLES triangles.
 *  \note   These are indices to the array \ref basic_positions */
const IcosahedronBase::Triangle IcosahedronBase::basic_triangles[NO_OF_TRIANGLES] = {
    {   0,  5,  6   },
    {   1,  6,  7   },
    {   2,  7,  8   },
    {   3,  8,  9   },
    {   4,  9, 10   },
    {  17, 12, 11   },
    {  18, 13, 12   },
    {  19, 14, 13   },
    {  20, 15, 14   },
    {  21, 16, 15   },
    {   6, 12,  7   },
    {   7, 12, 13   },
    {   7, 13,  8   },
    {   8, 13, 14   },
    {   8, 14,  9   },
    {   9, 14, 15   },
    {   9, 15, 10   },
    {  10, 15, 16   },
    {   5, 11,  6   },
    {  11, 12,  6   }
};

void IcosahedronBase::Initialize(unsigned level)
{
 SYS_DEBUG_MEMBER(DM_GLESLY);

 TriangleDivider triangles(*this);

 for (int i = 0; i < NO_OF_VERTICES; ++i) {
    SYS_DEBUG(DL_INFO1, i << ": " << (int)(basic_positions[i].lon * 4096.0f + 0.5f) << ", " << (int)(basic_positions[i].lat * 2048.0f + 0.5f));
    triangles.RegisterVertex(i, basic_positions[i]);
 }

 for (int i = 0; i < NO_OF_TRIANGLES; ++i) {
    triangles.RegisterTriangle(level, basic_triangles[i]);
 }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
 *                                                                                       *
 *       Class IcosahedronBase::TriangleDivider:                                         *
 *                                                                                       *
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

IcosahedronBase::TriangleDivider::TriangleDivider(IcosahedronBase & parent):
    myParent(parent)
{
 SYS_DEBUG_MEMBER(DM_GLESLY);

}

IcosahedronBase::TriangleDivider::~TriangleDivider()
{
 SYS_DEBUG_MEMBER(DM_GLESLY);

 myParent.RegisterFinished();
}

void IcosahedronBase::TriangleDivider::RegisterVertex(int index, const Vec3 & vertex)
{
 SYS_DEBUG_MEMBER(DM_GLESLY);

 myParent.RegisterVertex(index, vertex.x, vertex.y, vertex.z, vertex.lon, vertex.lat);
}

void IcosahedronBase::TriangleDivider::RegisterTriangle(unsigned level, const Triangle & triangle)
{
 SYS_DEBUG_MEMBER(DM_GLESLY);

 myParent.RegisterTriangle(triangle.a, triangle.b, triangle.c);
}

/* * * * * * * * * * * * * End - of - File * * * * * * * * * * * * * * */
