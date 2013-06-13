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

void MouseButton::State(bool pressed, const SYS::TimeDelay & time)
{
 SYS_DEBUG_MEMBER(DM_GLESLY);

 if (!myPressed) {
    if (pressed) {
        myPressed = true;
        myPreviousTime = time;
        Pressed();
    } else {
        Logic(time);
    }
 } else {
    if (!pressed) {
        myPressed = false;
        Released(time - myPreviousTime);
        myPreviousTime = time;
    } else {
        Logic(time);
    }
 }
}

void MouseButton::Logic(const SYS::TimeDelay & time)
{
 SYS_DEBUG_MEMBER(DM_GLESLY);

}

void MouseButton::Pressed(void)
{
 SYS_DEBUG_MEMBER(DM_GLESLY);

 SYS_DEBUG(DL_INFO3, "KGY: Mouse button " << myIndex << " is pressed");
}

void MouseButton::Released(const SYS::TimeDelay & time)
{
 SYS_DEBUG_MEMBER(DM_GLESLY);

 SYS_DEBUG(DL_INFO3, "KGY: Mouse button " << myIndex << " is released, time=" << time.ToMillisecond());
}

/* * * * * * * * * * * * * End - of - File * * * * * * * * * * * * * * */
