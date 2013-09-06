/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * Project:     Glesly: my GLES-based rendering library
 * Purpose:     C++ wrapper for GL/ES - X11-specific classes
 * Author:      György Kövesdi (kgy@teledigit.eu)
 * Licence:     GPL (see file 'COPYING' in the project root for more details)
 * Comments:    
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef __GLESLY_SRC_TARGET_X11_H_INCLUDED__
#define __GLESLY_SRC_TARGET_X11_H_INCLUDED__

#include <glesly/target.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>

namespace Glesly
{
    class TargetX11: public Glesly::Target
    {
     public:
        static TargetPtr Create(int width = 0, int height = 0, bool fullscreen_on_ebmedded = false);

        enum ScreenModes {
            FULLSCREEN_REDIRECT     = 0,
            FULLSCREEN_SINGLE,
            FULLSCREEN_MULTI
        };

     protected:
        TargetX11(int width, int height, bool fullscreen_on_ebmedded);
        virtual ~TargetX11();

        int myWidth;
        int myHeight;

        Window x11Window;
        Display* x11Display;
        long x11Screen;
        XVisualInfo x11Visual;
        Colormap x11Colormap;
        Atom myDeleteMessage;

     private:
        SYS_DEFINE_CLASS_NAME("Glesly::TargetX11");

        virtual EGLDisplay GetEGLDisplay(void);
        virtual EGLSurface CreateWindowSurface(EGLDisplay display, EGLConfig config);
        virtual void EnterEventLoop(Threads::Thread &);

        virtual int GetWidth(void) const
        {
            return myWidth;
        }

        virtual int GetHeight(void) const
        {
            return myHeight;
        }

        void ProcessPendingEvents(void);
        void SetupFullscreen(void);
        void SetupScreen(void);
        void CreateWindow(int width, int height, int redirect = 0);

    }; // class TargetX11

    inline TargetPtr TargetX11::Create(int width, int height, bool fullscreen_on_ebmedded)
    {
        return TargetPtr(static_cast<Glesly::Target*>(new TargetX11(width, height, fullscreen_on_ebmedded)));
    }

} // namespace Glesly

#endif /* __GLESLY_SRC_TARGET_X11_H_INCLUDED__ */

/* * * * * * * * * * * * * End - of - File * * * * * * * * * * * * * * */
