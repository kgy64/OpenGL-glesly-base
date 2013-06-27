/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * Project:     Glesly: my GLES-based rendering library
 * Purpose:     Low-level base of all Objects
 * Author:      György Kövesdi (kgy@teledigit.eu)
 * Licence:     GPL (see file 'COPYING' in the project root for more details)
 * Comments:    
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef __GLESLY_SRC_OBJECT_LIST_BASE_H_INCLUDED__
#define __GLESLY_SRC_OBJECT_LIST_BASE_H_INCLUDED__

#include <glesly/object-ptr.h>
#include <Threads/Mutex.h>

namespace Glesly
{
    class ObjectListBase
    {
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

            ObjectListBase & myParent;

            Threads::Lock myLock;

            ObjectListPtr myObjects;

            ObjectListPtr myModifiedObjects;

        }; // class ObjectListInternal

        inline ObjectListInternal GetObjectList(void)
        {
            return ObjectListInternal(*this);
        }

        Threads::Mutex & GetObjectMutex(void)
        {
            return myObjectMutex;
        }

     protected:
        inline ObjectListBase(void)
        {
        }

        virtual ObjectListPtr & GetObjectListPtr(void)
        {
            return myObjects;
        }

     private:
        SYS_DEFINE_CLASS_NAME("Glesly::ObjectListBase");

        ObjectListPtr myObjects;

        Threads::Mutex myObjectMutex;

    }; // class ObjectListBase

    inline ObjectListBase::ObjectListInternal::ObjectListInternal(ObjectListBase & parent):
        myParent(parent),
        myLock(parent.GetObjectMutex()),
        myObjects(parent.GetObjectListPtr())
    {
    }

    inline ObjectListBase::ObjectListInternal::~ObjectListInternal()
    {
        if (myModifiedObjects.get()) {
            myParent.GetObjectListPtr() = myModifiedObjects;
        }
    }

    inline void ObjectListBase::ObjectListInternal::Add(ObjectPtr object)
    {
        if (!myModifiedObjects.get()) {
            if (myObjects.get()) {
                myModifiedObjects.reset(new Objects(*myObjects));
            } else {
                myModifiedObjects.reset(new Objects);
            }
        }
        myModifiedObjects->push_front(object);
    }

    typedef ObjectListBase::ObjectListInternal ObjectList;

} // namespace Glesly

#endif /* __GLESLY_SRC_OBJECT_LIST_BASE_H_INCLUDED__ */

/* * * * * * * * * * * * * End - of - File * * * * * * * * * * * * * * */
