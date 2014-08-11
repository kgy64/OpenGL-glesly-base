/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * Project:     Glesly: my GLES-based rendering library
 * Purpose:     Base class for all Objects
 * Author:      György Kövesdi (kgy@teledigit.eu)
 * Licence:     GPL (see file 'COPYING' in the project root for more details)
 * Comments:    
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef __GLESLY_SRC_OBJECT_BASE_H_INCLUDED__
#define __GLESLY_SRC_OBJECT_BASE_H_INCLUDED__

#include <list>

#include <System/TimeElapsed.h>
#include <glesly/object-ptr.h>
#include <International/utf8.h>
#include <Debug/Debug.h>

SYS_DECLARE_MODULE(DM_GLESLY);

namespace Glesly
{
    class Render;
    class ObjectGroup;

    class ObjectBase
    {
        friend class Render;
        friend class ObjectPtr;
        friend class ObjectGroup;

     public:
        virtual ~ObjectBase()
        {
            SYS_DEBUG_MEMBER(DM_GLESLY);
        }

        inline ObjectPtr GetPtr(void) const
        {
            SYS_DEBUG_MEMBER(DM_GLESLY);
            ObjectPtr p = mySelf.lock();
            ASSERT(p, "Glesly::ObjectBase::GetPtr() is called on a deleted object");
            return p;
        }

        virtual void DrawFrame(const SYS::TimeDelay & frame_start_time) =0;

        virtual bool MouseClick(float x, float y, int index, int count)
        {
            return false;
        }

        virtual void KeyboardClick(UTF8::WChar key)
        {
        }

        inline void Enable(bool enable = true)
        {
            SYS_DEBUG_MEMBER(DM_GLESLY);
            SYS_DEBUG(DL_INFO2, "now it is " << (enable ? "enabled" : "disabled"));
            myEnabled = enable;
        }

        inline bool IsEnabled(void) const
        {
            return myEnabled;
        }

        inline Glesly::Render & GetRenderer(void)
        {
            return myBase.GetRenderer();
        }

        inline const Glesly::Render & GetRenderer(void) const
        {
            return myBase.GetRenderer();
        }

     protected:
        ObjectBase(Glesly::ObjectListBase & base);

        inline ObjectPtr Create(void)
        {
            SYS_DEBUG_MEMBER(DM_GLESLY);
            ObjectPtr p(this);
            mySelf = p;
            ReinitGL();
            return p;
        }

        class ObjectCallback
        {
         public:
            virtual ~ObjectCallback()
            {
                SYS_DEBUG_MEMBER(DM_GLESLY);
            }

         protected:
            inline ObjectCallback(void)
            {
                SYS_DEBUG_MEMBER(DM_GLESLY);
            }

         private:
            /*! Only the class \ref Glesly::ObjectBase is allowed to call the callback
             *  function \ref ObjectCallback::Execute().
             *  \see ObjectBase::ExecuteCallback() for details. */
            friend class Glesly::ObjectBase;

            /// Callback function to execute sometning on the given object
            /*! This function will be called before the next frame rendering.
             *  \param  obj     The target object of the callback.
             *  \retval true    Delete the callback from the object.
             *  \retval false   Keep the callback. It will be called before the next frame too. */
            virtual bool Execute(Glesly::ObjectBase & obj) =0;

            SYS_DEFINE_CLASS_NAME("Glesly::ObjectBase::ObjectCallback");

        }; // class ObjectCallback

        void ReinitGL(void);
        void ExecuteCallback(const SYS::TimeDelay & frame_start_time);

        typedef boost::shared_ptr<ObjectCallback> ObjectCallbackPtr;

        inline void Execute(ObjectCallbackPtr callback)
        {
            // TODO Lock?
            myCallbacks.push_back(callback);
        }

        ObjectWeak mySelf;

     private:
        SYS_DEFINE_CLASS_NAME("Glesly::ObjectBase");

        /// Generic OpenGL initializer function
        /*! Note that any object can be created by any thread, but OpenGL function calls can be
         *  initiated only from one thread (the OpenGL render thread here). This function is
         *  called from the OpenGL render thread, once in a life of such OpenGL objects, before
         *  any other drawing operations. All OpenGL-specific initializations must be done here,
         *  instead of the constructor. */
        virtual void initGL(void) =0;

        int GetCallbackTimeLimit(void) const;

        Glesly::ObjectListBase & myBase;

        bool myEnabled;

        int myCallbackTimeLimit;

        std::list<ObjectCallbackPtr> myCallbacks;

    }; // class ObjectBase

} // namespace Glesly

#endif /* __GLESLY_SRC_OBJECT_BASE_H_INCLUDED__ */

/* * * * * * * * * * * * * End - of - File * * * * * * * * * * * * * * */
