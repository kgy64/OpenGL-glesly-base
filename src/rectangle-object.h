/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * Project:     Glesly: my GLES-based rendering library
 * Purpose:     Generic rectangle-shaped 3D object
 * Author:      György Kövesdi (kgy@teledigit.eu)
 * Licence:     GPL (see file 'COPYING' in the project root for more details)
 * Comments:    
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef __GLESLY_SRC_RECTANGLE_OBJECT_H_INCLUDED__
#define __GLESLY_SRC_RECTANGLE_OBJECT_H_INCLUDED__

#include <glesly/generic-surface-object.h>

namespace Glesly
{
    template <unsigned W, unsigned H>
    class RectangleObject: public Glesly::GenericSurfaceObject<W*H, 6*(W-1)*(H-1)>
    {
     protected:
        RectangleObject(Glesly::Render & render):
            Glesly::GenericSurfaceObject<W*H, 6*(W-1)*(H-1)>(render)
        {
            SYS_DEBUG_MEMBER(DM_GLESLY);
        }

        virtual ~RectangleObject()
        {
            SYS_DEBUG_MEMBER(DM_GLESLY);
        }

        inline void InitGL(void)
        {
            SYS_DEBUG_MEMBER(DM_GLESLY);
            Glesly::GenericSurfaceObject<W*H, 6*(W-1)*(H-1)>::InitGL();
        }

     private:
        SYS_DEFINE_CLASS_NAME("Glesly::RectangleObject");

    }; // class RectangleObject

} // namespace Glesly

#endif /* __GLESLY_SRC_RECTANGLE_OBJECT_H_INCLUDED__ */

/* * * * * * * * * * * * * End - of - File * * * * * * * * * * * * * * */
