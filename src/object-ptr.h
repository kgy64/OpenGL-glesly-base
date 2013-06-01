/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * Project:     Glesly: my GLES-based rendering library
 * Purpose:     C++ wrapper for GL/ES - Smart Pointer for Objects
 * Author:      György Kövesdi (kgy@teledigit.eu)
 * Licence:     GPL (see file 'COPYING' in the project root for more details)
 * Comments:    
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef __GLESLY_SRC_OBJECT_PTR_H_INCLUDED__
#define __GLESLY_SRC_OBJECT_PTR_H_INCLUDED__

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

namespace Glesly
{
    class Object;

    typedef boost::shared_ptr<Object> ObjectPtr;
    typedef boost::weak_ptr<Object> ObjectWeak;

} // namespace Glesly

#endif /* __GLESLY_SRC_OBJECT_PTR_H_INCLUDED__ */

/* * * * * * * * * * * * * End - of - File * * * * * * * * * * * * * * */
