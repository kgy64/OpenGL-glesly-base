/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * Project:     
 * Purpose:     
 * Author:      
 * Licence:     GPL (see file 'COPYING' in the project root for more details)
 * Comments:    
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef __GLESLY_SRC_OBJECT_LIST_H_INCLUDED__
#define __GLESLY_SRC_OBJECT_LIST_H_INCLUDED__

#include <list>
#include <glesly/object-ptr.h>
#include <Threads/Mutex.h>
#include <Debug/Debug.h>

namespace Glesly
{
    typedef std::list<ObjectPtr> Objects;

    typedef Objects::iterator ObjectListIterator;

    class ObjectListBase
    {
     protected:
        typedef boost::shared_ptr<Objects> ObjectListPtr;

     public:
        class ObjectListInternal
        {
            friend class ObjectListBase;

         public:
            ~ObjectListInternal();

            void Add(ObjectPtr object);

            ObjectListIterator begin(void)
            {
                return myObjects->begin();
            }

            ObjectListIterator end(void)
            {
                return myObjects->end();
            }

         private:
            ObjectListInternal(ObjectListBase & parent);

            static Threads::Mutex myMutex;

            ObjectListBase & myParent;

            Threads::Lock myLock;

            ObjectListPtr myObjects;

        }; // class ObjectListInternal

        inline ObjectListInternal GetObjectList(void)
        {
            return ObjectListInternal(*this);
        }

        inline ObjectListPtr & GetObjectListPtr(void)
        {
            return myObjects;
        }

     protected:
        inline ObjectListBase(void):
            myObjects(new Objects)
        {
        }

     private:
        friend class ObjectListInternal;

        SYS_DEFINE_CLASS_NAME("Glesly::ObjectListBase");

        ObjectListPtr myObjects;

    }; // class ObjectListBase

    inline ObjectListBase::ObjectListInternal::ObjectListInternal(ObjectListBase & parent):
        myParent(parent),
        myLock(myMutex),
        myObjects(parent.myObjects)
    {
    }

    inline ObjectListBase::ObjectListInternal::~ObjectListInternal()
    {
        myParent.myObjects = myObjects;
    }

    inline void ObjectListBase::ObjectListInternal::Add(ObjectPtr object)
    {
        myObjects->push_front(object);
    }

    typedef ObjectListBase::ObjectListInternal ObjectList;

} // namespace Glesly

#endif /* __GLESLY_SRC_OBJECT_LIST_H_INCLUDED__ */

/* * * * * * * * * * * * * End - of - File * * * * * * * * * * * * * * */
