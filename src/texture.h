/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * Project:     Glesly: my GLES-based rendering library
 * Purpose:     Texture handling
 * Author:      György Kövesdi (kgy@teledigit.eu)
 * Licence:     GPL (see file 'COPYING' in the project root for more details)
 * Comments:    
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef __GLESLY_SRC_TEXTURE_H_INCLUDED__
#define __GLESLY_SRC_TEXTURE_H_INCLUDED__

#include <GLES2/gl2.h>

#include <File/FileMap.h>
#include <Debug/Debug.h>

SYS_DECLARE_MODULE(DM_GLESLY);

namespace Glesly
{
    class Target2D;

    class Texture2DRaw
    {
     protected:
        Texture2DRaw(void * pixels, int width, int height, GLenum format = GL_RGB);
        Texture2DRaw(const Target2D & target, GLenum format = GL_RGB, bool update_now = true);
        virtual ~Texture2DRaw();

        inline GLuint GetBuffer(void)
        {
            return myTexture;
        }

        inline void Bind(void) const
        {
            SYS_DEBUG_MEMBER(DM_GLESLY);
            SYS_DEBUG(DL_INFO3, " - glBindTexture(GL_TEXTURE_2D, " << myTexture << ");");
            glBindTexture(GL_TEXTURE_2D, myTexture);
        }

        GLuint myTexture;

        int myWidth;

        int myHeight;

        GLenum myFormat;

        const void * myPixels;

     public:
        int GetWidth(void) const
        {
            return myWidth;
        }

        int GetHeight(void) const
        {
            return myHeight;
        }

        void Update(void);

     private:
        SYS_DEFINE_CLASS_NAME("Glesly::Texture2DRaw");

        void Initialize();

    }; // class Texture2DRaw

} // namespace Glesly

#endif /* __GLESLY_SRC_TEXTURE_H_INCLUDED__ */

/* * * * * * * * * * * * * End - of - File * * * * * * * * * * * * * * */
