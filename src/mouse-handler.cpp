/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * Project:     Glesly: my GLES-based rendering library
 * Purpose:     C++ wrapper for GL/ES - Mouse Handler Classes
 * Author:      György Kövesdi (kgy@teledigit.eu)
 * Licence:     GPL (see file 'COPYING' in the project root for more details)
 * Comments:    
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "mouse-handler.h"

using namespace Glesly;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
 *                                                                                       *
 *     class MouseHandler:                                                               *
 *                                                                                       *
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

MouseHandler::MouseHandler(Target & parent):
    myParent(parent),
    myX(-1),
    myY(-1),
    buttons({
        MouseButton(*this, 0),
        MouseButton(*this, 1),
        MouseButton(*this, 2),
        MouseButton(*this, 3),
        MouseButton(*this, 4)
    })
{
 SYS_DEBUG_MEMBER(DM_GLESLY);
}

void MouseHandler::Position(int x, int y)
{
 SYS_DEBUG_MEMBER(DM_GLESLY);

 if (x != myX || y != myY) {
    myX = x;
    myY = y;
    SYS_DEBUG(DL_INFO3, "KGY: Mouse position=" << myX << "x" << myY);
 }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
 *                                                                                       *
 *     class MouseButton:                                                                *
 *                                                                                       *
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

MouseButton::MouseButton(MouseHandler & parent, int index):
    myIndex(index),
    myPressed(false)
{
 SYS_DEBUG_MEMBER(DM_GLESLY);
}

void MouseButton::State(bool pressed)
{
 SYS_DEBUG_MEMBER(DM_GLESLY);

 if (myPressed ^ pressed) {
    myPressed = pressed;
    SYS_DEBUG(DL_INFO3, "KGY: Mouse button " << myIndex << " is " << (pressed ? "pressed" : "released"));
 }
}

/* * * * * * * * * * * * * End - of - File * * * * * * * * * * * * * * */
