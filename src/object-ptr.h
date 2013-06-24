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

#include <list>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

namespace Glesly
{
    class Object;

    typedef boost::weak_ptr<Object> ObjectWeak;

    class ObjectPtr: public boost::shared_ptr<Object>
    {
     public:
        ObjectPtr(Object * obj);

        inline ObjectPtr(boost::shared_ptr<Object> & obj):
            boost::shared_ptr<Object>(obj)
        {
        }

        inline ObjectPtr & operator=(boost::shared_ptr<Object> & obj)
        {
            static_cast<boost::shared_ptr<Object>&>(*this) = obj;
            return *this;
        }

    }; // ObjectPtr

    typedef std::list<ObjectPtr> Objects;

    typedef Objects::iterator ObjectListIterator;

    typedef boost::shared_ptr<Objects> ObjectListPtr;

} // namespace Glesly

#endif /* __GLESLY_SRC_OBJECT_PTR_H_INCLUDED__ */

/* * * * * * * * * * * * * End - of - File * * * * * * * * * * * * * * */
