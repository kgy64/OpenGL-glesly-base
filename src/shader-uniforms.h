/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * Project:     
 * Purpose:     
 * Author:      
 * Licence:     GPL (see file 'COPYING' in the project root for more details)
 * Comments:    
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef __GLESLY_SRC_SHADER_UNIFORMS_H_INCLUDED__
#define __GLESLY_SRC_SHADER_UNIFORMS_H_INCLUDED__

#include <glesly/shader-vars.h>
#include <glesly/texture-2d.h>
#include <glesly/texture-cube.h>
#include <glesly/math/matrix.h>
#include <glesly/error.h>

namespace Glesly
{
    namespace Shaders
    {
        /// Represents a generic uniform variable by name
        class UniformBase: protected UniformElement
        {
         protected:
            inline UniformBase(UniformManager & obj, const char * name):
                UniformElement(obj),
                myName(name),
                myUniformID(-1)
            {
                SYS_DEBUG_MEMBER(DM_GLESLY);
            }

            virtual ~UniformBase()
            {
                SYS_DEBUG_MEMBER(DM_GLESLY);
            }

            inline GLint GetUniformID(void)
            {
                ASSERT(myUniformID != -1, "object '" << myName << "' is not initialized yet");
                return myUniformID;
            }

            virtual void initGL(void) override
            {
                InitGL();
            }

            const char * myName;

         private:
            SYS_DEFINE_CLASS_NAME("Glesly::Shaders::UniformBase");

            GLint myUniformID;

            inline void InitGL(void)
            {
                SYS_DEBUG_MEMBER(DM_GLESLY);
                myUniformID = GetParent().GetUniformLocationSafe(myName);
            }

        }; // class UniformBase

        class UniformFloat_ref: public UniformBase
        {
         public:
            UniformFloat_ref(UniformManager & obj, const char * name, GLfloat & value):
                UniformBase(obj, name),
                myRef(value)
            {
                SYS_DEBUG_MEMBER(DM_GLESLY);
            }

            virtual void Activate(void) override
            {
                SYS_DEBUG_MEMBER(DM_GLESLY);
                SYS_DEBUG(DL_INFO3, " - glUniform1f(" << GetUniformID() << "," << myRef << ");");
                glUniform1f(GetUniformID(), myRef);
                CheckEGLError("glUniform1f()");
            }

         protected:
            GLfloat & myRef;

         private:
            SYS_DEFINE_CLASS_NAME("Glesly::Shaders::UniformFloat");

        }; // class UniformFloat

        class UniformFloat: public UniformFloat_ref
        {
         public:
            UniformFloat(UniformManager & obj, const char * name):
                UniformFloat_ref(obj, name, myValue),
                myValue(0.0f)
            {
                SYS_DEBUG_MEMBER(DM_GLESLY);
            }

            inline GLfloat operator=(float value)
            {
                return myValue = value;
            }

            inline operator float() const
            {
                return myValue;
            }

         protected:
            GLfloat myValue;

         private:
            SYS_DEFINE_CLASS_NAME("Glesly::Shaders::UniformFloat");

        }; // class UniformFloat

        class UniformTexture2D: public UniformBase, public Texture2DRaw
        {
         public:
            UniformTexture2D(UniformManager & obj, const char * name, const Target2D & target, int index = 0, bool use_mipmap = true):
                UniformBase(obj, name),
                Texture2DRaw(target, use_mipmap),
                myIndex(index)
            {
                SYS_DEBUG_MEMBER(DM_GLESLY);
            }

            virtual ~UniformTexture2D()
            {
                SYS_DEBUG_MEMBER(DM_GLESLY);
            }

            virtual void Activate(void) override
            {
                SYS_DEBUG_MEMBER(DM_GLESLY);
                SYS_DEBUG(DL_INFO3, " - glActiveTexture(GL_TEXTURE" << myIndex << ");");
                glActiveTexture(GL_TEXTURE0 + myIndex);
                CheckEGLError("glActiveTexture()");
                SYS_DEBUG(DL_INFO3, " - glUniform1i(" << GetUniformID() << "," << myIndex << ");");
                glUniform1i(GetUniformID(), myIndex);
                CheckEGLError("glUniform1i()");
                Bind();
            }

            virtual void initGL(void) override
            {
                InitGL();
            }

         protected:
            int myIndex;

         private:
            SYS_DEFINE_CLASS_NAME("Glesly::Shaders::UniformTexture2D");

            inline void InitGL(void)
            {
                SYS_DEBUG_MEMBER(DM_GLESLY);
                UniformBase::initGL();
                Texture2DRaw::InitGL();
            }

        }; // class UniformTexture2D

        class UniformTextureCube: public UniformBase, public TextureCubeMap
        {
         public:
            UniformTextureCube(UniformManager & obj, const char * name, int index = 0, bool use_mipmap = true):
                UniformBase(obj, name),
                TextureCubeMap(use_mipmap),
                myIndex(index)
            {
                SYS_DEBUG_MEMBER(DM_GLESLY);
            }

            virtual ~UniformTextureCube()
            {
                SYS_DEBUG_MEMBER(DM_GLESLY);
            }

            virtual void Activate(void) override
            {
                SYS_DEBUG_MEMBER(DM_GLESLY);
                SYS_DEBUG(DL_INFO3, " - glActiveTexture(GL_TEXTURE" << myIndex << ");");
                glActiveTexture(GL_TEXTURE0 + myIndex);
                CheckEGLError("glActiveTexture()");
                SYS_DEBUG(DL_INFO3, " - glUniform1i(" << GetUniformID() << "," << myIndex << ");");
                glUniform1i(GetUniformID(), myIndex);
                CheckEGLError("glUniform1i()");
                Bind();
            }

            virtual void initGL(void) override
            {
                SYS_DEBUG_MEMBER(DM_GLESLY);
                UniformBase::initGL();
                TextureCubeMap::InitGL();
            }

         protected:
            int myIndex;

         private:
            SYS_DEFINE_CLASS_NAME("Glesly::Shaders::UniformTextureCube");

        }; // class UniformTextureCube

        template <typename T, unsigned N>
        class UniformMatrix_ref: public UniformBase
        {
         public:
            inline UniformMatrix_ref(UniformManager & obj, const char * name, Matrix<T, N, N> & variable):
                UniformBase(obj, name),
                myVariable(variable)
            {
                SYS_DEBUG_MEMBER(DM_GLESLY);
            }

            VIRTUAL_IF_DEBUG inline ~UniformMatrix_ref()
            {
                SYS_DEBUG_MEMBER(DM_GLESLY);
            }

            virtual void Activate(void) override
            {
                SYS_DEBUG_MEMBER(DM_GLESLY);
                switch (N) {
                    case 2:
                        SYS_DEBUG(DL_INFO3, " - glUniformMatrix2fv(" << UniformBase::GetUniformID() << ",1,GL_FALSE," << myVariable << "); name: '" << myName << "'");
                        glUniformMatrix2fv(UniformBase::GetUniformID(), 1, GL_FALSE, myVariable.get());
                        CheckEGLError("glUniformMatrix2fv()");
                    break;
                    case 3:
                        SYS_DEBUG(DL_INFO3, " - glUniformMatrix3fv(" << UniformBase::GetUniformID() << ",1,GL_FALSE," << myVariable << "); name: '" << myName << "'");
                        glUniformMatrix3fv(UniformBase::GetUniformID(), 1, GL_FALSE, myVariable.get());
                        CheckEGLError("glUniformMatrix3fv()");
                    break;
                    case 4:
                        SYS_DEBUG(DL_INFO3, " - glUniformMatrix4fv(" << UniformBase::GetUniformID() << ",1,GL_FALSE," << myVariable << "); name: '" << myName << "'");
                        glUniformMatrix4fv(UniformBase::GetUniformID(), 1, GL_FALSE, myVariable.get());
                        CheckEGLError("glUniformMatrix4fv()");
                    break;
                }
            }

            inline const T * operator=(const T * data)
            {
                return myVariable = data;
            }

            inline const Matrix<T,N,N> & operator=(const Matrix<T,N,N> & data)
            {
                return myVariable = data;
            }

            Matrix<T, N, N> & get(void)
            {
                return myVariable;
            };

         protected:
            Matrix<T, N, N> & myVariable;

         private:
            SYS_DEFINE_CLASS_NAME("Glesly::Shaders::UniformMatrix<T,N>");

        }; // class UniformMatrix_ref<T,N>

        /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

        inline void UniformManager::Register(UniformElement & var)
        {
            SYS_DEBUG_MEMBER(DM_GLESLY);
            Threads::Lock _l(membersMutex);
            var.next = myVars;
            myVars = &var;
        }

        inline void UniformManager::Unregister(UniformElement & var)
        {
            SYS_DEBUG_MEMBER(DM_GLESLY);
            Threads::Lock _l(membersMutex);
            for (UniformElement ** i = &myVars; *i; i = &(*i)->next) {
                if (*i == &var) {
                    *i = (*i)->next;    // Unchain
                    return;             // No more checks are necessary
                }
            }
        }

        inline void UniformManager::InitGLVariables(void)
        {
            SYS_DEBUG_MEMBER(DM_GLESLY);
            Threads::Lock _l(membersMutex);
            for (UniformElement * var = myVars; var; var=var->next) {
                if (!var->glInitialized) {
                    var->glInitialized = true;
                    var->initGL();
                }
            }
        }

        inline void UniformManager::ActivateVariables(void)
        {
            SYS_DEBUG_MEMBER(DM_GLESLY);
            Threads::Lock _l(membersMutex);
            for (UniformElement * var = myVars; var; var=var->next) {
                var->Activate();
            }
        }

    } // namespace Shaders

} // namespace Glesly

#endif /* __GLESLY_SRC_SHADER_UNIFORMS_H_INCLUDED__ */

/* * * * * * * * * * * * * End - of - File * * * * * * * * * * * * * * */
