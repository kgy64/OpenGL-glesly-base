/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * Project:     Glesly: my GLES-based rendering library
 * Purpose:     C++ wrapper for GL/ES
 * Author:      György Kövesdi (kgy@teledigit.eu)
 * Licence:     GPL (see file 'COPYING' in the project root for more details)
 * Comments:    
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "target.h"

using namespace Glesly;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
 *                                                                                       *
 *       class Target:                                                                   *
 *                                                                                       *
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

Target::Target(void):
    myParent(NULL),
    myMouse(*this)
{
 SYS_DEBUG_MEMBER(DM_GLESLY);
}

Target::~Target()
{
 SYS_DEBUG_MEMBER(DM_GLESLY);
}

void Target::CloseRequest(void)
{
 SYS_DEBUG_MEMBER(DM_GLESLY);
 if (myParent) {
    myParent->CloseRequest();
 }
}

void Target::ShiftState(bool pressed)
{
 SYS_DEBUG_MEMBER(DM_GLESLY);
}

void Target::AltState(bool pressed)
{
 SYS_DEBUG_MEMBER(DM_GLESLY);
}

void Target::ControlState(bool pressed)
{
 SYS_DEBUG_MEMBER(DM_GLESLY);
}

/* * * * * * * * * * * * * End - of - File * * * * * * * * * * * * * * */
