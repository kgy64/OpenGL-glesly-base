/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * Project:     Glesly: my GLES-based rendering library
 * Purpose:     Helper class for any kind of rectangle-shaped object
 * Author:      György Kövesdi (kgy@teledigit.eu)
 * Licence:     GPL (see file 'COPYING' in the project root for more details)
 * Comments:    
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "generic-rectangle.h"

using namespace Glesly;

void _RectangleInit::RectangleVerticesInit(Glesly::Shaders::VBOAttribFloatVector<4, 3> & position, Glesly::Shaders::VBOAttribFloatVector<4, 2> & texcoord, Shaders::VBOAttribBase & elements)
{
 SYS_DEBUG_STATIC(DM_GLESLY);

 static const float vertex_init[] = {
    -1.0,   -1.0,   0.0,
     1.0,   -1.0,   0.0,
     1.0,    1.0,   0.0,
    -1.0,    1.0,   0.0
 };

 position = vertex_init;

 static const float texture_pos[] = {
    0.0,    0.0,
    1.0,    0.0,
    1.0,    1.0,
    0.0,    1.0
 };

 texcoord = texture_pos;

 // We always draw two triangles:
 static const GLushort elem_init[] = {
    0, 1, 2,
    0, 2, 3
 };

 elements.Bind(elem_init, 6);
}

/* * * * * * * * * * * * * End - of - File * * * * * * * * * * * * * * */
