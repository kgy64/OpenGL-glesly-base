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
    template <unsigned P, unsigned E, unsigned N=2>
    class GenericSurfaceObject: public Glesly::Object
    {
     protected:
        GenericSurfaceObject(Glesly::Render & render):
            Glesly::Object(render),
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
            position.InitGL();
            texcoord.InitGL();
            elements.InitGL();
        }

     public:
        inline unsigned GetNoOfVertices(void) const
        {
            return P;
        }

        inline unsigned GetNoOfElements(void) const
        {
            return E;
        }

     private:
        SYS_DEFINE_CLASS_NAME("Glesly::GenericSurfaceObject");

        virtual void initGL(void) override
        {
            InitGL();
        }

    }; // class GenericSurfaceObject

} // namespace Glesly

#endif /* __GLESLY_NAVI_GLESLY_BASE_SRC_GENERIC_SURFACE_OBJECT_H_INCLUDED__ */

/* * * * * * * * * * * * * End - of - File * * * * * * * * * * * * * * */
