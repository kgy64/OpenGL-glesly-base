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
#include <glesly/texture.h>
#include <glesly/math/matrix.h>

namespace Glesly
{
    namespace Shaders
    {
        /// Represents a generic uniform variable by name
        class UniformBase: protected UniformList
        {
         protected:
            inline UniformBase(const UniformManager & obj, const char * name):
                UniformList(obj),
                myUniformID(-1)
            {
                SYS_DEBUG_MEMBER(DM_GLESLY);
            }

            VIRTUAL_IF_DEBUG inline ~UniformBase()
            {
                SYS_DEBUG_MEMBER(DM_GLESLY);
            }

            GLint GetUniformID(void)
            {
                if (myUniformID < 0) {
                    myUniformID = obj.GetUniformLocationSafe(name);
                }
                return myUniformID;
            }

         private:
            SYS_DEFINE_CLASS_NAME("Glesly::Shaders::UniformBase");

            GLint myUniformID;

        }; // class UniformBase

        class UniformTexture: public UniformBase, public Texture2DRaw
        {
         public:
            UniformTexture(const UniformManager & obj, const char * name, void * pixels, int width, int height, int index = 0, GLenum format = GL_RGB):
                UniformBase(obj, name),
                Texture2DRaw(pixels, width, height, format),
                myIndex(index)
            {
                SYS_DEBUG_MEMBER(DM_GLESLY);
            }

            UniformTexture(const UniformManager & obj, const char * name, const Target2D & target, int index = 0, GLenum format = GL_RGB):
                UniformBase(obj, name),
                Texture2DRaw(target, format, false),
                myIndex(index)
            {
                SYS_DEBUG_MEMBER(DM_GLESLY);
            }

            virtual ~UniformTexture()
            {
                SYS_DEBUG_MEMBER(DM_GLESLY);
            }

            virtual void Activate(void)
            {
                SYS_DEBUG_MEMBER(DM_GLESLY);
                SYS_DEBUG(DL_INFO3, " - glActiveTexture(GL_TEXTURE" << myIndex << ");");
                glActiveTexture(GL_TEXTURE0 + myIndex);
                SYS_DEBUG(DL_INFO3, " - glUniform1i(" << GetUniformID() << "," << myIndex << ");");
                glUniform1i(GetUniformID(), myIndex);
                Bind();
            }

         protected:
            int myIndex;

         private:
            SYS_DEFINE_CLASS_NAME("Glesly::Shaders::UniformTexture");

        }; // class UniformTexture

        template <typename T, unsigned N>
        class UniformMatrix_ref: public UniformBase
        {
         public:
            inline UniformMatrix_ref(const UniformManager & obj, const char * name, Matrix<T, N, N> & variable):
                UniformBase(obj, name),
                myVariable(variable)
            {
                SYS_DEBUG_MEMBER(DM_GLESLY);
            }

            VIRTUAL_IF_DEBUG inline ~UniformMatrix_ref()
            {
                SYS_DEBUG_MEMBER(DM_GLESLY);
            }

            virtual void Activate(void)
            {
                SYS_DEBUG_MEMBER(DM_GLESLY);
                switch (N) {
                    case 2:
                        SYS_DEBUG(DL_INFO3, " - glUniformMatrix2fv(" << UniformBase::GetUniformID() << ",1,GL_FALSE," << myVariable << ");");
                        glUniformMatrix2fv(UniformBase::GetUniformID(), 1, GL_FALSE, myVariable.get());
                    break;
                    case 3:
                        SYS_DEBUG(DL_INFO3, " - glUniformMatrix3fv(" << UniformBase::GetUniformID() << ",1,GL_FALSE," << myVariable << ");");
                        glUniformMatrix3fv(UniformBase::GetUniformID(), 1, GL_FALSE, myVariable.get());
                    break;
                    case 4:
                        SYS_DEBUG(DL_INFO3, " - glUniformMatrix4fv(" << UniformBase::GetUniformID() << ",1,GL_FALSE," << myVariable << ");");
                        glUniformMatrix4fv(UniformBase::GetUniformID(), 1, GL_FALSE, myVariable.get());
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

        inline void UniformManager::Register(UniformList & var) const
        {
            var.next = myVars;
            myVars = &var;
        }

        inline void UniformManager::ActivateVariables(void)
        {
            SYS_DEBUG_MEMBER(DM_GLESLY);
            for (UniformList * var = myVars; var; var=var->next) {
                var->Activate();
            }
        }

    } // namespace Shaders

} // namespace Glesly

#endif /* __GLESLY_SRC_SHADER_UNIFORMS_H_INCLUDED__ */

/* * * * * * * * * * * * * End - of - File * * * * * * * * * * * * * * */
