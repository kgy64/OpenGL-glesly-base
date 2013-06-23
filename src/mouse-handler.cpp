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

#include <glesly/target.h>

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
    SYS_DEBUG(DL_INFO3, "Mouse position=" << myX << "x" << myY);
 }
}

void MouseHandler::MouseClick(int index, int count)
{
 myParent.MouseClick(myX, myY, index, count);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
 *                                                                                       *
 *     class MouseButton:                                                                *
 *                                                                                       *
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

int MouseButton::ClickTime = 300;
int MouseButton::ReleaseTime = 300;

MouseButton::MouseButton(MouseHandler & parent, int index):
    myParent(parent),
    myIndex(index),
    myPressed(false),
    myPressCount(0)
{
 SYS_DEBUG_MEMBER(DM_GLESLY);
}

void MouseButton::State(bool pressed, const SYS::TimeDelay & time)
{
 SYS_DEBUG_MEMBER(DM_GLESLY);

 if (myPressed == pressed) {
    if (myPressCount) {
        Logic(time);
    }
 } else {
     myPressed = pressed;
     if (pressed) {
        Pressed();
     } else {
        Released(time - myPreviousTime);
     }
     myPreviousTime = time;
 }
}

void MouseButton::Logic(const SYS::TimeDelay & time)
{
 SYS_DEBUG_MEMBER(DM_GLESLY);

 SYS::TimeDelay elapsed = time - myPreviousTime;

 SYS_DEBUG(DL_INFO3, "elapsed time: " << elapsed.ToMillisecond() << " ms");

 if (myPressed) {
    if (elapsed.ToMillisecond() < ClickTime) {
        return;
    }
 } else {
    if (elapsed.ToMillisecond() < ReleaseTime) {
        return;
    }
 }

 myParent.MouseClick(myIndex, myPressCount);
 myPressCount = 0;
}

void MouseButton::Pressed(void)
{
 SYS_DEBUG_MEMBER(DM_GLESLY);

 SYS_DEBUG(DL_INFO3, "Mouse button " << myIndex << " is pressed");

 myParent.MouseClick(myIndex, 0);

 ++myPressCount;
}

void MouseButton::Released(const SYS::TimeDelay & time)
{
 SYS_DEBUG_MEMBER(DM_GLESLY);

 SYS_DEBUG(DL_INFO3, "Mouse button " << myIndex << " is released, time: " << time.ToMillisecond() << " ms");
}

/* * * * * * * * * * * * * End - of - File * * * * * * * * * * * * * * */
