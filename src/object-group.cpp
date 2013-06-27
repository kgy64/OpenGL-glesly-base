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

void ObjectGroup::NextFrame(void)
{
 SYS_DEBUG_MEMBER(DM_GLESLY);

 ObjectListPtr p = GetObjectListPtr(); // The pointer is copied here to solve thread safety

 for (ObjectListIterator i = p->begin(); i != p->end(); ++i) {
    (*i)->NextFrame();
 }
}

bool ObjectGroup::MouseClick(float x, float y, int index, int count)
{
 SYS_DEBUG_MEMBER(DM_GLESLY);

 ObjectListPtr p = GetObjectListPtr(); // The pointer is copied here to solve thread safety

 for (ObjectListIterator i = p->begin(); i != p->end(); ++i) {
   if ((*i)->MouseClick(x, y, index, count)) {
       return true;
   }
 }

 return false;
}

/* * * * * * * * * * * * * End - of - File * * * * * * * * * * * * * * */
