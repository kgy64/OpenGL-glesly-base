/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * Project:     Glesly: GLES-based rendering library
 * Purpose:     A Geodesic Dome, based on an Icosahedron to show a surfaced sphere, e.g. the Earth
 * Author:      György Kövesdi (kgy@teledigit.eu)
 * Licence:     GPL (see file 'COPYING' in the project root for more details)
 * Comments:    
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef __GLESLY_NAVI_GLESLY_BASE_SRC_SURFACED_ICOSAHEDRON_H_INCLUDED__
#define __GLESLY_NAVI_GLESLY_BASE_SRC_SURFACED_ICOSAHEDRON_H_INCLUDED__

#include <glesly/config.h>
#include <glesly/generic-surface-object.h>
#include <glesly/icosahedron-base.h>

namespace Glesly
{
    /// A surfaced Icosahedron object with any resolution
    /*! \param      N       If this is zero (the default), the basic Icosahedron is displayed (see \ref IcosahedronBase
     *                      for details). Increasing this value by one means dividing all triangles into four smaller
     *                      triangles, resulting in a higher resolution sphere. Be careful to use high values here! */
    template <unsigned N = 0U>
    class SurfacedIcosahedron: public Glesly::GenericSurfaceObject<(int)(24.0*(pow(4.0,N)+2.0)), 3*20*(int)pow(4,N)>, public IcosahedronBase
    {
     protected:
        SurfacedIcosahedron(Glesly::Render & render):
            Glesly::GenericSurfaceObject<(int)(24.0*(pow(4.0,N)+2.0)), 3*20*(int)pow(4,N)>(render),
            textureFile(CONFIG_ICON_DIR "/earth-001.tga", true),
            texture(*this, "texture", textureFile),
            myCurrentVertex(0U),
            myCurrentElement(0U)
        {
            SYS_DEBUG_MEMBER(DM_GLESLY);
            Initialize(N);
        }

        virtual unsigned RegisterVertex(const IcosahedronBase::Vec3 & vertex)
        {
            ASSERT(myCurrentVertex < (int)(24.0*(pow(4.0,N)+2.0)), "Vertex index is out of range");
            Glesly::GenericSurfaceObject<(int)(24.0*(pow(4.0,N)+2.0)), 3*20*(int)pow(4,N)>::position[myCurrentVertex][0] = vertex.x;
            Glesly::GenericSurfaceObject<(int)(24.0*(pow(4.0,N)+2.0)), 3*20*(int)pow(4,N)>::position[myCurrentVertex][1] = vertex.y;
            Glesly::GenericSurfaceObject<(int)(24.0*(pow(4.0,N)+2.0)), 3*20*(int)pow(4,N)>::position[myCurrentVertex][2] = vertex.z;
            Glesly::GenericSurfaceObject<(int)(24.0*(pow(4.0,N)+2.0)), 3*20*(int)pow(4,N)>::texcoord[myCurrentVertex][0] = vertex.lon;
            Glesly::GenericSurfaceObject<(int)(24.0*(pow(4.0,N)+2.0)), 3*20*(int)pow(4,N)>::texcoord[myCurrentVertex][1] = vertex.lat;
            return myCurrentVertex++;
        }

        virtual const float * GetVertex(unsigned index) const
        {
            ASSERT(index < myCurrentVertex, "Requesting nonexistent vertex");
            return Glesly::GenericSurfaceObject<(int)(24.0*(pow(4.0,N)+2.0)), 3*20*(int)pow(4,N)>::position[index];
        }

        virtual const float * GetTexcoord(unsigned index) const
        {
            ASSERT(index < myCurrentVertex, "Requesting nonexistent texcoord");
            return Glesly::GenericSurfaceObject<(int)(24.0*(pow(4.0,N)+2.0)), 3*20*(int)pow(4,N)>::texcoord[index];
        }

        virtual void RegisterTriangle(const IcosahedronBase::Triangle & triangle)
        {
            ASSERT(myCurrentElement <= 3*20*(int)pow(4,N)-3, "Too many triangles are registered, element count: " << myCurrentElement);
            myElems[myCurrentElement++] = triangle.a;
            myElems[myCurrentElement++] = triangle.b;
            myElems[myCurrentElement++] = triangle.c;
        }

        virtual void RegisterFinished(void)
        {
            SYS_DEBUG_MEMBER(DM_GLESLY);
            SYS_DEBUG(DL_INFO1, "Having " << myCurrentVertex << " of " << (int)(24.0*(pow(4.0,N)+2.0)) << " vertices and " << myCurrentElement << " of " << 3*20*(int)pow(4,N) << " elements");
            Glesly::GenericSurfaceObject<(int)(24.0*(pow(4.0,N)+2.0)), 3*20*(int)pow(4,N)>::elements.Bind(myElems, myCurrentElement);
            texture.Update();
        }

     public:
        virtual ~SurfacedIcosahedron()
        {
            SYS_DEBUG_MEMBER(DM_GLESLY);
        }

        inline static Glesly::ObjectPtr Create(Glesly::Render & render)
        {
            return Glesly::ObjectPtr(new SurfacedIcosahedron(render));
        }

     private:
        SYS_DEFINE_CLASS_NAME("Glesly::SurfacedIcosahedron");

        Glesly::ReadTGA textureFile;

        Glesly::Shaders::UniformTexture texture;

        GLushort myElems[3*20*(int)pow(4,N)];

        unsigned myCurrentVertex;

        unsigned myCurrentElement;

    }; // class SurfacedIcosahedron

} // namespace Glesly

#endif /* __GLESLY_NAVI_GLESLY_BASE_SRC_SURFACED_ICOSAHEDRON_H_INCLUDED__ */

/* * * * * * * * * * * * * End - of - File * * * * * * * * * * * * * * */
