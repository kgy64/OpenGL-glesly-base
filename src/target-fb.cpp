/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * Project:     Glesly: my GLES-based rendering library
 * Purpose:     C++ wrapper for GL/ES - FB-specific classes
 * Author:      György Kövesdi (kgy@teledigit.eu)
 * Licence:     GPL (see file 'COPYING' in the project root for more details)
 * Comments:    
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <linux/fb.h>
#include <sys/ioctl.h>

#include <glesly/error.h>
#include <Threads/Threads.h>

#include "target-fb.h"

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
 *                                                                                       *
 *       class TargetFB:                                                                *
 *                                                                                       *
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

using namespace Glesly;

TargetFB::TargetFB(int index):
    myIndex(index),
    myWidth(0),
    myHeight(0)
{
 SYS_DEBUG_MEMBER(DM_GLESLY);

 char devicename[40];

 sprintf(devicename, "/dev/fb%d", myIndex);
 myFd = open(devicename, O_RDWR);
 ASSERT_STD(myFd > 2);
 struct fb_var_screeninfo screeninfo_var;
 if (ioctl(myFd, FBIOGET_VSCREENINFO, &screeninfo_var) == 0) {
    myWidth = screeninfo_var.xres;
    myHeight = screeninfo_var.yres;
 }
}

TargetFB::~TargetFB()
{
 SYS_DEBUG_MEMBER(DM_GLESLY);

 if (myFd > 2) {
    close(myFd);
 }
}

EGLDisplay TargetFB::GetEGLDisplay(void)
{
 return eglGetDisplay(EGL_DEFAULT_DISPLAY);
}

EGLSurface TargetFB::CreateWindowSurface(EGLDisplay display, EGLConfig config)
{
 SYS_DEBUG_MEMBER(DM_GLESLY);

 EGLSurface result = eglCreateWindowSurface(display, config, (EGLNativeWindowType)myIndex, NULL);
 int status = eglGetError();
 if (status != EGL_SUCCESS) {
    throw Error("Could not eglCreateWindowSurface()") << status;
 }

 return result;
}

void TargetFB::EnterEventLoop(Threads::Thread & caller)
{
 SYS_DEBUG_MEMBER(DM_GLESLY);

 while (!caller.Finished()) {
    usleep(25000); // Temporary!
 }
}

void TargetFB::Wait4Sync(void)
{
 SYS_DEBUG_MEMBER(DM_GLESLY);

 if (myFd > 2) {
    int arg = 0;
    ioctl(myFd, FBIO_WAITFORVSYNC, &arg);
 }
}

/* * * * * * * * * * * * * End - of - File * * * * * * * * * * * * * * */
