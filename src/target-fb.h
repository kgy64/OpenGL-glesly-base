/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * Project:     Glesly: my GLES-based rendering library
 * Purpose:     C++ wrapper for GL/ES - FB-specific classes
 * Author:      György Kövesdi (kgy@teledigit.eu)
 * Licence:     GPL (see file 'COPYING' in the project root for more details)
 * Comments:    
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef __GLESLY_SRC_TARGET_FB_H_INCLUDED__
#define __GLESLY_SRC_TARGET_FB_H_INCLUDED__

#include <glesly/target.h>

namespace Glesly
{
    class TargetFB: public Glesly::Target
    {
     public:
        static TargetPtr Create(int index);

     protected:
        TargetFB(int index);
        virtual ~TargetFB();

        int myFd;

        int myIndex;

        int myWidth;
        int myHeight;

     private:
        SYS_DEFINE_CLASS_NAME("Glesly::TargetFB");

        virtual EGLDisplay GetEGLDisplay(void);
        virtual EGLSurface CreateWindowSurface(EGLDisplay display, EGLConfig config);
        virtual void ProcessPendingEvents(Glesly::Main &);
        virtual void Wait4Sync(void);

        virtual int GetWidth(void) const
        {
            return myWidth;
        }

        virtual int GetHeight(void) const
        {
            return myHeight;
        }

    }; // class TargetFB

    inline TargetPtr TargetFB::Create(int index = 0)
    {
        return TargetPtr(static_cast<Glesly::Target*>(new TargetFB(index)));
    }

} // namespace Glesly

#endif /* __GLESLY_SRC_TARGET_FB_H_INCLUDED__ */

/* * * * * * * * * * * * * End - of - File * * * * * * * * * * * * * * */
