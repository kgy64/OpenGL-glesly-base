/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * Project:     Glesly: my GLES-based rendering library
 * Purpose:     C++ wrapper for GL/ES - Main class
 * Author:      György Kövesdi (kgy@teledigit.eu)
 * Licence:     GPL (see file 'COPYING' in the project root for more details)
 * Comments:    
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "glesly-main.h"

#include <glesly/program.h>
#include <glesly/error.h>

#include <GLES2/gl2.h>

#include <unistd.h>

using namespace Glesly;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
 *                                                                                       *
 *     class Main:                                                                       *
 *                                                                                       *
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

Main::Main(void):
    myTimer(new TimerThread(*this))
{
 GetBackend().RegisterParent(this);
 myTimer->Start(myTimer, 4*65536);
}

Main::Main(TargetPtr & target):
    myTimer(new TimerThread(*this)),
    myBackend(target)
{
 SYS_DEBUG_MEMBER(DM_GLESLY);

 GetBackend().RegisterParent(this);
 myTimer->Start(myTimer, 4*65536);
}

Main::~Main()
{
 SYS_DEBUG_MEMBER(DM_GLESLY);

 GetBackend().RegisterParent(NULL); // unregister
}

void Main::Run(void)
{
 SYS_DEBUG_MEMBER(DM_GLESLY);

 {
    Glesly::TargetPtr target = GetBackend().GetTarget();

    // First: wait for the target initialization:
    // Note: it is necessary for e.g. Android where the screen can be assigned later.
    if (!target) {
        return;
    }
 }

 GetBackend().Initialize(); // Must be called from this thread

 Initialize();

 for (RenderList::iterator i = myRenders.begin(); i != myRenders.end(); ++i) {
    (*i)->ProgramInit();
    (*i)->Initialize();
 }

 myFrameStartTime.SetNow();

 while (!ToBeFinished()) {
    Glesly::TargetPtr target = GetBackend().GetTarget();

    if (!target) {
        goto finished;
    }

    SYS_DEBUG(DL_INFO3, "Starting Frame...");

    glClearColor(0.3, 0.5, 0.7, 1.0);

    Clear();

    for (RenderList::iterator i = myRenders.begin(); i != myRenders.end(); ++i) {
        if (ToBeFinished()) {
            goto finished;
        }
        (*i)->NextFrame(myFrameStartTime);
    }

    timerSemaphore.Post();

    GetBackend().SwapBuffers();

    {
        SYS::TimeDelay now;
        now.SetNow();

        int elapsed = (now - myFrameStartTime).ToMicrosecond();

        static constexpr int FRAME_DELAY_USEC = 1000000 / 60;

        // Calculate the time to wait to fill the actual frame time to the expected value:
        int time_diff = FRAME_DELAY_USEC - elapsed;

        if (time_diff > 2000) {
            // We must wait more than 2 ms:
            myFrameStartTime.AddMicrosecond(FRAME_DELAY_USEC);
            usleep(time_diff);
        } else {
            // Not necessary to wait:
            myFrameStartTime = now;
        }
    }
 }

finished:;

 SYS_DEBUG(DL_INFO1, "Frame Loop Exited");

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

 Threads::LockPtr _l = GetGraphicalLock();

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
    Threads::Thread("TimerThread"),
    myParent(parent)
{
 SYS_DEBUG_MEMBER(DM_GLESLY);
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
