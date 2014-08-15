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

/// Positions of the basic Icosahedron
/*! The <b>x,y,z</b> are the real 3D positions.
 *  Such an Icosahedron is built from 12 vertices.
 *  \see \ref basic_triangles   Builds the triangles of the basic Icosahedron.
 *  */
const IcosahedronBase::Vec3 IcosahedronBase::basic_positions[NO_OF_VERTICES] = {
    // ----- X: -------- Y: -------- Z: ----
    {       0.0f,       0.0f,       1.0f    },  // Vertex  0 (South Pole #1)
    {       0.0f,       0.0f,       1.0f    },  // Vertex  1 (South Pole #2)
    {       0.0f,       0.0f,       1.0f    },  // Vertex  2 (South Pole #3)
    {       0.0f,       0.0f,       1.0f    },  // Vertex  3 (South Pole #4)
    {       0.0f,       0.0f,       1.0f    },  // Vertex  4 (South Pole #5)

    {       0.0f,       p_1,        p_6     },  // Vertex  5
    {       p_3,        p_2,        p_6     },  // Vertex  6
    {       p_5,       -p_4,        p_6     },  // Vertex  7
    {      -p_5,       -p_4,        p_6     },  // Vertex  8
    {      -p_3,        p_2,        p_6     },  // Vertex  9
    {       0.0f,       p_1,        p_6     },  // Vertex 10 (Mirror of 5)

    {       p_5,        p_4,       -p_6     },  // Vertex 11
    {       p_3,       -p_2,       -p_6     },  // Vertex 12
    {       0.0f,      -p_1,       -p_6     },  // Vertex 13
    {      -p_3,       -p_2,       -p_6     },  // Vertex 14
    {      -p_5,        p_4,       -p_6     },  // Vertex 15
    {       p_5,        p_4,       -p_6     },  // Vertex 16 (Mirror of 11)

    {       0.0f,       0.0f,      -1.0f    },  // Vertex 17 (North Pole #1)
    {       0.0f,       0.0f,      -1.0f    },  // Vertex 18 (North Pole #2)
    {       0.0f,       0.0f,      -1.0f    },  // Vertex 19 (North Pole #3)
    {       0.0f,       0.0f,      -1.0f    },  // Vertex 20 (North Pole #4)
    {       0.0f,       0.0f,      -1.0f    }   // Vertex 21 (North Pole #5)

};

/// The triangles to build an Icosahedron
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
    SYS_DEBUG(DL_INFO1, "Vertex " << i);
    Vec3 pos = {
        basic_positions[i].x * mySize,
        basic_positions[i].y * mySize,
        basic_positions[i].z * mySize
    };
    triangles.RegisterVertex(pos);
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

unsigned IcosahedronBase::TriangleDivider::RegisterVertex(const Vec3 & vertex)
{
 SYS_DEBUG_MEMBER(DM_GLESLY);

 return myParent.RegisterVertex(vertex);
}

void IcosahedronBase::TriangleDivider::RegisterTriangle(unsigned level, const Triangle & triangle)
{
 SYS_DEBUG_MEMBER(DM_GLESLY);

 if (level == 0U) {
    myParent.RegisterTriangle(triangle);
    // Nothing more to do:
    return;
 }

 // Go down one level:
 --level;

 GLushort vertex_ab = VertexInterpolate(triangle.a, triangle.b);
 GLushort vertex_bc = VertexInterpolate(triangle.b, triangle.c);
 GLushort vertex_ac = VertexInterpolate(triangle.a, triangle.c);

 RegisterTriangle(level, triangle.a, vertex_ab, vertex_ac);
 RegisterTriangle(level, triangle.b, vertex_bc, vertex_ab);
 RegisterTriangle(level, triangle.c, vertex_ac, vertex_bc);
 RegisterTriangle(level, vertex_ab, vertex_bc, vertex_ac);
}

GLushort IcosahedronBase::TriangleDivider::VertexInterpolate(unsigned v1, unsigned v2)
{
 SYS_DEBUG_MEMBER(DM_GLESLY);

 GLushort & cached = myVerticeCache[v1][v2];

 if (cached) {
    SYS_DEBUG(DL_INFO1, "Using cached vertex entry " << cached << " between " << v1 << " and " << v2);
    return cached;
 }

 Vec3 interpolated;

 const float * v1_pos = GetVertex(v1);
 const float * v2_pos = GetVertex(v2);

 interpolated.x = (v1_pos[0] + v2_pos[0]) / 2.0f;
 interpolated.y = (v1_pos[1] + v2_pos[1]) / 2.0f;
 interpolated.z = (v1_pos[2] + v2_pos[2]) / 2.0f;

 // Adjust a unity vector:
 float size = sqrtf(interpolated.x * interpolated.x + interpolated.y * interpolated.y + interpolated.z * interpolated.z) / myParent.mySize;
 interpolated.x /= size;
 interpolated.y /= size;
 interpolated.z /= size;

 GLushort result = RegisterVertex(interpolated);

 SYS_DEBUG(DL_INFO1, "Registered vertex " << result << ", interpolated between " << v1 << " and " << v2);

 // Store the result in both combinations:
 myVerticeCache[v2][v1] = cached = result;

 return result;
}

/* * * * * * * * * * * * * End - of - File * * * * * * * * * * * * * * */
