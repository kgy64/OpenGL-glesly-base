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
        virtual ~Main();

        void Run(void);
        void Clear(void);

        virtual void Initialize(void) =0;
        virtual void NextFrame(void) =0;
        virtual void Cleanup(void) =0;

        virtual void WindowSizeChanged(int w, int h) { }

        inline void InsertRenderer(RenderPtr rp)
        {
            myRenders.push_front(rp);
        }

        inline void AppendRenderer(RenderPtr rp)
        {
            myRenders.push_back(rp);
        }

        inline Glesly::Backend & GetBackend(void)
        {
            return myBackend;
        }

     protected:
        Glesly::CameraMatrix myViewMatrix;

        SYS::TimeDelay myFrameStartTime;

        virtual bool IsFinished(void) const =0;

     private:
        SYS_DEFINE_CLASS_NAME("Glesly::Main");

        virtual void MouseClick(int x, int y, int index, int count);

        Glesly::Backend myBackend;

        typedef std::list<RenderPtr> RenderList;
        RenderList myRenders;

    }; // class Main

} // namespace Glesly

#endif /* __GLESLY_SRC_GLESLY_MAIN_H_INCLUDED__ */

/* * * * * * * * * * * * * End - of - File * * * * * * * * * * * * * * */
