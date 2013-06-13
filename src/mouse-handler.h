/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * Project:     Glesly: my GLES-based rendering library
 * Purpose:     C++ wrapper for GL/ES - Mouse Handler Classes
 * Author:      György Kövesdi (kgy@teledigit.eu)
 * Licence:     GPL (see file 'COPYING' in the project root for more details)
 * Comments:    
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef __GLESLY_SRC_MOUSE_HANDLER_H_INCLUDED__
#define __GLESLY_SRC_MOUSE_HANDLER_H_INCLUDED__

#include <Debug/Debug.h>

SYS_DECLARE_MODULE(DM_GLESLY);

namespace Glesly
{
    class Target;
    class MouseHandler;

    class MouseButton
    {
        friend class MouseHandler;

     protected:
        MouseButton(MouseHandler & parent, int index);

        inline VIRTUAL_IF_DEBUG ~MouseButton()
        {
            SYS_DEBUG_MEMBER(DM_GLESLY);
        }

        void State(bool pressed);

     private:
        SYS_DEFINE_CLASS_NAME("Glesly::MouseHandler");

        int myIndex;

        bool myPressed;

    }; // class MouseButton

    class MouseHandler
    {
        friend class Target;

     protected:
        MouseHandler(Target & parent);

        inline VIRTUAL_IF_DEBUG ~MouseHandler()
        {
            SYS_DEBUG_MEMBER(DM_GLESLY);
        }

        void Position(int x, int y);

        inline void ButtonState(int index, bool pressed)
        {
            ASSERT_FATAL(index >= 0 && index < 5, "Mouse button index out of range");
            buttons[index].State(pressed);
        }

     private:
        SYS_DEFINE_CLASS_NAME("Glesly::MouseHandler");

        Target & myParent;

        int myX;

        int myY;

        MouseButton buttons[5];

    }; // class MouseHandler

}; // namespace Glesly

#endif /* __GLESLY_SRC_MOUSE_HANDLER_H_INCLUDED__ */

/* * * * * * * * * * * * * End - of - File * * * * * * * * * * * * * * */
