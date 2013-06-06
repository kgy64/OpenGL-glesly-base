/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * Project:     Glesly: my GLES-based rendering library
 * Purpose:     C++ wrapper for GL/ES - Main class
 * Author:      György Kövesdi (kgy@teledigit.eu)
 * Licence:     GPL (see file 'COPYING' in the project root for more details)
 * Comments:    
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <GLES/gl.h>

#include <glesly/program.h>
#include <glesly/error.h>

#include "glesly-main.h"

using namespace Glesly;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
 *                                                                                       *
 *     class Main:                                                                       *
 *                                                                                       *
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

Main::Main(TargetPtr & backend):
    myBackend(backend),
    myFinished(false)
{
 SYS_DEBUG_MEMBER(DM_GLESLY);
}

Main::~Main()
{
 SYS_DEBUG_MEMBER(DM_GLESLY);
}

void Main::Run(void)
{
 SYS_DEBUG_MEMBER(DM_GLESLY);

 myFinished = false;

 Initialize();

 for (RenderList::iterator i = myRenders.begin(); i != myRenders.end(); ++i) {
    (*i)->Link();
    (*i)->Initialize();
 }

 while (!myFinished) {
    NextFrame();
    for (RenderList::iterator i = myRenders.begin(); i != myRenders.end(); ++i) {
        if (myFinished) {
            goto finished;
        }
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        (*i)->NextFrame();
    }
    myBackend.SwapBuffers();
    myBackend.ProcessPendingEvents(*this);
 }

finished:;

 for (RenderList::iterator i = myRenders.begin(); i != myRenders.end(); ++i) {
    (*i)->Cleanup();
 }

 Cleanup();
}

void Main::Clear(void)
{
 SYS_DEBUG_MEMBER(DM_GLESLY);

 SYS_DEBUG(DL_INFO3, " - glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);");

 glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
 if (eglGetError() != EGL_SUCCESS) {
    throw Error("Could not glClear()");
 }
}

/* * * * * * * * * * * * * End - of - File * * * * * * * * * * * * * * */
