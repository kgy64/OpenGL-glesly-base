/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * Project:     Glesly: my GLES-based rendering library
 * Purpose:     C++ wrapper for GL/ES - Shader handler classes
 * Author:      György Kövesdi (kgy@teledigit.eu)
 * Licence:     GPL (see file 'COPYING' in the project root for more details)
 * Comments:    
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef __GLESLY_SRC_SHADER_H_INCLUDED__
#define __GLESLY_SRC_SHADER_H_INCLUDED__

#include <string>
#include <vector>
#include <boost/scoped_ptr.hpp>

#include <GLES2/gl2.h>

#include <glesly/shader-names.h>
#include <glesly/object.h>
#include <File/FileMap.h>
#include <Debug/Debug.h>

SYS_DECLARE_MODULE(DM_GLESLY);

namespace Glesly
{
    /// A shader
    class Shader
    {
        class Reader
        {
         public:
            Reader(const char * source);
            Reader(const Glesly::ShaderSource & source);

            VIRTUAL_IF_DEBUG inline ~Reader()
            {
                SYS_DEBUG_MEMBER(DM_GLESLY);
            }

            inline const char * GetSource(void) const
            {
                return mySource;
            }

            inline int GetLength(void) const
            {
                return myLength;
            }

         protected:
            boost::scoped_ptr<FILES::FileMap> myFile;

            const char * mySource;

            int myLength;

            static bool fromFile;

         private:
            SYS_DEFINE_CLASS_NAME("Glesly::Reader");

        }; // class Reader

     public:
        VIRTUAL_IF_DEBUG ~Shader();

        inline static ShaderPtr Create(GLenum type, const char * source)
        {
            Reader r(source);
            return ShaderPtr(new Shader(type, r.GetSource(), r.GetLength()));
        }

        inline static ShaderPtr Create(GLenum type, const Glesly::ShaderSource & source)
        {
            Reader r(source);
            return ShaderPtr(new Shader(type, r.GetSource(), r.GetLength()));
        }

        inline GLuint GetShaderID(void)
        {
            return myShader;
        }

        inline GLenum GetType(void) const
        {
            return myType;
        }

     protected:
        Shader(GLenum type, const char * source, int length);

        std::string GetLogInfo(void);

     private:
        SYS_DEFINE_CLASS_NAME("Glesly::Shader");

        GLenum myType;
        GLuint myShader;

    }; // class SHader

} // namespace Glesly

#endif /* __GLESLY_SRC_SHADER_H_INCLUDED__ */

/* * * * * * * * * * * * * End - of - File * * * * * * * * * * * * * * */
