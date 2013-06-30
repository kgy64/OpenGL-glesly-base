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

using namespace Glesly;

void ObjectBase::ExecuteCallback(const SYS::TimeDelay & frame_start_time)
{
 SYS_DEBUG_MEMBER(DM_GLESLY);

 ObjectCallbackPtr executor = myCallback;
 if (executor.get()) {
    SYS::TimeElapsed elapsed(frame_start_time);
    SYS_DEBUG(DL_INFO2, "Time from frame start: " << elapsed.ToMillisecond() << " ms");
    // Note: in debug mode, this time can be significantly longer, so ignore it instead:
    if (!SYS_DEBUG_ON) {
        if (elapsed.ToMillisecond() > 30) {
            return;
        }
    }
    myCallback.reset();
    executor->Execute(*this);
 }
}

/* * * * * * * * * * * * * End - of - File * * * * * * * * * * * * * * */
