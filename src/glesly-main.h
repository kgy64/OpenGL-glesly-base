/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * Project:     Glesly: my GLES-based rendering library
 * Purpose:     C++ wrapper for GL/ES - Main class
 * Author:      György Kövesdi (kgy@teledigit.eu)
 * Licence:     GPL (see file 'COPYING' in the project root for more details)
 * Comments:    
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef __GLESLY_SRC_GLESLY_MAIN_H_INCLUDED__
#define __GLESLY_SRC_GLESLY_MAIN_H_INCLUDED__

#include <list>
#include <GLES2/gl2.h>

#include <Threads/Threads.h>
#include <Threads/Semaphore.h>
#include <glesly/backend.h>
#include <glesly/render.h>
#include <glesly/camera.h>

SYS_DECLARE_MODULE(DM_GLESLY);

namespace Glesly
{
    class Main: private TargetHolder
    {
     public:
        Main(void);
        Main(TargetPtr & target);
        virtual ~Main();

        void Run(void);
        void Clear(void);

        virtual void Initialize(void) =0;
        virtual void Cleanup(void) =0;

        virtual void WindowSizeChanged(int w, int h) { }
        virtual void MouseClick(int x, int y, int index, int count);

        inline Glesly::Backend & GetBackend(void)
        {
            return myBackend;
        }

        void Retarget(TargetPtr & target)
        {
            GetBackend().Retarget(target);
        }

        typedef std::list<RenderPtr> RenderList;

        inline RenderList & getRenderers(void)
        {
            return myRenders;
        }

        inline void InsertRenderer(RenderPtr rp)
        {
            myRenders.push_front(rp);
        }

        inline void AppendRenderer(RenderPtr rp)
        {
            myRenders.push_back(rp);
        }

        inline void DeleteRendeers(void)
        {
            myRenders.clear();
        }

        inline size_t getNoOfRenderers(void) const
        {
            return myRenders.size();
        }

        inline Threads::Semaphore & getTimerSemaphore(void)
        {
            return timerSemaphore;
        }

     protected:
        Glesly::CameraMatrix myViewMatrix;

        SYS::TimeDelay myFrameStartTime;

        virtual bool ToBeFinished(void) const =0;

        /// Called from the Timer Thread, after each frame
        /*! It is synchronized to the frame cycle of the OpenGL Render Thread.<br>
         *  All \ref ObjectBase::Timer() functions of the corresponding objects are also called.
         *  \note   If the timer functions are too slow, it is possible to lose the synchronization.
         *  */
        virtual void RenderTimer(void) { }

        inline bool IsRunning(void) const
        {
            return myBackend.IsRunning();
        }

        class TimerThread: public Threads::Thread
        {
         public:
            TimerThread(Main & parent);
            virtual ~TimerThread();

         protected:
            virtual int main(void) override;

            Main & myParent;

         private:
            SYS_DEFINE_CLASS_NAME("Glesly::Main::TimerThread");

        }; // class Glesly::Main::TimerThread

        Threads::Semaphore timerSemaphore;

        TimerThread myTimer;

     private:
        SYS_DEFINE_CLASS_NAME("Glesly::Main");

        Glesly::Backend myBackend;

        RenderList myRenders;

    }; // class Main

} // namespace Glesly

#endif /* __GLESLY_SRC_GLESLY_MAIN_H_INCLUDED__ */

/* * * * * * * * * * * * * End - of - File * * * * * * * * * * * * * * */
