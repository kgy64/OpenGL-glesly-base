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
 int fd = open(devicename, O_RDWR);
 ASSERT_STD(fd > 2);
 struct fb_var_screeninfo screeninfo_var;
 if (ioctl(fd, FBIOGET_VSCREENINFO, &screeninfo_var) == 0) {
    myWidth = screeninfo_var.xres;
    myHeight = screeninfo_var.yres;
 }
 close(fd);
}

TargetFB::~TargetFB()
{
 SYS_DEBUG_MEMBER(DM_GLESLY);
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

void TargetFB::ProcessPendingEvents(Glesly::Main & main)
{
 SYS_DEBUG_MEMBER(DM_GLESLY);

}

/* * * * * * * * * * * * * End - of - File * * * * * * * * * * * * * * */
