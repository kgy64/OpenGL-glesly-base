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
    class SurfacedIcosahedron: public Glesly::GenericSurfaceObject<22*(int)pow(2,N), 3*20*(int)pow(4,N)>, public IcosahedronBase
    {
     protected:
        SurfacedIcosahedron(Glesly::Render & render):
            Glesly::GenericSurfaceObject<22*(int)pow(2,N), 3*20*(int)pow(4,N)>(render),
            textureFile(CONFIG_ICON_DIR "/earth-001.tga", true),
            texture(*this, "texture", textureFile),
            myCurrentElement(0U)
        {
            SYS_DEBUG_MEMBER(DM_GLESLY);
            Initialize(N);
        }

        virtual void RegisterVertex(unsigned index, float x, float y, float z, float lon, float lat)
        {
            ASSERT(index < 22*(int)pow(2,N), "Vertex index is out of range");
            Glesly::GenericSurfaceObject<22*(int)pow(2,N), 3*20*(int)pow(4,N)>::position[index][0] = x;
            Glesly::GenericSurfaceObject<22*(int)pow(2,N), 3*20*(int)pow(4,N)>::position[index][1] = y;
            Glesly::GenericSurfaceObject<22*(int)pow(2,N), 3*20*(int)pow(4,N)>::position[index][2] = z;
            Glesly::GenericSurfaceObject<22*(int)pow(2,N), 3*20*(int)pow(4,N)>::texcoord[index][0] = lon;
            Glesly::GenericSurfaceObject<22*(int)pow(2,N), 3*20*(int)pow(4,N)>::texcoord[index][1] = lat;
        }

        virtual void RegisterTriangle(GLushort a, GLushort b, GLushort c)
        {
            ASSERT(myCurrentElement <= 3*20*(int)pow(4,N)-3, "Too many triangles are registered: " << myCurrentElement);
            myElems[myCurrentElement++] = a;
            myElems[myCurrentElement++] = b;
            myElems[myCurrentElement++] = c;
        }

        virtual void RegisterFinished(void)
        {
            Glesly::GenericSurfaceObject<22*(int)pow(2,N), 3*20*(int)pow(4,N)>::elements.Bind(myElems, myCurrentElement);
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

        unsigned myCurrentElement;

    }; // class SurfacedIcosahedron

} // namespace Glesly

#endif /* __GLESLY_NAVI_GLESLY_BASE_SRC_SURFACED_ICOSAHEDRON_H_INCLUDED__ */

/* * * * * * * * * * * * * End - of - File * * * * * * * * * * * * * * */
