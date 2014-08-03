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
    /// The number of elements (3*triangles) in the interpolated Icosahedron
    unsigned constexpr IH_ELEM(unsigned N) { return 3*20*(int)pow(4,N); }

    /// The number of vertices in the interpolated Icosahedron
    /*! \note   This gives not an exact value, just a heuristic algorythm, can be used for <b>0<=N<=5</b>. If you want
     *          to use N=6, the exponent must be 3.856, I have not tried other values. However, N=3 seems enough.<br>
     *          There are some more (<2%) vertices will be allocated this way, I hope it is not a problem.
     */
    unsigned constexpr IH_VERT(unsigned N) { return 20+(int)floor(12.5*pow(3.85,N)); }

    template <unsigned N>
    using IcosahedronParent = Glesly::GenericSurfaceObject<IH_VERT(N), IH_ELEM(N), 3>;

    /// A surfaced Icosahedron object with any resolution
    /*! \param      N       If this is zero (the default), the basic Icosahedron is displayed (see \ref IcosahedronBase
     *                      for details). Increasing this value by one means dividing all triangles into four smaller
     *                      triangles, resulting in a higher resolution sphere. Be careful to use high values here!
     *  \see        IH_VERT The number of vertices
     *  \see        IH_ELEM The number of elements */
    template <unsigned N = 0U>
    class SurfacedIcosahedron: public IcosahedronParent<N>, public IcosahedronBase
    {
        typedef IcosahedronParent<N> ParentType;

     protected:
        SurfacedIcosahedron(Glesly::Render & render, float size, const Glesly::Target2D * textures[6]):
            ParentType(render),
            IcosahedronBase(size),
            texture(*this, "texture", textures),
            myCurrentVertex(0U),
            myCurrentElement(0U)
        {
            SYS_DEBUG_MEMBER(DM_GLESLY);
            Initialize(N);
        }

     public:
        virtual ~SurfacedIcosahedron()
        {
            SYS_DEBUG_MEMBER(DM_GLESLY);
        }

     protected:
        virtual unsigned RegisterVertex(const IcosahedronBase::Vec3 & vertex)
        {
            ASSERT(myCurrentVertex < IH_VERT(N), "Vertex index is out of range");
            ParentType::position[myCurrentVertex][0] = vertex.x;
            ParentType::position[myCurrentVertex][1] = vertex.y;
            ParentType::position[myCurrentVertex][2] = vertex.z;
            ParentType::texcoord[myCurrentVertex][0] = vertex.x;
            ParentType::texcoord[myCurrentVertex][1] = vertex.y;
            ParentType::texcoord[myCurrentVertex][2] = vertex.z;
            return myCurrentVertex++;
        }

        virtual const float * GetVertex(unsigned index) const
        {
            ASSERT(index < myCurrentVertex, "Requesting nonexistent vertex");
            return ParentType::position[index];
        }

        virtual const float * GetTexcoord(unsigned index) const
        {
            ASSERT(index < myCurrentVertex, "Requesting nonexistent texcoord");
            return ParentType::texcoord[index];
        }

        virtual void RegisterTriangle(const IcosahedronBase::Triangle & triangle)
        {
            ASSERT(myCurrentElement <= IH_ELEM(N)-3, "Too many triangles are registered, element count: " << myCurrentElement);
            myElems[myCurrentElement++] = triangle.a;
            myElems[myCurrentElement++] = triangle.b;
            myElems[myCurrentElement++] = triangle.c;
        }

        virtual void RegisterFinished(void)
        {
            SYS_DEBUG_MEMBER(DM_GLESLY);
            SYS_DEBUG(DL_INFO1, "Having " << myCurrentVertex << " of " << IH_VERT(N) << " vertices and " << myCurrentElement << " of " << IH_ELEM(N) << " elements");
            ParentType::elements.Bind(myElems, myCurrentElement);
        }

        inline static Glesly::ObjectPtr Create(Glesly::Render & render, float size = 1.0f)
        {
            return Glesly::ObjectPtr(new SurfacedIcosahedron(render, size));
        }

     private:
        SYS_DEFINE_CLASS_NAME("Glesly::SurfacedIcosahedron");

        virtual void initGL(void) override
        {
            SYS_DEBUG_MEMBER(DM_GLESLY);
            Glesly::Shaders::UniformManager::InitGLVariables(); // Initialize my own objects too
            ParentType::InitGL();
        }

        Glesly::Shaders::UniformTextureCube texture;

        GLushort myElems[IH_ELEM(N)];

        unsigned myCurrentVertex;

        unsigned myCurrentElement;

    }; // class SurfacedIcosahedron

} // namespace Glesly

#endif /* __GLESLY_NAVI_GLESLY_BASE_SRC_SURFACED_ICOSAHEDRON_H_INCLUDED__ */

/* * * * * * * * * * * * * End - of - File * * * * * * * * * * * * * * */
