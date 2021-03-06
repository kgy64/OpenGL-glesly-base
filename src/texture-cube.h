/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * Project:     Glesly: my GLES-based rendering library
 * Purpose:     Texture handling: Cube Mapping
 * Author:      György Kövesdi (kgy@teledigit.eu)
 * Licence:     GPL (see file 'COPYING' in the project root for more details)
 * Comments:    
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef __GLESLY_NAVI_GLESLY_BASE_SRC_TEXTURE_CUBE_H_INCLUDED__
#define __GLESLY_NAVI_GLESLY_BASE_SRC_TEXTURE_CUBE_H_INCLUDED__

#include <File/FileMap.h>
#include <Debug/Debug.h>
#include <glesly/error.h>
#include <glesly/format.h>

SYS_DECLARE_MODULE(DM_GLESLY);

namespace Glesly
{
    class Target2D;

    class TextureCubeMap
    {
     protected:
        TextureCubeMap(bool use_mipmap = true);
        virtual ~TextureCubeMap();

        inline GLuint GetBuffer(void) const
        {
            ASSERT(myTexture != 0xffffffff, "object (TextureCubeMap) is not initialized yet");
            return myTexture;
        }

        inline void Bind(void) const
        {
            SYS_DEBUG_MEMBER(DM_GLESLY);
            GLuint texture = GetBuffer();
            SYS_DEBUG(DL_INFO3, " - glBindTexture(GL_TEXTURE_CUBE_MAP, " << texture << ");");
            glBindTexture(GL_TEXTURE_CUBE_MAP, texture);
            CheckEGLError("glBindTexture()");
        }

        GLuint myTexture;

        bool myUseMipmap;

     public:
        void Update(void);
        void InitGL();
        void UninitGL();

     private:
        SYS_DEFINE_CLASS_NAME("Glesly::TextureCubeMap");

        /// Array of 6 textures
        virtual const Target2D * const * getTargets(void) const =0;

    }; // class Glesly::TextureCubeMap

} // namespace Glesly

#endif /* __GLESLY_NAVI_GLESLY_BASE_SRC_TEXTURE_CUBE_H_INCLUDED__ */

/* * * * * * * * * * * * * End - of - File * * * * * * * * * * * * * * */
