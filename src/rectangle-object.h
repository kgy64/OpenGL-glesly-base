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
    template <unsigned S>
    class RectangleObject: public Glesly::Object
    {
     protected:
        RectangleObject(Glesly::Render & render):
            Glesly::Object(render),
            position(*this, "position", GL_STREAM_DRAW),
            texcoord(*this, "texcoord", GL_STREAM_DRAW),
            elements(*this, "elements")
        {
            SYS_DEBUG_MEMBER(DM_GLESLY);
        }

        Glesly::Shaders::VBOAttribFloatVector<S, 3> position;
        Glesly::Shaders::VBOAttribFloatVector<S, 2> texcoord;
        Glesly::Shaders::VBOUShortElementBuffer<6*S> elements;

        virtual void Frame(void)
        {
            SYS_DEBUG_MEMBER(DM_GLESLY);
            DrawArrays(GL_TRIANGLES, 0, S);
        }

     public:
        virtual ~RectangleObject()
        {
            SYS_DEBUG_MEMBER(DM_GLESLY);
        }

     private:
        SYS_DEFINE_CLASS_NAME("Glesly::RectangleObject");

    }; // class RectangleObject

} // namespace Glesly

#endif /* __GLESLY_SRC_RECTANGLE_OBJECT_H_INCLUDED__ */

/* * * * * * * * * * * * * End - of - File * * * * * * * * * * * * * * */
