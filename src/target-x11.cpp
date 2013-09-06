/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * Project:     Glesly: my GLES-based rendering library
 * Purpose:     C++ wrapper for GL/ES - X11-specific classes
 * Author:      György Kövesdi (kgy@teledigit.eu)
 * Licence:     GPL (see file 'COPYING' in the project root for more details)
 * Comments:    
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <glesly/error.h>
#include <Threads/Threads.h>

#include "target-x11.h"

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
 *                                                                                       *
 *       class TargetX11:                                                                *
 *                                                                                       *
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

using namespace Glesly;

TargetX11::TargetX11(int width, int height, bool fullscreen_on_ebmedded):
    myWidth(width),
    myHeight(height),
    x11Window(0),
    x11Display(0),
    x11Screen(0),
    x11Colormap(0)
{
 SYS_DEBUG_MEMBER(DM_GLESLY);

 x11Display = XOpenDisplay(0);
 if (!x11Display) {
    throw Error("Could not open X display");
 }

 x11Screen = XDefaultScreen(x11Display);
 int depth = DefaultDepth(x11Display, x11Screen);
 if (!XMatchVisualInfo( x11Display, x11Screen, depth, TrueColor, &x11Visual)) {
    throw Error("Could not find Visual Info");
 }

 bool fullscreen_request = myWidth < 8; // magic number :-)
 if (!fullscreen_request && fullscreen_on_ebmedded) {
    if (XDisplayWidth(x11Display, x11Screen) == myWidth && XDisplayHeight(x11Display, x11Screen) == myHeight) {
        fullscreen_request = true;
    }
 }

 if (fullscreen_request) {
    // Full-screen mode:
    myWidth = FULLSCREEN_SINGLE;
    SetupFullscreen();
 } else {
    // Window mode:
    SetupScreen();
    CreateWindow(myWidth, myHeight);
 }

 XFlush(x11Display);
}

TargetX11::~TargetX11()
{
 SYS_DEBUG_MEMBER(DM_GLESLY);

 if (x11Display) {
    if (x11Window) {
        XDestroyWindow(x11Display, x11Window);
    }
    if (x11Colormap) {
        XFreeColormap(x11Display, x11Colormap);
    }
    XCloseDisplay(x11Display);
 }
}

void TargetX11::SetupScreen(void)
{
 SYS_DEBUG_MEMBER(DM_GLESLY);

 int w = XDisplayWidth(x11Display, x11Screen);
 if (myWidth > w) {
    myWidth = w;
 }

 int h = XDisplayHeight(x11Display, x11Screen);
 if (myHeight > h) {
    myHeight = h;
 }
}

void TargetX11::SetupFullscreen(void)
{
 SYS_DEBUG_MEMBER(DM_GLESLY);

 int redirect = 0;

 XEvent xev;
 memset(&xev, 0, sizeof(xev));

 switch (myWidth) {
    case FULLSCREEN_REDIRECT:
        SYS_DEBUG(DL_INFO1, "Fullscreen: override_redirect mode");
        redirect = 1;
    break;
    case FULLSCREEN_SINGLE:
        SYS_DEBUG(DL_INFO1, "Fullscreen: one-monitor mode");
        xev.xclient.message_type = XInternAtom(x11Display, "_NET_WM_STATE", False);
        xev.xclient.data.l[0] = 1;
        xev.xclient.data.l[1] = XInternAtom(x11Display, "_NET_WM_STATE_FULLSCREEN", False);
        xev.xclient.data.l[2] = 0;
    break;
    case FULLSCREEN_MULTI:
        SYS_DEBUG(DL_INFO1, "Fullscreen: multi-monitor mode");
        xev.xclient.message_type = XInternAtom(x11Display, "_NET_WM_FULLSCREEN_MONITORS", False);
        xev.xclient.data.l[0] = (myHeight >> 12) & 15;  /* Topmost monitor number   */
        xev.xclient.data.l[1] = (myHeight >>  8) & 15;  /* Bottommost               */
        xev.xclient.data.l[2] = (myHeight >>  4) & 15;  /* Leftmost                 */
        xev.xclient.data.l[3] = (myHeight >>  0) & 15;  /* Rightmost                */
        xev.xclient.data.l[4] = 0;                      /* Source Indication        */
    break;
    default:
        throw Error("Invalid resolution");
    break;
 }

 CreateWindow(10, 10, redirect);

 myWidth = XDisplayWidth(x11Display, x11Screen);
 myHeight = XDisplayHeight(x11Display, x11Screen);

 SYS_DEBUG(DL_INFO3, "Size: " << myWidth << "x" << myHeight);

 if (redirect) {
    // Not necessary to send any message:
    return;
 }

 xev.type = ClientMessage;
 xev.xclient.window = x11Window;
 xev.xclient.format = 32;

 if (!XSendEvent(x11Display, RootWindow(x11Display, x11Screen), False, SubstructureRedirectMask | SubstructureNotifyMask, &xev)) {
    throw Error("Could not XSendEvent()");
 }

 myDeleteMessage = XInternAtom(x11Display, "WM_DELETE_WINDOW", False);
 XSetWMProtocols(x11Display, x11Window, &myDeleteMessage, 1);
}

void TargetX11::CreateWindow(int width, int height, int redirect)
{
 SYS_DEBUG_MEMBER(DM_GLESLY);

 SYS_DEBUG(DL_INFO1, "Creating window: size=" << width << "x" << height);

 x11Colormap = XCreateColormap(x11Display, RootWindow(x11Display, x11Screen), x11Visual.visual, AllocNone);

 XSetWindowAttributes attrib;
 memset(&attrib, 0, sizeof(attrib));

 attrib.override_redirect = redirect;
 attrib.colormap = x11Colormap;
 attrib.event_mask = StructureNotifyMask | ExposureMask | ButtonPressMask | ButtonReleaseMask | KeyPressMask | KeyReleaseMask;

 unsigned int mask = CWBackPixel | CWBorderPixel | CWEventMask | CWColormap;

 x11Window = XCreateWindow(x11Display, RootWindow(x11Display, x11Screen), 0, 0, width, height, 0, CopyFromParent, InputOutput, CopyFromParent, mask, &attrib);

 XMapWindow(x11Display, x11Window);
}

EGLDisplay TargetX11::GetEGLDisplay(void)
{
 return eglGetDisplay((EGLNativeDisplayType)x11Display);
}

EGLSurface TargetX11::CreateWindowSurface(EGLDisplay display, EGLConfig config)
{
 SYS_DEBUG_MEMBER(DM_GLESLY);

 EGLSurface result = eglCreateWindowSurface(display, config, (EGLNativeWindowType)x11Window, NULL);
 if (eglGetError() != EGL_SUCCESS) {
    throw Error("Could not eglCreateWindowSurface()");
 }

 return result;
}

void TargetX11::EnterEventLoop(Threads::Thread & caller)
{
 SYS_DEBUG_MEMBER(DM_GLESLY);

 while (!caller.Finished()) {
    ProcessPendingEvents();
    usleep(25000);
 }
}

void TargetX11::ProcessPendingEvents(void)
{
 SYS_DEBUG_MEMBER(DM_GLESLY);

 Window root;
 Window child;
 int rx, ry, wx, wy;
 unsigned int mask;

 bool result;

 {
    Threads::Lock _l(GetGraphicMutex());
    result = XQueryPointer(x11Display, x11Window, &root, &child, &rx, &ry, &wx, &wy, &mask);
 }

 if (result) {
    if (wx >= 0 && wx < myWidth && wy >= 0 && wy < myHeight) {
        ShiftState(mask & ShiftMask);
        AltState(mask & LockMask);
        ControlState(mask & ControlMask);
        MousePosition(wx, wy);
        SYS::TimeDelay now;
        now.SetNow();
        MouseButtonState(0, mask & Button1Mask, now);
        MouseButtonState(1, mask & Button2Mask, now);
        MouseButtonState(2, mask & Button3Mask, now);
        MouseButtonState(3, mask & Button4Mask, now);
        MouseButtonState(4, mask & Button5Mask, now);
    }
 }

 int messages = XPending(x11Display);
 for( int i = 0; i < messages; i++ ) {
    XEvent event;
    {
        Threads::Lock _l(GetGraphicMutex());
        XNextEvent(x11Display, &event);
    }
    switch(event.type)
    {
        case ButtonPress: // Mouse click
            SYS_DEBUG(DL_INFO2, "X11: Button press event...");
        break;
        case ButtonRelease: // Mouse click
            SYS_DEBUG(DL_INFO2, "X11: Button release event...");
        break;
        case ClientMessage:
            if (event.xclient.data.l[0] == (long)myDeleteMessage) {
                CloseRequest();
            }
        break;
        default:
            SYS_DEBUG(DL_INFO3, "X11: Unhandled event: " << (int)event.type << " ...");
        break;
    }
 }
}

/* * * * * * * * * * * * * End - of - File * * * * * * * * * * * * * * */
