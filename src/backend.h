/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * Project:     Glesly: my GLES-based rendering library
 * Purpose:     C++ wrapper for GL/ES - Backend Class
 * Author:      György Kövesdi (kgy@teledigit.eu)
 * Licence:     GPL (see file 'COPYING' in the project root for more details)
 * Comments:    
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef __GLESLY_SRC_BACKEND_H_INCLUDED__
#define __GLESLY_SRC_BACKEND_H_INCLUDED__

#include <boost/shared_ptr.hpp>
#include <EGL/egl.h>

#include <glesly/target.h>

SYS_DECLARE_MODULE(DM_GLESLY);

namespace Glesly
{
    class Main;

    class Backend: private Glesly::TargetHolder
    {
     public:
        Backend(void);
        Backend(TargetPtr & target);
        virtual ~Backend();

        void SwapBuffers(void);
        void Initialize(void);
        void Cleanup(void);
        void Retarget(TargetPtr & target);

        inline Glesly::TargetPtr GetTarget(void)
        {
            return myTarget;
        }

        inline bool IsRunning(void) const
        {
            return (bool)myTarget;
        }

        inline void RegisterParent(TargetHolder * parent = NULL)
        {
            myParent = parent;
        }

     protected:
        TargetPtr myTarget;

        TargetHolder * myParent;

        EGLint myEGLMajorVersion;
        EGLint myEGLMinorVersion;
        EGLDisplay myDisplay;
        EGLConfig myConfig;
        EGLSurface mySurface;
        EGLContext myContext;

     private:
        SYS_DEFINE_CLASS_NAME("Glesly::Backend");

        virtual void CloseRequest(void) override;
        virtual void MouseClick(int x, int y, int index, int count) override;

        void InitDisplay(void);
        void InitSurface(void);

    }; // class Backend

} // namespace Glesly

#endif /* __GLESLY_SRC_BACKEND_H_INCLUDED__ */

/* * * * * * * * * * * * * End - of - File * * * * * * * * * * * * * * */
