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

        class AttribList
        {
            friend class AttribManager;

         protected:
            inline AttribList(AttribManager & parent):
                myParent(parent)
                // Note: 'next' is uninitialized intentionally. It will be
                //       initialized in UniformManager::Register()
            {
                SYS_DEBUG_MEMBER(DM_GLESLY);
                myParent.Register(*this);
            }

            AttribManager & GetParent(void)
            {
                return myParent;
            }

         private:
            SYS_DEFINE_CLASS_NAME("Glesly::Shaders::AttribList");

            virtual void BufferData(void)=0;
            virtual void UnbufferData(void)=0;

            AttribManager & myParent;

            AttribList * next;

        }; // class AttribList

        /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

        class UniformList;

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

        class UniformManagerCopy: public UniformManager
        {
         public:
            UniformManagerCopy(const UniformManager & other):
                myParent(other)
            {
                SYS_DEBUG_MEMBER(DM_GLESLY);
            }

            virtual GLint GetUniformLocationSafe(const char * name) const
            {
                return myParent.GetUniformLocationSafe(name);
            }

         private:
            SYS_DEFINE_CLASS_NAME("Glesly::Shaders::UniformManagerCopy");

            const UniformManager & myParent;

        }; // class UniformManagerCopy

        class UniformList
        {
            friend class UniformManager;

         protected:
            inline UniformList(UniformManager & parent):
                myParent(parent)
                // Note: 'next' is uninitialized intentionally. It will be
                //       initialized in UniformManager::Register()
            {
                SYS_DEBUG_MEMBER(DM_GLESLY);
                myParent.Register(*this);
            }

            UniformManager & GetParent(void)
            {
                return myParent;
            }

         private:
            SYS_DEFINE_CLASS_NAME("Glesly::Shaders::UniformList");

            virtual void Activate(void)=0;

            UniformManager & myParent;

            UniformList * next;

        }; // class UniformList

        /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

        class VarManager: public AttribManager, public UniformManager
        {
            SYS_DEFINE_CLASS_NAME("Glesly::Shaders::VarManager");

        }; // class VarManager

    } // namespace Shaders

} // namespace Glesly

#endif /* __GLESLY_SRC_SHADER_VARS_H_INCLUDED__ */

/* * * * * * * * * * * * * End - of - File * * * * * * * * * * * * * * */
