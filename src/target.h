/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * Project:     Glesly: my GLES-based rendering library
 * Purpose:     C++ wrapper for GL/ES - Target Base Class
 * Author:      György Kövesdi (kgy@teledigit.eu)
 * Licence:     GPL (see file 'COPYING' in the project root for more details)
 * Comments:    
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef __GLESLY_SRC_GLESLY_H_INCLUDED__
#define __GLESLY_SRC_GLESLY_H_INCLUDED__

#include <boost/shared_ptr.hpp>
#include <EGL/egl.h>

#include <glesly/mouse-handler.h>
#include <Debug/Debug.h>

SYS_DECLARE_MODULE(DM_GLESLY);

namespace Glesly
{
    class Main;

    class Target
    {
     public:
        virtual ~Target();

        virtual EGLDisplay GetEGLDisplay(void) =0;
        virtual EGLSurface CreateWindowSurface(EGLDisplay display, EGLConfig config) =0;
        virtual int GetWidth(void) const =0;
        virtual int GetHeight(void) const =0;
        virtual void ProcessPendingEvents(Glesly::Main &) { }

     protected:
        Target(void);

        void ShiftState(bool pressed);
        void AltState(bool pressed);
        void ControlState(bool pressed);

        inline void MousePosition(int x, int y)
        {
            myMouse.Position(x, y);
        }

        inline void MouseButtonState(int index, bool pressed, const SYS::TimeDelay & time)
        {
            myMouse.ButtonState(index, pressed, time);
        }

        inline void MouseButtonState(int index, bool pressed)
        {
            SYS::TimeDelay now;
            myMouse.ButtonState(index, pressed, now);
        }

     private:
        SYS_DEFINE_CLASS_NAME("Glesly::Target");

        Glesly::MouseHandler myMouse;

    }; // class Target

    typedef boost::shared_ptr<Glesly::Target> TargetPtr;

} // namespace Glesly

#endif /* __GLESLY_SRC_GLESLY_H_INCLUDED__ */

/* * * * * * * * * * * * * End - of - File * * * * * * * * * * * * * * */
