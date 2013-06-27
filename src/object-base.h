/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * Project:     Glesly: my GLES-based rendering library
 * Purpose:     Base class for all Objects
 * Author:      György Kövesdi (kgy@teledigit.eu)
 * Licence:     GPL (see file 'COPYING' in the project root for more details)
 * Comments:    
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef __GLESLY_SRC_OBJECT_BASE_H_INCLUDED__
#define __GLESLY_SRC_OBJECT_BASE_H_INCLUDED__

#include <glesly/object-ptr.h>
#include <Debug/Debug.h>

namespace Glesly
{
    class Render;

    class ObjectBase
    {
        friend class ObjectPtr;

     public:
        virtual ~ObjectBase()
        {
            SYS_DEBUG_MEMBER(DM_GLESLY);
        }

        virtual void NextFrame(void) =0;
        virtual bool MouseClick(float x, float y, int index, int count) =0;

     protected:
        ObjectBase(Glesly::Render & renderer):
            myProgram(renderer)
        {
            SYS_DEBUG_MEMBER(DM_GLESLY);
        }

        inline Render & GetProgram(void)
        {
            return myProgram;
        }

        inline const Render & GetProgram(void) const
        {
            return myProgram;
        }

        ObjectWeak mySelf;

     private:
        SYS_DEFINE_CLASS_NAME("Glesly::ObjectBase");

        Render & myProgram;

    }; // class ObjectBase

} // namespace Glesly

#endif /* __GLESLY_SRC_OBJECT_BASE_H_INCLUDED__ */

/* * * * * * * * * * * * * End - of - File * * * * * * * * * * * * * * */
