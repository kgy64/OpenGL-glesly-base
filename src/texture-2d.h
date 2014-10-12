/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * Project:     Glesly: my GLES-based rendering library
 * Purpose:     Texture handling: 2D Texture
 * Author:      György Kövesdi (kgy@teledigit.eu)
 * Licence:     GPL (see file 'COPYING' in the project root for more details)
 * Comments:    
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef __GLESLY_SRC_TEXTURE_H_INCLUDED__
#define __GLESLY_SRC_TEXTURE_H_INCLUDED__

#include <File/FileMap.h>
#include <Debug/Debug.h>
#include <glesly/error.h>
#include <glesly/format.h>

SYS_DECLARE_MODULE(DM_GLESLY);

namespace Glesly
{
    class Target2D;

    class Texture2DRaw
    {
     protected:
        Texture2DRaw(const Target2D & target, bool use_mipmap = true);
        virtual ~Texture2DRaw();

        inline GLuint GetBuffer(void)
        {
            ASSERT(myTexture != 0xffffffff, "texture is not initialized yet");
            return myTexture;
        }

        int myWidth;

        int myHeight;

        GLenum myFormat;

        GLenum myPixelFormat;

        bool myUseMipmap;

        const Target2D & myTarget;

     private:
        GLuint myTexture;

     public:
        inline void Bind(void)
        {
            SYS_DEBUG_MEMBER(DM_GLESLY);
            GLuint texture = GetBuffer();
            SYS_DEBUG(DL_INFO3, " - glBindTexture(GL_TEXTURE_2D, " << texture << ");");
            glBindTexture(GL_TEXTURE_2D, texture);
            CheckEGLError("glBindTexture()");
        }

        int GetWidth(void) const
        {
            return myWidth;
        }

        int GetHeight(void) const
        {
            return myHeight;
        }

        void Update(void);

        void InitGL(void);

     private:
        SYS_DEFINE_CLASS_NAME("Glesly::Texture2DRaw");

        void Initialize(void);

    }; // class Texture2DRaw

} // namespace Glesly

#endif /* __GLESLY_SRC_TEXTURE_H_INCLUDED__ */

/* * * * * * * * * * * * * End - of - File * * * * * * * * * * * * * * */
