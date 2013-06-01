/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * Project:     Glesly: my GLES-based rendering library
 * Purpose:     Class to represent one OpenGL program
 * Author:      György Kövesdi (kgy@teledigit.eu)
 * Licence:     GPL (see file 'COPYING' in the project root for more details)
 * Comments:    
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef __GLESLY_SRC_PROGRAM_H_INCLUDED__
#define __GLESLY_SRC_PROGRAM_H_INCLUDED__

#include <GLES2/gl2.h>

#include <glesly/program-ptr.h>
#include <glesly/shader-ptr.h>
#include <Debug/Debug.h>

SYS_DECLARE_MODULE(DM_GLESLY);

namespace Glesly
{
    /// An OpenGL program
    class Program
    {
     public:
        virtual ~Program();

        static inline ProgramPtr Create(void)
        {
            return ProgramPtr(new Program());
        }

        void Attach(ShaderPtr shader);
        void Link(void);

        inline void BindAttribLocation(GLuint index, const char * name)
        {
            glBindAttribLocation(myProgram, index, name);
        }

        inline GLint GetUniformLocation(const char * name) const
        {
            SYS_DEBUG_MEMBER(DM_GLESLY);
            GLint result = glGetUniformLocation(myProgram, name);
            SYS_DEBUG(DL_INFO1, "Uniform '" << name << "' location: " << result);
            return result;
        }

        inline GLint GetAttribLocation(const char * name) const
        {
            SYS_DEBUG_MEMBER(DM_GLESLY);
            GLint result = glGetAttribLocation(myProgram, name);
            SYS_DEBUG(DL_INFO1, "Attribute '" << name << "' location: " << result);
            return result;
        }

        GLuint GetProgramID(void) const
        {
            return myProgram;
        }

        GLint GetUniformLocationSafe(const char * name) const;
        GLint GetAttribLocationSafe(const char * name) const;
        std::string GetLogInfo(void);

     protected:
        Program(void);

        GLuint myProgram;

        std::vector<ShaderPtr> myShaders;

     private:
        SYS_DEFINE_CLASS_NAME("Glesly::Program");

    }; // class Program

} // namespace Glesly

#endif /* __GLESLY_SRC_PROGRAM_H_INCLUDED__ */

/* * * * * * * * * * * * * End - of - File * * * * * * * * * * * * * * */
