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
        static void RectangleVerticesInit(Glesly::Shaders::VBOAttribFloatVector<4, 3> & position, Glesly::Shaders::VBOAttribFloatVector<4, 2> & texcoord, Shaders::VBOAttribBase & elements);

    }; // class _RectangleInit

    class PaCaObject: public Glesly::RectangleObject<2,2>, private _RectangleInit
    {
     protected:
         inline PaCaObject(Glesly::Render & render, int width, int height, float aspect = 1.0f):
            Glesly::RectangleObject<2,2>(render),
            myPaca(width, height, aspect),
            texture(*this, "texture", myPaca, 0, GL_RGBA)
        {
            SYS_DEBUG_MEMBER(DM_GLESLY);
            RectangleVerticesInit(position, texcoord, elements);
        }

        inline PaCaLib::PacaTarget & PaCa(void)
        {
            return myPaca;
        };

        inline void UpdateTexture(void)
        {
            texture.Update();
        }

        virtual void Frame(void);

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
