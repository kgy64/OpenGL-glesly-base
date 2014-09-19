/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * Project:     Glesly: my GLES-based rendering library
 * Purpose:     Helper class for any kind of rectangle-shaped object
 * Author:      György Kövesdi (kgy@teledigit.eu)
 * Licence:     GPL (see file 'COPYING' in the project root for more details)
 * Comments:    
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef __SRC_OBJECTS_GENERIC_RECTANGLE_H_INCLUDED__
#define __SRC_OBJECTS_GENERIC_RECTANGLE_H_INCLUDED__

#include <glesly/shader-attribs.h>

namespace Glesly
{
    class _RectangleInit
    {
     protected:
        static void RectangleVerticesInit(Glesly::Shaders::VBOAttribFloatVector<4, 3> & position, Glesly::Shaders::VBOAttribFloatVector<4, 2> & texcoord, Shaders::VBOAttribBase & elements);

    }; // class _RectangleInit

} // namespace Glesly

#endif /* __SRC_OBJECTS_GENERIC_RECTANGLE_H_INCLUDED__ */

/* * * * * * * * * * * * * End - of - File * * * * * * * * * * * * * * */
