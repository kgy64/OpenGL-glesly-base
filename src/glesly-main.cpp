/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * Project:     Glesly: my GLES-based rendering library
 * Purpose:     C++ wrapper for GL/ES - Main class
 * Author:      György Kövesdi (kgy@teledigit.eu)
 * Licence:     GPL (see file 'COPYING' in the project root for more details)
 * Comments:    
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <GLES2/gl2.h>

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

 myBackend.RegisterParent(this);
}

Main::~Main()
{
 SYS_DEBUG_MEMBER(DM_GLESLY);

 myBackend.RegisterParent(NULL); // unregister
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

void Main::CloseRequest(void)
{
 SYS_DEBUG_MEMBER(DM_GLESLY);

 SYS_DEBUG(DL_INFO1, "KGY: Closing down...");

 myFinished = true;
}

void Main::MouseClick(int x, int y, int index, int count)
{
 SYS_DEBUG_MEMBER(DM_GLESLY);

 for (RenderList::iterator i = myRenders.begin(); i != myRenders.end(); ++i) {
    (*i)->MouseClickRaw(x, y, index, count);
 }
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
