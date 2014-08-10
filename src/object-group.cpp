/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * Project:     Glesly: my GLES-based rendering library
 * Purpose:     Group of Objects
 * Author:      György Kövesdi (kgy@teledigit.eu)
 * Licence:     GPL (see file 'COPYING' in the project root for more details)
 * Comments:    
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "object-group.h"

using namespace Glesly;

void ObjectGroup::initGL(void)
{
 SYS_DEBUG_MEMBER(DM_GLESLY);
 DEBUG_OUT("Called unnecessary function ObjectGroup::initGL()");
}

void ObjectGroup::DrawFrame(const SYS::TimeDelay & frame_start_time)
{
 SYS_DEBUG_MEMBER(DM_GLESLY);

 if (!IsEnabled()) {
    return;
 }

 ExecuteCallback(frame_start_time);

 ObjectListPtr p = GetObjectListPtr(); // The pointer is copied here to solve thread safety

 if (!p) {
    // If a stupid group has no objects at all:
    SYS_DEBUG(DL_INFO2, "Having no objects");
    return;
 }

 SYS_DEBUG(DL_INFO2, "Having " << p->size() << " objects");

 for (ObjectListIterator i = p->begin(); i != p->end(); ++i) {
    (*i)->DrawFrame(frame_start_time);
 }
}

bool ObjectGroup::MouseClick(float x, float y, int index, int count)
{
 SYS_DEBUG_MEMBER(DM_GLESLY);

 if (!IsEnabled()) {
    return false;
 }

 ObjectListPtr p = GetObjectListPtr(); // The pointer is copied here to solve thread safety

 SYS_DEBUG(DL_INFO2, "Having " << p->size() << " objects");

 for (ObjectListIterator i = p->begin(); i != p->end(); ++i) {
    if (!(*i)->IsEnabled()) {
        continue;
    }
    if ((*i)->MouseClick(x, y, index, count)) {
       return true;
    }
 }

 return false;
}

void ObjectGroup::KeyboardClick(UTF8::WChar key)
{
 SYS_DEBUG_MEMBER(DM_GLESLY);

 if (!IsEnabled()) {
    return;
 }

 ObjectListPtr p = GetObjectListPtr(); // The pointer is copied here to solve thread safety

 for (ObjectListIterator i = p->begin(); i != p->end(); ++i) {
    if (!(*i)->IsEnabled()) {
        continue;
    }
    (*i)->KeyboardClick(key);
 }
}

/* * * * * * * * * * * * * End - of - File * * * * * * * * * * * * * * */
