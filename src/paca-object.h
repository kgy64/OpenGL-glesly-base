/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * Project:     Glesly: my GLES-based rendering library
 * Purpose:     Generic rectangle-shaped object using pangocairo
 * Author:      György Kövesdi (kgy@teledigit.eu)
 * Licence:     GPL (see file 'COPYING' in the project root for more details)
 * Comments:    
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef __GLESLY_SRC_PACA_OBJECT_H_INCLUDED__
#define __GLESLY_SRC_PACA_OBJECT_H_INCLUDED__

#include <glesly/rectangle-object.h>
#include <pacalib/pacalib.h>

namespace Glesly
{
    class _RectangleInit
    {
     protected:
        static void RectangleVerticesInit(Shaders::VBOAttribBase & attrib);

    }; // class _RectangleInit

    class PaCaObject: public Glesly::RectangleObject<4>, private _RectangleInit
    {
     protected:
         inline PaCaObject(Glesly::Render & render, int width, int height):
            Glesly::RectangleObject<4>(render),
            myPaca(width, height),
            texture(*this, "texture", myPaca, GL_RGBA)
        {
            SYS_DEBUG_MEMBER(DM_GLESLY);
            RectangleVerticesInit(elements);
        }

        inline PaCaLib::PacaTarget & PaCa(void)
        {
            return myPaca;
        };

     public:
        virtual ~PaCaObject()
        {
            SYS_DEBUG_MEMBER(DM_GLESLY);
        }

     private:
        SYS_DEFINE_CLASS_NAME("Glesly::PaCaObject");

        PaCaLib::PacaTarget myPaca;

        Glesly::Shaders::UniformTexture texture;

    }; // class PaCaObject

} // namespace Glesly

#endif /* __GLESLY_SRC_PACA_OBJECT_H_INCLUDED__ */

/* * * * * * * * * * * * * End - of - File * * * * * * * * * * * * * * */
