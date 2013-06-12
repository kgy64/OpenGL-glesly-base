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

 for (unsigned y = 0, i = 0; y < 2; ++y) {
    for (unsigned x = 0; x < 2; ++x, ++i) {
        position[i][0] = x - 0.5;
        position[i][1] = y - 0.5;
        position[i][2] = 0.0;
        texcoord[i][0] = x;
        texcoord[i][1] = y;
    }
 }

 // We always draw two triangles:
 GLushort elem_init[] = {
    0, 1, 2,
    1, 3, 2
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
