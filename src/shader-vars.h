/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * Project:     Glesly: my GLES-based rendering library
 * Purpose:     Shader variable handler classes
 * Author:      György Kövesdi (kgy@teledigit.eu)
 * Licence:     GPL (see file 'COPYING' in the project root for more details)
 * Comments:    
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef __GLESLY_SRC_SHADER_VARS_H_INCLUDED__
#define __GLESLY_SRC_SHADER_VARS_H_INCLUDED__

#include <GLES2/gl2.h>

#include <Debug/Debug.h>

SYS_DECLARE_MODULE(DM_GLESLY);

namespace Glesly
{
    namespace Shaders
    {
        class UniformList;
        class AttribList;

        class AttribManager
        {
         public:
            AttribManager(void):
                myAttribs(NULL)
            {
                SYS_DEBUG_MEMBER(DM_GLESLY);
            }

            virtual ~AttribManager()
            {
                SYS_DEBUG_MEMBER(DM_GLESLY);
            }

            void Register(AttribList & var) const;
            void BufferVariables(void);
            void UnbufferVariables(void);

         private:
            SYS_DEFINE_CLASS_NAME("Glesly::Shaders::AttribManager");

            mutable AttribList * myAttribs;

        }; // class AttribManager

        class UniformManager
        {
         public:
            UniformManager(void):
                myVars(NULL)
            {
                SYS_DEBUG_MEMBER(DM_GLESLY);
            }

            virtual ~UniformManager()
            {
                SYS_DEBUG_MEMBER(DM_GLESLY);
            }

            void Register(UniformList & var) const;
            void ActivateVariables(void);

            virtual GLint GetUniformLocationSafe(const char * name) const =0;

         private:
            SYS_DEFINE_CLASS_NAME("Glesly::Shaders::UniformManager");

            mutable UniformList * myVars;

        }; // class UniformManager

        class VarManager: public AttribManager, public UniformManager
        {
         protected:

         private:
            SYS_DEFINE_CLASS_NAME("Glesly::Shaders::VarManager");

        }; // class VarManager

        /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

        class UniformList
        {
            friend class UniformManager;

         protected:
            inline UniformList(const UniformManager & obj)
            {
                obj.Register(*this);
            }

         private:
            SYS_DEFINE_CLASS_NAME("Glesly::Shaders::UniformList");

            virtual void Activate(void)=0;

            UniformList * next;

        }; // class UniformList

        /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

        class AttribList
        {
            friend class AttribManager;

         protected:
            inline AttribList(const AttribManager & obj)
            {
                obj.Register(*this);
            }

         private:
            SYS_DEFINE_CLASS_NAME("Glesly::Shaders::AttribList");

            virtual void BufferData(void)=0;
            virtual void UnbufferData(void)=0;

            AttribList * next;

        }; // class AttribList

    } // namespace Shaders

} // namespace Glesly

#endif /* __GLESLY_SRC_SHADER_VARS_H_INCLUDED__ */

/* * * * * * * * * * * * * End - of - File * * * * * * * * * * * * * * */
