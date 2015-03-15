/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * Project:     Glesly: GLES-based rendering library
 * Purpose:     Generic, any-shaped 3D object
 * Author:      György Kövesdi (kgy@teledigit.eu)
 * Licence:     GPL (see file 'COPYING' in the project root for more details)
 * Comments:    
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef __GLESLY_NAVI_GLESLY_BASE_SRC_GENERIC_SURFACE_OBJECT_H_INCLUDED__
#define __GLESLY_NAVI_GLESLY_BASE_SRC_GENERIC_SURFACE_OBJECT_H_INCLUDED__

#include <glesly/object.h>

namespace Glesly
{
    /// Base class for all kind of OpenGL objects.
    /*! \param  P   Number of vertex positions
     *  \param  E   Number of texture positions
     *  \param  N   Dimensions of the texture positions. Its default value is 2. */
    template <unsigned P, unsigned E, unsigned N=2>
    class GenericSurfaceObject: public Glesly::Object
    {
     protected:
        GenericSurfaceObject(Glesly::ObjectListBase & base):
            Glesly::Object(base),
            position(*this, "position", GL_STREAM_DRAW),
            texcoord(*this, "texcoord", GL_STREAM_DRAW),
            elements(*this)
        {
            SYS_DEBUG_MEMBER(DM_GLESLY);
        }

        virtual ~GenericSurfaceObject()
        {
            SYS_DEBUG_MEMBER(DM_GLESLY);
        }

        virtual void Frame(void)
        {
            SYS_DEBUG_MEMBER(DM_GLESLY);
            UseDepth _d;
            UseCullFace _c;
            Glesly::Object::DrawElements(GL_TRIANGLES, GetNoOfElements());
        }

        /// Vertex positions, 3D
        Glesly::Shaders::VBOAttribFloatVector<P, 3> position;

        /// Texture positions, 2D or 3D
        Glesly::Shaders::VBOAttribFloatVector<P, N> texcoord;

        /// Element indices
        Glesly::Shaders::VBOUShortElementBuffer<E> elements;

        inline void InitGL(void)
        {
            SYS_DEBUG_MEMBER(DM_GLESLY);
            position.InitGL();
            texcoord.InitGL();
            elements.InitGL();
        }

     public:
        inline unsigned GetNoOfVertices(void) const
        {
            return P;
        }

        /*! Returns the number of elements to be displayed in the Element Buffer (see \ref GenericSurfaceObject::elements).<br>
         *  It can be overriden if not all the elements is used. */
        virtual unsigned GetNoOfElements(void) const
        {
            return E;
        }

        virtual void initGL(void) override
        {
            InitGL();
        }

     private:
        SYS_DEFINE_CLASS_NAME("Glesly::GenericSurfaceObject");

    }; // class GenericSurfaceObject

} // namespace Glesly

#endif /* __GLESLY_NAVI_GLESLY_BASE_SRC_GENERIC_SURFACE_OBJECT_H_INCLUDED__ */

/* * * * * * * * * * * * * End - of - File * * * * * * * * * * * * * * */
