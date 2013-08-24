/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * Project:     Glesly: my GLES-based rendering library
 * Purpose:     Generic rectangle-shaped object
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
    template <unsigned W, unsigned H>
    class RectangleObject: public Glesly::Object
    {
     protected:
        RectangleObject(Glesly::Render & render):
            Glesly::Object(render),
            position(*this, "position", GL_STREAM_DRAW),
            texcoord(*this, "texcoord", GL_STREAM_DRAW),
            elements(*this)
        {
            SYS_DEBUG_MEMBER(DM_GLESLY);
        }

        virtual ~RectangleObject()
        {
            SYS_DEBUG_MEMBER(DM_GLESLY);
        }

        /// Vertex positions, 3D
        Glesly::Shaders::VBOAttribFloatVector<W*H, 3> position;

        /// Texture positions, 2D
        Glesly::Shaders::VBOAttribFloatVector<W*H, 2> texcoord;

        /// Element indices
        Glesly::Shaders::VBOUShortElementBuffer<6*(W-1)*(H-1)> elements;

     private:
        SYS_DEFINE_CLASS_NAME("Glesly::RectangleObject");

    }; // class RectangleObject

} // namespace Glesly

#endif /* __GLESLY_SRC_RECTANGLE_OBJECT_H_INCLUDED__ */

/* * * * * * * * * * * * * End - of - File * * * * * * * * * * * * * * */
