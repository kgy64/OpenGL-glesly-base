/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * Project:     Glesly: my GLES-based rendering library
 * Purpose:     Generic lines
 * Author:      György Kövesdi (kgy@teledigit.eu)
 * Licence:     GPL (see file 'COPYING' in the project root for more details)
 * Comments:    
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef __GLESLY_SRC_RECTANGLE_OBJECT_H_INCLUDED__
#define __GLESLY_SRC_RECTANGLE_OBJECT_H_INCLUDED__

#include <glesly/object.h>

namespace Glesly
{
    template <unsigned N>
    class LineObject: public Glesly::Object
    {
     protected:
        LineObject(Glesly::Render & render):
            Glesly::Object(render),
            position(*this, "position"),
            elements(*this, "elements")
        {
            SYS_DEBUG_MEMBER(DM_GLESLY);
        }

        virtual ~LineObject()
        {
            SYS_DEBUG_MEMBER(DM_GLESLY);
        }

        /// Vertex positions, 3D
        Glesly::Shaders::VBOAttribFloatVector<N, 3> position;

        /// Element indices
        Glesly::Shaders::VBOUShortElementBuffer<2*N> elements;

     private:
        SYS_DEFINE_CLASS_NAME("Glesly::LineObject");

    }; // class LineObject

} // namespace Glesly

#endif /* __GLESLY_SRC_RECTANGLE_OBJECT_H_INCLUDED__ */

/* * * * * * * * * * * * * End - of - File * * * * * * * * * * * * * * */
