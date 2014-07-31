/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * Project:     Glesly: my GLES-based rendering library
 * Purpose:     Generic lines
 * Author:      György Kövesdi (kgy@teledigit.eu)
 * Licence:     GPL (see file 'COPYING' in the project root for more details)
 * Comments:    
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef __GLESLY_SRC_LINE_OBJECT_H_INCLUDED__
#define __GLESLY_SRC_LINE_OBJECT_H_INCLUDED__

#include <glesly/object.h>

namespace Glesly
{
    template <unsigned N>
    class LineObject: public Glesly::Object
    {
     protected:
        LineObject(Glesly::Render & render, GLenum mode = GL_LINES):
            Glesly::Object(render),
            position(*this, "position", GL_STREAM_DRAW),
            colour(*this, "colour", GL_STREAM_DRAW),
            elements(*this),
            myNoOfElements(0),
            myMode(mode)
        {
            SYS_DEBUG_MEMBER(DM_GLESLY);
        }

        virtual ~LineObject()
        {
            SYS_DEBUG_MEMBER(DM_GLESLY);
        }

        virtual void Frame(void)
        {
            SYS_DEBUG_MEMBER(DM_GLESLY);
            DrawElements(myMode, myNoOfElements);
        }


        /// Vertex positions, 3D
        Glesly::Shaders::VBOAttribFloatVector<N+1, 3> position;

        /// Vertex colours, RGBA
        Glesly::Shaders::VBOAttribFloatVector<N+1, 4> colour;

        /// Element indices
        Glesly::Shaders::VBOUShortElementBuffer<2*N> elements;

        int myNoOfElements;

        GLenum myMode;

     private:
        SYS_DEFINE_CLASS_NAME("Glesly::LineObject");

        virtual void initGL(void) override
        {
            SYS_DEBUG_MEMBER(DM_GLESLY);
            position.InitGL();
            colour.InitGL();
            elements.InitGL();
        }

    }; // class LineObject

} // namespace Glesly

#endif /* __GLESLY_SRC_LINE_OBJECT_H_INCLUDED__ */

/* * * * * * * * * * * * * End - of - File * * * * * * * * * * * * * * */
