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

Main::Main(TargetPtr & target):
    myBackend(target)
{
 SYS_DEBUG_MEMBER(DM_GLESLY);

 GetBackend().RegisterParent(this);
}

Main::~Main()
{
 SYS_DEBUG_MEMBER(DM_GLESLY);

 GetBackend().RegisterParent(NULL); // unregister
}

void Main::Run(void)
{
 SYS_DEBUG_MEMBER(DM_GLESLY);

 Initialize();

 for (RenderList::iterator i = myRenders.begin(); i != myRenders.end(); ++i) {
    (*i)->Link();
    (*i)->Initialize();
 }

 static constexpr int FRAME_DELAY_USEC  =   1000000 / 60;

 SYS::TimeDelay frameTime;
 frameTime.SetNow();

 while (!ToBeFinished()) {
    {
        Threads::Lock _l(GetBackend().GetTarget()->GetGraphicMutex());
        NextFrame();
    }

    for (RenderList::iterator i = myRenders.begin(); i != myRenders.end(); ++i) {
        if (ToBeFinished()) {
            goto finished;
        }
        (*i)->NextFrame(myFrameStartTime);
    }

    SYS::TimeDelay now;
    now.SetNow();

    int elapsed = FRAME_DELAY_USEC - (now - frameTime).ToMicrosecond();

    if (elapsed > 2000) {
        frameTime.AddMicrosecond(FRAME_DELAY_USEC);
        usleep(elapsed);
    } else {
        frameTime = now;
    }

    GetBackend().SwapBuffers();
 }

finished:;

 for (RenderList::iterator i = myRenders.begin(); i != myRenders.end(); ++i) {
    (*i)->Cleanup();
 }

 Cleanup();
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
