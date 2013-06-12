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

void _RectangleInit::RectangleVerticesInit(Shaders::VBOAttribBase & attrib)
{
 SYS_DEBUG_STATIC(DM_GLESLY);

 // We always draw two triangles:
 GLushort elem_init[] = {
    0, 1, 2,
    1, 3, 2
 };

 attrib.Bind(elem_init, 6);
}

/* * * * * * * * * * * * * End - of - File * * * * * * * * * * * * * * */
