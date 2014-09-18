/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * Project:     Glesly: my GLES-based rendering library
 * Purpose:     Base class for all Objects
 * Author:      György Kövesdi (kgy@teledigit.eu)
 * Licence:     GPL (see file 'COPYING' in the project root for more details)
 * Comments:    
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "object-base.h"

#include <glesly/render.h>

#include <Memory/Dump.h>

using namespace Glesly;

ObjectBase::ObjectBase(Glesly::ObjectListBase & base):
    myBase(base),
    myEnabled(true),
    myCallbackTimeLimit(0),
    toBeDeleted(false)
{
 SYS_DEBUG_MEMBER(DM_GLESLY);

 myCallbackTimeLimit = GetCallbackTimeLimit();
}

ObjectBase::~ObjectBase()
{
 SYS_DEBUG_MEMBER(DM_GLESLY);
}

/// Call this function to (re)initialize the OpenGL functionality
/*! \note   This function does not initialize it directly, but registers the object in
 *          the OpenGL Renderer to initialize it by the Render Thread later. It will be
 *          done at the next frame. */
void ObjectBase::ReinitGL(void)
{
 GetRenderer().InitGLObject(mySelf);
}

/// Calls \ref Glesly::ObjectBase::ObjectCallback::Execute() on demand
void ObjectBase::ExecuteCallback(const SYS::TimeDelay & frame_start_time)
{
 SYS_DEBUG_MEMBER(DM_GLESLY);

 // TODO Lock?
 if (myCallbacks.empty()) {
    return;
 }

 SYS::TimeElapsed elapsed(frame_start_time);
 SYS_DEBUG(DL_INFO2, "Time from frame start: " << elapsed.ToMillisecond() << " ms");

 // Note: in debug mode, this time can be significantly longer, so ignore it instead:
 if (!SYS_DEBUG_ON) {
    if (elapsed.ToMillisecond() > myCallbackTimeLimit) {
        myCallbackTimeLimit += 1 + myCallbackTimeLimit / 8; // Exponential increase
        return;
    }
    myCallbackTimeLimit = GetCallbackTimeLimit();
 }

 for (std::list<ObjectCallbackPtr>::iterator i = myCallbacks.begin(); i != myCallbacks.end(); ) {
    std::list<ObjectCallbackPtr>::iterator obj = i++;
    if ((*obj)->Execute(*this)) {
        myCallbacks.erase(obj);
    }
 }
}

int ObjectBase::GetCallbackTimeLimit(void) const
{
 return GetRenderer().GetCallbackTimeLimit();
}

/* * * * * * * * * * * * * End - of - File * * * * * * * * * * * * * * */
