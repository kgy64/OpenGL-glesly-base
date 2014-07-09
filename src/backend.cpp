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

Backend::Backend(void):
    myParent(NULL),
    myEGLMajorVersion(0),
    myEGLMinorVersion(0),
    myDisplay(0),
    myConfig(0),
    mySurface(0),
    myContext(0)
{
 SYS_DEBUG_MEMBER(DM_GLESLY);
}

Backend::Backend(TargetPtr & target):
    Backend()
{
 SYS_DEBUG_MEMBER(DM_GLESLY);

 myTarget = target;

 if (myTarget) {
    myTarget->RegisterParent(this);
 }
}

Backend::~Backend()
{
 SYS_DEBUG_MEMBER(DM_GLESLY);

 if (myTarget) {
    myTarget->RegisterParent(NULL); // Unregister
 }
}

void Backend::Retarget(TargetPtr & target)
{
 if (myTarget == target) {
    return;
 }

 if (myTarget) {
    myTarget->RegisterParent(NULL); // Unregister
 }

 myTarget = target;

 if (myTarget) {
    myTarget->RegisterParent(this);
 }
}

void Backend::Initialize(void)
{
 SYS_DEBUG_MEMBER(DM_GLESLY);

 if (!myTarget) {
    return;
 }

 myTarget->Initialize();

 InitDisplay();
 InitSurface();
}

void Backend::Cleanup(void)
{
 SYS_DEBUG_MEMBER(DM_GLESLY);

 eglMakeCurrent(myDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
 eglDestroyContext(myDisplay, myContext);
 eglDestroySurface(myDisplay, mySurface);
 eglTerminate(myDisplay);
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

 Glesly::TargetPtr target = GetTarget();

 if (!target) {
    // There is no target: It means that the main window is being closed.
    // Note: it does not happen on all platforms (but at least on Android it dores).
    return;
 }

 Threads::Lock _l(target->GetGraphicMutex());

 eglSwapBuffers(myDisplay, mySurface);
 if (eglGetError() != EGL_SUCCESS) {
    throw Error("Could not eglSwapBuffers()");
 }
 // Note: it should be discussed...
 // target->Wait4Sync();
}

/* * * * * * * * * * * * * End - of - File * * * * * * * * * * * * * * */
