/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * Project:     Glesly: my GLES-based rendering library
 * Purpose:     C++ wrapper for GL/ES - Smart Pointer for Objects
 * Author:      György Kövesdi (kgy@teledigit.eu)
 * Licence:     GPL (see file 'COPYING' in the project root for more details)
 * Comments:    
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "object-ptr.h"
#include "object.h"

using namespace Glesly;

ObjectPtr::ObjectPtr(ObjectBase * obj):
    boost::shared_ptr<ObjectBase>(obj)
{
 obj->mySelf = *this;
};

/* * * * * * * * * * * * * End - of - File * * * * * * * * * * * * * * */
