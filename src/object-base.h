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

#include <System/TimeElapsed.h>
#include <glesly/object-ptr.h>
#include <International/utf8.h>
#include <Debug/Debug.h>

SYS_DECLARE_MODULE(DM_GLESLY);

namespace Glesly
{
    class Render;

    class ObjectBase
    {
        friend class ObjectPtr;

     public:
        virtual ~ObjectBase()
        {
            SYS_DEBUG_MEMBER(DM_GLESLY);
        }

        virtual void NextFrame(const SYS::TimeDelay & frame_start_time) =0;

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

     protected:
        ObjectBase(Glesly::Render & renderer);

        class ObjectCallback
        {
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

        }; // class ObjectCallback

        typedef boost::shared_ptr<ObjectCallback> ObjectCallbackPtr;

        void ExecuteCallback(const SYS::TimeDelay & frame_start_time);

        inline void Execute(ObjectCallbackPtr callback)
        {
            myCallback = callback;
        }

        inline Render & GetRenderer(void)
        {
            return myRenderer;
        }

        inline const Render & GetRenderer(void) const
        {
            return myRenderer;
        }

        inline bool IsEnabled(void) const
        {
            SYS_DEBUG_MEMBER(DM_GLESLY);
            SYS_DEBUG(DL_INFO3, "the object is " << (myEnabled ? "enabled" : "disabled"));
            return myEnabled;
        }

        ObjectWeak mySelf;

     private:
        SYS_DEFINE_CLASS_NAME("Glesly::ObjectBase");

        Render & myRenderer;

        bool myEnabled;

        int myCallbackTimeLimit;

        ObjectCallbackPtr myCallback;

    }; // class ObjectBase

} // namespace Glesly

#endif /* __GLESLY_SRC_OBJECT_BASE_H_INCLUDED__ */

/* * * * * * * * * * * * * End - of - File * * * * * * * * * * * * * * */
