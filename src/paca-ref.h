/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * Project:     Glesly: my GLES-based rendering library
 * Purpose:     Rectangle-shaped object using pangocairo with independent Cairo and GLES objects
 * Author:      György Kövesdi (kgy@teledigit.eu)
 * Licence:     GPL (see file 'COPYING' in the project root for more details)
 * Comments:    
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef __SRC_OBJECTS_PACA_REF_H_INCLUDED__
#define __SRC_OBJECTS_PACA_REF_H_INCLUDED__

#include <glesly/rectangle-object.h>
#include <glesly/generic-rectangle.h>
#include <pacalib/pacalib.h>

namespace Glesly
{
    class PaCaRef: public Glesly::RectangleObject<2,2>, private _RectangleInit
    {
        class TextureUpdateCallback;
        friend class TextureUpdateCallback;

        class TextureUpdateCallback: public ObjectCallback
        {
            friend class PaCaRef;

            static ObjectCallbackPtr Create(void)
            {
                return ObjectCallbackPtr(new TextureUpdateCallback);
            }

            virtual bool Execute(Glesly::ObjectBase & obj)
            {
                static_cast<PaCaRef&>(obj).UpdateTextureDirect();
                return true; // One-shot
            }

        }; // class TextureUpdateCallback

     protected:
        inline PaCaRef(Glesly::Render & render, Glesly::Target2D & target):
            Glesly::RectangleObject<2,2>(render),
            myTarget(target),
            texture(*this, "texture", target, 0, GL_RGBA)
        {
            SYS_DEBUG_MEMBER(DM_GLESLY);
            RectangleVerticesInit(position, texcoord, elements);
        }

        inline void UpdateTexture(void)
        {
            Execute(TextureUpdateCallback::Create());
        }

        inline void UpdateTextureDirect(void)
        {
            SYS_DEBUG_MEMBER(DM_GLESLY);
            texture.Update();
        }

        virtual void Frame(void);

     public:
        virtual ~PaCaRef()
        {
            SYS_DEBUG_MEMBER(DM_GLESLY);
        }

     private:
        SYS_DEFINE_CLASS_NAME("Glesly::PaCaRef");

        Glesly::Target2D & myTarget;

        Glesly::Shaders::UniformTexture texture;

    }; // class PaCaRef

} // namespace Glesly

#endif /* __SRC_OBJECTS_PACA_REF_H_INCLUDED__ */

/* * * * * * * * * * * * * End - of - File * * * * * * * * * * * * * * */
