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
#include <glesly/shader-uniforms.h>

#include <Debug/Debug.h>

SYS_DECLARE_MODULE(DM_GLESLY);

namespace Glesly
{
    /// An OpenGL program
    class Program: public Glesly::Shaders::UniformManager
    {
     public:
        virtual ~Program();

        virtual void UseShaders(void) =0;

        void AddShader(ShaderPtr shader);
        void AttachShaders(void);
        void Link(void);
        void ProgramInit(void);
        void ProgramCleanup(void);

        inline void BindAttribLocation(GLuint index, const char * name)
        {
            SYS_DEBUG_MEMBER(DM_GLESLY);
            SYS_DEBUG(DL_INFO1, " - glBindAttribLocation(" << GetProgramID() << ", " << index << ", '" << name << "');");
            glBindAttribLocation(GetProgramID(), index, name);
        }

        inline GLint GetUniformLocation(const char * name) const
        {
            SYS_DEBUG_MEMBER(DM_GLESLY);
            GLint result = glGetUniformLocation(GetProgramID(), name);
            SYS_DEBUG(DL_INFO1, " - glGetUniformLocation(" << GetProgramID() << ", '" << name << "'): location=" << result);
            return result;
        }

        inline GLint GetAttribLocation(const char * name) const
        {
            SYS_DEBUG_MEMBER(DM_GLESLY);
            GLint result = glGetAttribLocation(GetProgramID(), name);
            SYS_DEBUG(DL_INFO1, " - glGetAttribLocation(" << GetProgramID() << ", '" << name << "'): location=" << result);
            return result;
        }

        inline GLuint GetProgramID(void) const
        {
            return myProgram;
        }

        inline void UseProgram(void) const
        {
            SYS_DEBUG_MEMBER(DM_GLESLY);
            SYS_DEBUG(DL_INFO3, " - glUseProgram(" << GetProgramID() << ");");
            glUseProgram(GetProgramID());
        }

        inline void UnuseProgram(void) const
        {
            SYS_DEBUG_MEMBER(DM_GLESLY);
            SYS_DEBUG(DL_INFO3, " - glUseProgram(0);");
            glUseProgram(0);
        }

        virtual GLint GetUniformLocationSafe(const char * name) const;
        GLint GetAttribLocationSafe(const char * name) const;

        std::string GetLogInfo(void);

     protected:
        Program(void);

        GLuint myProgram;

        typedef std::vector<ShaderPtr> ShaderList;

        ShaderList myShaders;

     private:
        SYS_DEFINE_CLASS_NAME("Glesly::Program");

    }; // class Program

    class UseDepth
    {
     public:
        inline UseDepth(void)
        {
            SYS_DEBUG_MEMBER(DM_GLESLY);
            glEnable(GL_DEPTH_TEST);
        }

        inline ~UseDepth()
        {
            SYS_DEBUG_MEMBER(DM_GLESLY);
            glDisable(GL_DEPTH_TEST);
        }

     private:
        SYS_DEFINE_CLASS_NAME("Glesly::Shader::UseDepth");

    }; // class UseDepth

    class UseCullFace
    {
     public:
        inline UseCullFace(void)
        {
            SYS_DEBUG_MEMBER(DM_GLESLY);
            glEnable(GL_CULL_FACE);
        }

        inline ~UseCullFace()
        {
            SYS_DEBUG_MEMBER(DM_GLESLY);
            glDisable(GL_CULL_FACE);
        }

     private:
        SYS_DEFINE_CLASS_NAME("Glesly::Shader::UseCullFace");

    }; // class UseCullFace

    class UseBlend
    {
     public:
        inline UseBlend(GLenum source_factor = GL_SRC_ALPHA, GLenum dest_factor = GL_ONE_MINUS_SRC_ALPHA)
        {
            SYS_DEBUG_MEMBER(DM_GLESLY);
            glEnable(GL_BLEND);
            glBlendFunc(source_factor, dest_factor);
        }

        inline ~UseBlend()
        {
            SYS_DEBUG_MEMBER(DM_GLESLY);
            glDisable(GL_BLEND);
        }

     private:
        SYS_DEFINE_CLASS_NAME("Glesly::Shader::UseBlend");

    }; // class UseBlend

} // namespace Glesly

#endif /* __GLESLY_SRC_PROGRAM_H_INCLUDED__ */

/* * * * * * * * * * * * * End - of - File * * * * * * * * * * * * * * */
