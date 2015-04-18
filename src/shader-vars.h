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

#include <Threads/Mutex.h>
#include <Debug/Debug.h>

SYS_DECLARE_MODULE(DM_GLESLY);

namespace Glesly
{
    namespace Shaders
    {
        class AttribElement;

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

            void UninitGL(void);
            void Unregister(AttribElement & var);
            void Register(AttribElement & var);
            void BufferVariables(void);
            void UnbufferVariables(void);

         private:
            SYS_DEFINE_CLASS_NAME("Glesly::Shaders::AttribManager");

            AttribElement * myAttribs;

            Threads::Mutex membersMutex;

        }; // class AttribManager

        class AttribElement
        {
            friend class AttribManager;

         public:
            virtual ~AttribElement()
            {
                SYS_DEBUG_MEMBER(DM_GLESLY);
                myParent.Unregister(*this);
            }

            virtual void uninitGL(void)
            {
            }

         protected:
            inline AttribElement(AttribManager & parent):
                myParent(parent),
                next(nullptr)
            {
                SYS_DEBUG_MEMBER(DM_GLESLY);
                myParent.Register(*this);
            }

            AttribManager & GetParent(void)
            {
                return myParent;
            }

         private:
            SYS_DEFINE_CLASS_NAME("Glesly::Shaders::AttribElement");

            virtual void BufferData(void)=0;
            virtual void UnbufferData(void)=0;

            AttribManager & myParent;

            AttribElement * next;

        }; // class AttribElement

        /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

        class UniformElement;

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

            void Unregister(UniformElement & var);
            void Register(UniformElement & var);
            void ActivateVariables(void);
            void InitGLVariables(void);

            virtual GLint GetUniformLocationSafe(const char * name) const =0;

         private:
            SYS_DEFINE_CLASS_NAME("Glesly::Shaders::UniformManager");

            UniformElement * myVars;

            Threads::Mutex membersMutex;

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

        class UniformElement
        {
            friend class UniformManager;

         public:
            virtual ~UniformElement()
            {
                SYS_DEBUG_MEMBER(DM_GLESLY);
                myParent.Unregister(*this);
            }

            virtual void initGL(void) =0;

         protected:
            inline UniformElement(UniformManager & parent):
                myParent(parent),
                glInitialized(false),
                next(nullptr)
            {
                SYS_DEBUG_MEMBER(DM_GLESLY);
                myParent.Register(*this);
            }

            UniformManager & GetParent(void)
            {
                return myParent;
            }

         private:
            SYS_DEFINE_CLASS_NAME("Glesly::Shaders::UniformElement");

            virtual void Activate(void) =0;

            UniformManager & myParent;

            bool glInitialized;

            UniformElement * next;

        }; // class UniformElement

        /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

        class VarManager: public AttribManager, public UniformManager
        {
            SYS_DEFINE_CLASS_NAME("Glesly::Shaders::VarManager");

        }; // class VarManager

    } // namespace Shaders

} // namespace Glesly

#endif /* __GLESLY_SRC_SHADER_VARS_H_INCLUDED__ */

/* * * * * * * * * * * * * End - of - File * * * * * * * * * * * * * * */
