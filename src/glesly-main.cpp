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

Main::Main(void):
    myTimer(*this)
{
 GetBackend().RegisterParent(this);
}

Main::Main(TargetPtr & target):
    myTimer(*this),
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

 if (!IsRunning()) {
    usleep(333333);     // cca 3 Hz
    return;
 }

 GetBackend().Initialize(); // Must be called from this thread

 Initialize();

 for (RenderList::iterator i = myRenders.begin(); i != myRenders.end(); ++i) {
    (*i)->ProgramInit();
    (*i)->Initialize();
 }

 while (!ToBeFinished()) {
    Glesly::TargetPtr target = GetBackend().GetTarget();

    if (!target) {
        goto finished;
    }

    SYS_DEBUG(DL_INFO3, "Starting Frame...");

    for (RenderList::iterator i = myRenders.begin(); i != myRenders.end(); ++i) {
        if (ToBeFinished()) {
            goto finished;
        }
        (*i)->NextFrame(myFrameStartTime);
    }

    timerSemaphore.Post();

    SYS::TimeDelay now;
    now.SetNow();

    int elapsed = (now - myFrameStartTime).ToMicrosecond();

    static constexpr int FRAME_DELAY_USEC = 1000000 / 60;

    if (SYS_DEBUG_ON) {
        // In the debug case, slow down the rendering to prevent log flood:
        usleep(100000); // cca. 10 Hz
    } else {
        int time_diff = FRAME_DELAY_USEC - elapsed;

        if (time_diff > 2000) {
            myFrameStartTime.AddMicrosecond(FRAME_DELAY_USEC);
            usleep(time_diff);
        } else {
            myFrameStartTime = now;
        }
    }

    GetBackend().SwapBuffers();

    SYS_DEBUG(DL_INFO2, "Loop Finished.");
 }

finished:;

 SYS_DEBUG(DL_INFO2, "Loop Exited.");

 myTimer.Kill();

 for (RenderList::iterator i = myRenders.begin(); i != myRenders.end(); ++i) {
    (*i)->Cleanup();
    (*i)->ProgramCleanup();
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

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
 *                                                                                       *
 *     class Main::TimerThread:                                                          *
 *                                                                                       *
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

Main::TimerThread::TimerThread(Main & parent):
    myParent(parent)
{
 SYS_DEBUG_MEMBER(DM_GLESLY);

 Start(4*65536);
}

Main::TimerThread::~TimerThread(void)
{
 SYS_DEBUG_MEMBER(DM_GLESLY);

}

int Main::TimerThread::main(void)
{
 SYS_DEBUG_MEMBER(DM_GLESLY);

 Main::RenderList & renders = myParent.getRenderers();
 Threads::Semaphore & semaphore = myParent.getTimerSemaphore();

 while (!ToBeFinished()) {
    // Wait for trigger from render thread first:
    semaphore.Wait();

    //
    myParent.RenderTimer();

    // Call timer functions:
    for (RenderList::iterator i = renders.begin(); i != renders.end(); ++i) {
        if (ToBeFinished()) {
            return 0;
        }
        (*i)->Timer();
    }
 }

 return 0;
}

/* * * * * * * * * * * * * End - of - File * * * * * * * * * * * * * * */
