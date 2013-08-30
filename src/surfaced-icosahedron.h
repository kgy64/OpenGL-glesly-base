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

#include <glesly/read-tga.h>
#include <glesly/config.h>
#include <glesly/generic-surface-object.h>
#include <glesly/icosahedron-base.h>

/// The number of elements (3*triangles) in the interpolated Icosahedron
#define     IH_ELEM     (3*20*(int)pow(4,N))

/// The number of vertices in the interpolated Icosahedron
/*! \note   This gives not an exact value, just a heuristic algorythm, can be used for <b>0<=N<=5</b>. If you want
 *          to use N=6, the exponent must be 3.856, I have not tried other values. However, N=3 seems enough.<br>
 *          There are some more (<2%) vertices will be allocated this way, I hope it is not a problem.
 */
#define     IH_VERT     (20+(int)floor(12.5*pow(3.85,N)))

namespace Glesly
{
    /// A surfaced Icosahedron object with any resolution
    /*! \param      N       If this is zero (the default), the basic Icosahedron is displayed (see \ref IcosahedronBase
     *                      for details). Increasing this value by one means dividing all triangles into four smaller
     *                      triangles, resulting in a higher resolution sphere. Be careful to use high values here!
     *  \see        IH_VERT The number of vertices
     *  \see        IH_ELEM The number of elements */
    template <unsigned N = 0U>
    class SurfacedIcosahedron: public Glesly::GenericSurfaceObject<IH_VERT, IH_ELEM>, public IcosahedronBase
    {
     protected:
        SurfacedIcosahedron(Glesly::Render & render, float size):
            Glesly::GenericSurfaceObject<IH_VERT, IH_ELEM>(render),
            IcosahedronBase(size),
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
            ASSERT(myCurrentVertex < IH_VERT, "Vertex index is out of range");
            Glesly::GenericSurfaceObject<IH_VERT, IH_ELEM>::position[myCurrentVertex][0] = vertex.x;
            Glesly::GenericSurfaceObject<IH_VERT, IH_ELEM>::position[myCurrentVertex][1] = vertex.y;
            Glesly::GenericSurfaceObject<IH_VERT, IH_ELEM>::position[myCurrentVertex][2] = vertex.z;
            Glesly::GenericSurfaceObject<IH_VERT, IH_ELEM>::texcoord[myCurrentVertex][0] = vertex.lon;
            Glesly::GenericSurfaceObject<IH_VERT, IH_ELEM>::texcoord[myCurrentVertex][1] = vertex.lat;
            return myCurrentVertex++;
        }

        virtual const float * GetVertex(unsigned index) const
        {
            ASSERT(index < myCurrentVertex, "Requesting nonexistent vertex");
            return Glesly::GenericSurfaceObject<IH_VERT, IH_ELEM>::position[index];
        }

        virtual const float * GetTexcoord(unsigned index) const
        {
            ASSERT(index < myCurrentVertex, "Requesting nonexistent texcoord");
            return Glesly::GenericSurfaceObject<IH_VERT, IH_ELEM>::texcoord[index];
        }

        virtual void RegisterTriangle(const IcosahedronBase::Triangle & triangle)
        {
            ASSERT(myCurrentElement <= IH_ELEM-3, "Too many triangles are registered, element count: " << myCurrentElement);
            myElems[myCurrentElement++] = triangle.a;
            myElems[myCurrentElement++] = triangle.b;
            myElems[myCurrentElement++] = triangle.c;
        }

        virtual void RegisterFinished(void)
        {
            SYS_DEBUG_MEMBER(DM_GLESLY);
            SYS_DEBUG(DL_INFO1, "Having " << myCurrentVertex << " of " << IH_VERT << " vertices and " << myCurrentElement << " of " << IH_ELEM << " elements");
            Glesly::GenericSurfaceObject<IH_VERT, IH_ELEM>::elements.Bind(myElems, myCurrentElement);
            texture.Update();
        }

     public:
        virtual ~SurfacedIcosahedron()
        {
            SYS_DEBUG_MEMBER(DM_GLESLY);
        }

        inline static Glesly::ObjectPtr Create(Glesly::Render & render, float size = 1.0f)
        {
            return Glesly::ObjectPtr(new SurfacedIcosahedron(render, size));
        }

     private:
        SYS_DEFINE_CLASS_NAME("Glesly::SurfacedIcosahedron");

        Glesly::ReadTGA textureFile;

        Glesly::Shaders::UniformTexture texture;

        GLushort myElems[IH_ELEM];

        unsigned myCurrentVertex;

        unsigned myCurrentElement;

    }; // class SurfacedIcosahedron

} // namespace Glesly

#endif /* __GLESLY_NAVI_GLESLY_BASE_SRC_SURFACED_ICOSAHEDRON_H_INCLUDED__ */

/* * * * * * * * * * * * * End - of - File * * * * * * * * * * * * * * */
