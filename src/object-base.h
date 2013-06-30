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

#include <glesly/object-ptr.h>
#include <International/utf8.h>
#include <Debug/Debug.h>

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

        virtual void NextFrame(void) =0;

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
        ObjectBase(Glesly::Render & renderer):
            myRenderer(renderer),
            myEnabled(true)
        {
            SYS_DEBUG_MEMBER(DM_GLESLY);
        }

        class ObjectCallback
        {
            friend class Glesly::ObjectBase;
            virtual void Execute(Glesly::ObjectBase & obj) =0;
        }; // class ObjectCallback

        typedef boost::shared_ptr<ObjectCallback> ObjectCallbackPtr;

        void ExecuteCallback(void)
        {
            ObjectCallbackPtr executor = myCallback;
            if (executor.get()) {
                myCallback.reset();
                executor->Execute(*this);
            }
        }

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

        ObjectCallbackPtr myCallback;

    }; // class ObjectBase

} // namespace Glesly

#endif /* __GLESLY_SRC_OBJECT_BASE_H_INCLUDED__ */

/* * * * * * * * * * * * * End - of - File * * * * * * * * * * * * * * */
