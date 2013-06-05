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

    class Backend
    {
     public:
        Backend(TargetPtr & target);
        virtual ~Backend();

        void SwapBuffers(void);

        inline void ProcessPendingEvents(Glesly::Main & main)
        {
            myTarget->ProcessPendingEvents(main);
        }

        Glesly::Target & GetTarget(void)
        {
            return *myTarget;
        }

     protected:
        TargetPtr myTarget;

        EGLint myEGLMajorVersion;
        EGLint myEGLMinorVersion;
        EGLDisplay myDisplay;
        EGLConfig myConfig;
        EGLSurface mySurface;
        EGLContext myContext;

     private:
        SYS_DEFINE_CLASS_NAME("Glesly::Backend");

        void InitDisplay(void);
        void InitSurface(void);

    }; // class Backend

} // namespace Glesly

#endif /* __GLESLY_SRC_BACKEND_H_INCLUDED__ */

/* * * * * * * * * * * * * End - of - File * * * * * * * * * * * * * * */
