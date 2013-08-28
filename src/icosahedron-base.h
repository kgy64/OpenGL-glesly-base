/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * Project:     Glesly: GLES-based rendering library
 * Purpose:     A Geodesic Dome, based on an Icosahedron to show a surfaced sphere, e.g. the Earth
 * Author:      György Kövesdi (kgy@teledigit.eu)
 * Licence:     GPL (see file 'COPYING' in the project root for more details)
 * Comments:    
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef __GLESLY_NAVI_GLESLY_BASE_SRC_ICOSAHEDRON_BASE_H_INCLUDED__
#define __GLESLY_NAVI_GLESLY_BASE_SRC_ICOSAHEDRON_BASE_H_INCLUDED__

#include <Debug/Debug.h>
#include <math.h>
#include <GLES2/gl2.h>

namespace Glesly
{
    /*! This class helps to create an <a href="http://en.wikipedia.org/wiki/Icosahedron">Icosahedron</a>
     *  with unity radius circumscribed sphere.
     *  <table border="0" cellspacing="20">
     *      <tr>
     *          <td>
     *              This is the basic Icosahedron built from three orthogonal golden rectangles.<br>
     *              See \ref basic_positions for the algirythmic details.
     *          </td>
     *          <td>
     *              \image html "geometry/Icosahedron-001.png"
     *          </td>
     *      </tr>
     *  </table>
     *  The resolution can be increased by dividing all triangles to four smaller triangles. The size can be changed
     *  by multiply all the vertex positions given in this class. */
    class IcosahedronBase
    {
        class TriangleDivider;
        friend class TriangleDivider;

     protected:
        void Initialize(unsigned level);

        virtual void RegisterVertex(unsigned index, float x, float y, float z, float lon, float lat) =0;
        virtual void RegisterTriangle(GLushort a, GLushort b, GLushort c) =0;
        virtual void RegisterFinished(void) =0;

        struct Vec3
        {
            float x;
            float y;
            float z;
            float lat;
            float lon;

        }; // struct Vec3

        struct Triangle
        {
            GLushort a;
            GLushort b;
            GLushort c;

        }; // struct Triangle

        enum {
            NO_OF_TRIANGLES     =   20,
            NO_OF_VERTICES      =   22
        };

        /*! The <a href="http://en.wikipedia.org/wiki/Golden_ratio">Golden Ratio</a>, used in the formulas
         *  in this class.
         *  \see \ref basic_positions   The algorythm details are described there. */
        static constexpr float golden = 1.61803398875f;

        /*! The angle between two connected vertices.<br>
         *  Its unit is radian. */
        static constexpr float alpha = 2.0f * atan2f(1.0f, golden);

        /*! Step between two connected vertices, in latitude direction.<br>
         *  Its unit is 1, can be used for texture positioning. */
        static constexpr float lat_step = alpha / M_PI;

        /*! Step between two connected vertices, in longitude direction.<br>
         *  Its unit is 1, can be used for texture positioning. */
        static constexpr float lon_step = 0.2f;

        /*! The absolute horizontal (X/Y) size of the inner 10 vertices (except the poles).<br>
         *  This must be divided into X and Y coordinates according to the angle of the vertex. Note that there
         *  are two angles: <b>π/5</b> and <b>2π/5</b> (see \ref p_2, \ref p_3, \ref p_4, and \ref p_5 below). */
        static constexpr float p_1 = 1.0f / sqrtf(1.25f);

        static constexpr float p_2 = p_1 * cosf(M_PI / 2.5f);

        static constexpr float p_3 = p_1 * sinf(M_PI / 2.5f);

        static constexpr float p_4 = p_1 * cosf(M_PI / 5.0f);

        static constexpr float p_5 = p_1 * sinf(M_PI / 5.0f);

        /*! This is the <b>Z</b> position of all inner 10 vertices. They have this position with altering sign. */
        static constexpr float p_6 = p_1 / 2.0f;

        static const Vec3 basic_positions[NO_OF_VERTICES];

        static const Triangle basic_triangles[NO_OF_TRIANGLES];

     private:
        SYS_DEFINE_CLASS_NAME("Glesly::IcosahedronBase");

        /// Helper class to handle Triangles and their Vertices
        class TriangleDivider
        {
            friend class Glesly::IcosahedronBase;

            TriangleDivider(Glesly::IcosahedronBase & parent);
            ~TriangleDivider();

            void RegisterVertex(int index, const Vec3 & vertex);
            void RegisterTriangle(unsigned level, const Triangle & triangle);

            SYS_DEFINE_CLASS_NAME("Glesly::IcosahedronBase::TriangleDivider");

            Glesly::IcosahedronBase & myParent;

        }; // class Glesly::IcosahedronBase::TriangleDivider

    }; // class IcosahedronBase

} // namespace Glesly

#endif /* __GLESLY_NAVI_GLESLY_BASE_SRC_ICOSAHEDRON_BASE_H_INCLUDED__ */

/* * * * * * * * * * * * * End - of - File * * * * * * * * * * * * * * */
