/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * Project:     Glesly: my GLES-based rendering library
 * Purpose:     C++ wrapper for GL/ES - Renderer Class (Backend)
 * Author:      György Kövesdi (kgy@teledigit.eu)
 * Licence:     GPL (see file 'COPYING' in the project root for more details)
 * Comments:    
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "backend.h"

#include <glesly/error.h>

using namespace Glesly;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
 *                                                                                       *
 *       class Backend:                                                                  *
 *                                                                                       *
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

Backend::Backend(TargetPtr & target):
    myTarget(target),
    myParent(NULL),
    myEGLMajorVersion(0),
    myEGLMinorVersion(0),
    myDisplay(0),
    myConfig(0),
    mySurface(0),
    myContext(0)
{
 SYS_DEBUG_MEMBER(DM_GLESLY);

 myTarget->RegisterParent(this);

 InitDisplay();
 InitSurface();
}

Backend::~Backend()
{
 SYS_DEBUG_MEMBER(DM_GLESLY);

 myTarget->RegisterParent(NULL); // unregister
}

void Backend::CloseRequest(void)
{
 SYS_DEBUG_MEMBER(DM_GLESLY);
 if (myParent) {
    myParent->CloseRequest();
 }
}

void Backend::MouseClick(int x, int y, int index, int count)
{
 SYS_DEBUG_MEMBER(DM_GLESLY);
 if (myParent) {
    myParent->MouseClick(x, y, index, count);
 }
}

void Backend::InitDisplay(void)
{
 SYS_DEBUG_MEMBER(DM_GLESLY);

 myDisplay = myTarget->GetEGLDisplay();
 if (!eglInitialize(myDisplay, &myEGLMajorVersion, &myEGLMinorVersion)) {
    throw Error("Could not eglInitialize()");
 }
 eglBindAPI(EGL_OPENGL_ES_API);
 if (eglGetError() != EGL_SUCCESS) {
    throw Error("Could not eglBindAPI()");
 }
 EGLint attribs[] = {
    EGL_SURFACE_TYPE,       EGL_WINDOW_BIT,
    EGL_RENDERABLE_TYPE,    EGL_OPENGL_ES2_BIT,
    EGL_RED_SIZE,           5,
    EGL_GREEN_SIZE,         6,
    EGL_BLUE_SIZE,          5,
    EGL_ALPHA_SIZE,         0,
    EGL_DEPTH_SIZE,         16,
    EGL_NONE
 };
 int configs;
 if (!eglChooseConfig(myDisplay, attribs, &myConfig, 1, &configs) || (configs != 1)) {
    throw Error("Could not eglChooseConfig()");
 }
}

void Backend::InitSurface(void)
{
 SYS_DEBUG_MEMBER(DM_GLESLY);

 mySurface = myTarget->CreateWindowSurface(myDisplay, myConfig);
 EGLint attribs[] = {
    EGL_CONTEXT_CLIENT_VERSION, 2,
    EGL_NONE
 };
 myContext = eglCreateContext(myDisplay, myConfig, NULL, attribs);
 if (eglGetError() != EGL_SUCCESS) {
    throw Error("Could not eglCreateContext()");
 }
 eglMakeCurrent(myDisplay, mySurface, mySurface, myContext);
 if (eglGetError() != EGL_SUCCESS) {
    throw Error("Could not eglMakeCurrent()");
 }
}

void Backend::SwapBuffers(void)
{
 SYS_DEBUG_MEMBER(DM_GLESLY);

 eglSwapBuffers(myDisplay, mySurface);
 if (eglGetError() != EGL_SUCCESS) {
    throw Error("Could not eglSwapBuffers()");
 }
 // Note: it should be discussed...
 // GetTarget().Wait4Sync();
}

/* * * * * * * * * * * * * End - of - File * * * * * * * * * * * * * * */
