/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * Project:     Glesly: my GLES-based rendering library
 * Purpose:     Generic rectangle-shaped object using pangocairo
 * Author:      György Kövesdi (kgy@teledigit.eu)
 * Licence:     GPL (see file 'COPYING' in the project root for more details)
 * Comments:    
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "paca-object.h"

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
 GLushort elem_init[] = {
    0, 1, 2,
    0, 2, 3
 };

 elements.Bind(elem_init, 6);
}

void PaCaObject::Frame(void)
{
 SYS_DEBUG_MEMBER(DM_GLESLY);

 UseBlend _b;

 DrawElements(GL_TRIANGLES, 6);
}

/* * * * * * * * * * * * * End - of - File * * * * * * * * * * * * * * */
